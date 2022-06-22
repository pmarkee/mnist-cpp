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

    double loss_sum = 0.0;
    size_t correct = 0;
    for (size_t i = 0; i < item_count; i++)
    {
        // test_images.dump(i);
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

        mathutils::Vector output = net.predict(input_layer);
        size_t prediction = vector_argmax(output);
        if (prediction == (size_t) label) {
            correct++;
        }

        // std::cout << "prediction: " << prediction << "\n";
        // std::cout << "label: " << (size_t) label << "\n";

        loss_sum += net.loss();
    }

    // std::cout << "item count: " << item_count << "\n";
    // std::cout << "correct: " << correct << "\n";
    std::cout << "accuracy: " << 1.0 * correct / item_count << "\n";
    // std::cout << "average loss: " << loss_sum / item_count << "\n";

    return 0;
}
