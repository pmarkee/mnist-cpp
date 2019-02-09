#ifndef IMGFILE_H
#define IMGFILE_H

#include "idxfile.h"

class imgfile : public idxfile
{
public:
    imgfile(const char* path);
    ~imgfile();

    void dump(size_t pos) const;
};

#endif /* IMGFILE_H */
