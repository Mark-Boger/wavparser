#ifndef RAD_WAVFILE_H
#define RAD_WAVFILE_H

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <bitset>

struct Chunk
{
    uint8_t id[4];
    uint32_t chunkSize;
};

struct Header : Chunk
{
    uint8_t format[4];
};

struct Fmt : Chunk
{
    uint16_t format;
    uint16_t channels;
    uint32_t sampleRate;
    uint32_t byteRate;
    uint16_t blockAlign;
    uint16_t bitsPerSample;
};

struct Data : Chunk
{
};

struct WAVFile
{
    Header header;
    Fmt fmt;
    Data data;

    std::vector<char> rawData;

    void write(std::ostream &os)
    {
        os.write(reinterpret_cast<char *>(&header), sizeof header);
        os.write(reinterpret_cast<char *>(&fmt), sizeof fmt);
        os.write(reinterpret_cast<char *>(&data), sizeof data);
        os.write(reinterpret_cast<char *>(&rawData[0]), rawData.size());
    }

    template<typename T>
    void writeData(std::ostream &os)
    {
        std::vector<T> right;
        std::vector<T> left;

        auto is_right = 0;
        for(auto i = 0; i < rawData.size(); i+=(fmt.blockAlign / fmt.channels))
        {
            if(is_right % 2 == 0)
            {
                left.push_back(*reinterpret_cast<T*>(&rawData[i]));
            }
            else
            {
                right.push_back(*reinterpret_cast<T*>(&rawData[i]));
            }

            is_right++;
        }

        double t_start = 0.0;
        double t_delta = 1. / fmt.sampleRate;
        double max     = maxNum(fmt.bitsPerSample);

        for(auto i = 0; i < right.size(); i++)
        {
            os << std::setw(11) << t_start; 
            os << std::setw(15) << (left[i] / max);
            os << std::setw(15) << (right[i] / max) << "\n";

            t_start += t_delta;
        }
    }

    double maxNum(int bits)
    {
        return (((unsigned) - 1) >> (33 - bits)) * 1.;
    }

    static std::string toString(uint8_t *bytes, unsigned int size = 4)
    {
        return std::string{(char *)bytes, size};
    }
};

#endif
