#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(){
   int fd = open("myfifo",O_WRONLY);
   if(fd<0){
        perror("open");
        return 1;
   }
   char buf[1024];
   while (1)
   {
        printf("please Enter mtx and order#");
        fflush(stdout);

        ssize_t len = read(0,buf,sizeof(buf)-1);
        if (len>0)
        {
            buf[len]=0;
	    write(fd,buf,strlen(buf));
        }
        else{
            perror("read");
            return 1;
        }
   }
   close(fd);
   return 0;
}
