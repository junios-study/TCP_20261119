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

	SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListenSocket == INVALID_SOCKET)
	{
		printf("socket create failed.\n");
		exit(-1);
	}

	//내가 쓸 주소 설정, ip, 설정
	sockaddr_in ListenSockAddr;
	memset(&ListenSockAddr, 0, sizeof(ListenSockAddr));
	ListenSockAddr.sin_addr.s_addr = INADDR_ANY;
	ListenSockAddr.sin_port = htons(20303);
	ListenSockAddr.sin_family = PF_INET; //IPv4

	Result = bind(ListenSocket, (sockaddr*)&ListenSockAddr, sizeof(ListenSockAddr));
	if (Result == SOCKET_ERROR)
	{
		printf("bind failed. SocketError %d\n", WSAGetLastError());
		exit(-1);
	}

	Result = listen(ListenSocket, 0);
	if (Result == SOCKET_ERROR)
	{
		printf("listen failed. SocketError %d\n", WSAGetLastError());
		exit(-1);
	}

	while (1)
	{
		sockaddr_in ClientSockAddr;
		memset(&ClientSockAddr, 0, sizeof(ClientSockAddr));
		int ClientSockAddrLength = sizeof(ClientSockAddr);

		//blocking
		SOCKET ClientSocket = accept(ListenSocket, (sockaddr*)&ClientSockAddr, &ClientSockAddrLength);
		if (ClientSocket == INVALID_SOCKET)
		{
			printf("accept failed. SocketError %d\n", WSAGetLastError());
			exit(-1);
		}

		char Buffer[1024] = { 0, };

		//blocking
		int RecvBytes = recv(ClientSocket, Buffer, 1024, 0);
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

		char Buffer2[1024] = "진영이가 한우 쏜다.";

		int SendBytes = send(ClientSocket, Buffer2, (int)(strlen(Buffer2) + 1), 0);
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

		closesocket(ClientSocket);
	}

	closesocket(ListenSocket);

	WSACleanup();

	return 0;
}