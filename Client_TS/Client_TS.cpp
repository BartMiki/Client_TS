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
	bool program = true;
	std::string buffer;
	char choice;
	cout <<
		" 1 - zapros uzytkownika,\n" <<
		" 2 - zaakceptuj zaproszenie, \n" <<
		" 3 - odrzuc zaproszenie, \n" <<
		" 4 - wyslij wiadomosc, \n" <<
		" 5 - zakoncz rozmowe z uzytkowniikiem, \n" <<
		" 6 - zakoncz polaczenie z serwerem, \n" <<
		" 7 - wylacz program, \n" <<
		" 8 - polacz z serwerem, \n" <<
		" h - wyswietl ponownie klawiszologie.\n";
	while (program)
	{
		Sleep(10);
		getline(cin, buffer);
		choice = buffer[0];
		switch (choice)
		{
		case '1':
		{
			if (client.connectedToServer == false)
			{
				cout << "# Nie jestes podlaczony do sewera. #\n";
				break;
			}
			if (client.sendedInvite)
			{
				cout << "# Juz wyslales zaproszenie do tej osoby #\n";
				break;
			}
			client.sendProtocol(new Protocol(PacketHeader::Invite , 0, "", client.getID()));
			cout << "# Wyslano zaproszenie. #\n";
			client.sendedInvite = true;
			break;
		}
		case '2':
		{
			if (client.connectedToServer == false)
			{
				cout << "# Nie jestes podlaczony do sewera. #\n";
				break;
			}
			if (client.havePendingInvite)
			{
				client.sendProtocol(new Protocol(PacketHeader::InviteAccpet, 0, "", client.getID()));
				client.havePendingInvite = false;
				client.connectedToAnotherClient = true;
			}
			else
			{
				cout << "# Nie masz zadnego zaproszenia #\n";
			}
			break;
		}
		case '3':
		{
			if (client.connectedToServer == false)
			{
				cout << "# Nie jestes podlaczony do sewera. #\n";
				break;
			}
			if (client.havePendingInvite)
			{
				client.sendProtocol(new Protocol(PacketHeader::InviteDecline, 0, "", client.getID()));
				client.havePendingInvite = false;
				client.connectedToAnotherClient = false;
			}
			else
			{
				cout << "# Nie masz zadnego zaproszenia #\n";
			}
			break;
		}
		case '4':
		{
			if (client.connectedToServer == false)
			{
				cout << "# Nie jestes podlaczony do sewera. #\n";
				break;
			}
			if (!client.connectedToAnotherClient)
			{
				cout << "# Nie jestes polaczony z uzytkownikiem, nie mozesz tego zrobic. #\n";
				break;
			}
			cout << "Wiadmosc:\n";
			getline(cin, buffer);
			buffer += '\0';
			client.sendProtocol(new Protocol(PacketHeader::Message, buffer.size(), buffer, client.getID()));
			cout << "# Wiadomosc wyslana. #\n";
			break;
		}
		case '5':
		{
			if (client.connectedToServer == false)
			{
				cout << "# Nie jestes podlaczony do sewera. #\n";
				break;
			}
			if (!client.connectedToAnotherClient)
			{
				cout << "# Nie jestes polaczony z uzytkownikiem, nie mozesz tego zrobic. #\n";
				break;
			}
			client.sendProtocol(new Protocol(PacketHeader::Bye, 0, "", client.getID()));
			cout << "# Zakonczono polaczenie z uzytkownikiem. #\n";
			break;
		}
		case '6':
		{
			if (client.connection == INVALID_SOCKET || client.connectedToServer == false)
			{
				cout << "# Nie jestes podlaczony do sewera. #\n ";
				break;
			}
			if (client.CloseConnection())
			{
				cout << "# Rozlaczono z serwerem. #\n ";
				client.connectedToServer = false;
			}
			else
			{
				cout << "# Blad przy rozlaczaniu od sewera: "<< WSAGetLastError() <<". #\n ";
			}
			break;
		}
		case '7':
		{
			if (client.CloseConnection()) {
				std::cout << "# Poprawnie zamknieto gniazdo. #" << std::endl;
			}
			else
			{
				std::cout << "# Blad podczas zamykania gniazda. #" << std::endl;
			}
			return 0;
			break;
		}
		case '8':
		{
			if (client.connectedToServer == true)
			{
				cout << "# Jestes juz podlaczony do sewera. #\n";
				break;
			}
			if (client.Connect())
			{
				cout << "# Polaczono z serwerem. #\n";
				break;
			}
			else
			{
				cout << "# Nie mozna polaczyc z serwerem:" << WSAGetLastError() << "#.\n";
				break;
			}
			break;
		}
		default:
		{
			cout <<
				" 1 - zapros uzytkownika,\n" <<
				" 2 - zaakceptuj zaproszenie, \n" <<
				" 3 - odrzuc zaproszenie, \n" <<
				" 4 - wyslij wiadomosc, \n" <<
				" 5 - zakoncz rozmowe z uzytkowniikiem, \n" <<
				" 6 - zakoncz polaczenie z serwerem, \n" <<
				" 7 - wylacz program, \n" <<
				" h - wyswietl ponownie klawiszologie.\n";
			break;
		}
		}
		Sleep(10);
	}
    return EXIT_SUCCESS;
}

