from PyQt5.QtCore import QThread, pyqtSignal
import serial


class ReadThread(QThread):
    sig = pyqtSignal(str)
    
    def __init__(self, serial: serial):
        QThread.__init__(self)
        self.ser = serial
    
    def __del__(self):
        self.wait()
    
    def run(self):
        input = self.ser.read_until()
        
        while True:
            if self.ser.is_open:
                input = self.ser.read_until().decode("ascii")
                if(len(input) > 0):
                    self.sig.emit(input)
