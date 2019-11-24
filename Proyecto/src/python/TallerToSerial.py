import serial
import time

transmissions=1
EncodedMessage=open("txt/enmessage.txt","r") #Se abre el mensaje codificado
en=EncodedMessage.read()
FirstByte=int(("0b"+en[8:15]),2) 
FirstByteChar=FirstByte.to_bytes((FirstByte.bit_length() + 7) // 8, 'big').decode()#Se codifica para enviarse como un caracter al arduino
SecondByte=int(("0b0"+en[0:7]),2)
SecondByteChar=SecondByte.to_bytes((SecondByte.bit_length() + 7) // 8, 'big').decode() #Se envian dos bytes porque el arduino lee desde el buffer por byte

print(FirstByteChar)
print(SecondByteChar)

def main():
    
    print("hi")
    ser1=serial.Serial('COM3',9600) #Se abre el puerto de comunicacion
    time.sleep(1)
    for k in range(transmissions):       
        ser1.write(FirstByteChar.encode())
        time.sleep(1)
        ser1.write(SecondByteChar.encode())
        print("sent")
    ser1.close()

main()
EncodedMessage.close()




