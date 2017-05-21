typedef struct tlv_s
{
    int type;
    int len;
}tlv_t;

typedef struct client_add_s
{
    char nick_name[100];
    char msg[1024];
    int action;
}client_add_t;

typedef enum {
    ADD = 1,
    DELETE,
}ACTIONs;

typedef enum {
    TLV_CLIENT_ADD = 1,
    TLV_CLIENT_NAME,
    TLV_CLIENT_MESSAGE,
    TLV_CLIENT_ADD_PAYLOAD,

    TLV_CLIENT_NICK_NAME,
    TLV_CLIENT_MESSAGE,
    TLV_CLIENT_MESSAGE_PAYLOAD,
}TLVs;
