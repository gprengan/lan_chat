void tlv_decode_server (char *buff, int buff_len)
{
    int type,len;
    type = len = 0;
    get_type_len(buff, &type, &len);

    if(len == 0)
        buff = buff+sizeof(tlv_t);
    else
        buff = buff+sizeof(int)+len;

    switch(type)
    {
        case TLV_CLIENT_ADD_PAYLOAD:
            tlv_decode_client_add(buff,buff_len-sizeof(int)+len);
            break;
        default:
            printf("\n Type not found");
            break;
    }
    return;
}
