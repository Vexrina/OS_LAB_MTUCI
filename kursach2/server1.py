#TODO Ширину и высоту рамки окна.
#TODO изменяет название окна сервера на переданное 
# из клиенсткого приложения и возвращает результат изменения
# PS1=$PS1"\[\e]0;test1\a\]" - НЕ РАБОТАЕТ ИЗ ПАЙТОНА

import socket, shutil, subprocess, time
from _thread import *

def take_cursor():
    cmd = ['xdotool', 'getmouselocation']
    output = subprocess.Popen(cmd, stdout=subprocess.PIPE).communicate()[0]
    output = output.decode()
    output = output.split()
    output.pop()
    output.pop()
    return output[0], output[1]

s = socket.socket()
port = 4444

try:
    s.bind(('', port))
except socket.error as e:
    print(str(e))
    
s.listen(5)

def multi_thread_client(c):
    while True:
        rcvdData = c.recv(1024).decode()
        print("Client: ", rcvdData)
        match rcvdData:
            case '1':
                col, rows = shutil.get_terminal_size()
                ans = (f'\tWidth is {col}, height is {rows}')
                c.send(ans.encode())
            case '2':
                xMouse, yMouse = take_cursor()
                ans = f'\tCoourdinates of mouse is {xMouse} and {yMouse}'
                c.send(ans.encode())
            case '3':
                col, rows = shutil.get_terminal_size()
                xMouse, yMouse = take_cursor()
                ans = f'\tWidth is {col}, height is {rows}\n\t\t\tCoourdinates of mouse is {xMouse} and {yMouse}'
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
    start_new_thread(multi_thread_client, (c))
s.close()

    