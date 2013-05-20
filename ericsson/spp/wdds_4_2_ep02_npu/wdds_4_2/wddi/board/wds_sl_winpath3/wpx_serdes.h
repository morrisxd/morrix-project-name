/*****************************************************************************
 * (C) Copyright 2000-2005, Wintegra. All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/*****************************************************************************
 *
 * File: wpx_serdes.h
 *
 * Purpose: serdes registers initialization for wp3 
 *
 ****************************************************************************/
#ifndef WPX_SERDES_H
#define WPX_SERDES_H

#define WPX_NUM_SINGLE_LANE_SERDES 2
#define WPX_NUM_QUAD_LANE_SERDES 1
#define WPX_RESERVED_QUAD_LANE_SERDES 2 /* place that should be reserved but not used */
#define WPX_SERDES_INIT_DELAY 100
#define WPX_SERDES_INIT_POLLING_LOOPS 10

#define WPX_SERDES_SINGLE_POLLING_MASK 0x10000000
#define WPX_SERDES_QUAD_POLLING_MASK 0x00001111

#define WPX_SERDES_LANE0_LOOPBACK 0x1
#define WPX_SERDES_LANE1_LOOPBACK 0x2
#define WPX_SERDES_LANE2_LOOPBACK 0x4
#define WPX_SERDES_LANE3_LOOPBACK 0x8
#define WPX_SERDES_TOGGLE_DELAY 1
typedef enum 
{
   WPX_SERDES0 ,
   WPX_SERDES_SINGLE_LANE_FIRST = WPX_SERDES0,
   WPX_SERDES1,
   WPX_SERDES_SINGLE_LANE_LAST = WPX_SERDES1,
   WPX_SERDES2,
   WPX_SERDES_QUAD_LANE_FIRST = WPX_SERDES2,
   WPX_SERDES_QUAD_LANE_LAST = WPX_SERDES2,
   WPX_SERDES_NUM,
   WPX_SERDES_NA=WPX_SERDES_NUM
}WPX_SERDES_LANES;

typedef enum
{
   WPX_SERDES_LANE_0,
   WPX_SERDES_LANE_1,
   WPX_SERDES_LANE_2,
   WPX_SERDES_LANE_3,
   WPX_SERDES_LANE_ALL,
   WPX_SERDES_LANE_NA
}WPX_SERDES_LANES_NUMS;

typedef enum
{
   WPX_SERDES_1G,
   WPX_SERDES_2_5G,
   WPX_SERDES_RATE_NA
}WPX_SERDES_RATE;

typedef struct
{
      WPX_SERDES_LANES serdes_index;
      WPX_SERDES_LANES_NUMS lane_index;
      WPX_SERDES_RATE rate;
} wpx_serdes_lane_data;

typedef struct
{
      wpx_serdes_lane_data lane_data[18];
}wpx_port_to_serdes_connection;

typedef struct 
{
      WP_U32 single_io_ctrl0;
      WP_U32 single_io_ctrl1;
}wpx_serdes_single_io_ctrl;

typedef struct 
{
      WP_U32 quad_io_ctrl0;
      WP_U32 quad_io_ctrl1;
      WP_U32 quad_io_ctrl2;
      WP_U32 quad_io_ctrl3;
      WP_U32 quad_io_ctrl4;
      WP_U32 quad_io_stat;
}wpx_serdes_quad_io_ctrl;

typedef struct
{
      WP_U32 mux_ctrl0;
      WP_U32 mux_ctrl1;
}wpx_serdes_mux_ctrl;

typedef struct 
{
      wpx_serdes_mux_ctrl mux_ctrl;
      WP_U32 clk_out0;
      WP_U32 clk_out1;
      wpx_serdes_single_io_ctrl io_ctrl_single[WPX_NUM_SINGLE_LANE_SERDES];
      wpx_serdes_quad_io_ctrl io_ctrl_quad[WPX_NUM_QUAD_LANE_SERDES+WPX_RESERVED_QUAD_LANE_SERDES];
      WP_U32 interrupt0;
      WP_U32 interrupt1;
      WP_U32 interrupt0_mask;
      WP_U32 interrupt1_mask;
}wpx_serdes_mux;

typedef struct 
{
      WP_U32 cmu[16];
      WP_U32 reserved1[80];
      WP_U32 lane[22];
      WP_U32 reserved2[74];
      WP_U32 gpon[10];
      WP_U32 reserved3[22];
      WP_U32 pipe[5];
      WP_U32 reserved4[27];
} wpx_serdes_single_lane;


typedef struct 
{
      WP_U32 cmu[16];
      WP_U32 reserved1[16];
      WP_U32 lane0[22];
      WP_U32 reserved2[10];
      WP_U32 lane1[22];
      WP_U32 reserved3[10];
      WP_U32 lane2[22];
      WP_U32 reserved4[10];
      WP_U32 lane3[22];
      WP_U32 reserved5[106];
}wpx_serdes_quad_lane;

typedef struct 
{
      wpx_serdes_mux serdes_mux;
      WP_U32 reserved[66];
      wpx_serdes_single_lane serdes_single[WPX_NUM_SINGLE_LANE_SERDES];
      wpx_serdes_quad_lane serdes_quad[WPX_NUM_QUAD_LANE_SERDES];
}wpx_serdes;

WP_status WPX_SerDesSet(WP_U32 wpid,
                        wpx_board_configuration configuration);
WP_status WPX_SerdesSetSingleLane(WP_U32 wpid, WP_U32 serdes, WP_U32 rate, WP_U8 loopback);
WP_status WPX_SerdesSetQuadLane(WP_U32 wpid, WP_U32 serdes, WP_U32 rate, WP_U8 loopback);
WP_status WPX_SetSerdesMux(WP_U32 wpid, wpx_board_configuration configuration);
#endif
