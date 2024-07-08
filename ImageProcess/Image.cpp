#include "Image.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>

int readDWORD(const unsigned char* buffer, int pos);
inline std::string getTime();

///
/// Reading RGB pixels until EOF
///
void Image::readRGB(std::ifstream& file) {
    RGB.clear();
    for (int i = 0; i < H*W; i++){
        unsigned char R, G, B;
        file.read(reinterpret_cast<char *>(&B), 1);
        file.read(reinterpret_cast<char *>(&G), 1);
        file.read(reinterpret_cast<char *>(&R), 1);
        RGB.emplace_back(B, G, R);
    }
}

///
/// Reading entire file: HEADER + RGB PIXELS
///
bool Image::readImage(const std::string &path) {
    RGB.clear();
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()){
        std::cerr << getTime() << "Image::readImage() | " <<  path << " not opened" << std::endl;
        return false;
    }

    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    W = readDWORD(header, 18);
    H = readDWORD(header, 22);
    file.seekg(54);
    readRGB(file);

    file.close();
    std::cout << getTime() << "Image::readImage() | " << path << " RGB read, pixels: " << RGB.size() << std::endl;
    return true;
}

///
/// Writing RGB pixels
///
void Image::writeRGB(std::ofstream& file) {
    for (auto pixel: RGB){
        file.put(pixel.B);
        file.put(pixel.G);
        file.put(pixel.R);
    }
}

///
/// Writing entire file: HEADER + RGB PIXELS
///
bool Image::saveImage(const std::string &path) {
    std::ofstream file(path, std::ios::binary);
    if (!file.is_open()){
        std::cerr << getTime() << "Image::saveImage() | " <<  path << " not opened" << std::endl;
        return false;
    }

    file.write(reinterpret_cast<char *>(&header), sizeof(header));
    writeRGB(file);

    file.close();
    std::cout << getTime() << "Image::writeRGB() | " << path << " RGB wrote." << std::endl;
    return true;
}





///
/// Converting RGB to YCbCr
///
bool Image::RGBtoYCbCr() {
    if (RGB.empty()){
        std::cerr << getTime() << "Image::RGBtoYCbCr() | Object have no RGB data." << std::endl;
        return false;
    }

    YCbCr.reserve(H*W);
    for (int p = 0; p < RGB.size(); p++){
        YCbCr.emplace_back(RGB[p].R, RGB[p].G, RGB[p].B);
    }

    RGB.clear();
    std::cout << getTime() << "Image::RGBtoYCbCr() | " << " RGB -> YCbCr created." << std::endl;
    return true;
}

///
/// Converting YCbCr to RGB
///
bool Image::YCbCrtoRGB() {
    if (YCbCr.empty()){
        std::cerr << getTime() << "Image::YCbCrtoRGB() | Object have no YCbCr data." << std::endl;
        return false;
    }

    RGB.reserve(H*W);
    for (int p = 0; p < YCbCr.size(); p++){
        RGB.emplace_back(YCbCr[p].Y, YCbCr[p].Cb, YCbCr[p].Cr);
    }

    YCbCr.clear();
    std::cout << getTime() << "Image::YCbCrtoRGB() | " << " YCbCr -> RGB created." << std::endl;
    return true;
}

Image::~Image(){
    RGB.clear();
    YCbCr.clear();
}

inline std::string getTime(){
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    std::stringstream ss;
    ss << (now->tm_year + 1900) << '-'
       << (now->tm_mon + 1) << '-'
       << now->tm_mday << " ";
    ss << now->tm_hour << ":"
       << now->tm_min << ":"
       << now->tm_sec << " | ";
    return ss.str();
}

int readDWORD(const unsigned char* buffer, int pos){
    int value = static_cast<int>(buffer[pos]) |
                (static_cast<int>(buffer[pos+1]) << 8) |
                (static_cast<int>(buffer[pos+2]) << 16) |
                (static_cast<int>(buffer[pos+3]) << 24);
    return value;
}
