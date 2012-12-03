/*****************************************************************************
 * (C) Copyright Wintegra 2000-2003.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************
/****************************************************************************
*
* File: frame_relay.h
*
* Description: frame relay H/W registers, structures and constants definitions, used for
*              general frame relay routines.
*
*
* Module Originator:  Ronen Weiss
* Creation Date: Jan 14, 2002
*
* Change History:
* +------------+--------------------+----------------------------------------
* | Date       | By                 | Description
* +------------+--------------------+----------------------------------------
* |            |                    |
* +------------+--------------------+----------------------------------------
* |            |                    |
* +------------+--------------------+----------------------------------------
*
****************************************************************************/


#pragma list raw;

#ifndef _FRAME_RELAY_H_
#define _FRAME_RELAY_H_






struct S_FrRxStatus0
{
   _bit OV             : 1;  /* TDI HDLC Rx FIFO Overflow */
   _bit ABRT           : 1;  /* Aborted frame error */
   union
   {
      _bit NO          : 1;  /* Non Octet in HDLC */
      _bit             : 1;  /* reserved for POS */
   } U_Bit10;
   _bit FES            : 1;  /* FIFO entry size: 1 - 32 bytes, 0 - 64 bytes. */
   _bit DUM            : 1;  /* Dummy error bit (indicated the previous error) */
   _bit GSE            : 1;  /* Global Serial Error bit */
   _bit SOP_EOP_E      : 1;  /* POS SOP/EOP error bit */
   _bit PE             : 1;  /* Parity Error */
   _bit INTM           : 1;  /* Rx overflow interrupt mask (TDI only) */
   _bit                : 1;
   _bit Addlm          : 1;  /* Address Lookup mode, 0 - range/ 1 - Hash */
   _bit IRQ_Num        : 2;  /* Interrup table number */
};


struct S_FrDpsStatus0
{
   _bit                : 5 ;
   _bit GCHD           : 1 ; /* Graceful Channel (flow) deactivate bit */
   _bit INTM           : 1 ; /* Interrupt mask */
   _bit OvUnFlowIntm   : 1;  /* Mask host queue over flow and FBP underun interrupts */
   _bit PSTE           : 1 ;
   _bit STE            : 1 ; /* Statistics Enable bit */
   _bit IWBGM          : 1 ; /* Interworking Background Task Mode */
   _bit IRQ_Num        : 2 ; /* Interrup table number */
};


struct S_FrSerialStatusReg0
{
   _bit First          : 1;  /* HDLC/POS receiver First packet bit    */
   _bit Last           : 1;  /* HDLC/POS reciever Last packet bit */
   _bit                : 2;
   _bit PosErr         : 1;  /* POS rx errorred frame indication */
   _bit AccErr         : 1;  /* CRC or FIFO put/get errors reported by WMM */
   _bit UpiGSE         : 1;
   _bit Bz             : 1;  /* Base size selector */
   _bit Base           : 5;  /* Base pointer to the TA RAM */
   _bit                : 5;
   _bit CRC            : 1;  /* Reserved for CRC-10 calculation */
   union
   {
      struct S_FrRxStatus0  Rx;
      struct S_FrDpsStatus0 Dps;
   } U_SerialStatus;
};

/*  Fr Status0 register structure */
struct S_FrTxSerialStatusReg0
{

   _bit                : 1;  /* HDLC/POS receiver First packet bit */
   _bit FifoExt        : 1;  /* HDLC/POS reciever Last packet bit */
   _bit                : 2;
   _bit PosErr         : 1;  /* POS rx errorred frame indication */
   _bit AccErr         : 1;  /* CRC or FIFO put/get errors reported by WMM */
   _bit                : 1;
   _bit Bz             : 1;  /* Base size selector */
   _bit Base           : 5;  /* Base pointer to the TA RAM */
   _bit                : 5;
   _bit CRC            : 1;  /* Reserved for CRC-10 calculation */
   union
   {
     struct S_PosTxStatus0 Tx;
     struct S_FrDpsStatus0 Dps;
   } U_SerialStatus;
};

#ifdef _WINPATH_REV_WP3_ENABLE_

struct S_FrRxSerialStatusReg1
{
   _bit SC             : 8;  /* Serial Counter */
   _bit                : 2;
   _bit SO             : 6;  /* Serial Order */
   _bit                : 6;  /* Phy Number */
   _bit DataUnitLength : 10;  /* Data unit length */
};

