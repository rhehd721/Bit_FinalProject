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

INSERT 기본 문법
```sql
INSERT INTO 테이블명 (COLUMN_LIST)
VALUES (COLUMN_LIST에 넣을 VALUE_LIST);
```
INSERT_Ex.
```sql
INSERT INTO PLAYER (PLAYER_ID, PLAYER_NAME, TEAM_ID, POSITION, HEIGHT, WEIGHT, BACK_NO)
VALUES ('2002007', ' 박지성', 'K07', 'MF' ,178, 73, 7);
```
UPDATE 기본 문법
```sql
UPDATE 테이블명
SET 수정되어야 할 컬럼명 = 수정되기를 원하는 새로운 값;
```
UPDATE_Ex.
```sql
UPDATE PLAYER
SET BACK_NO = 99;
```
```sql
UPDATE PLAYER
SET BACK_NO = 99
WHERE PLAYER_NAME = '박지성';
```

### DDL(Data Definition Language)
1. CREATE
2. ALTER
3. DROP
4. RENAME
5. TRUNCATE

CREATE 기본 문법
```sql
CREATE TABLE 테이블 이름 (
        컬럼명1 DATATYPE [DEFAULT 형식],
        컬럼명2 DATATYPE [DEFAULT 형식],
        컬럼명3 DATATYPE [DEFAULT 형식]
);
```
CREATE_Ex.
```sql
CREATE TABLE PLAYER (
       PLAYER_ID                CHAR(7)             NOT NULL,
       PLAYER_NAME        VARCHAR2(20)  NOT NULL,
       TEAM_ID                   CHAR(3)             NOT NULL,
       E_PLAYER_NAME   VARCHAR2(40),
    
      CONSTRAINT PLAYER_PK PRIMARY KEY (PLAYER_ID),
      CONSTRAINT PLAYER_FK FOREIGN KEY (TEAM_ID) REFERENCES TEAM(TEAM_ID)
    
);
```
DROP 기본 문법
```sql
DROP TABLE 테이블명 [CASCADE CONSTRAINT];
```
RENAME 기본 문법
```sql
RENAME 변경전 테이블명 TO 변경후 테이블명
```

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

## 정규화(Normalization)
자료의 저장공간(중복)과 자료 뷸일치를 최소하하여 자교 구조를 안정화 시킴
1. 제1 정규형(First Normal Form : 1NF) : 정규형 과정을 통해 원자값이 아닌 도메인을 분해하여 어떤 릴레이션 R에 속한 모든 도메인이 원자값으로만 되어 있도록 설계
2. 제2 정규형(Second Normal Form : 2NF) : 어떤 릴레이션 R이 제1정규화에 속하고 기본키에 속하지 않는 모든 속성이 키본키에 완전 함수적 종속이면 충족하는 정규화
3. 제3 정규형(Third Normal Form : 3NF) : 어떤 릴레이션 R이 제2정규화에 있으며 기본키에 속하지 않는 모든 속성이 기본키에 이행적 함수 종속이 아닌 상태의 관계
* 보이스 코드 정규형(Boyce-Codd Normal Form : BCNF) : 릴레이션 R의 모든 결정자가 후보키이면 릴레이션 R은 Boyce-Codd 정규형에 속하는 상태

    * BCNF 정규형에 속하는 릴레이션은 모두 제3 정규형에 속하지만 역으로는 성립되지 않는다는 점도 기억해 두어야 할 중요한 포인트
4. 제4 정규형(Four Normal Form : 4NF) : 릴레이션 R에 다중치 종속(Multi Value Dependency : MVD) A ->>B가 존재할 때 R의 모든 속성도 A에 함수 종속이면 릴레이션 R은 제4 정규형에 속합니다.
5. 제5 정규형(Five Normal Form : 5NF) : 릴레이션 R에 존재하는 모든 조인 종속(JD)이 릴레이션 R의 후보키를 통해서만 성립된다면 릴레이션 R은 제5 정규형 또는 PJ/NF(Projection-Join Normal Form)에 속합니다.

