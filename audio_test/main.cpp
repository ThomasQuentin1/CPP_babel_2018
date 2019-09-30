/*
** EPITECH PROJECT, 2018
** Project
** File description:
** Class file
*/

//
// Created by tom on 9/22/19.
//

#include <client/src/PortAudioRecorder.hpp>
#include <client/src/PortAudioSpeaker.hpp>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <shared/exceptions/SoundException.hpp>
#include "portaudio.h"
/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (512)
#define NUM_SECONDS     (5)
#define NUM_CHANNELS    (2)
/* #define DITHER_FLAG     (paDitherOff) */
#define DITHER_FLAG     (0)
#define WRITE_TO_FILE   (0)
/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE  paFloat32
typedef float SAMPLE;
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 1
#define PA_SAMPLE_TYPE  paInt16
typedef short SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
typedef char SAMPLE;
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
typedef unsigned char SAMPLE;
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif

/* This routine will be called by the PortAudio engine when audio is needed.
** It may be called at interrupt level on some machines so don't do anything
** that could mess up the system like calling malloc() or free().
*/
static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,void *userData )
{
    paTestData *data = (paTestData*)userData;
    const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
    SAMPLE *wptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    long framesToCalc;
    long i;
    int finished;
    unsigned long framesLeft = data->maxFrameIndex - data->frameIndex;

    if ( framesLeft < framesPerBuffer ) {
        framesToCalc = framesLeft;
        finished = paComplete;
    }
    else {
        framesToCalc = framesPerBuffer;
        finished = paContinue;
    }
    if ( inputBuffer == NULL ) {
        for( i=0; i<framesToCalc; i++ ) {
            *wptr++ = SAMPLE_SILENCE;  /* left */
            if( NUM_CHANNELS == 2 )
                *wptr++ = SAMPLE_SILENCE;  /* right */
        }
    } else {
        for( i=0; i<framesToCalc; i++ ) {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 )
                *wptr++ = *rptr++;  /* right */
        }
    }
    data->frameIndex += framesToCalc;
    return finished;
}
/* This routine will be called by the PortAudio engine when audio is needed.
4 ** It may be called at interrupt level on some machines so don't do anything
5 ** that could mess up the system like calling malloc() or free().
6 */
static int playCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags, void *userData )
{
    paTestData *data = (paTestData*)userData;
    SAMPLE *rptr = &data->recordedSamples[data->frameIndex * NUM_CHANNELS];
    SAMPLE *wptr = (SAMPLE*)outputBuffer;
    unsigned int i;
    int finished;
    unsigned int framesLeft = data->maxFrameIndex - data->frameIndex;

    if( framesLeft < framesPerBuffer ) {
        /* final buffer... */
        for( i = 0; i < framesLeft; i++ ) {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 )
                *wptr++ = *rptr++;  /* right */
        }
        for(; i<framesPerBuffer; i++ ) {
            *wptr++ = 0;  /* left */
            if( NUM_CHANNELS == 2 )
                *wptr++ = 0;  /* right */
        }
        data->frameIndex += framesLeft;
        finished = paComplete;
    } else {
        for( i=0; i<framesPerBuffer; i++ ) {
            *wptr++ = *rptr++;  /* left */
            if( NUM_CHANNELS == 2 )
                *wptr++ = *rptr++;  /* right */
        }
        data->frameIndex += framesPerBuffer;
        finished = paContinue;
    }
    return finished;
}

/*******************************************************************/

void initInput(paTestData *data)
{
    PaStream*           stream;
    PaError             err;
    PaStreamParameters  inputParameters;
    int                 i;
    int                 numSamples;
    int                 numBytes;

    data->maxFrameIndex  = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
    data->frameIndex = 0;
    numSamples = data->maxFrameIndex * NUM_CHANNELS;
    numBytes = numSamples * sizeof(SAMPLE);
    data->recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
    if( data->recordedSamples == NULL ) {
        printf("Could not allocate record array.\n");
        return;
    }
    for( i=0; i<numSamples; i++ )
        data->recordedSamples[i] = 0;
    err = Pa_Initialize();
    if( err != paNoError )
        return;
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        return;
    }
    inputParameters.channelCount = 2;                    /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(&stream, &inputParameters,NULL,                  /* &outputParameters, */
                        SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff,      /* we won't output out of range samples so don't bother clipping them */
                        recordCallback, data );
    if( err != paNoError )
        return;
    err = Pa_StartStream( stream );
    if( err != paNoError )
        return;
    printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);
    while( ( err = Pa_IsStreamActive( stream ) ) == 1 ) {
        Pa_Sleep(1000);
        printf("index = %d\n", data->frameIndex ); fflush(stdout);
    }
    if( err < 0 )
        return;
    err = Pa_CloseStream( stream );
    if( err != paNoError )
        return;
}

