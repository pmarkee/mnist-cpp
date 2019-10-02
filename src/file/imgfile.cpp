#include <cstdio>

#include "idxfile.h"

ImgFile::ImgFile(const char* path) :
    IdxFile(path)
{
}

ImgFile::~ImgFile()
{
}

/**
 * Dump image no. pos from the file.
 */
void ImgFile::dump(size_t pos) const
{
    const uint8_t* const img = this->data(pos);
    for (int i = 0; i < this->dimSize[1]; ++i)
    {
        for (int j = 0; j < this->dimSize[2]; ++j)
        {
            printf("%4d", img[this->dimSize[1] * i + j]);
        }
        printf("\n");
    }
}
