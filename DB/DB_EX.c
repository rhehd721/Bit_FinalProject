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
    
int main(void)
{

    // MYSQL	데이타 베이스에 연결했을때, 이 연결을 다루기 위해 사용되는 구조체 이다
    // MYSQL_RES	(SELECT, SHOW, DESCRIBE, EXPLAIN)등의 쿼리를 내렸을때 그 결과를 다루기 위해 사용되는 구조체이다.
    // MYSQL_ROW	이것은 데이타의 하나의 row 값을 가리킨다. 만약 row 값이 없다면 null 을 가르키게 된다.
    // MYSQL_FIELD	이 구조체는 각 필드의 정보를 가지고 있다. 여기에는 필드의 이름, 타입, 크기 등의 정보를 가지게 된다. mysql 에서 DESC 쿼리를 내렸을때의 정보를 가지고 있다고 보면된다.
    // MYSQL_FIELD_OFFSET	mysql 필드 리스트의 위치를 가진다.

    MYSQL *connection=NULL, conn;
    MYSQL_RES *sql_result;
    MYSQL_ROW sql_row;
    int query_stat; 

    char name[12];
    char address[80];
    char tel[12];
    char query[255];
    
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

    // 추가적으로 DB에 입력할 정보들을 사용자로부터 받아온다.
    printf("이름 :");
    fgets(name, 12, stdin);
    CHOP(name);

    printf("주소 :");
    fgets(address, 80, stdin);
    CHOP(address);

    printf("전화 :");
    fgets(tel, 12, stdin);
    CHOP(tel);

    // 받아온 데이터를 DB에 넣어준다.
    sprintf(query, "insert into address values "
                   "('%s', '%s', '%s')",
                   name, address, tel);

    // 명령이 잘 들어갔는지 확인한다.
    query_stat = mysql_query(connection, query);
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }

    // DB사용을 종료한다.
    mysql_close(connection);
}