// main.c
//
// The main function for truring machine
//
// Created by ddxxdd-code
// on 2020-05-21

#include <stdio.h>
#include <stdlib.h>
#include "turingMachine.h"

int main(int argc, char *argv[]) {
    if (argc == 2) {
        struct transitionMap *map = readFile(argv[1]);
        char sequence[256] = {0};
        scanf("%s", sequence);
        runTuringMachine(sequence, map->stages->name, map);
    } else if (argc == 3) {
        struct transitionMap *map = readFile(argv[1]);
        char sequence[256] = {0};
        scanf("%s", sequence);
        runTuringMachine(sequence, argv[2], map);
    }
    return 0;
}
