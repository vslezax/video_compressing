#ifndef VIDEO_COMPESS_DCT_H
#define VIDEO_COMPESS_DCT_H

#include <vector>
#include <iostream>
#include <cmath>
#include "clipping.h"
#include "FastDCT/NaiveDct.hpp"

inline int blockSize = 8;

inline std::vector<unsigned char> DCTimage(const std::vector<unsigned char>& pixels, int H, int W, bool forward){
    std::vector<std::vector<unsigned char>> data(H, std::vector<unsigned char>(W, 0));
    for (int i = 0; i < pixels.size(); i++){
        data[(int)(i / W)][i % W] = pixels[i];
    }
    std::vector<std::vector<unsigned char>> result = data;

    // Tiling entire image
    for (int i = 0; i < H; i += blockSize){
        std::cout << "Working on tile [" << i << ", " << 0 << "] x [" << i + blockSize << ", " << W << "]..." << std::endl;
        for (int j = 0; j < W; j += blockSize){
            std::vector<double> block(blockSize*blockSize, 0);
            // Forming block
            for (int k = 0; k < blockSize; k++){
                for (int l = 0; l < blockSize; l++){
                    block[k * blockSize + l] = data.at(i + k).at(j + l);
                }
            }

            // DCT
            block = forward? NaiveDct::transform(block): NaiveDct::inverseTransform(block);

            // Forming outputData
            for (int k = 0; k < blockSize; k++){
                for (int l = 0; l < blockSize; l++){
                    result.at(i + k).at(j + l) = block[k * blockSize + l];
                }
            }
        }
    }
    std::vector<unsigned char> output(H*W, 0);
    for (int i = 0; i < H; i++){
        for (int j = 0; j < W; j++){
            output[i * W + j] = result[i][j];
        }
    }
    return output;
}

#endif //VIDEO_COMPESS_DCT_H
