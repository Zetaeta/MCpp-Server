
#include <iostream>
#include <stdexcept>
#include <cxxabi.h>

#include "Utils.hpp"

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

bool inUsrShare() {
    return string(getenv("PWD")) == "/usr/share";
}

string demangle(const string &name) {
    int status;
    char *result = abi::__cxa_demangle(name.c_str(), NULL, NULL, &status);
    if (status == 0) {
        string ret(result);
        free(result);
        return ret;
    }
    else {
        throw std::logic_error("Invalid mangled name: " + name);
    }
}

double diffms(timespec start, timespec end) {
    std::cout << start.tv_nsec << ' ' << end.tv_nsec << '\n';
    long diff = end.tv_nsec - start.tv_nsec;
    std::cout << diff << '\n';
    return diff / 1000;
}

}
