#ifndef GRAPH_ORIENTED_FILE_H
#define GRAPH_ORIENTED_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "node.h"
#include "file_info.h"

uint64_t write(FILE *file, void *buffer, uint64_t offset, size_t size);

uint64_t read(FILE *file, void *buffer, uint64_t offset, size_t size);

int init_database(FILE *file);

#endif //GRAPH_ORIENTED_FILE_H
