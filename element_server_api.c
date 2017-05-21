#include "msg.h"

void element_decode_client_add (char *buff, int p_len)
{
    int len = 0;
    int type, elem_len,i;

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
    /*TODO add api for client entry*/
    return;
}
