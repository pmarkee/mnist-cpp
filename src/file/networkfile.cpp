#include "networkfile.h"
#include "neural.h"

#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>

// TODO auto-create new filename based on datetime
NetworkFile::NetworkFile(std::fstream* const stream, const std::ios_base::openmode& mode)
    : stream(stream)
    , mode(mode)
    , hasLoaded_(false)
{
    assert(mode & (std::ios::binary));
}

NetworkFile::NetworkFile(std::fstream* const stream, const std::ios_base::openmode& mode, const Network& net)
    : stream(stream)
    , mode(mode)
    , layerCount_(net.layerCount())
    , layerSizes_(net.layerSizes())
    , wMatrix_(net.weights())
    , bVector_(net.biases())
    , hasLoaded_(true)
{
    assert(mode & (std::ios::binary));
}

NetworkFile::~NetworkFile()
{
    if (this->stream->is_open()) {
        *this->stream << std::flush;
    }
}

const uint8_t NetworkFile::layerCount() const
{
    return this->layerCount_;
}

const std::vector<size_t> NetworkFile::layerSizes() const
{
    return this->layerSizes_;
}

const std::vector<mathutils::Matrix> NetworkFile::wMatrix() const
{
    return this->wMatrix_;
}

const std::vector<mathutils::Vector> NetworkFile::bVector() const
{
    return this->bVector_;
}

bool NetworkFile::hasLoaded() const
{
    return this->hasLoaded_;
}

void NetworkFile::doRead()
{
    assert(this->stream->is_open());
    assert(this->mode & (std::ios::in));

    if (this->hasLoaded_)
    {
        char error_message[255];
        sprintf(error_message, "NetworkFile object %p refusing to read, as it already has a network loaded.\n", this);
        throw NetworkFileError(error_message);
    }

    // Erase all loaded data.
    // TODO is this necessary with the above if block in place?
    this->layerSizes_.clear();
    this->wMatrix_.clear();
    this->bVector_.clear();

#ifdef DEBUG
    printf("NetworkFile %p starting to read network\n", this);
    printf("---------------------------------------\n");
#endif /* DEBUG */

    // 1. the count of the layers -> 1 byte, + 1 byte of padding.
    this->stream->read((char*)&this->layerCount_, sizeof(uint8_t));
    this->readPadding(1);

#ifdef DEBUG
    printf("layerCount: %d\n", this->layerCount_);
    printf("pos: %#x\n\n", this->stream->tellg());
#endif /* DEBUG */

    // this->layerSizes.reserve(this->layerCount);
    // this->wMatrix.reserve(this->layerCount - 1);
    // this->bVector.reserve(this->layerCount - 1);

    // 2. sizes of each layer -> layerCount * 8 bytes, + padded to 16 bytes.
    void* tmp = calloc(8, sizeof(uint8_t));
    for (size_t i = 0; i < this->layerCount_; i++)
    {
        this->stream->read((char*)tmp, sizeof(size_t));
        this->layerSizes_.push_back(*(size_t*)tmp);
    }
    this->readPadding(0x10 - this->stream->tellg() % 0x10);

#ifdef DEBUG
    for (size_t i = 0; i < this->layerSizes_.size(); i++)
    {
        printf("layerSizes_[%d] == %d\n", i, this->layerSizes_[i]);
    }
    printf("pos: %#x\n\n", this->stream->tellg());
#endif /* DEBUG */

    /** 3. Read each weight matrix as follows:
     * Read the weight matrix ROW BY ROW.
     * Each weight is a double value -> 8 bytes.
     * The end of this section is padded to 16 bytes.
     */
    for (size_t i = 0; i < this->layerCount_ - 1; i++)
    {
        size_t num_rows = this->layerSizes_[i + 1];
        size_t num_cols = this->layerSizes_[i];

        mathutils::Matrix mat;
        for (size_t row = 0; row < num_rows; row++)
        {
            mathutils::Vector vec;
            for (size_t col = 0; col < num_cols; col++)
            {
                // printf("%ld - %ld - %ld\n", i, row, col);
                this->stream->read((char*)tmp, sizeof(double));
                vec.push_back(*(double*)tmp);
            }
            mat.push_back(vec);
        }
        this->wMatrix_.push_back(mat);
    }
    this->readPadding(0x10 - this->stream->tellg() % 0x10);

#ifdef DEBUG
    printf("read %d weight matrices\n", this->wMatrix_.size());
    printf("pos: %#x\n\n", this->stream->tellg());
#endif /* DEBUG */

    /** 4. Read the bias vectors as follows:
     * Each bias is a double value -> 8 bytes.
     * The end of this section is padded to 16 bytes.
     */
    for (size_t i = 0; i < this->layerCount_ - 1; i++)
    {
        size_t num_rows = this->layerSizes_[i + 1];
        mathutils::Vector vec;
        for (size_t row = 0; row < num_rows; row++)
        {
            // printf("%ld - %ld\n", i, row);
            this->stream->read((char*)tmp, sizeof(double));
            vec.push_back(*(double*)tmp);
        }
        this->bVector_.push_back(vec);
    }
    this->readPadding(0x10 - this->stream->tellg() % 0x10);

    size_t pos = this->stream->tellg();
    this->stream->seekg(0, std::ios::end);
    size_t size = this->stream->tellg();
#ifdef DEBUG
    printf("read %d bias vectors\n", this->bVector_.size());
    printf("pos: %#x\n\n", pos);
    printf("size: %#x\n", size);
    printf("size - pos: %#x\n", size - pos);
#endif /* DEBUG */

    // We should have reached EOF at this point. If we have not, throw an error.
    // NOTE: this->stream->eof() returns false even if size - pos == 0,
    // because at that point we haven't actually read the eof "character".
    if (size - pos != 0)
    {
        char error_message[255];
        sprintf(error_message, "NetworkFile %p has not reached EOF", this);
        throw NetworkFileError(error_message);
    }

    this->hasLoaded_ = true;
}

