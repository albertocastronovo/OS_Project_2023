#include "crypto_functions.h"

int main(int argc, char **argv) {
    if (argc != 6) {
        printf("Usage: %s [key] [iv] [plaintext] [mode] [format]\n", argv[0]);
        return 1;
    }

    int fd[10];
    open_descriptors(fd);

    write(fd[5], argv[1], strlen(argv[1])); // Write key
    write(fd[6], argv[2], strlen(argv[2])); // Write IV
    write(fd[7], argv[3], strlen(argv[3])); // Write plaintext
    write(fd[1], argv[4], strlen(argv[4])); // Set mode
    write(fd[2], argv[5], strlen(argv[5])); // Set format

    // Start conversion
    char start_buf[] = "1";
    write(fd[3], start_buf, strlen(start_buf)); 

    //Wait for valid output
    char valid_value[2];
    do {
        read(fd[4], valid_value, 1);
    } while (valid_value[0] != '1');

    // Read output
    char output[100];
    read_output_hex(fd, output, sizeof(output));
    //read(fd[8], output, sizeof(output));
    printf("Output: %s\n", output);

    close_descriptors(fd);
    return 0;
}
