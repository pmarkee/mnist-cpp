#ifndef NEURAL_H
#define NEURAL_H

#include <vector>

#include "mathutils.h"

class Network {
    std::vector<mathutils::Vector> layers;
    std::vector<mathutils::Matrix> weightMatrices;
    std::vector<mathutils::Vector> biases;
    mathutils::activationFunction act; // TODO derivative?
    size_t iterations; // TODO actually use this
public:
    Network(const size_t layerCount,
            const size_t* layerSizes,
            mathutils::activationFunction act = &mathutils::sigmoid);

    Network(const std::vector<mathutils::Vector>& layers,
            const std::vector<mathutils::Matrix>& weightMatrices,
            const std::vector<mathutils::Vector>& biases,
            mathutils::activationFunction act = &mathutils::sigmoid,
            const size_t iterations = 0);

    ~Network();

    void nextIteration();
    void computeNewValues();
    void backpropagate();

    void info();
};

class NeuralException {
    const char* msg;
public:
    NeuralException();
    NeuralException(const char* msg);
    ~NeuralException();

    const char* getMsg();
};

#endif /* NEURAL_H */
