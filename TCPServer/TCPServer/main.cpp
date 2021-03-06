#include <iostream>
#include <WS2tcpip.h>
using namespace std;

#pragma comment (lib , "ws2_32.lib")

int main()
{
	//initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOK = WSAStartup(ver, &wsData);
	if (wsOK != 0)
	{
		cerr << "Can t initialize winsock! Quitting!" << endl;
		return 0;
	}
	//create a socket(sock_stream for tcp and sock_dgram for udp)
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create socket...Quitting!" << endl;
	}
	//bind the  ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET; // AF_INET for ipv4
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));
	
	//tell winsock the socket is for listening
	listen(listening, SOMAXCONN);
	
	//wait for connection
	sockaddr_in client;
	int clientSize = sizeof(client);
	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	
	char host[NI_MAXHOST]; //client's remote name
	char service[NI_MAXHOST];   //service (i.e. port) the client is connected on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, clientSize, host, NI_MAXHOST, service, NI_MAXSERV , 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}
	//close listening socket
	closesocket(listening);
	//while loop: accept and echo message back to the client
	char buf[4096];
	while (1)
	{
		ZeroMemory(buf, 4096);
		//wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cout << "Error in recv(). Quitting!" << endl;
			break;
		}
		else if(bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}
		//echo message back to client
		send(clientSocket, buf, bytesReceived + 1, 0);
	}
	//close the socket
	closesocket(clientSocket);

	//cleanup winsock
	WSACleanup();

	return 0;
}