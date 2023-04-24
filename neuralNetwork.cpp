#include <iostream>
#include <cstdlib>
#include <exception>
#include <chrono>
#include <cmath>
#include <type_traits>
//#include "MNIST_Read.h"

/*create a bundle which basically acts like an array but with a size attached,
idk abt performance though, might aswel use a vector at this point*/
template <typename T>
struct custArr {
    T * ptr;
    int size = 0;
    bool memAlocated = false;

    custArr() : ptr(NULL) {
        //std::cout << "NULLPTR CREATED: " << this << std::endl;
    };
    custArr(int iSize) : size(iSize) {
        if (size > 0) {
            ptr = new T[size];
            memAlocated = true;
            //std::cout << "PTR CREATED: " << this << std::endl;
        } else {
            std::cout << "UNDEFINED BEHAVIOUR: SIZE = " << size << std::endl;
            ptr = NULL;
        }
    };

    ~custArr() {
        memAlocated = false;
        //std::cout << "DESTROYING TYPE: " << typeid(T).name() << std::endl;
        //std::cout << (ptr != NULL? " DELETED\n" : " NULLPTR\n");
        if (memAlocated) {
            try {
                delete[] ptr;
            } catch (const std::exception& e) {
                std::cout << "ERROR: " << e.what() << std::endl;
            }
        }
    } 

    T& operator[](int i) {
        try {
            if (!memAlocated || ptr == NULL) {
                throw ("ERROR: invalid mem access attempt");
            }
            if (i < 0 || i >= size) {
                throw ("ERROR: out of bound");
            }
            return ptr[i];
        } catch (const char* error) {
            std::cout << error;
            exit(EXIT_FAILURE);
        }
    };

    custArr<T>& operator=(const custArr<T>& other) {
        this->size = other.size;
        this->memAlocated = other.memAlocated;
        this->ptr = memAlocated? other.ptr : NULL;
        //std::cout << "custArr coppied\n";
        return *this;
    }
};

struct neuron {
    float valueA = 0;
    float bias = 0;
    bool valAssigned = false;
    custArr<float> weight;

    neuron() {}; //placeholder
    neuron(int wSize) : weight(wSize) {
        for (int i = 0; i < weight.size; i++) {
            weight[i] = std::rand() % 10000 / 1000.0 - 5.0; //random float -4.999 < x < 4.999
        }
        bias = std::rand() % 10000 / 1000.0 - 5.0; //random float -4.999 < x < 4.999
    }

    //take valueA prev layer, sum(*weights, bias), use sigmoid
    void calcA(custArr<float> prevLayer) {
        float z = bias;
        for (int i = 0; i < prevLayer.size; i++) {
            z += prevLayer[i] * weight[i];
        }
        valueA = 1/(1+exp(z*-1.0)); //sigmoid function
    }
    void calcA(custArr<neuron> prevLayer) {
        float z = bias;
        for (int i = 0; i < prevLayer.size; i++) {
            z += prevLayer[i].valueA * weight[i];
        }
        valueA = 1/(1+exp(z*-1.0)); //sigmoid function
    }
};

class neuralNetwork {
    using neuronMatrix = custArr<custArr<neuron>>;
    neuronMatrix network;
    int iSize;
    int midLayers;

    //instantiate layer with <size> neurons each having <prevSize> weights
    void instLayer(int layer, int size) {
        int prevSize = layer == 0? iSize : network[layer-1].size;
        network[layer] = custArr<neuron>(size);
        for (int i = 0; i < size; i++) {
            network[layer][i] = *(new neuron(prevSize));
            //std::cout << "NEURON ADDED\n";
        }
    };

    //calculate valueA neurons of layer 0
    void calcLayer(custArr<float> inputLayer, int layerNum = 0) {
        custArr<neuron> layer = network[layerNum];
        for (int i = 0; i < layer.size; i++) {
            layer[i].calcA(inputLayer);
        }
    }
    //calculate valueA neurons of layer <layerNum>
    void calcLayer(int layerNum) {
        if (layerNum == 0) {
            std::cout << "ERROR: no input array given";
            exit(EXIT_FAILURE);
        }
        custArr<neuron> layer = network[layerNum];
        for (int i = 0; i < layer.size; i++) {
            layer[i].calcA(network[layerNum-1]);
        }
    }

    public:
        neuralNetwork(int iSize, int mSize, int midLayers, int oSize) : network(midLayers+1), iSize(iSize), midLayers(midLayers) {
            for (int i = 0; i < midLayers; i++) {
                instLayer(i, mSize);//create middle layers
            }
            instLayer(midLayers, oSize);//create output layer
        };

        custArr<float> run(custArr<float> inputArr) {
            calcLayer(inputArr); 
            for (int i = 1; i <= midLayers; i++) { //<= because totLayers = midLayers+1 (outLayer not included in midLayers)
                calcLayer(i);
            }
            //create array with all valueA from outputLayer
            custArr<neuron>& outLayer = network[midLayers];
            custArr<float> outputValues(outLayer.size);
            for (int i = 0; i < outLayer.size; i++) {
                outputValues[i] = outLayer[i].valueA;
            }
            return outputValues;
        };
};

int main() {
    //initializing seed for random number generation
    auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::srand(seed);

    //temp inputData array, needs to be changed later to take input from MNIST database
    // <NEXT> read the file with input formats and create an array of matrices with the image data + array of desired output ==> struct or pair?
    custArr<float> inData(4);
    inData[0] = 1.000;
    inData[1] = 0.000;
    inData[2] = 1.000;
    inData[3] = 0.000;

    //initializing the neural network
    int inSize = 28*28, midSize = 28*28, layer = 2, outSize = 10; //data can be changed later to use std::cin
    neuralNetwork myNetwork(inSize, midSize, layer, outSize);
    custArr<float> output = myNetwork.run(inData);

    std::cout << "OUTPUT ";
    for (int i = 0; i < output.size; i++) {
        std::cout << output[i] << " ";
    }
    std::cout << std::endl;
    return 0;
}