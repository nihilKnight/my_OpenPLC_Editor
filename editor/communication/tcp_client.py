import socket
import tkinter as tk
from tkinter import filedialog, messagebox
import os
import struct

class FileTransferApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Scripts Upload and Download")
        self.root.geometry("400x200")
        
        # self.server_host = "127.0.0.1"
        self.server_host = "192.168.217.132"
        self.server_port = 5000

        self.upload_button = tk.Button(self.root, text="Download Structured Text to PLC", command=self.upload_file)
        self.upload_button.pack(pady=20)

        self.download_button = tk.Button(self.root, text="Upload Structured Text from PLC", command=self.download_file)
        self.download_button.pack(pady=20)

    def upload_file(self):
        file_path = filedialog.askopenfilename()
        if not file_path:
            return
        
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
                client_socket.connect((self.server_host, self.server_port))

                command = '__U__'
                client_socket.send(struct.pack('I', len(command)))
                client_socket.send(command.encode())

                file_name = os.path.basename(file_path)
                file_size = os.path.getsize(file_path)
                
                client_socket.send(struct.pack('I', len(file_name)))
                client_socket.send(file_name.encode())

                client_socket.send(struct.pack('Q', file_size))

                with open(file_path, 'rb') as f:
                    while (data := f.read(1024)):
                        client_socket.send(data)

            messagebox.showinfo("Success", "Script successfully downloaded to PLC!")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to download to PLC: {e}")

    def download_file(self):
        file_name = filedialog.askstring("Upload ST from PLC", "Please type in the file name of the ST to upload:")
        if not file_name:
            return
        
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
                client_socket.connect((self.server_host, self.server_port))

                client_socket.send(b'__D__')

                client_socket.send(file_name.encode())

                file_name_from_server = client_socket.recv(1024).decode()
                if file_name_from_server == 'File not found':
                    messagebox.showerror("Error", "Script not found on the server")
                    return
                
                file_size = int(client_socket.recv(1024).decode())
                print(f"Prepare to receive: {file_name_from_server}, size: {file_size} bytes")

                with open(f"uploaded_{file_name_from_server}", 'wb') as f:
                    remaining = file_size
                    while remaining:
                        data = client_socket.recv(min(1024, remaining))
                        f.write(data)
                        remaining -= len(data)

            messagebox.showinfo("Success", f"Finished uploading from PLC: {file_name_from_server}")
        except Exception as e:
            messagebox.showerror("Error", f"Failed to upload from PLC: {e}")


if __name__ == "__main__":
    root = tk.Tk()
    app = FileTransferApp(root)
    root.mainloop()
