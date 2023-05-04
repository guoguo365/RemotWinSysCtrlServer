#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string.h>

#pragma comment(lib, "ws2_32.lib")


int main() {

	// 1. 确定协议版本信息
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		printf("服务器端-确定协议版本信息错误：%d\n", GetLastError());
		return -1;
	}
	printf("服务器端-确定协议版本信息成功！\n");

	// 2. 创建socket 
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET:IPV4, SOCK_STREAM:TCP, SOCK_DGRAM
	if (SOCKET_ERROR == serverSocket) {
		printf("服务器端-创建socket失败：%d\n", GetLastError());
		return -1;
	}
	printf("服务器端-创建socket成功！\n");

	// 3. 确定服务器协议地址蹙
	SOCKADDR_IN sAddr = { 0 };
	sAddr.sin_family = AF_INET; // socket函数第一个参数
	sAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	// 主机字节顺序转化为网络字节顺序 0.0.0.0, host to net long
	sAddr.sin_port = htons(10086); // 


	// 4. 绑定
	int r = bind(serverSocket, (sockaddr*)&sAddr, sizeof(sAddr));
	if (SOCKET_ERROR == r) {

		// 8. 关闭socket
		closesocket(serverSocket);

		// 9. 清理协议版本信息
		WSACleanup();
		printf("服务器端-绑定失败: %d\n", GetLastError());
		return -1;
	}
	printf("服务器端-绑定成功！\n");

	// 5. 监听
	r = listen(serverSocket, 10);
	if (SOCKET_ERROR == r) {

		// 8. 关闭socket
		closesocket(serverSocket);

		// 9. 清理协议版本信息
		WSACleanup();
		printf("服务器端-启动监听失败: %d\n", GetLastError());
		return -1;
	}
	printf("服务器端-启动监听成功！\n");

	// 6. 接收客户端连接
	while (true)
	{
		SOCKADDR_IN addrClient;
		int length = sizeof(addrClient);
		SOCKET sockConn = accept(serverSocket, (SOCKADDR*) &addrClient, &length);
		printf("客户端：%s:%d连接了服务器\n", inet_ntoa(addrClient.sin_addr), addrClient.sin_port);
		printf("请输入控制命令：\n");
		printf("1.注销 2.重启 3.关机 4.无限重启\n");
		printf("请输入控制命令编号 (1-4）:");

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
			printf("\n您选择了%s号命令\n", sCmd);
			break;
		default:
			printf("输入错误!\n");
			break;
		}
	}

	return 0;
}