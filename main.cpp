#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

#include "idxfile.h"
#include "mathutils.h"
#include "neural.h"
#include "networkfile.h"

int main(int argc, char const *argv[])
{
    ImgFile* train_images = new ImgFile("data/train-images.idx3-ubyte");
    IdxFile* train_labels = new IdxFile("data/train-labels.idx1-ubyte");

    assert(train_images->itemCount() == train_labels->itemCount());

#ifdef DEBUG
    //train_images->info();
    //train_labels->info();
    //train_images->dump(0);

    std::vector<size_t> layer_sizes;
    layer_sizes.push_back(train_images->elemSize());
    layer_sizes.push_back(10);
    Network net(layer_sizes);

    for (size_t i = 0; i < 10; i++)
    {
        mathutils::Vector input_layer;
        for (size_t j = 0; j < train_images->elemSize(); ++j)
        {
            input_layer.push_back(train_images->data(i)[j] / 255.0);
        }

        uint8_t label = *train_labels->data(i);
        mathutils::Vector expected;
        for (size_t j = 0; j < 10; ++j)
        {
            expected.push_back(j == label ? 1.0 : 0.0);
        }

        net.nextIteration(input_layer, expected);
        net.finalize();
    }

    /*std::ios_base::openmode mode = std::ios::out | std::ios::binary;
    std::fstream stream("./net", mode);
    NetworkFile file(&stream, mode, net);
    file.doWrite();*/
    std::ios_base::openmode mode = std::ios::in | std::ios::binary;
    std::fstream stream("./net", mode);
    NetworkFile file(&stream, mode);
    try {
        file.doRead();
    } catch (NetworkFileError& err) {
        std::cout << "shit hit the fan\n";
    }
#endif /* DEBUG */

    delete train_images;
    delete train_labels;

    return 0;
}
