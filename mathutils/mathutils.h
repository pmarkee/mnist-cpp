#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <iostream>
#include <vector>

namespace mathutils {

typedef std::vector<double> elementVector;
typedef std::vector<elementVector> elementMatrix;

class Vector
{
    size_t size;
    bool vertical;
    elementVector elems;
public:
    Vector();
    Vector(size_t size, bool vertical = true);
    Vector(elementVector& elems, bool vertical = true);
    ~Vector();

    const size_t getSize() const;
    const elementVector& getElems() const;
    const bool isVertical() const;

    const double& operator[](size_t pos) const;
    const Vector operator+(const Vector& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Vector& vect);
};

class Matrix
{
protected:
    size_t rows;
    size_t cols;
    elementMatrix elems;
public:
    Matrix();
    Matrix(size_t rows, size_t cols);
    Matrix(elementMatrix& elems);
    ~Matrix();

    const size_t getRows() const;
    const size_t getCols() const;
    const elementMatrix& getElems() const;

    const elementVector& operator[](size_t row) const;
    const Vector operator*(const Vector& vect) const;
    friend std::ostream& operator<<(std::ostream& os, const Matrix& mat);
};

class MathException
{
    const char* msg;
public:
    MathException(const char* msg);
    ~MathException();

    const char* getMsg() const;
};

} /* namespace mathutils */

#endif /* MATHUTILS_H */
