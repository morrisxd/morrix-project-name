#ifndef __BP_UTIL_C__
#define __BP_UTIL_C__



/******************************************************************************
 * Function name:   WTI_GenerateRandomData ()
 * Description  :   Creates some random data by calling the rand() function to fill
 *                  in the packet's payload.
 *
 * Input  params:   Hex: buffer pointer to save the result.
 *                  pattern1: fixed pattern 1
 *                  pattern2: fixed pattern 2
 *                  rand_flag: indicating use random data or not.
 *                  max_size: maximum number of data we need.
 *                  min_size: mininum number of data we need.
 *                  
 * Output params: 
 * Return val   : none
 ******************************************************************************/
void WTI_GenerateRandomData (WP_CHAR * Hex, WP_U8 pattern1, WP_U8 pattern2, WP_U8 rand_flag, WP_U32 max_size, WP_U32 min_size)
{
    WP_U8 tmprand;
    WP_U32 i, datarandlength;

    if (max_size == min_size)
        datarandlength = max_size;
    else if (min_size < max_size)
        datarandlength = min_size + (rand () % (max_size - min_size));
    else
        datarandlength = max_size + (rand () % (min_size - max_size));

    for (i = 0; i < datarandlength; i++)
    {

        if (rand_flag)
            tmprand = rand () % 256;    /* random numer 0-256 */
        else
        {
            if (i % 2 == 0)
                tmprand = pattern1;
            else
                tmprand = pattern2;
        }

        Hex[i] = tmprand;
    }
}

/******************************************************************************
 * Function name:   FlowCtrlDataUnitSetup ()
 * Description  :   Assembling the packet's content of flow control.
 *
 * Input  params:   data_unit: pointer to the buffer 
 *                  km_tx_buff: pointer to the buffer inside
 *                  km_data_ptr: pointer to the buffer inside
 * Output params: 
 * Return val   : none
 ******************************************************************************/
void FlowCtrlDataUnitSetup (WP_data_unit * data_unit, WP_data_segment * km_tx_buf, WP_U8 * km_data_ptr)
{
    WP_U32 header_size;
    WP_CHAR ascii_buffer[MAX_DATA_LENGTH * 2 + 1];
    WP_CHAR hexa_buffer[MAX_DATA_LENGTH * 2];

    header_size = 0;

    /* Prepare packet for Host Send  */
    data_unit->data_size = MAX_DATA_LENGTH;
    data_unit->n_active = 1;

    km_tx_buf->data_size = data_unit->data_size;
    km_data_ptr = WP_PoolAlloc (pool_144);
    km_tx_buf->pool_handle = pool_144;
    km_tx_buf->data = km_data_ptr;

    km_tx_buf->next = NULL;
    km_tx_buf->displacement = 0;

    data_unit->segment = (km_tx_buf);
    data_unit->n_segments = 1;
    data_unit->type = WP_DATA_ENET;
    data_unit->control = WP_HT_CONTROL (0);

    memset (ascii_buffer, 0, MAX_DATA_LENGTH * 2 + 1);
    memset (hexa_buffer, 0, MAX_DATA_LENGTH * 2);
// -------------   CBFC message -----------------
    strcat (ascii_buffer, "0180c2000001");
    strcat (ascii_buffer, "ab12cd34ef56");
    strcat (ascii_buffer, "8808");

    strcat (ascii_buffer, "0101");

#if 0
    strcat (ascii_buffer, "00ff");  /* CBFC vector */
#else
    if (0x01 == user_channel)
    {
        strcat (ascii_buffer, "0001");  /* CBFC vector, pause the first Q */
    }
    else if (0x03 == user_channel)
    {
        strcat (ascii_buffer, "0003");  /* CBFC vector, pause the first 2 Qs */
    }
    else if (0x07 == user_channel)
    {
        strcat (ascii_buffer, "0007");  /* CBFC vector, pause the first 3 Qs */
    }
    else 
    {
        strcat (ascii_buffer, "00ff");  /* CBFC vector, pause ALL 8 Qs */
    }
#endif

    strcat (ascii_buffer, "ffff");  /* CBFC  class 0 */
    strcat (ascii_buffer, "ffff");  /* CBFC  class 1 */
    strcat (ascii_buffer, "ffff");  /* CBFC  class 2 */
    strcat (ascii_buffer, "ffff");  /* CBFC  class 3 */
    strcat (ascii_buffer, "ffff");  /* CBFC  class 4 */
    strcat (ascii_buffer, "ffff");  /* CBFC  class 5 */
    strcat (ascii_buffer, "ffff");  /* CBFC  class 6 */
    strcat (ascii_buffer, "ffff");  /* CBFC  class 7 */

    strcat (ascii_buffer, "00000000");  /* Padding 28 */
    strcat (ascii_buffer, "00000000");
    strcat (ascii_buffer, "00000000");
    strcat (ascii_buffer, "00000000");
    strcat (ascii_buffer, "00000000");
    strcat (ascii_buffer, "00000000");
    strcat (ascii_buffer, "00000000");

    data_unit->data_size = 62;

    km_tx_buf->data_size = data_unit->data_size;

    App_TranslateAsciiToHex ((WP_CHAR *) hexa_buffer, (WP_CHAR *) ascii_buffer, data_unit->data_size);

    WP_MEM_BYTES_FILL (km_data_ptr, '0', buffer_data_144[0].size);
    WP_MEM_BYTES_SET (km_data_ptr, (WP_U8 *) hexa_buffer, MAX_DATA_LENGTH);

// #if DEBUG
#if 1
    {
    WP_U32 ii = 0;
    printf ("packet : %x \n ", (WP_U32) km_data_ptr);

    for (ii = 0; ii < data_unit->segment->data_size; ii++)
    {
        if ((ii) % 4 == 0)
            printf ("\n");
        printf ("%2.2x", data_unit->segment->data[ii]);
    }
    printf ("( %d bytes )\n", data_unit->segment->data_size);
    }
#endif
}

