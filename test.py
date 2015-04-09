import serial
import time

port = serial.Serial("/dev/ttyUSB0", baudrate = 9600, timeout = 3.0)

def sendAT(cmd):
	port.write(cmd + "\r")

while True:
	sendAT("AT")
	rcv = port.readline()
	rcv = port.readline()
	port.write(rcv.rstrip())
	time.sleep(1)