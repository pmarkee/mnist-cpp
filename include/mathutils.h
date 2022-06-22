#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

namespace mathutils {

typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

typedef Vector (*VectorFunction)(Vector);
typedef std::pair<VectorFunction, VectorFunction> ActivationFunction;

extern ActivationFunction sigmoid;
extern ActivationFunction softmax;

extern Vector vector_sigmoid(Vector z);
extern Vector d_vector_sigmoid(Vector z);
extern Vector vector_softmax(Vector z);
extern Vector d_vector_softmax(Vector z);

inline double diffSquare(double a, double b)
{
    return pow(a - b, 2);
}

/**
 * Custom exception class for error handling.
 */
class MathException
{
    const char* msg_;
public:
    MathException(const char* msg);
    ~MathException();

    const char* const msg() const;
};

} /* namespace mathutils */

const mathutils::Vector operator+(const mathutils::Vector& op1, const mathutils::Vector& op2);
const mathutils::Vector operator-(const mathutils::Vector& vec);
const mathutils::Vector operator-(const mathutils::Vector& op1, const mathutils::Vector& op2);
const mathutils::Vector operator*(const mathutils::Vector& vec, double mul);
// const mathutils::Vector operator*(const mathutils::Vector& vec, int mul);
const mathutils::Vector schurProduct(const mathutils::Vector& op1, const mathutils::Vector& op2);
const mathutils::Matrix operator*(const mathutils::Vector& op1, const mathutils::Vector& op2);
const mathutils::Vector operator*(const mathutils::Vector& vec, const mathutils::Matrix& mat);
const mathutils::Vector operator/(const mathutils::Vector& vec, size_t div);
std::ostream& operator<<(std::ostream& os, const mathutils::Vector& vec);
size_t vector_argmax(const mathutils::Vector& vec);

const mathutils::Matrix operator+(const mathutils::Matrix& op1, const mathutils::Matrix& op2);
const mathutils::Matrix operator-(const mathutils::Matrix& op1, const mathutils::Matrix& op2);
const mathutils::Matrix schurProduct(const mathutils::Matrix& mat, const mathutils::Vector& vec);
const mathutils::Matrix schurProduct(const mathutils::Matrix& op1, const mathutils::Matrix& op2);
const mathutils::Matrix operator*(const mathutils::Matrix& mat, double mul);
const mathutils::Vector operator*(const mathutils::Matrix& mat, const mathutils::Vector& vec);
const mathutils::Matrix operator*(const mathutils::Matrix& op1, const mathutils::Matrix& op2);
const mathutils::Matrix operator/(const mathutils::Matrix& mat, size_t div);
const mathutils::Matrix transpose(const mathutils::Matrix& op);
std::ostream& operator<<(std::ostream& os, const mathutils::Matrix& mat);

#endif /* MATHUTILS_H */
