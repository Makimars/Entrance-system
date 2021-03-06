import os
import sys
import time


def _append_run_path():
    if getattr(sys, 'frozen', False):
        pathlist = []

        # If the application is run as a bundle, the pyInstaller bootloader
        # extends the sys module by a flag frozen=True and sets the app
        # path into variable _MEIPASS'.
        pathlist.append(sys._MEIPASS)

        # the application exe path
        _main_app_path = os.path.dirname(sys.executable)
        pathlist.append(_main_app_path)

        # append to system path enviroment
        os.environ["PATH"] += os.pathsep + os.pathsep.join(pathlist)


_append_run_path()

import serial.tools.list_ports
from PyQt5 import QtWidgets, QtCore
from serial import Serial

import mainwindow
from ReadThread import ReadThread

simple_mode = True
numbers_list = []
language = "Czech"


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.ui = mainwindow.Ui_MainWindow()
        self.ui.setupUi(self)
        self.ui.open_file_button.clicked.connect(self.openFileButtonClicked)
        self.ui.load_file_button.clicked.connect(self.loadFileButtonClicked)
        self.ui.erase_device_button.clicked.connect(self.eraseMemoryButtonClicked)
        self.ui.add_number_button.clicked.connect(self.addNumberButtonClicked)
        self.ui.text_output.textChanged.connect(self.autoScrolling)
        self.ui.line_text_input.returnPressed.connect(self.commandEntered)
        self.ui.connect_device_button.clicked.connect(self.connectButtonClicked)
        self.ui.read_memory_button.clicked.connect(self.readMemoryButtonClicked)
        self.ui.get_number_count_button.clicked.connect(self.getNumberCount)
        self.ser = Serial(baudrate=115200, timeout=1)

        if simple_mode:
            self.ui.line_text_input.hide()

    def openFileButtonClicked(self):
        print("open clicked")
        fileName = QtWidgets.QFileDialog().getOpenFileName(self, "Open file", "", "Csv files(*.csv )")
        if os.path.exists(fileName[0]):
            print("file exist" + fileName[0])
            file_stream = open(fileName[0], "r")
            content = file_stream.read()
            lines = content.split('\n')
            for line in lines:
                print(line)
                if len(line.split(';')) > 2:
                    number = line.split(';')[2]
                    if len(number) == 9:
                        numbers_list.append(number)
            self.printMessage(QtCore.QCoreApplication.translate("messages", "File loaded"))

    def loadFileButtonClicked(self):
        if self.ser.is_open:
            if len(numbers_list) > 0:
                self.read_thread.terminate()
                for number in numbers_list:
                    self.sendToDevice("a" + number)
                    print(number + " added")

                    response = ""
                    while True:
                        line = self.ser.read_all().decode("ascii")
                        response += line
                        if response.strip() == "ok":
                            break
                        time.sleep(0.03)

                self.printMessage(QtCore.QCoreApplication.translate("messages", "File uploaded"))
                self.read_thread.start()
            else:
                self.printMessage(QtCore.QCoreApplication.translate("messages", "File not selected"))
        else:
            self.printMessage(QtCore.QCoreApplication.translate("messages", "Device not connected"))

    def eraseMemoryButtonClicked(self):
        self.sendToDevice("e")

    def addNumberButtonClicked(self):
        self.sendToDevice("a" + self.ui.number_line_input.text())
        self.ui.number_line_input.clear()

    def autoScrolling(self):
        self.ui.text_output.verticalScrollBar().setValue(self.ui.text_output.verticalScrollBar().maximum())

    def commandEntered(self):
        command = self.ui.line_text_input.text()
        if command == "clear":
            self.ui.text_output.clear()
        elif command == "connect":
            self.connectButtonClicked()
        else:
            self.sendToDevice(command)
        self.ui.line_text_input.setText("")

    def connectButtonClicked(self):
        port_address = ""
        if sys.platform == "linux":
            for i in range(0, 5, 1):
                port_address = "/dev/ttyUSB" + str(i)
                if os.path.exists(port_address) and (not self.ser.is_open):
                    self.tryConnectDevice(port_address)

        elif sys.platform == "win32":
            ports = serial.tools.list_ports.comports()

            for port, desc, hwid in sorted(ports):
                if not self.ser.is_open:
                    print(port)
                    self.tryConnectDevice(port)

        if self.ser.is_open:
            self.printMessage(QtCore.QCoreApplication.translate("messages", "Connected"))
            self.read_thread.start()
        else:
            self.printMessage(QtCore.QCoreApplication.translate("messages", "Connection failed"))

    def getNumberCount(self):
        self.printMessage(QtCore.QCoreApplication.translate("messages", "Messages count:"))
        self.sendToDevice("c")

    def tryConnectDevice(self, address: str):
        print(address)
        self.ser.port = address
        self.ser.open()
        if self.ser.isOpen():
            self.ser.read_until()
            input = self.ser.read(4).decode("ascii")
            if input == "init":
                self.read_thread = ReadThread(self.ser)
                self.read_thread.sig.connect(self.printInput)
                return
        print("closed")
        self.ser.close()

    def readMemoryButtonClicked(self):
        self.printMessage("-----------")
        self.sendToDevice("r")

    def sendToDevice(self, command: str):

        self.printOutput(command)

        if self.ser.isOpen():
            self.ser.write(command.encode("ascii"))
        else:
            self.printMessage(QtCore.QCoreApplication.translate("messages", "Device not available"))

    def readDevice(self):
        input = self.ser.read_until()

        while True:
            if len(input) > 0:
                print(input.decode("ascii"))
                self.printInput(input.decode("ascii"))
            input = self.ser.read_until()

    def printInput(self, message: str):
        if not simple_mode:
            self.ui.text_output.insertHtml("Device: " + message + "<br>")
        else:
            self.ui.text_output.insertHtml(message + "<br>")

    def printOutput(self, message: str):
        if not simple_mode:
            self.ui.text_output.insertHtml("PC: " + message + "<br>")

    def printMessage(self, message: str):
        self.ui.text_output.insertHtml(message + "<br>")
        self.showMessage(message)

    def showMessage(self, message: str):
        if False:
            msg = QtWidgets.QMessageBox()
            msg.setText(message)
            msg.exec()


app = QtWidgets.QApplication(sys.argv)

app_translator = QtCore.QTranslator()
if language != "English":
    app_translator.load(language + ".qm")
app.installTranslator(app_translator)

my_mainWindow = MainWindow()
my_mainWindow.show()

sys.exit(app.exec_())
