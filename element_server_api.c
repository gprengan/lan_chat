#include "msg.h"

void element_decode_client_add (char *buff, int p_len)
{
    int len = 0;
    int type, msg_len,i;
    client_add_t new_client;

    while(buff+len < buff+p_len)
    {
        len += get_type_len(buff+len, &type, &msg_len);

	if (buff+len > buff+p_len) {
	printf("\n ERROR");
            return;
	}

        switch (type)
        {
            case ELE_CLIENT_ACTION:
                len += msg_decode_1byte(buff+len,
                    (void *) &new_client.action);
            break;

            case ELE_CLIENT_NAME:
                memset(new_client.nick_name,0,sizeof(new_client.nick_name));
                len += msg_decode_string(buff+len,
                        (void *)&new_client.nick_name,
                        msg_len);
            break;

            default:
                len += sizeof(msg_t)+msg_len;
                printf("\nIN FUN:%s INVALID TYPE",__FUNCTION__);

        }
    }
    printf("\nACTION:%d NAME:%s\n",new_client.action, new_client.nick_name);
    /*TODO add api for client entry*/
    return;
}
