#ifndef GRAPH_ORIENTED_NODE_H
#define GRAPH_ORIENTED_NODE_H

#include <inttypes.h>

#include "settings.h"

struct node {
    uint64_t id;
    uint64_t nodes[MAX_NEIGHBOURS];
    uint64_t int_number;
    uint64_t float_number;
    uint64_t bool_number;
    uint64_t string_size;
    uint64_t link_to_string;
    uint64_t link_to_prev_node;
};

struct node_list {
    struct node *node;
    struct node_list *prev;
};

#endif //GRAPH_ORIENTED_NODE_H
