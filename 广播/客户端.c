#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock.h>
#pragma comment(lib,"WS2_32.lib")

#define   port		  5029
#define   ip_addr    "127.0.0.1"

void main()
{
    WSADATA WSA;		//对比版本
	WORD	sockVersion = MAKEWORD(2,2);//加载套接字库的版本号
	/*WSAStartup必须是应用程序的第一个Windows Sockets函数，应用程序只能在一次成功的WSAStartup()调用之后才能调用进一步的Windows Sockets API函数。*/
	/*WSAStartup就是为了向操作系统说明，我们要用哪个库文件，让该库文件与当前的应用程序绑定，从而就可以调用该版本的socket的各种函数了。*/
	int ads = WSAStartup(sockVersion, &WSA);
	printf("%d",ads);
	if (WSAStartup(sockVersion, &WSA) != 0)
	{
		printf("startup 失败");
		return 0;
	}


	SOCKET client;			//客户端
	struct sockaddr_in		serveraddr;//服务器地址
	int addrlength = 0;
	HANDLE hthread = NULL;//线程句柄
	int Ret = 0;//结果

	char sendbuf[256] = {0};


	client = socket(AF_INET,  SOCK_STREAM,0);//创建socket
	if (client == INVALID_SOCKET)
	{
		puts("客户端创建失败");
		system("pause");
	}

	serveraddr.sin_family = AF_INET;//协议簇
	serveraddr.sin_addr.s_addr = inet_addr(ip_addr);//设置地址
	serveraddr.sin_port = htons(port);//设置端口
	memset(serveraddr.sin_zero, 0x00, 8);//清空

	Ret = connect(client,  (struct sockaddr*)&serveraddr,  sizeof(serveraddr));//链接
	if (Ret != 0)
	{
		puts("客户端创建失败");
		system("pause");
	}

	while (1)
	{
		scanf("%s",sendbuf);//输入

		Ret = send( client, sendbuf, strlen(sendbuf),0 );
		
		if (Ret == SOCKET_ERROR)
		{
			puts("客户端发送失败");
			system("pause");
		}
	}

	closesocket(client);//关闭
	WSACleanup();//清空

	system("pause");
}

