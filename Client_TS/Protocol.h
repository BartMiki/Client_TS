#pragma once

#include "Bytes.h"
#include "PacketHeader.h"
#include <WinSock2.h>

#pragma comment(lib,"ws2_32.lib") 

class Protocol
{
private:
	Bytes packet;
	Byte packetHeader;
	Bytes8 dataSize;
	string data;
	Bytes8 id;
public:
	//Use it to create protocol to send
	Protocol(PacketHeader _packetHeader, u_int64 _dataSize = 0, string _data = "", u_int64 _id = 0);
	//Use it to create protocol from received data
	Protocol(char * received, int _size);
	//Get string to sent
	char *  getStringToSend(int & messageLenght);
	//Get header from received data
	PacketHeader getPacketHeader();
	//Get size from received data;
	u_int64 getDataSize();
	//Get data from received data;
	string getData();
	~Protocol();
};

