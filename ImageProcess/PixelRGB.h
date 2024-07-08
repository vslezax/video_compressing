#ifndef VIDEO_COMPESS_PIXELRGB_H
#define VIDEO_COMPESS_PIXELRGB_H

#include "clipping.h"

class PixelRGB {
public:
    unsigned char R;
    unsigned char G;
    unsigned char B;
    PixelRGB(){
        R = 0;
        G = 0;
        B = 0;
    }
    PixelRGB(unsigned char R, unsigned char G, unsigned char B){
        this->R = R;
        this->G = G;
        this->B = B;
    }
    void fromYCbCr(unsigned char Y, unsigned char Cb, unsigned char Cr){
        R = clipping(Y - 0.714*(Cr - 128) - 0.334*(Cb - 128));
        G = clipping(Y + 1.402*(Cr - 128));
        B = clipping(Y + 1.772*(Cb - 128));
    }
};

#endif //VIDEO_COMPESS_PIXELRGB_H
