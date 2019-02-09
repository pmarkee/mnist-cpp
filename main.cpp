#include <iostream>
#include <cassert>

#include "idxfile.h"
#include "imgfile.h"

#include "mathutils.h"

int main(int argc, char const *argv[])
{
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
    return 0;
}
