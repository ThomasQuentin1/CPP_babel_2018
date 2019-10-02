#include "core.hpp"

Core::Core(int argc, char *argv[])
{
 //   this->gui = std::make_unique<Gui>(argc, argv);
//    std::cerr << gui.get() << std::endl;
    gui = new Gui(argc, argv);
}

void Core::loop()
{
    int whilecond = 0;
    bool check = false;
    std::string names = "";
    commEnum_t ret = commEnum_t::NONE;
    commEnum_t argToSendToRefresh = commEnum_t::NONE;
    int i = 0;

    while (whilecond == 0) {
        comm.refresh();
        if (comm.incommingCall() != "") {
            names = comm.incommingCall();
            argToSendToRefresh = commEnum_t::INCOMMING_CALL;
            std::cout << names << " is calling me" << std::endl;
        }
        ret = gui->refresh(argToSendToRefresh, names);
        argToSendToRefresh = commEnum_t::NONE;
        names = "";
        // Ce qui est envoyé au serveur en fonction du return du refresh
        switch (ret) {
            case commEnum_t::CALL:
                comm.call(gui->getArgument());
                break;
            case commEnum_t::ACCEPT_CALL:
                comm.acceptCall();
                break;
            case commEnum_t::DECLINE_CALL:
                comm.declineCall();
                break;
            case commEnum_t::TRY_LOGIN:
                std::cout << "login" << std::endl;
                check = comm.login(gui->getArgument());
                if (check == true) {
                    argToSendToRefresh = commEnum_t::CONNECTION_SUCCESS;
                    isLogged = true;
                } else {
                    argToSendToRefresh = commEnum_t::CONNECTION_FAILED;
                }
                break;
            case commEnum_t::TRY_REGISTER:
                std::cout << "register" << std::endl;
                check = comm.signUp(gui->getArgument());
                if (check == true) {
                    argToSendToRefresh = commEnum_t::CONNECTION_SUCCESS;
                } else {
                    argToSendToRefresh = commEnum_t::CONNECTION_FAILED;
                }
                break;
            case commEnum_t::LOGOUT:
                isLogged = false;
                break;
            default:
                argToSendToRefresh = commEnum_t::NONE;
                break;
        }

        if (this->comm.isCommunicating()) {
            argToSendToRefresh = commEnum_t::IS_COMMUNICATING;
            if (!this->audio)
                this->audio = std::make_unique<PortAudio>();

            auto inputsound = this->audio->receiveSound();
            if (inputsound)
                this->comm.sendSound(inputsound);

            auto outputsound = this->comm.reciveSound();
			if (outputsound) {
				this->audio->sendSound(outputsound);
				std::cout << "Sending sound to speaker" << std::endl;
			}
        } else {
            if (comm.isCalling() == false && argToSendToRefresh == commEnum_t::NONE) {
                argToSendToRefresh = commEnum_t::CALL_DECLINED;
            }
            this->audio = nullptr;
        }

        i++;
        if (i >= 1000000 && isLogged) {
            i = 0;
            gui->setOnlineContact(stringToVector(comm.getOnlineUsers()));
        }
    }
    delete (gui);
}

std::vector<std::string> Core::stringToVector(std::string line)
{
    if (line.empty())
        return (std::vector<std::string>());
    std::vector<std::string> list;

    std::size_t current, previous = 0;
    current = line.find("\n");
    while (current != std::string::npos) {
        list.push_back(line.substr(previous, current - previous));
        previous = current + 1;
        current = line.find("\n", previous);
    }
    list.push_back(line.substr(previous, current - previous));
    return(list);
}
