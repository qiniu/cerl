#! /usr/bin/env ptyhon

from socket import *
from struct import *

HOST = '127.0.0.1'
PORT = 8633
bufSize = 1024

ADDR = (HOST, PORT)

tcpCliSock = socket(AF_INET, SOCK_STREAM)
tcpCliSock.connect(ADDR)
print "Connected!\n"


data = pack('llll', 1,2,3,1) + pack('7h', 12288, 1,0,12288, 2,0,0)
print len(data)

tcpCliSock.send(data)
print "Request Sent\n"
result = tcpCliSock.recv(bufSize)


print result

tcpCliSock.close()