## 저장 프로시저(Stored Procedure)
일련의 SQL 명령문을 하나의 함수처럼 실행하기 위한 쿼리의 집합
```sql
CREATE PROC [프로시저명]
AS
[쿼리문]
```
Ex.
```sql
CREATE PROCEDURE UP_EXPRO
(
    @P_COMPANY NVARCHAR(500),--회사코드
    @P_BIZAREA NVARCHAR(500),--고객사코드
    @P_PLANT NVARCHAR(500) --공장코드
)
AS
BEGIN
SET NOCOUNT ON 
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED;
	
    SELECT
    A.CD_SHOP,
    A.BIZAREA,
    B.NM_BIZAREA,
    FROM SA_Z_DZ_POS_SHOP_WJT A
    LEFT OUTER JOIN BIZAREA B ON A.COMPANY = B.COMPANY AND A.BIZAREA = B.BIZAREA
    WHERE A.CD_COMPANY = @P_CD_COMPANY AND(
    ISNULL(@P_CD_BIZAREA,'')='' OR @P_CD_BIZAREA = A.CD_BIZAREA)

SET NOCOUNT OFF
RETURN
END;
```

## 트리거(Trigger)
INSERT, DELETE, UPDATE 같은 DML 문 수행시 DB에서 자동으로 실행되도록 작성된 프로그램

## 뷰(View)
접근이 허용된 자료만을 제한적으로 보여주기 위해 기본 테이블로부터 유도된 가상의 테이블

1. 독립성 : 테이블 구조가 변경되어도 뷰를 사용하는 응용 프로그램은 변경하지 않아도 된다.
2. 편리성 : 복잡한 질의를 뷰로 생성함으로써 관련 질의를 단순하게 작성할 수 있다.
3. 보안성 : 숨기고 싶은 정보가 존재하는 경우, 뷰를 생성할 때 해당 컬럼을 빼고 생성하여 정보를 숨길 수 있다.

## 인덱스(Index)
Select의 속도는 빨라지나 Insert, Update 시에는 인덱스 수정이 발생하여 추가적인 I/O 발생

## ORDER BY(ASC, DESC)
기본형태 : select * from table_name ORDER BY cloumn_name;

## 서브쿼리(Subquery)
하나의 SQL 문에 포함되어 있는 또 다른 SQL 문

서브쿼리 사용시 주의사항
1. 서브쿼리를 괄호로 감싸서 사용한다.
2. 서브쿼리는 단일 행 또는 복수 행 비교 연산자와 함께 사용 가능하다.
3. 서브쿼리에서는 ORDER BY 를 사용하지 못한다.

서브쿼리가 사용 가능한 곳

1. SELECT 절
2. FROM 절
3. WHERE 절
4. HAVING 절
5. ORDER BY 절
6. INSERT 문의 VALUES 절
7. UPDATE 문의 SET 절

다중 행 비교 연산자(IN, ALL, ANY, SOME)

1. IN : 서브쿼리의 결과에 존재하는 임의의 값과 동일한 조건을 의미한다.
2. ALL : 서브쿼리의 결과에 존재하는 모든 값을 만족하는 조건을 의미한다.
3. ANY : 서브쿼리의 결과에 존재하는 어느 하나의 값이라도 만족하는 조건을 의미한다.
4. EXISTS : 서브쿼리의 결과를 만족하는 값이 존재하는지 여부를 확인하는 조건을 의미한다.

## JOIN
<img src="./img/SQL_JOIN.png" width="60%" height="45%" title="JOIN" ></img>

## 숫자형 자료형

1. TINYINT
    - 가장 작은 숫자 자료형.
    - 부호가 있는 경우 -128 ~ 127.
        - 부호가 없는 경우 0 ~ 255.
        - 1byte 크기를 갖는다.
2. SMALLINT
    - 부호가 있는 경우 -32768 ~ 32767.
    - 부호가 없는 경우 0 ~ 65535.
    - 2bytes 크기를 갖는다.
3. MEDIUMINT
    - 부호가 있는 경우 -8388608 ~ 8388607.
    - 부호가 없는 경우 0 ~ 16777215.
    - 3bytes 크기를 갖는다.
4. INT
    - 일반적인 숫자 자료형.
    - 부호가 있는 경우 -2147483648 ~ 2144483647.
    - 부호가 없는 경우 0 ~ 4294967295.
    - 4bytes 크기를 갖는다.
5. INTEGER
    - INT와 표기법만 다르고 같다.
6. BIGINT
    - 부호가 있는 경우 -922337036854775808 ~ 922337036854775807.
    - 부호가 없는 경우 0 ~ 18446744073709551615
    - 8bytes 크기를 갖는다.
