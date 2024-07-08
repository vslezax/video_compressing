#ifndef VIDEO_COMPESS_PIXELYCBCR_H
#define VIDEO_COMPESS_PIXELYCBCR_H

#include "clipping.h"

class PixelYCbCr {
public:
    unsigned char Y;
    unsigned char Cb;
    unsigned char Cr;

    PixelYCbCr() : Y(0), Cb(0), Cr(0) {}

    PixelYCbCr(unsigned char Y, unsigned char Cb, unsigned char Cr) : Y(Y), Cb(Cb), Cr(Cr) {}

    void fromRGB(unsigned char R, unsigned char G, unsigned char B) {
        float r = static_cast<float>(R);
        float g = static_cast<float>(G);
        float b = static_cast<float>(B);

        float y = 0.299f * r + 0.587f * g + 0.114f * b;
        float cb = -0.168736f * r - 0.331264f * g + 0.5f * b + 128.0f;
        float cr = 0.5f * r - 0.418688f * g - 0.081312f * b + 128.0f;

        Y = clipping(y);
        Cb = clipping(cb);
        Cr = clipping(cr);
    }
};

#endif //VIDEO_COMPESS_PIXELYCBCR_H
