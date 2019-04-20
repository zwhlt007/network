#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <netdb.h>

int main(int argc,char* argv[])
{
	if (argc<2)
	{
		printf("usage: %d ip_address port\n",basename(argv[0]));
		return 1;
	}
	
	char *host=argv[1];
	struct hostent* hostinfo=gethostbyname(host);
	assert(hostinfo);
	
	struct servent* servinfo=getservbyname("daytime","tcp");
	assert(servinfo);
	
	printf("daytime port is %d\n",ntohs(servinfo->s_port));

	
	struct sockaddr_in address;
	address.sin_family=AF_INET;
	address.sin_port=servinfo->s_port;
	address.sin_addr=*(struct in_addr*)*hostinfo->h_addr_list;
	
	int sock=socket(AF_INET,SOCK_STREAM,0);
	assert(sock>=0);
	
	if (connect(sock,(struct sockaddr*)&address,sizeof(address))<0)
	{
		printf("connection failed\n");
	}
	else
	{
		char buffer[128];
		int result=read(sock,buffer,sizeof(buffer));
		assert(result>0);
		buffer[result]='\0';
		printf("the daytime is : %s",buffer);
		
	}

	close(sock);
	return 0;

}
