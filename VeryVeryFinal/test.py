## file_sever.py
# -*- coding: utf-8 -*- 

import socket
from os.path import exists
import os

def run():
    host = "192.168.1.133"
    prot = 8080
    directory = "./"
    json = []
    image = []

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))

        for (i in range(0, len(json))):

            s.sendall('1'.encode()) # json 보낸다
            s.sendall(getFileData(json[i], directory).encode())

            s.sendall('2'.encode()) # image 보낸다
            s.sendall(getFileData(image[i], directory).encode())

            s.sendall('3'.encode()) # 끝났다
                
        conn.close()

## 파일의 내용을 반환하는 함수
def getFileData(fileName, directory):
        with open(directory+"/"+fileName, 'r', encoding="UTF-8") as f:
            data = ""
            ## 파일이 매번 각 라인을 읽어 리턴할 수 있기 때문에 라인마다 끊어서 저장
            for line in f:
                data += line
        return data

if __name__ == '__main__':
    run()