/******************************************************************************
 * Function name:   DataUnitSetup ()
 * Description  :   Assembling the content of the normal UDP data packet.
 *
 * Input  params:   data_unit: pointer to the buffer to save the result. 
 *                  km_tx_buf: pointer to the buffer inside
 *                  src_mac: source mac address
 *                  dst_mac: destination mac address
 *                  ip_src: source ip address
 *                  ip_dst: destination ip address
 *                  payload_size: size of the payload
 *                  channel_index: index of the one of the 8 channles
 *                  packet_index: index of the packets
 *
 * Output params: 
 * Return val   : none
 ******************************************************************************/
void DataUnitSetup (WP_data_unit * data_unit,
                    WP_data_segment * km_tx_buf,
                    WP_U8 * km_data_ptr, WP_CHAR * dst_mac, WP_CHAR * src_mac, WP_CHAR * ip_protocol, WP_CHAR * ip_src, WP_CHAR * ip_dst, WP_U32 payload_size, WP_U8 channel_index, WP_U8 packet_index)
{
    WP_U32 header_size;
    WP_CHAR ascii_buffer[MAX_DATA_LENGTH * 2 + 1];
    WP_CHAR hexa_buffer[MAX_DATA_LENGTH * 2];

    /* set the ip header the packet */
    WP_U8 temp_ip_header_hex[100];
    WP_U8 temp_ip_header_char[100];
    WP_U16 checksum;
    WP_U16 ip_length;
    WP_CHAR ip_length_char[10];
    
    memset (ascii_buffer, 0, MAX_DATA_LENGTH * 2 + 1);
    memset (hexa_buffer, 0, MAX_DATA_LENGTH * 2);

    /* Prepare packet for Host Send  */
    data_unit->data_size = MAX_DATA_LENGTH;
    data_unit->n_active = 1;

    km_tx_buf->data_size = data_unit->data_size;
    km_data_ptr = WP_PoolAlloc (pool_144);
    km_tx_buf->pool_handle = pool_144;
    km_tx_buf->data = km_data_ptr;

    km_tx_buf->next = NULL;
    km_tx_buf->displacement = 0;

    data_unit->segment = (km_tx_buf);
    data_unit->n_segments = 1;
    data_unit->type = WP_DATA_ENET;
    data_unit->control = WP_HT_CONTROL (0);

    /* set the src and dst mac address at the packet */
    strcpy (ascii_buffer, dst_mac);
    strcat (ascii_buffer, src_mac);

    /* set the ethernet type at the packet */
    strcat (ascii_buffer, "0800");
    ip_length = WT_IPV4_HEADER_SIZE + WT_L4_HEADER_SIZE + payload_size;
    sprintf (ip_length_char, "%04x", ip_length);    /* Total Length */

    sprintf ((WP_CHAR *) temp_ip_header_char, "45");    /* Version = Ipv4,  IHL = 5 */
    strcat ((WP_CHAR *) temp_ip_header_char, "80"); /* Type Of Service - 8 bit */
    strcat ((WP_CHAR *) temp_ip_header_char, ip_length_char);   /* Total Length */
    strcat ((WP_CHAR *) temp_ip_header_char, "00000000");   /* Id, Flags, frag */
    strcat ((WP_CHAR *) temp_ip_header_char, "0A"); /* TTL = 0x0a, */
    strcat ((WP_CHAR *) temp_ip_header_char, ip_protocol);  /* ip_protocol - 8 bits */
    strcat ((WP_CHAR *) temp_ip_header_char, "0000");   /* IP header Checksum - updated later */

    strcat ((WP_CHAR *) temp_ip_header_char, ip_src);   /* IP src address */
    strcat ((WP_CHAR *) temp_ip_header_char, ip_dst);   /* IP dst address */

    App_TranslateAsciiToHex ((WP_CHAR *) temp_ip_header_hex, (WP_CHAR *) temp_ip_header_char, WT_IPV4_HEADER_SIZE);
    checksum = WTI_IpChecksumCalc (temp_ip_header_hex, WT_IPV4_HEADER_SIZE);
    temp_ip_header_hex[10] = checksum >> 8;
    temp_ip_header_hex[11] = checksum & 0xff;
    WTI_TranslateHexToAscii ((WP_CHAR *) temp_ip_header_char, (WP_CHAR *) temp_ip_header_hex, WT_IPV4_HEADER_SIZE);

    strcat (ascii_buffer, (WP_CHAR *) temp_ip_header_char);

    header_size = strlen (ascii_buffer) / 2;
    l2_l3_size[channel_index][packet_index] = header_size;
#if DEBUG
    printf ("\nHOST SEND DATA: l2_l3_size=%d, payload_size=%d, channel_index=%d ", l2_l3_size[channel_index][packet_index], payload_size, channel_index);
#endif
    App_TranslateAsciiToHex (hexa_buffer, ascii_buffer, header_size);

    /* Src and Dst ports */
    hexa_buffer[header_size + 0] = IWF_Table[channel_index][2] >> 8;
    hexa_buffer[header_size + 1] = IWF_Table[channel_index][2] % 0xFF;
    hexa_buffer[header_size + 2] = IWF_Table[channel_index][3] >> 8;
    hexa_buffer[header_size + 3] = IWF_Table[channel_index][3] % 0xFF;
    header_size += WT_L4_HEADER_SIZE;

    WTI_GenerateRandomData (&hexa_buffer[header_size], channel_index, packet_index, 0, payload_size, payload_size);

    WP_MEM_BYTES_FILL (km_data_ptr, '0', buffer_data_144[0].size);
    WP_MEM_BYTES_SET (km_data_ptr, (WP_U8 *) hexa_buffer, MAX_DATA_LENGTH);

    data_unit->data_size = header_size + payload_size;
    km_tx_buf->data_size = data_unit->data_size;

#if DEBUG
    printf ("packet : %x \n ", (WP_U32) km_data_ptr);
    WP_U32 ii = 0;

    for (ii = 0; ii < data_unit->segment->data_size; ii++)
    {
        if ((ii) % 4 == 0)
            printf ("\n");
        printf ("%2.2x", data_unit->segment->data[ii]);
    }
    printf ("( %d bytes )\n", data_unit->segment->data_size);
#endif
}


