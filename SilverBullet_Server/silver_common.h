#ifndef _COMMON_H_
#define _COMMON_H_


std::string trim(std::string const& str)
{
    std::size_t first = str.find_first_not_of(' ');

    // If there is no non-whitespace character, both first and last will be std::string::npos (-1)
    // There is no point in checking both, since if either doesn't work, the
    // other won't work, either.
    if(first == std::string::npos)
        return "";

    std::size_t last  = str.find_last_not_of(' ');

    return str.substr(first, last-first+1);
}

#endif
