#ifndef IDXFILE_H
#define IDXFILE_H

#include <cstdint>

class IdxFile
{
protected:
    const char* path;
    uint8_t dataType;
    uint8_t dimension;
    uint32_t* dimSize;
    uint8_t* data_;
    size_t dataLen;
public:
    IdxFile(const char* path);
    ~IdxFile();

    uint32_t itemCount() const;
    size_t elemSize() const;

    void info() const;
    const uint8_t* const data(size_t pos) const;
};

class ImgFile : public IdxFile
{
public:
    ImgFile(const char* path);
    ~ImgFile();

    void dump(size_t pos) const;
};

#endif /* IDXFILE_H */
