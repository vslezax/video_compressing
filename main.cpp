#include <iostream>
#include "video.h"

int main() {
    //saveVideo("D:\\Sequence\\", Mode::SUBSAMPLING);
    //readVideo("D:\\Sequence\\untitled.video", "D:\\2");

    Image a;
    a.readImage("D:\\Sequence\\0001.bmp");
    a.RGBtoYCbCr();
    a.YCbCr_toChannels();
    a.DCT(DCT_mode::Y);
    a.unDCT(DCT_mode::Y);
    a.YCbCrChannel_toYCbCr();
    a.YCbCrtoRGB();
    a.saveImage("D:\\Sequence\\_dct.bmp");

 /*
    std::vector<double> a = {0, 1, 2, 3, 4};
    FastDctFft::transform(a);
    FastDctFft::inverseTransform(a);
    std::cout << std::endl;*/
}
