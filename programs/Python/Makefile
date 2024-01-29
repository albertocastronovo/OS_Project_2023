CC=/home/matte/buildroot/output/host/bin/riscv64-buildroot-linux-gnu-gcc

all: update_key update_iv insert_plaintext change_mode change_format perform_all_operations test_program

update_key: 
	$(CC) update_key.c crypto_functions.c -o update_key.o

update_iv: 
	$(CC) update_iv.c crypto_functions.c -o update_iv.o

insert_plaintext: 
	$(CC) insert_plaintext.c crypto_functions.c -o insert_plaintext.o

change_mode: 
	$(CC) change_mode.c crypto_functions.c -o change_mode.o

change_format: 
	$(CC) change_format.c crypto_functions.c -o change_format.o

perform_all_operations: 
	$(CC) perform_all_operations.c crypto_functions.c -o perform_all_operations.o

test_program: 
	$(CC) test_program.c crypto_functions.c -o test_program.o

clean:
	rm -f update_key update_iv insert_plaintext change_mode change_format perform_all_operations test_program
