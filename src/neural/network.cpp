#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "mathutils.h"
#include "neural.h"

#define RANDOM_VALUE(divider) ((double)rand() / (RAND_MAX * (divider)))

Network::Network(const size_t layerCount,
                 const size_t* layerSizes,
                 mathutils::activationFunction act)
    : iterations(0)
    , act(act)
{
    if (layerCount < 2) {
        throw NeuralException("Invalid layer count");
    }

    // Layers with random activation neurons
    for (int i = 0; i < layerCount; ++i)
    {
        if (layerSizes[i] <= 0)
        {
            throw NeuralException("Invalid layer size");
        }

        mathutils::Vector actLayer;
        for (int j = 0; j < layerSizes[i]; ++j)
        {
            actLayer.push_back(RANDOM_VALUE(1));
        }
        this->layers.push_back(actLayer);
    }

    // Random weight matrices
    for (int i = 0; i < layerCount - 1; ++i)
    {
        size_t rows = this->layers[i+1].size();
        size_t cols = this->layers[i].size();

        mathutils::Matrix mat;
        for (int i = 0; i < rows; ++i)
        {
            mathutils::Vector vec;
            for (int j = 0; j < cols; ++j)
            {
                vec.push_back(RANDOM_VALUE(1));
            }
            mat.push_back(vec);
        }

        this->weightMatrices.push_back(mat);
    }

    // Random bias vectors
    for (int i = 0; i < layerCount - 1; ++i)
    {
        mathutils::Vector vec;
        for (int j = 0; j < this->layers[i+1].size(); ++j)
        {
            vec.push_back(RANDOM_VALUE(0.1));
        }
        this->biases.push_back(vec);
    }
}

Network::Network(const std::vector<mathutils::Vector>& layers,
                 const std::vector<mathutils::Matrix>& weightMatrices,
                 const std::vector<mathutils::Vector>& biases,
                 mathutils::activationFunction act,
                 const size_t iterations)
    : layers(layers)
    , weightMatrices(weightMatrices)
    , biases(biases)
    , act(act)
    , iterations(iterations)
{
}

Network::~Network()
{
}

void Network::nextIteration(const mathutils::Vector& expected)
{
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
