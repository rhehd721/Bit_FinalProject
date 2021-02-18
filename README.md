# Bit_FinalProject
[DB]("./DB")

[Socket]("./Socket")

## 개발환경 
Linux_Ubuntu

## Linux C++ compile

1. g++설치 <sudo apt install g++>
2. compile <g++ -o (실행파일명) (파일경로)>
3. run <./(파일경로)> 

## Linux C compile

1. compile <gcc -o (실행파일명) (파일경로)>
2. run <./(파일경로)> 

## Python, C 연동 <pdf파일 참조>
C 코드를 파이썬에서 쓸려면 리눅스에서는 .a(정적라이브러리)나 .so(동적라이브러리)로 만들어야 한다.
1. gcc -c -fPIC test.c
2. gcc -shared -fPIC -o test.so test.o
```python
from ctypes import *

# load file
C_File = CDLL(‘./test.so’) ## DLL을 읽어옴

res_int = C_File.main()
```

## .encode('utf-8')
python에서 c언어로 문자열을 전송할때 < 문자열.encode('utf-8') > 으로 전달해야한다.