struct S_FrTxSerialStatusReg1
{
    _bit                : 16;
    _bit FN             : 2;  // FN=01 HDLC indication for TDI receiver
    _bit PN             : 6;  // Phy Number
    _bit SC             : 8;  // Serial Counter
};


#else
/*  Fr Status1 register structure */
struct S_FrSerialStatusReg1
{
   _bit SC             : 8;  /* Serial Counter */
   _bit                : 3;
   _bit SO             : 5;  /* Serial Order */
   _bit FN             : 2;  /* FN=01 HDLC indication for TDI receiver */
   _bit PN             : 6;  /* Phy Number */
   _bit DataUnitLength : 8;  /* Data unit length */
};

#define S_FrTxSerialStatusReg1 S_FrSerialStatusReg1
#define S_FrRxSerialStatusReg1 S_FrSerialStatusReg1
#endif

/*  Fr Status1 register structure */
struct S_FrTcrStatusReg
{
   _bit tcrv           : 1;
   _bit ProtocolType   : 6;
   _bit Bz             : 1;  /* Base size selector */
   _bit Base           : 5;  /* Base pointer to the TA RAM */
   _bit                : 3;
   _bit SC             : 8;  /* Serial Counter */
   _bit                : 8;
 
};



struct S_FrTcrStatusReg1
{

   _bit PN             : 16 ;  /* Phy Number */
   _bit FlowNu         : 16 ;  /* FlowNu     */
};

struct S_FragHeader
{
   _bit B              : 1 ; /* Begin - First fragment */
   _bit E              : 1 ; /* End   - Last fragment */
   _bit C              : 1 ; /* Control bit */
   _bit SeqNuMsb       : 4 ; /* Sequence number MSB */
   _bit                : 1 ; /* Reserved (should always be set) */
   _bit SeqNuLsb       : 8 ; /* Sequence number LSB */
};



/* TDI FR statistics. Statistics per TDM for FR fifo */
struct S_TdiFrStats
{
   _bit                : 32;  /* Res */
   _bit                : 32;  /* Res */
   _bit                : 16;  /* Res */
   _bit RxOverrun      : 16;  /* Reciver Over run. */
   _bit TxUnderrun     : 16;  /* transmitter under run */
   _bit                : 16;  /* Res */
   _bit                : 32;  /* Res */
   _bit                : 32;  /* Res */
   _bit                : 32;  /* Res */
   _bit                : 32;  /* Res */
};


#ifndef DPS_LEVEL_0_TESTING_UTIL

/************ Frame Relay enums/constants ***********/
enum FR_STATS_TYPES
{
   HOST_RX_GOOD_FRAMES,
   CRC_ERROR,
   RX_ERROR,
   POS_EOP_ERROR,
   POS_OVERRUN_ERROR,
   POS_PARITY_ERROR,
   ADDRESS_MISMATCH,
   BUFFER_OVERRUN,
   TX_FRAMES,
   IW_FBP_UNDERRUN,
   IW_MRU_ERROR,
   IW_RX_GOOD_FRAMES,
   FRAG_ERROR,
   FR_DEL_FLOW,
   IW_FRAME_ERROR
};

enum FR_ERR_LOCATION
  {
    FR_RX_ERR_MID_FRAME,
    FR_RX_ERR_EOF
  };


enum FR_ORDER_MODE
  {
    FR_RX_ERR_NO_ORDER,
    FR_RX_ERR_RELEASE_ORDER_KEY
  };

enum FR_FRAME_TYPE
{
   FR_NON_FRAG_FRAME_TYPE,
   FR_FRAG_FRAME_TYPE
};

enum FR_STAT_TYPE
{
   FR_TX_CHANNEL_STAT,
   FR_TX_PHY_STAT,
   FR_RX_CHANNEL_STAT,
   FR_RX_PHY_STAT,
};

enum FR_BUFFER_TYPE
{
   FR_MIDDLE_BUFFER,
   FR_FIRST_BUFFER
};

enum FR_GET_KEY_WITH_ORDER
{
   FR_GET_KEY_NO_ORDER,
   FR_GET_KEY_ORDER
};

enum FR_NUMBER_OF_KEYS
{
   FR_ORDER_ONE_KEY,
   FR_ORDER_ONE_OR_TWO_KEYS
};

enum FR_SERIAL_TYPE
{
   TDI,
   POS
} ;

