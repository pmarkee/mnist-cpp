#include <cmath>

#include "mathutils.h"

namespace mathutils {

double numericSigmoid(double d)
{
    return 1 / (1 + exp(-d));
}

Vector vectorSigmoid(Vector z)
{
    Vector ret;
    for (Vector::iterator i = z.begin(); i != z.end(); ++i)
    {
        ret.push_back(numericSigmoid(*i));
    }
    return ret;
}


double d_numericSigmoid(double d)
{
    double y = numericSigmoid(d);
    return y * (1 - y);
}

activationFunction sigmoid(vectorSigmoid, d_numericSigmoid);

} /* namespace mathutils */
