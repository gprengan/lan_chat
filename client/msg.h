#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define HEADER_LEN  0
#define ONE_BYTE    1
#define TWO_BYTES   2
#define FOUR_BYTES  4


typedef struct msg_s
{
    int type;
    int len;
}msg_t;

typedef struct client_s
{
    char nick_name[100];
    char rem_name[100];
    char msg_buff[2048];
    int status;
    int cli_socket;
    struct sockaddr_in remaddr;
    socklen_t addrlen;
}client_t;


typedef enum {
    ADD = 1,
    DELETE,
    REGISTERED,
    REJECTED,
    FULL,
}ACTIONs;

typedef enum {
    MSG_CLIENT_ADD = 1,
    MSG_CLIENT_DELETE,
    MSG_CLIENT_MESSAGE,
    MSG_KEEP_ALIVE,
// Add the new message above MSG_MAX
    MSG_MAX,
}MSGs;

typedef enum {
    ELE_CLIENT_NAME = 1,
    ELE_CLIENT_ACTION,
    ELE_CLIENT_MSG_BUFF,
}ELEMENTs;

int msg_encode_1byte(char *buff, void *val);
int msg_encode_2byte(char *buff, void *val);
int msg_encode_4bytes(char *buff, void *val);
int msg_encode_string(char *buff, void *val, int len);

int msg_decode_1byte(char *buff, void *val);
int msg_decode_2byte(char *buff, void *val);
int msg_decode_4byte(char *buff, void *val);
int msg_decode_string(char *buff, void *val, int len);

int msg_header_add(char *buff, int type, int len);
int get_type_len(char *buff, int *type, int *len);

void msg_decode_server (char *buff, int buff_len);
void element_decode_client_add (char *buff, int msg_len);

extern struct client_s client;

int msg_encode_add_client (char *buff, char *name);
void msg_decode_client (char *buff, int buff_len);

void element_decode_client_add_response(char *buff, int p_len);

int msg_encode_client_buff (char *buff, char *name, char *msg);
void element_decode_client_message(char *buff, int p_len);
