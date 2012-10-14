
currentdir=/home/daniel/Documents/Programming/MC++-Server

current_PROGRAMS = mc++-server

mc___server_CXXFLAGS = -O1 -g -std=c++11 -rdynamic -I src/ -Wall -Werror -Wno-error=unused-variable -pg
mc___server_LDFLAGS = -O1 -rdynamic -pg
mc___server_LDADD = -lreadline -lpthread -lssl -lcrypto -ldl -lNBT -lIOStream -lrt -lZUtil

main = src/main.cpp src/MinecraftServer.cpp src/Lock.cpp src/Condition.cpp \
     src/Scheduler.cpp src/SchedulerThread.cpp
network = src/network/NetworkServer.cpp src/network/ClientConnection.cpp \
    src/network/Packet.cpp src/network/PacketHandler.cpp
logging = src/logging/Logger.cpp src/logging/UIHandler.cpp \
    src/logging/LoggerStreamBuf.cpp
util = src/util/FSUtils.cpp \
    src/util/StringUtils.cpp src/util/Utils.cpp
plugin = src/plugin/PluginManager.cpp src/plugin/SOPluginLoader.cpp \
    src/plugin/PluginLoader.cpp
ui = src/ui/cli/CliUIManager.cpp src/ui/cli/ConsoleReader.cpp src/ui/UIManager.cpp
entity = src/game/entity/Entity.cpp src/game/entity/Player.cpp
block = src/game/block/Block.cpp
game = $(entity) $(block) src/game/Chunk.cpp src/game/World.cpp src/game/EntityManager.cpp

mc___server_SOURCES= $(main) $(network) $(logging) $(util) $(game) $(plugin) $(ui)

