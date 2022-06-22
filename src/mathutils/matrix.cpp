#include <iostream>

#include "mathutils.h"

const mathutils::Matrix operator+(const mathutils::Matrix& op1, const mathutils::Matrix& op2)
{
    mathutils::Matrix ret;
    if (op1.size() != op2.size()) {
        throw mathutils::MathException("Invalid size of operands");
    }

    for (int i = 0; i < op1.size(); ++i)
    {
        ret.push_back(op1[i] + op2[i]);
    }

    return ret;
}

const mathutils::Matrix operator-(const mathutils::Matrix& op1, const mathutils::Matrix& op2)
{
    mathutils::Matrix ret;
    if (op1.size() != op2.size()) {
        throw mathutils::MathException("Invalid size of operands");
    }

    for (int i = 0; i < op1.size(); ++i)
    {
        ret.push_back(op1[i] - op2[i]);
    }

    return ret;
}

// NOTE: this is not a real Schur product,
// this basically just multiplies every line
// of a matrix with the corresponding index
// of a vector
const mathutils::Matrix schurProduct(const mathutils::Matrix& mat, const mathutils::Vector& vec)
{
    mathutils::Matrix ret;
    // Comparing number of rows since this is a Schur product.
    if (mat.size() != vec.size()) {
        throw mathutils::MathException("Invalid size of operands");
    }

    for (int i = 0; i < mat.size(); i++) {
        // use mathutils::Vector operator*(mathutils::Vector, double)
        ret.push_back(mat[i] * vec[i]);
    }

    return ret;
}

// This one is the real Schur product
const mathutils::Matrix schurProduct(const mathutils::Matrix& op1, const mathutils::Matrix& op2)
{
    mathutils::Matrix ret;
    if (op1.size() != op2.size() || op1[0].size() != op2[0].size())
    {
        throw mathutils::MathException("Invalid size of operands");
    }

    for (int i = 0; i < op1.size(); i++)
    {
        mathutils::Vector vec;
        for (int j = 0; j < op1[0].size(); j++)
        {
            vec.push_back(op1[i][j] * op2[i][j]);
        }
        ret.push_back(vec);
    }
    return ret;
}

const mathutils::Matrix operator*(const mathutils::Matrix& mat, double mul)
{
    mathutils::Matrix ret;
    for (int i = 0; i < mat.size(); i++)
    {
        mathutils::Vector vec;
        for (int j = 0; j < mat[i].size(); j++)
        {
            vec.push_back(mat[i][j] * mul);
        }
        ret.push_back(vec);
    }
    return ret;
}

const mathutils::Vector operator*(const mathutils::Matrix& mat, const mathutils::Vector& vec)
{
    mathutils::Vector ret;
    if (mat[0].size() != vec.size())
    {
        throw mathutils::MathException("Invalid size of operands");
    }
    for (int i = 0; i < mat.size(); ++i)
    {
        double sum = 0;
        for (int j = 0; j < vec.size(); ++j)
        {
            sum += mat[i][j] * vec[j];
        }
        ret.push_back(sum);
    }
    return ret;
}

const mathutils::Matrix operator*(const mathutils::Matrix& op1, const mathutils::Matrix& op2)
{
    mathutils::Matrix ret;
    if (op1[0].size() != op2.size()) {
        throw mathutils::MathException("Invalid size of operands");
    }

    for (int i = 0; i < op1.size(); i++) {
        mathutils::Vector vec;
        for (int j = 0; j < op2[0].size(); j++) {
            double sum = 0;
            for (int k = 0; k < op2.size(); k++) {
                sum += op1[i][k] * op2[k][j];
            }
            vec.push_back(sum);
        }
        ret.push_back(vec);
    }
    return ret;
}

const mathutils::Matrix operator/(const mathutils::Matrix& mat, size_t div)
{
    mathutils::Matrix ret;
    for (int i = 0; i < mat.size(); ++i)
    {
        ret.push_back(mat[i] / div);
    }
    return ret;
}

const mathutils::Matrix transpose(const mathutils::Matrix& op)
{
    mathutils::Matrix ret;

    for (int j = 0; j < op[0].size(); j++)
    {
        mathutils::Vector vec;
        for (int i = 0; i < op.size(); i++)
        {
            vec.push_back(op[i][j]);
        }
        ret.push_back(vec);
    }

    return ret;
}

std::ostream& operator<<(std::ostream& os, const mathutils::Matrix& mat)
{
    for (int i = 0; i < mat.size(); ++i)
    {
        os << mat[i];
    }
    return os;
}
