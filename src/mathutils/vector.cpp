#include <iostream>

#include "mathutils.h"

const mathutils::Vector operator+(const mathutils::Vector& op1, const mathutils::Vector& op2)
{
    mathutils::Vector ret;
    if (op1.size() != op2.size())
    {
        throw mathutils::MathException("Invalid size of operands");
    }
    for (int i = 0; i < op1.size(); ++i)
    {
        ret.push_back(op1[i] + op2[i]);
    }
    return ret;
}

const mathutils::Vector operator-(const mathutils::Vector& vec)
{
    mathutils::Vector ret;

    for (int i = 0; i < vec.size(); ++i)
    {
        ret.push_back(-vec[i]);
    }
    return ret;
}

const mathutils::Vector operator-(const mathutils::Vector& op1, const mathutils::Vector& op2)
{
    if (op1.size() != op2.size()) {
        throw mathutils::MathException("Invalid size of operands");
    }
    return op1 + (-op2);
}

const mathutils::Vector operator*(const mathutils::Vector& vec, double mul)
{
    mathutils::Vector ret;
    // Multiply all elements of the vector.
    for (int i = 0; i < vec.size(); i++) {
        ret.push_back(vec[i] *  mul);
    }
    return ret;
}

const mathutils::Vector operator*(const mathutils::Vector& op1, const mathutils::Vector& op2)
{
    // NOTE: this is HADAMARD product!
    if (op1.size() != op2.size()) {
        throw mathutils::MathException("Invalid size of operands");
    }
    mathutils::Vector ret;

    for (int i = 0; i < op1.size(); ++i)
    {
        ret.push_back(op1[i] * op2[i]);
    }

    return ret;
}

const mathutils::Vector operator*(const mathutils::Vector& vec, const mathutils::Matrix& mat)
{
    // NOTE: vec is an 1 x n vector here!
    if (vec.size() != mat.size()) {
        throw mathutils::MathException("Invalid size of operands");
    }
    mathutils::Vector ret;

    for (int i = 0; i < mat[0].size(); ++i)
    {
        for (int j = 0; j < vec.size(); ++j) {
            ret.push_back(vec[j] * mat[j][i]);
        }
    }

    return ret;
}

const mathutils::Vector operator/(const mathutils::Vector& vec, const size_t div)
{
    mathutils::Vector ret;
    for (int i = 0; i < vec.size(); ++i)
    {
        ret.push_back(vec[i] / div);
    }
    return ret;
}

std::ostream& operator<<(std::ostream& os, const mathutils::Vector& vec)
{
    for (int i = 0; i < vec.size(); ++i)
    {
        os << vec[i] << " ";
    }
    os << "\n";
    return os;
}
