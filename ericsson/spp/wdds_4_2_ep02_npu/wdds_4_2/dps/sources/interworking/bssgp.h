
/* COPYRIGHT-ENEA-SRC-R2 *
 **************************************************************************
 * Copyright (C) 2009 by Enea AB.
 * All rights reserved.
 *
 * This Software is furnished under a software license agreement and
 * may be used only in accordance with the terms of such agreement.
 * Any other use or reproduction is prohibited. No title to and
 * ownership of the Software is hereby transferred.
 *
 * PROPRIETARY NOTICE
 * This Software consists of confidential information.
 * Trade secret law and copyright law protect this Software.
 * The above notice of copyright on this Software does not indicate
 * any actual or intended publication of such Software.
 **************************************************************************
 * COPYRIGHT-END */
/****************************************************************************
*
* File: bssgp.h
*
* Description: This file contains defenitions of bssgp stractures and
*              macros used by interwork.dpl files.
*
*
* Module Originator:  chace liu
* Creation Date:  June  18, 2011
*
* Change History:
* +------------+--------------------+----------------------------------------
* | Date       | By                 | Description
* +------------+--------------------+----------------------------------------
* | aug 12, 2011 |  chace.liu@enea.com | ver 1.0.0
* +------------+--------------------+----------------------------------------
* |            |                    |
* +------------+--------------------+----------------------------------------
*
*
****************************************************************************/

#ifndef _BSSGP_H_
#define _BSSGP_H_


/*BSSGP PDU type defenitions*/
#define	DL_UNITDATA                         0x00
#define	UL_UNITDATA                         0x01
#define	RA_CAPABILITY                       0x02
#define	DL_MBMS_UNITDATA                    0x04
#define	PAGING_PS                           0x06
#define	PAGING_CS                           0x07
#define	RA_CAPABILITY_UPDATE                0x08
#define	RA_CAPABILITY_UPDATE_ACK            0x09
#define	RADIO_STATUS                        0x0a
#define	SUSPEND                             0x0b
#define	SUSPEND_ACK                         0x0c
#define	SUSPEND_NACK                        0x0d
#define	RESUME                              0x0e
#define	RESUME_ACK                          0x0f
#define	RESUME_NACK                         0x10
#define	BVC_BLOCK                           0x20
#define	BVC_BLOCK_ACK                       0x21
#define	BVC_RESET                           0x22
#define	BVC_RESET_ACK                       0x23
#define	BVC_UNBLOCK                         0x24
#define	BVC_UNBLOCK_ACK                     0x25
#define	FLOW_CONTROL_BVC                    0x26
#define	FLOW_CONTROL_BVC_ACK                0x27
#define	FLOW_CONTROL_MS                     0x28
#define	FLOW_CONTROL_MS_ACK                 0x29
#define	FLUSH_LL                            0x2a
#define	FLUSH_LL_ACK                        0x2b
#define	LLC_DISCARDED                       0x2c
#define	FLOW_CONTROL_PFC                    0x2d
#define	FLOW_CONTROL_PFC_ACK                0x2e
#define	SGSN_INVOKE_TRACE                   0x40
#define	STATUS                              0x41
#define	DOWNLOAD_BSS_PFC                    0x50
#define	CREATE_BSS_PFC                      0x51
#define	CREATE_BSS_PFC_ACK                  0x52
#define	CREATE_BSS_PFC_NACK                 0x53
#define	MODIFY_BSS_PFC                      0x54
#define	MODIFY_BSS_PFC_ACK                  0x55
#define	DELETE_BSS_PFC                      0x56
#define	DELETE_BSS_PFC_ACK                  0x57
#define	DELETE_BSS_PFC_REQ                  0x58
#define	PS_HANDOVER_REQUIRED                0x59
#define	PS_HANDOVER_REQUIRED_ACK            0x5a
#define	PS_HANDOVER_REQUIRED_NACK           0x5b
#define	PS_HANDOVER_REQUEST                 0x5c
#define	PS_HANDOVER_REQUEST_ACK             0x5d
#define	PS_HANDOVER_REQUEST_NACK            0x5e
#define	PS_HANDOVER_COMPLETE                0x91
#define	PS_HANDOVER_CANCEL                  0x92
#define	PERFORM_LOCATION_REQUEST            0x60
#define	PERFORM_LOCATION_RESPONSE           0x61
#define	PERFORM_LOCATION_ABORT              0x62
#define	POSITION_COMMAND                    0x63
#define	POSITION_RESPONSE                   0x64
#define	RAN_INFORMATION                     0x70
#define	RAN_INFORMATION_REQUEST             0x71
#define	RAN_INFORMATION_ACK                 0x72
#define	RAN_INFORMATION_ERROR               0x73
#define	RAN_INFORMATION_APPLICATION_ERROR   0x74
#define	MBMS_SESSION_START_REQUEST          0x80
#define	MBMS_SESSION_START_RESPONSE         0x81
#define	MBMS_SESSION_STOP_REQUEST           0x82
#define	MBMS_SESSION_STOP_RESPONSE          0x83
#define	MBMS_SESSION_UPDATE_REQUEST         0x84
#define	MBMS_SESSION_UPDATE_RESPONSE        0x85

#define BSSGP_IEI_TLLI 0x1f
#define BSSGP_IEI_DRXP 0x0a
#define BSSGP_IEI_BVCI 0x04

#define BSSGP_TLLI_TYPE_LOCAL 3
#define BSSGP_TLLI_TYPE_FOREIGN 2

#define DPS_SWITCH_MAX_CASE    0x20

#define PARSE_BSSGP_UP_FLOW_BIT    0x1

#define CUSTOMER_ENET_TO_FRAME_RELAY_FIWT    0xa5a5
#define CUSTOMER_ENET_TO_ENET_FIWT    0xa6a6

