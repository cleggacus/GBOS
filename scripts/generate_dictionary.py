from __future__ import annotations
from queue import Queue
import math
import sys
import os

# Configuration for the C array
DICTIONARY_SIZE_MACRO = 'DICTIONARY_SIZE'
MAX_WORD_SIZE_MACRO = 'MAX_WORD_SIZE'

a_ascii = ord("a")

class Node:
    complete: bool
    score: int
    child_counter: int
    children: list[Node | None]

    def __init__(self):
        self.complete = False
        self.children = [None] * 26
        self.child_counter = 0
        self.score = 0

    def count(self):
        count = 1
        for child in self.children:
            if(child is not None):
                count += child.count()
        return count


    def push(self, val: str, score: int):
        if len(val) == 0:
            self.complete = True
            self.score = score
            return

        index = ord(val[0]) - a_ascii

        if self.children[index] is None:
            self.child_counter += 1
            self.children[index] = Node()

        node = self.children[index]
        assert node is not None
        node.push(val[1:], score)


def serialize(root: Node):
    queue = Queue()
    queue.put((root, -1))

    node_list: list[tuple[int, int, int, int, bool]] = []

    while not queue.empty():
        (node, parent_index) = queue.get()

        if parent_index >= 0:
            node_list[parent_index] = (node_list[parent_index][0], len(node_list), node_list[parent_index][2], node_list[parent_index][3], node_list[parent_index][4])

        # if parent_index == ord('m') - ord('a'):
        #     for i in range(0, 26):
        #         print(chr(ord('a') + i), node.children[i])

        for i, child in enumerate(node.children):
            if child is None:
                continue

            char = i
            child_ptr = 0 # no children is 0
            child_count = child.child_counter
            complete = child.complete
            score = child.score

            assert 0 <= char < 32
            assert 0 <= child_ptr < (1 << 15)
            assert 0 <= child_count < 32
            assert score < 128

            queue.put((child, len(node_list)))
            node_list.append((char, child_ptr, child_count, score, complete))

    node_count = len(node_list)
    node_bytes = bytearray(node_count * 4)

    # turn nodes in to compact 4 byte nodes

    for i, (char, child_ptr, child_count, score, complete) in enumerate(node_list):
        val = ((char & 0b11111) << 27) | \
            ((child_ptr & 0b111111111111111) << 12) | \
            ((child_count & 0b11111) << 7) | \
            ((score & 0b111111) << 1) | \
            (complete & 1)

        val_bytes = val.to_bytes(4, 'big')

        node_bytes[i*4] = val_bytes[0]
        node_bytes[i*4+1] = val_bytes[1]
        node_bytes[i*4+2] = val_bytes[2]
        node_bytes[i*4+3] = val_bytes[3]

    return node_bytes


def generate_c_file(input_file, output_file_prefix, num_files):
    # Read the words from the input text file
    root = Node()
    dictionary_size = 0
    max_word_size = 0

    for j in range(0, 26):
        root.push(chr(a_ascii + j), 0)
        dictionary_size += 1

    with open(input_file, 'r') as file:
        for i, line in enumerate(file):
            dictionary_size += 1

    with open(input_file, 'r') as file:
        for i, line in enumerate(file):
            word = line.strip()
            score = math.floor(0b111111 * (1 - i / dictionary_size))

            if len(word) > max_word_size:
                max_word_size = len(word)

            root.push(word, score)

    node_bytes = serialize(root)
    node_count = len(node_bytes) // 4

    # Determine the number of nodes per file
    nodes_per_file = node_count // num_files
    remainder = node_count % num_files

    start_index = 0


    h_code = f"""#ifndef DICTIONARY_H
#define DICTIONARY_H

#define DICTIONARY_SIZE {dictionary_size}
#define DICTIONARY_MAX_WORD_SIZE {max_word_size}
#define DICTIONARY_NODE_COUNT {node_count}

#endif"""

    with open(f"{output_file_prefix}.h", 'w') as f:
        f.write(h_code)

    for i in range(num_files):
        # Determine the number of nodes for this file
        if i < remainder:
            nodes_this_file = nodes_per_file + 1  # Distribute the remainder
        else:
            nodes_this_file = nodes_per_file

        # Calculate byte range
        end_index = start_index + nodes_this_file * 4
        file_node_bytes = node_bytes[start_index:end_index]
        start_index = end_index

        h_code = f"""#ifndef DICTIONARY_BANK_{31 - i}_H
#define DICTIONARY_BANK_{31 - i}_H

#define DICTIONARY_SIZE_{31 - i} {dictionary_size}
#define DICTIONARY_MAX_WORD_SIZE_{31 - i} {max_word_size}
#define DICTIONARY_NODE_COUNT_{31 - i} {len(file_node_bytes) // 4}

#include <stdint.h>

extern const uint8_t dictionary_tree_{31 - i}[{len(file_node_bytes)}];

#endif"""

        c_code = f"""#pragma bank {31 - i}

#include "dictionary_{31 - i}.h"
#include <gb/gb.h>

BANKREF(dictionary_{31 - i})

const uint8_t dictionary_tree_{31 - i}[{len(file_node_bytes)}] = {{"""

        for j, byte in enumerate(file_node_bytes):
            if j % 4 == 0:
                c_code += "\n    "

            c_code += "0x{:02X}".format(byte)

            if j < len(file_node_bytes) - 1:
                c_code += ", "

        c_code += "\n};\n"

        # Write the C code to the output file
        with open(f"{output_file_prefix}_{31 - i}.c", 'w') as f:
            f.write(c_code)

        with open(f"{output_file_prefix}_{31 - i}.h", 'w') as f:
            f.write(h_code)

        print(f"C file '{output_file_prefix}_{31 - i}' generated successfully.")


if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit(f"Incorrect arguments\npython {sys.argv[0]} <input_file> <output_file>") 

    if not os.path.exists(sys.argv[1]):
        print(f"Input file '{sys.argv[1]}' does not exist.")
    else:
        generate_c_file(sys.argv[1], sys.argv[2], 6)
