// DB Test 코드

#include "usr/include/mysql/mysql.h"
#include <string.h>
#include <stdio.h>

// DB정보 
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "1234"
#define DB_NAME "final"
    
int main(void)
{

    int query_stat;

    // MYSQL	데이타 베이스에 연결했을때, 이 연결을 다루기 위해 사용되는 구조체 이다
    // MYSQL_RES	(SELECT, SHOW, DESCRIBE, EXPLAIN)등의 쿼리를 내렸을때 그 결과를 다루기 위해 사용되는 구조체이다.
    // MYSQL_ROW	이것은 데이타의 하나의 row 값을 가리킨다. 만약 row 값이 없다면 null 을 가르키게 된다.
    // MYSQL_FIELD	이 구조체는 각 필드의 정보를 가지고 있다. 여기에는 필드의 이름, 타입, 크기 등의 정보를 가지게 된다. mysql 에서 DESC 쿼리를 내렸을때의 정보를 가지고 있다고 보면된다.
    // MYSQL_FIELD_OFFSET	mysql 필드 리스트의 위치를 가진다.

    MYSQL       *connection=NULL, conn;
    
    // mysql_init()함수는 MySQL 서버를 핸들링할 객체를 메모리에 할당하고 초기화 하는 역활을 합니다.
    mysql_init(&conn);

    // 초기화를 마쳤으니 DB연결
    connection = mysql_real_connect(&conn, DB_HOST,
                                    DB_USER, DB_PASS,
                                    DB_NAME, 3306,
                                    (char *)NULL, 0);

    // mysql_init()함수의 결과값을 검사합니다. 초기화에 실패했다면, 화면에 에러를 출력하고 종료하는 코드입니다.
    // 즉, 연결이 실패했다면 에러 출력
    if (connection == NULL)
    {
        fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
        return 1;
    }
    // sql에 query를 날린다.
    // 정상적으로 query가 보내졌다면 0을 return
    query_stat = mysql_query(connection, "create table UserImformation(
        id int)");
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }
    mysql_close(connection);

}