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

Vector d_vector_sigmoid(Vector z)
{
    Vector ret;
    for (Vector::iterator i = z.begin(); i != z.end(); ++i)
    {
        ret.push_back(d_numeric_sigmoid(*i));
    }
    return ret;
}

Vector vector_softmax(Vector z)
{
    Vector ret;
    size_t K = z.size();

    // When z values are big, sum becomes infinity, and ret thus has a bunch of 0 and nan values.
    double sum = 0;
    for (size_t i = 0; i < K; i++)
    {
        sum += exp(z[i]);
    }

    for (size_t i = 0; i < K; i++)
    {
        ret.push_back(exp(z[i]) / sum);
    }

    return ret;
}

Vector d_vector_softmax(Vector z)
{
    Vector ret;
    size_t N = z.size();

    for (size_t j = 0; j < N; j++)
    {
        double sum = 0;
        for (size_t i = 0; i < N; i++)
        {
            sum += (i == j) ? z[i] * (1 - z[j]) : -z[j] * z[i];
        }
        ret.push_back(sum / N);
    }
    return ret;
}

ActivationFunction sigmoid(vector_sigmoid, d_vector_sigmoid);
ActivationFunction softmax(vector_softmax, d_vector_softmax);

} /* namespace mathutils */
