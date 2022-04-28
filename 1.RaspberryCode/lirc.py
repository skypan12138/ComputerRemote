#!/usr/bin/env python
import RPi.GPIO as GPIO
import time
import serial

#For different GPIO's buttons, if button are not needed,this col con actually be removed
BtnPin_Shotdown = 19 
BtnPin_WeChat=17 
BtnPin_OneNote=13
BtnPin_Bilibili=27
BtnPin_Leetcode=26
BtnPin_Music=24
#Infrared remote control
InfraredPin = 18; 

#Set GPIO mode
def setup():
    GPIO.setwarnings(False)
    GPIO.setmode(GPIO.BCM)       # Numbers GPIOs by physical location
    GPIO.setup(BtnPin_Shotdown, GPIO.IN, pull_up_down=GPIO.PUD_UP)    # Set BtnPin_Shotdown's mode is input, and pull up to high level(3.3V) 
    GPIO.setup(BtnPin_WeChat, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) 
    GPIO.setup(BtnPin_OneNote, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) 
    GPIO.setup(BtnPin_Bilibili, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) 
    GPIO.setup(BtnPin_Leetcode, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) 
    GPIO.setup(BtnPin_Music, GPIO.IN, pull_up_down=GPIO.PUD_DOWN) 

    #================Infrared remote control================
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(InfraredPin,GPIO.IN,GPIO.PUD_UP)

if __name__ == '__main__':     # Program start from here
    setup()
    ser = serial.Serial("/dev/ttyAMA0", 9600)
    ser.flushInput()  
    ser.write("begin".encode("utf-8"))  
    try:
        while True:

            #================================Decode infrared signal============================
            if GPIO.input(InfraredPin) == 0:
                N=0
                EX0=0
                count = 0 
                print('step1 ...')
                while GPIO.input(InfraredPin) == 0 and count < 200:  #9ms
                    count += 1
                    time.sleep(0.00006)

                # count = 0 
                # while GPIO.input(InfraredPin) == 1 and count < 80:  #4.5ms
                #     count += 1
                #     time.sleep(0.00006)
                print('step2 ...')
                idx = 0 
                cnt = 0 
                data = [0,0,0,0,0,0,0,0]
                for j in range(0,4):
                    for k in range(0,8):

                        count = 0 
                        while GPIO.input(InfraredPin)==1 and count < 80 :    #4.5ms
                            count += 1
                            time.sleep(0.00006)
                        # print('InfraredPin turn to low ...')

                        while GPIO.input(InfraredPin)==0 :   
                            count += 1
                            time.sleep(0.00006)

                        # print('InfraredPin turn to high ...')
                        while GPIO.input(InfraredPin)==1 and N<=31:
                            time.sleep(0.00006)
                            N=N+1

                        data[j]=data[j]>>1
                        if(N>=8):
                            data[j]=data[j]|0x80
                        N=0

                if EX0==0:
                    data[5]=data[2]&0x0f
                    data[6]=data[2]>>4

                    if(data[5]>9):
                        data[5]=data[5]+0x37
                    else:
                        data[5]=data[5]+0x30
                
                    if(data[6]>9):
                        data[6]=data[6]+0x37
                    else:
                        data[6]=data[6]+0x30
        
                    if data[0]+data[1] == 0xFF and data[2]+data[3] == 0xFF:  #check
                        print("Get the code as: 0x%02x" %data[2])      

            #================================Corresponding signal============================
            count = ser.inWaiting()  
            if GPIO.input(BtnPin_Shotdown) == True:
                time.sleep(0.01)
                if GPIO.input(BtnPin_Shotdown)!=False:
                    print("send massage:S")
                    ser.write("S")  
                    time.sleep(0.3)
            elif GPIO.input(BtnPin_WeChat) == True or data[2]==0x0c:
                time.sleep(0.01)
                if GPIO.input(BtnPin_WeChat)!=False:
                    print("send massage:W")
                    ser.write("W")  
                    time.sleep(0.3)
            elif GPIO.input(BtnPin_OneNote) == True or data[2]==0x18:
                time.sleep(0.01)
                if GPIO.input(BtnPin_OneNote)!=False:
                    print("send massage:O")
                    ser.write("O")  
                    time.sleep(0.3)
            elif GPIO.input(BtnPin_Bilibili) == True or data[2]==0x5e:
                time.sleep(0.01)
                if GPIO.input(BtnPin_Bilibili)!=False:
                    print("send massage:B")
                    ser.write("B")  
                    time.sleep(0.3)
            elif GPIO.input(BtnPin_Leetcode) == True or data[2]==0x08:
                time.sleep(0.01)
                if GPIO.input(BtnPin_Leetcode)!=False:
                    print("send massage:L")
                    ser.write("L")  
                    time.sleep(0.3)  
            elif GPIO.input(BtnPin_Music) == True or data[2]==0x1c:
                time.sleep(0.01)
                if GPIO.input(BtnPin_Music)!=False:
                    print("send massage:M")
                    ser.write("M")  
                    time.sleep(0.3)                         
                                        
    except KeyboardInterrupt:  # When 'Ctrl+C' is pressed, the child program destroy() will be  executed.
        GPIO.cleanup()


