#pragma once
#include "UDPClient.h"
#include "windows.h"
#include <iostream>
using namespace std;
using namespace FeatherFAST;
void UDPClient::Init(char* Addr, char* Group, char* Source, char* Port)
{
	//cout << Addr<<" "<<Group<<" "<<Source<<" "<<Port;
	_group = Group;
	_addr = Addr;
	_source = Source;
	_port = Port;
	WSADATA wsDATA;
	if (WSAStartup(MAKEWORD(2, 2), &wsDATA) != 0)
		throw UDPWSAStartupError;
	ADDRINFOA hint, * resp;
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_DGRAM;
	hint.ai_protocol = IPPROTO_UDP;
	if (getaddrinfo(_addr, _port, &hint, &resp) != 0)
		throw UDPgetaddrinfoError;
	_sock = INVALID_SOCKET;
	_sock = socket(resp->ai_family, resp->ai_socktype, resp->ai_protocol);
	if (_sock == INVALID_SOCKET)
		throw UDPInvalidSocket;
	if (bind(_sock, resp->ai_addr, resp->ai_addrlen) == SOCKET_ERROR) {
		throw UDPsocketError;
	};
	struct ip_mreq mreq;
	struct ip_mreq_source imr;
	mreq.imr_multiaddr.s_addr = inet_addr(_group);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	imr.imr_multiaddr.s_addr = inet_addr(_group);
	imr.imr_sourceaddr.s_addr = inet_addr(_source);
	imr.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(_sock, IPPROTO_IP, IP_ADD_SOURCE_MEMBERSHIP, (char*)& imr, sizeof(imr)) < 0)
		throw UDPsetsockoptError;
	int Mode = 1;
	if (ioctlsocket(_sock, FIONBIO, (u_long*)& Mode) != NO_ERROR)
		throw UDPioctlsocketError;
}

void UDPClient::Read(FASTDecoder& FASTDec)
{
	sockaddr_in from;
	char buffer[MSGBUFSIZE];
	int fromlen = sizeof(from);
	int nbytes = 0;
	nbytes = recvfrom(_sock, (char*)buffer, MSGBUFSIZE, 0, (SOCKADDR*)& from, &fromlen);
	if (nbytes < 0) {
		if (WSAGetLastError() == 10035)
			return;
		throw UDPReadError;
	}
	else
	{
		FASTDec.Process(buffer, nbytes);
	}
}

int FeatherFAST::UDPClient::Read(char* output)
{
	sockaddr_in from;
	int fromlen = sizeof(from);
	int nbytes = 0;
	nbytes = recvfrom(_sock, output, MSGBUFSIZE, 0, (SOCKADDR*)& from, &fromlen);
	if (nbytes < 0) {
		if (WSAGetLastError() == 10035)
			return nbytes;
		throw UDPReadError;
	}
	return nbytes;
}
