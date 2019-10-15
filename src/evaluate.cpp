#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>

#include "idxfile.h"
#include "mathutils.h"
#include "networkfile.h"
#include "neural.h"

int main()
{
    ImgFile test_images("data/t10k-images.idx3-ubyte");
    IdxFile test_labels("data/t10k-labels.idx1-ubyte");

    assert(test_images.itemCount() == test_labels.itemCount());
    size_t item_count = test_images.itemCount();

    std::ios_base::openmode mode = std::ios::in | std::ios::binary;
    std::fstream stream("./net", mode);
    NetworkFile file(&stream, mode);
    file.doRead();
    Network net(file, mathutils::softmax);
    // net.info();

    for (size_t i = 0; i < 1; i++)
    {
        mathutils::Vector input_layer;
        for (size_t j = 0; j < test_images.elemSize(); ++j)
        {
            input_layer.push_back(test_images.data(i)[j] / 255.0);
        }

        uint8_t label = *(test_labels.data(i));
        mathutils::Vector expected;
        for (size_t j = 0; j < 10; ++j)
        {
            expected.push_back(j == label ? 1.0 : 0.0);
        }

        net.nextIteration(input_layer, expected);

        mathutils::Vector output = net.outputLayer();
        std::cout << expected;
        std::cout << output;
    }

    return 0;
}
