#include "stdafx.h"
#include "Client.h"

Client::Client(std::string IP, int PORT)
{
	//WinSock2 startup
	DllVersion = MAKEWORD(2, 2);

	//if WSAStartup return anything other than 0, that means an error has occured
	if (WSAStartup(DllVersion, &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}
	// --- Address setup ---
	sizeOfAddr = sizeof(addr);
	inet_pton(AF_INET, IP.c_str(), &(addr.sin_addr)); //Addres = local host (this PC)
	addr.sin_port = htons(PORT); //Port ("htons" means "Host TO Network Short")
	addr.sin_family = AF_INET; //IPv4 Socket

	clientPtr = this;
}

bool Client::Connect()
{
	// --- Connection socket ---
	connection = socket(AF_INET, SOCK_STREAM, NULL); //Set connection socket
	int code = connect(connection, (SOCKADDR*)&addr, sizeof(addr));
	if (code != 0) //If connecting to server failed
	{
		code = WSAGetLastError();
		std::string error = "Failed to connect, code: " + std::to_string(code);
		MessageBoxA(NULL, error.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false; //Failed to connect to server
	}

	//Succesful connection to server
	std::cout << "Connected to server!" << std::endl;

	//Create client thread to handle upcomming messages
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
	return true;
}

bool Client::CloseConnection()
{
	if (closesocket(connection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error occured
			return true; //Connection has been closed

		std::string error = "Failed to close Socket, WinSock: "+std::to_string(WSAGetLastError())+".";
		MessageBoxA(NULL, error.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false; //Failed to close connection
	}
	return true;
}

Protocol * Client::recvProtocol()
{
	Protocol * protocol;
	char * buffer = new char[9];
	//recv packet header and data size
	int check = recv(connection, buffer, 9, NULL);
	if (check == SOCKET_ERROR)
		return nullptr;

	protocol = new Protocol(buffer, 9);
	u_int64 dataSize = protocol->getDataSize();
	PacketHeader packetHeader = protocol->getPacketHeader();
	delete[] buffer;
	delete protocol;
	//if there is data field
	if (dataSize > 0)
	{
		buffer = new char[dataSize];
		//recv text message
		check = recv(connection, buffer, dataSize, NULL);
		if (check == SOCKET_ERROR)
			return nullptr;
	}
	string data(buffer);
	//recv client id
	delete[] buffer;
	buffer = new char[8];
	u_int64 ID;

	check = recv(connection, buffer, 8, NULL);
	if (check == SOCKET_ERROR)
		return nullptr;

	Bytes bytes;
	bytes.push_bytes(buffer, 8);
	ID = bytes.getBytes8(0,0,0).to_ullong();
	protocol = new Protocol(packetHeader, dataSize, data, ID);
	return protocol;
}

bool Client::sendProtocol(Protocol * protocol)
{
	int messageLenght = 0;
	char * message = protocol->getStringToSend(messageLenght);
	//Send string in strSize bytes
	int check = send(connection, message, messageLenght, NULL);
	if (check == SOCKET_ERROR) //If sending failed
		return false;
	return true;
}


bool Client::processProtocol(Protocol * protocol)
{
	if (protocol == nullptr)
		return false;

	PacketHeader packetHeader = protocol->getPacketHeader();
	u_int64 dataSize = protocol->getDataSize();
	string data = "";
	if (dataSize > 0)
	{
		protocol->getData();
	}
	switch (packetHeader)
	{
	case PacketHeader::Invite:
	{
		Protocol * answer;
		cout << "Zostales zaproszony do czatu. Wpisz:\n"
			<< " y - by zaakceptowac zaproszenie,\n"
			<< " n - by odrzucic zaproszenie.\n";
		char choice;
		cin >> choice;
		switch (choice)
		{
		case 'y':
		case 'Y':
			answer = new Protocol(PacketHeader::InviteAccpet, 0);
			break;
		default:
			answer = new Protocol(PacketHeader::InviteDecline, 0);
			break;
		}
		sendProtocol(answer);
		delete answer;
		break;
	}
	case PacketHeader::InviteAccpet:
	{
		cout << "Zaakceptowal zaproszenie";
		break;
	}
	case PacketHeader::InviteDecline:
	{
		cout << "Odrzucil zaproszenie";
		break;
	}
	default:
		cout << "# Odebrano nieznany pakiet. #" << endl;
		break;
	}
	return true;
}

void Client::ClientThread()
{
	Protocol * protocol;
	while (true)
	{
		protocol = clientPtr->recvProtocol();
		if (protocol == nullptr) //If appeard issue with receiving packetType
			break; //Break infinite loop
		if (!clientPtr->processProtocol(protocol)) //If packet is not properly processed
			break; //Break infinite loop
		delete protocol;
	}
	std::cout << "# Utracono polaczenie z serwerem. #" << std::endl;
	if (clientPtr->CloseConnection()) {
		std::cout << "# Poprawnie zamknieto gniazdo. #" << std::endl;
	}
	else
	{
		std::cout << "# Blad podczas zamykania gniazda. #" << std::endl;
	}
}
