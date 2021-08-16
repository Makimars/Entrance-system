import os
import sys

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


from PyQt5 import QtWidgets, QtCore
from MainWindow import MainWindow

language = "Czech"

app = QtWidgets.QApplication(sys.argv)

app_translator = QtCore.QTranslator()
if language != "English":
    app_translator.load(language + ".qm")
app.installTranslator(app_translator)

my_mainWindow = MainWindow()
my_mainWindow.show()

sys.exit(app.exec_())
