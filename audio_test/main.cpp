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
        auto packet = audio.receiveSound();
        if (packet)
            audio.sendSound(packet);
    }
    return 0;
}