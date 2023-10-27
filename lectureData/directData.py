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
        if "Silicon" in p.description:
            print(p.device)
            ser = serial.Serial(p.device, 115200)
    if ser != None:
        return ser
    else:
        print("No Arduino found")
        return None
    
def animate(i):

    global X,Y,Z,liste_temps,allData
    data = allData.readline()
    data = data.strip().split()
    if(data[0].decode()=="acc:"):
        if len(data)!=0:
            x = float(data[1].decode())
            y = float(data[2].decode())
            z = float(data[3].decode())
            X.append(x)
            Z.append(z)
            Y.append(y)
            temps_mesure = time.time()
            liste_temps.append(temps_mesure)
            temps_reel = temps_mesure - liste_temps[0]
            print(temps_reel)
            print(x,y,z)
            ax1.clear()
            ax1.plot(liste_temps,X)
            ax1.plot(liste_temps,Y)
            ax1.plot(liste_temps,Z)
            ax1.set_xlabel("Temps (s)")
            ax1.set_ylabel("Accélération (m/s²)")
            ax1.set_title("Accélération en fonction du temps")
            ax1.legend(["X","Y","Z"])
            ax1.grid()


temps_reel = 0
temps_aquisition = 100
X = []
Y = []
Z = []
liste_temps = []
allData = recup_port()
fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)
ani = animation.FuncAnimation(fig, animate, interval=100)
plt.show()
    

# if allData!=None:
#     while temps_reel < temps_aquisition:
#         data = allData.readline()
#         data = data.strip().split()
#         print(data)
#         if len(data)!=0:
#             x = float(data[1].decode())
#             y = float(data[2].decode())
#             z = float(data[3].decode())
#             X.append(x)
#             Y.append(y)
#             Z.append(z)
#             temps_mesure = time.time()
#             liste_temps.append(temps_mesure)
#             temps_reel = temps_mesure - liste_temps[0]
#             print(temps_reel)
#             print(x,y,z)





