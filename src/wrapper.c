#include "../include/wrapper.h"

void create_node_wrap(FILE *file, uint64_t int_number, uint64_t float_number, uint64_t bool_number, char *string) {
    clock_t cl = clock();
    create_node(file, int_number, float_number, bool_number, string);
    printf("%f\n", (double) (clock() - cl) / CLOCKS_PER_SEC);
}

void find_node_by_id_wrap(FILE *file, uint64_t id, struct node *node) {
    clock_t cl = clock();
    find_node_by_id(file, id, node);
    printf("%f\n", (double) (clock() - cl) / CLOCKS_PER_SEC);
}

void find_node_by_field_wrap(FILE *file, uint64_t field_name, uint64_t value, struct node_list **list) {
    clock_t cl = clock();
    find_node_by_field(file, field_name, value, list);
    printf("%f\n", (double) (clock() - cl) / CLOCKS_PER_SEC);
}

void update_node_by_id_wrap(FILE *file, uint64_t id, uint64_t field_name, uint64_t new_value) {
    clock_t cl = clock();
    update_node_by_id(file, id, field_name, new_value);
    printf("%f\n", (double) (clock() - cl) / CLOCKS_PER_SEC);
}

void connect_nodes_by_ids_wrap(FILE *file, uint64_t id_1, uint64_t id_2) {
    clock_t cl = clock();
    connect_nodes_by_ids(file, id_1, id_2);
    printf("%f\n", (double) (clock() - cl) / CLOCKS_PER_SEC);
}

void delete_node_by_id_wrap(FILE *file, uint64_t id) {
    clock_t cl = clock();
    delete_node_by_id(file, id);
    printf("%f\n", (double) (clock() - cl) / CLOCKS_PER_SEC);
}
