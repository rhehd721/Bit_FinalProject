#include <stdio.h>
#include <stdlib.h>

int main(void){
    char name[20];

    char hi[] = "hi";
    char hello[] = "hello";

    sprintf(name, "%s %s" , hi, hello);
    printf("%s", name);

    return 0;
}