/**************************************************************
 * main.c
 * rev 1.0 03-Apr-2025 Bruce
 * listen1
 * ***********************************************************/

#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "hardware/pwm.h"

#include "command_buffer.h"

#define RED_LED_PIN 11
#define GREEN_LED_PIN 12
#define BLUE_LED_PIN 13

void init_leds() {
  // gpio_init(RED_LED_PIN); // Initialize the GPIO pin for the red LED
  // gpio_set_dir(RED_LED_PIN, GPIO_OUT); // Set the GPIO pin as output
  // gpio_init(GREEN_LED_PIN); // Initialize the GPIO pin for the green LED
  // gpio_set_dir(GREEN_LED_PIN, GPIO_OUT); // Set the GPIO pin as output
  // gpio_init(BLUE_LED_PIN); // Initialize the GPIO pin for the blue LED
  // gpio_set_dir(BLUE_LED_PIN, GPIO_OUT); // Set the GPIO pin as output

  gpio_set_function(RED_LED_PIN, GPIO_FUNC_PWM); // Set the GPIO pin function to PWM for the red LED
  gpio_set_function(GREEN_LED_PIN, GPIO_FUNC_PWM); // Set the GPIO pin function to PWM for the green LED
  gpio_set_function(BLUE_LED_PIN, GPIO_FUNC_PWM); // Set the GPIO pin function to PWM for the blue LED

  pwm_set_enabled(pwm_gpio_to_slice_num(RED_LED_PIN), true); // Enable PWM for the red LED
  pwm_set_enabled(pwm_gpio_to_slice_num(GREEN_LED_PIN), true); // Enable PWM for the green LED
  pwm_set_enabled(pwm_gpio_to_slice_num(BLUE_LED_PIN), true); // Enable PWM for the blue LED

  pwm_set_gpio_level(RED_LED_PIN, 0); // Set initial PWM level for red LED to 0 (off)
  pwm_set_gpio_level(GREEN_LED_PIN, 0); // Set initial PWM level for green LED to 0 (off)
  pwm_set_gpio_level(BLUE_LED_PIN, 0); // Set initial PWM level for blue LED to 0 (off)
}

void set_leds(int red, int green, int blue) {
  if (red >= 0) { // Check if the red value is valid
    red = red > 255 ? 255 : red; // Clamp the value to a maximum of 255
    pwm_set_gpio_level(RED_LED_PIN, red * red); // Set the PWM level for the red LED
  }
  if (green >= 0) {
    green = green > 255 ? 255 : green; // Clamp the value to a maximum of 255
    pwm_set_gpio_level(GREEN_LED_PIN, green * green); // Set the PWM level for the green LED
  }
  if (blue >= 0) {
    blue = blue > 255 ? 255 : blue; // Clamp the value to a maximum of 255
    pwm_set_gpio_level(BLUE_LED_PIN, blue * blue); // Set the PWM level for the blue LED
  }
}

command_buffer_t command_buffer; // Declare a command buffer object

bool process_command(const char* buffer, char* error_buffer, size_t error_buffer_size) {
  int value = 0;
  if (sscanf(buffer, "red %d", &value) == 1) { // Check if the command is "red" followed by an integer
    if (value >= 0 && value <= 255) { // Check if the value is within the valid range
      set_leds(value, -1, -1); // Set the PWM level for the blue LED
      return true; // Command processed successfully
    } else {
      snprintf(error_buffer, error_buffer_size, "Value must be between 0 and 255. Received [%d]", value);
    }
  } else if (sscanf(buffer, "green %d", &value) == 1) { // Check if the command is "green" followed by an integer
    if (value >= 0 && value <= 255) { // Check if the value is within the valid range
      set_leds(-1, value, -1); // Set the PWM level for the blue LED
      return true; // Command processed successfully
    } else {
      snprintf(error_buffer, error_buffer_size, "Value must be between 0 and 255. Received [%d]", value);
    }
  } else if (sscanf(buffer, "blue %d", &value) == 1) { // Check if the command is "blue" followed by an integer
    if (value >= 0 && value <= 255) { // Check if the value is within the valid range
      set_leds(-1, -1, value); // Set the PWM level for the blue LED
      return true; // Command processed successfully
    } else {
      snprintf(error_buffer, error_buffer_size, "Value must be between 0 and 255. Received [%d]", value);
    }
  } else if (strcmp(buffer, "off") == 0) { // Check if the command is "off"
    set_leds(0, 0, 0); // Turn off all LEDs
    return true; // Command processed successfully
  } else if (strcmp(buffer, "on") == 0) { // Check if the command is "on"
    set_leds(255, 255, 255); // Turn on all LEDs at full brightness
    return true; // Command processed successfully
  }
  else {
    snprintf(error_buffer, error_buffer_size, "Invalid command [%s]", buffer); // Print error message for invalid value
  }
  return false; // Command not processed successfully
}

#define ECHO_ENABLED false // Enable echoing of input characters
#define SLEEP_MS 1000 // Sleep time in milliseconds

int main(void) {
  bool echo_enabled = ECHO_ENABLED; // Set the echo enabled flag
  char error_buffer[256]; // Buffer to store error messages

  // Initialise components and variables
  command_buffer_init(&command_buffer, echo_enabled); // Initialize the command buffer with echo enabled
  stdio_init_all(); // Initialize all standard I/O (UART, USB, etc.)
  init_leds(); // Initialize the LEDs
  
  while (true) {
    bool got_input = command_buffer_read_input(&command_buffer); // Read input from the command buffer
    if (got_input) {
      // Process the command in the command buffer
      if (process_command((const char*)command_buffer.buffer, error_buffer, sizeof(error_buffer)/sizeof(char))) {
        if (echo_enabled) {
          printf("Command processed successfully.\n"); // Print success message
        }
        else {
          printf("ok\n"); // Print "ok" if echo is disabled
        }
      } else {
        if (echo_enabled) {
          printf("Error: %s\n", error_buffer); // Print failure message
        }
        else {
          printf("error: %s\n", error_buffer); // Print failure message
        }
      }
    } 

    sleep_ms(SLEEP_MS); // Sleep for 100 milliseconds to avoid busy waiting
  }
}