7. FLOAT
    - 최소 +_1.175494351E-38 ~ 최대 +_3.402823466E_38.
    - 4bytes 크기를 갖는다.
8. DOUBLE
    - 최소 ±1.7976931348623157E-308 ~ 최대 ±2.2250738585072014E+308.
    - 8bytes 크기를 갖는다.
9. DECIMAL
    - 소수를 저장하는 용도로 사용.
    - 내부적으로는 문자 형태로 저장되는 타입이다.
    - 예를 들어, 3.141592 은 3 이 char 하나의 공간에 저장된다. 나머지 소수 한자리도 마찬가지다.
## 문자형 자료형
1. CHAR
    - CHAR(0) 도 지원한다.
    - CHAR(M)에서 실제값이 M만큼의 글자 수보다 작을 때 남는 자릿수만큼 공백을 붙여서 저장한다.
    - 즉, 지정한 크기만큼 공백을 붙여서라도 모두 사용한다는 뜻이다.
    - 1부터 최대 255 의 자릿수를 지원하며, CHAR(M)애서 M으로 지정한 용량만큼 바이트를 사용.
    - "입력한글자수" bytes 사용.
2. VARCHAR
    - 지정할 수 있는 길이(M)는 1 ~ 255 글자 수 지정.
    - VARCHAR(M)에서 사용한 길이(N)까지만 사용해서 저장.
    - char 형보다 기억장치를 효율적으로 입력한 글자 길이 만큼만 사용할 수 있다.
    - "입력한글자수"+1 bytes 사용.
3. TINYBLOB
    - 최대 255개의 문자를 저장.
    - "지정한 용량"+1 byte의 용량 사용.
4. TINYTEXT
    - TINYBLOB와 사용 형태가 같지만 text라는 명명의 의미가 같다.
5. BLOB
    - 최대 65535 개의 문자를 저장.
    - "지정한 용량"+2 byte의 용량 사용.
6. TEXT
    - BLOB와 사용 형태가 같지만 text라는 명명의 의미가 같다.
7. MEDIUMBLOB
    - 최대 16777215 개의 문자를 저장.
    - "지정한 용량"+3 byte의 용량 사용.
8. MEDIUMTEXT
    - MEDIUMBLOB와 사용 형태가 같지만 text라는 명명의 의미가 같다.
9. LONGBLOB
    - 최대 429496729 개의 문자를 저장.
    - "지정한 용량"+4 byte의 용량 사용.
10. LONGTEXT
    - LONGBLOB와 사용 형태가 같지만 text라는 명명의 의미가 같다.
11. ENUM
    - 입력한 문자형태의 값을 숫자로 저장.
    - value 중에 하나만 저장하며,
    - value가 ~255인 경우에는 1 byte 사용.
    - value가 256 ~ 65535인 경우에는 2 bytes 사용.
## 날짜형 자료형
1. DATE
    - '1001-01-01' ~ '9999-12-31' 를 저장할 수 있다.
    - 저장 용량은 3bytes가 필요.
    - 'YYYY-MM-DD' 와 같은 형식.
2. DATETIME
    - 날짜와 시간을 같이 저장한다.
    - '1001-01-01 00:00:00' ~ '9999-12-31 23:59:59' 를 저장할 수 있다.
    - 저장 용량은 8bytes가 필요.
    - 'YYYY-MM-DD HH:MM:SS' 와 같은 형식.
3. TIMESTAMP
    - '1970-01-01 00:00:00' 이후부터 초를 숫자로 저장하는 자료형.
    - 저장 용량은 4bytes가 필요.
    - 약 40억초를 저장할 수 있으므로 2037년까지의 값을 다룰 수 있다.
4. TIME
    - '-838:59:59' 에서 '838:59:59' 를 저장할 수 있다.
    - 저장 용량은 3bytes가 필요.
    - 'HH:MM:SS '와 같은 형식.
5. YEAR
    - 연도만 저장하는 자료형.
    - year(n)? 와 같은 형식.
    - n 은 2와 4를 지정할 수 있다.
    - n이 2인 경우에 값의 범위는 70 에서 69 .
    - n이 4인 경우에는 1970 에서 2069 .
    - 저장 용량은 1byte를 사용.

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
