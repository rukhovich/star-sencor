import serial
import usb
import argparse

ser = serial.Serial("COM3", 115200, timeout=3)

busses = usb.


adress = "A0"


reset = "0x01"

commands = {"reset":"0x01"}

def check_crc8(msg, crc8):
    return True

def creatr_msg(cmd, data):
    
    #I am creating string
    msg = adress + commands[cmd][2:] + len(hex(data)[2:]) + hex(data) 
    return msg

def get_msg(ser):
    
    #I got a number
    msg = hex(int(ser.read(3)))[2:]
    
    length = int(msg[4:6], base=16)
    
    data = hex(int(ser.get(length)))[2:]
    
    crc8 = ser.read(1)
    
    if 
    return "Error"
    
    
    

def send_and_get(ser, msg):

    ser.send(msg)
    ans = ser.get(3)
    ans = ans + ser.get(int(ans[3]))

    
    #I need to send a number
    return int(ans, base=16)



if __name__ = "__main__":
    send_and_get()
