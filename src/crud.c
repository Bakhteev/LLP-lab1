#include "../include/crud.h"

void append_to_node_list(struct node **node_to_add, struct node_list **result) {

    if ((*result) == NULL) {
        (*result) = malloc(sizeof(struct node_list));
        (*result)->prev = NULL;

    } else {
        struct node_list *new_node_list = malloc(sizeof(struct node_list));
        new_node_list->prev = *result;
        *result = new_node_list;
    }

    (*result)->node = *node_to_add;
    (*node_to_add) = malloc(sizeof(struct node));
}

void free_node_list(struct node_list *node_list) {
    if (node_list != NULL) {
        struct node_list *next;
        while (node_list != NULL) {
            next = node_list->prev;
            free(node_list->node);
            free(node_list);
            node_list = next;
        }
    }
}

enum crud_status
create_node(FILE *file, uint64_t int_number, uint64_t float_number, uint64_t bool_number, char *string) {

    struct file_info *fileInfo = malloc(sizeof(struct file_info));
    read(file, fileInfo, SEEK_SET, sizeof(struct file_info));

    struct node *node = malloc(sizeof(struct node));

    node->id = fileInfo->id_seq;

    for (size_t i = 0; i < MAX_NEIGHBOURS; i++) {
        node->nodes[i] = BAD_LINK;
    }

    node->int_number = int_number;
    node->float_number = float_number;
    node->bool_number = bool_number;
    node->string_size = strlen(string);
    node->link_to_string = fileInfo->link_to_next_free_memory;

    fileInfo->link_to_next_free_memory = write(file, string, fileInfo->link_to_next_free_memory,
                                               node->string_size);

    node->link_to_prev_node = fileInfo->link_to_last_element;
    if (fileInfo->id_seq == 1) {
        fileInfo->link_to_first_element = fileInfo->link_to_next_free_memory;
    }
    fileInfo->link_to_last_element = fileInfo->link_to_next_free_memory;
    fileInfo->link_to_next_free_memory = write(file, node, fileInfo->link_to_next_free_memory, sizeof(struct node));

    fileInfo->id_seq = fileInfo->id_seq + 1;
    write(file, fileInfo, SEEK_SET, sizeof(struct file_info));

    free(node);
    free(fileInfo);
    return CRUD_OK;
}

enum crud_status find_all_nodes(FILE *file, struct node_list **list) {

    struct file_info *fileInfo = malloc(sizeof(struct file_info));
    read(file, fileInfo, SEEK_SET, sizeof(struct file_info));

    uint64_t offset = fileInfo->link_to_last_element;
    struct node *node;

    for (size_t i = 0; offset != BAD_LINK; i++) {
        node = malloc(sizeof(struct node));
        read(file, node, offset, sizeof(struct node));
        offset = node->link_to_prev_node;
        append_to_node_list(&node, list);
    }

    free(fileInfo);

    return CRUD_OK;
}

enum crud_status find_node_by_id(FILE *file, uint64_t id, struct node *node) {
    struct file_info *fileInfo = malloc(sizeof(struct file_info));
    read(file, fileInfo, SEEK_SET, sizeof(struct file_info));

    uint64_t offset = fileInfo->link_to_last_element;
    for (size_t i = 0; offset != BAD_LINK; i++) {
        read(file, node, offset, sizeof(struct node));

        if (node->id == id) {
            free(fileInfo);
            return CRUD_OK;
        }

        offset = node->link_to_prev_node;
    }

    free(fileInfo);

    return CRUD_UNKNOWN_RESULT;
}

enum crud_status find_node_by_field(FILE *file, uint64_t field_name, uint64_t value, struct node_list **list) {
    struct file_info *fileInfo = malloc(sizeof(struct file_info));
    read(file, fileInfo, SEEK_SET, sizeof(struct file_info));

    uint64_t offset = fileInfo->link_to_last_element;
    struct node *node;
    for (size_t i = 0; offset != BAD_LINK; i++) {
        node = malloc(sizeof(struct node));
        read(file, node, offset, sizeof(struct node));
        offset = node->link_to_prev_node;

        if (strcmp((char *) field_name, BOOL) == 0) {
            if (value == node->bool_number)
                append_to_node_list(&node, list);

        } else if (strcmp((char *) field_name, INTEGER) == 0) {
            if (value == node->int_number)
                append_to_node_list(&node, list);

        } else if (strcmp((char *) field_name, FLOAT) == 0) {
            if (value == node->float_number)
                append_to_node_list(&node, list);

        } else if (strcmp((char *) field_name, STRING) == 0) {
            char *str = calloc(node->string_size, sizeof(char));
            read(file, str, node->link_to_string, node->string_size);

            if (strcmp(str, (char *) value) == 0) {
                append_to_node_list(&node, list);
            }
        } else {
            free(node);
            free(fileInfo);
            return CRUD_ERROR;
        }
        free(node);
    }

    free(fileInfo);
    return CRUD_OK;
}

enum crud_status update_node_by_id(FILE *file, uint64_t id, uint64_t field_name, uint64_t new_value) {
    struct file_info *fileInfo = malloc(sizeof(struct file_info));
    read(file, fileInfo, SEEK_SET, sizeof(struct file_info));

