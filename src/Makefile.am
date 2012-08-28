currentdir=/home/daniel/Documents/Programming/MC++-Server

current_PROGRAMS = mcpp-server

AM_CXXFLAGS = -std=c++11
AM_LDFLAGS = -lreadline -lpthread -lssl -lcrypto

main = main.cpp MinecraftServer.cpp ConsoleReader.cpp Unicode.cpp
network = network/NetworkServer.cpp network/NetUtils.cpp network/SocketOutputStream.cpp network/SocketInputStream.cpp network/SocketStream.cpp network/ClientConnection.cpp
logging = logging/Logger.cpp logging/ConsoleHandler.cpp

mcpp_server_SOURCES= $(main) $(network) $(logging)
