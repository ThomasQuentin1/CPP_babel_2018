/*
** EPITECH PROJECT, 2018
** Project
** File description:
** Class file
*/

#include <client/src/PortAudio.hpp>

int main()
{
    PortAudio audio;

    while (true) {
        std::cout << "loop" << std::endl;
        auto packet = audio.receiveSound();
        if (packet) {
            std::cout << "data" << std::endl;
            audio.sendSound(packet);
        }
    }
    return 0;
}