#define WP_IPEP_PORT_BASE 1024
#define WPI_VALID_BIT_MASK 0x80000000
#define WPI_NOPOOL_BIT_MASK 0x40000000
#define WPI_NSVC_REMOTE_BIT_MASK 0x20000000

#define WP_UL_RT_BASE_ADDRESS 0x0
#define WP_UL_RT_MEMORY_SIZE  0x4080000 // 0x1040000
#define WP_IPEP_BASE_ADDRESS (WP_UL_RT_BASE_ADDRESS + WP_UL_RT_MEMORY_SIZE) //0x1040000
#define WP_IPEP_MEMOREY_SIZE 0x2040000
#define WP_VPORT_BASE_ADDRESS (WP_IPEP_BASE_ADDRESS + WP_IPEP_MEMOREY_SIZE) //0x3080000
#define WP_VPORT_MEMOREY_SIZE 0x480000  //here we reach 0x3500000
#define WP_DL_RT_BASE_ADDRESS (WP_VPORT_BASE_ADDRESS + WP_VPORT_MEMOREY_SIZE) //0x3500000
#define WP_DL_RT_MEMOREY_SIZE 0x80000  //here we reach 0x3580000

#define WP_FLOW_STATIC_BASE_ADDRESS (WP_DL_RT_BASE_ADDRESS + WP_DL_RT_MEMOREY_SIZE) //0x3580000
#define WP_FLOW_STATIC_MEMOREY_SIZE 0x600000  //here we reach 0x3600000

#define WP_PRIORITY_HOST_ADDRESS (WP_FLOW_STATIC_BASE_ADDRESS + WP_FLOW_STATIC_MEMOREY_SIZE)
#define WP_PRIORITY_HOST_MEMORY_SIZE 0x100


/* Include Files */

/************************************* Start of NS && BSSGP **************************************/
struct S_BITSMAP
{
	_bit NsBytes           :1;
	_bit NsDataPackets           :1;
	_bit NsSignalPackets           :1;
	_bit BssgpBytes           :1;
	_bit BssgpDataPackets           :1;
	_bit BssgpSignalPackets           :1;
};

struct S_BITTLLIMAP
{
	  _bit    Byte1                         :8;
      _bit    Byte0                         : 1;
};

struct S_PARSE
{
   _bit ValidBit       :1;
   _bit UpFlow        :1;
   struct S_BITSMAP BitsMap;
   struct S_Byte BvciOffset;
   _bit NriOffset       :7;

   union {
   	   _bit Offset       :9;
   	   _bit Byte0       :9;
   	   struct S_BITTLLIMAP TlliBitsMap;
      } U_Offset_Byte0;   
};



struct S_TLLI
{
      _bit    Type                  : 2 ;
      _bit                          : 6 ;
      _bit     Nri                  : 8 ;//coresponding 64 sgsn
      _bit     Byte1                : 8;   
	  _bit                          :1;
	  _bit     Byte0                :1;
	  _bit                          :6;
};

struct S_TagLen
{
      _bit     Tag                          : 8 ;
      _bit      Mask                       : 1 ;
      
   union {
      _bit    H7bits                  : 7 ;
      _bit    Lenth                  : 15 ;
      } U_Lenth_Or_Bits;   
};

struct S_NSBGP
{
      struct S_Byte     NSType   ;// NS pdu type
      struct S_Byte     NSCtrl   ;
      struct S_HalfWord     NSBvci ;
      struct S_Byte     BGPType;
      struct S_TagLen BGPTagLen;
};


struct S_IP_EP
{
	_bit IpAddr                   : 32;
	struct S_HalfWord SrcPort;
	struct S_HalfWord DstPort;
} ;


struct S_IP_EP_Table
{
      struct S_IP_EP ip_ep0 ;  // Extension IP EndPoint 0
      struct S_IP_EP ip_ep1 ;  // Extension IP EndPoint 1
      struct S_IP_EP ip_ep2 ;  // Extension IP EndPoint 2
      struct S_IP_EP ip_ep3 ;  // Extension IP EndPoint 3
      struct S_IP_EP ip_ep4 ;  // Extension IP EndPoint 4
      struct S_IP_EP ip_ep5 ;  // Extension IP EndPoint 5
      struct S_IP_EP ip_ep6 ;  // Extension IP EndPoint 6
      struct S_IP_EP ip_ep7 ;  // Extension IP EndPoint 7
      struct S_IP_EP ip_ep8 ;  // Extension IP EndPoint 8
      struct S_IP_EP ip_ep9 ;  // Extension IP EndPoint 9
      struct S_IP_EP ip_ep10;  // Extension IP EndPoint 10
      struct S_IP_EP ip_ep11;  // Extension IP EndPoint 11
      struct S_IP_EP ip_ep12;  // Extension IP EndPoint 12
      struct S_IP_EP ip_ep13;  // Extension IP EndPoint 13
      struct S_IP_EP ip_ep14;  // Extension IP EndPoint 14
      struct S_IP_EP ip_ep15;  // Extension IP EndPoint 15
};

struct S_LOCAL_RT
{
	_bit valid_num                :32;	
      struct S_IP_EP ip_ep0 ;  // IP EndPoint 0
      struct S_IP_EP ip_ep1 ;  // IP EndPoint 1
      struct S_IP_EP ip_ep2 ;  // IP EndPoint 2
      struct S_IP_EP ip_ep3 ;  // IP EndPoint 3
      struct S_IP_EP ip_ep4 ;  // IP EndPoint 4
      struct S_IP_EP ip_ep5 ;  // IP EndPoint 5
      struct S_IP_EP ip_ep6 ;  // IP EndPoint 6
      struct S_IP_EP ip_ep7 ;  // IP EndPoint 7
      struct S_IP_EP ip_ep8 ;  // IP EndPoint 8
      struct S_IP_EP ip_ep9 ;  // IP EndPoint 9
      struct S_IP_EP ip_ep10;  // IP EndPoint 10
      struct S_IP_EP ip_ep11;  // IP EndPoint 11
      struct S_IP_EP ip_ep12;  // IP EndPoint 12
      struct S_IP_EP ip_ep13;  // IP EndPoint 13
      struct S_IP_EP ip_ep14;  // IP EndPoint 14
      struct S_IP_EP ip_ep15;  // IP EndPoint 15
      struct S_IP_EP_Table ip_ep_table1;
      struct S_IP_EP_Table ip_ep_table2;
      struct S_IP_EP_Table ip_ep_table3;
};
struct S_PORT_MAP_Table
{
      _bit port0            :32;
      _bit port1            :32;
      _bit port2            :32;
      _bit port3            :32;
      _bit port4            :32;
      _bit port5            :32;
      _bit port6            :32;
      _bit port7            :32;
      _bit port8            :32;
      _bit port9            :32;
      _bit port10           :32;
      _bit port11           :32;
      _bit port12           :32;
      _bit port13           :32;
      _bit port14           :32;
      _bit port15           :32;
};

