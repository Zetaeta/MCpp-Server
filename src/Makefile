
#My First Makefile :D

CC=g++

CFLAGS=-c -Wall -std=c++11
LDFLAGS= -lpthread

all: mcpp-server

main=main.o MinecraftServer.o
network=NetworkServer.o
logging=Logger.o ConsoleHandler.o

mcpp-server: $(main) $(network) $(logging)
	$(CC) $(LDFLAGS) *.o -o mcpp-server


main.o: main.cpp MinecraftServer.hpp
	$(CC) $(CFLAGS) -c main.cpp

MinecraftServer.o: MinecraftServer.cpp MinecraftServer.hpp network/NetworkServer.hpp
	$(CC) $(CFLAGS) -c MinecraftServer.cpp

network: NetworkServer.o

NetworkServer.o: network/NetworkServer.cpp network/NetworkServer.hpp
	$(CC) $(CFLAGS) -c network/NetworkServer.cpp

Logger.o: logging/Logger.cpp logging/Logger.hpp logging/Handler.hpp logging/ConsoleHandler.hpp
	$(CC) $(CFLAGS) -c logging/Logger.cpp

ConsoleHandler.o: logging/ConsoleHandler.hpp
	$(CC) $(CFLAGS) -c logging/ConsoleHandler.cpp

logging/ConsoleHandler.hpp: logging/Handler.hpp

%.obj: %.cpp
	$(CC) $(CFLAGS) -c $(.SOURCE)


clean:
	rm *.o