/******************************************************************************
 * Function name:   terminate_on_error ()
 * Description  :   Quit the program and reboot the wds3 board if error occurs.
 *
 * Input  params:   handle: handle to decide if error rises.
 *                  line:   line number of the source code when error rises.
 * Output params: 
 * Return val   : none
 ******************************************************************************/
static void terminate_on_error (WP_handle handle, WP_CHAR * s, WP_U32 line)
{
    if (WP_ERROR_P (handle))
    {
        printf ("%s: Terminal error (handle 0x%x) %s\n", appname, handle, s);
        printf ("%s: Error type - %s\n", appname, WP_ErrorString (handle));
        printf ("line = %d\n", line);
        printf ("Test Failed\n");

        WP_DriverRelease ();
        WT_Reboot ();
        exit (0);
    }
}

/******************************************************************************
 * Function name:   WTI_IpChecksumCalc ()
 * Description  :   Work out the check sum of the packet pointed by *buff
 *
 * Input  params:   buff: pointer to the buffer to save the packet content. 
 *                  len: length of the dat inside the buffer
 * Output params: 
 * Return val   : none
 ******************************************************************************/
WP_U16 WTI_IpChecksumCalc (WP_U8 * buff, WP_U16 len)
{
    WP_U16 bu16;
    WP_U32 sum = 0;
    WP_U32 i;

    /* make 16 bit words out of every two adjacent
       8 bit words in the packet and add them up */
    len = len / 2;
    for (i = 0; i < len; i++)
    {
        bu16 = (buff[2 * i] << 8) + buff[2 * i + 1];
        sum = sum + (WP_U32) bu16;
    }

    /* take only 16 bits out of the 32 bit sum
       and add up the carries */
    while (sum >> 16)
        sum = (sum & 0xFFFF) + (sum >> 16);

    /* one's complement the result */
    sum = ~sum;

    return ((WP_U16) sum);
}

