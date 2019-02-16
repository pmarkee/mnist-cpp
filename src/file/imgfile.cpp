#include <cstdio>

#include "idxfile.h"

imgfile::imgfile(const char* path) :
    idxfile(path)
{
}

imgfile::~imgfile()
{
}

/**
 * Dump image no. pos from the file.
 */
void imgfile::dump(size_t pos) const
{
    uint8_t* img = this->dataAt(pos);
    for (int i = 0; i < this->dimSize[1]; ++i)
    {
        for (int j = 0; j < this->dimSize[2]; ++j)
        {
            printf("%4d", img[this->dimSize[1] * i + j]);
        }
        printf("\n");
    }
}
