
#ifndef COMMANDMANAGER_HPP
#define COMMANDMANAGER_HPP

#include <unordered_map>
#include <map>

#include <Util/stlfwd.hpp>

namespace MCServer {

namespace Entities {
class Player;
}

namespace Command {

class Command;

using std::string;
using std::function;
using std::vector;
using std::shared_ptr;
using std::pair;

using Entities::Player;

class CommandManager {
public:
    void dispatchCommand(const string &command, const shared_ptr<Entities::Player> &);
    void registerCommand(const string &, const function<
        void (const string &, const shared_ptr<Player> &, const vector<string> &)> &);
    void registerCommand(const function<bool (const string &)> &rule, const 
    function<void (const string &, const shared_ptr<Player> &, const vector<string> &)> &);
private:
    std::unordered_map<string, Command *> commands;
//    std::map<std::function<bool (const std::string &)>, Command *> commandRules;
    vector<pair<function<bool (const string &)>, Command *>> commandRules;
};

}
}

#endif

