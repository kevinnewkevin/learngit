/*************************************************************************
  > File Name: client.c
  > Author: Chen
  > Mail: baiyccy@126.com 
  > Created Time: Wed 15 Apr 2015 11:29:31 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

int sendn(int, int, char*);
int recvn(int, int, char*);

int main(int argc, char* argv[])//server's ip and port
{
	int sdCli;
	sdCli = socket(AF_INET, SOCK_STREAM, 0);
	if(sdCli == -1)
	{
		perror("socket");
		exit(1);
	}

	struct sockaddr_in saSer;
	memset(&saSer, 0, sizeof(saSer));
	saSer.sin_family = AF_INET;
	saSer.sin_port = htons(atoi(argv[2]));
	saSer.sin_addr.s_addr = inet_addr(argv[1]);
	if(connect(sdCli, (struct sockaddr*)&saSer, sizeof(saSer)) == -1)
	{
		perror("connect");
		close(sdCli);
		exit(1);
	}

	int lMsg;
	char sRecv[1024];
	int flag;
	char sSend[1024];
	while(1)
	{
		recv(sdCli, &lMsg, 4, 0);
		memset(&sRecv, 0, 1024);
		recvn(sdCli, lMsg, sRecv);
		printf("local dir: %s\n", sRecv);

		recv(sdCli, &lMsg, 4, 0);
		memset(&sRecv, 0, 1024);
		recvn(sdCli, lMsg, sRecv);
		printf("--------------------------------------------\n");
		printf("%s", sRecv);
		printf("--------------------------------------------\n");


		fgets(sSend, 1024, stdin);
		lMsg = strlen(sSend);
		sSend[lMsg - 1] = '\0';
		lMsg -= 1;
		if(lMsg == 0)
			continue;
		send(sdCli, &lMsg, 4, 0);
		sendn(sdCli, lMsg, sSend);
		if(sSend[0] == 'c' && sSend[1] == 'd' && sSend[2] == ' ')
			continue;
		recv(sdCli, &flag, 4, 0);
		if(flag == -1)
			printf("file not exist!\n");
		else
			break;
	}

	int fdWrite;
	fdWrite = open(sSend, O_WRONLY | O_CREAT, 0666);
	if(fdWrite == -1)
	{
		perror("open");
		close(sdCli);
		exit(1);
	}

	while(1)
	{
		recv(sdCli, &lMsg, 4, 0);
		if(lMsg == -1)
		{
			printf("recv over!\n");
			break;
		}
		recvn(sdCli, lMsg, sRecv);
		write(fdWrite, sRecv, lMsg);
	}
	close(sdCli);
	return 0;
}

int sendn(int sdSend, int lMsg, char* sSend)
{
	int lDrift = 0;
	int lTemp;
	while(lMsg > lDrift)
	{
		lTemp = send(sdSend, sSend + lDrift, lMsg - lDrift, 0);
		lDrift += lTemp;
	}
	return lDrift;
}

int recvn(int sdRecv, int lMsg, char* sRecv)
{
	int lDrift = 0;
	int lTemp;
	while(lMsg > lDrift)
	{
		lTemp = recv(sdRecv, sRecv + lDrift, lMsg - lDrift, 0);
		lDrift += lTemp;
	}
	return lDrift;
}
