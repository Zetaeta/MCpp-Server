
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
using std::ostream;

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
    ostream cout, cerr;

    Lock lock;
    
    ConsoleReaderData(MinecraftServer *server) :server(0), running(true), cout(server->getStdout()), cerr(server->getStderr()) {}
};

ConsoleReader::ConsoleReader(MinecraftServer *server)
:m(new ConsoleReaderData(server)) {
    m->server = server;
    pthread_create(&m->thread, NULL, &startConsoleReader, this);
}

void ConsoleReader::run() {
    init();
#ifndef NOINPUT
    while (m->running) {
        const char *line = readline(">");
        if (!line) {
           break;
        }
        m->server->dispatchConsoleCommand(line);
    }
    exit(0);
#endif
}

void ConsoleReader::init() {
}

void ConsoleReader::println(const string &s) {
#ifndef NOINPUT
    m->lock.lock();
    int savedPoint = rl_point;
    char *savedLine = rl_copy_text(0, rl_end);
    rl_set_prompt("");
    rl_replace_line("", 0);
    rl_redisplay();
    m->cout << s << '\n';
    rl_set_prompt(">");
    rl_replace_line(savedLine, 0);
    rl_point = savedPoint;
    rl_redisplay();
    m->lock.unLock();
#else
    puts(s.c_str());
#endif
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