struct S_GLOBLE_RT
{
	_bit flow_number0           :32;
	//_bit flow_number1           :32;//This FIW will go to slave interface
      _bit port0            :32;
      _bit port1            :32;
      _bit port2            :32;
      _bit port3            :32;
      _bit port4            :32;
      _bit port5            :32;
      _bit port6            :32;
      _bit port7            :32;
      _bit port8            :32;
      _bit port9            :32;
      _bit port10           :32;
      _bit port11           :32;
      _bit port12           :32;
      _bit port13           :32;
      _bit port14           :32;
      _bit port15           :32;
      struct S_PORT_MAP_Table Port_Table1;
      struct S_PORT_MAP_Table Port_Table2;
      struct S_PORT_MAP_Table Port_Table3;
};


struct S_VPORT_GRP
{
	_bit port_num           :32;
	_bit next_flow           :32;
	_bit flow_number0           :32;
	_bit flow_number1           :32;
	_bit flow_number2           :32;
	_bit flow_number3           :32;
	_bit flow_number4           :32;
	_bit flow_number5           :32;
	_bit flow_number6           :32;
	_bit flow_number7           :32;
	_bit flow_number8           :32;
	_bit flow_number9           :32;
	_bit flow_number10           :32;
	_bit flow_number11           :32;
	_bit flow_number12           :32;
	_bit flow_number13           :32;
	_bit flow_number14           :32;
	_bit flow_number15           :32;
};
#if 0
struct DL_RT
{
	//_bit Nsei           :32;
      union
      {
         _bit Nsei     :32;
         _bit SrcPort     :32;
      } U_Nsei_or_SrcPort;
	//_bit Bvci           :32;
     struct S_HalfWord     IwFlowNum ;
     struct S_HalfWord     Bvci ;
};
#endif
struct DL_RT
{
	_bit IwFlowNum           :32;
      union
      {
         _bit Nsei     :16;
         _bit SrcPort     :16;
      } U_Nsei_or_SrcPort;
     struct S_HalfWord     Bvci ;
};

struct NS_BSSGP_STATIC
{
	_bit RxNsBytes           :64;
	_bit RxNsDataPackets           :64;
	_bit RxNsSignalPackets           :64;
	_bit RxBssgpBytes           :64;
	_bit RxBssgpDataPackets           :64;
	_bit RxBssgpSignalPackets           :64;
   	_bit TxNsBytes           :64;
	_bit TxNsDataPackets           :64;
	_bit TxNsSignalPackets           :64;
	_bit TxBssgpBytes           :64;
	_bit TxBssgpDataPackets           :64;
	_bit TxBssgpSignalPackets           :64;
};

struct PRIORITY_FIWT
{
	_bit IwFlowNum0           :32;
	_bit IwFlowNum1           :32;
	_bit IwFlowNum2           :32;
	_bit IwFlowNum3           :32;	
};


/************************************* End of NS && BSSGP  ***************************************/

/* M_BSSGP_Parse
  \brief  parse NS packet based on BSSGP pdu type

  *****************************************************************************
  *
  * Description:
  *     Parse pakcet NS and BSSGP head
  *     Moves Sorts Data in TRS (lift 1 register up) , Update Framelength ,
  *     Update ExtDataPtr
  *
  * Inputs: P_ExtDataPointer,P_NS_BSSGP_HD,P_PARSE_BITS,BGPType,Tlli
  *
  * Outputs:P_NS_BSSGP_HD  P_PARSE_BITS  Tlli
  *
  *
  * Modified and saved registers: none
  *****************************************************************************/
inline func M_UpdateFlowStatistics(in P_STATISTICS_OFFSET,
                                     in P_PARSE_BITS,
                                     in P_InterworkingFlowNumber,
                                     in P_L2FrameLength)
{
   /* Increment the relevants valid statistics */
   register struct S_TempReg     P_TEMPORY_OFFSET                        = r22;//I guess this will not affect Flowoffset
   
   P_STATISTICS_OFFSET +=  WP_FLOW_STATIC_BASE_ADDRESS;
   P_STATISTICS_OFFSET += P_InterworkingFlowNumber * 64 ;
   P_STATISTICS_OFFSET += P_InterworkingFlowNumber * 32 ;
   P_TEMPORY_OFFSET = P_STATISTICS_OFFSET;
   
   _mem(r0,B_CustomerSpecificA[P_STATISTICS_OFFSET], STATISTICS_SIZE_WORD|VP_LOOKUP_BUS) += P_L2FrameLength;
   
   if(P_PARSE_BITS.BitsMap.NsDataPackets)
      P_STATISTICS_OFFSET +=8;
   else
      P_STATISTICS_OFFSET +=16;
   
   _mem(r0,B_CustomerSpecificA[P_STATISTICS_OFFSET],STATISTICS_SIZE_WORD|VP_LOOKUP_BUS) += 1;
   
   if( P_PARSE_BITS.BitsMap.BssgpBytes)
   {
      P_STATISTICS_OFFSET =24 + P_TEMPORY_OFFSET;
      _mem(r0,B_CustomerSpecificA[P_STATISTICS_OFFSET],STATISTICS_SIZE_WORD|VP_LOOKUP_BUS) += P_L2FrameLength ;
      if(P_PARSE_BITS.BitsMap.BssgpDataPackets)
         P_STATISTICS_OFFSET +=8;
      else
         P_STATISTICS_OFFSET +=16;
      _mem(r0,B_CustomerSpecificA[P_STATISTICS_OFFSET],STATISTICS_SIZE_WORD|VP_LOOKUP_BUS) += 1;
   }

}// End of M_UpdateIwPortStatistics macro