/******************************************************************************
 * Function name:   WTI_TranslateHexToAscii ()
 * Description  :   Convert data in the buffer from binary data to ascii characters
 *
 * Input  params:   Ascii: buffer to save the result
 *                  Hex: buffer to save the data in binary format
 *                  length: length of the data to be converted.
 * Output params: 
 * Return val   : none
 ******************************************************************************/
void WTI_TranslateHexToAscii (WP_CHAR * Ascii, WP_CHAR * Hex, WP_U32 length)
{
    WP_S32 i;
    WP_CHAR temp_num;

    for (i = 0; i < length; i++)
    {
        temp_num = (Hex[i] & 0xf0) >> 4;
        if (temp_num < 10)
            Ascii[2 * i] = temp_num + '0';
        else
            Ascii[2 * i] = temp_num + 'a' - 10;

        temp_num = Hex[i] & 0x0f;
        if (temp_num < 10)
            Ascii[2 * i + 1] = temp_num + '0';
        else
            Ascii[2 * i + 1] = temp_num + 'a' - 10;

    }

    Ascii[2 * length] = NULL;

    return;
}


/******************************************************************************
 * Function name: App_Quit()
 * Description  :      
 *
 *
 * Input  params: none
 * Output params: 
 * Return val   : none
 ******************************************************************************/

void App_Reboot(void)
{
#if !defined(__linux__)
/* 
 * Rebooting is specific to board and target.  The test
 * directory should not be concerned how this is done.
 * Instead, it calls a board-specific function. 
 */
   if (WPI_REBOOT_ENABLE) {
      printf("Test Rebooting winmon by App_Reboot\nWinMon>\n");
      WPX_Reboot();
   }
   else
      printf("Not Rebooting with App_Reboot\nWinMon>\n");
#endif
}

void App_Quit(WP_U32 status)
{
   WP_DriverRelease();
   
   if (status)
   {
      App_Reboot();
   }   
   
   exit(0);
}





#endif


