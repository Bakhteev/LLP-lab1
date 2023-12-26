#include "../include/file.h"

uint64_t write(FILE *file, void *buffer, uint64_t offset, size_t size) {
    int err = fseek(file, offset, SEEK_SET);

    if (err == 0) {
        size_t length = fwrite(buffer, size, 1, file);
        offset = ftell(file);
        if (length < 1 || length > size)
            return 1;
    }

    return offset;
}

uint64_t read(FILE *file, void *buffer, uint64_t offset, size_t size) {
    int err = fseek(file, offset, SEEK_SET);

    if (err == 0) {
        size_t length = fread(buffer, size, 1, file);
        if (length < 1)
            return 1;
    }

    return 0;
}

int init_database(FILE *file) {

    struct file_info *fileInfo = malloc(sizeof(struct file_info));

    fileInfo->id_seq = 1;
    fileInfo->link_to_first_element = BAD_LINK;
    fileInfo->link_to_last_element = BAD_LINK;
    fileInfo->link_to_next_free_memory = sizeof(struct file_info);

    write(file, fileInfo, SEEK_SET, sizeof(struct file_info));

    return 0;
}
