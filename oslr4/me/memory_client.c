#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <error.h>
#include <string.h>
int main(){
    int shm_id;
    char* share[1024];
    shm_id = shmget(1234,getpagesize(), IPC_CREAT);
    if(shm_id == -1){
        perror("shmget()");
    }
    *share = (char*)shmat(shm_id, 0, 0);
    char comand[255] = "python3.10 ./gauss.py ";
    
    while(1){
	char call[255];
	strcat(call, comand);
	strcat(call, *share);
	
	//printf("%s\n", call);
	system(call);
        //printf("%s\n", *share);
	//strcpy(string, *share);
        //printf("%s\n", *string);
	strcpy(call, "");
	sleep(5);
    }
    return 0;
}
