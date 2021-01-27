import sys
from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt
import ctypes


class MyApp(QWidget):

    def __init__(self):
        super().__init__()
        self.initUI()

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

        btn.clicked.connect(self.btnRun_clicked)

        self.setLayout(layout)

        self.setWindowTitle('GUI')
        self.setLayout(layout)
        self.setGeometry(300, 300, 300, 200)
        self.show()

    def btnRun_clicked(self):
        self.label.setText("?")
        self.label.repaint()
        libc = ctypes.CDLL("./test.so")
        counter = libc.main()


if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = MyApp()
    sys.exit(app.exec_())