# Heart Rate and Respiration Monitor Client Side
# MIT License
# Roni Bandini, August 2024

from urllib.request import urlopen
from pinpong.board import Board,Pin,NeoPixel
from pinpong.extension.unihiker import *
from unihiker import GUI
import time
import os

Board().begin()
gui = GUI()

np1 = NeoPixel(Pin((Pin.P13)),3)

url = "http://192.168.1.51"
heartRateLimit=90

os.system('clear')
print("mmWave Heart and Respiration Monitor")
print("Roni Bandini, August 2024")
print("It will take some minutes to warm up")

print("")

img = gui.draw_image(x=0, y=0, w=240, h=320, image='images/background.png')
gui.draw_text(x=120,y=220,text="Starting...", font_size=15, color="black", origin='top')

# check lights
np1.brightness(128)
np1.range_color(0,2,0xFF0000)
time.sleep(1)
np1.range_color(0,2,0x00FF00)
time.sleep(1)
np1.range_color(0,2,0x0000FF)
time.sleep(1)

np1.brightness(0)


while True:

	page = urlopen(url)
	html_bytes = page.read()
	myArray = html_bytes.decode("utf-8")
	print("Info array: "+myArray)
	myValues=myArray.split(",")
	
	img = gui.draw_image(x=0, y=0, w=240, h=320, image='images/background.png')
	gui.draw_text(x=120,y=210,text="Heart: "+myValues[0], font_size=15, color="black", origin='top')
	gui.draw_text(x=120,y=240,text="Resp : "+myValues[1], font_size=15, color="black", origin='top')


	if int(myValues[0])>heartRateLimit:
		print("Red limit!")
		np1.range_color(0,2,0xFF0000)
		np1.brightness(128)
	elif int(myValues[0])>0:
		np1.range_color(0,2,0x00FF00)
		np1.brightness(64)
	elif int(myValues[0])==0:
		print("Warming up...")
		np1.range_color(0,2,0x0000FF)
		np1.brightness(64)
		
	time.sleep(5)