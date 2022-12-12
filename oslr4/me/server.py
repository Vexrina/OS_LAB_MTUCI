import socket
from generateMtx import GenerateMtx, MtxToStr
import time

s = socket.socket()
port = 1236
s.bind(('', port))
s.listen(5)
c, addr = s.accept()
print("Socket Up and running with a connection from",addr)
while True:
    rcvdData = c.recv(1024).decode()
    print("S:",rcvdData)
    mtx = GenerateMtx(2)
    sendData = MtxToStr(mtx)
    sendData = sendData + " "
    sendData = sendData + str(len(mtx))
    c.send(sendData.encode())
    time.sleep(5)
    if(sendData == "Bye" or sendData == "bye"):
        break
c.close()
