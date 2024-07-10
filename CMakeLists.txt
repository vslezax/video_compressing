cmake_minimum_required(VERSION 3.27)
project(video_compess)

set(CMAKE_CXX_STANDARD 17)

add_executable(video_compess main.cpp
        ImageProcess/Image.cpp
        ImageProcess/Image.h
        ImageProcess/PixelRGB.h
        ImageProcess/PixelYCbCr.h
        ImageProcess/clipping.h
        video.h
        ImageProcess/DCT.h
        ImageProcess/FastDCT/FftRealPair.hpp
        ImageProcess/FastDCT/FftRealPair.cpp
        ImageProcess/FastDCT/FastDctLee.hpp
        ImageProcess/FastDCT/FastDctLee.cpp
        ImageProcess/FastDCT/FastDctFft.cpp
        ImageProcess/FastDCT/NaiveDct.hpp
        ImageProcess/FastDCT/NaiveDct.cpp
)
