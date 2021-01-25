// gcc -o Thread Thread_Ex.c -lpthread
// gcc 컴파일 스크립트에 디폴트로 pthread 라이브러리가 포함되지 않아서 발생하는 에러다.
// `-lpthread` 를 옵션으로 줘서 pthread 라이브러리를 포함시켜 빌드하면 해결 할 수 있다.

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void * thread_main(void *arg);

int main(int argc, char *arg[]){
    pthread_t t_id;
    int thread_param = 5;

    if (pthread_create(&t_id, NULL, thread_main, (void*)&thread_param)!=0){
        puts("pthread_create() error");
        return -1;
    };
    sleep(10); puts("end of main");
    return 0;

}

void * thread_main(void *arg){
    int i;
    int cnt = *((int*)arg);
    for (i=0; i <cnt; i++){
        sleep(1); puts("running thread");
    }
    return NULL;
}