#include <iostream>

#include "mathutils.h"

const mathutils::Vector operator*(const mathutils::Matrix& mat, const mathutils::Vector& vec)
{
    mathutils::Vector ret;
    if (mat[0].size() != vec.size())
    {
        return ret;
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

std::ostream& operator<<(std::ostream& os, const mathutils::Matrix& mat)
{
    for (int i = 0; i < mat.size(); ++i)
    {
        os << mat[i];
    }
    return os;
}
