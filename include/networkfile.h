#ifndef NETWORKFILE_H
#define NETWORKFILE_H

#include "mathutils.h"
#include "neural.h"

#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

class Network;

class NetworkFile {
private:
    std::string path;
    std::fstream* stream;
    std::ios_base::openmode mode;

    uint8_t layerCount_;
    std::vector<size_t> layerSizes_;
    std::vector<mathutils::Matrix> wMatrix_;
    std::vector<mathutils::Vector> bVector_;
    bool hasLoaded_;

    void readPadding(size_t padding_size);
    void writePadding(size_t padding_size);
public:
    NetworkFile(std::fstream* const stream,
                const std::ios_base::openmode& mode);
    NetworkFile(std::fstream* const stream,
                const std::ios_base::openmode& mode,
                const Network& net);
    ~NetworkFile();

    const uint8_t layerCount() const;
    const std::vector<size_t> layerSizes() const;
    const std::vector<mathutils::Matrix> wMatrix() const;
    const std::vector<mathutils::Vector> bVector() const;
    bool hasLoaded() const;

    void doRead();
    void doWrite();
};

class NetworkFileError {
private:
    std::string msg;
public:
    NetworkFileError(const std::string& msg);
};

#endif /* NETWORKFILE_H */
