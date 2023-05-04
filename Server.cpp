#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")


int main() {

	// 1. ȷ��Э��汾��Ϣ
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		printf("��������-ȷ��Э��汾��Ϣ����%d\n", GetLastError());
		return -1;
	}
	printf("��������-ȷ��Э��汾��Ϣ�ɹ���\n");

	// 2. ����socket 
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET:IPV4, SOCK_STREAM:TCP, SOCK_DGRAM
	if (SOCKET_ERROR == serverSocket) {
		printf("��������-����socketʧ�ܣ�%d\n", GetLastError());
		return -1;
	}
	printf("��������-����socket�ɹ���\n");

	// 3. ȷ��������Э���ַ��
	SOCKADDR_IN sAddr = { 0 };
	sAddr.sin_family = AF_INET; // socket������һ������
	sAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	// �����ֽ�˳��ת��Ϊ�����ֽ�˳�� 0.0.0.0, host to net long
	sAddr.sin_port = htons(10086); // 


	// 4. ��
	int r = bind(serverSocket, (sockaddr*)&sAddr, sizeof(sAddr));
	if (SOCKET_ERROR == r) {

		// 8. �ر�socket
		closesocket(serverSocket);

		// 9. ����Э��汾��Ϣ
		WSACleanup();
		printf("��������-��ʧ��: %d\n", GetLastError());
		return -1;
	}
	printf("��������-�󶨳ɹ���\n");

	// 5. ����
	r = listen(serverSocket, 10);
	if (SOCKET_ERROR == r) {

		// 8. �ر�socket
		closesocket(serverSocket);

		// 9. ����Э��汾��Ϣ
		WSACleanup();
		printf("��������-��������ʧ��: %d\n", GetLastError());
		return -1;
	}
	printf("��������-���������ɹ���\n");

	// 6. ���տͻ�������
	while (true)
	{
		SOCKADDR_IN addrClient;
		int length = sizeof(addrClient);
		SOCKET sockConn = accept(serverSocket, (SOCKADDR*) &addrClient, &length);
		printf("�ͻ��ˣ�%s:%d�����˷�����\n", inet_ntoa(addrClient.sin_addr), addrClient.sin_port);
		printf("������������\n");
		printf("1.ע�� 2.���� 3.�ػ� 4.��������\n");
		printf("��������������� (1-4��:");

		int iCmd;
		scanf("%d", &iCmd);

		switch (iCmd) 
		{
		case 1:
		case 2:
		case 3:
		case 4:

			char sCmd[2];
			sprintf(sCmd, "%d", iCmd);
			send(sockConn, sCmd, sizeof(iCmd), NULL);
			printf("\n��ѡ����%s������\n", sCmd);
			break;
		default:
			printf("�������!\n");
			break;
		}
	}

	return 0;
}