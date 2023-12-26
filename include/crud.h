#ifndef GRAPH_ORIENTED_CRUD_H
#define GRAPH_ORIENTED_CRUD_H

#include <string.h>

#include "file.h"

enum crud_status {
    CRUD_OK = 0,
    CRUD_UNKNOWN_RESULT,
    CRUD_ERROR
};

enum crud_operation {
    CREATE = 0,
    FIND_ALL,
    FIND_BY_ID,
    FIND_BY_FILED,
    DELETE_BY_ID,
    UPDATE_BY_ID,
    CONNECT_BY_IDS
};

struct request {
    enum crud_operation type;
    uint64_t *parameters;
    char *string;
    struct node_list *list;
    struct node *node;
};

enum crud_status
create_node(FILE *file, uint64_t int_number, uint64_t float_number, uint64_t bool_number, char *string);

enum crud_status find_all_nodes(FILE *file, struct node_list **list);

enum crud_status find_node_by_id(FILE *file, uint64_t id, struct node *node);

enum crud_status find_node_by_field(FILE *file, uint64_t field_name, uint64_t value, struct node_list **list);

enum crud_status update_node_by_id(FILE *file, uint64_t id, uint64_t field_name, uint64_t new_value);

enum crud_status connect_nodes_by_ids(FILE *file, uint64_t id_1, uint64_t id_2);

enum crud_status delete_node_by_id(FILE *file, uint64_t id);

void free_node_list(struct node_list *node_list);

#endif //GRAPH_ORIENTED_CRUD_H
