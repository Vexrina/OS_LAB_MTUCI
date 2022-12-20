import socket
from gaussWithoutArg import InverseOfMtx, ParseArguments
from generateMtx import MtxToStr
import time

s = socket.socket()
s.connect(('127.0.0.1',1236))
str = input("S: ")
s.send(str.encode())
while True:
    recive = s.recv(1024).decode()
    order = int(recive[-1])
    recive = recive[:-2]
    print(recive)
    print(order)
    mtx = InverseOfMtx(ParseArguments(recive), order)
    print(mtx)
    s.send(MtxToStr(mtx).encode())
    time.sleep(5)
s.close()

