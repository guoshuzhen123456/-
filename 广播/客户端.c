#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<winsock.h>
#pragma comment(lib,"WS2_32.lib")

#define   port		  5029
#define   ip_addr    "127.0.0.1"

void main()
{
    WSADATA WSA;		//�ԱȰ汾
	WORD	sockVersion = MAKEWORD(2,2);//�����׽��ֿ�İ汾��
	/*WSAStartup������Ӧ�ó���ĵ�һ��Windows Sockets������Ӧ�ó���ֻ����һ�γɹ���WSAStartup()����֮����ܵ��ý�һ����Windows Sockets API������*/
	/*WSAStartup����Ϊ�������ϵͳ˵��������Ҫ���ĸ����ļ����øÿ��ļ��뵱ǰ��Ӧ�ó���󶨣��Ӷ��Ϳ��Ե��øð汾��socket�ĸ��ֺ����ˡ�*/
	int ads = WSAStartup(sockVersion, &WSA);
	printf("%d",ads);
	if (WSAStartup(sockVersion, &WSA) != 0)
	{
		printf("startup ʧ��");
		return 0;
	}


	SOCKET client;			//�ͻ���
	struct sockaddr_in		serveraddr;//��������ַ
	int addrlength = 0;
	HANDLE hthread = NULL;//�߳̾��
	int Ret = 0;//���

	char sendbuf[256] = {0};


	client = socket(AF_INET,  SOCK_STREAM,0);//����socket
	if (client == INVALID_SOCKET)
	{
		puts("�ͻ��˴���ʧ��");
		system("pause");
	}

	serveraddr.sin_family = AF_INET;//Э���
	serveraddr.sin_addr.s_addr = inet_addr(ip_addr);//���õ�ַ
	serveraddr.sin_port = htons(port);//���ö˿�
	memset(serveraddr.sin_zero, 0x00, 8);//���

	Ret = connect(client,  (struct sockaddr*)&serveraddr,  sizeof(serveraddr));//����
	if (Ret != 0)
	{
		puts("�ͻ��˴���ʧ��");
		system("pause");
	}

	while (1)
	{
		scanf("%s",sendbuf);//����

		Ret = send( client, sendbuf, strlen(sendbuf),0 );
		
		if (Ret == SOCKET_ERROR)
		{
			puts("�ͻ��˷���ʧ��");
			system("pause");
		}
	}

	closesocket(client);//�ر�
	WSACleanup();//���

	system("pause");
}

