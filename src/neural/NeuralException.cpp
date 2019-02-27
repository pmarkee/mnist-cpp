#include "neural.h"

NeuralException::NeuralException()
    : msg("")
{
}

NeuralException::NeuralException(const char* msg)
    : msg(msg)
{
}

const char* NeuralException::getMsg()
{
    return this->msg;
}

NeuralException::~NeuralException()
{
}
