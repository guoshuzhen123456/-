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

//����һ���ṹ�壬�洢�ͻ��˵�sClient��sockaddr_in�ṹ
typedef struct  _ClientStruct
{
	SOCKET csSocket;
	SOCKADDR_IN csSockAddr_In;
} ClientStruct, *LPClientStruct;

//��������ͻ����¼��ĳ���
void ClientEven(PVOID param);

int main()
{
	int ErrCode;    //�������
	int addrlen;
	WSADATA WSAdata;
	SOCKET sServer, sClient;
	SOCKADDR_IN saServer, saClient;

	//��ʼ��
	ErrCode = WSAStartup(MAKEWORD(2, 2), &WSAdata);
	if (ErrCode)
	{
		cout << "WSAStartup()����������룺#" << ErrCode << endl;
		WSACleanup();
		return -1;
	}

	//�����׽���
	sServer = socket(AF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == sServer)
	{
		cout << "socket()����������룺#" << WSAGetLastError() << endl;
		WSACleanup();
		return -2;
	}

	//��ʼ��saServer
	saServer.sin_family = AF_INET;
	saServer.sin_addr.S_un.S_addr = htons(INADDR_ANY);
	saServer.sin_port = htons(SERVER_PORT);
	//�󶨼���IP�Ͷ˿ڣ��Ա������listen()����
	ErrCode = bind(sServer, (SOCKADDR *)&saServer, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == ErrCode)
	{
		cout << "bind()����������룺#" << WSAGetLastError() << endl;
		WSACleanup();
		return -3;
	}

	//��ʼ����
	ErrCode = listen(sServer, 10);
	if (SOCKET_ERROR == ErrCode)
	{
		cout << "listen()����������룺#" << WSAGetLastError() << endl;
		WSACleanup();
		return -4;
	}
	cout << "���ڼ����˿ڣ�" << SERVER_PORT << endl;

	while (TRUE)
	{
		//�������Կͻ��˵���������
		addrlen = sizeof(saClient);
		sClient = accept(sServer, (SOCKADDR *)&saClient, &addrlen);
		if (INVALID_SOCKET == sClient)
		{
			cout << "accpet()����������룺#" << WSAGetLastError() << endl;
		}

		//����һ���̣߳�����������Ŀͻ����¼�
		LPClientStruct lpcsClient = new ClientStruct;    //���˾�������� new ��һ�����ٰ�����Ϊ�������̺߳����ֱ�ӽ�ClientStructָ��
		lpcsClient->csSocket = sClient;            //����ClientEven()�У���ô��������һ��Client����ʱ����ı�ClientEven�е�ClientStruct����
		lpcsClient->csSockAddr_In = saClient;        //���������new,�����߳���delete
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

	//������������������ɫ
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
	cout << buf << "\t" << inet_ntoa(((LPClientStruct)param)->csSockAddr_In.sin_addr) << ":"
		<< ((LPClientStruct)param)->csSockAddr_In.sin_port << "\t���������" << endl;
	//��ԭ������ɫ
	SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
	//���ӳɹ����ȷ���һ���ɹ�����Ϣ
	ErrCode = send(((LPClientStruct)param)->csSocket, "���ӷ������ɹ�", strlen("���ӵ��������ɹ���"), 0);
	if (SOCKET_ERROR == ErrCode)
	{
		cout << "send()����������룺#" << WSAGetLastError() << endl;
		delete (LPClientStruct)param;
		_endthread();
	}

	while (TRUE)
	{
		//������Ϣ
		ZeroMemory(buf, BUFSIZE);
		ErrCode = recv(((LPClientStruct)param)->csSocket, buf, sizeof(buf), 0);
		if (SOCKET_ERROR == ErrCode)
		{
			if (WSAGetLastError() == 10054)
			{
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				cout << inet_ntoa(((LPClientStruct)param)->csSockAddr_In.sin_addr) << ":" << ((LPClientStruct)param)->csSockAddr_In.sin_port << "\t��ǿ�ƶϿ����ӣ�" << endl;
				SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
			}
			else
			{
				cout << "recv()����������룺#" << WSAGetLastError() << endl;
			}

			delete (LPClientStruct)param;    //�˳��߳�֮ǰ��deleteǰ��new��ClientStruct
			_endthread();
		}

		cout << inet_ntoa(((LPClientStruct)param)->csSockAddr_In.sin_addr) << ":" << ((LPClientStruct)param)->csSockAddr_In.sin_port << "\t˵��" << buf << endl;

		//������Ϣ                
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
		//    strcpy(buf, "�������ܵ�ʧ�ܣ�");

		ErrCode = send(((LPClientStruct)param)->csSocket, buf, sizeof(buf), 0);

		if (SOCKET_ERROR == ErrCode)
		{
			cout << "send()����������룺#" << WSAGetLastError() << endl;

			delete (LPClientStruct)param;    //�˳��߳�֮ǰ��deleteǰ��new��ClientStruct
			_endthread();
		}
	}
	delete (LPClientStruct)param;    //�˳��߳�֮ǰ��deleteǰ��new��ClientStruct
	_endthread();
}