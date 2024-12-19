import socket
import os

def upload_file(file_path, server_ip, server_port):
    file_size = os.path.getsize(file_path)
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
        client_socket.connect((server_ip, server_port))
        
        # Send file size first
        client_socket.send(str(file_size).encode())
        response = client_socket.recv(1024)
        
        # Send file data
        with open(file_path, "rb") as file:
            while (chunk := file.read(1024)):
                client_socket.send(chunk)
        print("File uploaded successfully!")

if __name__ == "__main__":
    upload_file("../../test_files/hello", "192.168.217.132", 12345)