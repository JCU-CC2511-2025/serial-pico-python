#include "pico/stdlib.h"
#include "command_buffer.h"
#include <stdio.h>
#include <string.h> // For strlen

void command_buffer_init(command_buffer_t *cb, bool echo)
{
  cb->size = COMMAND_BUFFER_SIZE;            // Set the size of the command buffer
  cb->pos = 0;                               // Initialize the position in the buffer to 0
  cb->echo = echo;                           // Set the echo flag based on the input parameter
  memset(cb->buffer, 0, sizeof(cb->buffer)); // Clear the command buffer
}

bool command_buffer_read_input(command_buffer_t *cb)
{
  do
  {
    int ch = getchar_timeout_us(0); // Read a character with a timeout of 0 microseconds
    if (ch == PICO_ERROR_TIMEOUT)
    {
      return false; // No character available, return false
    }
    else if (ch == '\n' || ch == '\r')
    {
      cb->buffer[cb->pos] = '\0'; // Null-terminate the string
      cb->pos = 0;                // Reset the position in the buffer
      return true;                // Return true when a newline or carriage return is received
    }
    else if (ch == 0x7F || ch == 0x08)
    { // Handle backspace (0x7F or 0x08)
      if (cb->pos > 0)
      {
        cb->pos--; // Move back in the buffer
        if (cb->echo)
        {
          printf("\b \b"); // Erase the last character on the console
        }
      }
    }
    else if (cb->pos < cb->size - 1)
    {                             // Check if there is space in the buffer
      cb->buffer[cb->pos++] = ch; // Store the character in the buffer
      if (cb->echo)
      {
        putchar(ch); // Echo the character to the console
      }
    }
  } while (true); // Loop until a newline or carriage return is received
}
