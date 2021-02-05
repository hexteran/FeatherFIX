#include "TCPClient.h"
#include <iostream>
bool FeatherFIX::TCPClient::Connect(PCSTR Addr, PCSTR Port)
{
	int Result = WSAStartup(MAKEWORD(2, 2), &wsDATA);
	if (Result != 0) {
		throw 1;
		return -1;
	}
	ADDRINFOA hint, * resp;
	memset(&hint, 0, sizeof(hint));
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_protocol = IPPROTO_TCP;
	Result = getaddrinfo(Addr, Port, &hint, &resp);
	if (Result != 0) {
		throw 2;
		return -1;
	}
	sock = INVALID_SOCKET;
	sock = socket(resp->ai_family, resp->ai_socktype, resp->ai_protocol);
	if (sock == INVALID_SOCKET) {
		throw 3;
		return -1;
	}
	int timeout = 10000; //in milliseconds. this is 10 seconds вот ето вот константа нужно менять
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)& timeout, sizeof(int)); //setting the receive timeout

	Result = connect(sock, resp->ai_addr, (int)resp->ai_addrlen);
	if (Result != 0) {
		throw 4;
		return -1;
	}
	return 0;
};

int FeatherFIX::TCPClient::Send(PCSTR message, int len) {
	int Result = send(sock, message, len, 0);
	int LastError = WSAGetLastError();
	if (LastError != 0) {
		throw 5;
		return -1;
	}
	return Result;
}
int FeatherFIX::TCPClient::Receive(char* input, int buflen = 1048576) {
	int Result = 0;
	Result = recv(sock, input, buflen, 0);
	int LastError = WSAGetLastError();
	if (LastError != 0) {
		throw 6;
		return -1;
	}
	input[Result] = 0;
	std::cout <<input;
	return Result;
}