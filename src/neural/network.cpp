#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>
#include <cmath>

#include "mathutils.h"
#include "neural.h"

Network::Network(const std::vector<size_t>& layerSizes,
                 mathutils::activationFunction& act)
    : layerSizes(layerSizes)
    , iterations(0)
    , act(act)
{
    if (this->layerSizes.size() < 2) {
        throw NeuralException("Invalid layer count");
    }

    // TODO are these initializations really necessary?

    // Layers with random activation neurons
    for (int i = 0; i < layerSizes.size(); ++i)
    {
        if ((int64_t)layerSizes[i] <= 0)
        {
            // TODO give some warning here?
            // If a negative value is passed by accident, that will be a very large
            // number when handled as size_t.
        }

        mathutils::Vector actLayer;
        for (int j = 0; j < layerSizes[i]; ++j)
        {
            actLayer.push_back(0);
        }
        this->layers.push_back(actLayer);
        this->nonSigmoidLayers.push_back(actLayer);
    }

    std::default_random_engine generator;
    double wLimit = -4 * (sqrt(6) / sqrt(this->layers[0].size() + this->layerCount()));
    std::uniform_real_distribution<double> distribution(-wLimit, wLimit);

    // Random weight matrices
    for (int i = 0; i < this->layerCount() - 1; ++i)
    {
        size_t rows = this->layerSizes[i+1];
        size_t cols = this->layerSizes[i];

        mathutils::Matrix mat;
        mathutils::Matrix deltaMat;
        for (int i = 0; i < rows; ++i)
        {
            mathutils::Vector vec;
            mathutils::Vector deltaVec;
            for (int j = 0; j < cols; ++j)
            {
                // NOTE: this is only pseudo-random.
                vec.push_back(distribution(generator));
                deltaVec.push_back(0);
            }
            mat.push_back(vec);
            deltaMat.push_back(deltaVec);
        }

        this->weightMatrices.push_back(mat);
        this->deltaWeights.push_back(deltaMat);
    }

    // Random bias vectors
    for (int i = 0; i < this->layerCount() - 1; ++i)
    {
        mathutils::Vector vec;
        mathutils::Vector deltaVec;
        for (int j = 0; j < this->layerSizes[i+1]; ++j)
        {
            // NOTE: this is only pseudo-random.
            vec.push_back(distribution(generator));
            deltaVec.push_back(0);
        }
        this->biases.push_back(vec);
        this->deltaBiases.push_back(deltaVec);
    }

    for (size_t i = 0; i < layerSizes.back(); i++) {
        this->expected.push_back(0);
    }
}

Network::Network(const std::vector<mathutils::Matrix>& weightMatrices,
                 const std::vector<mathutils::Vector>& biases,
                 mathutils::activationFunction& act,
                 const size_t iterations)
    : weightMatrices(weightMatrices)
    , biases(biases)
    , act(act)
    , iterations(iterations)
{
    for (std::vector<mathutils::Vector>::iterator i = layers.begin(); i != layers.end(); ++i)
    {
        this->layerSizes.push_back(i->size());
    }
}

Network::~Network()
{
}

std::vector<mathutils::Vector> Network::getLayers() {
    return this->layers;
}

std::vector<mathutils::Vector> Network::getNonSigmoidLayers() {
    return this->nonSigmoidLayers;
}

mathutils::Vector Network::getExpected() {
    return this->expected;
}

mathutils::Matrix Network::getDeltaA() {
    return this->deltaA;
}

std::vector<mathutils::Matrix> Network::getWeights() {
    return this->weightMatrices;
}

std::vector<mathutils::Vector> Network::getBiases() {
    return this->biases;
}

mathutils::vectorFunction Network::getActivationFunction() {
    return this->act.first;
}

mathutils::numericFunction Network::getActivationFunctionDerivative() {
    return this->act.second;
}

size_t Network::layerCount() {
    return this->layers.size();
}

void Network::nextIteration(const mathutils::Vector& inputLayer, const mathutils::Vector& expected)
{
    this->iterations++;
    if (this->expected.size() != expected.size()) {
        throw NeuralException("Size of expected output is invalid");
    }
    this->expected = expected;

    if (this->layerSizes[0] != inputLayer.size()) {
        throw NeuralException("Invalid input layer size");
    }
    this->layers[0] = inputLayer;
    this->computeNewValues();

    double cost = this->computeCost();
    std::cout << cost << "\n";

    this->deltaC = deltaC_deltaA(*this);
    this->deltaA = deltaA_deltaA(*this, this->layerCount() - 1);
    this->backpropagate(0);
}

double Network::computeCost()
{
    mathutils::Vector actual = this->layers.back();
    // The actual and expected output layer have to be the same size.
    if (this->expected.size() != actual.size()) {
        throw NeuralException("Actual and expected output layer differ in length");
    }

    double sum = 0;
    for (size_t i = 0; i < actual.size(); ++i) {
        sum += mathutils::diffSquare(actual[i], this->expected[i]);
    }
    return sum;
}

void Network::computeNewValues()
{
    for (int i = 0; i < this->layerCount() - 1; ++i)
    {
        this->nonSigmoidLayers[i + 1] = this->weightMatrices[i] * this->layers[i] - this->biases[i];
        this->layers[i + 1] = (*this->getActivationFunction())(this->nonSigmoidLayers[i + 1]);
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
        this->deltaWeights[L-1] = this->deltaWeights[L-1] + schurProduct(deltaA_deltaW(*this, L), this->deltaC);
        this->deltaBiases[L-1] = this->deltaBiases[L-1] + deltaA_deltaB(*this, L) * this->deltaC;

        if (L > 1) {
            this->deltaA = this->deltaA * deltaA_deltaA(*this, L - 1);
        }
        this->deltaC = this->deltaC * this->deltaA;

        backpropagate(depth + 1);
    }
}

void Network::finalize()
{
    // Average the deltas over all the training examples that have been run.
    // Take a step in the direction of the negative gradient.
    // TODO do these by operator-=() and operator/().
    for (size_t i = 0; i < this->layerCount() - 1; i++) {
        this->weightMatrices[i] = this->weightMatrices[i] - this->deltaWeights[i] / this->iterations;
    }
    for (size_t i = 0; i < this->layerCount() - 1; i++) {
        this->biases[i] = this->biases[i] - this->deltaBiases[i] / this->iterations;
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
    for (std::vector<mathutils::Vector>::iterator i = this->layers.begin(); i != this->layers.end(); ++i)
    {
        std::cout << i->size() << " ";
    } 
    std::cout << "\n";

    std::cout << "Layers:\n";
    for(std::vector<mathutils::Vector>::iterator i = this->layers.begin(); i != this->layers.end(); ++i)
    {
        std::cout << *i;
        std::cout << "\n";
    }

    std::cout << "\nWeight matrices:\n";
    for (std::vector<mathutils::Matrix>::iterator i = this->weightMatrices.begin();
         i != this->weightMatrices.end();
         ++i)
    {
        std::cout << *i;
        std::cout << "\n";
    }

    std::cout << "Bias vectors:\n";
    for (std::vector<mathutils::Vector>::iterator i = this->biases.begin();
         i != this->biases.end();
         ++i)
    {
        std::cout << *i;
        std::cout << "\n";
    }
}
