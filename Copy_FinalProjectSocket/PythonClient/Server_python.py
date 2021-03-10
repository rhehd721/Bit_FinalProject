## file_sever.py
# -*- coding: utf-8 -*- 

import socket
import argparse
from os.path import exists
import os

def run_server(port, directory):
    host = '' 

    with socket.socket(family=socket.AF_INET, type=socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen(1)   
        
        conn, addr = s.accept()
        fileName = conn.recv(1024)
        fileName = fileName.decode()
        
        ## 해당 경로에 파일이 없으면 에러
        # if not exists(directory+"\\"+fileName):
        if not exists(directory+"/"+fileName):
                msg = "error"
                conn.sendall(msg.encode())
                conn.close()
                return
        
        conn.sendall(getFileSize(fileName, directory).encode())
                
        ## client가 파일 내용을 받을 준비 확인
        reReady = conn.recv(1024)
        if reReady.decode() == "ready":
                conn.sendall(getFileData(fileName, directory).encode())
        conn.close()

## 파일의 크기를 반환하는 함수
def getFileSize(fileName, directory):
        # fileSize = os.path.getsize(directory+"\\"+fileName)
        fileSize = os.path.getsize(directory+"/"+fileName)
        return str(fileSize)

## 파일의 내용을 반환하는 함수
def getFileData(fileName, directory):
        # with open(directory+"\\"+fileName, 'r', encoding="UTF-8") as f:
        with open(directory+"/"+fileName, 'r', encoding="UTF-8") as f:
            data = ""
            ## 파일이 매번 각 라인을 읽어 리턴할 수 있기 때문에 라인마다 끊어서 저장
            for line in f:
                data += line
        return data

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Echo server -p port -d directory")
    parser.add_argument('-p', help="port_number", required=True)
    parser.add_argument('-d', help="directory", required=True)

    args = parser.parse_args()
    run_server(port=int(args.p), directory=args.d)