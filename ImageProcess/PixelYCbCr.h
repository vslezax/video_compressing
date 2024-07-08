#ifndef VIDEO_COMPESS_PIXELYCBCR_H
#define VIDEO_COMPESS_PIXELYCBCR_H

#include "clipping.h"

class PixelYCbCr {
public:
    unsigned char Y;
    unsigned char Cb;
    unsigned char Cr;
    PixelYCbCr(unsigned char R, unsigned char G, unsigned char B){
        Y = clipping(0.299*R + 0.587*G + 0.114*B);
        Cb = clipping(0.5643*(B - Y) + 128);
        Cr = clipping(0.7132*(R - Y) + 128);
    }
};

#endif //VIDEO_COMPESS_PIXELYCBCR_H
