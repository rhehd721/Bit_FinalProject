// DB 종료코드

#include <mysql.h>
    
int main(void)
{
    // DB사용을 종료한다.
    mysql_close(connection);
}