void initOutput(paTestData *data)
{
    PaError             err;
    PaStreamParameters  outputParameters;
    PaStream*           stream;
    data->frameIndex = 0;
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        return;
    }
    outputParameters.channelCount = 2;                     /* stereo output */
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    printf("\n=== Now playing back. ===\n"); fflush(stdout);
    err = Pa_OpenStream( &stream, NULL, /* no input */ &outputParameters, SAMPLE_RATE,
                         FRAMES_PER_BUFFER, paClipOff,      /* we won't output out of range samples so don't bother clipping them */
                         playCallback, data );
    if( err != paNoError )
        return;
    if( stream ) {
        err = Pa_StartStream( stream );
        if( err != paNoError )
            return;
        printf("Waiting for playback to finish.\n"); fflush(stdout);
        while( ( err = Pa_IsStreamActive( stream ) ) == 1 ) Pa_Sleep(100);
        if( err < 0 )
            return;
        err = Pa_CloseStream( stream );
        if( err != paNoError )
            return;
        printf("Done.\n"); fflush(stdout);
    }
}

int maine(void) try {

/*
   paTestData data = {0, 0, 0};
   initInput(&data);
   initOutput(&data);
    return 0;*/

/*
    PortAudioRecorder recorder;
    PortAudioSpeaker speaker;
    paTestData *data;
    recorder.startRecording();
    speaker.startSpeaking();
    recorder.record();
    data = recorder.getData();
    speaker.addTestdata(data);
    speaker.playSound();*/
/*
    try {
        PortAudioRecorder recorder;
        PortAudioSpeaker speaker;
        paTestData *data;
        recorder.startRecording();
        speaker.startSpeaking();
        while ("tom") {
            if (recorder.isNewRecors()) {
                data = recorder.getData();
                initOutput(data);
                //speaker.addTestdata(data);
                //speaker.playSound();
            }
        }
        speaker.stopSpeaking();
        Pa_Terminate();
    } catch (ex::SoundException &ex){
        std::cout<<ex.what()<<" "<<ex.where();
    } catch (std::exception &ex)
    {
        std::cout<<ex.what();
    }*/

} catch (...) {
    return 0;
}

int maina(void) {
    PaStream*           stream;
    PaError             err;
    PaStreamParameters  inputParameters;
    int                 i;
    int                 numSamples;
    int                 numBytes;
    paTestData data;


    data.maxFrameIndex = NUM_SECONDS * SAMPLE_RATE; /* Record for a few seconds. */
    data.frameIndex = 0;
    numSamples = data.maxFrameIndex * NUM_CHANNELS;
    numBytes = numSamples * sizeof(SAMPLE);
    data.recordedSamples = (SAMPLE *) malloc( numBytes ); /* From now on, recordedSamples is initialised. */
    if( data.recordedSamples == NULL ) {
        printf("Could not allocate record array.\n");
        return 1;
    }
    for( i=0; i<numSamples; i++ )
        data.recordedSamples[i] = 0;

    err = Pa_Initialize();
    if( err != paNoError )
        return 1;
    inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    if (inputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default input device.\n");
        return 1;
    }
    inputParameters.channelCount = 2;                    /* stereo input */
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    /* Record some audio. -------------------------------------------- */
    err = Pa_OpenStream(&stream, &inputParameters,NULL,                  /* &outputParameters, */
                        SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff,      /* we won't output out of range samples so don't bother clipping them */
                        recordCallback, &data );
    if( err != paNoError )
        return 1;
    err = Pa_StartStream( stream );
    if( err != paNoError )
        return 1;
    printf("\n=== Now recording!! Please speak into the microphone. ===\n"); fflush(stdout);
    while( ( err = Pa_IsStreamActive( stream ) ) == 1 ) {
        Pa_Sleep(1000);
        printf("index = %d\n", data.frameIndex ); fflush(stdout);
    }
    if( err < 0 )
        return 1;
    err = Pa_CloseStream( stream );
    if( err != paNoError )
        return 1;

    PaStreamParameters  outputParameters;
    data.frameIndex = 0;
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");
        return 1;
    }
    outputParameters.channelCount = 2;                     /* stereo output */
    outputParameters.sampleFormat =  PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    printf("\n=== Now playing back. ===\n"); fflush(stdout);
    err = Pa_OpenStream( &stream, NULL, /* no input */ &outputParameters, SAMPLE_RATE,
                         FRAMES_PER_BUFFER, paClipOff,      /* we won't output out of range samples so don't bother clipping them */
                         playCallback, &data );
    if( err != paNoError )
        return 1;
    if( stream ) {
        err = Pa_StartStream( stream );
        if( err != paNoError )
            return 1;
        printf("Waiting for playback to finish.\n"); fflush(stdout);
        while( ( err = Pa_IsStreamActive( stream ) ) == 1 ) Pa_Sleep(100);
        if( err < 0 )
            return 1;
        err = Pa_CloseStream( stream );
        if( err != paNoError )
            return 1;
        printf("Done.\n"); fflush(stdout);
    }
    //initOutput(&data);
    return 0;
}

