
#include "Utils.hpp"

#include <iostream>

using std::string;
using std::cerr;

namespace MCServer {

void invalidOption(string message, int exitCode) {
    cerr << message << '\n';
    cerr << "For more information, see " << commandName << " --help\n";
    exit(exitCode);
}

void errorExit(string message, int exitCode) {
    cerr << message << '\n';
    exit(exitCode);
}

string commandName;

}
