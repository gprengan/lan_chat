#include "msg.h"

extern int server_socket;

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

        break;
        default:
            printf("\n Type not found in function:%s",__FUNCTION__);
        break;
    }
    if (len != 0)
        sendto(server_socket, buff, len, 0, (struct sockaddr *)rem_addr, sizeof(struct sockaddr));
}
