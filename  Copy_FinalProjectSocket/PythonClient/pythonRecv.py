## file_client.py

import socket
import argparse

host = "127.0.0.1"
port = "8080"

def run(host, port, fileName):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        s.sendall(fileName.encode())

        ## reSize = 파일 사이즈
        reSize = s.recv(1024)
        reSize = reSize.decode()
        
        ## 디렉토리에서 파일을 못찾아 error를 보냈을 경우
        if reSize == "error":
                print("파일을 찾을 수 없습니다.")
                return
        
        ## client가 파일 크기를 받았고, 파일 내용을 받을 준비가 되었다는 것을 알림
        msg = "ready"
        s.sendall(msg.encode())

        with open(fileName, 'w', encoding="UTF-8") as f:
            ## 파일 사이즈만큼 recv
            data = s.recv(int(reSize))
            f.write(data.decode())


        print("file name : "+fileName)
        print("size : "+reSize)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Echo client -p port -i host -f file")
    parser.add_argument('-p', help="port_number", required=True)
    parser.add_argument('-i', help="host_name", required=True)
    parser.add_argument('-f', help="file_name", required=True)

    args = parser.parse_args()
    run(host=args.i, port=int(args.p), fileName=args.f)