const RxBN      = 0x1;
const RxBsy     = 0x4;
const POS_ERROR = 0x3;
#endif

/************ Frame Relay Definitions ***********/
#define FR_ADV_TA_BASE_NU               0x00100000
#define FR_CHANGE_TX_STAT_BASE         (M_Mask(S_FrSerialStatusReg0.Bz)| FR_ADV_TA_BASE_NU)
#define FR_FCS_SIZE                     2
#define FR_RCPT_WRITE_SIZE              24
#define FR_RX_RW_RST_SIZE               24
#define FR_TX_RW_TST_SIZE               24
#define FR_MODE_FIELD_RCPT_OFFS         8
#define FR_STAT_FIELD_RCPT_OFFS         28     
#define FR_MIN_RX_BUFFER_SIZE           32
#define FR_RGPT_FIRST_WRITE_SIZE        20
#define FR_CRC_ERROR_OFFSET            (S_FrRxStTable.CrcError/8)
#define FR_RX_ERROR_OFFSET             (S_FrRxStTable.Abort/8)
#define FR_MAX_SDU_ERR_OFFSET          (S_FrRxStTable.FrameLengthErr/8)
#define FR_MIN_LEN_ERR_OFFSET          (S_FrRxStTable.FrameLengthErr/8)
#define FR_SERIAL_ERROR_OFFSET         (S_FrRxStTable.SerialError/8)
#define FR_PE_ERROR_OFFSET             (S_FrRxStTable.ParityErr/8)
#define FR_SOPEOP_ERROR_OFFSET         (S_FrRxStTable.SopEopErr/8)
#define FR_OVERRUN_ERROR_OFFSET        (S_FrRxStTable.SerialOverRun/8)
#define FR_ADDRESS_MISMATCH_OFFSET     (S_FrRxStTable.UnsupportedDLCI/8)
#define FR_LAST_DLCI_OFFSET            (S_FrRxStTable.LastDLCI/8)
#define FR_FRAG_HEADER_ERR_OFFSET      (S_FrRxStTable.HeaderError/8)
#define FR_RX_HOST_QUEUE_FULL          (S_FrRxStTable.HostQueueFull/8)
#define FR_RX_HEADER_ERROR_OFFSET      (S_FrRxStTable.HeaderError/8)
#define FR_NO_ERR_OFFSET               (S_FrRxStTable.NonOctetErr/8)
#define FR_FBP_EMPTY_OFFSET            (S_FrRxStTable.FbpEmpty/8)
#define FR_TOTAL_ERR_BYTE_OFFSET       (S_FrRxStTable.TotalErrBytes/8)
#define FR_FBP_TH_ERROR                 (FR_FBP_EMPTY_OFFSET + 1) /* offset indecation is also 
                                                                     used to designate a case for
                                                                     discard function 1 is added 
                                                                     to const to differ the two 
                                                                     cases of FBP drop */

// Tx statistics
#define FR_TX_UR_OFFSET                (S_FrTxStTable.SerialUnderRun/8)
#define FR_TX_BNR_OFFSET               (S_FrTxStTable.TxBNR/8)

// General defines
#define IW_CUT_THROUGH_BUFFER_ALIGNMENT 0x7f;
#define FR_FRAG_B_MASK                 (M_Mask(S_FragHeader.B)>>16)
#define FR_FRAG_E_MASK                 (M_Mask(S_FragHeader.E)>>16)
#define TDI_FR_TA_RAM_OFFSET            0x00200000
#define FR_RX_HOST_COUNTER_OFFS        (S_FrRcpt.HostQueueCnt/8)
#define FR_RX_LAST_BD_OFFS             (S_FrRcpt.LastBdPtr/8)
#define CRC16_CONST                     0x1D0F0000
#define CRC32_H_CONST                   0xC7040000
#define CRC32_L_CONST                   0x0000DD7B
#define FR_NULL_PARAMETER               0 /* This field is used in a macro        *
                                           * where a parameter field is redundant *
                                           * in that specific instanstiation      */

#define FR_TCPT_STATUS0_BITS_OFFSET     M_ByteToBit(M_BitToByte(S_FrTcpt.GCHD % 32)) /* find the octet number of the status0 bits in the TCPT */

#endif // #define _FRAME_RELAY_H_
/*****************************************************************************
 * (C) Copyright Wintegra 2000-2003, All rights reserved.
 *****************************************************************************/
