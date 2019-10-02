#include <cmath>

#include "mathutils.h"

namespace mathutils {

double numeric_sigmoid(double d)
{
    return 1 / (1 + exp(-d));
}

Vector vector_sigmoid(Vector z)
{
    Vector ret;
    for (Vector::iterator i = z.begin(); i != z.end(); ++i)
    {
        ret.push_back(numeric_sigmoid(*i));
    }
    return ret;
}


double d_numeric_sigmoid(double d)
{
    double y = numeric_sigmoid(d);
    return y * (1 - y);
}

ActivationFunction sigmoid(vector_sigmoid, d_numeric_sigmoid);

} /* namespace mathutils */
