// turingMachine.c
//
// This is the implementation for a turing machine
// This machine is based on A.M.Turing's 1936 paper
// ON COMPUTABLE NUMBERS, WITH AN APPLICATION TO THE ENTSCHEIDUNGSPROBLEM
// https://www.cs.virginia.edu/~robins/Turing_Paper_1936.pdf
//
// This machine defines 3 types of move
// move left, move right and stay
//
// Created by ddxxdd-code on 2020-05-20

#include "turingMachine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define STAY 2

#define INVALID_CONFIG 0
#define INVALID_SYMBOL 1
#define TOO_MANY_STEPS 2
#define MEMORY_RAN_OUT 3
#define OPEN_FILE_FAIL 4

#define MAX_LINE_LENGTH 256
#define MAX_WORD_LENGTH 64
#define MAX_STEPS 1000

#define REFRESH_TIME 0

static int charCmp(const char *character, const char *string);
static void displaySequence(char *sequence, int length, int curr);
static void error(int errorType);

// Read file and return the transition map
struct transitionMap *readFile(char *fileName) {
    // Read the file twice to set the transition map
    // (curr_stage->read->write->final_stage)
    // To construct this, I need a 2D array of pointers
    // (curr_stage + read -> write and next stage)
    // The first oteration will build two linked lists
    // One for all stages
    // One for all symbols
    Node stageList = NULL;
    Node symbolList = NULL;
    FILE *file = fopen(fileName, "r");
    // Read line by line and then cut the lines into words
    char line[MAX_LINE_LENGTH];
    if (!file) {
        error(OPEN_FILE_FAIL);
    } else {
        while (fgets(line, MAX_LINE_LENGTH, file)) {
            int last = strlen(line);
            if (line[last - 1] == '\n')
                line[last - 1] = '\0';
            char *element;
            element = strtok(line, " ");
            if (!element)
                error(INVALID_CONFIG);
            stageList = insertLinkedList(stageList, element, 0);
            
            element = strtok(NULL, " ");
            if (!element || strlen(element) > 1)
                error(INVALID_CONFIG);
            symbolList = insertLinkedList(symbolList, element, 0);
            
            element = strtok(NULL, " ");
            if (!element || strlen(element) > 1)
                error(INVALID_CONFIG);
            symbolList = insertLinkedList(symbolList, element, 0);
            
            element = strtok(NULL, " ");
            if (!element || strlen(element) > 1)
                error(INVALID_CONFIG);
            
            element = strtok(NULL, " ");
            if (!element)
                error(INVALID_CONFIG);
            stageList = insertLinkedList(stageList, element, 0);
            
            // Check if there is anything left in the line
            element = strtok(NULL, " ");
            if (element)
                error(INVALID_CONFIG);
        }
    }
    fclose(file);
    // Now that the stageList and symbolList are constructed
    // read the file again to construct the transitionMap
    // Construct the transition list
    struct transitionMap *transitionMapResult =
    malloc(sizeof(struct transitionMap));
    if (!transitionMapResult)
        error(MEMORY_RAN_OUT);
    transitionMapResult->stages = stageList;
    transitionMapResult->symbols = symbolList;
    
    int totalStages = lengthLinkedList(stageList);
    int totalSymbols = lengthLinkedList(symbolList);
    struct transition **matrix = calloc(totalStages, sizeof(struct transition **));
    if (!matrix)
        error(MEMORY_RAN_OUT);
    for (int i = 0; i < totalStages; i++) {
        matrix[i] = calloc(totalSymbols, sizeof(struct transition));
        if (!matrix[i])
            error(MEMORY_RAN_OUT);
        for (int j = 0; j < totalSymbols; j++) {
            matrix[i][j].writeIndex = -1;
            matrix[i][j].move = -1;
            matrix[i][j].nextStageIndex = -1;
        }
    }
    file = fopen(fileName, "r");
    if (!file) {
        error(OPEN_FILE_FAIL);
    } else {
        while (fgets(line, MAX_LINE_LENGTH, file)) {
            int last = strlen(line);
            if (line[last - 1] == '\n')
                line[last - 1] = '\0';
            // The following have to be changed
            char *element;
            element = strtok(line, " ");
            if (!element)
                error(INVALID_CONFIG);
            int stage = searchLinkedList(stageList, element, &strcmp);
            if (stage == -1) {
                error(INVALID_CONFIG);
            }
            
            element = strtok(NULL, " ");
            if (!element || strlen(element) > 1)
                error(INVALID_CONFIG);
            int read = searchLinkedList(symbolList, element, &charCmp);
            if (read == -1) {
                error(INVALID_CONFIG);
            }
            
            element = strtok(NULL, " ");
            if (!element || strlen(element) > 1)
                error(INVALID_CONFIG);
            int write = searchLinkedList(symbolList, element, &charCmp);
            if (write == -1)
                error(INVALID_CONFIG);
            matrix[stage][read].writeIndex = write;
            
            
            element = strtok(NULL, " ");
            if (!element || strlen(element) > 1)
                error(INVALID_CONFIG);
            switch (element[0] - 'A') {
                case 'l' - 'A': matrix[stage][read].move = MOVE_LEFT; break;
                case 'L' - 'A': matrix[stage][read].move = MOVE_LEFT; break;
                case 'r' - 'A': matrix[stage][read].move = MOVE_RIGHT; break;
                case 'R' - 'A': matrix[stage][read].move = MOVE_RIGHT; break;
                case 's' - 'A': matrix[stage][read].move = STAY; break;
                case 'S' - 'A': matrix[stage][read].move = STAY; break;
                default: error(INVALID_CONFIG); break;
            }
            
            element = strtok(NULL, " ");
            if (!element)
                error(INVALID_CONFIG);
            int nextStage = searchLinkedList(stageList, element, &strcmp);
            if (nextStage == -1)
                error(INVALID_CONFIG);
            matrix[stage][read].nextStageIndex = nextStage;
            
            // Check if there is anything left in the line
            element = strtok(NULL, " ");
            if (element)
                error(INVALID_CONFIG);
        }
    }
    fclose(file);
    transitionMapResult->transitionMatrix = matrix;
    return transitionMapResult;
}

