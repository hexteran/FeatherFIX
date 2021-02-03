#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")

using namespace std;
namespace FeatherFIX
{
	class TCPClient
	{
		SOCKET sock;
		WSADATA wsDATA;
	public:
		bool Connect(PCSTR Addr, PCSTR Port);
		int Send(PCSTR message, int len);
		int Receive(char*, int);
	};
};