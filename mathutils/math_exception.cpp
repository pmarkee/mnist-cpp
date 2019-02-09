#include "mathutils.h"

namespace mathutils
{

MathException::MathException(const char* msg) :
    msg(msg)
{
}

MathException::~MathException()
{
}

const char* MathException::getMsg() const
{
    return this->msg;
}

} /* namespace mathutils */
