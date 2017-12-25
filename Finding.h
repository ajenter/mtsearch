#pragma once
#include "Matching.h"

#include <string>
#include <vector>

struct ResolvedMatch {
    size_t line, column;
    std::string matchText;
};

using FindResult = std::vector<ResolvedMatch>;

FindResult findMatchesInFile(const std::string& filename, const std::string& mask);
