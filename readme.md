# Python PC scripts for serial communication with a microcontroller
Author: Bruce Belson  
Date: 4-Apr-2025  
Version: 1.0

These are very basic scripts for controlling a microcontroller via text commands on the serial port.

Both scripts assume that a Pi Pico running the **listen1** app is available on a COM port. This app accepts commands like the following:

```
red 255
green 10
blue 0
on
off
```

Neither of these scripts implements sufficient error and state checking.

## Setup

To run the Python scripts, you will need to install `pyserial`. You may need to install `tk` if it is not available on your machine.

## Connections

These scripts will fail to connect if you already have a connection open to the COM port connected to the Pi Pico, whether through Putty, the Serial Monitor, the debugger, or another Python instance.

You can always force a disconnect by removing and replacing the microUSB cable at the Pi Pico end.

## console.py

You should change this script to use the appropriate COM port number.

## gui.py

This script finds the first available COM port and connects to it at 115200 baud.

- Send three instructions at a time (red *n*, green *n*, blue *n*) using the **Send** button.
- Send the contents of a file using the **Send file** button.

