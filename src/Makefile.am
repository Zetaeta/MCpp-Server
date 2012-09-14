currentdir=/home/daniel/Documents/Programming/MC++-Server

current_PROGRAMS = mcpp-server

AM_CXXFLAGS = -std=c++11 -rdynamic
AM_LDFLAGS = -lreadline -lpthread -lssl -lcrypto -ldl -rdynamic

main = main.cpp MinecraftServer.cpp EntityManager.cpp
network = network/NetworkServer.cpp network/NetUtils.cpp network/PlainSocketOutputStream.cpp network/PlainSocketInputStream.cpp network/PlainSocketStream.cpp network/ClientConnection.cpp network/Packet.cpp network/PacketHandler.cpp network/EncryptedSocketInputStream.cpp network/EncryptedSocketOutputStream.cpp network/EncryptedSocketStream.cpp
logging = logging/Logger.cpp logging/UIHandler.cpp
util = util/Unicode.cpp util/FSUtils.cpp util/StringUtils.cpp util/Utils.cpp
entity = entity/Entity.cpp entity/Player.cpp
plugin = plugin/PluginManager.cpp
ui = ui/cli/CliUIManager.cpp ui/cli/ConsoleReader.cpp ui/UIManager.cpp

mcpp_server_SOURCES= $(main) $(network) $(logging) $(util) $(entity) $(plugin) $(ui)
