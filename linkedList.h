// linkedList.h
//
// This is the header file for linkedList ADT
// This is used to construct a linkedlist
// with string type data and integer type data
// with functions to insert, search and free in the list
//
// Created by ddxxdd-code on 2020-05-20

#define MAX_NAME_LENGTH 16

typedef struct node *Node;

struct node {
    char name[MAX_NAME_LENGTH];
    int rank;
    struct node *next;
};

// Append to end the new element to the list with head
Node insertLinkedList(Node head, char *newName, int startRank);

// Return the rank of name in the list.
// If name not found, return -1.
int searchLinkedList(Node head, char *name, int (*compar)(const char *first, const char *second));

char *searchRankLinkedList(Node head, int rank);

// Return the length of linkedlist
int lengthLinkedList(Node head);

// Free the linked list starting with head
void freeLinkedList(Node head);
