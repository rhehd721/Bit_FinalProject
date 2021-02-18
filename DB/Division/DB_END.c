// 이 프로그램이 하는 일은 최초 mysql DB에 연결을 한다음에 query 를 통하여 address 의 내용을 가져와서 화면에 출력시켜주고. 
// 다음에 사용자 입력을 받아서 DB에 저장하는데까지다. 

#include <mysql.h>
    
int main(void)
{
    // DB사용을 종료한다.
    mysql_close(connection);
}