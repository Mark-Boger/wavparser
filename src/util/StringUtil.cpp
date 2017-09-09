#include "StringUtil.h"

std::string rad::getStringAfter(const std::string &inp,
                                     const std::string &delim)
{
    return inp.substr(inp.find(delim) + 1);
}

std::vector<std::string> rad::split(const std::string &inp, char delim)
{
    std::istringstream temp(inp);
    std::string t_buff;
    std::vector<std::string> tokens;

    while (std::getline(temp, t_buff, delim))
    {
        tokens.push_back(t_buff);
    }

    return tokens;
}

std::string rad::cleanString(const std::string &inp)
{
    std::string ret;
    std::remove_copy_if(inp.begin(), inp.end(), std::back_inserter(ret),
                        std::ptr_fun<int, int>(&std::ispunct));
    return ret;
}

std::string rad::uppercase(const std::string &inp)
{
    std::string ret;
    std::transform(inp.begin(), inp.end(), std::back_inserter(ret), toupper);
    return ret;
}