#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
// #include <error.h>
  
int main()
{
	int index = 0;
	char * maxFile[100];
    DIR *dir;
    struct dirent *ent;
    dir = opendir ("./");
    if (dir != NULL) {
  
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
		maxFile[index++] = ent->d_name;
        printf ("%s\n", ent->d_name);
    }
	printf(" 총 파일의 갯수 : %d \n", index - 1);
    closedir (dir);
    } else {
         /* could not open directory */
         perror ("");
        return EXIT_FAILURE;
    }
}