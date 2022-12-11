import os
import argparse

def function(file_name, pid):
    flag = os.path.isfile(file_name)
    if not flag:
        print('File don\'t exist. Will be created with that name')
        command = f'pmap {pid} >> {file_name}'
        os.system(command)
    if flag:
        print('File exist. Do u wanna rewrite it (1) or change File Name (2)?\n')
        c = input()
        if c == '1':
            command = f'pmap {pid} >> {file_name}'
            os.system(command)
        elif c == '2':
            while(1):
                print('Enter the UNIQ filename\n')
                file_name = input()
                flag = os.path.isfile(file_name)
                if not flag:
                    command = f'pmap {pid} >> {file_name}'
                    os.system(command)
                    break
                else: print('File is existing')



if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('file_name')
    parser.add_argument('pid')
    args = parser.parse_args()
    file_name = args.file_name
    pid = args.pid
    pid = int(pid)
    function(file_name, pid)