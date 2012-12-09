
currentdir=/home/daniel/Documents/Programming/MC++-Server

current_PROGRAMS = mc++-server

TESTS = test/test-all

CXXFLAGS=
AM_CXXFLAGS=

@compiler_specifics@

mc___server_CXXFLAGS = -O0 -g -std=c++11  -I src/ -Wall -Wextra -Werror \
    -Wno-error=unused-variable  \
    -Wno-error=unused-parameter -Wno-error=unused-function $(compiler_specifics)
mc___server_LDFLAGS = -O0 -rdynamic
mc___server_LDADD = -lreadline -lpthread -lssl -lcrypto -ldl -lNBT -lIOStream -lrt -lZUtil

main = src/main.cpp src/MinecraftServer.cpp src/Lock.cpp src/ReentrantLock.cpp \
    src/Condition.cpp src/Scheduler.cpp src/SchedulerThread.cpp src/Future.cpp \
    src/ChatServer.cpp src/ServerCommandSender.cpp
packets = 
network = src/network/NetworkServer.cpp src/network/ClientConnection.cpp \
    src/network/Packet.cpp src/network/PacketHandler.cpp $(packets)
logging = src/logging/Logger.cpp src/logging/UIHandler.cpp \
    src/logging/LoggerStreamBuf.cpp
util = src/util/FSUtils.cpp \
    src/util/Utils.cpp
plugin = src/plugin/PluginManager.cpp src/plugin/SOPluginLoader.cpp \
    src/plugin/PluginLoader.cpp
command = src/command/CommandManager.cpp src/command/Command.cpp
ui = src/ui/cli/CliUIManager.cpp src/ui/cli/ConsoleReader.cpp src/ui/UIManager.cpp
event = src/event/EventManager.cpp src/event/Event.cpp \
    src/event/entity/player/PlayerEvent.cpp src/event/entity/player/PlayerChatEvent.cpp

entity = src/game/entity/Entity.cpp src/game/entity/Player.cpp
game = $(entity) src/game/Chunk.cpp src/game/World.cpp \
    src/game/EntityManager.cpp src/game/ChunkLoader.cpp src/game/PlayerInventory.cpp src/game/BlockAccess.cpp \
    src/game/Block.cpp

mc___server_SOURCES= $(main) $(network) $(logging) $(util) $(game) $(plugin) $(command) $(ui) $(event)

