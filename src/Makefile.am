currentdir=/home/daniel/Documents/Programming/MC++-Server

current_PROGRAMS = mcpp-server

AM_CXXFLAGS = -std=c++11
AM_LDFLAGS = -lreadline -lpthread -lssl -lcrypto

main = main.cpp MinecraftServer.cpp ConsoleReader.cpp EntityManager.cpp
network = network/NetworkServer.cpp network/NetUtils.cpp network/PlainSocketOutputStream.cpp network/PlainSocketInputStream.cpp network/PlainSocketStream.cpp network/ClientConnection.cpp network/Packet.cpp network/PacketHandler.cpp network/EncryptedSocketInputStream.cpp network/EncryptedSocketOutputStream.cpp network/EncryptedSocketStream.cpp
logging = logging/Logger.cpp logging/ConsoleHandler.cpp
util = util/Unicode.cpp
entity = entity/Entity.cpp

mcpp_server_SOURCES= $(main) $(network) $(logging) $(util)
