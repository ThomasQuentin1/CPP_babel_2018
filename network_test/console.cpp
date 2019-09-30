//
// Created by tom on 9/22/19.
//


#include <shared/network/ActionDyn.hpp>
#include <shared/network/TcpConnection.hpp>
#include <shared/exceptions/NetworkException.hpp>

void console() try
{
    network::TcpConnection sok("127.0.0.1", 9081);

    while (std::cin) {
        char op_nbr = 0;
        std::string argument;
        std::string op_nbr_str;
        std::cout << "Enter operation number :\t" << std::flush;
        std::getline(std::cin, op_nbr_str);
        op_nbr = std::stoi(op_nbr_str);
        std::cout << "Enter operation argument :\t" << std::flush;
        std::getline(std::cin, argument);


        size_t pos = 0;
        while ((pos = argument.find("\\n", pos)) != std::string::npos) {
            argument.replace(pos, 2, "\n");
            pos += 1;
        }
        sok.sendAction(static_cast<packet::operation>(op_nbr), argument);

        auto action = sok.recvAction();
        std::cout << "recived action : " << action.code() << " with argument : " << action.body() << std::endl;
    }
} catch (ex::NetworkException &ex) {
    std::cerr << std::endl << "error " << ex.what() << " at " << ex.where() << std::endl;
}