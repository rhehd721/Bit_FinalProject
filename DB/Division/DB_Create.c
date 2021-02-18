// 이 프로그램이 하는 일은 최초 mysql DB에 연결을 한다음에 query 를 통하여 address 의 내용을 가져와서 화면에 출력시켜주고. 
// 다음에 사용자 입력을 받아서 DB에 저장하는데까지다. 

#include <mysql.h>
#include <string.h>
#include <stdio.h>

// DB정보 
#define DB_HOST "127.0.0.1"
#define DB_USER "test"
#define DB_PASS "1234"
#define DB_NAME "test"
#define CHOP(x) x[strlen(x) - 1] = ' '
    
int main(MYSQL * (*connect))
{

    // MYSQL	데이타 베이스에 연결했을때, 이 연결을 다루기 위해 사용되는 구조체 이다
    // MYSQL_RES	(SELECT, SHOW, DESCRIBE, EXPLAIN)등의 쿼리를 내렸을때 그 결과를 다루기 위해 사용되는 구조체이다.
    // MYSQL_ROW	이것은 데이타의 하나의 row 값을 가리킨다. 만약 row 값이 없다면 null 을 가르키게 된다.
    // MYSQL_FIELD	이 구조체는 각 필드의 정보를 가지고 있다. 여기에는 필드의 이름, 타입, 크기 등의 정보를 가지게 된다. mysql 에서 DESC 쿼리를 내렸을때의 정보를 가지고 있다고 보면된다.
    // MYSQL_FIELD_OFFSET	mysql 필드 리스트의 위치를 가진다.

    

    MYSQL       *connection=NULL
    // 이부분 다시
    connection = *connect
    MYSQL_RES   *sql_result;
    MYSQL_ROW   sql_row;
    int       query_stat; 

    // sql에 query를 날린다.
    // 정상적으로 query가 보내졌다면 0을 return
    query_stat = mysql_query(connection, "select * from address");
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }
    
    // 쿼리로부터 받은 결과값을 mysql_store_result을 통해 받아온다
    sql_result = mysql_store_result(connection);
    
    printf("%+11s %-30s %-10s", "이름", "주소", "전화번호");
    // sql_result로부터 받아온 값들을 하나씩 출력한다.
    while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
    {
        printf("%+11s %-30s %-10s", sql_row[0], sql_row[1], sql_row[2]);
    }

    // 더이상 row(값)이 필요없다면 메모리를 돌려준다.
    mysql_free_result(sql_result);

}