
#include <iostream>
#include <string>
#include <stdlib.h>
#include <readline/readline.h>

#include "ConsoleReader.hpp"
#include "MinecraftServer.hpp"
#include "Lock.hpp"

using std::cout;
using std::cerr;
using std::cin;
using std::string;

namespace MCServer {
namespace UI {

namespace {

void * startConsoleReader(void *consoleReader) {
    static_cast<ConsoleReader *>(consoleReader)->run();
    return NULL;
}

}

struct ConsoleReaderData {
    pthread_t thread;
    MinecraftServer *server;
    string lineBuffer;
    bool running;

    Lock lock;
    
    ConsoleReaderData() :server(0), running(true) {}
};

ConsoleReader::ConsoleReader(MinecraftServer *server)
 :m(new ConsoleReaderData()) {
    m->server = server;
    pthread_create(&m->thread, NULL, &startConsoleReader, this);
}

void ConsoleReader::run() {
    init();
    while (m->running) {
        const char *line = readline(">");
        if (!line) {
            return;
        }
        m->server->dispatchConsoleCommand(line);
    }
}

void ConsoleReader::init() {
}

void ConsoleReader::println(const string &s) {
    m->lock.lock();
    int savedPoint = rl_point;
    char *savedLine = rl_copy_text(0, rl_end);
    rl_set_prompt("");
    rl_replace_line("", 0);
    rl_redisplay();
    cout << s << '\n';
    rl_set_prompt(">");
    rl_replace_line(savedLine, 0);
    rl_point = savedPoint;
    rl_redisplay();
    m->lock.unLock();
}

void ConsoleReader::clearLine() {
    cout << '\r';
    printAnsiEscape('K');
}

void ConsoleReader::printAnsiEscape(const char * escape) {
    cout << (char) 27 << '[' << escape;
}

void ConsoleReader::printAnsiEscape(char escape) {
    cout << (char) 27 << '[' << escape;
}

void ConsoleReader::stop() {
    m->running = false;
}

}
}

