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
    std::vector<unsigned char> Cb;
    std::vector<unsigned char> Cr;
    int H;
    int W;
    unsigned char header[54];
    bool readImage(const std::string& path);
    void readRGB(std::ifstream& file);
    bool RGBtoYCbCr();
    bool YCbCrtoRGB();
    void writeRGB(std::ofstream& file);
    bool saveImage(const std::string& path);
    ~Image();

    void subsampleChroma();
    void writeSubsampledYCbCr(std::ofstream& file);
    void readSubsampledYCbCr(std::ifstream& file);
};

int readDWORD(const unsigned char* buffer, int pos);


#endif //VIDEO_COMPESS_IMAGE_H