// Run the Turing machine on the given sequence
void runTuringMachine(char *sequence, char *startStage, struct transitionMap *transitions) {
    int currStage = 0;
    int sequenceLength = strlen(sequence);
    int currIndex = 0;
    char *currSequence = calloc(sequenceLength + 1, sizeof(char));
    if (!currSequence)
        error(MEMORY_RAN_OUT);
    strcpy(currSequence, sequence);
    int initialStage = searchLinkedList(transitions->stages, startStage, &strcmp);
    if (initialStage != -1)
        currStage = initialStage;
    int step = 0;
    while (step < MAX_STEPS) {
        // Display current sequence
        displaySequence(currSequence, sequenceLength, currIndex);
        
        // Read current
        int read = searchLinkedList(transitions->symbols, &currSequence[currIndex], &charCmp);
        if (read == -1)
            break;
        // Check if there is no transition,
        // end loop then halt
        if (transitions->transitionMatrix[currStage][read].writeIndex == -1)
            break;
        // Write to current
        currSequence[currIndex] = searchRankLinkedList(transitions->symbols, transitions->transitionMatrix[currStage][read].writeIndex)[0];
        // Move current
        switch (transitions->transitionMatrix[currStage][read].move) {
            case MOVE_LEFT: currIndex -= 1; break;
            case MOVE_RIGHT: currIndex += 1; break;
            case STAY: break;
        }
        if (currIndex < 0) {
            // Shift the whoole sequence right
            sequenceLength += 1;
            char *new = calloc(sequenceLength + 1, sizeof(char));
            if (!new)
                error(MEMORY_RAN_OUT);
            new[0] = '_';
            strcpy(&new[1], currSequence);
            currSequence = new;
            currIndex = 0;
        } else if (currIndex == sequenceLength) {
            sequenceLength += 1;
            char *new = calloc(sequenceLength + 1, sizeof(char));
            if (!new)
                error(MEMORY_RAN_OUT);
            strcpy(new, currSequence);
            new[sequenceLength - 1] = '_';
            new[sequenceLength] = '\0';
            currSequence = new;
        }
        // Change stage
        int nextStage = transitions->transitionMatrix[currStage][read].nextStageIndex;
        if (nextStage == -1)
            break;
        currStage = nextStage;
        step++;
    }
    if (step == MAX_STEPS)
        error(TOO_MANY_STEPS);
    printf("\nHalt\n");
}

// Error handling function
static void error(int errorType) {
    if (errorType == INVALID_CONFIG) {
        fprintf(stderr, "configuration file: INVALID transition definition\n");
    }
    if (errorType == INVALID_SYMBOL) {
        fprintf(stderr, "input tape: UNDEFINED symbol occurred\n");
        exit(1);
    }
    if (errorType == TOO_MANY_STEPS) {
        printf("Running step exceeded limit, please check whether this will halt\n");
        exit(1);
    }
    if (errorType == MEMORY_RAN_OUT) {
        fprintf(stderr, "INSUFFICIENT MEMORY\n");
        exit(1);
    }
    if (errorType == OPEN_FILE_FAIL) {
        fprintf(stderr, "Open file failed\n");
    }
}

static int charCmp(const char *character, const char *string) {
    if (*character == string[0])
        return 0;
    return 1;
}
static void displaySequence(char *sequence, int length, int curr) {
    printf("\r");
    for (int i = 0; i < length; i++) {
        if (i == curr) {
            printf("[%c]", sequence[i]);
        } else {
            printf("%c", sequence[i]);
        }
        usleep(REFRESH_TIME);
    }
}
