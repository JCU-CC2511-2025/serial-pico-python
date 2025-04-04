#ifndef __COMMAND_BUFFER_H__
#define __COMMAND_BUFFER_H__

#ifndef COMMAND_BUFFER_SIZE
#define COMMAND_BUFFER_SIZE 64 // Default size of the command buffer
#endif

#include <stddef.h> // For size_t
#include <stdbool.h> // For bool type

typedef struct command_buffer_t {
    volatile char buffer[COMMAND_BUFFER_SIZE]; // Pointer to the command buffer
    size_t size;  // Size of the command buffer
    size_t pos;   // Current position in the buffer
    bool echo;    // Flag to enable/disable echoing of input characters
} command_buffer_t;

void command_buffer_init(command_buffer_t *cb, bool echo);
bool command_buffer_read_input(command_buffer_t *cb);

#endif // __COMMAND_BUFFER_H__
