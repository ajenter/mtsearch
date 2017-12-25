#pragma once
#include <string>

struct CommandLineArgs {
    std::string filename;
    std::string mask;
};

CommandLineArgs parseCommandLine(int argc, char** argv);
