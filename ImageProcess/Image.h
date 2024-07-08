#ifndef VIDEO_COMPESS_IMAGE_H
#define VIDEO_COMPESS_IMAGE_H

#include <vector>
#include <string>
#include "PixelRGB.h"
#include "PixelYCbCr.h"
#include "Windows.h"

class Image {
public:
    std::vector<PixelRGB> RGB;
    std::vector<PixelYCbCr> YCbCr;
    short H;
    short W;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    bool readRGB(const std::string& path);
    bool readYCbCr(const std::string& path);
    bool RGBtoYCbCr();
    bool YCbCrtoRGB();
    bool writeRGB(const std::string& path);
};


#endif //VIDEO_COMPESS_IMAGE_H
