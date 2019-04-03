#include <iostream>
#include <cstdlib>
#include <random>
#include <vector>

#include "mathutils.h"
#include "neural.h"

Network::Network(const std::vector<size_t>& layerSizes,
                 mathutils::activationFunction act)
    : layerSizes(layerSizes)
    , iterations(0)
    , act(act)
{
    if (layerSizes.size() < 2) {
        throw NeuralException("Invalid layer count");
    }

    // Layers with random activation neurons
    for (int i = 0; i < layerSizes[i]; ++i)
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
    }

    std::default_random_engine generator;

    // Random weight matrices
    for (int i = 0; i < layerSizes.size() - 1; ++i)
    {
        size_t rows = this->layerSizes[i+1];
        size_t cols = this->layerSizes[i];

        std::uniform_real_distribution<double> distribution(-1, 1);
        mathutils::Matrix mat;
        for (int i = 0; i < rows; ++i)
        {
            mathutils::Vector vec;
            for (int j = 0; j < cols; ++j)
            {
                // NOTE: this is only pseudo-random.
                vec.push_back(distribution(generator));
            }
            mat.push_back(vec);
        }

        this->weightMatrices.push_back(mat);
    }

    // Random bias vectors
    for (int i = 0; i < layerSizes.size() - 1; ++i)
    {
        std::uniform_real_distribution<double> distribution(-10, 10);
        mathutils::Vector vec;
        for (int j = 0; j < this->layerSizes[i+1]; ++j)
        {
            // NOTE: this is only pseudo-random.
            vec.push_back(distribution(generator));
        }
        this->biases.push_back(vec);
    }
}

Network::Network(const std::vector<mathutils::Matrix>& weightMatrices,
                 const std::vector<mathutils::Vector>& biases,
                 mathutils::activationFunction act,
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

void Network::nextIteration(const mathutils::Vector& inputLayer, const mathutils::Vector& expected)
{
    if (this->layerSizes[0] != inputLayer.size()) {
        throw NeuralException("Invalid input layer size");
    }
    this->layers[0] = inputLayer;
    this->computeNewValues();
    double cost = this->computeCost(expected);
    std::cout << cost << "\n";
    // this->backpropagate(cost);
}

double Network::computeCost(const mathutils::Vector& expected)
{
    mathutils::Vector actual = this->layers.back();
    // The actual and expected output layer have to be the same size.
    if (expected.size() != actual.size()) {
        throw NeuralException("Actual and expected output layer differ in length");
    }

    double sum = 0;
    for (size_t i = 0; i < actual.size(); ++i) {
        sum += mathutils::diffSquare(actual[i], expected[i]);
    }
    return sum;
}

void Network::computeNewValues()
{
    for (int i = 0; i < this->layers.size() - 1; ++i)
    {
        this->layers[i + 1] = (*this->act)((this->weightMatrices[i] * this->layers[i] - this->biases[i]));
    }
}

void Network::backpropagate(double cost)
{
    // TODO
    throw NeuralException("Backpropagation not yet implemented");
}

void Network::info()
{
    std::cout << "----------------------\n";
    std::cout << "INFO: Network @ " << this << "\n"; 
    std::cout << "Layer count: " << this->layers.size() << "\n";
    
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
