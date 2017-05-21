#include "msg.h"

int msg_header_add(char *buff, int type, int len)
{
    msg_t msg;

    msg.type = type;
    msg.len = len;

    memcpy(buff, &msg, sizeof(msg));

    return sizeof(msg);
}

int msg_encode_1byte(char *buff, void *val)
{
    memcpy(buff,val,1);
    return 1;
}

int msg_encode_2byte(char *buff, void *val)
{
    memcpy(buff,val,2);
    return 2;
}

int msg_encode_4bytes(char *buff, void *val)
{
    memcpy(buff,val,4);
    return 4;
}

int msg_encode_string(char *buff, void *val, int len)
{
    memcpy(buff,val,len);
    return len;
}

int msg_decode_1byte(char *buff, void *val)
{
    memcpy(val,buff,1);
    return 1;
}

int msg_decode_2byte(char *buff, void *val)
{
    memcpy(val,buff,2);
    return 2;
}

int msg_decode_4byte(char *buff, void *val)
{
    memcpy(val,buff,4);
    return 4;
}

int msg_decode_string(char *buff, void *val, int len)
{
    memcpy(val, buff, len);
    return len;
}

int get_type_len(char *buff, int *type, int *len)
{
    msg_t *msg_ptr = (msg_t *)buff;
    *type = msg_ptr->type;
    *len = msg_ptr->len;
    return (sizeof(msg_t));
}

