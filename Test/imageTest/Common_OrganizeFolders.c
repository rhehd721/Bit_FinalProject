#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
// #include <error.h>

int compare(const void *a , const void *b){

    return strcmp( (char*)a , (char*)b );

}


int main()
{
	int index = 0;
	char * maxFile[100] = {0};
    DIR *dir;
    struct dirent *ent;
    dir = opendir ("./im");
    if (dir != NULL) {
  
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
		maxFile[index++] = ent->d_name;
        printf ("%s\n", ent->d_name);
    }
	printf(" 총 파일의 갯수 : %d \n", index - 2);
    
    for (int i =0; i <10; i++){
        printf("\n %s", maxFile[i]);
    }
    printf(" \n 응아아아아ㅏ아앙\n\n");

    qsort(maxFile , 100 , sizeof(maxFile[0]) , compare);

    for (int i =0; i <10; i++){
        printf("\n %s", maxFile[i]);
    }



    closedir (dir);
    } else {
         /* could not open directory */
         perror ("");
        return EXIT_FAILURE;
    }
}