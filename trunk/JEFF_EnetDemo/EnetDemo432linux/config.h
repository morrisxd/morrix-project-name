#if 1
#define INGRESS_PORT    WP_PORT_ENET15	// IXIA->wds3
#define EGRESS_PORT	WP_PORT_ENET11 //WP_PORT_ENET16	// wds3->IXIA
#define EGRESS_PORT2    WP_PORT_ENET12
// #define INGRESS_PORT	WP_PORT_ENET5	// IXIA->wds3
// #define EGRESS_PORT	WP_PORT_ENET4	// wds3->IXIA
#else
#define INGRESS_PORT	WP_PORT_ENET8
#define EGRESS_PORT	WP_PORT_ENET7
#endif

#define SECONDS_TO_WAIT	(10)

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






