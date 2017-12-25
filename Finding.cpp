#include "Finding.h"

#include "FileReading.h"
#include "Matching.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

struct Chunk : public boost::totally_ordered<Chunk> {

    size_t start, length;

    Chunk(size_t start, size_t length)
        : start(start)
        , length(length)
    {
    }

    bool operator<(const Chunk& rhs) const
    {
        return start < rhs.start ||
            (start == rhs.start && length < rhs.length);
    }

    bool operator==(const Chunk& rhs) const
    {
        return start == rhs.start && length == rhs.length;
    }

    friend std::ostream& operator<<(std::ostream& os, const Chunk& ch)
    {
        os << "{" << ch.start << ", " << ch.length << "}";
        return os;
    }
};

using Chunks = std::vector<Chunk>;

std::ostream& operator<<(std::ostream& os, const Chunks& chs)
{
    os << "Chunks[" << chs.size() << "]{";
    for (auto ch : chs)
        os << ch << ";";
    os << "}";
    return os;
}

Chunks splitIntoOverlappingChunks(size_t totalSize, size_t chunkLen, size_t overlap)
{
    if (overlap > chunkLen / 2)
        throw std::invalid_argument("Overlap must be smaller than half of chunk size");

    Chunks result;
    for (size_t i = 0; i < totalSize; i += chunkLen - overlap) {
        if (i + chunkLen > totalSize) {
            if (totalSize - i > overlap || totalSize == overlap)
                result.push_back(Chunk{i, totalSize - i});
            break;
        }
        result.push_back({i, chunkLen});
    }

    return result;
}

// converts absolute offset-based matches into line:column matches
FindResult resolveMatches(MatchResult&& matchResult)
{
    matchResult.finalize();

    FindResult fr;

    auto linesIt = matchResult.lineStarts.cbegin();
    size_t line = 1, lineStart = 0;
    for (auto mr : matchResult.matches) {
        while (linesIt != matchResult.lineStarts.end() && mr.offset > *linesIt) {
            lineStart = *linesIt;
            ++linesIt;
            ++line;
        }
        auto column = size_t(mr.offset - lineStart);
        fr.push_back({line, column, mr.matchText});
    }

    return fr;
}

MatchResult processChunk(const std::string& filename, const std::string& mask, const Chunk& chunk)
{
    auto contents = readFile(filename, chunk.start, chunk.length);
    return findMatches(contents.data(), contents.size(), mask, chunk.start);
}


////////////////////////////////////////////////////////////////////////////////////////////////

FindResult findMatchesInFile(const std::string& filename, const std::string& mask)
{
    using namespace std;

    const size_t chunkSize = 10000000;
    const auto filesize = getFileSize(filename);
    const auto chunks = splitIntoOverlappingChunks(filesize, chunkSize, mask.length());

    size_t lastDispatchedChunk = 0;

    auto makeFuture = [&] {
        assert(lastDispatchedChunk < chunks.size());
        return async(launch::async, bind(processChunk, filename, mask, chunks[lastDispatchedChunk++])).share();
    };

    // create futures
    const size_t numThreads = min(size_t(thread::hardware_concurrency()), chunks.size());
    vector<shared_future<MatchResult>> futures;
    generate_n(back_inserter(futures), numThreads, makeFuture);

    auto result = MatchResult();

    // kick off work and process all chunks
    while (lastDispatchedChunk < chunks.size()) {
        for (auto& f : futures) {
            if (f.wait_for(chrono::seconds(0)) == future_status::ready) {
                result.add(f.get());
                if (lastDispatchedChunk < chunks.size())
                    f = makeFuture();
            }
            this_thread::yield();
        }
    }

    // wait for all remaining futures
    for (auto f : futures)
        if (f.valid())
            result.add(f.get());

    return resolveMatches(std::move(result));
}
