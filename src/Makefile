
#My First Makefile :D

CC=g++

CFLAGS=-c -Wall
LDFLAGS= -lpthread

all: mcpp-server

main=main.o MinecraftServer.o
network=NetworkServer.o

mcpp-server: $(main) $(network)
	$(CC) $(LDFLAGS) *.o -o mcpp-server


main.o: main.cpp MinecraftServer.hpp
	$(CC) $(CFLAGS) -c main.cpp

MinecraftServer.o: MinecraftServer.cpp MinecraftServer.hpp network/NetworkServer.hpp
	$(CC) $(CFLAGS) -c MinecraftServer.cpp

network: NetworkServer.o

NetworkServer.o: network/NetworkServer.cpp network/NetworkServer.hpp
	$(CC) $(CFLAGS) -c network/NetworkServer.cpp

%.obj: %.cpp
	$(CC) $(CFLAGS) -c $(.SOURCE)

