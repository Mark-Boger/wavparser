#ifndef RAD_WAVEX_H
#define RAD_WAVEX_H

#include <iostream>
#include <exception>
#include <stdexcept>
#include <string>
#include <sstream>
#include <map>

enum WAVFileError
{
    InvalidHeaderID,
    InvalidHeaderFormat,
    InvalidFmtID,
    InvalidFmtFormat,
    InvalidDataFormat
};

class ErrorHelper
{
  public:
    static std::string findError(WAVFileError err)
    {
        return ErrorMap.find(err)->second;
    }

  private:
    static const std::map<WAVFileError, std::string> ErrorMap;
};

const std::map<WAVFileError, std::string> ErrorHelper::ErrorMap = {
        {InvalidHeaderFormat, "Invalid Format in header"},
        {InvalidHeaderID, "Invalid ID in header"},
        {InvalidFmtID, "Invalid ID in fmt subchunk"},
        {InvalidFmtFormat, "Invalid Format in fmt subchunk"},
        {InvalidDataFormat, "Invalid ID in data subchunk"}
};

class InvalidFileException : public std::runtime_error
{

  public:
    InvalidFileException(WAVFileError err)
        : runtime_error(ErrorHelper::findError(err)){};
};

#endif