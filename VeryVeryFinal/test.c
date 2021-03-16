#include <stdio.h>
#include <time.h>
 
// int main() {
//     time_t t = time(NULL);
// struct tm tm = *localtime(&t);

// char * time = printf("%d-%d-%d %d:%d:%d\n",
// tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday,
// tm.tm_hour, tm.tm_min, tm.tm_sec);
// }

int main() {
    time_t t = time(NULL);
struct tm tm = *localtime(&t);

// printf("%d.%d.%d.%d.%d.%d\n",
// tm.tm_year - 100, tm.tm_mon+1, tm.tm_mday,
// tm.tm_hour, tm.tm_min, tm.tm_sec);

char name[100];
sprintf(name, "%d.%d.%d.%d.%d.%d hhhhh",
tm.tm_year - 100, tm.tm_mon+1, tm.tm_mday,
tm.tm_hour, tm.tm_min, tm.tm_sec);

printf("\n\n\n %s \n\n", name);
}
