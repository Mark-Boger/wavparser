#ifndef RAD_WAVP_H
#define RAD_WAVP_H

#include <iostream>
#include <string>
#include <stdexcept>

#include "WAVExceptions.h"
#include "WAVFile.h"

class WAVParser
{
  public:
    WAVFile parse(std::istream &inp);

  private:
    template <typename T>
    T Parse(std::istream &inp)
    {
        T chunk = ParseChunk<T>(inp);
        Validate(chunk);
        return chunk;
    }

    template <typename T>
    T ParseChunk(std::istream &inp)
    {
        T chunk;
        inp.read(reinterpret_cast<char *>(&chunk), sizeof chunk);
        return chunk;
    }

    std::vector<char> ParseRaw(std::istream &inp, uint32_t size);

    void Validate(Header header);
    void Validate(Fmt fmt);
    void Validate(Data data);

    template <typename T, typename E>
    void ValidateRes(T res, E exp, WAVFileError err)
    {
        if (res != exp)
            throw InvalidFileException(err);
    }
};

#endif