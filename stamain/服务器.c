#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")

#define   port		5029
#define   ip_addr  "127.0.0.1"

//�����߳�
DWORD WINAPI clientthread(void *p)
{
	//SOCKETʵ������һ��ָ��
	SOCKET client = (SOCKET)p;//����ָ������ת��
	int Ret = 0;
	char receivebuf[256];

	while (1)
	{
		memset(receivebuf, 0, 256);//����
		Ret = recv(client,receivebuf,256,0);
		if (Ret == SOCKET_ERROR)
		{
			puts("�ͻ���sendʧ��");
			break;
		}
		printf("\n�յ�%s",receivebuf);
	}
	return 0;
}

//����������
void main()
{
	//WSADATA WSA;		//�ԱȰ汾
	SOCKET client,	server;			
	struct sockaddr_in	localeaddr,  clientaddr;//��������ַ,�ͻ��˵�ַ
	int addrlength = 0;
	HANDLE hthred = NULL;//�߳̾��
	int Ret = 0;//���

	char sendbuf[256] = { 0 };

	//if (WSAStartup(MAKEWORD(2, 2), &WSA) != 0);
	//{	//�汾������
	//	puts("�汾��һ�£�ͨ��ʧ��");
	//	system("pause");
	//	return;
	//}

	server = socket(AF_INET,SOCK_STREAM,0);

	if ( server == INVALID_SOCKET)
	{
		puts("����������ʧ��");
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
		puts("��ʧ��");
		system("pause");
		return;
	}

	//��ʼ����
	Ret = listen(server,5);//���ͬʱ5���ͻ���
	if (Ret != 0)
	{
		puts("����ʧ��");
		system("pause");
		return;
	}
	puts("����������");

	while (1)
	{
		addrlength = sizeof(clientaddr);
		client = accept(server,(struct sockaddr*)&clientaddr, &addrlength);//���ܿͻ�������
		if (client == INVALID_SOCKET)
		{
			puts("����ʧ��");
			system("pause");
		}
		printf("\n�ͻ�������%s %d",inet_ntoa(clientaddr.sin_addr), clientaddr.sin_port);

		hthred = CreateThread(NULL, 0, clientthread,  (void*)client, 0, NULL);

		//WaitForSingleObject(hthread,INFINITE);
			
	}

	closesocket(server);
	closesocket(client);
	WSACleanup();
	
}



