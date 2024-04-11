import serial.tools.list_ports

ports = serial.tools.list_ports.comports()
serialInst = serial.Serial()
portList =[]

for oneport in ports:
    portList.append(str(oneport))
    print(str(oneport))

val = input('select Port: COM')

for x in range(0,len(portList)):
    if portList[x].startswith("COM" + str(val)):
        portVar = 'COM' + str(val)
        print(portList[x])

serialInst.baudrate = 9600
serialInst.port = portVar
serialInst.open()

while True: 
    if serialInst.in_waiting:
        packet = serialInst.readline()
        print(packet.decode('utf'))
