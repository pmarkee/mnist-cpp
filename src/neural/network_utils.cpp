#include "neural.h"
#include "mathutils.h"

// TODO it would be better to pass const Network& in these functions,
// but it causes some compile errors.

mathutils::Vector deltaC_deltaZ(Network net) {
    size_t L = net.layerCount() - 1;
    std::vector<mathutils::Vector> a = net.layers();
    // std::vector<mathutils::Vector> z = net.zLayers();
    mathutils::VectorFunction d_act = net.d_act();
    mathutils::Vector y = net.expected();

    return schurProduct((a[L] - y) * 2, mathutils::d_vector_sigmoid(a[L]));
}

mathutils::Vector deltaZ_deltaW(Network net, size_t L)
{
    std::vector<mathutils::Vector> a = net.layers();

    return a[L-1];
}

mathutils::Vector deltaZ_deltaB(Network net, size_t L)
{
    mathutils::Vector ret;

    std::vector<mathutils::Vector> z = net.zLayers();

    for (int i = 0; i < z[L].size(); i++)
    {
        ret.push_back(1);
    }

    return ret;
}

mathutils::Matrix deltaZ_deltaZ(Network net, size_t L)
{
    mathutils::Matrix ret;

    std::vector<mathutils::Vector> z = net.zLayers();
    std::vector<mathutils::Matrix> w = net.weights();
    mathutils::VectorFunction d_act = net.d_act();

    return transpose(schurProduct(transpose(w[L-1]), mathutils::d_vector_sigmoid(z[L-1])));
}