/* M_BSSGP_Parse
  \brief  parse NS packet based on BSSGP pdu type

  *****************************************************************************
  *
  * Description:
  *     Parse pakcet NS and BSSGP head
  *     Moves Sorts Data in TRS (lift 1 register up) , Update Framelength ,
  *     Update ExtDataPtr
  *
  * Inputs: P_ExtDataPointer,P_NS_BSSGP_HD,P_PARSE_BITS,BGPType,Tlli
  *
  * Outputs:P_NS_BSSGP_HD  P_PARSE_BITS  Tlli
  *
  *
  * Modified and saved registers: none
  *****************************************************************************/

//Ericsson guys always change their mind until the last time, so I have no choice but to move packet parse to wddi

inline func M_BSSGP_Parse(
	                     in FlowNumberOffset,
                         in P_ExtDataPointer,
                         inout P_NS_BSSGP_HD,
                         inout P_PARSE_BITS,
                         inout BGPType,
                         inout Tlli,
                         in P_TAG_LEN)
{
        register struct S_TempReg     P_STATISTICS_OFFSET                        = r21;
        P_STATISTICS_OFFSET = 0;
        
      *(&P_NS_BSSGP_HD) = _data_mem(r0, P_ExtDataPointer,sizeof(S_NSBGP),0);

      BGPType = P_NS_BSSGP_HD.BGPType;
      BGPType <<=2;

      *(&P_PARSE_BITS) =_mem(r0,B_CustomerSpecificA[BGPType],4,  VP_LOOKUP_BUS);

      //P_PARSE_BITS.BitsMap.NsBytes = 1; just using NsBytes to distinguish if it is a fast path flow
      
      if(P_NS_BSSGP_HD.NSType != 0)
      {
            P_PARSE_BITS.BitsMap.NsSignalPackets = 1;
                  //Update ingress statistics
            M_UpdateFlowStatistics(P_STATISTICS_OFFSET,
                     P_PARSE_BITS,
                     InterworkingFlowNumber,
                     L2FrameLength);

            /*for priority message*/
			if(P_TAG_LEN == 0)/*FR*/
			{
				FlowNumberOffset = WP_PRIORITY_HOST_ADDRESS;
				*(& P_TAG_LEN)=_mem(r0,B_CustomerSpecificA[FlowNumberOffset], 4,	VP_LOOKUP_BUS); 
				if(P_TAG_LEN & WPI_VALID_BIT_MASK)
				{
					P_TAG_LEN &= ~WPI_VALID_BIT_MASK;
					{
					/*update  flowNumber to FR header ,use FlowNumberOffset for tmp register*/
					 FlowNumberOffset = InterworkingFlowNumber;
					 _bf(FlowNumberOffset,0, 16) =   _bf(FlowNumberOffset,16, 16);
					 /*reach to FR header*/
					 P_ExtDataPointer -=2;
                    _data_mem(r0, P_ExtDataPointer, 2,0) = (FlowNumberOffset);
					 P_ExtDataPointer +=2;
					}
					InterworkingFlowNumber = P_TAG_LEN&0xffff;
					FlowNumberOffset = InterworkingFlowNumber * sizeof(S_FIWT);
					*(&FIWT)=_mem(r0,B_FIWT[FlowNumberOffset],sizeof(S_FIWT),  FIWT_BUS);

					goto L_IWM_FIWT_Analyze_After_FIWT_Read;
				}				
			}
			else/*IP*/
			{
			        FlowNumberOffset = WP_PRIORITY_HOST_ADDRESS;
				FlowNumberOffset += sizeof(PRIORITY_FIWT);
				*(& P_TAG_LEN)=_mem(r0,B_CustomerSpecificA[FlowNumberOffset], 4,	VP_LOOKUP_BUS);
				if(P_TAG_LEN & WPI_VALID_BIT_MASK)
				{
				    P_TAG_LEN &= ~WPI_VALID_BIT_MASK;
				}
				InterworkingFlowNumber = P_TAG_LEN&0xffff;
				FlowNumberOffset = InterworkingFlowNumber * sizeof(S_FIWT);
				*(&FIWT)=_mem(r0,B_FIWT[FlowNumberOffset],sizeof(S_FIWT),  FIWT_BUS);

			    goto L_IWM_FIWT_Analyze_After_FIWT_Read;
			}
      }
      
      P_PARSE_BITS.BitsMap.NsDataPackets = 1;
      P_PARSE_BITS.BitsMap.BssgpBytes = 1;
      //if(P_NS_BSSGP_HD.NSBvci > 1)
      if((BGPType == (DL_UNITDATA<<2))||(BGPType == (UL_UNITDATA<<2)))
      {
         P_PARSE_BITS.BitsMap.BssgpDataPackets = 1;
      }
      else
      {
         P_PARSE_BITS.BitsMap.BssgpSignalPackets = 1;
      }
      
      //Update ingress statistics
      M_UpdateFlowStatistics(P_STATISTICS_OFFSET,
               P_PARSE_BITS,
               InterworkingFlowNumber,
               L2FrameLength);
      
      if(P_NS_BSSGP_HD.NSCtrl != 0)
            goto L_IWM_FIWT_Analyze_After_FIWT_Read;


      if(P_PARSE_BITS.ValidBit == 0)
            goto L_IWM_FIWT_Analyze_After_FIWT_Read;
      
      if ((P_PARSE_BITS.UpFlow == 0) && (P_NS_BSSGP_HD.NSBvci <= 1))  
      {
            goto L_IWM_FIWT_Analyze_After_FIWT_Read;
      }

      /*if ((BGPType == (RADIO_STATUS<<2))  && (P_NS_BSSGP_HD.BGPTagLen.Tag != BSSGP_IEI_TLLI))  
      {
            goto L_IWM_FIWT_Analyze_After_FIWT_Read;
      }*/


      if(P_PARSE_BITS.U_Offset_Byte0.Offset) //has tlli, so read it 
      {
         P_ExtDataPointer += P_PARSE_BITS.U_Offset_Byte0.Offset;
         Tlli = _data_mem(r0, P_ExtDataPointer,4,0);
         P_ExtDataPointer -= P_PARSE_BITS.U_Offset_Byte0.Offset;
		 if(P_PARSE_BITS.UpFlow == 1)/*down link flow don't check tlli type*/
		 {
			 if((Tlli.Type != BSSGP_TLLI_TYPE_LOCAL) && (Tlli.Type != BSSGP_TLLI_TYPE_FOREIGN))
				goto L_IWM_FIWT_Analyze_After_FIWT_Read;
		 }
		 
		 //P_PARSE_BITS.U_Offset_Byte0.Byte0 = Tlli.Byte0;

		 P_PARSE_BITS.U_Offset_Byte0.TlliBitsMap.Byte1 = Tlli.Byte1;
		 P_PARSE_BITS.U_Offset_Byte0.TlliBitsMap.Byte0 = Tlli.Byte0;
    #if 0
         {
            register PatchOffset = r10;
            PatchOffset =0x4;
			P_TAG_LEN = P_PARSE_BITS.U_Offset_Byte0.Byte0;
            _mem(r0,B_DEBUG1[PatchOffset],4,DEBUG_BUS) =*(&P_TAG_LEN);;
         }
    #endif 		 
      } 
      else // have not tlli, How to get rand tlli value for down flow packet, IP-IP down flow also need tlli to do link select
      {
        // P_ExtDataPointer -= 18; 
        // Tlli = _time_stamp(); 
         //Tlli = 0;
         /*IP<-IP> downlink will use pdu type as TLLI*/
         P_PARSE_BITS.U_Offset_Byte0.Byte0 = P_NS_BSSGP_HD.BGPType;
      }
      
      //P_PARSE_BITS.U_Offset_Byte0.Byte0 = Tlli.Byte0;

      //I guss ericsson don't want so complex packet parse, these packet should go host directly?
      if((BGPType == (PAGING_PS<<2))||(BGPType == (PAGING_CS<<2)))
      {
      //   P_PARSE_BITS.BvciOffset = P_NS_BSSGP_HD.BGPTagLen.Lenth + 7;

      /*******************************************************************/
      /* NS type C/R BVCI  BSSGP type IMIS(tag lenth value) DRX(tag lenth value)
      /*     1        1      2             1                 1     1      5-10   
      /*
      /*******************************************************************/
  //       P_PARSE_BITS.BvciOffset = P_NS_BSSGP_HD.BGPTagLen.Lenth + 7;
         if(P_NS_BSSGP_HD.BGPTagLen.Mask)
         {
           P_PARSE_BITS.BvciOffset = P_NS_BSSGP_HD.BGPTagLen.U_Lenth_Or_Bits.H7bits + 7;
         }
         else
         {
           P_PARSE_BITS.BvciOffset = P_NS_BSSGP_HD.BGPTagLen.U_Lenth_Or_Bits.Lenth + 8;
         }

         P_ExtDataPointer += P_PARSE_BITS.BvciOffset;
        *(&P_TAG_LEN) = _data_mem(r0, P_ExtDataPointer,sizeof(S_TagLen),0);
         if(P_TAG_LEN.Tag == BSSGP_IEI_DRXP)
         {
            P_PARSE_BITS.BvciOffset += 4;
            P_ExtDataPointer +=4;
         }
        *(&P_TAG_LEN) = _data_mem(r0, P_ExtDataPointer,sizeof(S_TagLen),0);
         if(P_TAG_LEN.Tag == BSSGP_IEI_BVCI)
         {
            P_PARSE_BITS.BvciOffset +=2;
         }
         else
         {
            P_PARSE_BITS.BvciOffset =0;
         }
      }


}

