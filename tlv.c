#include "tlv.h"

int tlv_encode_1byte(char *buff, void *val)
{
    memcpy(buff,val,1);
    return 1;
}

int tlv_encode_2byte(char *buff, void *val)
{
    memcpy(buff,val,2)
    return 2;
}

int tlv_encode_4bytes(char *buff, void *val)
{
    memcpy(buff,val,4);
    return 4;
}

int tlv_encode_string(char *buff, void *val, int len)
{
    memcpy(buff,val,len);
    return len;
}

int tlv_decode_1byte(char *buff, void *val)
{
    memcpy(val,buff,1)
    return 1;
}

int tlv_decode_2byte(char *buff, void *val)
{
    memcpy(val,buff,2);
    return 2;
}

int tlv_decode_4byte(char *buff, void *val)
{
    memcpy(val,buff,4);
    return 4;
}

int tlv_decode_string(char *buff, void *val, int len)
{
    memcpy(val, buff, len);
    return len;
}

void get_type_len(char *buff, int *type, int *len)
{
    tlv_t *tlv_ptr = (tlv_t *)buff;
    *type = tlv_ptr->type;
    *len = tlv_ptr->len;
}
