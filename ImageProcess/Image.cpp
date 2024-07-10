#include "Image.h"
#include <fstream>
#include <iostream>
#include <chrono>
#include <sstream>
#include "DCT.h"

#define LOG true

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
        RGB.emplace_back(R, G, B);
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
    if (RGB.empty()){
        std::cerr << getTime() << "Image::saveImage() | Image have no RGB data!" << std::endl;
        exit(-1);
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
        PixelYCbCr pix;
        pix.fromRGB(RGB[p].R, RGB[p].G, RGB[p].B);
        YCbCr.emplace_back(pix);
    }

    RGB.clear();
    if (LOG) std::cout << getTime() << "Image::RGBtoYCbCr() | " << " RGB -> YCbCr created." << std::endl;
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
        PixelRGB pixel;
        pixel.fromYCbCr(YCbCr[p].Y, YCbCr[p].Cb, YCbCr[p].Cr);
        RGB.emplace_back(pixel);
    }

    YCbCr.clear();
    if (LOG) std::cout << getTime() << "Image::YCbCrtoRGB() | " << " YCbCr -> RGB created." << std::endl;
    return true;
}

///
/// Create Cb and Cr vector based on YCbCr
///
void Image::subsampleChroma() {
    if (YCbCr.empty()){
        std::cerr << getTime() << "Image::subsampleChroma() | Object have no YCbCr data." << std::endl;
        return;
    }

    Cb.resize(H*W/4);
    Cr.resize(H*W/4);
    int n = 0;
    for (int i = 0; i < H; i = i + 2){
        for (int j = 0; j < W; j = j + 2){
            int tmpCb = YCbCr[i * W + j].Cb + YCbCr[i * W + j + 1].Cb + YCbCr[(i + 1) * W + j].Cb + YCbCr[(i + 1) * W + j + 1].Cb;
            tmpCb /= 4;
            int tmpCr = YCbCr[i * W + j].Cr + YCbCr[i * W + j + 1].Cr + YCbCr[(i + 1) * W + j].Cr + YCbCr[(i + 1) * W + j + 1].Cr;
            tmpCr /= 4;
            Cb[n] = clipping(static_cast<float>(tmpCb));
            Cr[n++] = clipping(static_cast<float>(tmpCr));
        }
    }
    if (LOG) std::cout << getTime() << "Image::subsampleChroma() | " << " Successful subsample." << std::endl;
}

///
/// Write H*W Y component, and H*W/4 Cb and Cr components
///
void Image::writeSubsampledYCbCr(std::ofstream& file) {
    if (YCbCr.empty() || Cb.empty() || Cr.empty()){
        std::cerr << getTime() << "Image::subsampleChroma() | Object have no YCbCr, Cb or Cr data." << std::endl;
        return;
    }

    for (int i = 0; i < H*W; i++){
        file.put(YCbCr[i].Y);
    }
    for (int i = 0; i < H*W/4; i++){
        file.put(Cb[i]);
    }
    for (int i = 0; i < H*W/4; i++){
        file.put(Cr[i]);
    }
    Cb.clear();
    Cr.clear();
}

///
/// Read H*W Y component, and H*W/4 Cb and Cr components
///
void Image::readSubsampledYCbCr(std::ifstream& file){
    YCbCr.resize(H * W);
    Cb.resize(H * W / 4);
    Cr.resize(H * W / 4);

    for (int i = 0; i < H * W; i++){
        unsigned char Y;
        file.read(reinterpret_cast<char *>(&Y), 1);
        YCbCr[i] = PixelYCbCr(Y, 0, 0);
    }
    for (int i = 0; i < H * W / 4; i++){
        unsigned char cb;
        file.read(reinterpret_cast<char *>(&cb), 1);
        Cb[i] = cb;
    }
    for (int i = 0; i < H * W / 4; i++){
        unsigned char cr;
        file.read(reinterpret_cast<char *>(&cr), 1);
        Cr[i] = cr;
    }

    for (int i = 0; i < H; i += 2){
        for (int j = 0; j < W; j += 2){
            int index = (i / 2) * W/2 + (j / 2);
            unsigned char tmpCb = Cb[index];
            unsigned char tmpCr = Cr[index];

            YCbCr[i * W + j].Cb = tmpCb; YCbCr[i * W + j].Cr = tmpCr;
            YCbCr[i * W + j + 1].Cb = tmpCb; YCbCr[i * W + j + 1].Cr = tmpCr;
            YCbCr[(i + 1) * W + j].Cb = tmpCb; YCbCr[(i + 1) * W + j].Cr = tmpCr;
            YCbCr[(i + 1) * W + j + 1].Cb = tmpCb; YCbCr[(i + 1) * W + j + 1].Cr = tmpCr;
        }
    }


    Cb.clear();
    Cr.clear();
    YCbCrtoRGB();
}


