#ifndef VIDEO_COMPESS_PIXELRGB_H
#define VIDEO_COMPESS_PIXELRGB_H

#include "clipping.h"

class PixelRGB {
public:
    unsigned char R;
    unsigned char G;
    unsigned char B;
    PixelRGB(unsigned char Y, unsigned char Cb, unsigned char Cr){
        unsigned char R = clipping(Y - 0.714*(Cr - 128) - 0.334*(Cb - 128));
        unsigned char G = clipping(Y + 1.402*(Cr - 128));
        unsigned char B = clipping(Y + 1.772*(Cb - 128));
    }
};

#endif //VIDEO_COMPESS_PIXELRGB_H
