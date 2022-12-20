from tkinter import *

def server1():
    data = quest_entry.get()
    print(f'send {data} to server1')
    info['text'] = f'Server1 {data}'

def servers():
    data = quest_entry.get()
    print(f'send {data} to servers')
    info['text'] = f'Servers {data}'

def server2():
    data = quest_entry.get()
    print(f'send {data} to server2')
    info['text'] = f'Server2 {data}'


root = Tk()

root['bg'] = '#fafafa'
root.title('Client')
root.geometry('300x200')
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

info = Label(frame_bot, text='Information', bg='#ffb700', font=40)
info.pack()

root.mainloop()