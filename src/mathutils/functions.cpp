#include <cmath>

#include "mathutils.h"

namespace mathutils {

Vector sigmoid(Vector z)
{
    Vector ret;
    for (Vector::iterator i = z.begin(); i != z.end(); ++i)
    {
        ret.push_back(1 / (1 + exp(-(*i))));
    }
    return ret;
}

} /* namespace mathutils */
