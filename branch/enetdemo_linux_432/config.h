#if 0
#define INGRESS_PORT	WP_PORT_ENET5	// IXIA->wds3
#define EGRESS_PORT	WP_PORT_ENET4	// wds3->IXIA
#else
#define INGRESS_PORT	WP_PORT_ENET8
#define EGRESS_PORT	WP_PORT_ENET7
#endif


typedef struct map_register_group {
	WP_U32 tcntr;
	WP_U32 mac1;
	WP_U32 mac2;
	WP_U32 serdes_io_ctrl;
	WP_U32 mac3;
	WP_U32 mac4;
	WP_U32 mac5;
	WP_U32 mac6;
	WP_U32 mac7;
	WP_U32 mac8;
	WP_U32 mac9;
}map_register_group;


#define ENET_NUMBER_TOTAL 	12
map_register_group map_address_reg[WP_PORT_IW_HOST];

/*-----------------------------------------------------------*\
  It is very important.
  When facing full speed 1G traffic generated from IXIA, 
  the interrupt queue size MUST be larger than 253, or the system 
  will die. 
  253 is A VERY IMPORTANT critical threshold !!!!
                        Morris
                        Jul/11/2013
\*-----------------------------------------------------------*/

#define INT_QUEUE_SIZE	(254) // it will die if 253

