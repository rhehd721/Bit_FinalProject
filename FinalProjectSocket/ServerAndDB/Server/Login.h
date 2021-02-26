// DB에 쿼리를 보내 데이터를 읽어오는 코드

#include <mysql.h>
#include <string.h>
#include <stdio.h>

#define RETURN_NAME 30
#define RETURN_PASS 50

// RaspberryNo or UserName 반환
int Login(MYSQL *connection, int Command, char *IDorRas_pi)
{
    MYSQL_ROW sql_row;
    MYSQL_RES *sql_result;
    int query_stat;
    char Check_CameraNum_query[255] = NULL;
    char Check_User_query[255] = NULL;
    char ReturnName[RETURN_NAME];
    char ID[100];
    
    if (Command == 0){  // Camera경우 Num만 확인한다.
        sprintf(query, "select RaspberryNo from User_Table where RaspberryNo = %s",
                   IDorRas_pi);
        query_stat = mysql_query(connection, query);
    }
    else if (Command == 1){ // Client접속의 경우 ID와 PASS를 확인한다.
        for (int i = 0; IDorRas_pi[i] != '_', i++){
           ID[i] = IDorRas_pi[i]
        }
        sprintf(query, "select ID, PASS from User_Table where ID = %s",
                   ID);
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

    if(Command == 0){
        while ( (sql_row = mysql_fetch_row(sql_result)) != NULL ){
            sprintf(ReturnName, "%s", sql_row[0]);
        }
        if (ReturnName != IDorRas_pi){
            return -1;
        }
    }
    else{
        while ( (sql_row = mysql_fetch_row(sql_result)) != NULL ){
            sprintf(ReturnName, "%s_%s", sql_row[0],sql_row[1]);
        }
        if (ReturnName != IDorRas_pi){
            return -1;
        }
    }
    
    // 더이상 row(값)이 필요없다면 메모리를 돌려준다.
    mysql_free_result(sql_result);

    return 1;
}