import subprocess

def call_c_program(program_name, *args):
    subprocess.run(["./" + program_name, *args])

def get_input(prompt, length):
    while True:
        value = input(prompt)
        if len(value) == length:
            return value
        print(f"Invalid input. Please enter a string of length {length}.")

def main():
    while True:
        print("Select an operation:")
        print("1) Update the key register")
        print("2) Update the IV register")
        print("3) Insert a new plaintext")
        print("4) Change mode (encrypt or decrypt)")
        print("5) Change format (ECB, etc.)")
        print("6) Perform all operations together")
        choice = input("Enter your choice (or 'exit' to quit): ")

        if choice == 'exit':
            break
        elif choice == '1':
            key = get_input("Enter the new key (32 characters): ", 32)
            call_c_program("update_key", key)
        elif choice == '2':
            iv = get_input("Enter the new IV (16 characters): ", 16)
            call_c_program("update_iv", iv)
        elif choice == '3':
            plaintext = get_input("Enter the new plaintext (16 characters): ", 16)
            call_c_program("insert_plaintext", plaintext)
        elif choice == '4':
            mode = get_input("Enter the new mode (1 character): ", 1)
            call_c_program("change_mode", mode)
        elif choice == '5':
            format = get_input("Enter the new format (1 character): ", 1)
            call_c_program("change_format", format)
        elif choice == '6':
            key = get_input("Enter the new key (32 characters): ", 32)
            iv = get_input("Enter the new IV (16 characters): ", 16)
            plaintext = get_input("Enter the new plaintext (16 characters): ", 16)
            mode = get_input("Enter the new mode (1 character): ", 1)
            format = get_input("Enter the new format (1 character): ", 1)
            call_c_program("perform_all_operations.o", key, iv, plaintext, mode, format)

if __name__ == "__main__":
    main()
