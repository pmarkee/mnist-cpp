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
    uint8_t layerCount;
    std::vector<size_t> layerSizes;
    std::vector<mathutils::Matrix> wMatrix;
    std::vector<mathutils::Vector> bVector;
    bool hasLoadedNet;

    void readPadding(size_t padding_size);
    void writePadding(size_t padding_size);
public:
    NetworkFile(std::fstream* const stream,
                const std::ios_base::openmode& mode);
    NetworkFile(std::fstream* const stream,
                const std::ios_base::openmode& mode,
                const Network& net);
    ~NetworkFile();

    const uint8_t getLayerCount() const;
    const std::vector<size_t> getLayerSizes() const;
    const std::vector<mathutils::Matrix> getWMatrix() const;
    const std::vector<mathutils::Vector> getBVector() const;
    bool hasLoaded() const;

    void setLayerCount(size_t layerCount);
    void setLayerSizes(const std::vector<size_t>& layerSizes);
    void setWMatrix(const std::vector<mathutils::Matrix>& w);
    void setBVector(const std::vector<mathutils::Vector>& b);

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
