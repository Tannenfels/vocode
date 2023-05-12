
#include <assert.h>
#include <string.h>
#if _WIN32
#include <winsock.h>
#include <malloc.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <alloca.h>
#endif
#include "encoder.h"

int vocode_f(TinyWav *tw, void *data, unsigned int len) {
    switch (tw->sampFmt) {
        case TW_INT16: {
            int16_t *interleaved_data = (int16_t *) alloca(tw->numChannels * len * sizeof(int16_t));
            size_t samples_read = fread(interleaved_data, sizeof(int16_t), tw->numChannels*len, tw->f);
            int valid_len = (int) samples_read / tw->numChannels;
            switch (tw->chanFmt) {
                case TW_INTERLEAVED: { // channel buffer is interleaved e.g. [LRLRLRLR]
                    for (int pos = 0; pos < tw->numChannels * valid_len; pos++) {
                        ((float *) data)[pos] = (float) interleaved_data[pos] / INT16_MAX;
                    }
                    return valid_len;
                }
                case TW_INLINE: { // channel buffer is inlined e.g. [LLLLRRRR]
                    for (int i = 0, pos = 0; i < tw->numChannels; i++) {
                        for (int j = i; j < valid_len * tw->numChannels; j += tw->numChannels, ++pos) {
                            ((float *) data)[pos] = (float) interleaved_data[j] / INT16_MAX;
                        }
                    }
                    return valid_len;
                }
                case TW_SPLIT: { // channel buffer is split e.g. [[LLLL],[RRRR]]
                    for (int i = 0, pos = 0; i < tw->numChannels; i++) {
                        for (int j = 0; j < valid_len; j++, ++pos) {
                            ((float **) data)[i][j] = (float) interleaved_data[j*tw->numChannels + i] / INT16_MAX;
                        }
                    }
                    return valid_len;
                }
                default: return 0;
            }
        }
        case TW_FLOAT32: {
            float *interleaved_data = (float *) alloca(tw->numChannels*len*sizeof(float));
            size_t samples_read = fread(interleaved_data, sizeof(float), tw->numChannels*len, tw->f);
            int valid_len = (int) samples_read / tw->numChannels;
            switch (tw->chanFmt) {
                case TW_INTERLEAVED: { // channel buffer is interleaved e.g. [LRLRLRLR]
                    memcpy(data, interleaved_data, tw->numChannels*valid_len*sizeof(float));
                    return valid_len;
                }
                case TW_INLINE: { // channel buffer is inlined e.g. [LLLLRRRR]
                    for (int i = 0, pos = 0; i < tw->numChannels; i++) {
                        for (int j = i; j < valid_len * tw->numChannels; j += tw->numChannels, ++pos) {
                            ((float *) data)[pos] = interleaved_data[j];
                        }
                    }
                    return valid_len;
                }
                case TW_SPLIT: { // channel buffer is split e.g. [[LLLL],[RRRR]]
                    for (int i = 0, pos = 0; i < tw->numChannels; i++) {
                        for (int j = 0; j < valid_len; j++, ++pos) {
                            ((float **) data)[i][j] = interleaved_data[j*tw->numChannels + i];
                        }
                    }
                    return valid_len;
                }
                default: return 0;
            }
        }
        default: return 0;
    }
}