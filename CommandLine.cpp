#include "CommandLine.h"

#include <stdexcept>

static const int ExpectedArgCount = 3;
static const int MaskMaxLength = 100;

// When command line params get more complicated, we can switch to boost::program_options

CommandLineArgs parseCommandLine(int argc, char** argv)
{
    if (argc != ExpectedArgCount)
        throw std::invalid_argument(argc == 0 ? "Usage: mtsearch <filename> <mask>"
                                              : argc < ExpectedArgCount ? "Not enough arguments provided"
                                                                        : "Too many arguments provided");
    const std::string filename(argv[1]);
    const std::string mask(argv[2]);

    if (mask.length() > MaskMaxLength)
        throw std::invalid_argument("Mask too long");

    return {filename, mask};
}
