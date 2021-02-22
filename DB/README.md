# DB

## DB [행(low)과 열(column)으로 구성된 2차원 데이터]
[관련 youtube](youtube.com/watch?v=vgIc4ctNFbc&t=2090s)

1. Not NULL
2. Primary key (ex. 사번)
3. Unique (ex. 전화번호)
4. Foreign Key (DB에 저장되어있는 Data)

## SQL Language

### DML(Data Manipulation Language)
1. SELECT
2. INSERT
3. UPDATE
4. DELETE

### DDL(Data Definition Language)
1. CREATE
2. ALTER
3. DROP
4. RENAME
5. TRUNCATE

### DCL(Data Control Language)
1. GRANT
2. REVOKE

### TCL(Transaction Control Language)
1. COMMIT
2. ROLLBACK
3. SAVEPOINT

## 제약 조건

### PK(PRIMARY KEY) - 기본키
1. 태이블에 저장된 행 데이터를 고유하게 식별하기 위한 기본키 정의
2. 하나의 테이블에 하나의 기본키 제약만 정의할 수 있다.
3. 기본키 제약을 정의하면 DBMS는 자동으로 UNIQUE 인덱스를 생성하며, 기본키를 구성하는 컬럼에는 NULL을 입력할 수 없다.
4. PRIMARY KEY = UNIQUE KEY & NOT NULL

### UK(UNIQUE KEY) - 고유키
1. 테이블에 저장된 행 데이터를 고유하게 식별하기 위한 고유키를 정의한다. (NULL 입력 가능)

### NOT NULL
1. NULL 값의 입력을 금지한다

### CHECK
1. 입력할 수 있는 값의 범위 등을 제한한다. (CHECK 제약으로는 TRUE or FALSE로 정의)

### FOREIGN KEY
1. 테이블 간의 관계를 정의하기 위해 기본키를 다른 테이블의 외래키로 복사하는 경우 외래키가 생성된다.
2. 외래키 지정시 참조 무결성 제약 옵션을 선택할 수 있다.

---

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
5. 권한 부여 : GRANT ALL PRIVILEGES ON 데이터베이스이름.* to'권할을 부여할 사용자'@'localhost';
6. DB 보기 : SHOW GRANTS FOR'UserName'@'localhost';
7. 새로운 사용자 로그인 : mysql -u userName -p
8. 생성한 DB 사용 : USE DBname;

### mysql.h 연동

1. mysql.h 없을 시 다운
```
apt-get install libmysqlclient-dev
```

2. mysql.h 위치 찾는 명령어
```
mysql_config --cflags 
```
3. include  mysql.h 방법
```
#include "/usr/include/mysql/mysql.h"     (위에서 찾은 위치경로를 입력합니다)
```
4. 컴파일 방법
```
gcc -o sqlprogtam sqlprogram.c -lmysqlclient
```

### ERROR 1366
한글 사용시 발생하는 에러

```sql
ERROR 1366 (HY000): Incorrect string value: '\xED\x95\x9C\xEA\xB8\x80...' for column 'name' at row 1
```

1. whereis mysql (MySQL의 config 파일 위치 찾기)
2. cd /etc/mysql (my.cnf파일이 있는 폴더로 이동)
3. sudo vi my.cnf (root 권한으로 아래 내용을 설정파일의 제일 밑에 입력 후, 저장)
```sql
[mysql]
default-character-set = utf8

[client]
default-character-set = utf8

[mysqld]
character-set-server = utf8
collation-server = utf8_general_ci
init_connect='SET NAMES utf8'
```
4. sudo service mysql(mysqld) restart (MySQL 서비스 재시작)
5. 설정이 끝났으면 캐릭터셋을 확인하기 위해 MySQL에 접속해 status; 명령어를 입력.
6. 이미 만드어진 table 수정 : ALTER TABLE tableName CONVERT TO character SET utf8;

### ERROR 1184
/etc/mysql my.cnf 에서 init부분 오류
