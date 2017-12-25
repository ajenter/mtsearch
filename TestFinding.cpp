#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "Finding.cpp"

#include <boost/format.hpp>

using boost::format;

BOOST_AUTO_TEST_SUITE(TestFinding)

BOOST_AUTO_TEST_CASE(SplitIntoOverlappingChunks)
{
    // clang-format off
    struct {
        size_t total, chunkSize, overlap;     Chunks expected;
    } tests[] = {
        {          0,         1,       0,     {}},
        {          1,        10,       1,     {{0, 1}}},
        {          1,         1,       0,     {{0, 1}}},
        {          2,         2,       1,     {{0, 2}}},
        {          2,         1,       0,     {{0, 1}, {1, 1}}},
        {          3,         2,       1,     {{0, 2}, {1, 2}}},
        {         10,         5,       1,     {{0, 5}, {4, 5}, {8, 2}}},
        {         11,         5,       1,     {{0, 5}, {4, 5}, {8, 3}}},
        {         12,         5,       1,     {{0, 5}, {4, 5}, {8, 4}}},
        {         13,         5,       1,     {{0, 5}, {4, 5}, {8, 5}}},
        {         14,         5,       1,     {{0, 5}, {4, 5}, {8, 5}, {12, 2}}},
        {         10,         5,       2,     {{0, 5}, {3, 5}, {6, 4}}},
    };
    // clang-format on

    for (auto t : tests) {
        auto r = splitIntoOverlappingChunks(t.total, t.chunkSize, t.overlap);
        BOOST_TEST_INFO(format("%d-%d-%d") % t.total % t.chunkSize % t.overlap);
        BOOST_CHECK_EQUAL(r, t.expected);
    }
}

BOOST_AUTO_TEST_CASE(SplitIntoOverlappingChunks_TooLargeOverlap)
{
    auto f = [] {
        splitIntoOverlappingChunks(10, 5, 3);
    };
    BOOST_CHECK_THROW(f(), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
