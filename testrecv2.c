#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define BUF_SIZE 1024

int main(int argc,char* argv[])
{
	if (argc<=2)
	{
		printf("usage: %d ip_address port\n",basename(argv[0]));
		return 1;
	}
	
	const char* ip=argv[1];
	int port=atoi(argv[2]);
	
	struct sockaddr_in address;
	bzero(&address,sizeof(address));
	address.sin_family=AF_INET;
	inet_pton(AF_INET,ip,&address.sin_addr);
	address.sin_port=htons(port);
	
	int sock=socket(PF_INET,SOCK_STREAM,0);
	assert(sock>=0);
	
	int recvbuf=atoi(argv[3]);
	int len=sizeof(recvbuf);
	setsockopt(sock,SOL_SOCKET,SO_RCVBUF,&recvbuf,sizeof(recvbuf));
	getsockopt(sock,SOL_SOCKET,SO_RCVBUF,&recvbuf,(socklen_t*)&len);
	printf ("the tcp recv buffer is %d\n",recvbuf);

	int ret=bind(sock,(struct sockaddr*)&address,sizeof(address));
	assert(ret!=-1);
	
	ret=listen(sock,5);
	assert(ret!=-1);

	struct sockaddr_in client;
	socklen_t client_addrlen=sizeof(client);
	int connfd=accept(sock,(struct sockaddr*)&client,&client_addrlen);
	if (connfd<0)
	{
		printf("errno is : %d\n",errno);
	}
	else
	{
		char buffer[BUF_SIZE];
		memset(buffer,'\0',BUF_SIZE);
		while (recv(connfd,buffer,BUF_SIZE-1,0)>0){}
		close(connfd);
	}

	close(sock);
	return 0;

}
