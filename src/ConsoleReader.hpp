
#ifndef CONSOLEREADER_HPP
#define CONSOLEREADER_HPP


namespace MCServer {

class MinecraftServer;
struct ConsoleReaderData;

class ConsoleReader {
public:
    ConsoleReader(MinecraftServer *server);
    void run();
    void println(const std::string &);
    void stop();
private:
    void init();
    void clearLine();
    void printAnsiEscape(const char *);
    void printAnsiEscape(char);

    ConsoleReaderData *m;
};

}

#endif // CONSOLEREADER_HPP
