#pragma once

#include <bitset>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

typedef bitset<8> Byte;
typedef bitset <64> Bytes8;

class Bytes
{
private:
	vector<Byte> bytes;
	int currentBit;
	int currentByte;
	int size;
public:
	Bytes();
	~Bytes();
	void push_bytes(string _bytes, size_t _size);
	void push_bytes(char * _bytes, size_t _size);
	void push_bits(Byte _byte, size_t _size);
	void push_bytes(Bytes8 _bytes, size_t _size = 64);
	Byte getBits(size_t _bitBeginIndex, size_t _byteIndex, size_t _bitNumber = 8);
	Bytes8 getBytes8(size_t _byteBeginIndex, size_t _byteNumber, size_t _bitBeginIndex = 0);
	vector<Byte> getBytes(size_t _byteBeginIndex, size_t _byteNumber, size_t _bitBeginIndex = 0);
	string toString();
	size_t getSize();
};

