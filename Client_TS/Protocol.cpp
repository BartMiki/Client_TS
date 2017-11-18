#include "stdafx.h"
#include "Protocol.h"


Protocol::Protocol(PacketHeader _packetHeader, u_int64 _dataSize, string _data, u_int64 _id)
{
	packetHeader = (char)_packetHeader;
	dataSize = _dataSize;
	data = _data;
	id = _id;
	packet.push_bits(packetHeader, 7);
	packet.push_bytes(dataSize);
	if (_dataSize > 0)
	{
		packet.push_bytes(data, data.size());
	}
	packet.push_bytes(id);
}

Protocol::Protocol(char * received, int _size)
{
	packet.push_bytes(received, _size);
	packetHeader = packet.getBits(0, 0, 7);
	dataSize = packet.getBytes8(0, 8, 7); // take 8 bytes from 7 bit of 0 byte
	if (dataSize.to_ullong() > 0)
	{
		data = getData();
	}
	else
	{
		data = "";
	}
}

char * Protocol::getStringToSend(int & messageLenght)
{
	//dataSize + headerSize(1) + dataSizeSize(8) 
	messageLenght = dataSize.to_ullong() + 1 + 8;
	vector<Byte> temp = packet.getBytes(0, messageLenght);
	char * toSend = new char[messageLenght];
	for (int i = 0; i < messageLenght; i++)
	{
		toSend[i] = temp[i].to_ullong();
	}
	return toSend;
}

PacketHeader Protocol::getPacketHeader()
{
	Byte _byte = packet.getBits(0, 0, 7);
	PacketHeader result = static_cast<PacketHeader>(static_cast<char>(_byte.to_ullong()));
	return result;
}

u_int64 Protocol::getDataSize()
{
	Bytes8 result = packet.getBytes8(0, 8, 7);
	return result.to_ullong();
}

string Protocol::getData()
{
	string result = "";
	vector<Byte> temp = packet.getBytes(8, dataSize.to_ullong(), 7);
	for (auto a : temp)
	{
		result += static_cast<char>(a.to_ullong());
	}
	return result;
}

Protocol::~Protocol()
{

}
