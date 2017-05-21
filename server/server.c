#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "msg.h"

#define PORT 9999

struct dbase_s {
    char nick_name[100];
    struct sockaddr_in cli_addr;
};

typedef struct dbase_s dbase_t;

dbase_t cli_dbase[10];
int dbase_count = 0;
int server_socket = 0;

void find_client_name_from_sock(struct sockaddr_in *rem_addr, char *name)
{
    int i;
    for (i = 0; i < 10; i++)
    {
        if (rem_addr->sin_port == cli_dbase[i].cli_addr.sin_port) {
            strncpy(name, cli_dbase[i].nick_name, strlen(cli_dbase[i].nick_name));
        }
    }
}

int forward_client_message(char *buff, client_t *client, struct sockaddr_in *rem_addr)
{
    int i;
    char name[100] = {0};
    find_client_name_from_sock(rem_addr, name);
    for (i = 0; i < 10; i++)
    {
        if(!strncmp(cli_dbase[i].nick_name, client->rem_name, sizeof(client->rem_name))) {
             rem_addr->sin_family = cli_dbase[i].cli_addr.sin_family;
             rem_addr->sin_addr.s_addr = cli_dbase[i].cli_addr.sin_addr.s_addr;
             rem_addr->sin_port = cli_dbase[i].cli_addr.sin_port;
              return msg_encode_client_buff(buff, name, client->msg_buff);
        }
    }
    return 0;
}

int add_new_client_entry(client_t *new_client, struct sockaddr_in *rem_addr)
{
    int i;

    for (i = 0; i < 10; i++)
    {
        if(!strncmp(cli_dbase[i].nick_name, new_client->nick_name, strlen(new_client->nick_name))) {
           printf("\n client rejected %s\n", new_client->nick_name);
            return REJECTED;
        }
        if (dbase_count < 10)
        {
            memset(cli_dbase[dbase_count].nick_name,0,sizeof(cli_dbase[dbase_count].nick_name));
            strcpy(cli_dbase[dbase_count].nick_name, new_client->nick_name);
            cli_dbase[dbase_count].cli_addr.sin_family = rem_addr->sin_family;
            cli_dbase[dbase_count].cli_addr.sin_addr.s_addr = rem_addr->sin_addr.s_addr;
            cli_dbase[dbase_count].cli_addr.sin_port = rem_addr->sin_port;
            dbase_count++;
            printf("\n clinet added %s %d\n", cli_dbase[dbase_count].nick_name, dbase_count);
            return REGISTERED;
        }
        return FULL;
    }
}

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
    server_socket = server;
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

        msg_decode_server (buff, len, &rem_addr);

    }

    close(server);
    return 0;

}
