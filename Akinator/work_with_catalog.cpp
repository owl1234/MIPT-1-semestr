#include <stdio.h>
#include <stdlib.h>
#include "work_with_catalog.h"
#include "binary_tree.h"
#include "warnings.h"

#define VERIFY_CATALOG_PTR                          \
        if(!catalog_name_nodes) {                   \
            warning("xxx", INFORMATION_ABOUT_CALL); \
            return CATALOG_BAD_POINTER;             \
        }

CATALOG_STATUS catalog_names_construct(Catalog_names* catalog_name_nodes) {
    VERIFY_CATALOG_PTR

    catalog_name_nodes->capacity    = BEGIN_SIZE_CATALOG;
    catalog_name_nodes->count_nodes = 0;
    catalog_name_nodes->nodes       = (Node_characteristics*)calloc(BEGIN_SIZE_CATALOG, sizeof(Node_characteristics));

    if(!catalog_name_nodes->nodes)
        return CATALOG_BAD_POINTER;

    return CATALOG_OKEY;
}

CATALOG_STATUS catalog_resize(Catalog_names* catalog_name_nodes) {
    VERIFY_CATALOG_PTR

    Node_characteristics* new_nodes = (Node_characteristics*)realloc(catalog_name_nodes->buffer, catalog_name_nodes->capacity * 2 * sizeof(Node_characteristics));

    catalog_name_nodes->nodes = new_nodes;
    catalog_name_nodes->capacity *= 2;

    return CATALOG_OKEY;
}

CATALOG_STATUS add_into_catalog(Catalog_names* catalog_name_nodes, size_t pos_into_buffer, size_t length) {
    VERIFY_CATALOG_PTR

    if(catalog_name_nodes->count_nodes + 1 >= catalog_name_nodes->capacity)
        catalog_resize(catalog_name_nodes);

    catalog_name_nodes->nodes[catalog_name_nodes->count_nodes].length_name                = length;
    catalog_name_nodes->nodes[catalog_name_nodes->count_nodes++].count_symbols_from_begin = pos_into_buffer;

    return CATALOG_OKEY;
}

