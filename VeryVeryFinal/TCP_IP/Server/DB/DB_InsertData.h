int Insert_Inventory(MYSQL *connection, char * Item_Name, int Amount, char * ImagePath, char * Item_place)
{
    // time_t t = time(NULL);
    // struct tm tm = *localtime(&t);

    // char * time = printf("%d-%d-%d %d:%d:%d\n",
    //      tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
    //      tm.tm_hour, tm.tm_min, tm.tm_sec);
    int query_stat; 

    char query[255];

    // 받아온 데이터를 DB에 넣어준다.
    sprintf(query, "insert into Inventory_Table values "
                   "('%s', '%d', now(), '%s', '%s')",
                   Item_Name, Amount, ImagePath, Item_place);

    // 명령이 잘 들어갔는지 확인한다.
    query_stat = mysql_query(connection, query);
    if (query_stat != 0)
    {
        return -1;
    }

    return 0;
}