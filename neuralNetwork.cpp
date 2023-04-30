#include <iostream>
#include <cstdlib>
#include <chrono>
#include "NNclass&funct.h"

int main() {
    //initializing seed for random number generation
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::srand(seed);

    //initializing the neural network
    int inSize = 10, midSize = 28*28, layer = 2, outSize = 10; //data can be changed later to use std::cin

    //temp inputData array, needs to be changed later to take input from MNIST database
    // <NEXT> read the file with input formats and create an array of matrices with the image data + array of desired output ==> struct or pair?
    custArr<float> inData(inSize);
    for (int i = 0; i < inSize; i++) {
        inData[i] = rand() % 2;
        //std::cout << "input " << i << " = " << inData[i] << std::endl;
    }

    neuralNetwork myNetwork(inSize, midSize, layer, outSize);
    custArr<float> output = myNetwork.run(inData);

    std::cout << "OUTPUT ";
    for (int i = 0; i < output.size; i++) {
        std::cout << output[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}