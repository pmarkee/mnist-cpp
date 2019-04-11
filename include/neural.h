#ifndef NEURAL_H
#define NEURAL_H

#include <vector>

#include "mathutils.h"

class Network;

mathutils::Vector deltaC_deltaA(Network net);
mathutils::Matrix deltaA_deltaW(Network net, size_t depth);
mathutils::Vector deltaA_deltaB(Network net, size_t depth);
mathutils::Matrix deltaA_deltaA(Network net, size_t depth);

class Network {
    std::vector<size_t> layerSizes;
    std::vector<mathutils::Vector> layers;
    std::vector<mathutils::Vector> nonSigmoidLayers;
    mathutils::Vector expected;
    std::vector<mathutils::Matrix> weightMatrices;
    std::vector<mathutils::Vector> biases;
    mathutils::activationFunction act;
    size_t iterations; // TODO actually use this

    std::vector<mathutils::Matrix> deltaWeights;
    std::vector<mathutils::Vector> deltaBiases;
    mathutils::Vector deltaC;
    mathutils::Matrix deltaA;
public:
    Network(const std::vector<size_t>& layerSizes,
            mathutils::activationFunction& act = mathutils::sigmoid);

    Network(const std::vector<mathutils::Matrix>& weightMatrices,
            const std::vector<mathutils::Vector>& biases,
            mathutils::activationFunction& act = mathutils::sigmoid,
            const size_t iterations = 0);

    ~Network();

    std::vector<mathutils::Vector> getLayers();
    std::vector<mathutils::Vector> getNonSigmoidLayers();
    mathutils::Vector getExpected();
    std::vector<mathutils::Matrix> getWeights();
    std::vector<mathutils::Vector> getBiases();
    mathutils::vectorFunction getActivationFunction();
    mathutils::numericFunction getActivationFunctionDerivative();
    size_t layerCount();

    void nextIteration(const mathutils::Vector& inputLayer, const mathutils::Vector& expected);
    void computeNewValues();
    double computeCost();
    void backpropagate(size_t depth);
    mathutils::Matrix getDeltaA();
    void finalize();

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
