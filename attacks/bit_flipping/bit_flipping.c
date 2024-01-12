#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include "crypto_functions.h"

static void xor_bytes(const uint8_t* buf_1, const uint8_t* buf_2, uint8_t* buf_out, ssize_t len);
static void print_uint8_buf(const uint8_t *buf, ssize_t len);
static void print_uint8_buf_n(const uint8_t *buf, ssize_t len);
static void ssepstr_to_uint8(const uint8_t *ssep, uint8_t *num);

int main()
{
	uint8_t key[33] = "rzx9vTCisJ6nYjcHoltLSZz5mT5daXZd\0";	// arbitrary
	uint8_t iv[17] = "oAN/Bz6iqrKWZZ0m\0";			// arbitrary
	uint8_t p1[17] = "xxxxxxxxxxxxxxxx\0";			// text to encrypt
	uint8_t p2[17] = "&set_admin=true;\0";			// test to write to output
	uint8_t x[17];						// XOR of P1 and P2
	uint8_t c1[17];
	uint8_t c2[17];
	uint8_t p3[17];
	char c1_char[100];
	char p3_char[100];
	int fd[10];
	int valid_flag = 0;

	printf("P1: ");
	print_uint8_buf(p1, 16);
	printf("P2: ");
	print_uint8_buf(p2, 16);
	printf("P1 (uint8): ");
	print_uint8_buf_n(p1, 16);
	printf("P2 (uint8): ");
	print_uint8_buf_n(p2, 16);
	xor_bytes(p1, p2, x, 16);		// X = P1 XOR P2
	printf("X (uint8): ");
	print_uint8_buf_n(x, 16);
	open_descriptors(fd);
	write_key(fd, key);
	write_iv(fd, iv);
	write_input(fd, p1);
	set_mode(fd, 0);
	set_format(fd, 2);
	start_operation(fd);
	while(valid_flag == 0)
	{
		valid_flag = is_output_valid(fd);
	}
	read_output(fd, c1_char, 100);
	ssepstr_to_uint8(c1_char, c1);		// C1 = P1 XOR E
	clear_valid(fd);
	printf("C1 (uint8): ");
	print_uint8_buf_n(c1, 16);
	xor_bytes(c1, x, c2, 16);		// C2 = C1 XOR X
	printf("C2 (uint8): ");
	print_uint8_buf_n(c2, 16);
	write_iv(fd, iv);			// counter reset for decrypt
	write_input(fd, c2);
	set_mode(fd, 1);
	start_operation(fd);
	while(valid_flag == 0)
	{
		valid_flag = is_output_valid(fd);
	}
	read_output(fd, p3_char, 100);
	ssepstr_to_uint8(p3_char, p3);		// P3 = C2 XOR E = P2
	clear_valid(fd);
	printf("P3 (uint8): ");
	print_uint8_buf_n(p3, 16);
	printf("P3: ");
	print_uint8_buf(p3, 16);
	return 0;
}

static void xor_bytes(const uint8_t* buf_1, const uint8_t* buf_2, uint8_t* buf_out, ssize_t len)
{
	for(ssize_t i = 0; i < len; i += 1)
	{
		buf_out[i] = buf_1[i] ^ buf_2[i];
	}
}
static void print_uint8_buf(const uint8_t *buf, ssize_t len)
{
	for(ssize_t i = 0; i < len; i += 1)
	{
		printf("%c", buf[i]);
	}
	printf("\n");
}
static void ssepstr_to_uint8(const uint8_t *ssep, uint8_t *num)
{
	uint8_t n = 0;
	ssize_t len_out = 0;
	for(ssize_t i = 0; ssep[i] != '\0' && len_out < 16; i += 1)
	{
		if(ssep[i] >= '0' && ssep[i] <= '9')
		{
			n = n*10 + (ssep[i] - '0');
		} else
		{
			num[len_out++] = n;
			n = 0;
		}
	}
	num[len_out] = n;
}
static void print_uint8_buf_n(const uint8_t *buf, ssize_t len)
{
	for(ssize_t i = 0; i < len; i += 1)
	{
		printf("%u ", buf[i]);
	}
	printf("\n");
}
