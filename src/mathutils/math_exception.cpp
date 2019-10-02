#include "mathutils.h"

namespace mathutils
{

MathException::MathException(const char* msg_)
    : msg_(msg_)
{
}

MathException::~MathException()
{
}

const char* const MathException::msg() const
{
    return this->msg_;
}

} /* namespace mathutils */
