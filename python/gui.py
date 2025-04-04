import tkinter as tk
from tkinter import messagebox, filedialog
import serial
import serial.tools.list_ports

def find_serial_port():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print(f"Found port: {p.device}")
    return ports[0].device if ports else None

class SerialSenderApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Serial RGB Sender")
        self.root.configure(padx=20, pady=20)  # Add padding to the window

        # Entry widgets for the three integers
        self.entries = []
        for i, label in enumerate(["Red", "Green", "Blue"]):
            tk.Label(root, text=label).grid(row=i, column=0, padx=10, pady=5, sticky="e")
            entry = tk.Entry(root)
            entry.insert(0, "0")  # Initialize with 0
            entry.grid(row=i, column=1)
            self.entries.append(entry)

        # Send button
        self.send_button = tk.Button(root, text="Send", command=self.send_data)
        self.send_button.grid(row=3, column=0, columnspan=2, pady=10)

        # File send button
        self.file_button = tk.Button(root, text="Send File", command=self.send_file)
        self.file_button.grid(row=4, column=0, columnspan=2, pady=5)
        
        # Try to find and open a serial port
        self.serial_port = find_serial_port()
        if self.serial_port:
            try:
                self.ser = serial.Serial(self.serial_port, 115200, timeout=None)
                print(f"Connected to {self.serial_port}")
            except serial.SerialException as e:
                messagebox.showerror("Serial Error", str(e))
                self.ser = None
        else:
            messagebox.showwarning("No Port Found", "No serial port found.")
            self.ser = None

    def send_data(self):
        try:
            values = [int(entry.get()) for entry in self.entries]
            if any(v < 0 or v > 255 for v in values):
                raise ValueError("All values must be between 0 and 255")

            if self.ser and self.ser.is_open:
                for i in [0, 1, 2]:
                  cmd = f'{["red", "green", "blue"][i]} {values[i]}\n'
                  print("Sending command:", cmd.strip())
                  # data = bytes(values)
                  self.ser.write(cmd.encode())
                  print(f"Sent: {cmd}")
                  response = self.ser.readline()
                  print("Received data:", response.decode().strip())
            else:
                messagebox.showerror("Serial Not Open", "Serial port is not open.")
        except ValueError as ve:
            messagebox.showerror("Input Error", str(ve))

    def send_file(self):
        file_path = filedialog.askopenfilename(title="Select File to Send")
        if not file_path:
            return

        if self.ser and self.ser.is_open:
            try:
                with open(file_path, 'r') as file:
                    for line in file:
                        line = line.strip()
                        if line:
                            self.ser.write(line.encode('utf-8') + b'\n')
                            print(f"Sent line: {line}")
                            response = self.ser.readline()
                            print("Received data:", response.decode().strip())
                messagebox.showinfo("File Sent", "File contents sent successfully.")
            except Exception as e:
                messagebox.showerror("File Error", str(e))
        else:
            messagebox.showerror("Serial Not Open", "Serial port is not open.")

if __name__ == '__main__':
    root = tk.Tk()
    app = SerialSenderApp(root)
    root.mainloop()
