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
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

static const char* status_line[2]={"200 OK","500 Internal server error"};

int main(int argc,char* argv[])
{
	if (argc<=3)
	{
		printf("usage: %d ip_address port filename\n",basename(argv[0]));
		return 1;
	}
	
	const char* ip=argv[1];
	int port=atoi(argv[2]);
	const char* file_name=argv[3];
	
	struct sockaddr_in address;
	bzero(&address,sizeof(address));
	address.sin_family=AF_INET;
	inet_pton(AF_INET,ip,&address.sin_addr);
	address.sin_port=htons(port);
	
	int sock=socket(PF_INET,SOCK_STREAM,0);
	assert(sock>=0);
	
	
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
		char header_buf[BUFFER_SIZE];
		memset(header_buf,'\0',BUFFER_SIZE);
		char* file_buf;
		struct stat file_stat;
		
		bool valid=true;
		int len=0;
		if (stat(file_name,&file_stat)<0)
		{
			valid=false;
		}

		close(connfd);
	}

	close(sock);
	return 0;

}
