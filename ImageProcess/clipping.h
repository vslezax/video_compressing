#ifndef VIDEO_COMPESS_CLIPPING_H
#define VIDEO_COMPESS_CLIPPING_H

inline unsigned char clipping(float value) {
    if (value < 0.0f) return 0;
    if (value > 255.0f) return 255;
    return static_cast<unsigned char>(value);
}

#endif //VIDEO_COMPESS_CLIPPING_H
