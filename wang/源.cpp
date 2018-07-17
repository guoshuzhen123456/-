#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <process.h>

//#include <WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")

using namespace std;
#define SERVER_IP        "127.0.0.1"
#define SERVER_PORT        9999
#define BUFSIZE            (1024)

//定义一个结构体，存储客户端的sClient、sockaddr_in结构
typedef struct  _ClientStruct
{
	SOCKET csSocket;
	SOCKADDR_IN csSockAddr_In;
} ClientStruct, *LPClientStruct;

//处理后续客户端事件的程序
void ClientEven(PVOID param);

int main()
{
	int ErrCode;    //错误代码
	int addrlen;
	WSADATA WSAdata;
	SOCKET sServer, sClient;
	SOCKADDR_IN saServer, saClient;

	//初始化
	ErrCode = WSAStartup(MAKEWORD(2, 2), &WSAdata);
	if (ErrCode)
	{
		cout << "WSAStartup()出错！错误代码：#" << ErrCode << endl;
		WSACleanup();
		return -1;
	}

	//创建套接字
	sServer = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sServer)
	{
		cout << "socket()出错！错误代码：#" << WSAGetLastError() << endl;
		WSACleanup();
		return -2;
	}

	//初始化saServer
	saServer.sin_family = AF_INET;
	saServer.sin_addr.S_un.S_addr = htons(INADDR_ANY);
	saServer.sin_port = htons(SERVER_PORT);
	//绑定监听IP和端口，以便下面的listen()监听
	ErrCode = bind(sServer, (SOCKADDR *)&saServer, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == ErrCode)
	{
		cout << "bind()出错！错误代码：#" << WSAGetLastError() << endl;
		WSACleanup();
		return -3;
	}

	//开始监听
	ErrCode = listen(sServer, 10);
	if (SOCKET_ERROR == ErrCode)
	{
		cout << "listen()出错！错误代码：#" << WSAGetLastError() << endl;
		WSACleanup();
		return -4;
	}
	cout << "正在监听端口：" << SERVER_PORT << endl;

	while (TRUE)
	{
		//接受来自客户端的连接请求
		addrlen = sizeof(saClient);
		sClient = accept(sServer, (SOCKADDR *)&saClient, &addrlen);
		if (INVALID_SOCKET == sClient)
		{
			cout << "accpet()出错！错误代码：#" << WSAGetLastError() << endl;
		}

		//启动一个线程，来处理后续的客户端事件
		LPClientStruct lpcsClient = new ClientStruct;    //个人觉得这儿用 new 是一个创举啊，因为在启动线程后，如果直接将ClientStruct指针
		lpcsClient->csSocket = sClient;            //传到ClientEven()中，那么，当有中一个Client接入时，会改变ClientEven中的ClientStruct内容
		lpcsClient->csSockAddr_In = saClient;        //所以这儿用new,再在线程中delete
		_beginthread(ClientEven, 0, lpcsClient);

	}
	WSACleanup();
	return 0;
}


void ClientEven(PVOID param)
{
	int ErrCode;

	char buf[BUFSIZE] = { 0 };
	SYSTEMTIME time;
	GetLocalTime(&time);
	sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	//设置下输出的字体的颜色
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << buf << "\t" << inet_ntoa(((LPClientStruct)param)->csSockAddr_In.sin_addr) << ":"
		<< ((LPClientStruct)param)->csSockAddr_In.sin_port << "\t接入服务器" << endl;
	//还原字体颜色
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
	//连接成功，先发送一条成功的消息
	ErrCode = send(((LPClientStruct)param)->csSocket, "连接服务器成功", strlen("连接到服务器成功！"), 0);
	if (SOCKET_ERROR == ErrCode)
	{
		cout << "send()出错！错误代码：#" << WSAGetLastError() << endl;
		delete (LPClientStruct)param;
		_endthread();
	}

	while (TRUE)
	{
		//接受消息
		ZeroMemory(buf, BUFSIZE);
		ErrCode = recv(((LPClientStruct)param)->csSocket, buf, sizeof(buf), 0);
		if (SOCKET_ERROR == ErrCode)
		{
			if (WSAGetLastError() == 10054)
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				cout << inet_ntoa(((LPClientStruct)param)->csSockAddr_In.sin_addr) << ":" << ((LPClientStruct)param)->csSockAddr_In.sin_port << "\t被强制断开连接！" << endl;
				SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
			}
			else
			{
				cout << "recv()出错！错误代码：#" << WSAGetLastError() << endl;
			}

			delete (LPClientStruct)param;    //退出线程之前先delete前面new的ClientStruct
			_endthread();
		}

		cout << inet_ntoa(((LPClientStruct)param)->csSockAddr_In.sin_addr) << ":" << ((LPClientStruct)param)->csSockAddr_In.sin_port << "\t说：" << buf << endl;

		//反馈消息                
		//if ((pPipe = _popen(buf, "rt")) != NULL)
		//{
		//    while (!feof(pPipe))
		//    {
		//        fgets(szResult, 32, pPipe);
		//        strcat(buf, szResult);
		//        if (strlen(buf) == 1024)
		//        {
		//            buf[1024] = 'c';    //c --continue
		//            send(((LPClientStruct)param)->csSocket, buf, sizeof(buf), 0);
		//            ZeroMemory(buf, BUFSIZE);
		//        }
		//    }
		//    _pclose(pPipe);
		//}
		//else
		//    strcpy(buf, "打开匿名管道失败！");

		ErrCode = send(((LPClientStruct)param)->csSocket, buf, sizeof(buf), 0);

		if (SOCKET_ERROR == ErrCode)
		{
			cout << "send()出错！错误代码：#" << WSAGetLastError() << endl;

			delete (LPClientStruct)param;    //退出线程之前先delete前面new的ClientStruct
			_endthread();
		}
	}
	delete (LPClientStruct)param;    //退出线程之前先delete前面new的ClientStruct
	_endthread();
}