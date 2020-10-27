#ifndef NEURAL_H
#define NEURAL_H

#include <vector>

#include "mathutils.h"
#include "networkfile.h"

class Network;
class NetworkFile;

// NOTE: these differ from standard naming for better readability.
mathutils::Vector deltaC_deltaZ(Network net);
mathutils::Vector deltaZ_deltaW(Network net, size_t depth);
mathutils::Vector deltaZ_deltaB(Network net, size_t depth);
mathutils::Matrix deltaZ_deltaZ(Network net, size_t depth);

class Network {
    std::vector<size_t> layerSizes_;
    std::vector<mathutils::Vector> layers_;
    std::vector<mathutils::Vector> zLayers_;
    mathutils::Vector expected_;
    std::vector<mathutils::Matrix> weights_;
    std::vector<mathutils::Vector> biases_;
    mathutils::ActivationFunction act_;
    size_t iterations;

    std::vector<mathutils::Matrix> deltaWeights;
    std::vector<mathutils::Vector> deltaBiases;
    mathutils::Vector deltaC;
    mathutils::Matrix deltaZ_;
    mathutils::Matrix prevDeltaZ;
public:
    Network(const NetworkFile& file,
            mathutils::ActivationFunction& act_ = mathutils::sigmoid);
    Network(const std::vector<size_t>& layerSizes_,
            mathutils::ActivationFunction& act_ = mathutils::sigmoid);

    ~Network();

    void init();

    std::vector<size_t> layerSizes() const;
    std::vector<mathutils::Vector> layers() const;
    mathutils::Vector outputLayer() const;
    std::vector<mathutils::Vector> zLayers() const;
    mathutils::Vector expected() const;
    std::vector<mathutils::Matrix> weights() const;
    std::vector<mathutils::Vector> biases() const;
    mathutils::Matrix deltaZ() const;
    size_t layerCount() const;
    // NOTE: these two getters do not just directly just return this->act_,
    // as that is a typedef to an std::pair.
    mathutils::VectorFunction act() const;
    mathutils::VectorFunction d_act() const;

    void nextIteration(const mathutils::Vector& inputLayer, const mathutils::Vector& expected, bool learn = true);
    void evaluate();
    double loss();
    void backpropagate(size_t depth);
    void finalize();

    void info();
};

class NeuralException {
    const char* msg_;
public:
    NeuralException();
    NeuralException(const char* msg);
    ~NeuralException();

    const char* msg();
};

#endif /* NEURAL_H */
