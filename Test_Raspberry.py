# 통신을 받으면 파일을 작성하는 코드를 수행 후 파일 전송
import os

def createFolder(directory):
    try:
        if not os.path.exists(directory):
            os.makedirs(directory)
    except OSError:
        print ('Error: Creating directory. ' +  directory)
 
createFolder('./Send')

New_File = open("Send/New.txt", 'w')
for i in range(1, 11):
    data = "%d번째 줄입니다.\n" % i
    New_File.write(data)
New_File.close()