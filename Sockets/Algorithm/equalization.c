#include "equalization.h"

unsigned char* Apply_Equalization(unsigned char* data, int width, int height, int channels){

    int histogram[3][256] = {{0}};
    for (int i = 0; i < width * height * channels; i += channels) {
        for (int channel = 0; channel < channels; channel++) {
            unsigned char intensity = data[i + channel];
            histogram[channel][intensity]++;
        }
    }

    unsigned char equalizationVals[3][256];
    for (int channel = 0; channel < channels; channel++) {
        int cdf[256] = {0};
        cdf[0] = histogram[channel][0];
        for (int i = 1; i < 256; i++) {
            cdf[i] = cdf[i - 1] + histogram[channel][i];
        }

        for (int i = 0; i < 256; i++) {
            equalizationVals[channel][i] = (unsigned char)((cdf[i] / (float)(width * height)) * 255);
        }
    }


    for (int i = 0; i < width * height * channels; i += channels) {
        for (int channel = 0; channel < channels; channel++) {
            unsigned char intensity = data[i + channel];
            unsigned char newIntensity = equalizationVals[channel][intensity];
            data[i + channel] = newIntensity;
        }
    }

    return data;
}

