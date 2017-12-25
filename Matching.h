#pragma once
#include <boost/operators.hpp>

#include <iostream>
#include <string>
#include <vector>

struct Match : public boost::totally_ordered<Match> {
    size_t offset;
    std::string matchText;

    Match(size_t offset, const std::string& matchText);
    bool operator==(const Match& rhs) const;
    bool operator<(const Match& rhs) const;
    friend std::ostream& operator<<(std::ostream& os, const Match& m);
};

using Matches = std::vector<Match>;
std::ostream& operator<<(std::ostream& os, const Matches& ms);

using LineStarts = std::vector<size_t>;
inline std::ostream& operator<<(std::ostream& os, const LineStarts& lso);

struct MatchResult {
    LineStarts lineStarts;
    Matches matches;

    void add(const MatchResult& mr);
    void finalize();

    friend std::ostream& operator<<(std::ostream& os, const MatchResult& m);
};

MatchResult findMatches(const char* buffer, size_t bufferLen, const std::string& mask, size_t bufferStartOffset = 0);