    struct node *node = malloc(sizeof(struct node));
    enum crud_status status = find_node_by_id(file, id, node);

    if (status != CRUD_OK) {
        free(node);
        free(fileInfo);
        return CRUD_ERROR;
    }

    uint64_t offset_update_node = ftell(file) - sizeof(struct node);

    if (strcmp((char *) field_name, INTEGER) == 0) {
        node->int_number = new_value;
    } else if (strcmp((char *) field_name, FLOAT) == 0) {
        node->float_number = new_value;
    } else if (strcmp((char *) field_name, BOOL) == 0) {
        node->bool_number = new_value;
    } else if (strcmp((char *) field_name, STRING) == 0) {
        if (strlen((char *) new_value) > node->string_size) {
            node->string_size = strlen((char *) new_value);
            node->link_to_string = fileInfo->link_to_next_free_memory;
            fileInfo->link_to_next_free_memory = write(
                    file,
                    (char *) new_value,
                    fileInfo->link_to_next_free_memory,
                    node->string_size
            );

            write(file, fileInfo, SEEK_SET, sizeof(struct file_info));

        } else {
            node->string_size = strlen((char *) new_value);
            write(file, (char *) new_value, node->link_to_string, node->string_size);
        }
    } else {
        free(fileInfo);
        free(node);
        return CRUD_UNKNOWN_RESULT;
    }

    write(file, node, offset_update_node, sizeof(struct node));

    free(node);
    free(fileInfo);
    return CRUD_OK;
}

enum crud_status connect_nodes_by_ids(FILE *file, uint64_t id_1, uint64_t id_2) {

    struct node *first_node = malloc(sizeof(struct node));
    enum crud_status status = find_node_by_id(file, id_1, first_node);
    uint64_t first_node_offset = ftell(file) - sizeof(struct node);

    if (status != CRUD_OK) {
        free(first_node);
        return CRUD_ERROR;
    }

    struct node *second_node = malloc(sizeof(struct node));
    status = find_node_by_id(file, id_2, second_node);
    uint64_t second_node_offset = ftell(file) - sizeof(struct node);

    if (status != CRUD_OK) {
        free(first_node);
        free(second_node);
        return CRUD_ERROR;
    }

    int count_connected = 0;
    for (size_t i = 0; i < MAX_NEIGHBOURS; i++) {
        if (first_node->nodes[i] == BAD_LINK || first_node->nodes[i] == second_node->id) {
            first_node->nodes[i] = second_node->id;
            count_connected++;
            break;
        }
    }
    for (size_t i = 0; i < MAX_NEIGHBOURS; i++) {
        if (second_node->nodes[i] == BAD_LINK || first_node->nodes[i] == second_node->id) {
            second_node->nodes[i] = first_node->id;
            count_connected++;
            break;
        }
    }
    if (count_connected != 2) {
        free(first_node);
        free(second_node);
        return CRUD_ERROR;
    }

    write(file, first_node, first_node_offset, sizeof(struct node));
    write(file, second_node, second_node_offset, sizeof(struct node));

    free(first_node);
    free(second_node);
    return CRUD_OK;
}

enum crud_status delete_node_by_id(FILE *file, uint64_t id) {
    struct file_info *fileInfo = malloc(sizeof(struct file_info));
    read(file, fileInfo, SEEK_SET, sizeof(struct file_info));

    struct node *delete_node = malloc(sizeof(struct node));
    enum crud_status status = find_node_by_id(file, id, delete_node);
    uint64_t offset_delete_node = ftell(file) - sizeof(struct node);

    if (status != CRUD_OK) {
        free(delete_node);
        free(fileInfo);
        return CRUD_ERROR;
    }

    struct node *last_node = malloc(sizeof(struct node));
    read(file, last_node, fileInfo->link_to_last_element, sizeof(struct node));
    uint64_t offset_last_node = ftell(file) - sizeof(struct node);

    if (offset_delete_node == fileInfo->link_to_last_element) {
        fileInfo->link_to_last_element = last_node->link_to_prev_node;
        fileInfo->link_to_next_free_memory = offset_delete_node;

    } else if (offset_delete_node == fileInfo->link_to_first_element) {

        fileInfo->link_to_last_element = last_node->link_to_prev_node;
        last_node->link_to_prev_node = BAD_LINK;
        fileInfo->link_to_next_free_memory = offset_last_node;
        write(file, last_node, offset_delete_node, sizeof(struct node));

    } else {
        fileInfo->link_to_next_free_memory = fileInfo->link_to_last_element;
        fileInfo->link_to_last_element = last_node->link_to_prev_node;
        last_node->link_to_prev_node = delete_node->link_to_prev_node;

        write(file, last_node, offset_delete_node, sizeof(struct node));
    }

    write(file, fileInfo, SEEK_SET, sizeof(struct file_info));

    free(last_node);
    free(fileInfo);

    for (size_t i = 0; i < MAX_NEIGHBOURS; i++) {
        if (delete_node->nodes[i] != BAD_LINK) {
            delete_node_by_id(file, delete_node->nodes[i]);
        }
    }

    free(delete_node);

    return CRUD_OK;
}