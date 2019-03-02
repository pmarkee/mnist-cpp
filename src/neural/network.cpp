#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

#include "mathutils.h"
#include "neural.h"

#define RANDOM_VALUE ((double)rand() / RAND_MAX)

Network::Network(const size_t layerCount, const size_t* layerSizes)
    : iterations(0)
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
            actLayer.push_back(RANDOM_VALUE);
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
                vec.push_back(RANDOM_VALUE);
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
            vec.push_back(RANDOM_VALUE);
        }
        this->biases.push_back(vec);
    }
}

Network::Network(const std::vector<mathutils::Vector>& layers,
                 const std::vector<mathutils::Matrix>& weightMatrices,
                 const std::vector<mathutils::Vector>& biases)
    : layers(layers)
    , weightMatrices(weightMatrices)
    , biases(biases)
    , iterations(0)
{
}

Network::~Network()
{
}

void Network::nextIteration()
{
    this->computeNewValues();
    // this->backpropagate();
}

void Network::computeNewValues()
{
    return;
    for (int i = 0; i < this->layers.size() - 1; ++i)
    {
        // TODO activation function!
        // TODO this assignment will not be good (vector<double> = mathutils::Vector)
        // this->layers[i + 1] = this->weightMatrices[i] * this->layers[i] + this->biases[i];
    }
}

void Network::backpropagate()
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
