
//#ifndef WORK_WITH_CATALOG_H
//#define WORK_WITH_CATALOG_H

//#include "warnings.h"

const int BEGIN_SIZE_CATALOG = 128;

struct Node_characteristics {
    size_t count_symbols_from_begin;
    size_t length_name;
};

struct Catalog_names {
    size_t count_nodes;
    size_t capacity_nodes;
    Node_characteristics* nodes;

    char* buffer;
    size_t size_buffer;
    size_t capacity_buffer;
    bool is_someone_user_add_into_buffer;
};

enum CATALOG_STATUS {
    CATALOG_BAD_POINTER    = 1,
    CATALOG_OKEY           = 2,
    CATALOG_BAD_CAPACITY   = 3,
    CATALOG_BAD_NODES_PTR  = 4,
    CATALOG_BAD_BUFFER_PTR = 5,
};

CATALOG_STATUS catalog_names_construct(Catalog_names* catalog_name_nodes);

CATALOG_STATUS catalog_resize_nodes(Catalog_names* catalog_name_nodes);

CATALOG_STATUS catalog_resize_buffer(Catalog_names* catalog_name_nodes);

CATALOG_STATUS add_into_catalog(Catalog_names* catalog_name_nodes, size_t pos_into_buffer, size_t length);

CATALOG_STATUS write_into_catalog_buffer(Catalog_names* catalog_name_nodes, const char* line, const int length);

//#endif // WORK_WITH_CATALOG_H

