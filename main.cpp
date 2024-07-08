#include <iostream>
#include "video.h"

int main() {
    //saveVideo("D:\\Sequence\\", Mode::SUBSAMPLING);
    //readVideo("D:\\Sequence\\untitled.video", "D:\\2");

    Image a;
    a.readImage("D:\\Sequence\\0001.bmp");
    a.RGBtoYCbCr();
    a.subsampleChroma();
    std::ofstream file("D:\\Sequence\\0.bmp", std::ios::binary);
    a.writeSubsampledYCbCr(file);
    file.close();

    std::ifstream file1("D:\\Sequence\\0.bmp", std::ios::binary);
    a.readSubsampledYCbCr(file1);
    file1.close();
    a.saveImage("D:\\Sequence\\_1.bmp");
}
