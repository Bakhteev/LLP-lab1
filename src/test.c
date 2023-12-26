#include "../include/test.h"

void print_node(FILE *file, struct node *node) {

    printf("id - %lu\n", node->id);
    printf("connected nodes:\n");

    int count = 0;
    for (size_t i = 0; i < MAX_NEIGHBOURS; i++) {
        if (node->nodes[i] != BAD_LINK) {
            printf("%lu ", node->nodes[i]);
            count++;
        }
    }
    if (count != 0) printf("\n");

    printf("%s - %lu\n", INTEGER, node->int_number);
    float f;
    memcpy(&f, &node->float_number, sizeof(float));
    printf("%s - %.2f\n", FLOAT, f);
    printf("%s - %lu\n", BOOL, node->bool_number);

    char *str = calloc(node->string_size, sizeof(char));
    read(file, str, node->link_to_string, node->string_size);

    printf("%s - %s\n\n", STRING, str);
    free(str);
}

void print_node_list(FILE *file, struct node_list *list) {
    if (list != NULL) {
        struct node_list *next;
        while (list != NULL) {
            next = list->prev;
            print_node(file, list->node);
            list = next;
        }
    }
}

void create(FILE *file, uint64_t int_value) {

    struct request *request = malloc(sizeof(struct request));
    request->parameters = malloc(sizeof(uint64_t) * 3);
    request->type = CREATE;
    request->parameters[0] = int_value;
    float f = 22.2;
    memcpy(&request->parameters[1], &f, sizeof(float));
    request->parameters[2] = 1;
    request->string = "Hello world";

    enum execute_status status = execute(file, request);
    if (status != OK) {
        printf("create error\n");
        free(request->parameters);
        free(request);
        return;
    }

    free(request->parameters);
    free(request->string);
    free(request);
}

void find_all(FILE *file) {

    struct request *request = malloc(sizeof(struct request));
    request->type = FIND_ALL;
    request->list = NULL;

    enum execute_status status = execute(file, request);
    if (status != OK) {
        printf("find all error\n");
        free_node_list(request->list);
        free(request);
        return;
    }

    print_node_list(file, request->list);

    free_node_list(request->list);
    free(request);
}

void find_by_id(FILE *file, uint64_t id) {

    struct request *request = malloc(sizeof(struct request));
    request->type = FIND_BY_ID;
    request->parameters = malloc(sizeof(uint64_t));
    request->parameters[0] = id;
    request->node = malloc(sizeof(struct node));

    enum execute_status status = execute(file, request);
    if (status == ERROR) {
        printf("find by id error\n");
        free(request->parameters);
        free(request->node);
        free(request);
        return;
    }
    if (status == UNKNOWN_RESULT) {
        printf("no such node by id\n");
        free(request->parameters);
        free(request->node);
        free(request);
        return;
    }

    print_node(file, request->node);

    free(request->parameters);
    free(request->node);
    free(request);
}

void find_by_field(FILE *file, uint64_t field_name, uint64_t value) {

    struct request *request = malloc(sizeof(struct request));
    request->type = FIND_BY_FILED;
    request->list = NULL;
    request->parameters = malloc(sizeof(uint64_t) * 2);
    request->parameters[0] = field_name;
    request->parameters[1] = value;

    enum execute_status status = execute(file, request);
    if (status != OK) {
        printf("find by field error\n");
        free(request->parameters);
        free_node_list(request->list);
        free(request);
        return;
    }

    print_node_list(file, request->list);

    free(request->parameters);
    free_node_list(request->list);
    free(request);
}

void update_by_id(FILE *file, uint64_t id, uint64_t field_name, uint64_t value) {
    struct request *request = malloc(sizeof(struct request));
    request->type = UPDATE_BY_ID;
    request->list = NULL;
    request->parameters = malloc(sizeof(uint64_t) * 3);
    request->parameters[0] = id;
    request->parameters[1] = field_name;
    request->parameters[2] = value;

    enum execute_status status = execute(file, request);
    if (status != OK) {
        printf("update by id error\n");
        free(request->parameters);
        free(request);
        return;
    }

    free(request->parameters);
    free(request);
}

void connected_nodes(FILE *file, uint64_t id_1, uint64_t id_2) {
    struct request *request = malloc(sizeof(struct request));
    request->type = CONNECT_BY_IDS;
    request->list = NULL;
    request->parameters = malloc(sizeof(uint64_t) * 2);
    request->parameters[0] = id_1;
    request->parameters[1] = id_2;

    enum execute_status status = execute(file, request);
    if (status != OK) {
        printf("connected nodes error\n");
        free(request->parameters);
        free(request);
        return;
    }

    free(request->parameters);
    free(request);
}

void delete_by_id(FILE *file, uint64_t id) {
    struct request *request = malloc(sizeof(struct request));
    request->type = DELETE_BY_ID;
    request->list = NULL;
    request->parameters = malloc(sizeof(uint64_t) * 1);
    request->parameters[0] = id;

    enum execute_status status = execute(file, request);
    if (status != OK) {
        printf("delete by id error\n");
        free(request->parameters);
        free(request);
        return;
    }

    free(request->parameters);
    free(request);
}

void test(FILE *file) {

    init_database(file);

    create(file, 22);
    create(file, 22);
    printf("FIND ALL ELEMENTS\n");
    find_all(file);
    printf("ADD ELEMENT\n");
    create(file, 33);
    printf("FIND ALL ELEMENTS\n");
    find_all(file);
    printf("FIND BY ID 2\n");
    find_by_id(file, 2);
    printf("FIND BY FIELD INTEGER 33\n");
    find_by_field(file, (uint64_t) INTEGER, 33);
    printf("UPDATE BY ID INTEGER 123\n");
    update_by_id(file, 1, (uint64_t) INTEGER, 123);
    printf("CONNECT BY IDs 1 and 2\n");
    connected_nodes(file, 1, 2);
    printf("FIND ALL ELEMENTS\n");
    find_all(file);
    printf("DELETE BY ID 3\n");
    delete_by_id(file, 3);
    printf("FIND ALL ELEMENTS\n");
    find_all(file);
}