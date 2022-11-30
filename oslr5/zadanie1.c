#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    FILE *f;
    char s, *p, buf[150], fileName[80];
    int pid;
    long conv = strtol(argv[2], &p, 10);

    pid = conv;

    if (argc<3){
        printf("Enter filename and pid");
        return 0;
    }
    f = fopen(argv[1], "rw");
    if(!f){
        printf("File dont exist. Will be created with that name");
        if(!(f = fopen(argv[1], "w"))){
            printf("Cant open the file\n");
            return 0;
        }
        sprintf(buf, "%s %d %s %s", "pmap", pid, ">>", argv[1]);
        system(buf);
        printf("Write is ended");
        return 0;
    }
    else {
        printf("File exist. Do u wanna rewrite them (1) or change filename (2)?");
        s = getchar();
        if(s == '1'){
            fclose(f);
            remove(argv[1]);
            f = fopen(argv[1], "w");
            sprintf(buf, "%s %d %s %s", "pmap", pid, ">>", argv[1]);
            system(buf);
            fclose(f);
            printf("Write is ended");
            return 0;
        }
        else if (s=='2'){
            fclose(f);
            while(1){
                printf("Enter the uniq filename\n");
                scanf("%s", &fileName);
                f = fopen(fileName, "rw");
                if(!f){
                    if(!(f = fopen(argv[1], "w"))){
                        printf("Cant open the file\n");
                        return 0;
                    }
                    sprintf(buf, "%s %d %s %s", "pmap", pid, ">>", argv[1]);
                    system(buf);
                    printf("Write is ended");
                }
                else printf("File is existing");
            }
        }
        else{
            printf("Error with entering data");
            return 0;
        }
    }
}