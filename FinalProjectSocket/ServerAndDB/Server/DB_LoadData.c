// DB에 쿼리를 보내 데이터를 읽어오는 코드

#include <mysql.h>
#include <string.h>
#include <stdio.h>

#define RETURN_NAME 30
#define RETURN_PASS 50

// RaspberryNo or UserName 반환
char * Login(MYSQL *connection, int Command, char *IDorRas_pi)
{
    MYSQL_ROW sql_row;
    MYSQL_RES *sql_result;
    int query_stat;
    char query[255] = NULL;
    char ReturnName[RETURN_NAME];
    
    // sql에 query를 날린다.
    // 정상적으로 query가 보내졌다면 0을 return
    if (Command == 0){
        sprintf(query, "select RaspberryNo from User_Table where %s",
                   IDorRas_pi);
        query_stat = mysql_query(connection, query);
    }
    else if (Command == 1){
        sprintf(query, "select ID from User_Table where %s",
                   IDorRas_pi);
        query_stat = mysql_query(connection, query);
    }
    else{
        return -1;
    }
    
    if (query_stat != 0)
    {
        return -1;
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

// User의 Password를 반환
char * Load_PASS(MYSQL *connection, char *ID)
{
    MYSQL_ROW sql_row;
    MYSQL_RES *sql_result;
    int query_stat; 
    char ReturnPass[RETURN_PASS];
    char query[255] = NULL;
    
    // PASS를 받아오는 쿼리
    sprintf(query, "select PASS from User_Table where %s",
                   ID);
    query_stat = mysql_query(connection, query);
    
    if (query_stat != 0)
    {
        return -1;
    }
    
    // 쿼리로부터 받은 결과값을 mysql_store_result을 통해 받아온다
    sql_result = mysql_store_result(connection);
    // sql_result로부터 받아온 값들을 하나씩 출력한다.
    while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
    {
        sprintf(ReturnPass, "%s", sql_row[0]);
    }

    // 더이상 row(값)이 필요없다면 메모리를 돌려준다.
    mysql_free_result(sql_result);

    return &ReturnPass;
}

    
// int main(MYSQL *connection)
// {
//     MYSQL_RES *sql_result;
//     MYSQL_ROW sql_row;
//     int query_stat; 

//     // sql에 query를 날린다.
//     // 정상적으로 query가 보내졌다면 0을 return
//     sprintf(query, "select RaspberryNo from User_Table where %s",
//                    IDorRas_pi);
//     query_stat = mysql_query(connection, "select * from address");
//     if (query_stat != 0)
//     {
//         return -1;
//     }
    
//     // 쿼리로부터 받은 결과값을 mysql_store_result을 통해 받아온다
//     sql_result = mysql_store_result(connection);
    
//     printf("%+11s %-30s %-10s", "이름", "주소", "전화번호");
//     // sql_result로부터 받아온 값들을 하나씩 출력한다.
//     while ( (sql_row = mysql_fetch_row(sql_result)) != NULL )
//     {
//         printf("%+11s %-30s %-10s", sql_row[0], sql_row[1], sql_row[2]);
//     }

//     // 더이상 row(값)이 필요없다면 메모리를 돌려준다.
//     mysql_free_result(sql_result);

// }