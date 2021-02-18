// DB에 쿼리를 보내 데이터를 읽어오는 코드

#include <mysql.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
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