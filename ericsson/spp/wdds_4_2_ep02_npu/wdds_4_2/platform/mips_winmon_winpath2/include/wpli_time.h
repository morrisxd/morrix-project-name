#ifndef _WPLI_TIME_H__
#define _WPLI_TIME_H__

#define WPL_TA_BASE_REGS      320
#define WPL_TA_ORDER_REGS     176
#define WPL_TA_DEV_MAP_REGS   32
#define WPL_TRS_SETS          32

typedef struct WPL_ta_block
{
      WP_U32 b[WPL_TA_BASE_REGS];
      WP_U32 pad_0[192];
      WP_VU32 order[WPL_TA_ORDER_REGS];
      WP_U32 pad_1[80];
      WP_U32 dev_map[WPL_TA_DEV_MAP_REGS];
      WP_VU32 trs_stat[WPL_TRS_SETS];
      WP_VU32 ta_cam[WPL_TRS_SETS];
      WP_VU32 queue_stat[8];
      WP_U32 ts_prescaler;
      WP_VU32 ts_timer[2];
      WP_U32 pad_4a[5];
      WP_U32 crc_data[4][2];
      WP_U32 pad_4b[72];
      WP_VU32 ta_pc[WPL_TRS_SETS];
      WP_U32 pad_5[64 - WPL_TRS_SETS];
} WPL_ta_block;

#endif /*_WPLI_TIME_H__*/
