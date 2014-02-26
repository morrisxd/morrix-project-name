#ifndef __HZSDN_H__
#define __HZSDN_H__

#define MORRIS_CHANGE_PACKET_BUS_TO_HOST	(0)
#define MORRIS4	MORRIS3


#ifdef MORRIS_WDS_SL
#warning BOARD_IS_WDS_SL
#define WP_BOARD_WDS3_SL		(1)
#else
#undef WP_BOARD_WDS3_SL
#warning BOARD_IS_WDS3_STD
#endif


#endif
