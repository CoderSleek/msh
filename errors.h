#ifndef MSHERRORS_H
#define MSHERRORS_H

// #define INPUT_INVALID_ERROR "Empty Input\n"

#define FORKING_FAILED_ERROR "Forking Unsuccessful\n"
#define FORKING_FAILED_CODE 1

#define CMD_EXEC_FAILED_ERROR "Command execution failed\n"
#define CMD_EXEC_FAILED_CODE 2

#define BUFFER_ALLOC_ERROR "Buffer allocation failed\n"
#define BUFFER_ALLOC_CODE 3

#define INPUT_TOO_LONG_ERROR "User input cannot be longer than 500 charachters\n"
#define INPUT_TOO_LONG_CODE 4

#define ARGV_ALLOC_FAILED_ERROR "Could not allocate memory for args array\n"
#define ARGV_ALLOC_FAILED_CODE 5

#define INPUT_FAILED_ERROR "User Input failed\n"
#define INPUT_FAILED_CODE 6

#define MEMORY_ERROR "Could not allocate memory\n"
#define MEMORY_CODE 6

#define COMMAND_NOT_FOUND_ERROR "Command not found\n"

#endif