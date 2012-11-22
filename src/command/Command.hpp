
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <Util/stlfwd.hpp>

namespace MCServer {

using std::function;
using std::string;
using std::vector;
using std::shared_ptr;

class CommandSender;

namespace Command {

class Command {
public:
    Command(const function<void (const string &, const shared_ptr<CommandSender> &, const vector<string> &)> &);
    function<void (const string &, const shared_ptr<CommandSender> &, const vector<string> &)>
        getHandler();
private:
    function<void (const string &, const shared_ptr<CommandSender> &, const vector<string> &)> handler;
};

}
}

#endif

