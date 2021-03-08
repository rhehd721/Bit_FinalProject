#define _CRT_SECURE_NO_WARNINGS    // fopen 보안 경고로 인한 컴파일 에러 방지
#include <stdio.h>     // 파일 처리 함수가 선언된 헤더 파일
#include <stdlib.h>    // malloc, free 함수가 선언된 헤더 파일
#include <stdbool.h>   // bool, true, false가 정의된 헤더 파일
#include <string.h>    // strchr, memset, memcpy 함수가 선언된 헤더 파일

#include "./ReadJson.h"

int main(){
    char ** list = R();
    printf("%s", *(list + 2));
    free(*list + 2);
}

