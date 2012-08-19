
#include <iostream>
#include <string>
#include <stdlib.h>
#include <readline/readline.h>

#include "ConsoleReader.hpp"
#include "MinecraftServer.hpp"

namespace MCServer {

using std::cout;
using std::cerr;
using std::cin;
using std::string;

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
/*        cout << '>';
        string line;
        char ch;
        while ((cin >> ch) && ch != '\n') {
            putchar(ch);
            m->lineBuffer += ch;
        }
        m->lineBuffer.clear(); */
        const char *line = readline(">");
        if (!line) {
            return;
        }
//        m->server->dispatchConsoleCommand(line);
    }
}

void ConsoleReader::init() {
//    system("stty -icanon min 1 -icrnl -inlcr -echo");
}

void ConsoleReader::println(const string &s) {
//    clearLine();
//    cout << s << '\n';
//    rl_forced_update_display();
/*    clearLine();
    if (s[s.size() - 1] == '\n') {
        cout << s;
    }
    else {
        cout << s << '\n';
    }
    cout << '>' << m->lineBuffer;
*/
    int savedPoint = rl_point;
    char *savedLine = rl_copy_text(0, rl_end);
    rl_set_prompt("");
    rl_replace_line("", 0);
    rl_redisplay();
    cout << s << '\n';
    rl_set_prompt(">");
    rl_replace_line(savedLine, 0);
    rl_point = savedPoint;
//    cerr << "rl_line_buffer: " << rl_line_buffer << '\n';
//    if (strlen(savedLine) != 0) {
        rl_redisplay();
//    }

//    cout << '\r';
//    cout << s << '\n';
//    rl_set_prompt(">");
//    rl_redisplay();
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