int main(int ac, char *av[]) {
    PortAudioRecorder recorder;
    PortAudioSpeaker speaker;

    while (true) {
		auto records = recorder.sendSound();
        for (auto const &data : records)
        speaker.addData(records);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "portaudio.h"
/* #define SAMPLE_RATE  (17932) // Test failure to open with this value. */
/* #define DITHER_FLAG     (paDitherOff)  */
#define DITHER_FLAG           (0)
/* Select sample format. */
#if 1
#define PA_SAMPLE_TYPE  paFloat32
#define SAMPLE_SIZE (4)
#define SAMPLE_SILENCE  (0.0f)
#define PRINTF_S_FORMAT "%.8f"
#elif 0
#define PA_SAMPLE_TYPE  paInt16
#define SAMPLE_SIZE (2)
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt24
#define SAMPLE_SIZE (3)
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#elif 0
#define PA_SAMPLE_TYPE  paInt8
#define SAMPLE_SIZE (1)
#define SAMPLE_SILENCE  (0)
#define PRINTF_S_FORMAT "%d"
#else
#define PA_SAMPLE_TYPE  paUInt8
#define SAMPLE_SIZE (1)
#define SAMPLE_SILENCE  (128)
#define PRINTF_S_FORMAT "%d"
#endif
/*******************************************************************/
int mainccc(void) // I/O BLOCKING
{
    PaStreamParameters inputParameters, outputParameters;
    PaStream *stream = NULL;
    PaError err;
    const PaDeviceInfo* inputInfo;
    const PaDeviceInfo* outputInfo;
    char *sampleBlock = NULL;
    int i;
    int numBytes;
    int numChannels;
     printf("patest_read_write_wire.c\n"); fflush(stdout);
     printf("sizeof(int) = %lu\n", sizeof(int)); fflush(stdout);
     printf("sizeof(long) = %lu\n", sizeof(long)); fflush(stdout);

     err = Pa_Initialize();
     if( err != paNoError ) goto error2;

     inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
     printf( "Input device # %d.\n", inputParameters.device );
     inputInfo = Pa_GetDeviceInfo( inputParameters.device );
     printf( "    Name: %s\n", inputInfo->name );
     printf( "      LL: %g s\n", inputInfo->defaultLowInputLatency );
     printf( "      HL: %g s\n", inputInfo->defaultHighInputLatency );

     outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
     printf( "Output device # %d.\n", outputParameters.device );
     outputInfo = Pa_GetDeviceInfo( outputParameters.device );
     printf( "   Name: %s\n", outputInfo->name );
     printf( "     LL: %g s\n", outputInfo->defaultLowOutputLatency );
     printf( "     HL: %g s\n", outputInfo->defaultHighOutputLatency );

    numChannels = inputInfo->maxInputChannels < outputInfo->maxOutputChannels
            ? inputInfo->maxInputChannels : outputInfo->maxOutputChannels;
    printf( "Num channels = %d.\n", numChannels );
    inputParameters.channelCount = numChannels;
    inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    inputParameters.suggestedLatency = inputInfo->defaultHighInputLatency ;
    inputParameters.hostApiSpecificStreamInfo = NULL;
    outputParameters.channelCount = numChannels;
    outputParameters.sampleFormat = PA_SAMPLE_TYPE;
    outputParameters.suggestedLatency = outputInfo->defaultHighOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;
    /* -- setup -- */
    err = Pa_OpenStream(
               &stream,
               &inputParameters,
               &outputParameters,
               SAMPLE_RATE,
              FRAMES_PER_BUFFER,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
               NULL, /* no callback, use blocking API */
              NULL ); /* no callback, so no callback userData */
    if( err != paNoError ) goto error2;
    numBytes = FRAMES_PER_BUFFER * numChannels * SAMPLE_SIZE ;
    sampleBlock = (char *) malloc( numBytes );
    if( sampleBlock == NULL )
    {
        printf("Could not allocate record array.\n");
        goto error1;
    }
    memset( sampleBlock, SAMPLE_SILENCE, numBytes );
    err = Pa_StartStream( stream );
    if( err != paNoError ) goto error1;
    printf("Wire on. Will run %d seconds.\n", NUM_SECONDS); fflush(stdout);
    for( i=0; i<(NUM_SECONDS*SAMPLE_RATE)/FRAMES_PER_BUFFER; ++i )
    {
        // You may get underruns or overruns if the output is not primed by PortAudio.
        err = Pa_WriteStream( stream, sampleBlock, FRAMES_PER_BUFFER );
        if( err ) goto xrun;
        err = Pa_ReadStream( stream, sampleBlock, FRAMES_PER_BUFFER );
        if( err ) goto xrun;
    }
    printf("Wire off.\n"); fflush(stdout);
    err = Pa_StopStream( stream );
    if( err != paNoError ) goto error1;
    free( sampleBlock );
    Pa_Terminate();
    return 0;
xrun:
    printf("err = %d\n", err); fflush(stdout);
    if( stream ) {
       Pa_AbortStream( stream );
       Pa_CloseStream( stream );
    }
    free( sampleBlock );
    Pa_Terminate();
    if( err & paInputOverflow )
       fprintf( stderr, "Input Overflow.\n" );
    if( err & paOutputUnderflow )
       fprintf( stderr, "Output Underflow.\n" );
    return -2;
error1:
    free( sampleBlock );
error2:
    if( stream ) {
       Pa_AbortStream( stream );
       Pa_CloseStream( stream );
    }
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return -1;
}