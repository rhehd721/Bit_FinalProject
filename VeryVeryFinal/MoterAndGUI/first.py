from PyQt5 import uic
import sys
import cv2
from PyQt5.QtWidgets import *
from PyQt5.QtCore import Qt
from PyQt5.QtGui import QPixmap
form_class = uic.loadUiType("first.ui")[0]
from socket import *
import csv
import os





readlines = []
with open('test.csv', newline='') as f:
    reader = csv.reader(f)
    for row in reader:
        readlines.append(row)
for idx in range(1,len(readlines)):
    print(readlines[idx])


class SocketInfo():
    HOST = "192.168.1.133"
    PORT = 8080
    BUFSIZE = 4
    ADDR = (HOST, PORT)


class MyApp(QMainWindow,form_class):

    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.initUI()

        self.show()

    def initUI(self):
        self.setWindowTitle('first try')
        self.runAllBtn.clicked.connect(self.sendRunAll)
        self.stopBtn.clicked.connect(self.sendStop)
        self.leftBtn.clicked.connect(self.sendLeft)
        self.rightBtn.clicked.connect(self.sendRight)
        self.upBtn.clicked.connect(self.sendUp)
        self.downBtn.clicked.connect(self.sendDown)
        self.refreshBtn.clicked.connect(self.showDialog)
        self.showDialog()
        # self.runTab.currentTabText("runTab")
        self.call_csv()
        self.show()

    def call_csv(self):
        readlines = []
        with open('test.csv', newline='') as objInfo:
            reader = csv.reader(objInfo)
            for row in reader:
                readlines.append(row)
        max = len(readlines)

        for idx in range(1,max):
            obj = readlines[idx][0]
            amount = readlines[idx][1]
            time = readlines[idx][2]
            place = readlines[idx][4]
            self.set_col(idx-1, obj, amount, time, place)
        objInfo.close()


    def set_col(self, row, obj, amount, time, place):
        self.statustable.setItem(row, 0, QTableWidgetItem(obj))
        self.statustable.setItem(row, 1, QTableWidgetItem(amount))
        self.statustable.setItem(row, 2, QTableWidgetItem(time))
        self.statustable.setItem(row, 3, QTableWidgetItem(place))

    def sendRunAll(self):
        to_server = int(1)
        senddata = to_server.to_bytes(4, byteorder = "little")
        sent = csock.send(senddata)
        print("Sent : {}".format(to_server))


    def sendStop(self):
        to_server = int(2)
        senddata = to_server.to_bytes(4, byteorder="little")
        sent = csock.send(senddata)
        print("Sent : {}".format(to_server))

    def sendLeft(self):
        to_server = int(3)
        senddata = to_server.to_bytes(4, byteorder="little")
        sent = csock.send(senddata)
        print("Sent : {}".format(to_server))

    def sendRight(self):
        to_server = int(4)
        senddata = to_server.to_bytes(4, byteorder="little")
        sent = csock.send(senddata)
        print("Sent : {}".format(to_server))

    def sendUp(self):
        to_server = int(5)
        senddata = to_server.to_bytes(4, byteorder="little")
        sent = csock.send(senddata)
        print("Sent : {}".format(to_server))

    def sendDown(self):
        to_server = int(6)
        senddata = to_server.to_bytes(4, byteorder="little")
        sent = csock.send(senddata)
        print("Sent : {}".format(to_server))

    def showDialog(self):
        splitlist = []
        splitlist.clear()
        self.imgrefresh(splitlist)

        ChoiceFile1 = splitlist[0]
        print(ChoiceFile1)
        ChoiceFile2 = splitlist[1]
        print(ChoiceFile2)
        ChoiceFile3 = splitlist[2]
        print(ChoiceFile3)

        ChangeImage1 = cv2.imread(ChoiceFile1, cv2.IMREAD_COLOR)
        ChangeImage2 = cv2.imread(ChoiceFile2, cv2.IMREAD_COLOR)
        ChangeImage3 = cv2.imread(ChoiceFile3, cv2.IMREAD_COLOR)

        xLength = 211
        yLength = 121

        # cv2.resize(원본 이미지, 결과 이미지 크기, 보간법)
        img1 = cv2.resize(ChangeImage1, (xLength, yLength), cv2.INTER_AREA)
        img2 = cv2.resize(ChangeImage2, (xLength, yLength), cv2.INTER_AREA)
        img3 = cv2.resize(ChangeImage3, (xLength, yLength), cv2.INTER_AREA)
        tmp = './tmp'

        if not os.path.isdir(tmp):
            os.makedirs(tmp)

        cv2.imwrite("./tmp/resize1.png", img1)
        cv2.imwrite("./tmp/resize2.png", img2)
        cv2.imwrite("./tmp/resize3.png", img3)


        # cv2.imwrite("Input/ChoiceImage.png", ChangeImage)

        self.firstLbl.setPixmap(QPixmap("./tmp/resize1.png"))
        self.secondLbl.setPixmap(QPixmap("./tmp/resize2.png"))
        self.thirdLbl.setPixmap(QPixmap("./tmp/resize3.png"))
        self.firstLbl.repaint()
        self.secondLbl.repaint()
        self.thirdLbl.repaint()

    def imgrefresh(self, imglist):
        imglist.clear()
        orgdir = '\imgs'
        cwd = os.getcwd()
        fname = os.path.join(cwd + orgdir)
        imgDir = (os.listdir(fname))

        for imgdiridx in range(0, len(imgDir)):
            firstDir = os.path.join(fname, imgDir[imgdiridx])
            imglist.append(firstDir)

        for idx in range(0, len(imglist)):
            subdir = os.listdir(imglist[idx])
            for jdx in range(len(subdir)):
                subdir[jdx] = os.path.join(imglist[idx], subdir[jdx])
            subdir.sort(key=lambda f: os.stat(f).st_ctime, reverse=True)
            imglist[idx] = subdir[0]
            subdir.clear()

if __name__ == '__main__':

    print("trying to connect")
    csock = socket(AF_INET, SOCK_STREAM)
    csock.connect(SocketInfo.ADDR)

    # command = csock.recv(SocketInfo.BUFSIZE, MSG_WAITALL)
    # data = command.decode("UTF-8")
    # if (command):
    #     print("conecction success")
    #     print("type : {}, data len : {}, data : {}, Contents : {}".format(type(command), len(command), command, data))
    #
    #     to_server = int(0)
    #     senddata = to_server.to_bytes(4, byteorder='little')
    #     sent = csock.send(senddata)
    # else:
    #     print("connection failed")
    #     exit()

    app = QApplication(sys.argv)
    ex = MyApp()
    sys.exit(app.exec_())
    csock.close()
    exit()