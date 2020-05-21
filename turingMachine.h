// turingMachine.h
//
// This is the header file for a turing machine
// This machine is based on A.M.Turing's 1936 paper
// ON COMPUTABLE NUMBERS, WITH AN APPLICATION TO THE ENTSCHEIDUNGSPROBLEM
// https://www.cs.virginia.edu/~robins/Turing_Paper_1936.pdf
//
// This machine defines 3 types of move
// move left, move right and stay
//
// Created by ddxxdd-code on 2020-05-20

#include "linkedList.h"

struct transition {
    int writeIndex;
    int move;
    int nextStageIndex;
};

struct transitionMap {
    struct transition **transitionMatrix;
    Node stages;
    Node symbols;
};

// Read file and return the transition map
struct transitionMap *readFile(char *fileName);

// Run the Turing machine on the given sequence
void runTuringMachine(char *sequence, char *startStage, struct transitionMap *transitions);
