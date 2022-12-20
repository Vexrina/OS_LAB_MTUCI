import socket, sys
from tkinter import *

port1 = 4444
port2 = 5555

SS1 = socket.socket()
SS2 = socket.socket()

try:
    SS1.connect(('127.0.0.1', port1))
    SS2.connect(('127.0.0.1', port2))
except socket.error as e:
    print(str(e))

def server1():
    data_to_send = quest_entry.get()
    SS1.send(data_to_send.encode())
    recive = SS1.recv(1024).decode()
    info['text'] = f'Server1 {recive}'
    print(f'Server is said: ', recive)

def server2():
    data_to_send = quest_entry.get()
    SS2.send(data_to_send.encode())
    recive = SS2.recv(1024).decode()
    info['text'] = f'Server2 {recive}'
    print(f'Server is said: ', recive)

def servers():
    data_to_send = quest_entry.get()
    SS1.send(data_to_send.encode())
    SS2.send(data_to_send.encode())
    receive1 = SS1.recv(1024).decode()
    receive2 = SS2.recv(1024).decode()
    info['text'] = f'Server1 {receive1}\nServer2 {receive2}'
    print(f'Server1 {receive1}\nServer2 {receive2}')

def CloseAll():
    SS1.close()
    SS2.close()
    sys.exit('Client is close')
# choice  = int(input("What server do u wanna see? 1, 2 or both(3)?\t"))
# flag = True
# while flag:
#     match choice:
#         case 1:
#             SendData = input("What quest do you wanna see? 1, 2, or both(3)?\t")
#             server1(SendData)
#             time.sleep(5)
#             choice = int(input("What server do u wanna see? 1, 2 or both(3)?\t"))
#         case 2: 
#             SendData = input("What quest do you wanna see? 1, 2, or both(3)?\t")
#             server2(SendData)
#             time.sleep(5)
#             choice = int(input("What server do u wanna see? 1, 2 or both(3)?\t"))
#         case 3:
#             SendData = input("What quest do you wanna see? 1, 2, or both(3)?\t")
#             server1(SendData)
#             server2(SendData)
#             time.sleep(5)
#             choice = int(input("What server do u wanna see? 1, 2 or both(3)?\t"))
#         case _:
#             print("Bad request! Application will be closed!")
#             flag = False    

root = Tk()

root['bg'] = '#fafafa'
root.title('Client')
root.geometry('350x300')
root.resizable(width=False, height=False)

frame_top = Frame(root, bg='red')
frame_top.place(relheight=0.5, relwidth=1)

frame_bot = Frame(root, bg='yellow')
frame_bot.place(rely =  0.5, relheight=0.5, relwidth=1)

quest_entry = Entry(frame_top, bg='white', font=30)
quest_entry.pack()

btn_server1 = Button(frame_top, text='send to server1',command=server1)
btn_server1.pack()

btn_server2 = Button(frame_top, text='send to server2',command=server2)
btn_server2.pack()

btn_servers = Button(frame_top, text='send to servers',command=servers)
btn_servers.pack()

btn_close = Button(frame_top, text='close', command=CloseAll)
btn_close.pack()

info = Label(frame_bot, text='Information', bg='#ffb700', font=40)
info.pack()

root.mainloop()