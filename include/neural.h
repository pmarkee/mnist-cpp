#ifndef NEURAL_H
#define NEURAL_H

#include <vector>

#include "mathutils.h"
#include "networkfile.h"

class Network;
class NetworkFile;

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
    Network(const NetworkFile& file);
    Network(const std::vector<size_t>& layerSizes,
            mathutils::activationFunction& act = mathutils::sigmoid);

    Network(const std::vector<mathutils::Matrix>& weightMatrices,
            const std::vector<mathutils::Vector>& biases,
            mathutils::activationFunction& act = mathutils::sigmoid,
            const size_t iterations = 0);

    ~Network();

    std::vector<size_t> getLayerSizes() const;
    std::vector<mathutils::Vector> getLayers() const;
    std::vector<mathutils::Vector> getNonSigmoidLayers() const;
    mathutils::Vector getExpected() const;
    std::vector<mathutils::Matrix> getWeights() const;
    std::vector<mathutils::Vector> getBiases() const;
    mathutils::vectorFunction getActivationFunction() const;
    mathutils::numericFunction getActivationFunctionDerivative() const;
    mathutils::Matrix getDeltaA() const;
    size_t layerCount() const;

    void nextIteration(const mathutils::Vector& inputLayer, const mathutils::Vector& expected);
    void computeNewValues();
    double computeCost();
    void backpropagate(size_t depth);
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
