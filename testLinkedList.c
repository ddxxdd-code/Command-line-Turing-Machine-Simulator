// This is the testfile for linkedList ADT
// Created by ddxxdd-code on 2020-05-20

#include <stdio.h>
#include <string.h>
#include "linkedList.h"

void showList(Node head);
static int compar(const char *first, const char *second);

int main() {
    Node head = NULL;
    head = insertLinkedList(head, "A", 0);
    head = insertLinkedList(head, "B", 0);
    head = insertLinkedList(head, "C", 0);
    showList(head);
    head = insertLinkedList(head, "A", 0);
    showList(head);
    printf("Length: %d\n", lengthLinkedList(head));
    printf("Rank of A is %d\n", searchLinkedList(head, "A", &compar));
    printf("Rank of B is %d\n", searchLinkedList(head, "B", &compar));
    printf("Rank of C is %d\n", searchLinkedList(head, "C", &compar));
    printf("Rank of D is %d\n", searchLinkedList(head, "D", &compar));
    printf("Free the list\n");
    freeLinkedList(head);
    return 0;
}

void showList(Node head) {
    if (!head) {
        printf("[X]\n");
        return;
    }
    printf("%d %s -> ", head->rank, head->name);
    showList(head->next);
}

static int compar(const char *first, const char *second) {
    return strcmp(first, second);
}