Image::~Image(){
    RGB.clear();
    YCbCr.clear();
    Cb.clear();
    Cr.clear();
}

void Image::DCT(DCT_mode mode) {
    switch (mode){
        case DCT_mode::Y:{
            if (Y.empty()){
                std::cerr << getTime() << "Image::DCT() | Object have no Y data." << std::endl;
                return;
            }
            Y = DCTimage(Y, H, W, true);
        }
        case DCT_mode::Cb:{
            if (Cb.empty()){
                std::cerr << getTime() << "Image::DCT() | Object have no Cb data." << std::endl;
                return;
            }
            Cb = DCTimage(Cb, H, W, true);
        }
        case DCT_mode::Cr:{
            if (Cr.empty()){
                std::cerr << getTime() << "Image::DCT() | Object have no Cr data." << std::endl;
                return;
            }
            Cr = DCTimage(Cr, H, W, true);
        }
    }
}

void Image::unDCT(DCT_mode mode) {
    switch (mode){
        case DCT_mode::Y:{
            if (Y.empty()){
                std::cerr << getTime() << "Image::DCT() | Object have no Ydata." << std::endl;
                return;
            }
            Y = DCTimage(Y, H, W, false);
        }
        case DCT_mode::Cb:{
            if (Cb.empty()){
                std::cerr << getTime() << "Image::DCT() | Object have no Cb data." << std::endl;
                return;
            }
            Cb = DCTimage(Cb, H, W, false);
        }
        case DCT_mode::Cr:{
            if (Cr.empty()){
                std::cerr << getTime() << "Image::DCT() | Object have no Cr data." << std::endl;
                return;
            }
            Cr = DCTimage(Cr, H, W, false);
        }
    }
}

void Image::reworkYCbCrbyCbCr() {
    for (int i = 0; i < H; i += 2){
        for (int j = 0; j < W; j += 2){
            int index = (i / 2) * W/2 + (j / 2);
            unsigned char tmpCb = Cb[index];
            unsigned char tmpCr = Cr[index];

            YCbCr[i * W + j].Cb = tmpCb; YCbCr[i * W + j].Cr = tmpCr;
            YCbCr[i * W + j + 1].Cb = tmpCb; YCbCr[i * W + j + 1].Cr = tmpCr;
            YCbCr[(i + 1) * W + j].Cb = tmpCb; YCbCr[(i + 1) * W + j].Cr = tmpCr;
            YCbCr[(i + 1) * W + j + 1].Cb = tmpCb; YCbCr[(i + 1) * W + j + 1].Cr = tmpCr;
        }
    }
}

void Image::YCbCr_toChannels() {
    Y.resize(H*W);
    Cb.resize(H*W);
    Cr.resize(H*W);
    for (int i = 0; i < H*W; i++){
        Y[i] = YCbCr[i].Y;
        Cb[i] = YCbCr[i].Cb;
        Cr[i] = YCbCr[i].Cr;
    }
    YCbCr.clear();
}

void Image::YCbCrChannel_toYCbCr() {
    YCbCr.resize(H*W);
    for (int i = 0; i < H*W; i++){
        YCbCr[i] = PixelYCbCr(Y[i], Cb[i], Cr[i]);
    }
    Y.clear();
    Cb.clear();
    Cr.clear();
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
