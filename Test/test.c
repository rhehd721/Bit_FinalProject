# include <stdio.h>
#include <stdlib.h>
# include <string.h>

char* Name(char * name){

    char* Rname = NULL;
    
    Rname = (char*)malloc(strlen(name)+1);

    sprintf(Rname, "%s", name);

    return Rname;

}


int main(){
    
    char *reslut[10];

    printf("hello world \n");

    reslut[0] = Name("Hello");

    printf("%s\n", reslut[0]);




    return 0;
}