#ifndef COMMON_IO_H
#define COMMON_IO_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace helper
{

    class io
    {
    public:
        static std::vector<char> readFile (const std::string &filename);

    };

}
#endif
