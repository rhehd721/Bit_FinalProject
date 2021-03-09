// 폴더를 생성하는 코드
// 완료

#include <stdio.h>
#include <direct.h>		//mkdir

int main()
{
	char SaveTxtPath[] = { "/home/mango/Desktop/SaveJson" };
	char SaveImagePath[] = { "/home/mango/Desktop/SaveImage" };
	
	int TxtResult = mkdir( SaveTxtPath );
	int ImageResult = mkdir( SaveImagePath );

	if( TxtResult == 0 && ImageResult == 0)
	{
		printf( "폴더 생성 성공" );
	}
	else if( TxtResult == -1 || ImageResult == -1 )
	{
		printf( "폴더 생성 실패 - 폴더가 이미 있거나 부정확함\n" );
	}

	return 0;
}