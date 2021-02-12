# -*- coding: utf-8 -*- 
import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt
import ctypes
import threading


class MyApp(QWidget):

    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        OpenSoket = ctypes.CDLL("./OpenSocket.so")
        self.socket = OpenSoket.OpenSocket("127.0.0.1".encode('utf-8'), "8080".encode('utf-8'), "park".encode('utf-8'))
        
        # 라벨 생성 및 센터 배치
        self.label = QLabel('Label', self)
        self.label.setAlignment(Qt.AlignCenter)

        # 버튼생성
        SendTextBtn = QPushButton(self)
        SendTextBtn.setText('SendText')

        SendImageBtn = QPushButton(self)
        SendImageBtn.setText('SendImage')

        SendMsgBtn = QPushButton(self)
        SendMsgBtn.setText('SendMsg')

        # 라벨 폰트지정
        font1 = self.label.font()
        font1.setPointSize(20)
        
        # 폰트 적용
        self.label.setFont(font1)

        layout = QVBoxLayout()
        layout.addWidget(self.label)
        layout.addWidget(SendTextBtn)
        layout.addWidget(SendImageBtn)
        layout.addWidget(SendMsgBtn)

        # 버튼을 누루면 함수 활성화
        SendTextBtn.clicked.connect(self.SendText())
        SendImageBtn.clicked.connect(self.SendImage())
        SendMsgBtn.clicked.connect(self.SendMsg())

        self.setLayout(layout)

        # file 받는 쓰레드
        RecvTh = threading.Thread(target=self.Recv)
        th2.start()

        self.setWindowTitle('GUI')
        self.setLayout(layout)
        self.setGeometry(300, 300, 300, 200)
        self.show()
        

    # 동작 명령어를 눌렀을때 작동하는 부분
    def SendText(self):
        sock = self.socket

        SendText = ctypes.CDLL("./SendText.so")
        main = SendText.SendText(sock)
        self.label.setText("SendText")
        self.label.repaint()

    def SendImage(self):
        sock = self.socket

        SendFile = ctypes.CDLL("./SendFile.so")
        main = SendFile.SendFile(sock)
        self.label.setText("SendFile")
        self.label.repaint()

    def SendMsg(self):
        sock = self.socket
        SendMsg = ctypes.CDLL("./SendMsg.so")
        main = SendMsg.SendMsg(sock, "park".encode('utf-8'), "Start".encode('utf-8') )

        self.label.setText("SendMsg")
        self.label.repaint()

    def Recv(self):

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = MyApp()
    sys.exit(app.exec_())