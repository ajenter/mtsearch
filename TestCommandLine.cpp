#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

#include "CommandLine.cpp"

template <typename T, int N>
constexpr int countof(T const (&)[N]) noexcept
{
    return N;
}

BOOST_AUTO_TEST_SUITE(TestCommandLine)

BOOST_AUTO_TEST_CASE(Normal)
{
    const char* args[] = {"1", "2", "3"};
    auto a = parseCommandLine(countof(args), const_cast<char**>(args));

    BOOST_CHECK(a.filename == "2");
    BOOST_CHECK(a.mask == "3");
}

BOOST_AUTO_TEST_CASE(NoArgs)
{
    BOOST_CHECK_THROW(parseCommandLine(0, nullptr), std::invalid_argument);
    BOOST_CHECK_THROW(parseCommandLine(1, nullptr), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE(NotEnoughArgs)
{
    auto f = [] {
        const char* args[] = {"1", "2"};
        parseCommandLine(countof(args), const_cast<char**>(args));
    };
    BOOST_CHECK_THROW(f(), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE(TooManyArgs)
{
    auto f = [] {
        const char* args[] = {"1", "2", "3", "4"};
        parseCommandLine(countof(args), const_cast<char**>(args));
    };
    BOOST_CHECK_THROW(f(), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(MaskTooLong)
{
    auto f = [] {
        const char* args[] = {"1", "2", "01234567890123456789012345678901234567890123456789"
                                        "01234567890123456789012345678901234567890123456789"
                                        "moreThan100"};
        parseCommandLine(countof(args), const_cast<char**>(args));
    };
    BOOST_CHECK_THROW(f(), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
