
CC=clang
CFLAGS=-g -Wall

SRC=src
OBJ=obj
INC=include
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c,$(OBJ)/%.o, $(SRCS))

BINDIR=bin
BIN=bin/main

LIBS=SDL2
LINKLIBS=-l $(LIBS)

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(LINKLIBS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -I $(INC) $< -o $@

format:
	clang-format -i src/* include/*
clean:
	$(RM) -r $(BINDIR)/* $(OBJ)/*
