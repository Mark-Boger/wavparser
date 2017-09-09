#ifndef RAD_STRUTIL_H
#define RAD_STRUTIL_H

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

namespace rad
{
    inline void ltrim(std::string &s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
                    return !std::isspace(ch);
                }));
    }

    inline void rtrim(std::string &s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
                    return !std::isspace(ch);
                }).base(),
                s.end());
    }

    inline void trim(std::string &s)
    {
        ltrim(s);
        rtrim(s);
    }

    std::string uppercase(const std::string &inp);
    std::string cleanString(const std::string &inp);
    std::vector<std::string> split(const std::string &inp, char delim);
    std::string getStringAfter(const std::string &inp, const std::string &delim);
};

#endif