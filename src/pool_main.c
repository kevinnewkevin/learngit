/*************************************************************************
  > File Name: pool_main.c
  > Author: Chen
  > Mail: baiyccy@126.com 
  > Created Time: Thu 09 Apr 2015 11:00:17 PM CST
 ************************************************************************/

#include"pool_threads.h"

int sendn(int, int, char*);
int recvn(int, int, char*);
void ShowDir(int);

void* myhand(void* arg)
{
	pthd mypthd = (pthd) arg;
	elem_t sdCli;
	while(1)
	{

		sdCli = th_pop(mypthd);
		ShowDir(sdCli);
		int lMsg;
		char sRecv[1024];
		char sPath[256];
		int fdSend;
		while(1)
		{
			//recv filename
			recv(sdCli, &lMsg, 4, 0);
			recvn(sdCli, lMsg, sRecv);

			//open file
			memset(sPath, 0, 256);
			sprintf(sPath, "%s/%s", DOWN_PATH, sRecv);
			fdSend = open(sPath, O_RDONLY);
			if(fdSend == -1)
			{
				lMsg = -1;

				send(sdCli, &lMsg, 4, 0);
				continue;
			}
			lMsg = 1;
			send(sdCli, &lMsg, 4, 0);
			break;
		}
		//send file
		char sSend[1024];
		while(memset(sSend, 0, 1024), (lMsg = read(fdSend, sSend, 1023)) > 0)
		{
			send(sdCli, &lMsg, 4, 0);
			sendn(sdCli, lMsg, sSend);
		}
		lMsg = -1;
		send(sdCli, &lMsg, 4, 0);
		close(fdSend);
		close(sdCli);
	}
}

int main(int argc, char* argv[])
{
	elem_t val;
	thd mythd;

	if(argc != 3)
	{
		printf("too few\n");
		exit(1);
	}
	th_init(&mythd, 10, 3, &myhand, (void*)&mythd);

	int sdSer;
	sdSer = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in saSer;
	memset(&saSer, 0, sizeof(saSer));
	saSer.sin_family = AF_INET;
	saSer.sin_port = htons(atoi(argv[2]));
	saSer.sin_addr.s_addr = inet_addr(argv[1]);
	if(-1 == bind(sdSer, (struct sockaddr*)&saSer, sizeof(saSer)))
	{
		perror("bind");
		close(sdSer);
		exit(1);
	}

	if(-1 == listen(sdSer, 10))
	{
		perror("listen");
		close(sdSer);
		exit(1);
	}

	int sdCli;
	struct sockaddr_in saCli;
	memset(&saCli, 0, sizeof(saCli));
	socklen_t salCli = sizeof(saCli);
	while(1)
	{
		sdCli = accept(sdSer, (struct sockaddr*)&saCli, &salCli);
		if( sdCli == -1)
		{
			perror("accept");
			close(sdSer);
			exit(1);
		}
		printf("%s:%d connect....\n", inet_ntoa(saCli.sin_addr), ntohs(saCli.sin_port));	
		th_push(&mythd, sdCli);
	}

	th_destory(&mythd, atoi(argv[2]));
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

void ShowDir(int sdShow)
{
	DIR* pDir;
	pDir = opendir(DOWN_PATH);
	if(pDir == NULL)
	{
		perror("opendir");
		return;
	}

	struct dirent* pFile;
	char sSend[1024] = "";
	while((pFile = readdir(pDir)) != NULL)
	{
		if(strcmp(".", pFile->d_name) == 0 || strcmp("..", pFile->d_name) == 0)
		{
			continue;
		}
		strcat(sSend, pFile->d_name);
		strcat(sSend, "\n");
	}
	int lSend;
	lSend = strlen(sSend);
	send(sdShow, &lSend, 4, 0);
	sendn(sdShow, lSend, sSend);
}
