
#ifndef CHUNKLOADER_HPP
#define CHUNKLOADER_HPP

namespace MCServer {

class ChunkLoader {
public:
    void start();
    void run();
    void shutdown();
private:
    bool running;
};

}

#endif
