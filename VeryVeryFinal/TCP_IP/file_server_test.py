## file_sever.py
# -*- coding: utf-8 -*- 

import socket
from os.path import exists
import os



def run():
    host = "192.168.1.133"
    port = 8080
    directory = "./result"
    json = ["sample1.json"]
    image = ["3.png", "2.png", "1.png"]
   
    


    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((host, port))

    for i in range(0, len(json)):
            sendfirst = 1
            s.sendall("2".encode())  # json 보낸다
            
#            sendsec = getFileData(json[i],directory)
#            print("sendsec:", sendsec)
#            s.sendall(getFileData(json[i],directory).encode())
#            print("json send done")
#        getFileData(json[i],directory).encode("UTF-8")
#            s.sendall(sendsec.encode("UTF-8"))  # image 보낸다
#
            s.sendall(sendimg(image[i],directory))
            print("img sent")
#            sendthird = str(3)
#            s.sendall(sendthird.encode("UTF-8"))  # 끝났다

    s.close()


## 파일의 내용을 반환하는 함수
def getFileData(fileName, directory):
    with open(directory + "/" + fileName, 'r') as f:
        data = ""
        ## 파일이 매번 각 라인을 읽어 리턴할 수 있기 때문에 라인마다 끊어서 저장
        for line in f:
            print(f)
            print("line:", line)
            data += line
        print(data)
#        byte_date = data.to_bytes(4, order = 'little')
    f.close()
    return data
def sendimg(filename, directory):
    filedir = os.path.join(directory, filename)
    myimg = open(filedir, 'rb')
    bytes = myimg.read()
    myimg.close()
    return bytes


if __name__ == '__main__':
    run()
