#ifndef VIDEO_COMPESS_PIXELRGB_H
#define VIDEO_COMPESS_PIXELRGB_H

#include "clipping.h"

class PixelRGB {
public:
    unsigned char R;
    unsigned char G;
    unsigned char B;

    PixelRGB() : R(0), G(0), B(0) {}

    PixelRGB(unsigned char R, unsigned char G, unsigned char B){
        this->R = R;
        this->G = G;
        this->B = B;
    }
    void fromYCbCr(unsigned char Y, unsigned char Cb, unsigned char Cr){
        float y = static_cast<float>(Y);
        float cb = static_cast<float>(Cb) - 128.0f;
        float cr = static_cast<float>(Cr) - 128.0f;

        float r = y + 1.402f * cr;
        float g = y - 0.344136f * cb - 0.714136f * cr;
        float b = y + 1.772f * cb;

        R = clipping(r);
        G = clipping(g);
        B = clipping(b);
    }
};

#endif //VIDEO_COMPESS_PIXELRGB_H
