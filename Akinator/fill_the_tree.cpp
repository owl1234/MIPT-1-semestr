#include <stdio.h>
#include <stdlib.h>
#include "work_with_file.h"

size_t tree_count_nodes(File* base) {
    int is_found_node = 0;
    size_t count_nodes = 0;

    for(size_t i=0; i<base->size_buffer; ++i) {
        if(base->buffer[i] == '"') {
            if(!is_found_node) {
                ++count_nodes;
            }
            is_found_node = (is_found_node + 1) % 2;
        }
    }

    return count_nodes;
}


