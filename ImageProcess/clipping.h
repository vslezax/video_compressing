#ifndef VIDEO_COMPESS_CLIPPING_H
#define VIDEO_COMPESS_CLIPPING_H

inline unsigned char clipping(double value){
    if (value > 255) return 255;
    if (value < 0) return 0;
    return (unsigned char)value;
}

#endif //VIDEO_COMPESS_CLIPPING_H
