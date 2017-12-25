#include "Matching.h"

#include "Utils.h"

#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace {
    inline bool charMatches(char c, char maskChar)
    {
        return maskChar == '?' || c == maskChar;
    }
}

MatchResult findMatches(const char* buffer, size_t bufferLen, const std::string& mask, size_t bufferStartOffset)
{
    MatchResult result;
    if (!buffer || bufferLen == 0)
        return result;

    const size_t maskLen = mask.size();
    if (!maskLen)
        throw std::invalid_argument("Mask cannot be empty");

    for (size_t i = 0; i < bufferLen - maskLen + 1; ++i) {

        // handle newlines
        if (buffer[i] == '\n') {
            result.lineStarts.push_back(bufferStartOffset + i);
            continue;
        }

        // try to match mask from current position
        for (size_t j = 0; j < maskLen; ++j) {

            const auto c = buffer[i + j];
            if (!std::isprint(static_cast<unsigned char>(c)))
                break; // mask can only match printable symbols

            if (!charMatches(c, mask[j]))
                break; // no match

            if (j == maskLen - 1) { // match found!
                std::string matchText(buffer + i, buffer + i + maskLen);
                result.matches.push_back({bufferStartOffset + i, std::move(matchText)});
                i += j;
            }
        }
    }

    return result;
}

std::ostream& operator<<(std::ostream& os, const Matches& ms)
{
    return printContainer(os, ms);
}

std::ostream& operator<<(std::ostream& os, const LineStarts& lso)
{
    return printContainer(os, lso);
}

/// Match ///////////////////////////////////////////////////////////////////////////

Match::Match(size_t offset, const std::string& matchText)
    : offset(offset)
    , matchText(matchText)
{
}

bool Match::operator==(const Match& rhs) const
{
    return offset == rhs.offset &&
        matchText == rhs.matchText;
}

bool Match::operator<(const Match& rhs) const
{
    return offset < rhs.offset;
}

std::ostream& operator<<(std::ostream& os, const Match& m)
{
    os << "Match {ofs=" << m.offset << ",txt=" << m.matchText << "}";
    return os;
}

std::ostream& operator<<(std::ostream& os, const MatchResult& m)
{
    os << "{" << m.lineStarts << ";" << m.matches << "}";
    return os;
}

void MatchResult::add(const MatchResult& mr)
{
    lineStarts.insert(lineStarts.end(), mr.lineStarts.begin(), mr.lineStarts.end());
    matches.insert(matches.end(), mr.matches.begin(), mr.matches.end());
}

// before using results, sort and uniquefy them
// this is needed because we use vectors instead of sets
// vectors were preferred because of memory effectiveness
void MatchResult::finalize()
{
    std::sort(lineStarts.begin(), lineStarts.end());
    std::unique(lineStarts.begin(), lineStarts.end());

    std::sort(matches.begin(), matches.end());
    std::unique(matches.begin(), matches.end());
}
