
#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

namespace MCServer {
namespace Command {

class Command;

class CommandManager {
public:
    void dispatchCommand();
private:
    map<std::string, Command *> commands;
};

}
}

#endif

