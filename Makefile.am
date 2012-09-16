#SUBDIRS = src

currentdir=/home/daniel/Documents/Programming/MC++-Server

current_PROGRAMS = mc++-server

AM_CXXFLAGS = -std=c++11 -rdynamic -Isrc
AM_LDFLAGS = -lreadline -lpthread -lssl -lcrypto -ldl -rdynamic

main = src/main.cpp src/MinecraftServer.cpp src/EntityManager.cpp src/Lock.cpp
network = src/network/NetworkServer.cpp src/network/NetUtils.cpp src/network/PlainSocketOutputStream.cpp src/network/PlainSocketInputStream.cpp src/network/PlainSocketStream.cpp src/network/ClientConnection.cpp src/network/Packet.cpp src/network/PacketHandler.cpp src/network/EncryptedSocketInputStream.cpp src/network/EncryptedSocketOutputStream.cpp src/network/EncryptedSocketStream.cpp
logging = src/logging/Logger.cpp src/logging/UIHandler.cpp
util = src/util/Unicode.cpp src/util/FSUtils.cpp src/util/StringUtils.cpp src/util/Utils.cpp
entity = src/game/entity/Entity.cpp src/game/entity/Player.cpp
plugin = src/plugin/PluginManager.cpp
ui = src/ui/cli/CliUIManager.cpp src/ui/cli/ConsoleReader.cpp src/ui/UIManager.cpp

mc___server_SOURCES= $(main) $(network) $(logging) $(util) $(entity) $(plugin) $(ui)
