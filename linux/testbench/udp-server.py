import socket
import sys
import time 
import struct
if __name__ == '__main__':
	
	UDP_IP ="localhost"
	try:
		UDP_PORT = int (sys.argv[1])
	except:
		print("user port don't number")	
	else:
		print("User port",sys.argv[1]) 
		sock =socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		mount_message = 0
		while 1:
			mount_message +=1
			MESSAGE = "HELLO_DVB_T2_MI" +str(mount_message)
			sock.sendto(MESSAGE.encode(),(UDP_IP,UDP_PORT))

			#time.sleep(1)
			print (mount_message,MESSAGE) 

