/*****************************************************************************
 * (C) Copyright 2000-2006, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wufei_reg_fifo_manager.h
 *
 * Purpose: the UFE FIFO manager macro and structures
 *
 ****************************************************************************/

#ifndef WUFEI_HW_FIFO_MANAG_H
#define WUFEI_HW_FIFO_MANAG_H

typedef struct
{
   WP_U16 control_bits_1;          /* holds the priotiy function indication*/
   WP_U16 next_buff_desc;

}WUFEI_hw_buff_desc ;

typedef struct
{
      WP_U16 fnwp;  /*Free buffer pool Next Write Pointer    */
      WP_U16 fllp;  /*Free buffer pool Last Link Pointer     */
      WP_U16 fnfb; /*Free buffer pool Number of Free Buffers */
}WUFEI_hw_fifom;

struct WUFEI_hw_rspt_buff_mang
{
   WP_U32 reserved_1;
   WP_U16 control_bits_1;          /* holds the priotiy function indication*/
   WP_U16 write_buff_offset;
   WP_U8  buff_queue_max_value;
   WP_U8  buff_queue_count;
   WP_U16 read_buff_offset;
};
#define WUFEI_RSPT_CNTRL_1_PRI_TBL_S (18)
#define WUFEI_RSPT_CNTRL_1_PRI_TBL_W (2)
#define WUFEI_RSPT_CNTRL_1_PRI_TBL_F(v) WUFEI_FIELD(v, WUFEI_RSPT_CNTRL_1_PRI_TBL_S)
#define WUFEI_RSPT_CNTRL_1_PRI_TBL_V(f) \
        WUFEI_VALUE(f, WUFEI_RSPT_CNTRL_1_PRI_TBL_S, WUFEI_RSPT_CNTRL_1_PRI_TBL_W)
#define WUFEI_RSPT_CNTRL_1_PRI_TBL_M \
        WUFEI_MASK(WUFEI_RSPT_CNTRL_1_PRI_TBL_S, WUFEI_RSPT_CNTRL_1_PRI_TBL_W)

struct WUFEI_hw_tspt_buff_mang
{
   WP_U16 control_bits_1;          /* holds the priotiy function indication*/
   WP_U8  buff_queue_start_threshold;
   WP_U8  buff_queue_max_value;
   WP_U16 next_read_buff_offset;
   WP_U16 read_buff_offset;
   WP_U8  buff_last_count;
   WP_U8  buff_queue_count;
   WP_U16 write_buff_offset;
};
#define WUFEI_TSPT_CNTRL_1_PRI_TBL_S (26)
#define WUFEI_TSPT_CNTRL_1_PRI_TBL_W (2)
#define WUFEI_TSPT_CNTRL_1_PRI_TBL_F(v) WUFEI_FIELD(v, WUFEI_TSPT_CNTRL_1_PRI_TBL_S)
#define WUFEI_TSPT_CNTRL_1_PRI_TBL_V(f) \
        WUFEI_VALUE(f, WUFEI_TSPT_CNTRL_1_PRI_TBL_S, WUFEI_TSPT_CNTRL_1_PRI_TBL_W)
#define WUFEI_TSPT_CNTRL_1_PRI_TBL_M \
        WUFEI_MASK(WUFEI_TSPT_CNTRL_1_PRI_TBL_S, WUFEI_TSPT_CNTRL_1_PRI_TBL_W)

#define WUFEI_TSPT_CNTRL_1_FAST_PORT_S (28)
#define WUFEI_TSPT_CNTRL_1_FAST_PORT_W (1)
#define WUFEI_TSPT_CNTRL_1_FAST_PORT_F(v) WUFEI_FIELD(v, WUFEI_TSPT_CNTRL_1_FAST_PORT_S)
#define WUFEI_TSPT_CNTRL_1_FAST_PORT_V(f) \
        WUFEI_VALUE(f, WUFEI_TSPT_CNTRL_1_FAST_PORT_S, WUFEI_TSPT_CNTRL_1_FAST_PORT_W)
#define WUFEI_TSPT_CNTRL_1_FAST_PORT_M \
        WUFEI_MASK(WUFEI_TSPT_CNTRL_1_FAST_PORT_S, WUFEI_TSPT_CNTRL_1_FAST_PORT_W)

#define WUFEI_TSPT_CNTRL_1_WAIT_INIT_S (26)
#define WUFEI_TSPT_CNTRL_1_WAIT_INIT_W (2)
#define WUFEI_TSPT_CNTRL_1_WAIT_INIT_F(v) WUFEI_FIELD(v, WUFEI_TSPT_CNTRL_1_WAIT_INIT_S)
#define WUFEI_TSPT_CNTRL_1_WAIT_INIT_V(f) \
        WUFEI_VALUE(f, WUFEI_TSPT_CNTRL_1_WAIT_INIT_S, WUFEI_TSPT_CNTRL_1_WAIT_INIT_W)
#define WUFEI_TSPT_CNTRL_1_WAIT_INIT_M \
        WUFEI_MASK(WUFEI_TSPT_CNTRL_1_WAIT_INIT_S, WUFEI_TSPT_CNTRL_1_WAIT_INIT_W)

#define WUFEI_TSPT_CNTRL_1_WAIT_CNT_S (26)
#define WUFEI_TSPT_CNTRL_1_WAIT_CNT_W (2)
#define WUFEI_TSPT_CNTRL_1_WAIT_CNT_F(v) WUFEI_FIELD(v, WUFEI_TSPT_CNTRL_1_WAIT_CNT_S)
#define WUFEI_TSPT_CNTRL_1_WAIT_CNT_V(f) \
        WUFEI_VALUE(f, WUFEI_TSPT_CNTRL_1_WAIT_CNT_S, WUFEI_TSPT_CNTRL_1_WAIT_CNT_W)
#define WUFEI_TSPT_CNTRL_1_WAIT_CNT_M \
        WUFEI_MASK(WUFEI_TSPT_CNTRL_1_WAIT_CNT_S, WUFEI_TSPT_CNTRL_1_WAIT_CNT_W)

#define WUFEI_BD_CNTRL_READY_S (31)
#define WUFEI_BD_CNTRL_READY_W (1)
#define WUFEI_BD_CNTRL_READY_F(v) WUFEI_FIELD(v, WUFEI_BD_CNTRL_READY_S)
#define WUFEI_BD_CNTRL_READY_V(f) \
        WUFEI_VALUE(f, WUFEI_BD_CNTRL_READY_S, WUFEI_BD_CNTRL_READY_W)
#define WUFEI_BD_CNTRL_READY_M \
        WUFEI_MASK(WUFEI_BD_CNTRL_READY_S, WUFEI_BD_CNTRL_READY_W)

/* continue with oll the other bits
   EOP
   SOP
   ERR
   ...
   ...
*/
#endif /* WUFEI_HW_FIFO_MANAG_H*/
