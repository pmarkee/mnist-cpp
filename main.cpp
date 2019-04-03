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
    trainImages->dump(0);
#endif /* DEBUG */

    delete trainImages;
    delete trainLabels;

#ifdef DEBUG
    size_t layerSizes[2] = {400, 10};
    Network net(2, layerSizes);
    // net.info();

    net.nextIteration(expected);
    // net.info();
#endif /* DEBUG */

    return 0;
}
