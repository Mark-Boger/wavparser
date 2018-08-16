#include "WAVParser.h"

WAVFile WAVParser::parse(std::istream &inp)
{
    auto header = Parse<Header>(inp);
    auto fmt    = Parse<Fmt>(inp);
    auto data   = Parse<Data>(inp);
    auto raw    = ParseRaw(inp, data.chunkSize);
    
    return WAVFile{header, fmt, data, raw};
}

std::vector<char> WAVParser::ParseRaw(std::istream &inp, uint32_t size)
{
    std::vector<char> raw;

    raw.resize(size);
    inp.read(reinterpret_cast<char *>(&raw[0]), size);

    return raw;
}

void WAVParser::Validate(Header header)
{
    ValidateRes(WAVFile::toString(header.id), "RIFF", InvalidHeaderID);
    ValidateRes(WAVFile::toString(header.format), "WAVE", InvalidHeaderFormat);
}

void WAVParser::Validate(Fmt fmt)
{
    ValidateRes(WAVFile::toString(fmt.id), "fmt ", InvalidFmtID);
    ValidateRes(fmt.format, 1, InvalidFmtFormat);
}

void WAVParser::Validate(Data data)
{
    ValidateRes(WAVFile::toString(data.id), "data", InvalidDataFormat);
}
