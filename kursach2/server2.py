# TODO Размер файла подкачки в байтах
# TODO Количество свободных байтов файла подкачки

import socket, subprocess, time
from _thread import *

def fromToByte(number, metric):
    match metric:
        case 'K':
            number=number * 1024
        case 'M':
            number=number * 1024 * 1024
        case 'G':
            number = number * 1024 * 1024 * 1024
        case 'B':
            return number
    return number

def take_swapon():
    cmd = ['swapon', '--show']
    output = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    output = output.decode()
    output = output.split()
    while(output[0] != 'partition'):
        output.pop(0)
    output.pop()
    output.pop(0)	

    return output[0], output[1]

s = socket.socket()
port = 5555

try:
    s.bind(('', port))
except socket.error as e:
    print(str(e))

s.listen(5)

def multi_threaded_client(c):
    while True:
        rcvdData = c.recv(1024).decode()
        print("Client: ", rcvdData)
        AlSwapon, FrSwapon = take_swapon()
        metricAll = AlSwapon[-1]
        AlSwapon = int(AlSwapon[:-1])
        metricFr = FrSwapon[-1]
        FrSwapon = int(FrSwapon[:-1])
        match rcvdData:
            case '1':
                AlSwapon = fromToByte(AlSwapon, metricAll)
                ans = f'\tSize of swapopn is {AlSwapon}'
                print(ans)
                c.send(ans.encode())
            case '2':
                print('here')
                FrSwapon = fromToByte(FrSwapon, metricFr)
                ans = f'\tFree of swapon is {FrSwapon}'
                print(ans)
                c.send(ans.encode())
            case '3':
                AlSwapon = fromToByte(AlSwapon, metricAll)
                FrSwapon = fromToByte(FrSwapon, metricFr)
                ans = f'\tSize of swapopn is {AlSwapon}\n\t\t\tFree of swapon is {FrSwapon}'
                print(ans)
                c.send(ans.encode())
            case _:
                ans = 'Smth went wrong, try again'
                c.send(ans.encode())
                break
        time.sleep(5)
    c.close()

while True:
    c, addr = s.accept()
    print("Socket up and running with a connection from", addr)
    start_new_thread(multi_threaded_client, (c))

s.close()