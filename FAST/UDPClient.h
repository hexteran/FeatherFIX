#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include "FASTDecoder.h"
#pragma comment(lib,"ws2_32.lib")

#define UDPWSAStartupError 301
#define UDPgetaddrinfoError 302
#define UDPInvalidSocket 303
#define UDPsocketError 304
#define UDPsetsockoptError 305
#define UDPioctlsocketError 306
#define UDPReadError 307

#define MSGBUFSIZE 16384
namespace FeatherFAST {
	class __declspec(dllexport) UDPClient
	{
	public:
		int _thread_id;
		SOCKET _sock;
		char* _addr, * _source, * _group, * _port;
	public:
		void Init(char* Addr, char* Group, char* Source, char* Port);
		void Read(FASTDecoder& FASTDec);
		int Read(char* output);
	};

	class TCPClient
	{

	};
};