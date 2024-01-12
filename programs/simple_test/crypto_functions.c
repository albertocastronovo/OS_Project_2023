#include "crypto_functions.h"

/*
	dev_file_open is used for opening one of the device files
*/
int dev_file_open(char *subdir, int flag)
{
    int fd;
    char file_path[100];
    sprintf(file_path, "%s%s", DEVICE_DIR, subdir);
    fd = open(file_path, flag);
    if(fd == -1)
    {
        perror("error opening file");
        exit(EXIT_FAILURE);
    }
        
    return fd;
}


//	all file descriptors are opened, via multiple calls to dev_file_open
void open_descriptors(int *fd)
{
   	fd[0] = dev_file_open(DIR_PROC_ID,	O_RDONLY);
	fd[1] = dev_file_open(DIR_MODE,		O_RDWR);
	fd[2] = dev_file_open(DIR_FORMAT, 	O_RDWR);
	fd[3] = dev_file_open(DIR_START, 	O_RDWR);
	fd[4] = dev_file_open(DIR_VALID,	O_RDWR);
	fd[5] = dev_file_open(DIR_KEY,		O_WRONLY);
	fd[6] = dev_file_open(DIR_IV,		O_WRONLY);
	fd[7] = dev_file_open(DIR_IN,		O_WRONLY);
	fd[8] = dev_file_open(DIR_OUT,		O_RDONLY);
	fd[9] = dev_file_open(DIR_OUT_HEX,	O_RDONLY);
}

void close_descriptors(int *fd)
{
    for(uint8_t i = 0; i < 10; i += 1)
    {
        close(fd[i]);
    }
}

//	read a register into the passed buffer and reset fd to the beginning of file
void read_reg(int fd, char *buf)
{
    ssize_t read_b;
    read_b = read(fd, buf, sizeof(read_b));
    if(read_b == -1)
    {
        perror("error reading register");
        exit(EXIT_FAILURE);
    }
    lseek(fd, 0, SEEK_SET);
}

//	same as read_reg, with custom read byte count
void read_reg_to_length(int fd, char *buf, int length)
{
    ssize_t read_b;
    read_b = read(fd, buf, length);
    if(read_b == -1)
    {
        perror("error reading register");
        exit(EXIT_FAILURE);
    }
	lseek(fd, 0, SEEK_SET);
}

// 	write the passed buffer into a register and reset fd to the beginning of file
void write_reg(int fd, char *buf, int bytes)
{
    ssize_t write_b;
    write_b = write(fd, buf, bytes);
    if(write_b == -1)
    {
        perror("error writing register");
        exit(EXIT_FAILURE);
    }
	lseek(fd, 0, SEEK_SET);
}

//	custom calls of read_reg and write_reg to the specific crypto core registers
void get_core_id(int *fd, char* id_buf)
{
    read_reg(fd[0], id_buf);
}

void write_key(int *fd, char* key_buf)
{
    write_reg(fd[5], key_buf, 32);
}

void write_iv(int *fd, char* iv_buf)
{
    write_reg(fd[6], iv_buf, 16);
}

void write_input(int *fd, char* in_buf)
{
    write_reg(fd[7], in_buf, 16);
}

void set_mode(int *fd, int enc_dec)
{
    if(enc_dec == 1)
    {
        write_reg(fd[1], "1\0", 2);
    } else if (enc_dec == 0)
    {
        write_reg(fd[1], "0\0", 2);
    } else 
    {
        exit(EXIT_FAILURE);
    }
    
}

void set_format(int *fd, int aes_format)
{
    if(aes_format == 0)
    {
        write_reg(fd[2], "0\0", 2);
    } else if(aes_format == 1)
    {
        write_reg(fd[2], "1\0", 2);
    } else
    {
        write_reg(fd[2], "2\0", 2);
    }
}

//	alias for the operation of writing 1 to the 'start' register
void start_operation(int *fd)
{
    write_reg(fd[3], "1\0", 3);
}

void read_output(int *fd, char* out_buf, int length)
{
    read_reg_to_length(fd[8], out_buf, length);
}

void read_output_hex(int *fd, char* out_buf, int length)
{
	read_reg_to_length(fd[9], out_buf, length);
}

//	alias for the operation of reading the content of 'valid' register
int is_output_valid(int *fd)
{
    char valid_reg[4];
    int valid_int;
    read_reg(fd[4], valid_reg);
    valid_int = atoi(valid_reg);
    return valid_int;
}

//	clear the 'valid' register. This must be performed after each reading for continuous operation
void clear_valid(int *fd)
{
    write_reg(fd[4], "0\0", 2);
}
