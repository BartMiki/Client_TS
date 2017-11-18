// Client_TS.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"

int main()
{
	Client client("127.0.0.1", 5555);
	if (!client.Connect())
	{
		std::cout << "Failed to connect to server." << std::endl;
		getchar();
		return EXIT_FAILURE;
	}

	std::string buffer;
	while (true)
	{
		cout << "Zapros uzytkownika\n";
		getchar();
		//cout << "0 - connect to server and\n"
		//std::getline(std::cin, buffer); //User input
		if (!client.sendProtocol(new Protocol(PacketHeader::Invite, 0, "", 16)))
			break;
		cout << "Wyslano zaproszenie\n";
		Sleep(10);
	}
    return EXIT_SUCCESS;
}

