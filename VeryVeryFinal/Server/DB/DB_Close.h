// DB 종료코드
    
void DB_Close(MYSQL * connection)
{
    // DB사용을 종료한다.
    mysql_close(connection);
}