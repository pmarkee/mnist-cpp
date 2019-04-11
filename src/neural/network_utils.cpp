#include "neural.h"
#include "mathutils.h"

// TODO it would be better to pass const Network& in these functions,
// but it causes some compile errors.

mathutils::Vector deltaC_deltaA(Network net) {
    mathutils::Vector ret;

    size_t L = net.layerCount() - 1;
    std::vector<mathutils::Vector> a = net.getLayers();
    std::vector<mathutils::Vector> z = net.getNonSigmoidLayers();
    mathutils::Vector y = net.getExpected();

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

    std::vector<mathutils::Vector> a = net.getLayers();
    std::vector<mathutils::Vector> z = net.getNonSigmoidLayers();
    std::vector<mathutils::Matrix> w = net.getWeights();
    mathutils::numericFunction d_act = net.getActivationFunctionDerivative();

    for (int j = 0; j < w[L-1].size(); j++) {
        mathutils::Vector vec;
        for (int k = 0; k < w[L-1][j].size(); k++) {
            vec.push_back(d_act(z[L][j]) * a[L-1][k]);
        }
        ret.push_back(vec);
    }

    return ret;
}

mathutils::Vector deltaA_deltaB(Network net, size_t L)
{
    mathutils::Vector ret;

    std::vector<mathutils::Vector> z = net.getNonSigmoidLayers();
    std::vector<mathutils::Vector> b = net.getBiases();
    mathutils::numericFunction d_act = net.getActivationFunctionDerivative();

    for (int j = 0; j < b[L-1].size(); j++) {
        ret.push_back(d_act(z[L][j]));
    }

    return ret;
}

mathutils::Matrix deltaA_deltaA(Network net, size_t L)
{
    mathutils::Matrix ret;

    std::vector<mathutils::Vector> a = net.getLayers();
    std::vector<mathutils::Vector> z = net.getNonSigmoidLayers();
    std::vector<mathutils::Matrix> w = net.getWeights();
    mathutils::numericFunction d_act = net.getActivationFunctionDerivative();

    for (int j = 0; j < a[L].size(); j++) {
        mathutils::Vector vec;
        for (int k = 0; k < a[L-1].size(); k++) {
            vec.push_back(d_act(z[L][j]) * w[L-1][j][k]);
        }
        ret.push_back(vec);
    }

    return ret;
}
