import serial
import serial.tools.list_ports
import time
import matplotlib.pyplot as plt
from matplotlib import animation

def recup_port():
    ser = None
    #check value of ser.portstr
    ports= list(serial.tools.list_ports.comports())
    for p in ports:
        if "Arduino" in p.description:
            print(p.device)
            ser = serial.Serial(p.device, 115200)
    if ser != None:
        return ser
    else:
        print("No Arduino found")
        return None
    
Data = recup_port()
if Data!=None:
    for i in range(20):
        print(Data.readline())
        time.sleep(0.1)




