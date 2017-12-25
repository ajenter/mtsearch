#include "CommandLine.h"
#include "Finding.h"

#include <cstdlib>
#include <iomanip>
#include <iostream>

using std::cout;
using std::endl;
using std::setw;

void displayResult(const FindResult& result)
{
    const auto tab = setw(8);

    cout << tab << result.size() << endl;

    for (auto r : result) {
        cout << tab << r.line
             << tab << r.column
             << " " << r.matchText << endl;
    }
}


int main(int argc, char** argv)
{
    try {
        const auto args = parseCommandLine(argc, argv);
        const auto result = findMatchesInFile(args.filename, args.mask);

        displayResult(result);

        return EXIT_SUCCESS;
    }
    catch (std::exception& e) {
        cout << e.what();
    }

    return EXIT_FAILURE;
}
