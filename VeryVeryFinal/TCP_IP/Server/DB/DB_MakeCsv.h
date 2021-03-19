// SELECT * FROM Inventory_Table
// INTO OUTFILE '/var/lib/mysql-files/orders.csv'
// FIELDS TERMINATED BY ','
// ENCLOSED BY '"'
// LINES TERMINATED BY '\n'

int MakeCsv(MYSQL *connection)
{
    int query_stat; 

    char query[500];

    // 받아온 데이터를 DB에 넣어준다.
    sprintf(query, " SELECT * FROM Inventory_Table INTO OUTFILE '/var/lib/mysql-files/orders.csv' FIELDS TERMINATED BY ',' ENCLOSED BY '""' LINES TERMINATED BY '\n' " );

    // 명령이 잘 들어갔는지 확인한다.
    query_stat = mysql_query(connection, query);
    if (query_stat != 0)
    {
        return -1;
    }

    return 0;
}