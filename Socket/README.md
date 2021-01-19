## TCP / IP <Linux_Ubuntu>
리눅스는 윈도우와 달리 소켓조작을 파일조작과 동일하게 간주한다.

---
## Socket 통신 

### Server
<br>

1. 소켓생성 <socket 함수호출>
``` c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
```
2. IP주소와 PORT번호 할당 <bind 함수호출>
``` c
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *myaddr, socklen_t addrlen);
```
3. 연결요청 가능상태로 변경 <listen 함수호출>
``` c
#include <sys/socket.h>

int listen(int sockfd, int backlog);
```
4. 연결요청에 대한 수락 <accept> 함수호출>
``` c
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
```

---

### Client
<br>

1. 소켓연결 <connect 함수호출>
``` c
#include <sys/socket.h>

int connect(int sockfd, struct sockaddr *serv_addr, socklen_t addrlen);
```
---

## 파일 관리
<br>

|파일 디스크립터|대 상|
|:--------------:|---|
|0|표준입력 : Standard Input|
|1|표준출력 : Standard Output|
|2|표준에러 : Standard Error|

<br>

1. 파일 열기
``` c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// path : 파일 이름을 나타내는 문자열의 주솟값
// flag : 파일 오픈 모드 정보 전달
int open(const char *path, int flag);
```
2. 파일 닫기
``` c
#include <unistd.h>

// fd : 닫고자 하는 파일 또눈 소켓의 파일 디스크립터 전달
int close(int fd);
```
3. 데이터 쓰기
``` c
#include <unistd.h>

// fd : 데이터 전송대상을 나타내는 파일디스크립터 전달
// buf : 전송할 데이터가 저장된 버퍼의 주소 값 전달
// nbytes : 전송할 데이터의 바이트 수 전달
size_t write( int fd, const void *buf, size_t nbytes);
```

|오픈 모드|의 미|
|------|---|
|O_CREAT|필요하면 파일을 생성|
|O_TRUNC|기존 데이터 전부 삭제|
|O_APPEND|기본 데이터 보존하고, 뒤에 이어서 저장|
|O_RDONLY|읽기 전용으로 파일 오픈|
|O_WRONLY|쓰기 전용으로 파일 오픈|
|O_RDWR|읽기, 쓰기 겸용으로 파일 오픈|