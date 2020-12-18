#include <stdio.h>
#include <stdlib.h>
#include "work_with_catalog.h"
#include "warnings.h"
//#include "warnings.h"
//#include "binary_tree.h"

#define VERIFY_CATALOG_PTR                          \
        if(!catalog_name_nodes) {                   \
            warning("xxx", INFORMATION_ABOUT_CALL); \
            return CATALOG_BAD_POINTER;             \
        }

#define VERIFY_CATALOG_NODES_PTR                    \
        if(!catalog_name_nodes) {                   \
            warning("yyy", INFORMATION_ABOUT_CALL); \
            return CATALOG_BAD_NODES_PTR;              \
        }

#define VERIFY_CATALOG_BUFFER_PTR                    \
        if(!catalog_name_nodes) {                   \
            warning("zzz", INFORMATION_ABOUT_CALL); \
            return CATALOG_BAD_BUFFER_PTR;              \
        }



CATALOG_STATUS catalog_names_construct(Catalog_names* catalog_name_nodes) {
    VERIFY_CATALOG_PTR

    catalog_name_nodes->capacity_nodes    = BEGIN_SIZE_CATALOG;
    catalog_name_nodes->count_nodes       = 0;
    catalog_name_nodes->nodes             = (Node_characteristics*)calloc(BEGIN_SIZE_CATALOG, sizeof(Node_characteristics));
    catalog_name_nodes->is_someone_user_add_into_buffer = false;

    if(!catalog_name_nodes->nodes)
        return CATALOG_BAD_POINTER;

    return CATALOG_OKEY;
}

CATALOG_STATUS catalog_resize_nodes(Catalog_names* catalog_name_nodes) {
    VERIFY_CATALOG_PTR

    VERIFY_CATALOG_NODES_PTR

    Node_characteristics* new_nodes = (Node_characteristics*)realloc(catalog_name_nodes->nodes, catalog_name_nodes->capacity_nodes * 2 * sizeof(Node_characteristics));

    catalog_name_nodes->nodes           = new_nodes;
    catalog_name_nodes->capacity_nodes *= 2;

    VERIFY_CATALOG_NODES_PTR

    return CATALOG_OKEY;
}

CATALOG_STATUS catalog_resize_buffer(Catalog_names* catalog_name_nodes) {
    VERIFY_CATALOG_PTR

    VERIFY_CATALOG_BUFFER_PTR

    char* new_buffer = (char*)realloc(catalog_name_nodes->buffer, catalog_name_nodes->capacity_buffer * 2 * sizeof(char));

    catalog_name_nodes->buffer           = new_buffer;
    catalog_name_nodes->capacity_buffer *= 2;

    VERIFY_CATALOG_BUFFER_PTR

    return CATALOG_OKEY;
}

CATALOG_STATUS add_into_catalog(Catalog_names* catalog_name_nodes, size_t pos_into_buffer, size_t length) {
    VERIFY_CATALOG_PTR

    if(catalog_name_nodes->count_nodes + 1 >= catalog_name_nodes->capacity_nodes)
        catalog_resize_nodes(catalog_name_nodes);

    catalog_name_nodes->nodes[catalog_name_nodes->count_nodes].length_name                = length;
    catalog_name_nodes->nodes[catalog_name_nodes->count_nodes++].count_symbols_from_begin = pos_into_buffer;

    return CATALOG_OKEY;
}


CATALOG_STATUS write_into_catalog_buffer(Catalog_names* catalog_name_nodes, const char* line, const int length) {
    VERIFY_CATALOG_PTR

    VERIFY_CATALOG_BUFFER_PTR

    if(!catalog_name_nodes->is_someone_user_add_into_buffer) {
        catalog_name_nodes->size_buffer = catalog_name_nodes->capacity_buffer + 1;
        catalog_name_nodes->is_someone_user_add_into_buffer = true;
    }

    if(catalog_name_nodes->size_buffer + length >= catalog_name_nodes->capacity_buffer)
        catalog_resize_buffer(catalog_name_nodes);

    for(int i=0; i<length; ++i)
        catalog_name_nodes->buffer[i + catalog_name_nodes->size_buffer] = line[i];

    catalog_name_nodes->size_buffer += length + 1;

    return CATALOG_OKEY;
}

