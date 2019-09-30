//
// Created by tom on 9/18/19.
//

#include <shared/packets.hpp>
#include "BabelServer.hpp"
#include "shared/network/WindowsInit.hpp"

bool sigint_recived = false;

void sig_handler(int sig) {
    if (sig == SIGINT)
        sigint_recived = true;
}


int main(int ac, char *av[]) try
{
	network::WindowsInit wsa_init;
    BabelServer server(9081);

    std::cout << "Starting babel server on port 9081" << std::endl;

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