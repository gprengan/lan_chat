#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9999

struct dbase_s {
	char nick_name[100];
	struct sockaddr_in cli_addr;
};

typedef struct dbase_s dbase_t;

dbase_t cli_dbase[10];
int dbase_count = 0;

int main()
{
	int server, ret, len;
	struct sockaddr_in ser_addr, rem_addr;
	char buff[1024] = {0};
	char buff1[1024] = {0};
	socklen_t addrlen = sizeof(rem_addr);
	
	server = socket(AF_INET, SOCK_DGRAM, 0);
	if(server < 0) {
		perror("socket:");
		return -1;
	}
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = INADDR_ANY;
	ser_addr.sin_port = htons( PORT );
	
	
	if(bind(server, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) < 0) {
		perror("bind:");
		return -1;
	}
        memset((char *) &rem_addr, 0, sizeof(rem_addr));

	while(1)
	{

		memset(buff,0,sizeof(buff));
		len = recvfrom(server, buff, sizeof(buff), 0, (struct sockaddr *)&rem_addr, &addrlen);

		memset(buff1,0,sizeof(buff));
		len = sprintf(buff1,"echo message:%s",buff);

		if (sendto(server, buff1, len, 0, (struct sockaddr *)&rem_addr, addrlen) < 0)
                        perror("sendto");
	}

	close(server);
	return 0;
	
}
