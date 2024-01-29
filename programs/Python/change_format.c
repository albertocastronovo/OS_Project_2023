#include "crypto_functions.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s [format]\n", argv[0]);
        return 1;
    }

    int fd = dev_file_open(DIR_FORMAT, O_WRONLY);
    write(fd, argv[1], strlen(argv[1]));
    close(fd);

    return 0;
}
