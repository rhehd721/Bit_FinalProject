# 통신을 받으면 파일을 작성하는 코드를 수행 후 파일 전송
import os

# 폴더 생성
def createFolder(directory):
    try:
        if not os.path.exists(directory):
            os.makedirs(directory)
    except OSError:
        print ('Error: Creating directory. ' +  directory)
 
createFolder('./SendFileFolder')

# packet 통신부분
if (packet == 1):
    pass
elif( packet == 2):
    pass
elif( packet == 3):
    pass
elif( packet == 4):
    pass
else:
    pass

# 명령이 들어오면 packet에 해당하는 작업 실행 후 파일 작성
New_File = open("SendFileFolder/New.txt", 'w')
for i in range(1, 11):
    data = "%d번째 줄입니다.\n" % i
    New_File.write(data)
# file 사용이 끝나면 닫아주기
New_File.close()