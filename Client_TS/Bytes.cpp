#include "stdafx.h"
#include "Bytes.h"


Bytes::Bytes()
{
	currentBit = 7;
	currentByte = 0;
	size = 0;
}

Bytes::~Bytes() {}

void Bytes::push_bytes(string _bytes, size_t _size)
{
	for (int i = 0; i < _size; i++) {
		Byte new8bits = static_cast<Byte>(_bytes[i]);
		push_bits(new8bits, 8);
	}
}

void Bytes::push_bytes(char * _bytes, size_t _size)
{
	for (int i = 0; i < _size; i++) {
		Byte new8bits = static_cast<Byte>(_bytes[i]);
		push_bits(new8bits, 8);
	}
}

void Bytes::push_bits(Byte _byte, size_t _size)
{
	int test = currentBit;
	if (_size > test || size == 0)
	{
		bytes.push_back(0);
		size++;
	}
	for (int i = 0, j = 7; i < _size && j >= 0; i++, j--) {
		bytes[currentByte][currentBit] = _byte[j];
		currentBit--;
		if (currentBit < 0)
		{
			currentBit = 7;
			currentByte++;
		}
	}
}

void Bytes::push_bytes(Byte8 _bytes, size_t _sizeInBits)
{
	int byteSize = 8;
	for (int i = 7; i >= 0; i--)
	{
		Byte new8bits;
		for (int j = 7 + i*byteSize, k = 7; j >= 0 + i*byteSize; j--, k--) {
			new8bits[k] = _bytes[j];
		}
		push_bits(new8bits, 8);
	}
}

Byte Bytes::getBits(size_t _bitBeginIndex, size_t _byteIndex, size_t _bitNumber)
{
	Byte result;
	int currentGetBit = 7 - _bitBeginIndex;
	for (int i = 0, j = 7; i < _bitNumber && j >= 0; i++, j--)
	{
		result[j] = bytes[_byteIndex][currentGetBit];
		currentGetBit--;
		if (currentGetBit < 0)
		{
			currentGetBit = 7;
			_byteIndex++;
		}
	}
	return result;
}

Byte8 Bytes::getByte8(size_t _byteBeginIndex, size_t _numberOfBytes, size_t _bitBeginIndex)
{
	Byte8 result;
	Byte new8bits;
	int byteSize = 8;
	for (int i = 0; i < 8; i++)
	{
		new8bits = getBits(_bitBeginIndex, _byteBeginIndex + _numberOfBytes - 1 - i);
		for (int j = 7 + i*byteSize, k = 7; j >= 0 + i*byteSize; j--, k--) {
			result[j] = new8bits[k];
		}
	}
	return result;
}

vector<Byte> Bytes::getBytes(size_t _byteBeginIndex, size_t _numberOfBytes, size_t _bitBeginIndex)
{
	vector<Byte> result;
	Byte byte;
	if (_numberOfBytes<0 || _numberOfBytes > size)
	{
		_numberOfBytes = size;
	}
	for (int i = 0; i < _numberOfBytes; i++)
	{
		byte = getBits(_bitBeginIndex, _byteBeginIndex + i);
		result.push_back(byte);
	}
	return result;
}

string Bytes::toString()
{
	string result = "", byteString;
	for (auto a : bytes) {
		byteString = a.to_string();
		result += byteString + " ";
	}
	return result;
}

size_t Bytes::getSize()
{
	return size;
}