/* M_BSSGP_LookUp
  \brief select one route entry based on NRI 

  *****************************************************************************
  *
  * Description:
  *      Calculate one ip endpoint base on tlli mod result
  *      save ip endpoint in temp register
  *
  * Inputs: FlowNumberOffset ,Port, IpEpOffset,P_PARSE_BITS,InterworkingFlowNumber,InterworkingFlowNumber0
  *
  * Outputs:InterworkingFlowNumber IpEp
  *
  *
  * Modified and saved registers:
  *****************************************************************************/
inline func M_BSSGP_LookUp(in FlowNumberOffset,
                                            inout Port,
                                            in IpEpOffset,
                                            in Tlli,
                                            in P_PARSE_BITS,
                                            inout InterworkingFlowNumber,
                                            inout InterworkingFlowNumber0,
                                            inout IpEpNum,
                                            inout IpEp)

{
    // register struct S_TempReg     P_STATISTICS_OFFSET                        = r21;

   //sizeof(S_GLOBLE_RT)
      FlowNumberOffset = InterworkingFlowNumber * 4;
      FlowNumberOffset += InterworkingFlowNumber * 1024; //256;
      FlowNumberOffset += InterworkingFlowNumber * 4; //257;

      //Borrow tempary register to store interworking flow number to host terminal channel
      InterworkingFlowNumber0 = InterworkingFlowNumber;
      /*
         struct S_GLOBLE_RT
         {
         	_bit flow_agg_num           :32;
            _bit port0            :32;
            _bit port1            :32;
         };

         The first 4 bytes comprise interwork flow number to ethernet tx channel.
      */

      InterworkingFlowNumber =_mem(r0,B_CustomerSpecificA[FlowNumberOffset],4,  VP_LOOKUP_BUS);


      /*Offset surpasses interwork flow number*/
      FlowNumberOffset += 4 ;
      /*Here offset has reached NRI base table*/
      //sizeof(S_LOCAL_RT) = 4
      //Tlli.Nri  >>= P_PARSE_BITS.NriOffset;

      Port = 256;
	  FlowNumberOffset += Port * 4 ;
      *(&Port) = _mem(r0,B_CustomerSpecificA[FlowNumberOffset],sizeof(S_TempReg),  VP_LOOKUP_BUS);
	  if((Port & WPI_NOPOOL_BIT_MASK) == 0)
	  {
          Port = 256;
	      FlowNumberOffset -= Port * 4 ;
	      Port = Tlli.Nri;
	      Port  >>= P_PARSE_BITS.NriOffset;
	      FlowNumberOffset += Port * 4;
	      *(&Port) = _mem(r0,B_CustomerSpecificA[FlowNumberOffset],sizeof(S_TempReg),  VP_LOOKUP_BUS);
	  }

      if(Port & WPI_VALID_BIT_MASK == 0)
      {
            InterworkingFlowNumber = InterworkingFlowNumber0;
            goto L_IWM_FIWT_Analyze_After_FIWT_Read;
      }
      
      Port &= ~WPI_VALID_BIT_MASK;
      Port &= ~WPI_NOPOOL_BIT_MASK;

      //Port before 1024 is well know port
      FlowNumberOffset = WP_IPEP_BASE_ADDRESS ;
      FlowNumberOffset += Port * 4;
      FlowNumberOffset += Port * 512;

      /*Here offset has reached a port table, one active IP end point should be selected*/
      *(&IpEpNum) = _mem(r0,B_CustomerSpecificA[FlowNumberOffset],sizeof(S_TempReg),  VP_LOOKUP_BUS);
      /*If there is not any active forwarding path, restore the interworking flow number to host terminal channel*/
      if(IpEpNum == 0)
      {     
	/*drop packet*/
	/*
            InterworkingFlowNumber = InterworkingFlowNumber0;
	F_ForwardingDropPacket(HOST_VIO_ERR_TYPE_GENERAL,
			       M_BitToByte(S_FlowStatistics.DeniedPackets),
			       DROP_WITH_STATISTCS);
	*/
            InterworkingFlowNumber = InterworkingFlowNumber0;
            goto L_IWM_FIWT_Analyze_After_FIWT_Read;
            //goto Layer3_Deny_Packet;
      }
      /*only forward useing LOCAL IPEP*/
      if((IpEpNum & WPI_VALID_BIT_MASK) != 0)
      {
	      IpEpNum&=~WPI_VALID_BIT_MASK;
		  IpEpNum= IpEpNum>>16;/*get LOCAL IPEP number*/
	  }
	  IpEpNum&=0xffff;
      /*
      struct S_LOCAL_RT
      {
      	_bit valid_num                :32;	
      	struct S_IP_EP ip_ep0;
      	struct S_IP_EP ip_ep1;
      };
      Here offset surpasses the ip endpoint num .valid_num */
      FlowNumberOffset += 4;
      
	      /*Here offset has reached to ip_ep0*/
		  //IpEpOffset = Tlli.Byte0;
		  
          switch(IpEpNum)
          {   case 0:
              case 1:
			      IpEpOffset = 0;
				  break;
			  case 2:
     	          IpEpOffset = (P_PARSE_BITS.U_Offset_Byte0.Byte0)%2;
				  break;
			  case 4:
     	          IpEpOffset = (P_PARSE_BITS.U_Offset_Byte0.Byte0)%4;
				  break;
			  case 8:
     	          IpEpOffset = (P_PARSE_BITS.U_Offset_Byte0.Byte0)%8;
                  break;
			  default:
			  {
			  	  IpEpOffset = (P_PARSE_BITS.U_Offset_Byte0.Byte0);
		          for(; IpEpOffset>=IpEpNum; IpEpOffset -=IpEpNum);
				  break;
			  }
		  }
      /*Choose the right ip endpoint base on load balace arithmetic */
      FlowNumberOffset += IpEpOffset * sizeof(S_IP_EP);      

     *(& IpEp)=_mem(r0,B_CustomerSpecificA[FlowNumberOffset],sizeof(S_IP_EP),  VP_LOOKUP_BUS);

    /* P_STATISTICS_OFFSET = 48;
     M_UpdateFlowStatistics(P_STATISTICS_OFFSET,
         P_PARSE_BITS,
         InterworkingFlowNumber,
               L2FrameLength);*/

      InterworkingFlowNumber >>=IpEp.SrcPort;//0 or 16, it must be those two value
      InterworkingFlowNumber &=  0xffff;
}


