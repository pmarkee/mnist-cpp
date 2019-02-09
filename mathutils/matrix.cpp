#include <iostream>
#include <vector>

#include "mathutils.h"

namespace mathutils
{

Matrix::Matrix()
{
}

Matrix::Matrix(size_t rows, size_t cols) :
    rows(rows),
    cols(cols)
{
    for (int i = 0; i < rows; ++i)
    {
        elementVector* newRow = new elementVector;
        for (int j = 0; j < cols; ++j)
        {
            newRow->push_back(0);
        }
        this->elems.push_back(*newRow);
        delete newRow;
    }
}

Matrix::Matrix(elementMatrix& elems) :
    elems(elems)
{
    for (elementMatrix::iterator row = elems.begin(); row != elems.end() - 1; ++row)
    {
        if (row->size() != (row+1)->size())
        {
            throw MathException("Invalid element matrix: row sizes are not equal");
        }
    }

    this->rows = elems.size();
    this->cols = elems[0].size();
}

Matrix::~Matrix()
{
}

const size_t Matrix::getRows() const
{
    return this->rows;
}

const size_t Matrix::getCols() const
{
    return this->cols;
}

const elementMatrix& Matrix::getElems() const
{
    return this->elems;
}

const elementVector& Matrix::operator[](size_t row) const
{
    return this->elems[row];
}

/**
 * Multiply by vector from right.
 */
const Vector Matrix::operator*(const Vector& vect) const
{
    if (!vect.isVertical())
    {
        throw MathException("Cannot right-multiply with non-vertical vector");
    }
    if (vect.getSize() != this->cols)
    {
        throw MathException("Cannot multiply matrix and vector: lengths differ");
    }

    elementVector result;
    double sum;
    for (int i = 0; i < this->getRows(); ++i)
    {
        sum = 0;
        for (int j = 0; j < this->getCols(); ++j)
        {
            sum += (*this)[i][j] * vect[j];
        }
        result.push_back(sum);
    }

    return Vector(result);
}

std::ostream& operator<<(std::ostream& os, const Matrix& mat)
{
    for (int i = 0; i < mat.getRows(); ++i)
    {
        for (int j = 0; j < mat.getCols(); ++j)
        {
            os << mat[i][j] << " ";
        }
        os << "\n";
    }
    return os;
}

} /* namespace mathutils */
