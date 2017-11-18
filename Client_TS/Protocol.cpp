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
	dataSize = packet.getByte8(0, 8, 7); // take 8 bytes from 7 bit of 0 byte
	if (dataSize.to_ullong() > 0)
	{
		data = getData();
	}
	else
	{
		data = "";
	}
	id = packet.getByte8(_size - 9, 8, 7);
}

char * Protocol::getMessageToSend()
{
	vector<Byte> temp = packet.getBytes();
	int messageLenght = temp.size();
	char * toSend = new char[messageLenght];
	for (int i = 0; i < messageLenght; i++)
	{
		toSend[i] = temp[i].to_ullong();
	}
	return toSend;
}

int Protocol::getMessageSize()
{
	return packet.getSize();
}

PacketHeader Protocol::getPacketHeader()
{
	return static_cast<PacketHeader>(packetHeader.to_ulong());
}

u_int64 Protocol::getDataSize()
{
	return dataSize.to_ullong();
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

u_int64 Protocol::getID()
{
	return id.to_ullong();
}

Protocol::~Protocol()
{

}
