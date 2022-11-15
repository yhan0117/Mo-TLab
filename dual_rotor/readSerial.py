import serial
import numpy as np
import csv
import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial.tools.list_ports as ports

def Port():
    portsList = []

    for onePort in ports.comports():
        portsList.append(str(onePort))
        print(str(onePort))

    val = input("Select Port: COM")

    for x in range(0, len(portsList)):
        if portsList[x].startswith("COM" + str(val)):
            portvar = "COM" + str(val)  # open serial port
            print("Port selected: " + portvar)         # check which port was really used


    return portvar

def readSensor(serialInst):
    read = False

    while read == False:
        if serialInst.in_waiting:
            buffer = serialInst.readline()
            val = buffer.decode('utf').rstrip("\n")

            read = True
    
    return float(val)


def animate(i, xs, ys, serialInst):

    # Read temperature (Celsius) from TMP102
    temp_c = readSensor(serialInst)
    # Add x and y to lists
    xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
    ys.append(temp_c)

    # Limit x and y lists to 20 items
    xs = xs[-20:]
    ys = ys[-20:]

    # Draw x and y lists
    ax.clear()
    ax.plot(xs, ys)


if __name__ == "__main__":

    serialInst = serial.Serial() 
    serialInst.port = "COM21"
    serialInst.baudrate = 9600
    serialInst.open()
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    xs = []
    ys = []
    ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys, serialInst), interval=1000)
    plt.show()
    

            
