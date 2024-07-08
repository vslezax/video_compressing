#ifndef VIDEO_COMPESS_VIDEO_H
#define VIDEO_COMPESS_VIDEO_H

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <fstream>
#include "ImageProcess/Image.h"

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

std::vector<std::string> listFiles(const std::string& path){
    std::vector<std::string> files;

    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {

        for (const auto& entry: std::filesystem::directory_iterator(path)) {
            if (entry.path().extension() == ".bmp") files.push_back(entry.path().string());
        }
    }
    else{
        std::cerr << getTime() << "video.h::listFiles() | " << path << "not exist or is not directory." << std::endl;
    }

    return files;
}

void writeFirstFrame(std::ofstream& video, const std::string& file){
    // Reading first frame and saving headers
    Image firstFrame;
    firstFrame.readImage(file);
    video.write(reinterpret_cast<char*>(&firstFrame.header), sizeof(firstFrame.header));
    video.write(reinterpret_cast<char*>(&firstFrame.RGB), sizeof(firstFrame.RGB));
}

void saveUncompressedVideo(const std::string& path){
    std::cout << getTime() << "Working with directory '" + path + "'." << std::endl;
    auto files = listFiles(path);
    if (files.empty()){
        std::cerr << "Unable to read images." << std::endl;
        exit(-1);
    }

    std::ofstream video(path + "untitled.video", std::ios::binary);
    if (!video.is_open()){
        std::cerr << "Unable to create video file." << std::endl;
        exit(-1);
    }
    video.put(0); // mode = 0 (uncompressed)

    Image frame;
    frame.readImage(files[0]);
    int H = frame.H;
    int W = frame.W;
    video.write(reinterpret_cast<char*>(&frame.header), sizeof(frame.header));
    frame.writeRGB(video);
    for (int i = 1; i < files.size(); i++){
        frame.readImage(files[i]);
        if (H == frame.H && W == frame.W) frame.writeRGB(video);
        else std::cout << getTime() << files[i] << " skipped because of another resolution." << std::endl;
    }
    video.close();

    std::cout << path + "untitled.video created. Frames = " << files.size() << std::endl;
}

void readVideo(const std::string& videoPath, const std::string& output){
    std::ifstream video(videoPath, std::ios::binary);
    if (!video.is_open()){
        std::cerr << "Unable to create video file." << std::endl;
        exit(-1);
    }

    int mode;
    video.read(reinterpret_cast<char*>(&mode), 1);

    Image frame;
    video.read(reinterpret_cast<char*>(&frame.header), sizeof(frame.header));
    frame.W = readDWORD(frame.header, 18);
    frame.H = readDWORD(frame.header, 22);
    int i = 1;
    while (!video.eof()){
        frame.readRGB(video);
        if (video.eof()) break;
        std::stringstream ss;
        ss << output << "\\" << i << ".bmp";
        frame.saveImage(ss.str());
        std::cout << getTime() << " Exported " << i++ << ".bmp" << std::endl;
    }

    std::cout << output + " images created. Frames = " << i << std::endl;
}

#endif //VIDEO_COMPESS_VIDEO_H
