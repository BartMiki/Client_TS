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
	connectedToAnotherClient = false;
	id = 0ll;
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
		std::string error = "Nie mozna polaczyc z serwerem, kod: " + std::to_string(code);
		MessageBoxA(NULL, error.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false; //Failed to connect to server
	}

	//Succesful connection to server
	std::cout << "Polaczono z serwerem!" << std::endl;

	//Create client thread to handle upcomming messages
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);
	return sendProtocol(new Protocol(PacketHeader::ServerRequestID));
}

bool Client::CloseConnection()
{
	sendProtocol(new Protocol(PacketHeader::Quit, 0, "", id));
	if (closesocket(connection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK) //If socket error occured
			return true; //Connection has been closed

		std::string error = "Nie mozna zamknac Gniazda: "+std::to_string(WSAGetLastError())+".";
		MessageBoxA(NULL, error.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false; //Failed to close connection
	}
	return true;
}

Protocol * Client::recvProtocol()
{
	Protocol * protocol;
	int bufferSize = 1500;
	char * buffer = new char[bufferSize];
	//recv packet header and data size
	int check = recv(connection, buffer, bufferSize, NULL);
	if (check == SOCKET_ERROR)
		return nullptr;

	protocol = new Protocol(buffer, 9);
	int recvSize = protocol->getDataSize();
	recvSize += 17;
	delete protocol;

	protocol = new Protocol(buffer, recvSize);
	delete[] buffer;
	return protocol;
}

bool Client::sendProtocol(Protocol * protocol)
{
	int messageLenght = 0;
	char * message = protocol->getStringToSend(messageLenght);
	//Send string in strSize bytes
	int check = send(connection, message, messageLenght, NULL);
	if (check == SOCKET_ERROR) //If sending failed
	{
		delete protocol;
		return false;
	}
	delete protocol;
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
	case PacketHeader::ServerSendID
	{
		id = protocol->getID();
		cout << "# Otrzymano identyfikator sesji od sewera: " << id << " #\n";
	}
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
			sendProtocol(new Protocol(PacketHeader::InviteAccpet, 0, "", id));
			break;
		default:
			sendProtocol(Protocol(PacketHeader::InviteDecline, 0, "", id));
			break;
		}
	}
	case PacketHeader::InviteAccpet:
	{
		cout << "# Uzytkownik zaakceptowal zaproszenie #\n";
		connectedToAnotherClient = true;
		break;
	}
	case PacketHeader::InviteDecline:
	{
		cout << "# Uzytkownik odrzucil zaproszenie #\n";
		break;
	}
	case PacketHeader::Bye:
	{
		cout << "# Uzytkownik zakonczyl z Toba polaczenie #\n";
		connectedToAnotherClient = false;
		break;
	}
	case PacketHeader::Message:
	{
		cout << ">" << data << "\n";
		break;
	}
	case PacketHeader::ServerAlone:
	{
		cout << "# Uzytkownik odlaczyl sie od serwera #\n";
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
