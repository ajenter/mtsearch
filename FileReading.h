#pragma once
#include <iostream>
#include <string>
#include <vector>

using Buffer = std::vector<char>;

size_t getFileSize(const std::string& filename);

Buffer readFile(const std::string& filename, size_t offset, size_t length);
