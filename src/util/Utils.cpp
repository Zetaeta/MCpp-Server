
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

long diffms(timespec start, timespec end) {
    long nsdiff = end.tv_nsec - start.tv_nsec;
    long sdiff = end.tv_sec - start.tv_sec;
    return sdiff * 1000 + nsdiff / 1000;
}

long currentTimeMillis() {
    timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

}
