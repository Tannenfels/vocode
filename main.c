#include <stdio.h>
#include "fft/fft.h"
#include "tinywav/tinywav.h"

#define NUM_CHANNELS 2
#define SAMPLE_RATE 48000
#define BLOCK_SIZE 480

TinyWav tw;
uint32_t get_bitrate(TinyWav*);

int main(int argc,  char** argv)
{
    if (argc < 2) {
        printf("%s", "No path to a file was passed!");
        return 1;
    }

    tinywav_open_read(&tw, argv[1], TW_SPLIT);

    printf("%u", get_bitrate(&tw));
    for (int i = 0; i < 100; i++) {
        // samples are always provided in float32 format,
        // regardless of file sample format
        float samples[NUM_CHANNELS * BLOCK_SIZE];

        // Split buffer requires pointers to channel buffers
        float* samplePtrs[NUM_CHANNELS];
        for (int j = 0; j < NUM_CHANNELS; ++j) {
            samplePtrs[j] = samples + j*BLOCK_SIZE;
        }

        tinywav_read_f(&tw, samplePtrs, BLOCK_SIZE);
    }

    tinywav_close_read(&tw);

    /*cplx buf[] = {1, 1, 1, 1, 0, 0, 0, 0};

    fft(buf, 8);*/


    return 0;
}

uint32_t get_bitrate(TinyWav* tw_ptr) {
    return tw_ptr->h.ByteRate;
}


