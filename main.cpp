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
    imgfile* trainImages = new imgfile("data/train-images.idx3-ubyte");
    idxfile* trainLabels = new idxfile("data/train-labels.idx1-ubyte");

    assert(trainImages->itemCount() == trainLabels->itemCount());

#ifdef DEBUG
    //trainImages->info();
    //trainLabels->info();
    //trainImages->dump(0);

    std::vector<size_t> layerSizes;
    layerSizes.push_back(trainImages->elemSize());
    layerSizes.push_back(10);
    Network net(layerSizes);

    for (size_t i = 0; i < 10; i++)
    {
        mathutils::Vector inputLayer;
        for (size_t j = 0; j < trainImages->elemSize(); ++j)
        {
            inputLayer.push_back(trainImages->dataAt(i)[j] / 255.0);
        }

        uint8_t label = *trainLabels->dataAt(i);
        mathutils::Vector expected;
        for (size_t j = 0; j < 10; ++j)
        {
            expected.push_back(j == label ? 1.0 : 0.0);
        }

        net.nextIteration(inputLayer, expected);
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

    delete trainImages;
    delete trainLabels;

    return 0;
}
