import serial

serdev = '/dev/ttyUSB0'
s = serial.Serial(serdev, 9600)

read=False

while 1:
    line = s.readline()
    print(line.decode())
    if read :
        f.write(line.decode())
    if line.decode()=="start\r\n":
        f=open("log.txt","w")
        read=True
    if line.decode()=="end\r\n":
        f.close()
        break

    