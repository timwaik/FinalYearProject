from network import WLAN
from network import LoRa
from machine import UART
import socket
import machine
import time
import pycom
import os

#Here we disable the startup heartbeat
pycom.heartbeat(False)


#Here we create a socket for WiFi communication between LoPys
time.sleep(2)
UDP_IP = "192.168.4.1"
UDP_PORT = 9000
sck1 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sck1.bind((UDP_IP, UDP_PORT))
sck1.setblocking(0)
sck1.settimeout(0.01)
time.sleep(1)

#When all sockets connected, light up green
pycom.rgbled(0x007f00) # green


####LORA TRANSMISSION PART####
buffer_size = 256

def lora_rx_cb(s):
    pycom.rgbled(0x00ff00) #greenish
    temp = s.recv(buffer_size)
    uart1.write(temp)
    pycom.rgbled(0x0000ff)  #blue

#######

# this uses the UART_1 non-default pins for TXD and RXD
uart1 = UART(1, baudrate=115200, timeout_chars=2, pins=('P3','P4'))
uart2 = UART(2, baudrate=115200, timeout_chars=2, pins=('P10','P9'))

# Initialize LoRa in LORA mode.
lora = LoRa(mode=LoRa.LORA, region=LoRa.EU868)

s = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
s.setblocking(False)
s.settimeout(0.01)

# specify callback function
lora.callback(LoRa.RX_PACKET_EVENT, handler=lora_rx_cb, arg=s)

#######

while True:

    if (uart1.any() > 0):
        temp = uart1.read(buffer_size)
        s.send(temp)
        pycom.rgbled(0xffff00) #yellow

    if (uart2.any() > 0):
        wifimessagesend = uart2.read(buffer_size)
        sck1.sendto(wifimessagesend,('255.255.255.255', 9000))
        pycom.rgbled(0xff0000)       #red


    try:
        wifimessagereceive, address = sck1.recvfrom(buffer_size)
        print(wifimessagereceive.decode('utf-8'))

        if (len(wifimessagereceive) > 0):
            uart2.write(wifimessagereceive)
            wifimessagereceive = None
            pycom.rgbled(0xee82ee)  #purple
    except socket.timeout as e:
        store = e     #redundant
