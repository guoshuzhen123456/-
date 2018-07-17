#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")

#define   port		5029
#define   ip_addr  "127.0.0.1"

//创建线程
DWORD WINAPI clientthread(void *p)
{
	//SOCKET实际上是一个指针
	SOCKET client = (SOCKET)p;//参数指针类型转换
	int Ret = 0;
	char receivebuf[256];

	while (1)
	{
		memset(receivebuf, 0, 256);//清零
		Ret = recv(client,receivebuf,256,0);
		if (Ret == SOCKET_ERROR)
		{
			puts("客户端send失败");
			break;
		}
		printf("\n收到%s",receivebuf);
	}
	return 0;
}

//服务器主程
void main()
{
	//WSADATA WSA;		//对比版本
	SOCKET client,	server;			
	struct sockaddr_in	localeaddr,  clientaddr;//服务器地址,客户端地址
	int addrlength = 0;
	HANDLE hthred = NULL;//线程句柄
	int Ret = 0;//结果

	char sendbuf[256] = { 0 };

	//if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0);
	//{	//版本不符合
	//	puts("版本不一致，通信失败");
	//	system("pause");
	//	return;
	//}

	server = socket(AF_INET,SOCK_STREAM,0);

	if ( server == INVALID_SOCKET)
	{
		puts("服务器创建失败");
		system("pause");
		return;
	}

	localeaddr.sin_family = AF_INET;
	localeaddr.sin_addr.s_addr = inet_addr(ip_addr);
	localeaddr.sin_port = htons(port);
	memset(localeaddr.sin_zero,0x00,8);

	Ret = bind(server, (struct sockaddr*)&localeaddr, sizeof(localeaddr));
	if (Ret!=0)
	{
		puts("绑定失败");
		system("pause");
		return;
	}

	//开始监听
	Ret = listen(server,5);//最多同时5个客户端
	if (Ret != 0)
	{
		puts("监听失败");
		system("pause");
		return;
	}
	puts("服务器启动");

	while (1)
	{
		addrlength = sizeof(clientaddr);
		client = accept(server,(struct sockaddr*)&clientaddr, &addrlength);//接受客户端请求
		if (client == INVALID_SOCKET)
		{
			puts("接收失败");
			system("pause");
		}
		printf("\n客户端链接%s %d",inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port);

		hthred = CreateThread(NULL, 0, clientthread,  (void*)client, 0, NULL);

		//WaitForSingleObject(hthread,INFINITE);
			
	}

	closesocket(server);
	closesocket(client);
	WSACleanup();
	
}



