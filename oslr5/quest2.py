import argparse

def function(file_name, count):
    try:
        a = []
        with open(file_name) as f:
            for line in f.readlines():
                a.append(line.split('\n'))
        cnt = 0
        while(cnt!=len(a)):
            print(a[cnt])
            if cnt%count==0:
                input('enter any char for continue')
            cnt+=1
        print('file is over')
    except any:
        print('Can\'t open the file')
            


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('file_name')
    parser.add_argument('count')
    args = parser.parse_args()
    file_name = args.file_name
    count = args.count
    count = int(count)
    function(file_name, count)