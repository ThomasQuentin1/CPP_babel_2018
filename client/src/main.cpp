//
// Created by fusee on 9/17/19.
//

#include "shared/headers.hpp"
#include "shared/network/WindowsInit.hpp"
#include "core.hpp"
#include <shared/exceptions/NetworkException.hpp>

int main(int ac, char *av[]) try
{
    network::WindowsInit window;

    Core core(ac, av);

    core.loop();
}
catch (ex::NetworkException & ex) {
	std::cerr << "Unable to connect to the server" << std::endl;
	return 84;
} catch (std::exception & ex) {
    std::cerr << "Fatal error : " << ex.what() << std::endl;
    return 84;
} catch (int ex) {
    std::cerr << "Fatal error : code " << ex << std::endl;
    return ex;
} catch (...)
{
    std::cerr << "Fatal unknown error" << std::endl;
    return 84;
}
