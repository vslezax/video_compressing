#include <iostream>
#include "ImageProcess/Image.h"

int main() {
    Image a;
    a.readRGB("D:\\image.bmp");
    a.writeRGB("D:\\12.bmp");
}
