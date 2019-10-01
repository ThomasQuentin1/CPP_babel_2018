//
// Created by tom on 9/18/19.
//

#include <shared/packets.hpp>
#include <server/src/singleton/Database.hpp>
#include "BabelServer.hpp"
#include "shared/network/WindowsInit.hpp"

bool sigint_recived = false;

void sig_handler(int sig) {
    if (sig == SIGINT)
        sigint_recived = true;
}

inline bool file_exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

int main(int ac, char *av[]) try
{
    if (ac != 3) {
        std::cerr << "Usage : " << av[0] << " port path_to_database_file" << std::endl;
        return 84;
    }
    if (!file_exists(av[2])) {
        std::cerr << "Your database file don't exists" << std::endl;
        return 84;
    }
    Database::file_name = av[2];
	network::WindowsInit wsa_init;
    BabelServer server(std::stoi(av[1]));
    std::cout << "Starting babel server on port " << av[1] << std::endl;

    signal(SIGINT, sig_handler);

    while (!server.refresh() && !sigint_recived);

    std::cout << "Closing babel server, disconnecting all clients." << std::endl;

    return 0;
} catch (std::exception const &ex)
{
    std::cerr << "Fatal error : " << ex.what() << std::endl;
    return 84;
} catch (int ex) {
    std::cerr << "Fatal error : code " << ex << std::endl;
    return ex;
} catch (...) {
    std::cerr << "Fatal unknown error" << std::endl;
    return 84;
}
