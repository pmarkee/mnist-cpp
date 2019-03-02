#include <iostream>

#include "mathutils.h"

const mathutils::Vector operator+(const mathutils::Vector& op1, const mathutils::Vector& op2)
{
    mathutils::Vector ret;
    if (op1.size() != op2.size())
    {
        return ret;
    }
    for (int i = 0; i < op1.size(); ++i)
    {
        ret.push_back(op1[i] + op2[i]);
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
