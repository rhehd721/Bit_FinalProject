// DB와 연결시키는 코드
// 완료

#include <mysql.h>
#include <string.h>
#include <stdio.h>

// DB정보 
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASS "1234"
#define DB_NAME "Bit"

// DB와 연결 후 return Connection 
MYSQL *DB_Connect()
{

    // MYSQL	데이타 베이스에 연결했을때, 이 연결을 다루기 위해 사용되는 구조체 이다.
    MYSQL *connection=NULL, conn;
    
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

    return connection;
}