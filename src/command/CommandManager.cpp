
#include <string>
#include <vector>
#include <functional>
#include <algorithm>
#include <memory>

#include <Util/StringUtils.hpp>

#include "CommandManager.hpp"
#include "Command.hpp"

namespace MCServer {
namespace Command {

using std::string;
using std::function;
using std::vector;
using std::pair;
using std::for_each;
using std::shared_ptr;

using Util::split;

using Entities::Player;

void CommandManager::dispatchCommand(const string &command, const shared_ptr<Player> &player) {
   auto it = commands.find(command);
    if (it != commands.end()) {
        size_t firstSpace = command.find(' ');
        if (firstSpace != string::npos) {
            string actualCmd = command.substr(0, firstSpace);
            vector<string> args = split(command.substr(firstSpace + 1), " ");
            it->second->getHandler()(actualCmd, player, args);
        }
        it->second->getHandler()(command, player, {});
    }
    for_each(commandRules.begin(), commandRules.end(),
            [&] (const pair<function<bool (const string &)>, Command *> rule) {
        if (rule.first(command)) {
            rule.second->getHandler()(command, player, {});
        }
    });
}

void CommandManager::registerCommand(const string &command,
                 const function<void (const string &, const shared_ptr<Player> &, const vector<string> &)> &func) {
    Command *cmd = new Command(func);
    commands[command] = cmd;
}

void CommandManager::registerCommand(const function<bool (const string &)> &rule,
                 const function<void (const string &, const shared_ptr<Player> &, const vector<string> &)> &func) {
//    commandRules[rule] = new Command(func);
    commandRules.push_back(make_pair(rule, new Command(func)));
}

}
}