/* M_BSSGP_LookUp
  \brief select one route entry based on NRI 

  *****************************************************************************
  *
  * Description:
  *      Calculate one ip endpoint base on tlli mod result
  *      save ip endpoint in temp register
  *
  * Inputs: FlowNumberOffset ,Port, IpEpOffset,P_PARSE_BITS,InterworkingFlowNumber,InterworkingFlowNumber0
  *
  * Outputs:InterworkingFlowNumber IpEp
  *
  *
  * Modified and saved registers:
  *****************************************************************************/
inline func M_BSSGP_CalcIPEP(in FlowNumberOffset,
                                            in P_PORT_I,
                                            in IpEpOffset,
                                            in TlliByte0,/*==ParseBits.U_Offset_Byte0.Byte0*/
                                            inout IpEpNum,
                                            inout IpEp)

{
      
      //Port before 1024 is well know port
      FlowNumberOffset = WP_IPEP_BASE_ADDRESS ;
      P_PORT_I -= WP_IPEP_PORT_BASE;
      FlowNumberOffset += P_PORT_I * 4;
      FlowNumberOffset += P_PORT_I * 512;

      /*Here offset has reached a port table, one active IP end point should be selected*/
      *(&IpEpNum) = _mem(r0,B_CustomerSpecificA[FlowNumberOffset],sizeof(S_TempReg),  VP_LOOKUP_BUS);
      /*If there is not any active forwarding path, restore the interworking flow number to host terminal channel*/
      if(IpEpNum == 0)
      {
            goto L_IWM_FIWT_Analyze_After_FIWT_Read;
            //goto Layer3_Deny_Packet;
      }

      /*only forward useing LOCAL IPEP*/
      if((IpEpNum & WPI_VALID_BIT_MASK) != 0)
      {
	      IpEpNum&=~WPI_VALID_BIT_MASK;
		  IpEpNum= IpEpNum>>16;/*get LOCAL IPEP number*/
	  }
	  IpEpNum&=0xffff;
      /*
      struct S_LOCAL_RT
      {
      	_bit valid_num                :32;	
      	struct S_IP_EP ip_ep0;
      	struct S_IP_EP ip_ep1;
      };
      Here offset surpasses the ip endpoint num .valid_num */
      FlowNumberOffset += 4;
      
      /*Here offset has reached to ip_ep0*/
          switch(IpEpNum)
          {
              case 0:
              case 1:
			      IpEpOffset = 0;
				  break;
			  case 2:
     	          IpEpOffset = TlliByte0%2;
				  break;
			  case 4:
     	          IpEpOffset = TlliByte0%4;
				  break;
			  case 8:
     	          IpEpOffset = TlliByte0%8;
                  break;
			  default:
			  		{
						IpEpOffset = TlliByte0;
						/*Load balance can not use mod, substract will be ok? if the IpEpNum is one, it will take a long time to get the mod*/
						for(; IpEpOffset>=IpEpNum; IpEpOffset -=IpEpNum);
						break;
			        }
		  }
      /*Choose the right ip endpoint base on load balace arithmetic */
      FlowNumberOffset += IpEpOffset * sizeof(S_IP_EP);      

     *(& IpEp)=_mem(r0,B_CustomerSpecificA[FlowNumberOffset],sizeof(S_IP_EP),  VP_LOOKUP_BUS);

     
      InterworkingFlowNumber >>=IpEp.SrcPort;
      InterworkingFlowNumber &=  0xffff;
}

