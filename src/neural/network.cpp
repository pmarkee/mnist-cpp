#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#include <cmath>

#include "mathutils.h"
#include "neural.h"
#include "networkfile.h"

// TODO activation function should somehow be saved into the network file to be loaded here automatically.
Network::Network(const NetworkFile& file,
                 mathutils::ActivationFunction& act_)
    : layerSizes_(file.layerSizes())
    , weights_(file.wMatrix())
    , biases_(file.bVector())
    , act_(act_)
{
    init();
}

Network::Network(const std::vector<size_t>& layerSizes_,
                 mathutils::ActivationFunction& act_)
    : layerSizes_(layerSizes_)
    , iterations(0)
    , act_(act_)
{
    init();
}

Network::~Network()
{
}

void Network::init()
{
    if (this->layerSizes_.size() < 2) {
        throw NeuralException("Invalid layer count");
    }

    // TODO are these initializations really necessary?

    // Layers with random activation neurons
    for (int i = 0; i < this->layerSizes_.size(); ++i)
    {
        if ((int64_t)this->layerSizes_[i] <= 0)
        {
            // TODO give some warning here?
            // If a negative value is passed by accident, that will be a very large
            // number when handled as size_t.
        }

        mathutils::Vector act_layer;
        for (int j = 0; j < this->layerSizes_[i]; ++j)
        {
            act_layer.push_back(0);
        }
        this->layers_.push_back(act_layer);
        this->zLayers_.push_back(act_layer);
    }

    std::default_random_engine generator;
    double w_limit = -4 * (sqrt(6) / sqrt(this->layers_[0].size() + this->layerCount()));
    std::uniform_real_distribution<double> distribution(-w_limit, w_limit);

    // Random weight matrices
    for (int i = 0; i < this->layerCount() - 1; ++i)
    {
        size_t rows = this->layerSizes_[i+1];
        size_t cols = this->layerSizes_[i];

        mathutils::Matrix mat;
        mathutils::Matrix delta_mat;
        for (int i = 0; i < rows; ++i)
        {
            mathutils::Vector vec;
            mathutils::Vector delta_vec;
            for (int j = 0; j < cols; ++j)
            {
                // NOTE: this is only pseudo-random.
                vec.push_back(distribution(generator));
                delta_vec.push_back(0);
            }
            mat.push_back(vec);
            delta_mat.push_back(delta_vec);
        }

        this->weights_.push_back(mat);
        this->deltaWeights.push_back(delta_mat);
    }

    // Random bias vectors
    for (int i = 0; i < this->layerCount() - 1; ++i)
    {
        mathutils::Vector vec;
        mathutils::Vector delta_vec;
        for (int j = 0; j < this->layerSizes_[i+1]; ++j)
        {
            // NOTE: this is only pseudo-random.
            vec.push_back(distribution(generator));
            delta_vec.push_back(0);
        }
        this->biases_.push_back(vec);
        this->deltaBiases.push_back(delta_vec);
    }

    for (size_t i = 0; i < this->layerSizes_.back(); i++) {
        this->expected_.push_back(0);
    }
}

std::vector<size_t> Network::layerSizes() const {
    return this->layerSizes_;
}

std::vector<mathutils::Vector> Network::layers() const {
    return this->layers_;
}

mathutils::Vector Network::outputLayer() const {
    return this->layers_.back();
}

std::vector<mathutils::Vector> Network::zLayers() const {
    return this->zLayers_;
}

mathutils::Vector Network::expected() const {
    return this->expected_;
}

std::vector<mathutils::Matrix> Network::weights() const {
    return this->weights_;
}

std::vector<mathutils::Vector> Network::biases() const {
    return this->biases_;
}

mathutils::VectorFunction Network::act() const {
    return this->act_.first;
}

mathutils::VectorFunction Network::d_act() const {
    return this->act_.second;
}

size_t Network::layerCount() const {
    return this->layers_.size();
}

