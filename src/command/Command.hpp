
#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <Util/stlfwd.hpp>

namespace MCServer {

namespace Entities {
class Player;
}

using std::function;
using std::string;
using std::vector;
using std::shared_ptr;

using Entities::Player;

namespace Command {

class Command {
public:
    Command(const function<void (const string &, const shared_ptr<Player> &, const vector<string> &)> &);
    function<void (const string &, const shared_ptr<Player> &, const vector<string> &)>
        getHandler();
private:
    function<void (const string &, const shared_ptr<Player> &, const vector<string> &)> handler;
};

}
}

#endif

