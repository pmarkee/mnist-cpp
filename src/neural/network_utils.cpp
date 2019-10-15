#include "neural.h"
#include "mathutils.h"

// TODO it would be better to pass const Network& in these functions,
// but it causes some compile errors.

mathutils::Vector deltaC_deltaA(Network net) {
    mathutils::Vector ret;

    size_t L = net.layerCount() - 1;
    std::vector<mathutils::Vector> a = net.layers();
    std::vector<mathutils::Vector> z = net.zLayers();
    mathutils::Vector y = net.expected();

    for (int j = 0; j < a[L].size(); j++) {
        ret.push_back(2 * (a[L][j] - y[j]));
    }

    return ret;
}

// TODO use deltaZ_deltaW and deltaZ_deltaB instead (and deltaZ instead of deltaA in Network).
// That way the derivative of z[L] is only calculated once instead of twice.
mathutils::Matrix deltaA_deltaW(Network net, size_t L)
{
    mathutils::Matrix ret;

    std::vector<mathutils::Vector> a = net.layers();
    std::vector<mathutils::Vector> z = net.zLayers();
    std::vector<mathutils::Matrix> w = net.weights();
    mathutils::VectorFunction d_act = net.d_act();

    return d_act(z[L]) * a[L-1];
}

mathutils::Vector deltaA_deltaB(Network net, size_t L)
{
    std::vector<mathutils::Vector> z = net.zLayers();
    mathutils::VectorFunction d_act = net.d_act();

    return d_act(z[L]);
}

mathutils::Matrix deltaA_deltaA(Network net, size_t L)
{
    mathutils::Matrix ret;

    std::vector<mathutils::Vector> z = net.zLayers();
    std::vector<mathutils::Matrix> w = net.weights();
    mathutils::VectorFunction d_act = net.d_act();

    return schurProduct(w[L-1], d_act(z[L]));
}
