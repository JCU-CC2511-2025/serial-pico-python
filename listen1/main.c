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

int led_pins[] = {RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN};  // Array of LED pins
#define LED_COUNT (sizeof(led_pins) / sizeof(led_pins[0]))    // Number of LEDs

/**
 * @brief Initialize the LEDs
 * @details This function sets the GPIO pins for the LEDs to 
 *          PWM mode and initializes them to off.
 */
void init_leds() {
  for (int i = 0; i < LED_COUNT; i++) { // Loop through each LED pin
    gpio_set_function(led_pins[i], GPIO_FUNC_PWM);
    pwm_set_enabled(pwm_gpio_to_slice_num(led_pins[i]), true);
    pwm_set_gpio_level(led_pins[i], 0); 
  }
}

/**
 * @brief Set the brightness of the LEDs
 * @param red The brightness of the red LED (-1 or 0-255)
 * @param green The brightness of the green LED (-1 or 0-255)
 * @param blue The brightness of the blue LED (-1 or 0-255)
 * @details The brightness values are clamped to a maximum of 255.
 *          If a value is negative, the corresponding LED is not changed.
 */
void set_leds(int red, int green, int blue) {
  if (red >= 0) {                               // Check if the red value is valid
    red = red > 255 ? 255 : red;                // Clamp the value to a maximum of 255
    pwm_set_gpio_level(RED_LED_PIN, red * red); // Set the PWM level for the red LED
  }
  if (green >= 0) {
    green = green > 255 ? 255 : green;
    pwm_set_gpio_level(GREEN_LED_PIN, green * green); 
  }
  if (blue >= 0) {
    blue = blue > 255 ? 255 : blue; 
    pwm_set_gpio_level(BLUE_LED_PIN, blue * blue); 
  }
}

/**
 * @brief Process the command received from the command buffer
 * @param buffer The command buffer containing the command
 * @param error_buffer The buffer to store error messages
 * @param error_buffer_size The size of the error buffer
 * @return true if the command was processed successfully, false otherwise
 */
bool process_command(const char* buffer, char* error_buffer, size_t error_buffer_size) {
  int value = 0;
  if (sscanf(buffer, "red %d", &value) == 1) {  // Check if the command is "red" followed by an integer
    if (value >= 0 && value <= 255) {           // Check if the value is within the valid range
      set_leds(value, -1, -1);                  // Set the PWM level for the red LED
      return true;                              // Command processed successfully
    } else {
      snprintf(error_buffer, error_buffer_size, "Value must be between 0 and 255. Received [%d]", value);
    }
  } else if (sscanf(buffer, "green %d", &value) == 1) { 
    if (value >= 0 && value <= 255) { 
      set_leds(-1, value, -1); 
      return true; 
    } else {
      snprintf(error_buffer, error_buffer_size, "Value must be between 0 and 255. Received [%d]", value);
    }
  } else if (sscanf(buffer, "blue %d", &value) == 1) { 
    if (value >= 0 && value <= 255) { 
      set_leds(-1, -1, value); 
      return true; 
    } else {
      snprintf(error_buffer, error_buffer_size, "Value must be between 0 and 255. Received [%d]", value);
    }
  } else if (strcmp(buffer, "off") == 0) {        // Check if the command is "off"
    set_leds(0, 0, 0); // Turn off all LEDs
    return true; 
  } else if (strcmp(buffer, "on") == 0) {         // Check if the command is "on"
    set_leds(255, 255, 255); // Turn on all LEDs at full brightness
    return true; 
  }
  else {
    // If the command is not recognized, set an error message 
    snprintf(error_buffer, error_buffer_size, "Invalid command [%s]", buffer); 
  }
  return false;                                   // Command not processed successfully
}

#define ECHO_ENABLED false  // Enable echoing of input characters
#define SLEEP_MS 500        // Sleep time in milliseconds

command_buffer_t command_buffer; // Declare a command buffer object

int main(void) {
  bool echo_enabled = ECHO_ENABLED; // Set the echo enabled flag
  char error_buffer[256];           // Buffer to store error messages

  // Initialise components and variables
  command_buffer_init(&command_buffer, echo_enabled); 
  stdio_init_all(); 
  init_leds(); 
  
  // Read & process commands in a loop
  while (true) {
    bool got_input = command_buffer_read_input(&command_buffer); // Read input from the command buffer
    if (got_input) {
      // Process the command in the command buffer
      if (process_command((const char*)command_buffer.buffer, error_buffer, sizeof(error_buffer)/sizeof(char))) {
        printf("ok\n"); 
      } else {
        printf("error: %s\n", error_buffer); // Print failure message
      }
    } 

    sleep_ms(SLEEP_MS); // Sleep for 100 milliseconds to avoid busy waiting
  }
}
