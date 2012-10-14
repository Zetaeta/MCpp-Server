
#include <map>

#include <getopt.h>

#include "MinecraftServer.hpp"
#include "util/Utils.hpp"

using std::map;
using std::string;

using MCServer::MinecraftServer;

static option options[] = {
    {"gui", no_argument, 0, 'g'},
    {"ui", required_argument, 0, 'u'},
    {"cli", no_argument, 0, 'c'},
    {"mtrace", optional_argument, 0, 'm'},
    {0, 0, 0, 0},
};

int main(int argc, char **argv)
{
    MCServer::commandName = argv[0];

    map<string, string *> optionMap;
    int c;
    int optionIndex;
    while ((c = getopt_long(argc, argv, "g::u:", options, &optionIndex)) != -1) {
        switch (c) {
        case 'g':
            optionMap["gui"] = NULL;
            break;
        case 'u':
            optionMap["ui"] = new string(optarg);
            break;
        case 'm':
            if (optarg) {
                optionMap["mtrace"] = new string(optarg);
            }
            else {
                optionMap["mtrace"] = new string("mtrace.log");
            }
        }
    }

    MinecraftServer *server = new MinecraftServer(optionMap, argc, argv);

    server->run();
}
