#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 9999

int main(int argc, char *argv[])
{
	int client, i;
	struct sockaddr_in ser_addr, remaddr;
        socklen_t addrlen = sizeof(remaddr);
	char buff[1024] = {0};

	if (argc < 4) {
		printf("\n Not enough args passed udp_cli <your name> <remote name> \"<message>\"\n");
		return -1;
        }

	client = socket(AF_INET, SOCK_DGRAM, 0);
	if(client < 0) {
		perror("socket:");
		return -1;
	}

	memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(PORT);
	if (inet_aton("127.0.0.1", &remaddr.sin_addr)==0) {
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	for (i = 1; i <= argc - 1; i++) {
		strcat(buff, argv[i]);
                strcat(buff, " ");
	}

	if (sendto(client, buff, strlen(buff), 0, (struct sockaddr *)&remaddr, addrlen) < 0)
	perror("sendto");

        memset(buff,0,sizeof(buff));
	recvfrom(client, buff, sizeof(buff), 0, (struct sockaddr *)&remaddr, &addrlen);
	printf("\n%s\n",buff);

	close(client);

	return 0;
}

