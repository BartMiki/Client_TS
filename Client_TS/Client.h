#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "stdafx.h"
#include "Protocol.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>

#pragma comment(lib,"ws2_32.lib") 

class Client
{
public:
	// --- methods ---
	Client(std::string IP, int PORT);
	bool Connect();
	bool CloseConnection();
	u_int64 getID();
	// --- method send---
	bool sendProtocol(Protocol * protocol);
	bool connectedToAnotherClient;
	bool havePendingInvite;
	bool sendedInvite;
	bool connectedToServer;
	SOCKET connection;
private:
	// --- data fields ---
	WSAData wsaData;
	WORD DllVersion;
	SOCKADDR_IN addr; //Addres to bind connection socket to
	int sizeOfAddr;
	u_int64 id;
	// --- methods ---
	Protocol * recvProtocol();
	bool processProtocol(Protocol * protocol);
	static void ClientThread();
};

static Client * clientPtr;

