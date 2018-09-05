import network
from network import LoRa
from machine import UART
import socket
import machine
import time
import pycom

#Here we disable the startup heartbeat
pycom.heartbeat(False)

#Here we either disable the WiFi in AP mode if necessary, and set it to station mode. (Slave)
wlan = network.WLAN(mode=network.WLAN.STA)

#Configure IP addresses for slaves (Slave)
wlan.ifconfig(config=('192.168.4.3','255.255.255.0','192.168.0.1','192.168.0.1'))


#We then set it to connect to an AP (Slave)
wlan.scan()
wlan.connect('lopy4-wlan-54cc', auth=(network.WLAN.WPA2, 'www.pycom.io'))
while not wlan.isconnected():
    time.sleep_ms(50)

#Here we create a socket for WiFi communication between LoPys
UDP_IP = "192.168.4.3"
UDP_PORT = 9000
sck = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
sck.bind((UDP_IP, UDP_PORT))
sck.setblocking(0)
sck.settimeout(0.01)
pycom.rgbled(0x007f00) # green

#Here we initialise the UART
uart1 = UART(1, baudrate=115200, timeout_chars=2, pins=('P3','P4'))
uart2 = UART(2, baudrate=115200, timeout_chars=2, pins=('P10','P9'))

#######LORA TRANSMISSIONS##########
buffer_size = 256
def lora_rcv_UART(lora_sck):
    pycom.rgbled(0x00ff00) #greenish
    messagercv = lora_sck.recv(buffer_size)
    uart1.write(messagercv)
    pycom.rgbled(0x0000ff)  #blue

# Initialize LoRa in LORA mode.
lora = LoRa(mode=LoRa.LORA, sf=7, region=LoRa.EU868)

lora_sck = socket.socket(socket.AF_LORA, socket.SOCK_RAW)
lora_sck.setblocking(False)
lora_sck.settimeout(0.01)

# specify callback function
lora.callback(LoRa.RX_PACKET_EVENT, handler=lora_rcv_UART, arg=lora_sck)



#In this code when we receive something from LoRa we pass it on to a temporary buffer and send it to the UART. Likewise when we receive something from the UART we pass it to a temporary buffer and then send it through LoRa
while True:

    if (uart1.any() > 0):
        messagesend = uart1.read(buffer_size)
        lora_sck.send(messagesend) # send
        pycom.rgbled(0xffff00) #yellow

    if (uart2.any() > 0):
        wifimessagesend = uart2.read()
        sck.sendto(wifimessagesend,('255.255.255.255', 9000))
        pycom.rgbled(0xff0000) # red

    try:
        wifimessagereceive, address = sck.recvfrom(buffer_size)
        print(wifimessagereceive.decode('utf-8'))

        if (len(wifimessagereceive) > 0):
            uart2.write(wifimessagereceive)
            wifimessagereceive = None
            pycom.rgbled(0xee82ee)  #violet
    except socket.timeout as e:
        store = e   #redundant
