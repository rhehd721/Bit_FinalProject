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
        btn = QPushButton(self)
        btn.setText('Button')

        # 라벨 폰트지정
        font1 = self.label.font()
        font1.setPointSize(20)
        

        # 폰트 적용
        self.label.setFont(font1)

        layout = QVBoxLayout()
        layout.addWidget(self.label)
        layout.addWidget(btn)

        # 버튼을 누루면 함수 활성화
        btn.clicked.connect(self.btnRun_clicked())

        self.setLayout(layout)

        self.setWindowTitle('GUI')
        self.setLayout(layout)
        self.setGeometry(300, 300, 300, 200)
        self.show()
        

    # 동작 명령어를 눌렀을때 작동하는 부분
    def btnRun_clicked(self):
        print(self.socket)
        print(type(self.socket))
        sock = self.socket
        SendMsg = ctypes.CDLL("./SendMsg.so")
        main = SendMsg.SendMsg(sock, "park".encode('utf-8'), "Start".encode('utf-8') )
        SendFile = ctypes.CDLL("./SendFile.so")
        main = SendFile.SendFile(sock)
        self.label.setText("Start")
        self.label.repaint()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = MyApp()
    sys.exit(app.exec_())