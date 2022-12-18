import socket
import time

port1 = 4444
port2 = 5555

SS1 = socket.socket()
SS2 = socket.socket()

try:
    SS1.connect(('127.0.0.1',port1))
    SS2.connect(('127.0.0.1',port2))
except socket.error as e:
    print(str(e))

def server1(data_to_send):
   SS1.send(data_to_send.encode())
   recive = SS1.recv(1024).decode()
   print(f'Server is said: ', recive)

def server2(data_to_send):
    SS2.send(data_to_send.encode())
    recive = SS2.recv(1024).decode()
    print(f'Server is said: ', recive)


choice  = int(input("What server do u wanna see? 1, 2 or both(3)?\t"))
flag = True
while flag:
    match choice:
        case 1:
            SendData = input("What quest do you wanna see? 1, 2, or both(3)?\t")
            server1(SendData)
            time.sleep(5)
            choice = int(input("What server do u wanna see? 1, 2 or both(3)?\t"))
        case 2: 
            SendData = input("What quest do you wanna see? 1, 2, or both(3)?\t")
            server2(SendData)
            time.sleep(5)
            choice = int(input("What server do u wanna see? 1, 2 or both(3)?\t"))
        case 3:
            SendData = input("What quest do you wanna see? 1, 2, or both(3)?\t")
            server1(SendData)
            server2(SendData)
            time.sleep(5)
            choice = int(input("What server do u wanna see? 1, 2 or both(3)?\t"))
        case _:
            print("Bad request! Application will be closed!")
            flag = False
    
    

SS1.close()
SS2.close()
