#include "tlv.h"

void tlv_decode_client_add (char *buff, int msg_len)
{
    int len = 0, tlv_len = 0;
    int type;
    client_add_t new_client;

    get_type_len(buff, &type, &tlv_len);

    while(buff+len < buff+msg_len)
    {

        switch (type)
        {
            case TLV_CLIENT_ADD:
                len += tlv_decode_1byte(buff+len+tlv_len,
                    (void *) &new_client.action);
            break;

            case TLV_CLIENT_NAME:
                len += tlv_decode_string(buff+len+tlv_len,
                        (void *)&new_client.nick_name,
                        tlv_len);
            break;

            case TLV_CLIENT_MESSAGE:
                len += tlv_decode_string(buff+len+tlv_len,
                        (void *)&new_client.msg,
                        tlv_len);
            default:
                printf("IN FUN:%s INVALID TYPE",__FUNCTION__);

        }
        get_type_len(buff, &type, &tlv_len);
    }
    return;
}
