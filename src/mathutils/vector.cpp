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

const mathutils::Vector schurProduct(const mathutils::Vector& op1, const mathutils::Vector& op2)
{
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

const mathutils::Matrix operator*(const mathutils::Vector& op1, const mathutils::Vector& op2)
{
    // NOTE: there is a very strong assumption here that these are n x 1 and 1 x m vectors.
    // Basically we are taking the transpose of op2.
    mathutils::Matrix ret;

    for (int i = 0; i < op1.size(); i++)
    {
        mathutils::Vector vec;
        for (int j = 0; j < op2.size(); j++)
        {
            vec.push_back(op1[i] * op2[j]);
        }
        ret.push_back(vec);
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
        double sum = 0;
        for (int j = 0; j < vec.size(); ++j) {
            sum += vec[j] * mat[j][i];
        }
        ret.push_back(sum);
    }

    return ret;
}

const mathutils::Vector operator/(const mathutils::Vector& vec, size_t div)
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
