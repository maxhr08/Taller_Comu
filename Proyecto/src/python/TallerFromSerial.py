import serial
import time

rawdata=[]

def clean(L): #Funcion para limpiar el mensaje desde a conexion serial de arduino
    newL=[]
    for i in range(len(L)):
        temp=L[i][2:]
        newL.append(temp[:-5])
        return newL

    
def write(L): #Funcion para escribir el mensaje recibido en un txt 
    RecievedMessage=open("txt/PytoMat.txt",mode='w')
    for i in range(len(L)):
        RecievedMessage.write(L[i]+'\n')
    RecievedMessage.close()

def main():
    print("Hi")
    
    ser=serial.Serial('COM3',9600,timeout=None) #Se abre la conexion serial 
    time.sleep(1)

    #for i in range(4): 
    rawdata.append(str(ser.readline())) #Se guardan los datos en un array

    data=clean(rawdata) #Se limpia
    write(data)         #Se escribe en el txt
    ser.close()         #Se cierra la conexion serial 
    
main()






