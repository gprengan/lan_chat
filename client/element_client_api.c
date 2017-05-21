#include "msg.h"

int msg_encode_add_client (char *buff, char *name)
{
    char val;
    int len = 0;

    len += msg_header_add(buff+len, MSG_CLIENT_ADD, HEADER_LEN);

    len += msg_header_add(buff+len, ELE_CLIENT_ACTION, ONE_BYTE);
    val = ADD;
    len += msg_encode_1byte(buff+len, &val);

    len += msg_header_add(buff+len, ELE_CLIENT_NAME, strlen(name));
    len += msg_encode_string(buff+len, name, strlen(name));

    return len;
}

int msg_encode_client_buff (char *buff, char *name, char *msg)
{
    int len = 0;

    len += msg_header_add(buff+len, MSG_CLIENT_MESSAGE, HEADER_LEN);

    len += msg_header_add(buff+len, ELE_CLIENT_NAME, strlen(name));
    len += msg_encode_string(buff+len, name, strlen(name));

    len += msg_header_add(buff+len, ELE_CLIENT_MSG_BUFF, strlen(msg));
    len += msg_encode_string(buff+len, msg, strlen(msg));

    return len;
}

void element_decode_client_add_response(char *buff, int p_len)
{
    int len = 0;
    int type, elem_len,i;

    while(buff+len < buff+p_len)
    {
        len += get_type_len(buff+len, &type, &elem_len);

    if (buff+len > buff+p_len) {
        printf("\n ERROR in fun:%s",__FUNCTION__);
        return;
    }

        switch (type)
        {
            case ELE_CLIENT_ACTION:
                len += msg_decode_1byte(buff+len,
                    (void *) &client.status);
            break;

            default:
                len += sizeof(msg_t)+elem_len;
                printf("\nIN FUN:%s INVALID TYPE",__FUNCTION__);

        }
    }
    return;
}

void element_decode_client_message(char *buff, int p_len)
{
    int len = 0;
    int type, elem_len,i;

    while(buff+len < buff+p_len)
    {
        len += get_type_len(buff+len, &type, &elem_len);

        if (buff+len > buff+p_len) {
            printf("\n ERROR in fun:%s",__FUNCTION__);
            return;
        }

        switch (type)
        {
            case ELE_CLIENT_NAME:
                memset(&client.rem_name, 0, sizeof(client.rem_name));
                len += msg_decode_string(buff+len,
                    (void *) &client.rem_name,elem_len);
            break;

            case ELE_CLIENT_MSG_BUFF:
                memset(&client.msg_buff, 0, sizeof(client.msg_buff));
                len += msg_decode_string(buff+len,
                    (void *) &client.msg_buff,elem_len);
            break;

            default:
                len += sizeof(msg_t)+elem_len;
                printf("\nIN FUN:%s INVALID TYPE:%d LEN:%d",__FUNCTION__
                          , type, len);

        }
    }
    return;
}

