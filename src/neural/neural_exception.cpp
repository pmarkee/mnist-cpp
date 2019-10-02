#include "neural.h"

NeuralException::NeuralException()
    : msg_("")
{
}

NeuralException::NeuralException(const char* msg_)
    : msg_(msg_)
{
}

const char* NeuralException::msg()
{
    return this->msg_;
}

NeuralException::~NeuralException()
{
}
