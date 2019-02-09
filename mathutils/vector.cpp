#include "mathutils.h"

namespace mathutils
{

Vector::Vector() :
    vertical(true),
    size(0),
    elems(elementVector())
{
}

Vector::Vector(size_t size, bool vertical) :
    size(size),
    vertical(vertical)
{
    for (int i = 0; i < size; ++i)
    {
        this->elems.push_back(0);
    }
}

Vector::Vector(elementVector& elems, bool vertical) :
    size(elems.size()),
    vertical(vertical),
    elems(elems)
{
}

Vector::~Vector()
{
}

const size_t Vector::getSize() const
{
    return this->size;
}

const elementVector& Vector::getElems() const
{
    return this->elems;
}

const bool Vector::isVertical() const
{
    return this->vertical;
}

const double& Vector::operator[](size_t pos) const
{
    return this->elems[pos];
}

const Vector Vector::operator+(const Vector& other) const
{
    if (other.getSize() != this->getSize())
    {
        throw MathException("Cannot add vectors: lenghts differ");
    }
    else if (this->isVertical() != other.isVertical())
    {
        throw MathException("Cannot add vertical and horizontal vectors");
    }

    elementVector newVector;
    for (int i = 0; i < this->getSize(); ++i)
    {
        newVector.push_back((*this)[i] + other[i]);
    }

    return Vector(newVector, this->vertical);
}

std::ostream& operator<<(std::ostream& os, const Vector& vect)
{
    for (int i = 0; i < vect.getSize(); ++i)
    {
        os << vect[i] << " ";
    }
    os << "\n";
    return os;
}

} /* namespace mathutils */
