#include "crypto_functions.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s [plaintext]\n", argv[0]);
        return 1;
    }

    int fd[10];
    open_descriptors(fd);

    write(fd[7], argv[1], strlen(argv[1])); // Write plaintext

    // Start conversion
    char start_buf[] = "1";
    write(fd[3], start_buf, strlen(start_buf)); 

    // Wait for valid output
    char valid_value[2];
    do {
        read(fd[4], valid_value, 1);
    } while (valid_value[0] != '1');


    // Read output
    char output[100];
    read_output_hex(fd, output, sizeof(output));
    printf("Output: %s\n", output);

    close_descriptors(fd);
    return 0;
}