/* M_BSSGP_UpdateFIWT
  \brief use ip endpointer update corresponding field

  *****************************************************************************
  *
  * Description:
  *      Insert  ip destion address, udp source and destion port to prefixhead
  *      
  *
  * Inputs: InterworkingFlowNumber,PrefixData,IpEp
  *
  * Outputs:FIWT
  *
  *
  * Modified and saved registers:FIWT(r64)
  *****************************************************************************/

inline func M_BSSGP_UpdateFIWT(in InterworkingFlowNumber,
                                              in FlowNumberOffset,
                                              in ExtensionTableOffset,
                                              inout FIWT,
                                              in PrefixData,
                                              in IpEp,
                                              in UpdateSrcPort)
{
    // Insert the "outgoing Ttl" to the Ip header
     FlowNumberOffset = InterworkingFlowNumber * sizeof(S_FIWT);

     // Fetch the match Interworking Flow Table.
     *(&FIWT)=_mem(r0,B_FIWT[FlowNumberOffset],sizeof(S_FIWT),  FIWT_BUS);
     if(IpEp.SrcPort == 16)
     {
     //Prefix head offset for destination IP address should be 34. 
      _bf($reg(FIWT.L2PrefixWords.Word8),16, 16) =   _bf($reg(IpEp.IpAddr),0, 16);
      // if prefix head size larger than 36, the rest will be comprise in the extend table.
      ExtensionTableOffset = InterworkingFlowNumber  * sizeof(S_PrefixExtensionTable);
      
      *(&PrefixData) =  _mem(r0, B_FIWT_TABLE_EXT[ExtensionTableOffset], 4, FIWT_BUS); 
      _bf(PrefixData,0, 16) =   _bf($reg(IpEp.IpAddr),16, 16);
      //_bf(PrefixData,16, 16) =  IpEp.SrcPort;
      _bf(PrefixData,16, 16) =  _bf(UpdateSrcPort,16,16);
   
   // _mem don't support no-word (4 bytes align)access
       _mem(r0, B_FIWT_TABLE_EXT[ExtensionTableOffset], 4, FIWT_BUS) = 
                              *(&PrefixData);
      
      ExtensionTableOffset +=4;
      *(&PrefixData) =  _mem(r0, B_FIWT_TABLE_EXT[ExtensionTableOffset], 4, FIWT_BUS);
      _bf(PrefixData,0, 16) =   IpEp.DstPort;
      
       _mem(r0, B_FIWT_TABLE_EXT[ExtensionTableOffset], 4, FIWT_BUS) = 
                              *(&PrefixData);
     }
     else
    {
    
      $reg(FIWT.L2PrefixWords.Word4) = IpEp.IpAddr;
      //_bf($reg(FIWT.L2PrefixWords.Word5),0, 16) = IpEp.SrcPort;
      _bf($reg(FIWT.L2PrefixWords.Word5),0, 16) = _bf(UpdateSrcPort,16,16);;
      _bf($reg(FIWT.L2PrefixWords.Word5),16, 16) = IpEp.DstPort;
    }
}
/* M_BSSGP_DLLookUp
  \brief down flow route select

  *****************************************************************************
  *
  * Description:
  *      use harsh result to get down flow route information
  *      
  *
  * Inputs: P_DL_RT,PrefixData,FlowNumberOffset
  *
  * Outputs:DlTable
  *
  *
  * Modified and saved registers:DlTable(Tempory registes)
  *****************************************************************************/


