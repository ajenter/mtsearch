#include "Matching.cpp"
//#include "Utils.h"

#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

// note: this is required because of Boost.Test deficiencies
// see https://stackoverflow.com/questions/17572583
namespace boost {
    namespace test_tools {
        namespace tt_detail {
            template <>
            struct print_log_value<LineStarts> {
                void operator()(std::ostream& os, const LineStarts& ts)
                {
                    ::operator<<(os, ts);
                }
            };
        }
    }
}

BOOST_AUTO_TEST_SUITE(TestMatching)

// clang-format off
static struct {
    std::string description,           text,            mask;       Matches expected;          LineStarts ls;
} tests[] = {       
    {"empty buffer",                   "",              "_",        {}},
    
    {"simple case",                    "aba",           "a",        {{0, "a"}, {2, "a" }}},
    
    {"longer mask",                    "aba",           "abba",     {}},    
    
    {"simple newlines",                "ab\nba",        "a",        {{0, "a"}, {4, "a"}},      {2} },    
    
    {"consec newlines",                "ab\n\nba",      "a",        {{0, "a"}, {5, "a"}},      {2,3} },    
    
    {"rn newlines",                    "\r\nba",        "a",        {{3, "a"}},                {1}},
    {"one ?",                          "abcd",          "a?c",      {{0, "abc"}}},
    {"two ?s",                         "abcd",          "b??",      {{1, "bcd"}}},
    {"all ?s",                         "abcd",          "???",      {{0, "abc"}}},
    {"consecutive ?s",                 "abcd",          "??",       {{0, "ab"}, {2, "cd"}}},
    {"match equal to whole input",     "abcd",          "abcd",     {{0, "abcd"}}},
    {"? match equal to whole input",   "abcd",          "????",     {{0, "abcd"}}},
    {"matching is case-sensitive",     "abcd",          "A",        {}},
    {"? doesn't match newline",        "ab\ncd",        "ab?",      {},                        {2}},
    {"? doesn't match newline 2",      "ab\ncd",        "?cd",      {},                        {2}},
    {"? doesn't match \\r",            "ab\r\ncd",      "ab?",      {},                        {3}},
    {"? don't mess up line counts",    "ab\nab\nabcd",  "ab??",     {{6, "abcd"}},             {2,5}},        
};
// clang-format on

BOOST_AUTO_TEST_CASE(All)
{
    for (auto t : tests) {
        auto m = findMatches(t.text.c_str(), t.text.size(), t.mask);

        BOOST_TEST_INFO(t.description);
        BOOST_CHECK_EQUAL(m.matches, t.expected);
        BOOST_TEST_INFO(t.description);
        BOOST_CHECK_EQUAL(m.lineStarts, t.ls);
    }
}

BOOST_AUTO_TEST_CASE(ZeroBufferNoMatches)
{
    auto m = findMatches(nullptr, 0, "");
    BOOST_CHECK_EQUAL(m.matches, Matches());
    BOOST_CHECK_EQUAL(m.lineStarts, LineStarts());
}


BOOST_AUTO_TEST_CASE(EmptyMaskThrows)
{
    auto f = [] {
        std::string text = "some";
        findMatches(text.c_str(), text.size(), "");
    };
    BOOST_CHECK_THROW(f(), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
