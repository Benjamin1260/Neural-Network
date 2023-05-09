#include <iostream>
#include <cstdlib>
#include <vector>
#include <cmath>

struct neuron {
    float valueA = 0;
    float bias = 0;
    std::vector<float> weight;

    neuron() {}; //placeholder
    neuron(int wSize) : weight(wSize) {
        for (int i = 0; i < weight.size(); i++) {
            weight[i] = std::rand() % 10000 / 1000.0 - 5.0; //random float -4.999 < x < 4.999
        }
        bias = std::rand() % 10000 / 1000.0 - 5.0; //random float -4.999 < x < 4.999
    };

    //take valueA prev layer, sum(*weights, bias), use sigmoid
    void calcA(std::vector<float> prevLayer) {
        float z = bias;
        for (int i = 0; i < prevLayer.size(); i++) {
            z += prevLayer[i] * weight[i];
        }
        valueA = 1/(1+exp(z*-1.0)); //sigmoid function
    };
    void calcA(std::vector<neuron> prevLayer) {
        float z = bias;
        for (int i = 0; i < prevLayer.size(); i++) {
            z += prevLayer[i].valueA * weight[i];
        }
        valueA = 1/(1+exp(z*-1.0)); //sigmoid function
    };
};

class neuralNetwork {
    using neuronMatrix = std::vector<std::vector<neuron>>;
    neuronMatrix network;
    int iSize;
    int midLayers;

    //instantiate layer with <size> neurons each having <prevSize> weights
    void instLayer(int layer, int size) {
        int prevSize = layer == 0? iSize : network[layer-1].size();
        network[layer] = std::vector<neuron>(size);
        for (int i = 0; i < size; i++) {
            network[layer][i] = *(new neuron(prevSize));
            //std::cout << "NEURON ADDED\n";
        }
    };

    //calculate valueA neurons of layer 0
    void calcLayer(std::vector<float> inputLayer, int layerNum = 0) {
        std::vector<neuron> &layer = network[layerNum];
        for (int i = 0; i < layer.size(); i++) {
            layer[i].calcA(inputLayer);
        }
    }
    //calculate valueA neurons of layer <layerNum>
    void calcLayer(int layerNum) {
        if (layerNum == 0) {
            std::cout << "ERROR: no input array given";
            exit(EXIT_FAILURE);
        }
        std::vector<neuron> &layer = network[layerNum];
        for (int i = 0; i < layer.size(); i++) {
            layer[i].calcA(network[layerNum-1]);
        }
    }

    public:
        neuralNetwork(int iSize, int mSize, int midLayers, int oSize) : network(midLayers+1), iSize(iSize), midLayers(midLayers) {
            for (int i = 0; i < midLayers; i++) { instLayer(i, mSize); }//create middle layers
            instLayer(midLayers, oSize);//create output layer
        };

        std::vector<float> run(std::vector<float> inputArr) {
            calcLayer(inputArr); 
            for (int i = 1; i <= midLayers; i++) { //<= because totLayers = midLayers+1 (outLayer not included in midLayers)
                calcLayer(i);
            }
            //create array with all valueA from outputLayer
            std::vector<neuron>& outLayer = network[midLayers];
            std::vector<float> outputValues(outLayer.size());
            for (int i = 0; i < outLayer.size(); i++) {
                outputValues[i] = outLayer[i].valueA;
            }
            return outputValues;
        };
};