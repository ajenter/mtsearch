#include "FileReading.h"

#include <boost/filesystem.hpp>

#include <iostream>
#include <stdexcept>

namespace fs = boost::filesystem;
using namespace std;

size_t getFileSize(const std::string& filename)
{
    fs::path p(filename);
    if (!fs::exists(p) || !fs::is_regular_file(p))
        throw std::runtime_error("File doesn't exist");

    return fs::file_size(p);
}

Buffer readFile(const std::string& filename, size_t offset, size_t length)
{
    std::ifstream ifs;
    ifs.exceptions(ios::badbit | ios::failbit);

    ifs.open(filename, ios::in | ios::binary);

    Buffer b;
    b.resize(length);

    ifs.seekg(ifstream::off_type(offset), ios::beg);
    ifs.read(b.data(), ifstream::off_type(length));

    return b;
}
