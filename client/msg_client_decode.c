#include "msg.h"

void msg_decode_client (char *buff, int buff_len)
{
    int type,len;
    type = len = 0;
    get_type_len(buff, &type, &len);

    buff += sizeof(msg_t)+len;

    switch(type)
    {
        case MSG_CLIENT_ADD:
            element_decode_client_add_response(buff,
                           buff_len-sizeof(msg_t)+len);
        break;

        case MSG_CLIENT_MESSAGE:
            element_decode_client_message(buff,
                           buff_len-sizeof(msg_t)+len);
        break;

        default:
            printf("\n Type not found");
        break;
    }
    return;
}
