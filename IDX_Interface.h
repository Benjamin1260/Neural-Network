#include <iostream>
#include <vector>
#include <cstddef>
#include <cmath>
#include <fstream>
#include <cstdint>
#include <utility>

int binaryToInt(std::vector<std::byte> inBinary) {
    int sum = 0;
    for (int i = 0; i < inBinary.size(); i++) {
        std::byte currentByte = inBinary[i];
        sum += std::pow(256, (inBinary.size()-i-1)) * std::to_integer<int>(currentByte);
    }
    return sum;
};

std::vector<int> findSize(std::ifstream *iFile, int dim) {
    iFile->seekg(4); //go to beginning past magic number
    std::vector<int> sizeArr(dim);
    for (int i = 0; i < dim; i++) {
        std::vector<std::byte> byteCache(4);
        for (int j = 0; j < 4; j++) {
            byteCache[j] = std::byte(iFile->get());
        }
        sizeArr[i] = binaryToInt(byteCache);
        //std::cout << "Size: " << sizeArr[i] << std::endl;
    }
    return sizeArr;
};

struct inputFilesStruct {
    std::ifstream *inputImages;
    std::ifstream *inputValues;
    std::vector<int> iiSize;
    std::vector<int> ivSize;

    inputFilesStruct(std::ifstream *iiiFile, std::ifstream *iivFile) : inputImages(iiiFile), inputValues(iivFile) {
        //initializing sizes
        iiSize = std::vector<int>(2);
        std::vector<int> sizeArr = findSize(inputImages, 3);
        iiSize[0] = sizeArr[0];
        iiSize[1] = sizeArr[1] * sizeArr[2];

        ivSize = std::vector<int>(1);
        ivSize[0] = findSize(inputValues, 1)[0];
    };
};

using pairArrArr = std::pair <std::vector<uint8_t>, std::vector<uint8_t>>;
pairArrArr nextImageLabelPair(inputFilesStruct inputFiles, int pos) {
    std::vector<uint8_t> outImArr(inputFiles.iiSize[1]);
    for (int i = 0; i < inputFiles.iiSize[1]; i++) {
        (*inputFiles.inputImages) >> outImArr[i];
    }
    
    std::vector<uint8_t> outValArr(10, 0);
    uint8_t value;
    (*inputFiles.inputValues) >> value;
    outValArr[value] = 1;

    pairArrArr outPair(outImArr, outValArr);
    return outPair;
}

//assumes magic number: (inputImages == 0083 && inputValues == 0081)
std::vector<pairArrArr> *MNIST_init(std::ifstream *inputImagesFile, std::ifstream *inputValuesFile) {
    if (sizeof(char) != 1) {std::cout << "charSize != 1"; exit;} //fstream outputs char, gets used as if 1 byte, if char extrected != 1 byte, code will be off

    inputFilesStruct inputFiles(inputImagesFile, inputValuesFile);

    //create output array and assign value to every image
    std::vector<pairArrArr> *outArrPair = new std::vector<pairArrArr>(inputFiles.iiSize[0]);
    for (int i = 0; i < outArrPair->size(); i++) {
        (*outArrPair)[i] = nextImageLabelPair(inputFiles, i);
    }

    return outArrPair; //final out
}