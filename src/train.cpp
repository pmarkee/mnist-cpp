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
    ImgFile train_images("data/train-images.idx3-ubyte");
    IdxFile train_labels("data/train-labels.idx1-ubyte");

    assert(train_labels.itemCount() == train_images.itemCount());
    size_t item_count = train_images.itemCount();

    printf("item count: %ld\n", item_count);

    std::vector<size_t> layer_sizes;
    layer_sizes.push_back(train_images.elemSize());
    layer_sizes.push_back(100);
    layer_sizes.push_back(10);
    Network net(layer_sizes, mathutils::softmax);

    size_t batch_size = 100;
    assert(item_count >= batch_size);
    size_t batch_number = item_count / batch_size;

    printf("batch number: %ld\n", batch_number);

    for (size_t batch = 0; batch < batch_number; ++batch)
    {
        for (size_t i = batch * batch_size; i < (batch + 1) * batch_size; ++i)
        {
            mathutils::Vector input_layer;
            for (size_t j = 0; j < train_images.elemSize(); ++j)
            {
                input_layer.push_back(train_images.data(i)[j] / 255.0);
            }

            uint8_t label = *(train_labels.data(i));
            mathutils::Vector expected;
            for (size_t j = 0; j < 10; ++j)
            {
                expected.push_back(j == label ? 1.0 : 0.0);
            }

            net.nextIteration(input_layer, expected);
        }
        net.finalize();
        
        if (batch % (batch_number / 100) == 0) {
            std::cout << "batch " << batch << "\n";
        }
    }

    std::ios_base::openmode mode = std::ios::out | std::ios::binary;
    std::fstream stream("./net", mode);
    NetworkFile file(&stream, mode, net);
    file.doWrite();

    return 0;
}
