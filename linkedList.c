// linkedList.c
// 
// This is the implementation for linkedList ADT
// This is used to construct a linkedlist
// with string type data and integer type data
// with functions to insert, search and free in the list
//
// Created by ddxxdd-code on 2020-05-20

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "linkedList.h"

static Node newNode(char *newName, int newRank);

// Append to end the new element to the list with head
// return the head
Node insertLinkedList(Node head, char *newName, int startRank) {
    if (!head)
        return newNode(newName, startRank);
    if (strcmp(head->name, newName) == 0)
        return head;
    head->next = insertLinkedList(head->next, newName, head->rank + 1);
    return head;
}

// Return the rank of name in the list.
// If name not found, return -1.
int searchLinkedList(Node head, char *searchName, int (*compar)(const char *first, const char *second)) {
    if (!head)
        return -1;
    if ((*compar)(searchName, head->name) == 0)
        return head->rank;
    return searchLinkedList(head->next, searchName, compar);
}

char *searchRankLinkedList(Node head, int searchRank) {
    if (!head)
        return NULL;
    if (head->rank == searchRank)
        return head->name;
    return searchRankLinkedList(head->next, searchRank);
}

// Return the length of linkedlist
int lengthLinkedList(Node head) {
    if (!head)
        return 0;
    return 1 + lengthLinkedList(head->next);
}

// Free the linked list starting with head
void freeLinkedList(Node head) {
    if (!head)
        return;
    freeLinkedList(head->next);
    free(head);
}

static Node newNode(char *newName, int newRank) {
    Node new = malloc(sizeof(struct node));
    if (!new) {
        fprintf(stderr, "Making new node: insufficient memory\n");
        exit(1);
    }
    strcpy(new->name, newName);
    new->rank = newRank;
    new->next = NULL;
    return new;
}
