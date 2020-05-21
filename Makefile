CC = gcc
CFLAGS=-Wall -Werror -g

all : main.o turingMachine.o linkedList.o
	$(CC) -o turingMachine main.o turingMachine.o linkedList.o
testLinkedList : testLinkedList.c linkedList.o
	$(CC) -o testLinkedList testLinkedList.c linkedList.o

turingMachine.o : turingMachine.c turingMachine.h
linkedList.o : linkedList.c linkedList.h

clear : rm -fr main.o turingMachine.o linkedList.o turingMachine
