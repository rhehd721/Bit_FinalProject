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

    def sum(self, low, high):
        total = 0
        for i in range(low, high):
            total += i
        print("Subthread", total)
        while(1):
            pass

    def initUI(self):
        
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
        btn.clicked.connect(self.btnRun_clicked)

        self.setLayout(layout)

        self.setWindowTitle('GUI')
        self.setLayout(layout)
        self.setGeometry(300, 300, 300, 200)
        self.show()
        

    # 동작 명령어를 눌렀을때 작동하는 부분
    def btnRun_clicked(self):
        # libc = ctypes.CDLL("./test.so")
        # main = libc.main("127.0.0.1".encode('utf-8'), "8080".encode('utf-8'), "park".encode('utf-8'))
        self.label.setText("?")
        self.label.repaint()

        t = threading.Thread(target=self.sum, args=(1, 100000))
        t.start()
        # self.sum(1, 10000)
        # C파일 불러오기
        # counter = libc.Event_Start()

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = MyApp()
    sys.exit(app.exec_())


# import threading
 
# def sum(low, high):
#     total = 0
#     for i in range(low, high):
#         total += i
#     print("Subthread", total)
 
# t = threading.Thread(target=sum, args=(1, 100000))
# t.start()
 
# print("Main Thread")