void NetworkFile::doWrite()
{
    assert(this->stream->is_open());
    assert(this->mode & (std::ios::out | std::ios::app | std::ios::trunc));

    // 1. the count of the layers -> 1 byte, + 1 byte of padding.
    this->stream->write((char*)&this->layerCount_, 2 * sizeof(uint8_t));

    // 2. sizes of each layer -> layerCount_ * 8 bytes, + padded to 16 bytes.
    for (int i = 0; i < layerSizes_.size(); i++) {
        this->stream->write((char*)&this->layerSizes_[i], sizeof(size_t));
    }
    this->writePadding(0x10 - this->stream->tellp() % 0x10);

    /** 3. Write each weight matrix as follows:
     * Write the weight matrix ROW BY ROW.
     * Each weight is a double value -> 8 bytes.
     * The end of this section is padded to 16 bytes.
     */
    for (size_t i = 0; i < wMatrix_.size(); i++)
    {
        size_t num_rows = wMatrix_[i].size();
        for (size_t row = 0; row < num_rows; row++)
        {
            size_t num_cols = wMatrix_[i][row].size();
            for (size_t col = 0; col < num_cols; col++)
            {
                // printf("%ld - %ld - %ld\n", i, row, col);
                this->stream->write((char*)&this->wMatrix_[i][row][col], sizeof(double));
            }
        }
    }
    this->writePadding(0x10 - this->stream->tellp() % 0x10);

    /** 4. Write the bias vectors as follows:
     * Each bias is a double value -> 8 bytes.
     * The end of this section is padded to 16 bytes.
     */
    for (size_t i = 0; i < bVector_.size(); i++)
    {
        size_t num_rows = bVector_[i].size();
        for (size_t row = 0; row < num_rows; row++)
        {
            // printf("%ld - %ld\n", i, row);
            this->stream->write((char*)&this->bVector_[i][row], sizeof(double));
        }
    }
    this->writePadding(0x10 - this->stream->tellp() % 0x10);
}

void NetworkFile::readPadding(size_t padding_size)
{
    // We don't need the actual values of paddings, so seek ahead instead of reading.
    this->stream->seekg(padding_size, std::ios_base::cur);
}

void NetworkFile::writePadding(size_t padding_size)
{
    uint8_t padding[padding_size] = { 0 };
    this->stream->write((char*)padding, padding_size * sizeof(uint8_t));
}

NetworkFileError::NetworkFileError(const std::string& msg)
    : msg(msg)
{
}
