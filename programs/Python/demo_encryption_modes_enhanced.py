
import subprocess
import os
import secrets

def generate_random_bytes(length):
	'''
	Generates a secure random sequence of bytes.
	'''
	return secrets.token_hex(length)

def call_c_program(program_name, *args):
	'''
	Encrypts the given plaintext using AES-256 in the specified mode.
	'''

	subprocess.run(["./" + program_name, *args])

def demonstrate_encryption_modes():
	'''
	Demonstrates the vulnerability of ECB mode and compares it with another mode (CBC).
	'''
	key = generate_random_bytes(16) # 256-bit key
	plaintext = '1234567890ABCDEF' # 16-character plaintext

	for _ in range(3):
	
        	iv = generate_random_bytes(8) # 128-bit IV
		print(f"Using Key: {key}")
		print(f"Using IV: {iv}")
		print(f"Plaintext: {plaintext}")

		# Encrypt with ECB mode
		encrypted_ecb = call_c_program("perform_all_operations.o", key, iv, plaintext, 0, 0)

		if encrypted_ecb:
			print(f"ECB Mode Encrypted data: {encrypted_ecb}")

		# Encrypt with CBC mode
		encrypted_cbc = call_c_program("perform_all_operations.o", key, iv, plaintext, 0, 1)
		if encrypted_cbc:
			print(f"CBC Mode Encrypted data: {encrypted_cbc}")


if __name__ == '__main__':
    demonstrate_encryption_modes()
