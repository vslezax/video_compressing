#include "Image.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

std::string getTime(){
    auto now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);
    std::tm *lt = std::localtime(&tt);
    std::stringstream ss;
    ss.fill('0');
    ss << std::setw(2) << lt->tm_hour << ":"
       << std::setw(2) << lt->tm_min << ":"
       << std::setw(2) << lt->tm_sec;
    return " | " + ss.str() + " | Image.cpp::Image";
}

bool Image::readRGB(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()){
        std::cerr << getTime() << "::readRGB() | " <<  path << " not opened" << std::endl;
        return false;
    }
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    H = infoHeader.biHeight;
    W = infoHeader.biWidth;

    if (infoHeader.biBitCount != 24) {
        std::cerr << "::readRGB() | " << path << " is not True Color 24 BMP!";
        exit(-1);
    }

    W = infoHeader.biWidth;
    H = infoHeader.biHeight;

    RGB.reserve(H*W);
    file.read(reinterpret_cast<char *>(RGB.data()), infoHeader.biSizeImage);

    file.close();
    std::cout << getTime() << "::readRGB() | " << path << " RGB read." << std::endl;
    return true;
}

bool Image::readYCbCr(const std::string &path) {
    return false;
}

bool Image::RGBtoYCbCr() {
    if (RGB.empty()){
        std::cerr << getTime() << "::RGBtoYCbCr() | Object have no RGB data." << std::endl;
        return false;
    }

    YCbCr.reserve(H*W);
    for (int p = 0; p < RGB.size(); p++){
        YCbCr.emplace_back(RGB[p].R, RGB[p].G, RGB[p].B);
    }

    RGB.clear();
    std::cout << getTime() << "::RGBtoYCbCr() | " << " RGB -> YCbCr created." << std::endl;
    return true;
}

bool Image::YCbCrtoRGB() {
    if (YCbCr.empty()){
        std::cerr << getTime() << "::YCbCrtoRGB() | Object have no YCbCr data." << std::endl;
        return false;
    }

    RGB.reserve(H*W);
    for (int p = 0; p < YCbCr.size(); p++){
        RGB.emplace_back(YCbCr[p].Y, YCbCr[p].Cb, YCbCr[p].Cr);
    }

    YCbCr.clear();
    std::cout << getTime() << "::YCbCrtoRGB() | " << " YCbCr -> RGB created." << std::endl;
    return true;
}

bool Image::writeRGB(const std::string &path) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()){
        std::cerr << getTime() << "::writeRGB() | " <<  path << " not opened" << std::endl;
        return false;
    }

    file.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    file.write(reinterpret_cast<char *>(RGB.data()), infoHeader.biSizeImage);

    file.close();
    std::cout << getTime() << "::writeRGB() | " << path << " RGB wrote." << std::endl;
    return true;
}
