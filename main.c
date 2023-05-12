#include <stdio.h>
#include <string.h>
#include "fft/fft.h"
#include "tinywav/tinywav.h"

#define NUM_CHANNELS 2
#define SAMPLE_RATE 48000

TinyWav tw;
uint32_t get_bitrate(TinyWav*);
char output[32] = "out.wav";

int main(int argc,  char** argv)
{
    if (argc < 2) {
        printf("%s", "No path to a file was passed!");
        return 1;
    } else if (argc >= 3) {
        if (strlen(argv[2]) >= 32) {
            printf("%s", "Output file name too long!");
            return 1;
        }
        strcpy(output, argv[2]);
    }

    tinywav_open_read(&tw, argv[1], TW_SPLIT);

    unsigned int block_size = get_bitrate(&tw) / 20;
    printf("%u", get_bitrate(&tw));
    for (int i = 0; i < 100; i++) {
        // samples are always provided in float32 format,
        // regardless of file sample format
        float samples[NUM_CHANNELS * block_size];

        // Split buffer requires pointers to channel buffers
        float* samplePtrs[NUM_CHANNELS];
        for (int j = 0; j < NUM_CHANNELS; ++j) {
            samplePtrs[j] = samples + j * block_size;
        }

        tinywav_read_f(&tw, samplePtrs, block_size);

        for (int j = 0; j < NUM_CHANNELS; ++j) {
            cplx buf[] = {samplePtrs[j]};
            fft(buf, 8);
        }




        for (int k = 0; k < NUM_CHANNELS; k++) {

        }

    }

    tinywav_close_read(&tw);




    return 0;
}

uint32_t get_bitrate(TinyWav* tw_ptr) {
    return tw_ptr->h.ByteRate;
}


