
#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP

#include <string>

namespace MCServer {

bool endsWith(const std::string &string, const std::string &ending);
std::string toLower(const std::string &string);

// Template function to check ending, useful for "Plugins::PluginLoaderRule"'s
template<const char *Ending> bool endsWithTemplate(const char *string) {
    return endsWith(string, Ending);
}

//typedef bool(_endsWithFunction)(const char *);

//template <const char *Ending> 
//endsWithFunction getEndsWith(Ending) {
//    return &endsWithTemplate<Ending>;
//}

}

#endif
