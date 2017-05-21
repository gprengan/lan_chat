#include "msg.h"
 
extern int server_socket;

int encode_client_add_response(char *buff, int buff_len, unsigned char status)
{
    int len = 0;
// sanity checks need to add
    memset(buff, 0, buff_len);
    len += msg_header_add(buff+len, MSG_CLIENT_ADD, HEADER_LEN);
    len += msg_header_add(buff+len, ELE_CLIENT_ACTION, ONE_BYTE);
    len += msg_encode_1byte(buff+len, &status);
    return len;
}
 
void msg_decode_server (char *buff, int buff_len, struct sockaddr_in *rem_addr)
{
    int type,len, status;
    type = len = 0;
    get_type_len(buff, &type, &len);
 
    buff += sizeof(msg_t)+len;
 
    switch(type)
    {
        case MSG_CLIENT_ADD:
            status = element_decode_client_add(buff,buff_len-sizeof(msg_t)+len, rem_addr);
            len = encode_client_add_response(buff, buff_len, status);
            break;
        case MSG_CLIENT_MESSAGE:
            len = element_decode_client_message(buff,
                           buff_len-sizeof(msg_t)+len, rem_addr);
   
             printf("\n in MSG_CLIENT_MESSAGE\n");
        break;
        default:
            printf("\n Type not found");
            break;
    }
    sendto(server_socket, buff, len, 0, (struct sockaddr *)rem_addr, sizeof(struct sockaddr));
}
