#ifndef NEURAL_H
#define NEURAL_H

#include <vector>

#include "mathutils.h"

class Network {
    std::vector<mathutils::Vector> layers;
    std::vector<mathutils::Matrix> weightMatrices;
    std::vector<mathutils::Vector> biases;
    size_t iterations;
    // TODO activation function?
    // TODO activation function derivative?
public:
    Network(const size_t layerCount, const size_t* layerSizes);
    Network(const std::vector<mathutils::Vector>& layers,
            const std::vector<mathutils::Matrix>& weightMatrices,
            const std::vector<mathutils::Vector>& biases);

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
