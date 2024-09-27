#ifndef T9_H
#define T9_H

#include <stdint.h>

#include "dictionary.h"

#define MAX_T9_WORDS 10
#define MAX_STACK (DICTIONARY_MAX_WORD_SIZE * 2 + 1)
#define DICTIONARY_NODE_SIZE 6
#define T9_NODE_SIZE DICTIONARY_NODE_SIZE + DICTIONARY_MAX_WORD_SIZE + 1
#define MAX_T9_RESULTS 10
#define T9_RESULT_SIZE DICTIONARY_MAX_WORD_SIZE + 1

typedef struct DictionaryNode {
    char c;
    uint16_t children;
    uint8_t child_count;
    uint8_t complete;
    uint8_t score;
} DictionaryNode;

typedef struct T9Node {
    DictionaryNode dictionary_node;
    uint8_t depth;
    char value[DICTIONARY_MAX_WORD_SIZE];
} T9Node;

typedef struct T9Result {
    uint8_t score;
    char value[DICTIONARY_MAX_WORD_SIZE];
} T9Result;

typedef struct T9Results {
    uint8_t result_count;
    T9Result results[MAX_T9_RESULTS];
} T9Results;

void t9_algorithm(char* val, uint8_t size, T9Results* result);

#endif
