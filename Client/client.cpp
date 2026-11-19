#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <WinSock2.h> //ws2_32.dll

#pragma comment(lib, "ws2_32")

int main()
{
	WSAData wsaData;
	int Result = WSAStartup(MAKEWORD(2, 2), &wsaData); //windows에서만 씀
	if (Result != 0)
	{
		printf("WSAStartup failed.\n");
		exit(-1);
	}

	SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET)
	{
		printf("socket create failed.\n");
		exit(-1);
	}

	//내가 쓸 주소 설정, ip, 설정
	sockaddr_in ServerSockAddr;
	memset(&ServerSockAddr, 0, sizeof(ServerSockAddr));
	ServerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ServerSockAddr.sin_port = htons(20303);
	ServerSockAddr.sin_family = PF_INET; //IPv4

	Result = connect(ServerSocket, (sockaddr*)&ServerSockAddr, sizeof(ServerSockAddr));

	if (Result == SOCKET_ERROR)
	{
		printf("connected failed. SocketError %d\n", WSAGetLastError());
		exit(-1);
	}

	char Buffer[1024] = "Hello World";

	int SendBytes = send(ServerSocket, Buffer, (int)strlen(Buffer) + 1 , 0);
	if (SendBytes == 0)
	{
		printf("disconnect.\n");
		exit(-1);
	}
	else if (SendBytes < 0)
	{
		printf("error disconnect. %d\n", WSAGetLastError());
		exit(-1);
	}

	//blocking
	int RecvBytes = recv(ServerSocket, Buffer, 1024, 0);
	if (RecvBytes == 0)
	{
		printf("disconnect.\n");
		exit(-1);
	}
	else if (RecvBytes < 0)
	{
		printf("error disconnect. %d\n", WSAGetLastError());
		exit(-1);
	}

	printf("Recv Data %s", Buffer);



	closesocket(ServerSocket);

	WSACleanup();

	return 0;
}