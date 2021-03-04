// DB에 쿼리를 통해 데이터를 저장하는 코드

#include "usr/include/mysql/mysql.h"
#include <string.h>
#include <stdio.h>

// DB정보 
#define DB_HOST "127.0.0.1"
#define DB_USER "test"
#define DB_PASS "1234"
#define DB_NAME "test"
#define CHOP(x) x[strlen(x) - 1] = ' '
    
int Insert_Inventory(MYSQL *connection, char * ID, char * Product_Name, int Amount, char * ImagePath, char * Time)
{
    int query_stat; 

    char query[255];

    // 받아온 데이터를 DB에 넣어준다.
    sprintf(query, "insert into %s_Inventory_Change_Table values "
                   "('%s', '%s', '%s', '%s)",
                   ID, Product_Name, Amount, ImagePath, Time);

    // 명령이 잘 들어갔는지 확인한다.
    query_stat = mysql_query(connection, query);
    if (query_stat != 0)
    {
        return -1;
    }

    // DB사용을 종료한다.
    mysql_close(connection);
}