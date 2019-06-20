#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>

namespace mathutils {

typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

typedef Vector (*vectorFunction)(Vector);
typedef double (*numericFunction)(double);
typedef std::pair<vectorFunction, numericFunction> activationFunction;

Vector vectorSigmoid(Vector z);
double d_numericSigmoid(double d);

extern activationFunction sigmoid;

inline double diffSquare(double a, double b)
{
    return pow(a - b, 2);
}

/**
 * Custom exception class for error handling.
 */
class MathException
{
    const char* msg;
public:
    MathException(const char* msg);
    ~MathException();

    const char* getMsg() const;
};

} /* namespace mathutils */

// TODO some of these operator functions could maybe use the other ones, investigate this.
const mathutils::Vector operator+(const mathutils::Vector& op1, const mathutils::Vector& op2);
const mathutils::Vector operator-(const mathutils::Vector& vec);
const mathutils::Vector operator-(const mathutils::Vector& op1, const mathutils::Vector& op2);
const mathutils::Vector operator*(const mathutils::Vector& vec, double mul);
const mathutils::Vector operator*(const mathutils::Vector& op1, const mathutils::Vector& op2);
const mathutils::Vector operator*(const mathutils::Vector& vec, const mathutils::Matrix& mat);
const mathutils::Vector operator/(const mathutils::Vector& vec, const size_t div);
std::ostream& operator<<(std::ostream& os, const mathutils::Vector& vec);

const mathutils::Matrix operator+(const mathutils::Matrix& op1, const mathutils::Matrix& op2);
const mathutils::Matrix operator-(const mathutils::Matrix& op1, const mathutils::Matrix& op2);
const mathutils::Matrix schurProduct(const mathutils::Matrix& mat, const mathutils::Vector& vec);
const mathutils::Vector operator*(const mathutils::Matrix& mat, const mathutils::Vector& vec);
const mathutils::Matrix operator*(const mathutils::Matrix& op1, const mathutils::Matrix& op2);
const mathutils::Matrix operator/(const mathutils::Matrix& mat, const size_t div);
std::ostream& operator<<(std::ostream& os, const mathutils::Matrix& mat);

#endif /* MATHUTILS_H */
