// DB에 쿼리를 보내 데이터를 읽어오는 코드

#include <mysql.h>
#include <string.h>
#include <stdio.h>

#define RETURN_NAME 30

char * Load_NAME(MYSQL **connection, int Command, char *IDorRas_pi)
{
    MYSQL_ROW sql_row;
    int query_stat; 
    char ReturnName[RETURN_NAME];
    
    // sql에 query를 날린다.
    // 정상적으로 query가 보내졌다면 0을 return
    if (Command == 0){
        query_stat = mysql_query(connection, "select RaspberryNo from User_Table where <ID = ????>");
    }
    else if (Command == 1){
        query_stat = mysql_query(connection, "select ID from User_Table where <RaspberryNo = ????>");
    }
    else{
        return -1;
    }
    
    if (query_stat != 0)
    {
        fprintf(stderr, "Mysql query error : %s", mysql_error(&conn));
        return 1;
    }
    
    // 쿼리로부터 받은 결과값을 mysql_store_result을 통해 받아온다
    sql_result = mysql_store_result(connection);
    // sql_result로부터 받아온 값들을 하나씩 출력한다.
    while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
    {
        sprintf(ReturnName, "%s", sql_row[0]);
    }

    // 더이상 row(값)이 필요없다면 메모리를 돌려준다.
    mysql_free_result(sql_result);

    return &ReturnName;
}