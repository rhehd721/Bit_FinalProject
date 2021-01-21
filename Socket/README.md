## TCP / IP <Linux_Ubuntu>
리눅스는 윈도우와 달리 소켓조작을 파일조작과 동일하게 간주한다.

---
## Socket 통신 

### Server
<br>

1. 소켓생성 <socket 함수호출>
``` c
#include <sys/socket.h>

// domain : 소켓이 사용할 프로토콜 체계 정보 전달
// type : 소켓의 데이터 전송방식에 대한 정보 전달
// protocol : 두 컴퓨터간 통신에 사용되는 프로토콜 정보 전달
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
<br>

4. 파일에 저장된 데이터 읽기
``` c
#include <unistd.h>

// fd : 데이터 수신대상을 나타내는 파일디스크립터 전달
// buf : 수신한 데이터가 저장할 버퍼의 주소 값 전달
// nbytes : 수신할 최대 바이트 수 전달
ssize_t read( int fd, const void *buf, size_t nbytes);
```

## 프로토콜(Protocol) : 소켓함수 1번째 인자
컴퓨터 상호간의 대화에 필요한 통신규약

<img src="./image/TCP_vs_UDP.png" width="40%" height="30%" title="TCP vs UDP" ></img>
<br>
<br>

### 프로토콜 체계(Protocol Family)
[헤더파일 sys/socket.h에 선언되어 있는 프로토콜 체계]
|이름|프로토콜 체계(Protocol Family)|
|------|---|
|PF_INET|IPv4 인터넷 프로토콜 체계|
|PF_INET6|IPv6 인터넷 프로토콜 체계|
|PF_LOCAL|로컬 통신을 위한 UNIX 프로토콜 체계|
|PF_PACKET|Low Level 소켓을 위한 프로토콜 체계|
|PF_IPX|IPX 노벨 프로토콜 체계|
<br>
<br>

## 소켓타입(Type) : 소켓함수 2번째 인자

### 연결지향형 소켓(SOCK_STREAM) vs 비 연결지향형 소켓(SOCK_DGRAM)
* 연결지향형 소켓(SOCK_STREAM) : 1대 1 컨베이너 벨트
  1. 중간에 데이터가 소멸되지 않고 목적지로 전송된다.
  2. 전송 순서대로 데이터가 수신된다.
  3. 전송 데이터의 경계(Boundary)가 존재하지 않는다.
* 비 연결지향형 소켓(SOCK_DGRAM) : 택배
  1. 전송 데이터의 손실, 파손의 우려가 있다.
  2. 전송되는 데이터의 경계(Boundary)가 존재한다.
  3. 한번에 전송할 수 있는 데이터의 크기가 존재한다.

## 소켓함수 3번째 인자

### 소켓함수의 3번째 인자는 다음과 같은 상황이 발생할 때 필요하다.
* 하나의 프로토콜 체계 안에 데이터의 전송방식이 동일한 프로토콜이 둘 이상 존재할 경우

ex1 ) IPv4 인터넷 프로토콜 체계에서 동작하는 연결지향형 테이터 전송 소켓 
``` c
int tcp_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
```
ex2 ) IPv4 인터넷 프로토콜 체계에서 동작하는 비 연결지향형 테이터 전송 소켓
``` c
int tcp_socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
```

## 소켓에 항당되는 IP주소와 PORT번호

### 인터넷 주소(Internet Address)
* IPv4(Internet Protocol version 4) : 4Byte 주소체계
* IPv6(Internet Protocol version 6) : 16Byte 주소체계

### 클래스 별 네트워크 주소와 호스트 주소의 경계
* 클래스 A의 첫 번쨰 바이트 범위 0 ~ 127
* 클래스 B의 첫 번쨰 바이트 범위 128 ~ 191
* 클래스 C의 첫 번쨰 바이트 범위 192 ~ 223
---
이는 다음과 같이 달리 표현될 수 있다.
* 클래스 A의 첫 번쨰 비트는 항상 0으로 시작
* 클래스 B의 첫 번쨰 비트는 항상 10으로 시작
* 클래스 C의 첫 번쨰 비트는 항상 110으로 시작

### 소켓의 구분에 활용되는 PORT번호
IP를 통해 서버를 찾았다면 PORT를 이용하여 서버에서 내가 필요로 하는 프로그램을 찾는다.
<br>
그러므로 PORT번호는 하나의 소켓에만 할당이 가능하다 (2이상 소켓에 할당할 수 없다.
<br>
포트번호는 16비트로 표현되며 0 ~ 65535이하개가 존재하며 이중 0 ~ 1023번 까진 특정 프로그램에 할당되어 있다.
<br>
! TCP소켓과 UDP소켓은 PORT를 공유하지 않기 때문에 중복이 가능하다!

## 네트워크 바이트 순서

### 빅 엔디안(Big Endian) vs 리틀 엔디안(Little Endian)
* 빅 엔디안(Big Endian) : 상위 바이트의 값을 작은 번지수에 저장
* 리틀 엔디안(Little Endian) : 상위 바이트의 값을 큰 번지수에 저장

네트워크상 테이터를 전송할 땐 빅 엔디안을 사용하기로 약속

## Half-Close

### 스트림(Stream) : 소켓을 통해 두 호스트가 연결된 상태

<img src="./image/Stream.png" width="45%" height="30%" title="Stream" ></img>

Half-Close 는 2개의 입출력 스티림중 1개의 스트림을 끊어버리는 것을 의미한다. 
``` c
#include <sys/socket.h>

// sock : 종료할 소켓의 파일 디스크립터 전달
// howto : 종료방법에 대한 정보 전달
int shutdown(int sock, int howto);
```
이중 howto에 전달될 수 있는 매개변수는 다음과 같다.
1. SHUT_RD : 입력 스트림 종료
2. SHUT_WR : 출력 스트림 종료
3. SHUT_RDWR : 입출력 스트림 종료