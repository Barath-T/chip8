
CC=clang
CFLAGS=-g -Wall

SRC=src
OBJ=obj
INC=include
SRCS=$(wildcard $(SRC)/*.c)
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

BINDIR=bin
BIN=bin/main

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c -I $(INC) $< -o $@

clean:
	$(RM) -r $(BINDIR)/* $(OBJ)/*
