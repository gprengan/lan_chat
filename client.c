#include "msg.h"
#include"pthread.h"

#define PORT 9999

client_t client;

void set_client_socket_parms(void)
{
    memset(&client, 0, sizeof(client_t));
    client.cli_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if(client.cli_socket < 0) {
        perror("socket:");
        exit(1);
    }

    memset((char *) &client.remaddr, 0, sizeof(client.remaddr));

    client.remaddr.sin_family = AF_INET;
    client.remaddr.sin_port = htons(PORT);

    if (inet_aton("127.0.0.1", &client.remaddr.sin_addr)==0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    client.addrlen = sizeof(client.remaddr);
}

void *receiveMessage(void *arg) {
    int len;
    char buff[1024];

    while(1) {
        memset(buff,0,sizeof(buff));
        len = recvfrom(client.cli_socket, buff, sizeof(buff), 0,
               (struct sockaddr *)&client.remaddr, &client.addrlen);

    /*TODO forward to decode routine and print the message*/
    }
}

int main()
{

    char buff[1024] = {0};
    char msg[1024] = {0};
    char name[100] = {0};
    int ret, len = 0;
    pthread_t rThread;

    set_client_socket_parms();

    /* send the client name to server for registration */
    printf("\n Set your name for registration:");
    memset(name, 0, sizeof(name));
    scanf("%s",name);

    memset(buff, 0, sizeof(buff));
    len = msg_encode_add_client (buff, name);

    if (sendto(client.cli_socket, buff, len, 0,
          (struct sockaddr *)&client.remaddr, client.addrlen) < 0)
        perror("sendto");

    /* TODO check the client registered in server */

    while(1)
    {
        memset(buff, 0, sizeof(buff));
        len = recvfrom(client.cli_socket, buff, sizeof(buff), 0,
               (struct sockaddr *)&client.remaddr, &client.addrlen);

        /* msg_decode_client will redirect to proper function */
        msg_decode_client(buff, len);

        if(client.status == REGISTERED) {
            printf("\n CLIENT REGISTERED in server");
            break;
        } else if(client.status == FULL) {
            printf("\n No space for new client in server");
            exit(0);
        } else if (client.status == REJECTED) {
        /* send the client name to server for registration */
            printf("\n Set your name for registration:");
            memset(name, 0, sizeof(name));
            scanf("%s",name);

            memset(buff, 0, sizeof(buff));
            /* encode the client add payload with new name */
            len = msg_encode_add_client (buff, name);

            if (sendto(client.cli_socket, buff, len, 0,
                 (struct sockaddr *)&client.remaddr, client.addrlen) < 0)
                perror("sendto");

        } else {
            printf("\n Invalid STATUS from server");
        }
    }


    //creating a new thread for receiving messages from the server after registration
    ret = pthread_create(&rThread, NULL, receiveMessage, (void *) NULL);
    if (ret) {
        printf("ERROR: Return Code from pthread_create() is %d\n", ret);
        exit(1);
    }

    while(1)
    {
        printf("\nTo:");
        memset(name, 0, sizeof(name));
        fgets(name, sizeof(name), stdin);

        printf("\n Message:");
        memset(msg, 0, sizeof(msg));

    /*TODO enode name and message and sendto server */

    }
    close(client);

    return 0;
}
