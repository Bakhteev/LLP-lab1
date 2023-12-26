#include "../include/executor.h"

enum execute_status execute(FILE *file, struct request *request) {

    switch (request->type) {
        case CREATE: {
            enum crud_status status = create_node(
                    file,
                    request->parameters[0],
                    request->parameters[1],
                    request->parameters[2],
                    request->string
            );
            if (status != CRUD_OK) {
                return ERROR;
            }
            break;
        };
        case FIND_ALL: {
            enum crud_status status = find_all_nodes(
                    file,
                    &request->list
            );
            if (status != CRUD_OK) {
                return ERROR;
            }
            break;
        };
        case FIND_BY_ID: {
            enum crud_status status = find_node_by_id(
                    file,
                    request->parameters[0],
                    request->node
            );
            if (status == CRUD_ERROR) {
                return ERROR;
            }
            if (status == CRUD_UNKNOWN_RESULT) {
                return UNKNOWN_RESULT;
            }
            break;
        };
        case FIND_BY_FILED: {
            enum crud_status status = find_node_by_field(
                    file,
                    request->parameters[0],
                    request->parameters[1],
                    &request->list
            );
            if (status != CRUD_OK) {
                return ERROR;
            }
            break;
        };
        case UPDATE_BY_ID: {
            enum crud_status status = update_node_by_id(
                    file,
                    request->parameters[0],
                    request->parameters[1],
                    request->parameters[2]
            );
            if (status != CRUD_OK) {
                return ERROR;
            }
            break;
        };
        case CONNECT_BY_IDS: {
            enum crud_status status = connect_nodes_by_ids(
                    file,
                    request->parameters[0],
                    request->parameters[1]
            );
            if (status != CRUD_OK) {
                return ERROR;
            }
            break;
        };
        case DELETE_BY_ID: {
            enum crud_status status = delete_node_by_id(
                    file,
                    request->parameters[0]
            );
            if (status != CRUD_OK) {
                return ERROR;
            }
            break;
        }
        default: {
            printf("Unknown operation\n");
            return ERROR;
        }
    }
    return OK;
}
