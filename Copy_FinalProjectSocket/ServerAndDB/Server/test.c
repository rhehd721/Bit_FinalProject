#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>     // 파일 처리 함수가 선언된 헤더 파일
#include <stdlib.h>    // malloc, free 함수가 선언된 헤더 파일
#include <stdbool.h>   // bool, true, false가 정의된 헤더 파일
#include <string.h>    // strchr, memset, memcpy 함수가 선언된 헤더 파일

#include "./ReadJson.h"

int main(){
    char ** list = R();

    // char * Item_Name;
    // char * Amount;
    // char * ImagePath;
    // char * Item_place;
    // char * time;

    // printf("%s \n", *list);
    printf("살려주세요 \n");
    // sprintf(Item_Name, "%s", *(list + 0));
    // sprintf(Amount, "%s", *(list + 1));
    // sprintf(ImagePath, "%s", *(list + 2));
    // sprintf(Item_place, "%s", *(list + 3));
    // sprintf(time, "%s", *(list + 4));

    // printf("%s, %s, %s, %s, %s",Item_Name, Amount, ImagePath, Item_place, time );
    
    printf("%s \n", (*list + 0));
    printf("%s \n", (*(list + 1)));
    printf("%s \n", (*(list + 2)));
    // printf("%s \n", (*(list+1) ));
    // printf("%s \n", (*(list) ));
    // printf("%s \n", (*(list+ sizeof(**list)) ));
        // printf("%s \n", (*(list+ (sizeof(**list)) )));

    // printf("%s \n", (list));
    // printf("%s \n", *(list + 3));
    // printf("%s \n", *(list + i));
    
    
    // free....
}

