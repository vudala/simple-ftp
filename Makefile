#!/bin/bash

CC=g++
FLAGS=-Wall -g
LIBS=
SRC=$(wildcard *.cpp)

# todos os objetos a serem compilados
OBJS=$(subst .cpp,.o,$(SRC))

# apenas os objetos com a main
MAIN_OBJS=server.o client.o teste.o

# apenas os commmons objects
COMMON_OBJS=$(filter-out $(MAIN_OBJS),$(OBJS))
RM=rm -f

all: client server

teste: $(COMMON_OBJS) teste.o
	$(CC) $(FLAGS) $(COMMON_OBJS) $@.o -o teste $(LIBS)

client: $(COMMON_OBJS) client.o
	$(CC) $(FLAGS) $(COMMON_OBJS) $@.o -o client $(LIBS)

server: $(COMMON_OBJS) server.o
	$(CC) $(FLAGS) $(COMMON_OBJS) $@.o -o server $(LIBS)

%.o: %.cpp
	$(CC) $(FLAGS) -c $<

clean:
	$(RM) $(OBJS)

purge: clean
	$(RM) client
	$(RM) server
	$(RM) teste
