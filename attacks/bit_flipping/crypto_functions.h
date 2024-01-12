#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#define DEVICE_DIR 	"/sys/bus/platform/devices/crypto_core@8000000"
#define DIR_PROC_ID 	"/proc_id"
#define DIR_MODE	"/mode"
#define DIR_FORMAT	"/format"
#define DIR_START	"/start"
#define DIR_VALID	"/valid"
#define DIR_KEY		"/key_char"
#define DIR_IV		"/iv_char"
#define DIR_IN		"/in_char"
#define DIR_OUT		"/out_char"
#define DIR_OUT_HEX	"/out_hex"


int dev_file_open(char *subdir, int flag);
void open_descriptors(int *fd);
void close_descriptors(int *fd);
void read_reg(int fd, char *buf);
void read_reg_to_length(int fd, char *buf, int length);
void write_reg(int fd, char *buf, int bytes);
void get_core_id(int *fd, char* id_buf);
void write_key(int *fd, char* key_buf);
void write_iv(int *fd, char* iv_buf);
void write_input(int *fd, char* in_buf);
void set_mode(int *fd, int enc_dec);
void set_format(int *fd, int aes_format);
void start_operation(int *fd);
void read_output(int *fd, char* out_buf, int length);
void read_output_hex(int *fd, char* out_buf, int length);
int is_output_valid(int *fd);
void clear_valid(int *fd);
