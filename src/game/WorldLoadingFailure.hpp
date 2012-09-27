
#ifndef WORLDLOADINGFAILURE_HPP
#define WORLDLOADINGFAILURE_HPP

#include <stdexcept>
#include <string>
#include <iostream>

class WorldLoadingFailure : public std::runtime_error {
public:
    WorldLoadingFailure(const std::string &what)
    :std::runtime_error(what) {
        std::cout << "WorldLoadingFailure( " << what << ");\n";
    }
};

#endif

