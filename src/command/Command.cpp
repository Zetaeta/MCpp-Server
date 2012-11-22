
#include <functional>

#include "Command.hpp"

namespace MCServer {
namespace Command {

using std::string;
using std::vector;
using std::function;


Command::Command(const function<void (const string &, const shared_ptr<CommandSender> &, const vector<string> &)> &func)
:handler(func) {}

function<void (const string &, const shared_ptr<CommandSender> &, const vector<string> &)> Command::getHandler() {
    return handler;
}

}
}

