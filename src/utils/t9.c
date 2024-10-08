#include "t9.h"
#include "string.h"
#include <gb/gb.h>
#include <stdint.h>
#include "dictionary.h"
#include "dictionary_31.h"
#include "dictionary_30.h"
#include "dictionary_29.h"
#include "dictionary_28.h"
#include "dictionary_27.h"
#include "dictionary_26.h"
#include "../core/print.h"

BANKREF_EXTERN(dictionary_26)
BANKREF_EXTERN(dictionary_27)
BANKREF_EXTERN(dictionary_28)
BANKREF_EXTERN(dictionary_29)
BANKREF_EXTERN(dictionary_30)
BANKREF_EXTERN(dictionary_31)

void get_node(DictionaryNode* node, uint16_t index) {
    uint8_t save = CURRENT_BANK;

    uint8_t* dictionary_tree;

    if(index < DICTIONARY_NODE_COUNT_31) {
        SWITCH_ROM(BANK(dictionary_31));
        dictionary_tree = (uint8_t*)dictionary_tree_31;
    } else if(index < DICTIONARY_NODE_COUNT_31 + DICTIONARY_NODE_COUNT_30) {
        SWITCH_ROM(BANK(dictionary_30));
        dictionary_tree = (uint8_t*)dictionary_tree_30;
        index -= DICTIONARY_NODE_COUNT_31;
    } else if(index < DICTIONARY_NODE_COUNT_31 + DICTIONARY_NODE_COUNT_30 + DICTIONARY_NODE_COUNT_29) {
        SWITCH_ROM(BANK(dictionary_29));
        dictionary_tree = (uint8_t*)dictionary_tree_29;
        index -= DICTIONARY_NODE_COUNT_31 + DICTIONARY_NODE_COUNT_30;
    } else if(index < DICTIONARY_NODE_COUNT_31 + DICTIONARY_NODE_COUNT_30 + DICTIONARY_NODE_COUNT_29 + DICTIONARY_NODE_COUNT_28) {
        SWITCH_ROM(BANK(dictionary_28));
        dictionary_tree = (uint8_t*)dictionary_tree_28;
        index -= DICTIONARY_NODE_COUNT_31 + DICTIONARY_NODE_COUNT_30 + DICTIONARY_NODE_COUNT_29;
    } else if(index < DICTIONARY_NODE_COUNT_31 + DICTIONARY_NODE_COUNT_30 + DICTIONARY_NODE_COUNT_29 + DICTIONARY_NODE_COUNT_28 + DICTIONARY_NODE_COUNT_27) {
        SWITCH_ROM(BANK(dictionary_27));
        dictionary_tree = (uint8_t*)dictionary_tree_27;
        index -= DICTIONARY_NODE_COUNT_31 + DICTIONARY_NODE_COUNT_30 + DICTIONARY_NODE_COUNT_29 + DICTIONARY_NODE_COUNT_28;
    } else if(index < DICTIONARY_NODE_COUNT_31 + DICTIONARY_NODE_COUNT_30 + DICTIONARY_NODE_COUNT_29 + DICTIONARY_NODE_COUNT_28 + DICTIONARY_NODE_COUNT_27 + DICTIONARY_NODE_COUNT_26) {
        SWITCH_ROM(BANK(dictionary_26));
        dictionary_tree = (uint8_t*)dictionary_tree_26;
        index -= DICTIONARY_NODE_COUNT_31 + DICTIONARY_NODE_COUNT_30 + DICTIONARY_NODE_COUNT_29 + DICTIONARY_NODE_COUNT_28 + DICTIONARY_NODE_COUNT_27;
    }

    uint8_t byte1 =  dictionary_tree[index * 4];
    uint8_t byte2 = dictionary_tree[index * 4 + 1];
    uint8_t byte3 = dictionary_tree[index * 4 + 2];
    uint8_t byte4 = dictionary_tree[index * 4 + 3];

    node->c = 'a' + ((byte1 & 0b11111000) >> 3);
    node->children = (((uint16_t)byte1 & 0b00000111) << 12) | (uint16_t)(byte2 << 4) | (uint16_t)(byte3 >> 4);
    node->child_count = ((byte3 & 0b00001111) << 1) | ((byte4 & 0b10000000) >> 7);
    node->score = (byte4 & 0b01111110) >> 1;
    node->complete = byte4 & 0b00000001;

    SWITCH_ROM(save);
}

