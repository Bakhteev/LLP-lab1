#ifndef GRAPH_ORIENTED_FILE_INFO_H
#define GRAPH_ORIENTED_FILE_INFO_H

#include <inttypes.h>

struct file_info {
    uint64_t id_seq;
    uint64_t link_to_first_element;
    uint64_t link_to_last_element;
    uint64_t link_to_next_free_memory;
};

#endif //GRAPH_ORIENTED_FILE_INFO_H
