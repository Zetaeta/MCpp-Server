
#include "MinecraftServer.hpp"

using MCServer::MinecraftServer;

int main(int argc, char **argv)
{
    MinecraftServer *server = new MinecraftServer();

    server->run();
}
