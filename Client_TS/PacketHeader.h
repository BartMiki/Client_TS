#pragma once

enum class PacketHeader : char {
	ServerRequestID = 0x00, // 0000 0000 - prosi serwer o identyfikator sesji
	ServerSendID = 0x02, // 0000 0010 - serwer wysyla idetyfikator sesji
	ServerAlone = 0x04, // 0000 0100 - serwer wysyla informacje ze nie ma drugiego uzytkownika
	Invite = 0x12, // 0001 0010 - zaproszenie drugiego uzytkownika
	InviteAccpet = 0x14, // 0001 0100 - drugi uzytkownik zaakceptowal zaproszenie
	InviteDecline = 0x16, // 0001 0110 - drugi uzytkownik odrzucil zaproszenie
	Message = 0x20, // 0010 0000 - wiadomosc wyslana przez uzytkownika
	Bye = 0x30, // 0011 0000 - uzytkownik konczy komunikacje z innym uzytkownikiem
	Quit = 0x40 // 0100 0000 - uzytkownik odlacza sie od serwera
};
