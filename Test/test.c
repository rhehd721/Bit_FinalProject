#include<stdio.h>


char * re (){
    return "HELLO_WORLD";
}

int main(){
    char name[100];
    char * retur = re();

    for (int i =0; retur[i] != '_'; i++){
        name[i] = retur[i];
        printf("%s\n", retur);
        printf("cccc : %c \n",retur[i] );
    }

    printf("%s\n", name);
    // printf("%s\n", retur);

    return 0;
}
