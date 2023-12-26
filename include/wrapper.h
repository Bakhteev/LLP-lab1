#ifndef GRAPH_ORIENTED_WRAPPER_H
#define GRAPH_ORIENTED_WRAPPER_H

#include <time.h>
#include "crud.h"

void create_node_wrap(FILE *file, uint64_t int_number, uint64_t float_number, uint64_t bool_number, char *string);

void find_node_by_id_wrap(FILE *file, uint64_t id, struct node *node);

void find_node_by_field_wrap(FILE *file, uint64_t field_name, uint64_t value, struct node_list **list);

void update_node_by_id_wrap(FILE *file, uint64_t id, uint64_t field_name, uint64_t new_value);

void connect_nodes_by_ids_wrap(FILE *file, uint64_t id_1, uint64_t id_2);

void delete_node_by_id_wrap(FILE *file, uint64_t id);

#endif //GRAPH_ORIENTED_WRAPPER_H