void Network::nextIteration(const mathutils::Vector& inputLayer, const mathutils::Vector& expected, bool learn)
{
    this->iterations++;
    this->expected_ = expected;

    if (this->layerSizes_[0] != inputLayer.size()) {
        throw NeuralException("Invalid input layer size");
    }
    this->layers_[0] = inputLayer;
    this->evaluate();

    if (learn)
    {
        this->deltaC = deltaC_deltaZ(*this);
        this->deltaZ_ = this->prevDeltaZ = deltaZ_deltaZ(*this, this->layerCount() - 1);
        this->backpropagate(0);
    }
}

double Network::loss()
{
    mathutils::Vector actual = this->layers_.back();
    // The actual and expected output layer have to be the same size.
    if (this->expected_.size() != actual.size()) {
        throw NeuralException("Actual and expected output layer differ in length");
    }

    double sum = 0;
    for (size_t i = 0; i < actual.size(); ++i) {
        sum += mathutils::diffSquare(actual[i], this->expected_[i]);
    }
    return sum / actual.size();
}

void Network::evaluate()
{
    for (int i = 0; i < this->layerCount() - 1; ++i)
    {
        this->zLayers_[i + 1] = this->weights_[i] * this->layers_[i] + this->biases_[i];
        this->layers_[i + 1] = (*this->act())(this->zLayers_[i + 1]);
    }
}

void Network::backpropagate(size_t depth)
{
    size_t L = this->layerCount() - depth - 1;

    if (depth < this->layerCount() - 1) {
        // Compute the elements of the gradient.
        // (i.e. necessary changes to the weight and bias values of the network)
        // NOTE: deltas are not overwritten in every iteration, they are added up instead.
        // Later they will be averaged by using finalize().
        this->deltaWeights[L-1] = this->deltaWeights[L-1] + this->deltaC * deltaZ_deltaW(*this, L);
        this->deltaBiases[L-1] = this->deltaBiases[L-1] + schurProduct(this->deltaC, deltaZ_deltaB(*this, L));

        this->deltaC = this->deltaC * this->prevDeltaZ;
        if (L > 1) {
            this->prevDeltaZ = deltaZ_deltaZ(*this, L - 1);
            this->deltaZ_ = this->deltaZ_ * this->prevDeltaZ;
        }

        backpropagate(depth + 1);
    }
}

void Network::finalize()
{
    // Average the deltas over all the training examples that have been run.
    // Take a step in the direction of the negative gradient.
    // TODO do these by operator-=() and operator/().
    for (size_t i = 0; i < this->layerCount() - 1; i++) {
        this->weights_[i] = this->weights_[i] - this->deltaWeights[i] / this->iterations;
    }
    for (size_t i = 0; i < this->layerCount() - 1; i++) {
        this->biases_[i] = this->biases_[i] - this->deltaBiases[i] / this->iterations;
    }
    // Reset the iteration counter so we are ready for the next subset of training data.
    this->iterations = 0;
}

void Network::info()
{
    std::cout << "----------------------\n";
    std::cout << "INFO: Network @ " << this << "\n";
    std::cout << "Layer count: " << this->layerCount() << "\n";

    std::cout << "Layer sizes: ";
    for (std::vector<mathutils::Vector>::iterator i = this->layers_.begin(); i != this->layers_.end(); ++i)
    {
        std::cout << i->size() << " ";
    }
    std::cout << "\n";

    std::cout << "Layers:\n";
    for(std::vector<mathutils::Vector>::iterator i = this->layers_.begin(); i != this->layers_.end(); ++i)
    {
        std::cout << *i;
        std::cout << "\n";
    }

    std::cout << "\nWeight matrices:\n";
    for (std::vector<mathutils::Matrix>::iterator i = this->weights_.begin();
         i != this->weights_.end();
         ++i)
    {
        std::cout << *i;
        std::cout << "\n";
    }

    std::cout << "Bias vectors:\n";
    for (std::vector<mathutils::Vector>::iterator i = this->biases_.begin();
         i != this->biases_.end();
         ++i)
    {
        std::cout << *i;
        std::cout << "\n";
    }
}
