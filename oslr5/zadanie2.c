#include <stdlib.h>
#include <stdio.h>

int main(int argc, char ** argv){
    FILE *f;
    char s;
    char p*;
    int n;
    
    long viv = strtol(argv[2], &p, 10);
    n = viv;

    if(argc<3){
        printf("Enter filename and number of string in group");
        return 0;
    }
    
    if(f = fopen(argv[1], "r")){
        if(strcmp(argv[2],"0")==0){
            s=fgetc(f);
            while(s!=EOF){
                printf("%c", s);
                s = fgetc(f);
            }
        }
        else{
            while (1)
            {
                for(int i = 0; i<n;){
                    s = fgetc(f);
                    if(s!=EOF){
                        while(s!='\n'){
                            printf("%c", s);
                            s = fgetc(f);
                        }
                        printf("\n");
                        i++;
                    }
                    else if (feof(f)!=0){
                        printf("\n reading file is over\n");
                        return 0;
                    }
                    else{
                        printf("Error \n");
                        return 0;
                    }
                }
                printf("\nFor continue press any button");
                getchar();
            }
        }
        fclose(f);
    }
    else{
        printf("Cant open file \n");
    }
    return 0;
}