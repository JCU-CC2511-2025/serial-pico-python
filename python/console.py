import serial

ser = serial.Serial()
ser.port = 'COM18'
ser.baudrate = 115200
# ser.timeout = 1
ser.timeout = None
ser.open()
ser.flushInput()

while True:
  for i in range(0, 257, 10):
      cmd = "red " + str(i) + "\n"
      # print("sending " + cmd)
      ser.write(cmd.encode())
      # print("Sent data")
      data = ser.readline()
      print("Received data:", data.decode().strip())

