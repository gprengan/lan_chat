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

void get_first_arg(char *buff1, char *buff)
{
	char ch;
	int i = 0;
	ch = buff[0];
	while(ch && ch != ' ')
	{
		buff1[i] = buff[i];
		i++;
		ch = buff[i];
	}
}

void get_second_arg(char *buff1, char *buff)
{
	char ch;
	int i = 0, j = 0;
        ch = buff[0];
	while(ch && ch != ' ')                                                                                  
        {                                                                                                       
		buff1[i] = buff[i];                                                                             
		i++;                                                                                            
		ch = buff[i];                                                                                   
        }                                                                                                       
	j = 0; i++;
	ch = buff[i];
	while(ch && ch != ' ')                                                                                  
	{                                                                                                       
		buff1[j] = buff[i];                                                                             
		i++; j++;
		ch = buff[i];                                                                                   
	}
}   

void remove_two_args(char *buff)
{
	char ch;
	int i = 0, j = 0;;
	ch = buff[0];
	do {
		if(ch && ch == ' ')
			i++;
		j++;
		ch = buff[j];
        } while(i<=1);
	memcpy(buff, buff+j, sizeof(buff));	
}

int find_rem_cli(char *buff)
{
	int i;                                                                                                  
	char nick_name[100];                                                                                    

	memset(nick_name,0,sizeof(nick_name));                                                                  
	get_second_arg(nick_name,buff);                                                                          

	for(i = 0; i < 10; i++)                                                                                 
	{                                                                                                       
		if(!strcmp(cli_dbase[i].nick_name,nick_name))                                                   
			return i;                                                                               
	}
	return -1;
}

int find_cli_dbase(struct sockaddr_in *rem_addr, char *buff)
{
	int i;
	char nick_name[100];

	memset(nick_name,0,sizeof(nick_name));
	get_first_arg(nick_name,buff);
printf("\n in fun:%s L:%d nick_name:%s buff:%s",__FUNCTION__,__LINE__,nick_name,buff);

	for(i = 0; i < 10; i++)
        {
		if(!strcmp(cli_dbase[i].nick_name,nick_name))
			return i;
	}
        if (dbase_count < 10) {                                                                                 
                get_first_arg(nick_name,buff);                                                                  
                memset(cli_dbase[dbase_count].nick_name,0,sizeof(cli_dbase[dbase_count].nick_name));            
                strcpy(cli_dbase[dbase_count].nick_name,nick_name);                                             
                cli_dbase[dbase_count].cli_addr.sin_family = rem_addr->sin_family;                              
                cli_dbase[dbase_count].cli_addr.sin_addr.s_addr = rem_addr->sin_addr.s_addr;                    
                cli_dbase[dbase_count].cli_addr.sin_port = rem_addr->sin_port;                                  
                dbase_count++;                                                                                  
        } else {                                                                                                
                return -1;                                                                                      
        }          
	return 0;
}

int main()
{
	int server, ret;
	struct sockaddr_in ser_addr, rem_addr;
	char buff[1024] = {0};
	socklen_t addrlen = sizeof(rem_addr);
        char *rem, space = ' ';
	
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
		recvfrom(server, buff, sizeof(buff), 0, (struct sockaddr *)&rem_addr, &addrlen);
		printf("\n%s\n",buff);
	
printf("\n L:%d buff:%s",__LINE__,buff);
		ret = find_cli_dbase(&rem_addr, buff);
printf("\n ret:%d",ret);		
		if(ret < 0){
			memset(buff,0,sizeof(buff));
			sprintf(buff,"Nick name not found");
			if (sendto(server, buff, strlen(buff), 0, (struct sockaddr *)&rem_addr, addrlen) < 0)
			perror("sendto");
		} else {
			ret = find_rem_cli(buff);
				if (ret < 0) {
					memset(buff,0,sizeof(buff));
					sprintf(buff,"Remote user not found");
					if (sendto(server, buff, strlen(buff), 0, (struct sockaddr *)&rem_addr, addrlen) < 0)
						perror("sendto");
			} else {
				remove_two_args(buff);
				if (sendto(server, buff, strlen(buff), 0, (struct sockaddr *)&cli_dbase[ret].cli_addr, addrlen) < 0)
					perror("sendto");
			}
		}
        }
	
	close(server);
	return 0;
}

