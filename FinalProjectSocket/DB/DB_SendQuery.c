// DB에 각종 쿼리를 보내는 예제 코드

#include <mysql.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
    // sql에 query를 날린다.
    // 정상적으로 query가 보내졌다면 0을 return
    query_stat = mysql_query(connection, "create table UserImformation(
        id varchar(20) not null primary key,
        pass varchar(20) not null,
        No int unique not null
    )");
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }

}