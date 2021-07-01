#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib , "ws2_32.lib")

using namespace std;
int main()
{
	string ipAddress = "127.0.0.1"; //ip address of the server
	int port = 54000;    //Listening port on the server

	//Initialize Winsock
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can t start winsocket..." << " Error number " << wsResult << "\n";
		return 0;
	}

	//Create a socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket";
		WSACleanup();
	}

	//Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//Connect to the server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 0;
	}
	//Do-while loop to send and receive data
	char buffer[4096];
	string userInput;

	do {
		//prompt the user for some text
		cout << "Enter some text\n";
		getline(cin, userInput);
		//send the text
		if (userInput.size() > 0)
		{

			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buffer, 4096);
				int bytesReceived = recv(sock, buffer, 4096, 0);
				if (bytesReceived > 0)
				{
					cout << "Server " << string(buffer, 0, bytesReceived) << endl;
				}
			}
			//wait for response
			//echo response to console
		}

	} while (userInput.size() > 0);

	//Close down everything
	closesocket(sock);
	WSACleanup();
}
