#include <cstdio>
#include <iostream>
#include <fstream>

#include "idxfile.h"

uint32_t encodeBytesAsLittleEndian(uint8_t* bytes)
{
    return bytes[3] << 24 | bytes[2] << 16 | bytes[1] << 8 | bytes[0];
}

uint32_t encodeBytesAsBigEndian(uint8_t* bytes)
{
    return bytes[0] << 24 | bytes[1] << 16 | bytes[2] << 8 | bytes[3];
}

idxfile::idxfile(const char* path) :
    path(path)
{
    try {
        std::ifstream input(path, std::ios_base::binary);

        // Skip 2 zero bytes, then read magic number info.
        input.seekg(2);
        this->dataType = input.get();
        this->dimension = input.get();
        this->dimSize = new uint32_t[dimension];

#ifdef DEBUG
        printf("\n");
#endif /* DEBUG */

        // Read the 4 byte dimension sizes into buffer byte by byte, then store them
        // in the appropriate 4 byte array element.
        uint8_t* buffer = new uint8_t[4];
        for (int i = 0; i < this->dimension; ++i)
        {
            input.read((char*)buffer, 4);
#ifdef DEBUG
            printf("idxfile@%p (%s): reading into dimSize[%d], bytes: ", this, this->path, i);
            printf("%02x %02x %02x %02x\n", buffer[0], buffer[1], buffer[2], buffer[3]);
#endif /* DEBUG */
            this->dimSize[i] = encodeBytesAsBigEndian(buffer);
        }
        delete[] buffer;

        // Get the length of the data in the file.
        this->dataLen = this->itemCount() * this->elemSize();

        // Allocate resources and read data.
        this->data = new uint8_t[this->dataLen];
        input.read((char*)(this->data), this->dataLen);

        input.close();
    } catch (std::ifstream::failure& e) {
        std::cerr << "Failed to open/read file: " << this->path << "\n";
    }
}

idxfile::~idxfile()
{
    delete this->dimSize;
    delete[] this->data;
}

/**
 * Get the count of data items in the file.
 */
uint32_t idxfile::itemCount() const
{
    return this->dimSize[0];
}

/**
 * Get the size of each element in the file.
 */
size_t idxfile::elemSize() const
{
    size_t ret = 1;
    for (int i = 1; i < this->dimension; ++i)
    {
        ret *= this->dimSize[i];
    }
    return ret;
}

/**
 * Print info about the .idx file.
 */
void idxfile::info() const
{
    printf("\ninfo: idxfile@%p (%s)\n", this, this->path);
    printf("magic:\t 0 0 %d %d\n", this->dataType, this->dimension);

    for (int i = 0; i < this->dimension; ++i)
    {
        printf("dimSize[%d]:\t%#x\t%d\n", i, this->dimSize[i], this->dimSize[i]);
    }

    printf("dataLen:\t%ld\n", this->dataLen);

    printf("------------------------------\n");
}

/**
 * Get a pointer to the beginning of the data item at no. pos.
 */
uint8_t* idxfile::dataAt(size_t pos) const
{
    return this->data + pos * this->elemSize();
}
