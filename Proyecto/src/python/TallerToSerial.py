import serial
import time

transmissions=1

def Ordenar(en):    
    FirstByte=int(("0b"+en[8:15]),2) 
    FirstByteChar=FirstByte.to_bytes((FirstByte.bit_length() + 7) // 8, 'big').decode()#Se codifica para enviarse como un caracter al arduino
    SecondByte=int(("0b0"+en[0:7]),2)
    SecondByteChar=SecondByte.to_bytes((SecondByte.bit_length() + 7) // 8, 'big').decode() #Se envian dos bytes porque el arduino lee desde el buffer por byte
    print(FirstByteChar)
    print(SecondByteChar)
    return FirstByteChar,SecondByteChar

def main():
    
    print("hi")
    ser1=serial.Serial('COM8',9600) #Se abre el puerto de comunicacion
    time.sleep(2)
    EncodedMessage=open("../Matlab/TX/mensaje_codificado.txt","r") #Se abre el mensaje codificado
    en=EncodedMessage.read().splitlines()
    print(en)

    
    for i in range(len(en)):

        #binary=format(int(en[i],2),'018b')
        time.sleep(0.5)
        #[FirstByteChar, SecondByteChar]=Ordenar(en[k])
        #FirstBytechar=en[0:7]
        #SecondByteChar=en[8:15]
        #time.sleep(0.4)
        #print(binary)
        ser1.write(en[i])
        
        #time.sleep(0.6)
        #ser1.write(FirstByteChar.encode())
        #time.sleep(0.5)
        print("sent")

    parada=b'\r'
    ser1.write(parada)    
    ser1.close()
    EncodedMessage.close()
    
main()