void t9_letter_to_number(char* val, uint8_t size) {
    for(uint8_t i = 0; i < size; i++) {
        // to lower
        if(val[i] >= 'A' && val[i] <= 'Z') {
            val[i] += 'a' - 'A';
        }

        if(val[i] >= 'a' && val[i] <= 'z') {
            val[i] -= 'a' + (val[i] > 'p') + (val[i] > 'w');
            val[i] /= 3;
            val[i] += '2';
        }
    }
}

void insert_result(T9Results* results, T9Result* result) {
    if(results->result_count < MAX_T9_RESULTS) {
        results->results[results->result_count].score = 0;
        results->result_count++;
    } else if(result->score <= results->results[results->result_count - 1].score) {
        return;
    }

    uint8_t i;

    for (i = results->result_count - 1; i > 0; i--) {
        if (result->score > results->results[i - 1].score) {
            memcpy(&results->results[i], &results->results[i - 1], T9_RESULT_SIZE);
        } else {
            break;
        }
    }

    memcpy(&results->results[i], result, T9_RESULT_SIZE);

    if (results->result_count > MAX_T9_RESULTS) {
        results->result_count = MAX_T9_RESULTS;
    }
}


void t9_algorithm(char* val, uint8_t size, T9Results* results) {
    t9_letter_to_number(val, size);

    uint16_t stack_tail = 0;
    T9Node stack[MAX_STACK];

    uint8_t key = val[0] - '2';
    uint8_t letter_count = 3 + (key == 5 || key == 7);
    uint8_t letter_start = key * 3 + (key > 5);

    for(uint8_t i = 0; i < letter_count; i++) {
        stack[stack_tail].depth = 1;
        stack[stack_tail].value[0] = 'a' + letter_start + i;
        get_node(&stack[stack_tail].dictionary_node, letter_start + i);
        stack_tail++;
    }

    while(stack_tail > 0) {
        stack_tail--;

        T9Node parent;
        memcpy(&parent, &stack[stack_tail], T9_NODE_SIZE);

        if(parent.depth > size) {
            break;
        }

        if(parent.depth == size) {
            if(parent.dictionary_node.complete) {
                T9Result result = {
                    .score = parent.dictionary_node.complete ? parent.dictionary_node.score : 0,
                };

                memcpy(&result.value, &parent.value, size);
                result.value[size] = '\0';

                insert_result(results, &result);
            }

            continue;
        }

        if(parent.dictionary_node.child_count == 0) {
            continue;
        }

        key = val[parent.depth] - '2';
        letter_count = 3 + (key == 5 || key == 7);
        letter_start = key * 3 + (key > 5);

        for(uint8_t i = 0; i < letter_count; i++) {
            char next_char = 'a' + letter_start + i;
            uint8_t found_child = 0;
            DictionaryNode child_node;

            for(uint8_t j = 0; j < parent.dictionary_node.child_count; j++) {
                get_node(&child_node, parent.dictionary_node.children+j);

                if(child_node.c == next_char) {
                    found_child = 1;
                } 

                if(child_node.c >= next_char) {
                    break;
                }
            }

            if(!found_child) continue;

            memcpy(&stack[stack_tail].dictionary_node, &child_node, DICTIONARY_NODE_SIZE);
            stack[stack_tail].depth = parent.depth+1;
            memcpy(stack[stack_tail].value, parent.value, parent.depth);
            stack[stack_tail].value[parent.depth] = next_char;
            stack_tail++;
        }
    }
}

