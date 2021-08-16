# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui_mainwindow.ui'
#
# Created by: PyQt5 UI code generator 5.14.1
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(698, 444)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.centralwidget)
        self.verticalLayout.setObjectName("verticalLayout")
        self.horizontalLayout = QtWidgets.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.open_file_button = QtWidgets.QPushButton(self.centralwidget)
        self.open_file_button.setObjectName("open_file_button")
        self.horizontalLayout.addWidget(self.open_file_button)
        self.load_file_button = QtWidgets.QPushButton(self.centralwidget)
        self.load_file_button.setObjectName("load_file_button")
        self.horizontalLayout.addWidget(self.load_file_button)
        self.erase_device_button = QtWidgets.QPushButton(self.centralwidget)
        self.erase_device_button.setObjectName("erase_device_button")
        self.horizontalLayout.addWidget(self.erase_device_button)
        self.read_memory_button = QtWidgets.QPushButton(self.centralwidget)
        self.read_memory_button.setObjectName("read_memory_button")
        self.horizontalLayout.addWidget(self.read_memory_button)
        self.get_number_count_button = QtWidgets.QPushButton(self.centralwidget)
        self.get_number_count_button.setObjectName("read_memory_button_2")
        self.horizontalLayout.addWidget(self.get_number_count_button)
        self.verticalLayout.addLayout(self.horizontalLayout)
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.number_line_input = QtWidgets.QLineEdit(self.centralwidget)
        self.number_line_input.setObjectName("number_line_input")
        self.horizontalLayout_2.addWidget(self.number_line_input)
        self.add_number_button = QtWidgets.QPushButton(self.centralwidget)
        self.add_number_button.setObjectName("add_number_button")
        self.horizontalLayout_2.addWidget(self.add_number_button)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.connect_device_button = QtWidgets.QPushButton(self.centralwidget)
        self.connect_device_button.setObjectName("connect_device_button")
        self.verticalLayout.addWidget(self.connect_device_button)
        self.text_output = QtWidgets.QTextBrowser(self.centralwidget)
        self.text_output.setObjectName("text_output")
        self.verticalLayout.addWidget(self.text_output)
        self.line_text_input = QtWidgets.QLineEdit(self.centralwidget)
        self.line_text_input.setObjectName("line_text_input")
        self.verticalLayout.addWidget(self.line_text_input)
        MainWindow.setCentralWidget(self.centralwidget)
        self.actionQuit = QtWidgets.QAction(MainWindow)
        self.actionQuit.setObjectName("actionQuit")

        self.retranslateUi(MainWindow)
        self.actionQuit.triggered.connect(MainWindow.close)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "Entrance system"))
        self.open_file_button.setText(_translate("MainWindow", "Open File"))
        self.load_file_button.setText(_translate("MainWindow", "Upload File"))
        self.erase_device_button.setText(_translate("MainWindow", "Clear memory"))
        self.read_memory_button.setText(_translate("MainWindow", "Read memory"))
        self.get_number_count_button.setText(_translate("MainWindow", "Get number count"))
        self.add_number_button.setText(_translate("MainWindow", "Add Number"))
        self.connect_device_button.setText(_translate("MainWindow", "Connect"))
        self.actionQuit.setText(_translate("MainWindow", "&Quit"))
