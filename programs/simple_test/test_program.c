#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "crypto_functions.h"

int main(int argc, char **argv)
{
	// Input arguments check
	if(argc != 5)
	{
		printf("Program error: invalid number of arguments.\n");
		exit(EXIT_FAILURE);
	}
	if(strlen(argv[1]) != 32)
	{
		printf("Program error: invalid key length (should be 32 bytes).\n");
		exit(EXIT_FAILURE);
	}
	if(strlen(argv[2]) != 16)
	{
		printf("Program error: invalid IV length (should be 16 bytes).\n");
		exit(EXIT_FAILURE);
	}
	if( (atoi(argv[3]) == 0) && (!((strlen(argv[3]) == 1) && (argv[3][0] == '0'))) )
	{
		printf("Program error: invalid mode input (should be a single digit integer).\n");
		exit(EXIT_FAILURE);
	}
	if( (atoi(argv[4]) == 0) && (!((strlen(argv[4]) == 1) && (argv[4][0] == '0'))) )
	{
		printf("Program error: invalid format input (should be a single digit integer).\n");
		exit(EXIT_FAILURE);
	}

	FILE *f_in = fopen("input.txt", "r");
	if(f_in == NULL)
	{
		printf("Program error: input.txt could not be found or opened.\n");
		exit(EXIT_FAILURE);
	}

	// we need 10 file descriptors for the crypto_functions.h functions
	int fd[10];
	// variables for storing crypto core ID, key, IV, file input, file output
	char proc_id[5];
	char key[33];
	char iv[17];
	char in_str[100];
	char out_str[100];
	// variables used for intermediate operations
	char in_sub[16];
	char in_str_p[256];
	int start_int = 1;
	int mode_int = 0;
	int format_int = 0;
	uint8_t i = 0;
	struct timespec t_start, t_stop;
	double exec_time;
	int valid_flag = 0;
	// command line arguments: key, IV, mode, format
	printf("Argument scanning.\n");

	sscanf(argv[1], "%s", key);
	sscanf(argv[2], "%s", iv);
	mode_int = atoi(argv[3]);	// 0 for encrypt, 1 for decrypt
	format_int = atoi(argv[4]);	// 0 for ECB, 1 for CBC, other for CTR

	printf("Setting registers.\n");
	// key, iv, mode and format registers set up with crypto_functions.h functions
	open_descriptors(fd);
	get_core_id(fd, proc_id);
	write_key(fd, key);
	write_iv(fd, iv);
	set_mode(fd, mode_int);
	set_format(fd, format_int);

	// read input from file

	fgets(in_str, 100, f_in);
	fclose(f_in);
	printf("Input text: %s\n", in_str);

	/*
		As we require the input string to be a multiple of 16 bytes long,
		a padding operation (adding 0's to the end) is performed.
	*/

	if(strlen(in_str) % 16 != 0)
	{
		sprintf((char*)in_str_p, "%s%0*d", in_str, 16*(strlen(in_str)/16+1) - strlen(in_str), 0); 
	} else
	{
		strcpy(in_str_p, in_str);
	}

	printf("Padded text: %s\nWith length: %d\n", in_str_p, strlen(in_str_p));

	// we iterate over every 16-byte block of the padded string and perform the operation

	while(i < strlen(in_str_p))
	{
		// read 16-byte substring
		sprintf((char*)in_sub,
			"%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",
			in_str_p[i], in_str_p[i+1], in_str_p[i+2], in_str_p[i+3],
			in_str_p[i+4], in_str_p[i+5], in_str_p[i+6], in_str_p[i+7],
			in_str_p[i+8], in_str_p[i+9], in_str_p[i+10], in_str_p[i+11],
			in_str_p[i+12], in_str_p[i+13], in_str_p[i+14], in_str_p[i+15]);
		// set input register
		write_input(fd, in_sub);

		// start operation
		start_operation(fd);

		// wait for completion

		while(valid_flag == 0)
		{
			valid_flag = is_output_valid(fd);
		}
		// read the output register
		read_output_hex(fd, out_str, 100);
		// set the last character as \0 to be able to use string functions properly
		out_str[33] = '\0';
		// clear the 'valid' bit that was set by the processor at the end of the operation
		clear_valid(fd);
		// display the results
		printf("%s\n", out_str);
		valid_flag = 0;
		// increment i to select the following 16-bytes block
		i += 16;
	}
	close_descriptors(fd);

	return 0;
}

