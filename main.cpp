#include <iostream>
#include <cassert>
#include <cstdlib>
#include <ctime>

#include "idxfile.h"
#include "mathutils.h"
#include "neural.h"

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    imgfile* trainImages = new imgfile("data/train-images.idx3-ubyte");
    idxfile* trainLabels = new idxfile("data/train-labels.idx1-ubyte");

    assert(trainImages->itemCount() == trainLabels->itemCount());

#ifdef DEBUG
    trainImages->info();
    trainLabels->info();
    trainImages->dump(3254);
#endif /* DEBUG */

    delete trainImages;
    delete trainLabels;

#ifdef DEBUG
    size_t layerSizes[3] = {4,3,2};
    Network net(3, layerSizes);
    net.info();
#endif

    return 0;
}
