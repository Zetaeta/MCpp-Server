
#My First Makefile :D

CC=g++

CFLAGS=-c -Wall -std=c++11 -g
LDFLAGS= -lpthread -lreadline

all: mcpp-server

main=main.o MinecraftServer.o ConsoleReader.o
network=NetworkServer.o
logging=Logger.o ConsoleHandler.o

mcpp-server: $(main) $(network) $(logging)
	$(CC) $(LDFLAGS) *.o -o mcpp-server


main.o: main.cpp MinecraftServer.hpp
	$(CC) $(CFLAGS) -c main.cpp

MinecraftServer.o: MinecraftServer.cpp MinecraftServer.hpp network/NetworkServer.hpp logging/Logger.hpp ConsoleReader.hpp
	$(CC) $(CFLAGS) -c MinecraftServer.cpp

network: NetworkServer.o

NetworkServer.o: network/NetworkServer.cpp network/NetworkServer.hpp
	$(CC) $(CFLAGS) -c network/NetworkServer.cpp

Logger.o: logging/Logger.cpp logging/Logger.hpp logging/Handler.hpp logging/ConsoleHandler.hpp MinecraftServer.hpp
	$(CC) $(CFLAGS) -c logging/Logger.cpp

ConsoleHandler.o: logging/ConsoleHandler.cpp logging/ConsoleHandler.hpp MinecraftServer.hpp ConsoleReader.hpp
	$(CC) $(CFLAGS) -c logging/ConsoleHandler.cpp

logging/ConsoleHandler.hpp: logging/Handler.hpp

logging/Logger.hpp: logging/Level.hpp

logging/Handler.hpp: logging/Logger.hpp

ConsoleReader.o: ConsoleReader.hpp ConsoleReader.cpp MinecraftServer.hpp
	$(CC) $(CFLAGS) -c ConsoleReader.cpp

%.obj: %.cpp
	$(CC) $(CFLAGS) -c $(.SOURCE)


clean:
	rm *.o
