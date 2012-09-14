
#include "CliUIManager.hpp"
#include "ConsoleReader.hpp"

namespace MCServer {
namespace UI {

struct CliUIManagerData {
    MinecraftServer *server;
    ConsoleReader *consoleReader;
};

CliUIManager::CliUIManager(MinecraftServer *server)
:m(new CliUIManagerData) {
    m->server = server;
}

void CliUIManager::setup() {
    m->consoleReader = new ConsoleReader(m->server);
}

void CliUIManager::println(const std::string &s) {
    m->consoleReader->println(s);
}

}
}
