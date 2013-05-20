/*****************************************************************************
 * (C) Copyright 2001-2009, Wintegra.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 ****************************************************************************/

/************************************************************************/
/*    ETHERNET PROTOCOL PHYS and CARDS INITIALIZATION                   */
/************************************************************************/

/* Note: This is a preliminary release, which assumes a default memory map. */
#include <stdio.h>

#ifndef WP_TYPES_H
#include "api/wp_types.h"
#endif
#ifndef WP_WDDI_H
#include "api/wp_wddi.h"
#endif
#ifndef WP_PROTOTYPES_H
#include "api/wp_prototypes.h"
#endif
#ifndef WPX_ENET_PHY_H
#include "wpx_enet_phy.h"
#endif
#ifndef WPX_PHY_DATA_H
#include "wpx_phy_data.h"
#endif
#ifndef WPI_HW_WINNET_H
#include "hardware/winpath3/wpi_hw_winnet.h"
#endif
#ifndef WPX_PINMUX_H
#include "wpx_pinmux.h"
#endif

/* definition for backward compatability*/
#define WPU_GeiPhyInit WPU_EnetPhyInit

static WP_boolean first_time = WP_TRUE;

WP_status WPU_EnetPhyInit(WP_U32 port, WP_U32 mode)
{
   WP_U8 serial_control=0;
   WP_U32 write_data = 0, read_data=0;
   WP_U32 first_phy_addr = 0, phys_num = 0, ii = 0;
   WP_U32 mdc, mdio;
   WP_status status;
   WP_U32 mgmt_winnet;
   WP_U32 major_mode = mode & WPU_MAJOR_MODE_MASK;
   WP_U32 loopback = ((mode & WPU_PHY_LOOPBACH_MASK) == WPU_MODE_GEI_LOOPBACK_ON);
   WP_U32 autoneg = ((mode & WPU_PHY_AUTONEG_MASK) == WPU_ANEG_OFF)?0:1;

   wpx_fpga *fpga;
   
   WPX_FpgaMatchRegSet(0);
   fpga = (wpx_fpga *)WPL_PHY_VIRTUAL(0, WPI_WDS3_FPGA_BASE);

   if(first_time)
   {
      /* unreset cards on serial connectors*/
      serial_control = 0x0;
      WP_MEM_BYTE_SET(fpga->serial_control, serial_control);
      serial_control = 0xff;
      WP_MEM_BYTE_SET(fpga->serial_control, serial_control);
      WP_Delay(1000000);
   }
   WP_Delay(1000000);
   
   switch (major_mode)
   {
      case WPU_MODE_SMII:
      case WPU_MODE_SS_SMII:
      {
         /*SMII and SS-SMII phys connections:
         Serial connector 1 - configured through winnet 2: 
         Option 1: 
         WINNET0 GROUP1 - ENET 2 - connected to phys 0-7 in phy
         WINNET1 GROUP1 - ENET 4 - connected to phys 8-15 in phy
         Option 2 - does not work for now
         WINNET2 GROUP1 - ENET 6 - connected to phys 0-7 in phy
         WINNET1 GROUP1 - ENET 4 - connected to phys 8-15 in phy

         Serial connector 2 - configured through winnet 3: 
         Option 1 - does not work for now 
         WINNET3 GROUP1 - ENET 8 - connected to phys 0-7 in phy
         WINNET4 GROUP1 - ENET 10 - connected to phys 8-15 in phy
         Option 2:
         WINNET4 GROUP1 - ENET 10 - connected to phys 8-15 in phy
         WINNET5 GROUP1 - ENET 12 - connected to phys 0-7 in phy
         */
        phys_num = 8;

        switch (port)
        {
           case WP_PORT_ENET2:
              mgmt_winnet = 2;
              mdc = WP_MII_MGMT_MDC_1;
              mdio = WP_MII_MGMT_MDIO1;
              first_phy_addr = 0 + mdio;
              break;
           case WP_PORT_ENET4:
              mgmt_winnet = 2;
              mdc = WP_MII_MGMT_MDC_1;
              mdio = WP_MII_MGMT_MDIO1;
              first_phy_addr = 8 + mdio;
              break;
           case WP_PORT_ENET6:
              mgmt_winnet = 2;
              mdc = WP_MII_MGMT_MDC_1;
              mdio = WP_MII_MGMT_MDIO1;
              first_phy_addr = 0 + mdio;
              break;
           case WP_PORT_ENET8:
              mgmt_winnet = 3;
              mdc = WP_MII_MGMT_MDC_2;
              mdio = WP_MII_MGMT_MDIO2;
              first_phy_addr = 0 + mdio;
              break;
           case WP_PORT_ENET10:
              mgmt_winnet = 3;
              mdc = WP_MII_MGMT_MDC_2;
              mdio = WP_MII_MGMT_MDIO2;
              first_phy_addr = 8 + mdio;
              break;
           case WP_PORT_ENET12:
              mgmt_winnet = 3;
              mdc = WP_MII_MGMT_MDC_2;
              mdio = WP_MII_MGMT_MDIO2;
              first_phy_addr = 0 + mdio;
              break;
           default:
              return WPI_ERROR(WP_ERR_CONFIG);
        }

        if(first_time)
        {
           status = WP_SysMiiMgmtClockConfig(0, mdc, WP_MII_MGMT_MDC_DIVIDER_4096);
           if(status != WP_OK)
           {
              return status;
           }
           
           WPX_PinMuxSetMgmtWinnet(0, mgmt_winnet);
           
           /* unreset cards on serial connectors*/
           WPI_MEM_BYTE_GET(serial_control, fpga->serial_control);
           serial_control |= 0xa;
           WPI_MEM_BYTE_SET(fpga->serial_control, serial_control);
           WP_Delay(1000000);
           first_time = WP_FALSE;
        }           
        for(ii = 0; ii< phys_num; ii++)
        {
           write_data = WPX_SMII_PHY_RESET_M |
              WPX_SMII_PHY_LOOPBACK_F(loopback) |
              WPX_SMII_PHY_SPEED_F(1) |
              WPX_SMII_PHY_AUTONEG_F(autoneg)|
              WPX_SMII_PHY_HALFDUPLEX_F(WPX_SMII_PHY_FULL_DUPLEX);
           status = WP_SysMiiMgmtWrite(0, first_phy_addr+ii, 0, write_data);
           if(status != WP_OK)
           {
              return status;
           }
           WP_Delay(1000000);
           status = WP_SysMiiMgmtRead(0, first_phy_addr+ii, 0, &read_data);
           if(status != WP_OK)
           {
              return status;
           }
           WP_Delay(1000000);
        }
      }
      break;
      case WPU_MODE_RMII:
      {
         /* RMII DC connection:

         Serial connector 1 - configured through winnet 2: 
         With RMII-DC you can check WN0.0  - WP_PORT_ENET1 phy 0
                                  & WN0.5  - WP_PORT_ENET2 phy 1

         WN0_RM0 <--> phy 0.6
         WN0_RM5 <--> phy 1.6

         Front view of RMII-DC
         |---|---|---|---|---|---|---|---|
         |   |   |   |   |   |   |   |   |
         |---|---|---|---|---|---|---|---|
         |1.6|   |   |   |0.6|   |   |   |
         |---|---|---|---|---|---|---|---|
         */
         switch (port)
        {
           case WP_PORT_ENET1:
              mgmt_winnet = 2;
              mdc = WP_MII_MGMT_MDC_1;
              mdio = WP_MII_MGMT_MDIO1;
              first_phy_addr = 6 + mdio;
              break;
           case WP_PORT_ENET2:
              mgmt_winnet = 2;
              mdc = WP_MII_MGMT_MDC_1;
              mdio = WP_MII_MGMT_MDIO1;
              first_phy_addr = 14 + mdio;
              break;
           default:
              return WPI_ERROR(WP_ERR_CONFIG);
        }
        
        if(first_time)
        {
           status = WP_SysMiiMgmtClockConfig(0, mdc, WP_MII_MGMT_MDC_DIVIDER_4096);
           if(status != WP_OK)
           {
              return status;
           }
           
           WPX_PinMuxSetMgmtWinnet(0, mgmt_winnet);
           
           /* unreset cards on serial connectors*/
           WP_MEM_BYTE_GET(serial_control, fpga->serial_control);
           serial_control |= 0xa;
           WP_MEM_BYTE_SET(fpga->serial_control, serial_control);
           WP_Delay(1000000);
           first_time = WP_FALSE;
        }           

        write_data = WPX_RMII_PHY_RESET_M |
           WPX_RMII_PHY_LOOPBACK_F(loopback) |
           WPX_RMII_PHY_SPEED_F(1) |
           WPX_RMII_PHY_AUTONEG_F(autoneg)|
           WPX_RMII_PHY_HALFDUPLEX_F(WPX_RMII_PHY_FULL_DUPLEX);
           status = WP_SysMiiMgmtWrite(0, first_phy_addr, 0, write_data);
           if(status != WP_OK)
           {
              return status;
           }
           WP_Delay(1000000);
           status = WP_SysMiiMgmtRead(0, first_phy_addr, 0, &read_data);
           if(status != WP_OK)
           {
              return status;
           }
      }
      
      break;
      case WPU_MODE_RGMII:
      {
       /* RGMII phy connection:
          Serial connector 1 - configured through winnet 2: 
          ENET PORT 1 - phy 0
          ENET PORT 2 - phy 1
          ENET PORT 3 - phy 2
          ENET PORT 4 - phy 3
          Serial connector 2 - configured through winnet 3: 
          ENET PORT 9 - phy 2
          ENET PORT 10 - phy 3
          ENET PORT 11 - phy 0
          ENET PORT 12 - phy 1

       */
        switch (port)
        {
           case WP_PORT_ENET1:
              mgmt_winnet = 2;
              mdc = WP_MII_MGMT_MDC_1;
              mdio = WP_MII_MGMT_MDIO1;
              first_phy_addr = 0 + mdio;
              break;
           case WP_PORT_ENET2:
              mgmt_winnet = 2;
              mdc = WP_MII_MGMT_MDC_1;
              mdio = WP_MII_MGMT_MDIO1;
              first_phy_addr = 1 + mdio;
              break;
           case WP_PORT_ENET3:
              mgmt_winnet = 2;
              mdc = WP_MII_MGMT_MDC_1;
              mdio = WP_MII_MGMT_MDIO1;
              first_phy_addr = 2 + mdio;
              break;
           case WP_PORT_ENET4:
              mgmt_winnet = 2;
              mdc = WP_MII_MGMT_MDC_1;
              mdio = WP_MII_MGMT_MDIO1;
              first_phy_addr = 3 + mdio;
              break;
           case WP_PORT_ENET9:
              mgmt_winnet = 3;
              mdc = WP_MII_MGMT_MDC_2;
              mdio = WP_MII_MGMT_MDIO2;
              first_phy_addr = 2 + mdio;
              break;
           case WP_PORT_ENET10:
              mgmt_winnet = 3;
              mdc = WP_MII_MGMT_MDC_2;
              mdio = WP_MII_MGMT_MDIO2;
              first_phy_addr = 3 + mdio;
              break;
           case WP_PORT_ENET11:
              first_phy_addr = 0;
              mgmt_winnet = 3;
              mdc = WP_MII_MGMT_MDC_2;
              mdio = WP_MII_MGMT_MDIO2;
              first_phy_addr = 0 + mdio;
              break;
           case WP_PORT_ENET12:
              mgmt_winnet = 3;
              mdc = WP_MII_MGMT_MDC_2;
              mdio = WP_MII_MGMT_MDIO2;
              first_phy_addr = 1 + mdio;
              break;
           default:
              return WPI_ERROR(WP_ERR_CONFIG);
        }

        status = WP_SysMiiMgmtClockConfig(0, mdc, WP_MII_MGMT_MDC_DIVIDER_4096);
        if(status != WP_OK)
        {
           return status;
        }
           
        WPX_PinMuxSetMgmtWinnet(0, mgmt_winnet);
        
         /* unreset cards on serial connectors*/
         serial_control = 0x0;
         WPI_MEM_BYTE_SET(fpga->serial_control, serial_control);
         WP_Delay(10000);
         serial_control = 0xa;
         WPI_MEM_BYTE_SET(fpga->serial_control, serial_control);
         WP_Delay(10000);

         /* change to extended registers */
         status = WP_SysMiiMgmtWrite(0, first_phy_addr, 0x1f, 1);
         if(status != WP_OK)
         {
            return status;
         }
         /* sigdet - active low */
         status = WP_SysMiiMgmtWrite(0, first_phy_addr, 0x13, 1);
         if(status != WP_OK)
         {
            return status;
         }
         /* change to normal registers */
         status = WP_SysMiiMgmtWrite(0, first_phy_addr, 0x1f, 0);
         if(status != WP_OK)
         {
            return status;
         }

         write_data = WPX_RGMII_PHY_SPEED_LSB_F(WPX_RGMII_PHY_SPEED_1000_LSB)|
            WPX_RGMII_PHY_SPEED_MSB_F(WPX_RGMII_PHY_SPEED_1000_MSB)|
            WPX_RGMII_PHY_AUTONEG_F(autoneg)|
            WPX_RGMII_PHY_HALFDUPLEX_F(WPX_SMII_PHY_FULL_DUPLEX);

         status = WP_SysMiiMgmtWrite(0, first_phy_addr, 0x0, write_data);
         if(status != WP_OK)
         {
            return status;
         }
         status = WP_SysMiiMgmtRead(0, first_phy_addr, 0x0, &read_data);
         if(status != WP_OK)
         {
            return status;
         }
         if(write_data != read_data)
         {
            printf("first_phy_addr = %d write_data = 0x%x read_data = 0x%x\n", first_phy_addr, write_data, read_data);
            
            return WPI_ERROR(WP_ERR_PHY);
         }
         /* Set loopback */
         write_data |= WPX_RGMII_PHY_LOOPBACK_F(loopback);
         status = WP_SysMiiMgmtWrite(0, first_phy_addr, 0x0, write_data);
         if(status != WP_OK)
         {
            return status;
         }

         status = WP_SysMiiMgmtRead(0, first_phy_addr, 0x0, &read_data);
         if(status != WP_OK)
         {
            return status;
         }
         if(write_data != read_data)
         {
            return WPI_ERROR(WP_ERR_PHY);
         }
      }
      break;
   }
   return WP_OK;
}

