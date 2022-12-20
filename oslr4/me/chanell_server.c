#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(){
    umask(0);
    int ret = mkfifo("myfifo",0666);
    if(ret<0){
        perror("mkfifo");
        return 1;
    }
    
    char buf[1024];
    int fd = open("myfifo",O_RDONLY);
    while (1)
    {
        int len = read(fd,buf,sizeof(buf)-1);
        if(len>0){
		    char command[] = "python3 ./gauss.py ";
            buf[len] = 0;
		    strcat(command, buf);
		    //printf(command);
		    system(command);
        }
        else if (len==0){
            printf("client quit!\n");
            break;
        }
        else{
            perror("read");
            return 1;
        }
    }
    close(fd);
    return 0;
}
