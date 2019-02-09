#ifndef IDXFILE_H
#define IDXFILE_H

#include <cstdint>

class idxfile
{
protected:
    const char* path;
    uint8_t dataType;
    uint8_t dimension;
    uint32_t* dimSize;
    uint8_t* data;
    size_t dataLen;
public:
    idxfile(const char* path);
    ~idxfile();

    uint32_t itemCount() const;
    size_t elemSize() const;

    void info() const;
    uint8_t* dataAt(size_t pos) const;
};

#endif /* IDXFILE_H */
