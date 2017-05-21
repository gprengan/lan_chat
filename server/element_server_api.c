#include "msg.h"

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


int element_decode_client_add (char *buff, int p_len, struct sockaddr_in *rem_addr)
{
    int len = 0;
    int type, elem_len,i;
    client_t client;

    memset(&client, 0, sizeof(client));

    while(buff+len < buff+p_len)
    {
        len += get_type_len(buff+len, &type, &elem_len);

        if (buff+len > buff+p_len) {
            printf("\n ERROR in %s", __FUNCTION__);
            return;
        }

        switch (type)
        {
            case ELE_CLIENT_ACTION:
                len += msg_decode_1byte(buff+len,
                    (void *) &client.status);
            break;

            case ELE_CLIENT_NAME:
                memset(client.nick_name,0,sizeof(client.nick_name));
                len += msg_decode_string(buff+len,
                        (void *)&client.nick_name,
                        elem_len);
            break;

            default:
                len += sizeof(msg_t)+elem_len;
                printf("\nIN FUN:%s INVALID TYPE",__FUNCTION__);

        }
    }

    printf("\nACTION:%d NAME:%s\n",client.status, client.nick_name);
    if (client.status == ADD)
        return add_new_client_entry(&client, rem_addr);
}

int element_decode_client_message(char *buff, int p_len, struct sockaddr_in *rem_addr)
{
    int len = 0;
    int type, elem_len,i;
    client_t client;

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
                printf("\nIN FUN:%s INVALID TYPE",__FUNCTION__);

        }
    }
    return forward_client_message(buff, &client, rem_addr);
}
