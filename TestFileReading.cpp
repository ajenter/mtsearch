#include <boost/test/unit_test.hpp>

#include "FileReading.cpp"

BOOST_AUTO_TEST_CASE(GetFileSize_ThrowsWhenFileNotExists)
{
    BOOST_CHECK_THROW(getFileSize("this file doesnt exist"), std::runtime_error);
}
