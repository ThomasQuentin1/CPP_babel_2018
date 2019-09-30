/*
** EPITECH PROJECT, 2018
** Project
** File description:
** Class file
*/

#ifndef INC_6A76468E862945AF88E74D0E2165000D
    #define INC_6A76468E862945AF88E74D0E2165000D


class IRecorder;

struct paTestData
{
    int frameIndex;  /* Index into sample array. */
    int maxFrameIndex;
    float *recordedSamples;
};

struct UserData
{
    paTestData *data;
    IRecorder *recorder;
};

namespace audioConfig {
    static const int channels = 2;
    static const int frameSize = 960;
    static const int sample_rate = 44100;
    static const int frames_per_buffer = 512;
    static const int max_packet_size = (3*1276);
}

#endif //INC_6A76468E862945AF88E74D0E2165000D
