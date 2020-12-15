#ifndef WORK_WITH_CATALOG_H
#define WORK_WITH_CATALOG_H

const int BEGIN_SIZE_CATALOG = 128;

struct Node_characteristics {
    size_t count_symbols_from_begin;
    size_t length_name;
};

struct Catalog_names {
    size_t count_nodes;
    size_t capacity;
    char* buffer;
    Node_characteristics* nodes;
};

enum CATALOG_STATUS {
    CATALOG_BAD_POINTER  = 1,
    CATALOG_OKEY         = 2,
    CATALOG_BAD_CAPACITY = 3,
};

CATALOG_STATUS catalog_names_construct(Catalog_names* catalog_name_nodes);

CATALOG_STATUS catalog_resize(Catalog_names* catalog_name_nodes);

CATALOG_STATUS add_into_catalog(Catalog_names* catalog_name_nodes, size_t pos_into_buffer, size_t length);

#endif // WORK_WITH_CATALOG_H
