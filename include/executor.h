#ifndef GRAPH_ORIENTED_EXECUTOR_H
#define GRAPH_ORIENTED_EXECUTOR_H

#include "crud.h"

enum execute_status {
    OK = 0,
    UNKNOWN_RESULT,
    ERROR
};

enum execute_status execute(FILE *file, struct request *request);

#endif //GRAPH_ORIENTED_EXECUTOR_H
