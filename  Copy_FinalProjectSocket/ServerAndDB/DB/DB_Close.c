// DB 종료코드
// 완료

#include <mysql.h>
    
void DB_Close(MYSQL * connection)
{
    // DB사용을 종료한다.
    mysql_close(connection);
}