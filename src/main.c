#include "../include/test.h"

int main(int argc, char **argv) {
    FILE *file = NULL;

    if (argc > 1 && argv[1] != NULL) {
        file = fopen(argv[1], "r+");
    }

    if (file == NULL) {
        printf("No such file\n");
        return 1;
    }

    test(file);

    if (file != NULL)
        fclose(file);

    return 0;
}


/* check file's size after insert node

   for (int i = 0; i < 100; i++) {
       create_node(file, intgr, dbl, bln, str);
       fseek(file, 0, SEEK_END);
       printf("%ld\n", ftell(file));
   }

*/

// utilit "gnome monitor" for check program's RAM
