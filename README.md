# Bit_FinalProject
[DB](https://github.com/rhehd721/Bit_FinalProject/tree/main/DB)

[Socket](https://github.com/rhehd721/Bit_FinalProject/tree/main/Socket)

## 개발환경 
Linux_Ubuntu

## Linux C++ compile

1. g++설치 <sudo apt install g++>
2. compile <g++ -o (실행파일명) (파일경로)>
3. run <./(파일경로)> 

## Linux C compile

1. compile <gcc -o (실행파일명) (파일경로)>
2. run <./(파일경로)> 

## Python, C 연동 <pdf파일 참조>
C 코드를 파이썬에서 쓸려면 리눅스에서는 .a(정적라이브러리)나 .so(동적라이브러리)로 만들어야 한다.
1. gcc -c -fPIC test.c
2. gcc -shared -fPIC -o test.so test.o
```python
from ctypes import *

# load file
C_File = CDLL(‘./test.so’) ## DLL을 읽어옴

res_int = C_File.main()
```

## Mysql
[참고 blog](https://m.blog.naver.com/jesang1/221993846056)

### 설치 및 설정

1. 설치 : sudo apt-get install mysql-server
2. 3306Port Open : sudo ufw allow mysql
3. 실행 : sudo systemctl start mysql
4. Ubuntu 서버 재시작시 Mysql 자동 재시작 : sudo systemctl enable mysql
5. Mysql 접속 : sudo /usr/bin/mysql -u root -p
6. Version 확인 : mysql> show variables like "%version%";
7. 비밀번호 변경 : SET PASSWORD FOR 'root'@'localhost' = PASSWORD('new password'); * 괄호와 '' 포함
8. 사용자 정보 확인 : SELECT User, Host, authentication_string FROM mysql.user;

### DB 다루기

1. 생성 : CREATE DATABASE (DataBaseName);
2. 확인 : SHOW DATABASES;
3. 새로운 사용자 생성 : CREATE USER 'newUser'@'localhost' IDENTIFIED BY 'mysql password';
4. 변경사항 새로고침 : FLUSH PRIVILEGES; 
5. 권한 부여 : GRANT ALL PRIVILEGES ON 데이터베이스이름.* FOR'권할을 부여할 사용자'@'localhost';
6. DB 보기 : SHOW GRANTS FOR'UserName'@'localhost';

## .encode('utf-8')
python에서 c언어로 문자열을 전송할때 < 문자열.encode('utf-8') > 으로 전달해야한다.
