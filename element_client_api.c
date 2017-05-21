#include "msg.h"

int msg_encode_add_client (char *buff, char *name)
{
	char val;
	int len = 0;

	len += msg_header_add(buff+len, MSG_CLIENT_ADD, HEADER_LEN);

	len += msg_header_add(buff+len, ELE_CLIENT_ACTION, 1);
	
	val = ADD;
	len += msg_encode_1byte(buff+len, &val);

	len += msg_header_add(buff+len, ELE_CLIENT_NAME, strlen(name));
	len += msg_encode_string(buff+len, name, strlen(name));

	return len;
}

void element_decode_client_add_response(char *buff, int p_len)
{
    int len = 0;
    int type, msg_len,i;

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
                    (void *) &client.status);
            break;

            default:
                len += sizeof(msg_t)+msg_len;
                printf("\nIN FUN:%s INVALID TYPE",__FUNCTION__);

        }
    }
    printf("\n %s ACTION:%d", __FUNCTION__, client.status);
    
    
    return;
}