inline func M_BSSGP_DLLookUp(P_PARSE_BITS,
                                              in P_DL_RT,
                                              inout FlowNumberOffset,
                                              inout DlTable,
                                              inout InterworkingFlowNumber)
{   
     //register struct S_TempReg     P_STATISTICS_OFFSET                        = r21;
     FlowNumberOffset = WP_DL_RT_BASE_ADDRESS;
     FlowNumberOffset +=  P_DL_RT* sizeof(DL_RT);
     *(& DlTable)=_mem(r0,B_CustomerSpecificA[FlowNumberOffset], sizeof(DL_RT),  VP_LOOKUP_BUS); 
     
     /*if(DlTable.U_Nsei_or_SrcPort.SrcPort & WPI_VALID_BIT_MASK == 0)
     {
         goto L_IWM_FIWT_Analyze_After_FIWT_Read;
     }
     
     DlTable.U_Nsei_or_SrcPort.SrcPort &= ~WPI_VALID_BIT_MASK;*/

     InterworkingFlowNumber = DlTable.IwFlowNum;//how to access slave flow number.

     /*if(InterworkingFlowNumber)
     {
         P_STATISTICS_OFFSET = 48;
        M_UpdateFlowStatistics(P_STATISTICS_OFFSET,
            P_PARSE_BITS,
            InterworkingFlowNumber,
                  L2FrameLength);
     }*/

}
/* M_BSSGP_CalcFIWT
  \brief down flow route select

  *****************************************************************************
  *
  * Description:
  *      use harsh result to get down flow route information
  *      
  *
  * Inputs: P_DL_RT,PrefixData,FlowNumberOffset
  *
  * Outputs:DlTable
  *
  *
  * Modified and saved registers:DlTable(Tempory registes)
  *****************************************************************************/
inline func M_BSSGP_CalcFIWT(in P_PARSE_BITS,
                                              inout P_NSEI_I,
                                              inout VpNum,
                                              inout NextVp,
                                              inout FlowNumberOffset,
                                              inout InterworkingFlowNumber,
                                              inout FIWT)
{   
     //register struct S_TempReg     P_STATISTICS_OFFSET                        = r21;
     
     FlowNumberOffset = WP_VPORT_BASE_ADDRESS;
    // FlowNumberOffset +=  P_NSEI_I * sizeof(S_VPORT_GRP);
     FlowNumberOffset +=  P_NSEI_I * 8;
     FlowNumberOffset +=  P_NSEI_I * 64;
     *(& VpNum)=_mem(r0,B_CustomerSpecificA[FlowNumberOffset],4,  VP_LOOKUP_BUS);
     
     if(VpNum & WPI_VALID_BIT_MASK == 0)
     {
         goto L_IWM_FIWT_Analyze_After_FIWT_Read;
     }
     VpNum &= ~WPI_VALID_BIT_MASK;
      /*only forward useing LOCAL IPEP*/
      if((VpNum & WPI_NSVC_REMOTE_BIT_MASK) != 0)
      {
	      VpNum&=~WPI_NSVC_REMOTE_BIT_MASK;
		  VpNum= VpNum>>16;/*get LOCAL IPEP number*/
	  }
	  VpNum&=0xffff;	 
      switch(VpNum)
      {   case 0:
          case 1:
		      NextVp = 0;
			  break;
		  case 2:
 	          NextVp = (P_PARSE_BITS.U_Offset_Byte0.Byte0)%2;
			  break;
		  case 4:
 	          NextVp = (P_PARSE_BITS.U_Offset_Byte0.Byte0)%4;
			  break;
		  case 8:
 	          NextVp = (P_PARSE_BITS.U_Offset_Byte0.Byte0)%8;
              break;
		  default:
		  {
		  	  NextVp = P_PARSE_BITS.U_Offset_Byte0.Byte0;
	          for(; NextVp>=VpNum; NextVp -=VpNum);
			  break;
		  }
	  }
	 
     FlowNumberOffset += 8;     /*reach vport table*/
     FlowNumberOffset += NextVp *4;
     
     *(&InterworkingFlowNumber)=_mem(r0,B_CustomerSpecificA[FlowNumberOffset],4,  VP_LOOKUP_BUS);

      FlowNumberOffset = InterworkingFlowNumber * sizeof(S_FIWT);
      // Fetch the match Interworking Flow Table.
      *(&FIWT)=_mem(r0,B_FIWT[FlowNumberOffset],sizeof(S_FIWT),  FIWT_BUS);


    /*P_STATISTICS_OFFSET = 48;
     M_UpdateFlowStatistics(P_STATISTICS_OFFSET,
         P_PARSE_BITS,
         InterworkingFlowNumber,
               L2FrameLength);*/

}

/* M_BSSGP_UpdateBvci
  \brief down flow route select

  *****************************************************************************
  *
  * Description:
  *      use harsh result to get down flow route information
  *      
  *
  * Inputs: P_DL_RT,PrefixData,FlowNumberOffset
  *
  * Outputs:DlTable
  *
  *
  * Modified and saved registers:DlTable(Tempory registes)
  *****************************************************************************/
inline func M_BSSGP_UpdateBvci(in P_ExtDataPointer,
                         in P_PARSE_BITS,
                         in BVCI_N)
{   
      _bf(BVCI_N,0, 16) =   _bf(BVCI_N,16, 16);
      _data_mem(r0, P_ExtDataPointer, 2,0) = (BVCI_N);

   //Update BSSGP bvci if needed
 //     P_PARSE_BITS.BvciOffset = 0x7;
      if(P_PARSE_BITS.BvciOffset)
      {
          P_ExtDataPointer += P_PARSE_BITS.BvciOffset; 
          P_ExtDataPointer -= 2; 
         _data_mem(r0, P_ExtDataPointer, 2,0) = (BVCI_N);
      }
     
}

#endif /* _BSSGP_H_ */
