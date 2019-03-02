#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <iostream>
#include <vector>

namespace mathutils {

typedef std::vector<double> Vector;
typedef std::vector<Vector> Matrix;

typedef Vector (*activationFunction)(Vector);

Vector sigmoid(Vector z);

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

const mathutils::Vector operator+(const mathutils::Vector& op1, const mathutils::Vector& op2);
const mathutils::Vector operator-(const mathutils::Vector& vec);
const mathutils::Vector operator-(const mathutils::Vector& op1, const mathutils::Vector& op2);
std::ostream& operator<<(std::ostream& os, const mathutils::Vector& vec);

const mathutils::Vector operator*(const mathutils::Matrix& mat, const mathutils::Vector& vec);
std::ostream& operator<<(std::ostream& os, const mathutils::Matrix& mat);

#endif /* MATHUTILS_H */
