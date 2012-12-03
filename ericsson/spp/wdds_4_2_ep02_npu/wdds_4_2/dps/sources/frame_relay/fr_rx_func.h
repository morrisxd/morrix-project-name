/*****************************************************************************
 * (C) Copyright Wintegra 2000-2010.  All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************
/****************************************************************************
 *
 * File: fr_rx_func.h
 *
 * Description: Functions and Macros used by Frame Relay Receiver.
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
 * / CVS information /  $RCSfile: fr_rx_func.h,v $ $VER$
 ****************************************************************************/

/************************************************************************************************
 ********************************       Functions        ****************************************
 ************************************************************************************************/


section Fr_Rx_Functions
{


/*****************************************************************************
    *
    * Macro Name: FrRxBufferChaining
    *
    * Description:
    *
    *
    * Inputs: 
    *
    * Outputs:
    *    
    *    
    *
    * Modified and saved registers:
    *     -
    *****************************************************************************/

   inline  func B_FrRxBufferChaining(in P_ErrorLable,
                                     in RGPT,
                                     in P_mode1)
   {
      register                       TempDiff             =  r10;
      register                       TempOffset           =  r10;
      register                       TempFBPnum           =  r10;
      register                       ByteOffset           =  r12;
      register                       CurrentBuffersInFifo =  r12;
      register                       CurrentBdPtr         =  r13;
      register                       L2ProtocolSpecific   =  r15;
      register struct S_MatrixBd     NewBdPtr             =  r80;
      register struct S_MatrixBd     CurrentBd            =  r84;



      CurrentBdPtr = RCPT.BdPtr;
      TempFBPnum = RGPT.FBPnum;


      NewBdPtr = _fifo(r0,  TempFBPnum, CurrentBuffersInFifo, sizeof(S_MatrixBd), FBP_PARM_BUS);

      if(status0.AccErr)
      {

#pragma outofline;


         P_mode1.FbpEmpty = 1;
         F_FrRxErrFrameLastPac(FrRcptKey,
                               status0,
                               RcptOffset,
                               DataLength,
                               FlowNu,
                               mode0,
                               FrameLength,
                               RGPT,
                               phy,
			                      FR_RX_ERR_MID_FRAME,
                               Rrp,
                               RCPT,
                               FrMode1,
                               FrIwHostKey);

      }

      RCPT.FbpCounter =  _bf(CurrentBuffersInFifo,0,16);

      if(mode0.MTBD)
      {
         /* more than one buffer has already been used by this point */


         ByteOffset =  RGPT.RBS;
         ByteOffset -= (Rrp + DataLength);

         L2ProtocolSpecific = _wbs(_bf(mode0,0,16));
         L2ProtocolSpecific  >>= 9;
         L2ProtocolSpecific &= (M_Mask(S_FrRxBd.FECN)|
               M_Mask(S_FrRxBd.BECN)|
               M_Mask(S_FrRxBd.DE)|
               M_Mask(S_FrRxBd.CR));


         /* set the pointer to the start of the data */
         RCPT.DataPtr -= ByteOffset;

         M_CloseBufferChainingBd(CurrentBd,
                                 NewBdPtr.NextBdPtr,
                                 RCPT.BdPtr,
                                 RCPT.DataPtr,
                                 ByteOffset,
                                 S_FrRcptMode,
                                 RCPT.ModeField,
                                 TempFBPnum,
                                 MIDDLE_BD,
                                 L2ProtocolSpecific);
      }
      else
      {
         /* Current buffer to be closed is first buffer in frame */


         /* NextBdPtr holds ptr to the start of the data,
            DataPtr holds ptr to the current location of data */
   	   NewBdPtr.U_ProtocolSpecific.MatrixBuffChainInfo = 0;
	
         ByteOffset = RCPT.DataPtr - RGPT.FirstDataPtr;
         if ((TempDiff = NewBdPtr.NextBdPtr - RCPT.BdPtr)>=0)
         {
            _bf($reg(NewBdPtr.U_ProtocolSpecific.MatrixBuffChainInfo.DiffPtr),0,24) =_bf(TempDiff,8,24);

         }
         else
         {
            TempDiff = RCPT.BdPtr - NewBdPtr.NextBdPtr;

            _bf($reg(NewBdPtr.U_ProtocolSpecific.MatrixBuffChainInfo.DiffPtr),0,24) =_bf(TempDiff,8,24);

            NewBdPtr.U_ProtocolSpecific.MatrixBuffChainInfo.Sign = 1; // setting sign bit
         }



         NewBdPtr.U_ProtocolSpecific.MatrixBuffChainInfo |=  ByteOffset;
         TempOffset = CurrentBdPtr + (S_MatrixBd.U_ProtocolSpecific.MatrixBuffChainInfo)/8;
         _mem(r0,TempOffset,sizeof(S_MatrixBuffChainInfo),FBP_PARM_BUS) =  NewBdPtr.U_ProtocolSpecific.MatrixBuffChainInfo;

         mode0.MTBD = 1;

      }

      /* fetch new bd and reset RRP */
      Rrp = RGPT.RBS - DataLength;

      RCPT.BdPtr = NewBdPtr.NextBdPtr;

      RCPT.DataPtr = NewBdPtr.DataPtr;
      RCPT.DataPtr &= IW_DATA_BUFFER_ALIGNMENT;

   } /* B_FrRxBufferChaining */




   /*****************************************************************************
    *
    * Macro Name: M_OrderRgptMasKey
    *
    * Description:
    *
    *     Creates serial tag for RGPT key and FrMasterKey according to serial
    *     number and phy number. status1 is used to ensure order of get_key
    *     Orders both keys according to the above tags.
    *     The FrRgptKey will provide ordering and coherency on RGPT table parameters.
    *     FrMasterKey - This key will provide order for a later per flow get_key operation
    *
    *
    * Inputs: P_Status_Reg - key for order of getkey (serial nu), phy
    *
    * Outputs:
    *     FrRgptKey - key for locking RGPT table
    *     FrRcptKey - key for ordering additioal per flow keys.
    *
    * Modified and saved registers:
    *     -
    *****************************************************************************/
   inline func M_OrderRgptMasKey(in P_Status_Reg,
                                 in phy,
                                 in P_GET_KEY_WITH_ORDER,
                                 out FrRgptKey,
                                 out FrMasterKey)
      {
      // Prefer  keys, copy serial number and phy number to
         // key, set SerialMode key bit, and set specific
      // serial key label.
         FrRgptKey = (P_Status_Reg.SC) <<8;
         _bf(FrRgptKey, 0, 16) = phy;

         FrMasterKey = FrRgptKey;
         FrMasterKey |= ( (SERIAL_TAG_LABEL_TYPE_FR_MASTERKEY << 2) |
                          M_Mask(S_Tag.U_TagFields.SerialTag.SerialMode));
         FrRgptKey |= ( (SERIAL_TAG_LABEL_TYPE_HDLC_RCPT << 2) |
                        M_Mask(S_Tag.U_TagFields.SerialTag.SerialMode));

         // get two keys use (optionally) the order_id/counter in P_Status_Reg  to
         // ensure order of get operation
         if (P_GET_KEY_WITH_ORDER == FR_GET_KEY_ORDER)
         {
            _get_key(P_Status_Reg, FrRgptKey, 2,_ORDER_UNLOCK);
         }
         else
         {
            _get_key(r0, FrRgptKey, 2,0);
         }
      }


   /*****************************************************************************
    *
    * Macro Name: M_OrderRptKey
    *
    * Description:
    *
    *     Creates label tag for RCPT key according to  Flow number.
    *     Orders key according to the above tags. (FrMasterKey is released)
    *     The FrRcptKey will provide ordering and coherency on RCPT table parameters.
    *
    * Inputs:  FrMasterKey, FlowNu, status0,
    *
    * Outputs:
    *     FrRcptKey   - key for locking RCPT table
    *****************************************************************************/
   inline func M_OrderRptKey(in FrMasterKey,
                             in FlowNu,
                             in status0,
                             out FrRcptKey)
      {
         // Preper  keys, flow number to key
         // set labelMode key bit, and set specific
         // lebel key label.
         FrRcptKey = FlowNu << 16;

         // Set the Tag base mode bit and insert the RCPT base number
         FrRcptKey |= ((LABEL_TAG_LABEL_FR_RCPT<<8) |
                       M_Mask(S_Tag.U_TagFields.LabelTag.LabelMode));
         // get Rcpt Key
         _get_key(FrMasterKey,FrRcptKey, 1, _ORDER_UNLOCK);

      }

   /*****************************************************************************
    *
    * Macro Name: M_OrderIwHostKey
    *
    * Description:
    *
    *     Creates label tag for FrIwHostKey according to  Flow number.
    *     Orders key according to the above tag.
    *     Key ordering is performed whithin Rcpt lock.
    *     FrIwHostKey - This key is ordered only for the last packet in a frame,
    *     In host operation it is used to ensure order while linking received
    *     frame to host queue, In IW operation it is used to ensure order of
    *     L3 processing (the key is used as a master key to fetch further keys)
    *     (IwHostkey tag is also copied to IwMasterKey3Tag)
    *
    * Inputs:  FlowNu, status0,
    *
    * Outputs:
    *     FrIwHostKey - IW master key / Host key for linking new buffer
    *
    *****************************************************************************/
   inline func M_OrderIwHostKey(in  FlowNu,
                                in  status0,
                                out FrIwHostKey,
                                out IwMasterKey3Tag)
      {
         // Preper  keys, flow number to key
         // set labelMode key bit, and set specific
         // lebel key label.
         FrIwHostKey = FlowNu << 16;

         // Set the Tag base mode bit and insert the RCPT base number
         FrIwHostKey |= ((LABEL_TAG_LABEL_FR_IW_HOST<<8) |
                         M_Mask(S_Tag.U_TagFields.LabelTag.LabelMode));

         /* Save the original interworking tag for the IW background task */
         IwMasterKey3Tag = FrIwHostKey;
         _get_key(r0, FrIwHostKey, 1);
      }


   inline func B_FrBufferChainFcs(in P_DataLen,
                                  in P_mode,
                                  in RCPT)
   {
         register           TempReadData  = r10;


	 if (!FrMode1.POS) 
     { 
         RCPT.DataPtr -= DataLength;
         TempReadData =_data_mem(r0,RCPT.DataPtr,DataLength);
         if (!P_mode.CRCT)
         {
            _data_mem(r0,RCPT.DataPtr,DataLength,_BIT_SWAP|_CRC16) = *tar0;
            _data_mem(r0,RCPT.DataPtr++,DataLength)= TempReadData;
         }
         else
         {
            _data_mem(r0,RCPT.DataPtr,DataLength,_BIT_SWAP|_CRC32|_LAST_BUF) = *tar0;
            _data_mem(r0,RCPT.DataPtr++,DataLength)= TempReadData;
         }
     } 


	 /* Data length incremented to "include the complete FCS" code will later
	    remove FCS length. north was already incremented. Rrp has a negative value
	    later in code this negative value would be subtracted from RBS and then 
	    corrected with FCS to provide correct value */
       if (P_mode.MTBD)
       {
        	 /* to compensate on B_FrRxLastPacket code */
        	 RCPT.DataPtr += DataLength;
       }


   } /* B_FrBufferChainFcs */

   /*****************************************************************************
    *
    * Macro Name:  B_RxFRCheckFirstState
    *
    * Description:  This macro checks the FR headr, updates the RGPT
    *               and calls the DLCI lookup module.
    *
    *
    * Inputs:  TempData, mode0, tar0, FrMode1, FrMasterKey, status0,
    *          FrRgptKey, RgptOffset, phy, DataLength
    *
    * Outputs: FBecnDeTmp, HeaderSize,  RcptOffset, FlowNu
    *
    * Modified registers: RGPT
    *
    *****************************************************************************/
   inline func B_RxFRCheckFirstState(in TempData,
                                     in mode0,
                                     in tar0,
                                     in FrMode1,
                                     in FrMasterKey,
                                     in status0,
                                     in FrRgptKey,
                                     in RgptOffset,
                                     in phy,
                                     in  DataLength,
                                     inout RGPT,
                                     out FBecnDeTmp,
                                     out HeaderSize,
                                     out RcptOffset,
                                     out FlowNu)
      {

         // Local registers
         register struct S_FrHeader FrHeader        = r12;
         register                   P_ErrorTypeOffs = r20;

      FrHeader = _bf(TempData,0,32);
         if (!FrMode1.GlobalMode.LookUpDis)
         {
            //***************************************************/
            //*******          DLCI LookUp           ************/
            //***************************************************/
            // Check if Frame header format is legal
         if (TempData & M_Mask(S_FrHeader.EA0))
	    {
            P_ErrorTypeOffs  =  FR_RX_HEADER_ERROR_OFFSET;
                  goto FR_Rx_IleaglHeader;
                }               




            /* Extract DLCI from FR header and find Flow number */
         B_FR_Rx_DLCI_LkUp(DataLength,
                              FrHeader,
                              FrMasterKey,
                              FrRgptKey,
                              FrMode1,
                              status0,
                              mode0.PBT,
                              RgptOffset,
                              phy,
                              RGPT,
                              FBecnDeTmp,
                              HeaderSize,
                              RcptOffset,
                              FlowNu);


         }
         else
         {
            //***************************************************/
            //*******       Bypass DLCI LookUp       ************/
            //***************************************************/
#pragma outofline;


            RGPT.FlowNu = RGPT.MinDlci;
            FBecnDeTmp = 0x0000;
            HeaderSize = 0x02;
            if (!FrMode1.GlobalMode.TwoByteFr)
            {
               HeaderSize += 2;
            }
            FlowNu = RGPT.FlowNu;


            if (FlowNu == phy)
	    {

               // Channel was deactivated, flow hunt mode set by
               // Fr rx delete host command, release keys and Terminate

               // Optionally update mismaatch statistic for each frame
               // received while channel (single channel running over phy)
               // is disabled
               if (FrMode1.GlobalMode.PSTE)
               {
                  M_UpdateRxStatistics(FrMasterKey,
                                       FR_ADDRESS_MISMATCH_OFFSET,
                                       phy,
                                       status0,
                                    DataLength,
                                       _ORDER_UNLOCK,
                                       FR_RX_PHY_STAT);
               }
              else
               _task_switch(FrMasterKey , _ORDER_UNLOCK);

               $reg(RGPT.ModeField) &= ~M_Mask(S_FrRgpt.ModeField.OutF);
               _mem(FrRgptKey,B_FrRCPT[status0][RgptOffset],4,
                    EHP_CPT_BUS|_ORDER_UNLOCK | _TERMINATE) = *(&($reg(RGPT.ModeField)));
         }
         }
      }

   /*****************************************************************************
    *
    * Macro Name:  B_FrUnsupportedDlci
    *
    * Description:  This macro discards frames with unsupported DLCIs.
    *               or ilegal FR headrs.
    *
    * Inputs:  status0, FrRgptKey, RgptOffset, phy, FrMode1, status1
    *          FrMasterKey,  FlowNu, DLCITemp, P_ErrorTypeOffs, P_DataLength
    *
    * Modified registers: RGPT
    *
    *****************************************************************************/
   inline func  B_FrUnsupportedDlci(in    P_DataLength,
                                    in    status0,
                                    in    phy,
                                    in    FrRgptKey,
                                    in    RgptOffset,
                                    in    FrMode1,
                                    in    status1,
                                    in    FrMasterKey,
                                    in    FlowNu,
                                    in    DLCITemp,
                                    in    P_ErrorTypeOffs,
                                    inout RGPT)
      {
        // local register
        register        NullParameter = r80 ;
        

         // Release RGPT key
         if (!status0.Last )
         {
            // First Packet of a new frame
            // Update RGPT
            // Flow number is inited with phy number so that RGPT will
            // act as the RCPT for the current unsuppored frame
            // untill Flow Hunt mode is exited
            $reg(RGPT.ModeField) &= ~M_Mask(S_FrRgpt.ModeField.OutF);
	    $reg(RGPT.ModeField)|= M_Mask(S_FrRgpt.ModeField.FlowHunt); /* added by Jane to resolve the bus error issues caused by phy = FlowNu FlowHunt in RGPT should be always set to 1 in RGPT, for some unknown case, this bit was cleared, so correct it, when it will be used */ 
            RGPT.FlowNu = phy;
            _mem(FrRgptKey,B_FrRCPT[status0][RgptOffset],4,
                 EHP_CPT_BUS|_ORDER_UNLOCK) = *(&($reg(RGPT.ModeField)));
         }
         else
         {
            // Last Packet of a new frame
            // no need to update global
            _task_switch(FrRgptKey, _ORDER_UNLOCK);
         }

         // Updated statistics and terminate
         F_FrRxDiscardFrame(RGPT,
                            FrMode1,
                            status1,
                            FrMasterKey,
                            FlowNu,
                            phy,
                            status0,
                            DLCITemp,
                            P_ErrorTypeOffs,
                            P_DataLength,
                            NullParameter);
      }



   /*******************************************************************
    *
    * Macro Name:  B_FR_FetchFirstBuff
    *
    * Description: This macro fetches a new BD from the FBP,
    *              If the FBP is empty it will try to fetch the
    *              BD from the COMMON IW FBP, if this FBP is also
    *              empty the frame would be discarded and the thread
    *              terminated.
    *
    * Inputs: RGPT, status1, FrMasterKey, FlowNu, phy,  P_FRAME_TYPE
    *     -   FrRcptKey, status0, RcptOffset, FrIwHostKey, P_length
    *
    * Outputs: RBD,  FrFbpCnt, TempIwbt
    *     -
    * Modified  registers: RCPT, mode1, status0,  mode0, P_FrameLen
    *
    ********************************************************************/
   inline func B_FR_FetchFirstBuff(in    P_length,
                              in    RGPT,
                              in    status1,
                              in    FrMasterKey,
                              in    FlowNu,
                              in    phy,
                              in    FcsSize,
                              inout RCPT,
                              inout status0,
                              inout FrMode1,
                              out   RBD,
                              out   FrFbpCnt,
                              out   TempIwbt)
     {
       // Local register
        register     ErrorInformation   = r22;
      register     TempFbpNu          = acc;
       
       
      TempFbpNu = RGPT.FBPnum;
      *(&RBD) = _fifo(r0, TempFbpNu, FrFbpCnt,8 , FBP_PARM_BUS);
       
       // Extract the Data BUS type from RGPT and update it in mode0
      // (Update performed at end of macro)
       TempIwbt = _bf($reg(RGPT.RgptStatus.DBT),16,8);
       TempIwbt &= M_Mask(S_FrRcptMode.DBT);
       
      if (status0.AccErr)
       {


#pragma outofline;
          
          

             // There were no buffer in COMMON FBP, Frame is discarded
         // and thread terminated	(note that in this instantiation of
             // F_FrRxDiscardFrame the parameter ErrorInformation is not required)


         // Release RGPT key
         if (!status0.Last )
         {
             // First Packet of a new frame Update RGPT
             $reg(RGPT.ModeField) &= ~M_Mask(S_FrRgpt.ModeField.OutF);
             _mem(FrRgptKey,B_FrRCPT[status0][RgptOffset],FR_RGPT_FIRST_WRITE_SIZE,
             EHP_CPT_BUS|_ORDER_UNLOCK) = *(&($reg(RGPT.ModeField)));
         }
         else
             {
             // Last Packet of a new frame
             // no need to update global
             _task_switch(FrRgptKey, _ORDER_UNLOCK);
         }

         /* First buffer of frame */
                F_FrRxDiscardFrame(RGPT,
                                   FrMode1,
                                   status1,
                                   FrMasterKey,
                                   FlowNu,
                                   phy,
                                   status0,
                                   ErrorInformation,
                                   FR_FBP_EMPTY_OFFSET,
                            r0,
                                   RBD);

             }
             

     }
   
   
   /*******************************************************************
    *
    * Macro Name:  B_FR_CloseRxBd
    *
    * Description:
    *
    *     Closes the current buffer descriptor with received data.
    *     Update crrent buffer BD in external memory.
    *     Read and update last pointer in RCPT. link
    *     buffer to last buffer in rx queue and update host queue
    *     counter
    *
    * Inputs:  P_CurrentBdPtr , P_Length, in P_FrProtocolBits,
    *          FrMode1, FrIwHostKey, RcptOffs, DataPtr
    *          status0, IWExControlWord
    *
    * Outputs: RBD
    *
    ********************************************************************/
   inline func B_FR_CloseRxBd(in P_CurrentBdPtr,
                              in P_Length,
                              in P_FrProtocolBits,
                              in FrMode1,
                              in mode0,
                              in FrIwHostKey,
                              in RcptOffs,
                              in DataPtr,
                              in status0,
                              in IWExControlWord,
                              out RBD)
      {
         // Local Register
         register   BdStatusAdd     = r11;
         register   LastBD          = r12;
         register   BdError         = r17;
      register   BdWriteSize     = r17;

         // Update FECN, BECN, DE and CR bits in RBD
         $reg(RBD.FECN) = P_FrProtocolBits;

         /* set V bit */
      $reg(RBD.V) = (M_Mask(S_FrRxBd.V)|
		     M_Mask(S_FrRxBd.E));


         // in an event of an error update error type in BD
         if (FrMode1.RxError)
         {
            if (!status0.U_SerialStatus.Rx.U_Bit10.NO)
            {
            BdError = FrMode1 & (M_Mask(S_FrRxMode1Field.SOP_EOP_E)|
                     M_Mask(S_FrRxMode1Field.PE)|
                  M_Mask(S_FrRxMode1Field.FbpEmpty)|
                     M_Mask(S_FrRxMode1Field.AbrtPosErr)|
                     M_Mask(S_FrRxMode1Field.MLE)|
                     M_Mask(S_FrRxMode1Field.CrcE));
               // Move error bit to correct location in BD
               BdError = BdError << 22;
               $reg(RBD.V) |= BdError;

            }
            else
               $reg(RBD.V) |= M_Mask(S_FrRxBd.NO);
         }


      BdWriteSize = sizeof(S_MatrixBd);

     
      if (mode0.MTBD)
      {
	      /* Update first BD of frame */

         RBD.MTBD = 1;
	      $reg(RBD.E) &= ~M_Mask(S_FrRxBd.E);
         BdWriteSize -= sizeof(S_MatrixBd.U_ProtocolSpecific.MatrixBuffChainInfo);
      }

         /* clear BD's next BD pointer */
         RBD.BdPtr &= ~M_Mask(S_FrRxBd.BdPtr);
         RBD.Length = P_Length;

      $reg(RBD.V) |= (mode0 & M_Mask(S_FrRcptMode.DBT)) <<
         (M_ShiftLeft(S_MatrixBd.MatrixBdStatus.DaBus) - M_ShiftRight(S_FrRcptMode.DBT));

      $reg(RBD.V) |= (RGPT.FBPnum << M_ShiftLeft(S_LinkIwTBD.FbpNumber));

         /* save the BD after updating the actual data length in that BD */
      if (mode0.MTBD)
      {
         RBD.DataPtr = RGPT.FirstDataPtr;
         _mem(r0, RGPT.FirstBdPtr, BdWriteSize ,FBP_PARM_BUS) = *(&RBD);
         LastBD = RGPT.FirstBdPtr;
      }
      else
      {
         RBD.DataPtr = DataPtr;
         _mem(r0, P_CurrentBdPtr, BdWriteSize ,FBP_PARM_BUS) = *(&RBD);
         LastBD = P_CurrentBdPtr;
      }

         // Update last BD pointer. Read previous last BD, and replace with current BD.
         RcptOffset += FR_RX_LAST_BD_OFFS;
         LastBD = _mem(FrIwHostKey,B_FrRCPT[status0][RcptOffset],4,
                       EHP_CPT_BUS | _BLOCK_SWAP | _ORDER_LOCK);

      if (mode0.MTBD)
      {
         // add BD to queue
         _mem(FrIwHostKey, LastBD, 4, _ORDER_UNLOCK | FBP_PARM_BUS) = RGPT.FirstBdPtr;
      }
      else
      {
         // add BD to queue
         _mem(FrIwHostKey, LastBD, 4, _ORDER_UNLOCK | FBP_PARM_BUS) = P_CurrentBdPtr;
      }

         RcptOffset += (FR_RX_HOST_COUNTER_OFFS - FR_RX_LAST_BD_OFFS);
      _mem(r0,B_FrRCPT[status0][RcptOffset],EHP_CPT_BUS | _HALF_WORD) += 1;
      }


   /*****************************************************************************
    *
    * Function Name: F_FrRxDiscardFrame
    *
    * Description:
    *
    *     This block handles error states were the complete frame should be
    *     discarded, this may either be due to the FBP being empty,
    *     or an unsupported DLCI.
    *     Flow hunt mode state is entered
    *     statistics table updated, RCPT saved and thread terminated.
    *
    * Inputs: RGPT, status1, FrMode1, FrMasterKey, FlowNu, phy, status0, RGPT
    *         P_UnsupportedDLCI
    *
    *****************************************************************************/
   static func F_FrRxDiscardFrame(in struct S_FrRgpt RGPT = r72,
                                  in struct S_FrRxMode1Field FrMode1 = r1,
                                  in struct S_FrRxSerialStatusReg1 status1 = STATUS1_REG,
                                  in FrMasterKey = TagKey4,
                                  in FlowNu = r28,
                                  in phy = r36,
                                  in struct S_FrSerialStatusReg0 status0 = r7,
                                  in P_UnsupportedDLCI = r22,
                                  in P_ErrType = r20,
                                  in P_DataErrLength = r112,
                                  in P_ReturnBdPtr = r80)
      {
         // local registers
         register FrIwHostKey          = TagKey1;
         register FrRcptKey            = TagKey2;
         register RcptOffset           = r10;
      register TempFBPnum           = r11;

         // RCPT is read localy by function
         register struct S_FrRcpt RCPT = r56;

         // Fetch keys
           M_OrderRptKey(FrMasterKey,
                         FlowNu,
                         status0,
                         FrRcptKey);               
      
         
      if (P_ErrType == FR_FBP_EMPTY_OFFSET)
           {
             RcptOffset = FlowNu * sizeof(S_FrRcpt);

                 // FBP Error - no buffers to receive new frame
                 
                 // Read status word for RCPT, to see if channel 
		 // statistics and INTM are enabled
                 RcptOffset += FR_STAT_FIELD_RCPT_OFFS;
                 $reg(RCPT.INTM) = _mem(r0,B_FrRCPT[status0][RcptOffset],4,EHP_CPT_BUS);
                 _bf(status0, 24, 8) = _bf($reg(RCPT.INTM),0,8);
                 
                 RcptOffset -= (FR_STAT_FIELD_RCPT_OFFS - FR_MODE_FIELD_RCPT_OFFS);
             
             _mem(FrRcptKey,B_FrRCPT[status0][RcptOffset],EHP_CPT_BUS|
                  _HIGH_WORD |_ORDER_UNLOCK)|= M_Mask(S_FrRcpt.ModeField.FlowHunt);
             
             /* issue the Rx buffer underrun interrupt
                (FBP empty, FBP 15 empty too)*/
                 if ( status0.U_SerialStatus.Dps.OvUnFlowIntm )
                   {
                     M_Interrupt(status1,
                                 PROTOCOL_TYPE_FR,
                                 RxBN,
                                 FlowNu,
                                 NULL_CID);
                   }
                 
            
                 if (status0.U_SerialStatus.Dps.STE)
                   {
                     // Update flow statisics error counter
                     M_UpdateRxStatistics(r0,
                                          P_ErrType,
                                          FlowNu,
                                          status0,
                                          FR_NULL_PARAMETER,
                                          FR_NULL_PARAMETER,
                                          FR_RX_CHANNEL_STAT);
                   }
           }
         else
      {
           // Unsupported DLCI
           _task_switch(RcptKey,  _ORDER_UNLOCK);
      }


         // Update port statisics
         if (FrMode1.GlobalMode.PSTE)
         {
            // Update Port statisics error counter
            M_UpdateRxStatistics(r0,
                                 P_ErrType,
                                 phy,
                                 status0,
                                 P_DataErrLength,
                                 FR_NULL_PARAMETER,
                                 FR_RX_PHY_STAT);

            if (P_ErrType ==  FR_ADDRESS_MISMATCH_OFFSET)
            {
               B_WriteRxStatistics(FR_LAST_DLCI_OFFSET,
                                   phy,
                                   status0,
                                   P_UnsupportedDLCI);
            }
         }

         _task_switch(r0, _TERMINATE);
      }


   /*****************************************************************************
    *
    * Macro Name:  M_UpdateRxStatistics
    *
    * Description:  Updates the corresponding counter in the statistics table
    *
    * Inputs: statistics type (parameter), P_TableNu, status0 ,P_Attribute
    *
    *****************************************************************************/
   inline func M_UpdateRxStatistics(in P_Key,
                                    in P_type,
                                    in P_TableNu,
                                    in status0,
                                    in P_PacketLength,
                                    in P_Attribute,
                                    in P_TableType)
      {
         // Local register
         register offset    =  r10;

         offset = P_TableNu*sizeof(S_FrRxStTable);

         if (P_TableType == FR_RX_PHY_STAT)
         {
            // Update Byte counter
            M_FrRxUpdateErrByteCnt(status0,
                                   offset,
                                   P_PacketLength);
         }
         offset +=  P_type;
         _mem(P_Key, B_FrRxStat[status0][offset],  _HALF_WORD | STATISTICS_BUS | P_Attribute) += 1;
      }


   /*****************************************************************************
    *
    * Macro Name B_WriteRxStatistics
    *
    * Description:
    *
    *     Writes the error parameter field to the approptiate entry of the
    *     Statistics table (used to write last unknown DLCI)
    *
    * Inputs: statistics type (parameter), P_TableNu, status0, P_ErrorValue
    *
    * Outputs:
    *
    * Modified and saved registers:
    *
    *****************************************************************************/
   inline func B_WriteRxStatistics(in P_type,
                                   in P_TableNu,
                                   in status0,
                                   in P_ErrorValue)
      {
         register offset = r10;

         offset = P_TableNu*sizeof(S_FrRxStTable);
         offset +=  P_type;
         _mem(r0, B_FrRxStat[status0][offset],4, STATISTICS_BUS) = P_ErrorValue;
      }


   /*****************************************************************************
    *
    * Macro Name:  B_FrRxLastPacket
    *
    * Description: This block handles the last packet of cell.
    *              The block of code will first parse the frame
    *              prepering internal and external words
    *              a decision will also be made if to pass the
    *              frame to L3 processing or to enqueue it to host
    *              queue (IW mode only).
    *
    *              The RCPT is next saved and then the data is written
    *              to buffer (CRC is always calculated).
    *              The frame is checked for errors and either passed to L3
    *              or enqued to host termination queue.
    *
    * Inputs:     mode0, DataLength, tar0, FlowNu, HeaderSize,  status0,
    *             FrRcptKey, RGPT, RcptOffset, FrIwHostKey, status1, phy
    *             Rrp
    * Outputs:    CurrentBdPtr, L2HeaderSize
    *
    * Modified and saved registers: RCPT, FrMode1
    *
    *****************************************************************************/
   inline func B_FrRxLastPacket(in    mode0,
                              in    DataLength,
                              in    tar0,
                              in    FlowNu,
                              in    phy,
                              in    HeaderSize,
                              in    status0,
                              in    FrRcptKey,
                              in    RGPT,
                              in    RcptOffset,
                              in    FrIwHostKey,
                              in    status1,
                              in    Rrp,
                                in    P_FcsSize,
                              inout RCPT,
                              inout FrMode1,
                              out   CurrentBdPtr,
                              out   L2HeaderSize)
      {
         // local register

     register                       CurrentDataPtr    = r10;
     register                       ByteOffset        = r12;
         register InterworkingType    =  r26;
     register struct S_LinkIwTBD    IwLastBd          = r64 ;


      /* In multi buffer operation BD pointer in RCPT is overwritten    *
       * so it must be copied aside, for reasons of code compatibility *
          * this is done here too                                         */
         CurrentBdPtr = RCPT.BdPtr;
         
         M_OrderIwHostKey(FlowNu,
                          status0,
                          FrIwHostKey,
                          IwMasterKey3Tag);
         
         /* save RCPT */
         RCPT.ModeField &= ~(M_Mask(S_FrRcptMode.FirstBuf)|
                             M_Mask(S_FrRcptMode.F));
         _mem(FrRcptKey, B_FrRCPT[status0][RcptOffset],FR_RCPT_WRITE_SIZE,
              EHP_CPT_BUS|_ORDER_UNLOCK) = *(&RCPT);
         
      /* in case Rrp is less than 0 , it means we had a chunk with only FCS
       * and we took care of the CRC, and updated DataPtr, no need to write the data
       * and also dont have place to */

#ifdef _MODULE_MATRIX_BUFF_CHAIN_ENABLE_
      if (Rrp >= 0)
      {
#endif

         // save data (and calculate CRC for TDI mode of operation)
         if (!mode0.CRCT)
            _data_mem(r0,RCPT.DataPtr++,DataLength,_BIT_SWAP|_CRC16) = *tar0;
         else
            _data_mem(r0,RCPT.DataPtr++,DataLength,_BIT_SWAP|_CRC32|_LAST_BUF) = *tar0;

#ifdef _MODULE_MATRIX_BUFF_CHAIN_ENABLE_
      }
#endif

         // Calculate length of current buffer
         BufferLength = FrameLength;

         // init data ptr to point to begin of buffer, (so encapsulation fields at begining
         // of buffer may be read, and so that dataptr in BD would not be corrupted when  BD
         // is updated

      DataPtr = RGPT.FirstDataPtr;

         
         if (!FrMode1.POS )
         {
	     BufferLength -= P_FcsSize;

            // Check if there was a CRC error, and
         // correct Frame length (subtract FCS size)
            M_FrRxCheckFcs(FR_NON_FRAG_FRAME_TYPE,
                           mode0,
                           status0,
                        P_FcsSize,
                           FrMode1,
                           RCPT.CrcTmp,
                           RCPT.Norf,
                           Rrp);

      }
#ifdef _MODULE_EX_MULTI_PHY2_ENABLE_
      else if (mode0.CrcRem)
      {
         /* When FR runs over emphy 2, the FCS size (2 or 4 bytes)
               should be removed from the frame length */
         BufferLength    -= P_FcsSize;
         FrameLength     -= P_FcsSize;
         $reg(RCPT.Norf) -= P_FcsSize;
         }
#endif
         
         // IWM Mode - Parse L2 header
         // Frame will be checked against expected encapsulation
         // and an appropriate decision made if to pass frame to
         // Interworking DPS (by setting IWF bit) or to pass frame
         // to host termination queue. (note that mode1 MSB are
         //loaded here with L2 table bits
         
         
         
         if (mode0.IWM)
         {
            M_FrL2interWorking(PKT_LAST,
                            P_FcsSize,
                               HeaderSize,
                               DataPtr,
                               FlowNu,
                               FrameLength,
                               BufferLength,
                               status0,
                               mode0,
                               mode1,
                               RCPT,
                               tar0,
                               L2IWT,
                               IwExControlWord,
                               IWExtentionWord,
                               L2HeaderSize,
                               InterworkingType);
            
            
         }
         
        Fr_Rx_Update_Last_Bd_Ptr:
         
         // Check if Frame payload contains at least one octet
         M_FrRxCheckMinLen(mode0,
			   status0,
                        P_FcsSize,
			   DataLength,
			   HeaderSize,
			   Fr_RxLastBypassSaveData,
			   FrMode1);
         
         
        Fr_RxLastBypassSaveData:
         
         // update FECN,BECN DE and CR in IWInControlWord.L2ProtocolSpecific
         IWInControlWord.U_Protocol.L2ProtocolSpecific = _wbs(_bf(mode0,0,16));
         IWInControlWord.U_Protocol.L2ProtocolSpecific = IWInControlWord.U_Protocol.L2ProtocolSpecific >> 9;
         IWInControlWord.U_Protocol.L2ProtocolSpecific &= (M_Mask(S_FrRxBd.FECN)|
                                                M_Mask(S_FrRxBd.BECN)|
                                                M_Mask(S_FrRxBd.DE)|
                                                M_Mask(S_FrRxBd.CR));

#ifdef _MODULE_MATRIX_BUFF_CHAIN_ENABLE_
      if (mode0.MTBD)
      {
         ByteOffset = RGPT.RBS - Rrp;
         CurrentDataPtr = RCPT.DataPtr - ByteOffset;
         ByteOffset -= P_FcsSize;



         M_CloseBufferChainingBd(IwLastBd,
                                 NULL_POINTER,
                                 RCPT.BdPtr,
                                 CurrentDataPtr,
                                 ByteOffset,
                                 S_FrRcptMode,
                                 mode0,
                                 RGPT.FBPnum,
                                 LAST_BD,
                                 IWInControlWord.U_Protocol.L2ProtocolSpecific);

      }
#endif


         if (mode0.IWF)
         {
            /******************************************************
             **********       InterWorked Frame        ************
             ******************************************************/
            if (!FrMode1.RxError)
            {
               // Valid IW frame
               /* initializes parameters of the internal and Ext control words *
                * used by the interworking module
                * (Frame Length extracted from RCPT by previous IWM macro*/
            Fr_RxIwInterface:

                // pass InterworkingType to M_L2toL3Interface instead of mode1.
               B_FrL2IwIntExtInterface(L2_MASK_PROTOCOL_TYPE_FR,
                                       RCPT,
                                    RGPT,
                                       InterworkingType,
                                       mode0,
                                       L2HeaderSize,
                                       DataPtr,
                                       CurrentBdPtr,
                                       RCPT.Norf,
                                       IWExControlWord,
                                       IWInControlWord);

               /* update FR Rx good frames statistics that pass to IW module */
               if (status0.U_SerialStatus.Dps.STE)
               {
                  M_FrRxUpdateValidStatics(FlowNu,
                                           status0,
                                           mode0,
                                           FR_NULL_PARAMETER,
                                           FrameLength);
               }
               if (FrMode1.GlobalMode.PSTE)
               {
                  M_FrRxUpdateValidStatics(phy,
                                           status0,
                                           mode0,
                                           FR_NULL_PARAMETER,
                                           FrameLength);
               }



#ifdef  _MODULE_INTERWORKING_ENABLE_
	       
               /* pragma no sync used at destination, hence need to ensure no
      		   restriction applies before jumping */
#pragma _sync;
               goto L_IWM_L2_ExtWordInTrsInterface; 

#else
               // IW module not part of package release key and terminater thread
               _task_switch(IwMasterKey3, _ORDER_UNLOCK |_TERMINATE);
#endif  /* end of package splitting tag _MODULE_INTERWORKING_ENABLE_ */

	    }// end of valid IW frame
       else
	    {
               /*************************************
                ***       Faulty IW frame        ****
                *************************************/
	    L_IgnoreIWError:

            F_FrRxErrFrameLastPac(FrRcptKey,
                                        status0,
                                  RcptOffset,
                                  FrameLength,
                                  FlowNu,
                                  mode0,
                                        FrameLength,
                                  RGPT,
                                  phy,
                                  FR_RX_ERR_EOF,
                                  Rrp,
                                  RCPT,
                                        FrMode1,
                                  FrIwHostKey);
                 
         }// end IW frame
         }
         else
         {
            /**************************************************
             ***********  Host termination frame      *********
             **************************************************/
            // Host termination

            // Check if Rx queue is not full
            /* note that RCPT was already saved we may corrupt the fields    *
             * near RCPT.MaxQueueSize and use the same register for the mask */
            $reg(RCPT.MaxQueueSize)  = $reg(RCPT.MaxQueueSize) & M_Mask(S_FrRcpt.MaxQueueSize);
         
            RCPT.HostQueueCnt - $reg(RCPT.MaxQueueSize);
            if (neg)
            {

                  /********************************
                ***    enqueue valid frame     ***
                   ********************************/

                  // Link Buffer to Rx Queue and terminate.
                  B_FR_CloseRxBd(CurrentBdPtr,
                                 BufferLength,
                                 IWInControlWord.U_Protocol.L2ProtocolSpecific,
                                 FrMode1,
                                 mode0,
                                 FrIwHostKey,
                                 RcptOffs,
                                 DataPtr,
                                 status0,
                                 IWExControlWord,
                                 RBD);

                  /* issue Rx interrupt */
                  if ( status0.U_SerialStatus.Dps.INTM )
                  {
                     M_Interrupt(status1,
                                 PROTOCOL_TYPE_FR,
                                 RxE,
                                 FlowNu,
                                 NULL_CID);
                  }

                  /* This function checks if statistics
                   * should be updated. then updates the appropriate
                   * statistic. The Function terminates thread. */
                  B_FrRxChkUpdateStatistics(FrMode1,
                                            mode0,
                                            phy,
                                            FlowNu,
                                            FrameLength,
                                            status0,
                                         P_FcsSize);

         } // End Rx queue not full
               else
               {
               /*************************************
                ** Errored Frame - Rx queue full  ***
                *************************************/
            /* Rx Queue full - overrun, function will terminate
             * after dropping frame
             */


               F_FrRxDiscardBuffer(RGPT,
                                   status1,
                                   mode0,
                                   FrMode1,
                                   FrIwHostKey,
                                   FlowNu,
                                   phy,
                                   RxBsy,
                                   FR_RX_HOST_QUEUE_FULL,
                                   RCPT.BdPtr,
                                   status0,
                                   RCPT.Norf);
            }
         }// end of host termination frame
      }

   /*****************************************************************************
    *
    * Macro Name:  B_RxMaxSduExcedded
    *
    * Description: Close a frame that exceded Max SDU, for host application
    *              buffer/frame would be closed with the amount of data received
    *              up to this stage, IW connection would discard the complete
    *              buffer if this is first (single) buffer of frame, or close
    *              current buffer with error/abort indication if a few buffers
    *              were already past to frame.
    *
    * Inputs:      FlowNu, status0, FrRcptKey, mode0, BufferLength
    *              DataLength
    * Outputs:     DataPtr, IWExControlWord, FrIwHostKey, IwMasterKey3Tag
    *              FrameLength, CurrentBdPtr, phy, L2HeaderSize
    *
    * Modified and saved registers: FrMode1, RCPT, Rrp
    *
    *****************************************************************************/
   inline func B_RxMaxSduExcedded(in    FlowNu,
                                  in    status0,
                                  in    FrRcptKey,
                                  in    RcptOffset,
                                  in    mode0,
                                  in    phy,
                                  in    DataLength,
                                  inout FrMode1,
                                  inout RCPT,
                                  inout Rrp,
                                  out   DataPtr,
                                  out   IWExControlWord,
                                  out   FrIwHostKey,
                                  out   IwMasterKey3Tag,
                                  out   FrameLength,
                                  out   CurrentBdPtr,
                                  out   L2HeaderSize)
      {
         // Local Registers
         register Rbs            =  r10;
         register BufferLength   =  Rrp;
 
         // Frame length was exceeded (norf is corrected for
         // for host frames to value before frame length was
         // exceeded
         $reg(RCPT.Norf) -= DataLength;
         FrameLength -= DataLength;
         FrMode1 |= (M_Mask(S_FrRxMode1Field.RxError)|
                     M_Mask(S_FrRxMode1Field.MLE));
         
         
         if (FrMode1.GlobalMode.PSTE)
         {
            M_FrRxUpdateErrByteCnt(status0,
                                   phy*sizeof(S_FrRxStTable),
                                   DataLength);
         }

         /* In multi buffer operation BD pointer in RCPT is overwriten    *
          * so it must be copied asside, for reasons of code competabilty *
          * this is done here too                                         */
         CurrentBdPtr = RCPT.BdPtr;

       
       
            /**************************************/
            /*** Close Frame - and Enqueu frame  **/
            /**************************************/
            // A buffer that is either not the first buffer
      // of frame or alternatively belongs to a host
            // application would be be closed with an error
            // (abort in IW indication) and enqueued/passed
            // to IW code.
            BufferLength = FrameLength;

            F_FrRxErrFrameLastPac(FrRcptKey,
                                  status0,
                                  RcptOffset,
                                  DataLength,
                                  FlowNu,
                                  mode0,
                                  FrameLength,
                            RGPT,
                            phy,
    			                FR_RX_ERR_MID_FRAME,
                                  Rrp,
                                  RCPT,
                                  FrMode1,
                            FrIwHostKey);

         }


   /*****************************************************************************
    *
    * Macro Name:  B_FrRxMiddlePacket
    *
    * Description: This macro saves the recieved data, calculates CRC16
    *              over the data, updates the RCPT and terminates thread.
    *
    * Inputs: DataLength, DataLength, tar0, FrRcptKey, status0, status1,
    *         FrIwHostKey, mode0, HeaderSize, FrameLength
    *
    * Outputs: CurrentBdPtr, DataPtr, L2HeaderSize,
    *
    * Modified and saved registers: RCPT, Rrp, BufferLength 
    *
    *****************************************************************************/
   inline func B_FrRxMiddlePacket(in    RGPT,
                                in    FrRcptKey,
                                in    FrIwHostKey,
                                in    mode0,
                                in    status0,
                                in    status1,
                                in    FlowNu,
                                in    phy,
                                in    tar0,
                                in    DataLength,
                                in    RcptOffset,
                                in    HeaderSize,
                                in    FrameLength,
                                inout Rrp,
                                inout RCPT,
                                inout BufferLength,
                                out   CurrentBdPtr,
                                out   DataPtr,
                                out   L2HeaderSize)
      {


         
         /* copy data buffer */
         if (!mode0.CRCT)
            _data_mem(r0,RCPT.DataPtr++,DataLength,_BIT_SWAP|_CRC16) = *tar0;
         else
            _data_mem(r0,RCPT.DataPtr++,DataLength,_BIT_SWAP|_CRC32) = *tar0;
         
      {
         /**************************************/
         /**     Middle of current Buffer     **/
         /**************************************/

         RCPT.Rrp = Rrp;
         RCPT.ModeField = mode0 & ~M_Mask(S_FrRcptMode.F);
         _mem(FrRcptKey,B_FrRCPT[status0][RcptOffset],FR_RCPT_WRITE_SIZE,
              EHP_CPT_BUS|_ORDER_UNLOCK|_TERMINATE) = *(&RCPT);
      }
         }
            
            
            
            
   /****************************************************************************
 *
 * Macro Name: B_HandleRxOverrun
 *
 * Description: This macro handles an event of a serial overrun (TDI only),
 *              an interupt is optionally raised, statistics updated and
 *               thread terminated
 *
 * Inputs: status0, status1 , phy
 *
 * Outputs:
 *
 * Modified and saved registers:
 *
 *****************************************************************************/
   inline func B_HandleRxOverrun(in status0,
                                 in status1,
                                 in phy)
      {
         // Local register
         register  offset  =  r10;

         /* set interrupt queue zero at the beginning */
         status0 &= ~M_Mask(S_FrRxStatus0.IRQ_Num);

         /* send Rx overflow interrupt if the Rx overflow interrupt mask bit set */
         if ( status0.U_SerialStatus.Rx.INTM )
         {
            M_Interrupt(status1,
                        PROTOCOL_TYPE_TDI_HDLC,
                        Overrun,
                        NULL_CHANNEL,
                        NULL_CID);
         }



         /* update FR statistics and terminate the task */
         // add terminate attribute
         M_UpdateRxStatistics(status1,
                              FR_OVERRUN_ERROR_OFFSET,
                              phy,
                              status0,
                              status1.DataUnitLength,
                              (_TERMINATE | _ORDER_UNLOCK),
                              FR_RX_PHY_STAT);
      }


   /*******************************************************************
 *
 * Macro Name:  B_FR_Rx_DLCI_LkUp
 *
 * Description: This macro translates the received DLCI into
 *              a 16 bit flow number
 *
 * Inputs: FrHeader, FrMasterKey, status0, P_PBT, RgptOffset, phy
 *         FrMode1, FrRgptKey, P_DataLength
 *
 * Outputs: FBecnDeTmp, HeaderSize, RcptOffset, FlowNu
 *
 * Modified and saved registers: RGPT
 *
 *******************************************************************/
   inline func B_FR_Rx_DLCI_LkUp(in P_DataLength,
                                 in FrHeader,
                                 in FrMasterKey,
                                 in FrRgptKey,
                                 in FrMode1,
                                 in status0,
                                 in P_PBT,
                                 in RgptOffset,
                                 in phy,
                                 inout RGPT,
                                 out FBecnDeTmp,
                                 out HeaderSize,
                                 out RcptOffset,
                                 out FlowNu)
      {
         // Local registers
         register            DatOffs         = r10;
         register            P_ErrorTypeOffs = r20;
         register            DLCITemp        = r22;
         register            HashIndexMask   = r114;


         // extract FECN, BECN and DE bits from Frheader
         FBecnDeTmp = $reg(FrHeader.FECN) & (M_Mask(S_FrHeader.CR)|
                                             M_Mask(S_FrHeader.FECN)|
                                             M_Mask(S_FrHeader.BECN)|
                                             M_Mask(S_FrHeader.DE));

         // Extract DLCI from FrHeader
         DLCITemp = FrHeader >> ($bitsize(S_FrHeader.CR)+
                                 $bitsize(S_FrHeader.EA0));
         _bf(DLCITemp,8,8) = _bf($reg(FrHeader.DLCI1),8,8);

         // Check if this a 2 or 4 byte header
         if (FrHeader & M_Mask(S_FrHeader.EA1))
         {
            /********************************************/
            /* second EA bit set - 2 byte header format */
            /********************************************/

            DLCITemp = DLCITemp >> ($bitsize(S_FrHeader.FECN)+
                                    $bitsize(S_FrHeader.BECN)+
                                    $bitsize(S_FrHeader.DE)+
                                    $bitsize(S_FrHeader.EA1)+
                                    $bitsize(S_FrHeader.DLCI2)+
                                    $bitsize(S_FrHeader.EA2)+
                                    $bitsize(S_FrHeader.DLCI3)+
                                    $bitsize(S_FrHeader.DC)+
                                    $bitsize(S_FrHeader.EA2));

            HeaderSize = 0x2;

            /* Discard frames whose header size does  *
             * not conform to configured size         */
            if (!FrMode1.GlobalMode.TwoByteFr)
            {
#pragma outofline;
               P_ErrorTypeOffs  =  FR_RX_HEADER_ERROR_OFFSET ;
               goto FR_Rx_IleaglHeader;
            }
         }
         else
         {
            /************************************************/
            /* second EA bit cleared - 4 byte header format */
            /************************************************/
            DLCITemp = DLCITemp >> ($bitsize(S_FrHeader.FECN)+
                                    $bitsize(S_FrHeader.BECN)+
                                    $bitsize(S_FrHeader.DE)+
                                    $bitsize(S_FrHeader.EA1));
            _bf(DLCITemp,16,8) =  _bf($reg(FrHeader.DLCI2),16,8);

            DLCITemp = DLCITemp >> $bitsize(S_FrHeader.EA2);
            _bf(DLCITemp,24,8) = _bf($reg(FrHeader.DLCI3),24,8);

            DLCITemp = DLCITemp >>  ($bitsize(S_FrHeader.DC)+
                                     $bitsize(S_FrHeader.EA2));
            HeaderSize = 0x4;
            /* Discard frames whose header size does  *
             * not conform to configured size         */
            if (FrMode1.GlobalMode.TwoByteFr)
            {
#pragma outofline;
               P_ErrorTypeOffs  =  FR_RX_HEADER_ERROR_OFFSET ;
               goto FR_Rx_IleaglHeader;
            }
         }


         if (!status0.U_SerialStatus.Rx.Addlm)
         {


            /*********************************************
             ****        Range Reduction Lookup       ****
             *********************************************/

            // The Recieved DLCI is first compared against
            // Minmum DLCI value, if it is larger (or equal)
            // The difference between this value is compared against
            // the number of supported flows.
            DatOffs = DLCITemp - RGPT.MinDlci;
            if (!neg)
            {
               if (DatOffs > RGPT.U_LkUpSize.RangeLkUp.MaxNuFlows) goto FR_Rx_unsupported_DLCI;
            }
            else
            {
               goto FR_Rx_unsupported_DLCI;
            }

            //Fetch flow number
            DatOffs *= sizeof(S_DatEntry);
            DatOffs += RGPT.DatBasePtr;
            // Flow number reg is cleared (WMM will write to LSB only)
            // FlowNu MSB are assured to be cleared
            // as FlowNu uses the same register as MaxNumOfFlows
            *(&FlowNu + 2)= _mem(r0,DatOffs,2,DAT_BUS);

            // if they are equal zero -> Flow number is not valid.
            FlowNu = FlowNu & 0x0000ffff;
            if (!eq)
            {
               // Flow number!= 0x0000 valid flow number
               RGPT.FlowNu = FlowNu;
            }
            else
            {
               /**************************************************
             ****     unsupported DLCI/Illegal header       ****
                **************************************************/
               // Flow number = 0x0000 - unsupported DLCI
#pragma outofline;
            FR_Rx_unsupported_DLCI:
               P_ErrorTypeOffs  =  FR_ADDRESS_MISMATCH_OFFSET;

            FR_Rx_IleaglHeader:
               B_FrUnsupportedDlci(P_DataLength,
                                   status0,
                                   phy,
                                   FrRgptKey,
                                   RgptOffset,
                                   FrMode1,
                                   status1,
                                   FrMasterKey,
                                   FlowNu,
                                   DLCITemp,
                                   P_ErrorTypeOffs,
                                   RGPT);

            }
         }
         else
         {


            /*********************************************
             ****             Hash Lookup             ****
             *********************************************/
            HashIndexMask = RGPT.U_LkUpSize.HashLkUp.HashIndexMask;
            F_HashLookup(status0,
                         DLCITemp,
                         phy,
                         HashIndexMask,
                         RGPT.DatBasePtr,
                         FlowNu,
                         FR_HASH_LOOKUP);


            // Flow number MSB are cleared while LSB are checked,
            // if they are equal zero -> Flow number is not valid.
            if (FlowNu == 0xffff) goto FR_Rx_unsupported_DLCI;
            // Flow number!= 0xFFFF valid flow number
            RGPT.FlowNu = FlowNu;

         }
	 /*
	 if(phy == FlowNu)
	   {
	     P_ErrorTypeOffs = 0x0001;
	     *(&P_ErrorTypeOffs)= _mem(r0,P_ErrorTypeOffs,2,DAT_BUS);
	   }
	 *//*rizh*/
	 
	   if(phy == FlowNu)
	   {
	     register PatchOffset = r10;
	     PatchOffset =0x10;
	     _mem(r0,B_DEBUG1[PatchOffset],4,DEBUG_BUS) =*(&DLCITemp);
	     PatchOffset +=4;
	     _mem(r0,B_DEBUG1[PatchOffset],4,DEBUG_BUS) =*(&phy);
             PatchOffset +=4;
	     _mem(r0,B_DEBUG1[PatchOffset],4,DEBUG_BUS) =*(&HashIndexMask);
             PatchOffset +=4;
	     _mem(r0,B_DEBUG1[PatchOffset],4,DEBUG_BUS) =*(&RGPT.DatBasePtr);
             PatchOffset +=4;
	     _mem(r0,B_DEBUG1[PatchOffset],4,DEBUG_BUS) =*(&FlowNu);
	     //P_ErrorTypeOffs = 0x0001;
	     //*(&P_ErrorTypeOffs)= _mem(r0,P_ErrorTypeOffs,2,DAT_BUS);
             goto FR_Rx_unsupported_DLCI;
	   }
	}



   /*****************************************************************************
 *
 * Macro Name:  M_FrRxCheckFcs
 *
 * Description: This macro is used when FR is run over TDI I/F
 *              to check if there was a CRC error.
    *              The Frame length is corrected - FCS size is subtracted
 *              from the overall frame length.
 *
 * Inputs:    P_FRAG_MODE -> indicating where macro was called from
 *            mode0, status0
 * 
    * Modified registers: P_FrameLength, FrMode1
 *                     P_CrcResult, P_Rrp
 *
 *****************************************************************************/
   inline func M_FrRxCheckFcs(in P_FRAG_MODE,
                              in mode0,
                              in status0,
                              in P_FcsSize,
                              inout FrMode1,
                              inout P_CrcResult,
                              inout P_FrameLength,
                              inout P_Rrp)
      {
         /* checks whether CRC correct or not */
         if (!mode0.CRCT)
         {
            P_CrcResult &= 0xFFFF0000;
            P_CrcResult ^ CRC16_CONST;
         }
         else
         {
            P_CrcResult ^= CRC32_L_CONST;
            P_CrcResult ^ CRC32_H_CONST;
         }
         
         if (!eq)
         {
#pragma outofline;
            FrMode1 |= (M_Mask(S_FrRxMode1Field.RxError)|
                        M_Mask(S_FrRxMode1Field.CrcE));
         }
         
         /* remove CRC length from the total frame length */
         /* the interworking module don't need it. only (IP frame
          * length is required. */
      $reg(P_FrameLength) -= P_FcsSize;
         
      }
   
   
   
   /*****************************************************************************
 *
 * Macro Name:  M_FrRxCheckMinLen
 *
 * Description: This macro checks if the frame payload contains at least
 *              one byte of data
 *              (note that if this is not a first/last packet then the frame
    *              is at least 32 byte long, and there is no need for the check)
 *
 * Inputs:     mode0, status0, P_FrameLength, P_HeaderSize, P_ErrorLabel
 *
 * Modified and saved registers: FrMode1
 *
 *****************************************************************************/
   inline func M_FrRxCheckMinLen(in mode0,
                               in status0,
                               in P_FcsSize,
                               in P_FrameLength,
                               in P_HeaderSize,
                               in P_ErrorLabel,
                               inout FrMode1)
      {
         // Local registers
         register  TempLen = r10;
         
      // If packet is not first then frame is at least 32 bytes long
      // and check is redundant
      TempLen = P_FrameLength - P_HeaderSize;
         
         if (mode0.F)
         {
            // Subtract header and FCS from frame
            // length to determine payload length
         if ((!FrMode1.POS) || mode0.CrcRem)
            {
               TempLen -= FcsSize;
            }
                     
         // check if payload contains at least one byte.
            if (TempLen < 1)
            {
#pragma outofline;
               FrMode1 |= (M_Mask(S_FrRxMode1Field.MLE)|
                           M_Mask(S_FrRxMode1Field.RxError));
               
               BufferLength = P_FrameLength;
               goto P_ErrorLabel;
               
            }
         }
      }
   
   /*****************************************************************************
      *
      * Macro Name:  F_FrRxUpdateErrStatics
      *
      * Description: This macro is called at the end of frame processing.
      *              the macro is invoked only for faulty frames.
      *              The macro checks the mode register to check which error
      *              occured then update approprate statistic.
      *              The macro may optionally terminate thread.
      *
      * Inputs:     P_TableNu, status0, mode, P_Attributes, P_FrameLen, P_Mode0
      *
      * Modified and saved registers:
      *
      *****************************************************************************/
     func F_FrRxUpdateErrStatics(in P_offset = r10,
                                 in P_Length = r113,
                                 in struct S_FrSerialStatusReg0 status0 = r7,
                                 in struct S_FrRxMode1Field FrMode1 = r1,
                                 in P_Attributes = r112,
                                 in P_StatisticsType = r114,
                                 in struct S_FrRcptMode P_Mode0 = r6)
     {
        // Local registers
        register CoherencyTag    =  r11;
   
        if (P_StatisticsType == FR_RX_PHY_STAT)
        {
           M_FrRxUpdateErrByteCnt(status0,
                                  P_offset,
                                  P_Length);
        }

        // Errored frame
        if ( FrMode1.AbrtPosErr)
           P_offset += FR_RX_ERROR_OFFSET;
        else if (status0.U_SerialStatus.Rx.U_Bit10.NO)
           P_offset += FR_NO_ERR_OFFSET;
        else if (FrMode1.PE)
           P_offset += FR_PE_ERROR_OFFSET;
        else if (FrMode1.SOP_EOP_E)
           P_offset += FR_SOPEOP_ERROR_OFFSET;
        else if ( FrMode1.MLE )
           P_offset += FR_MAX_SDU_ERR_OFFSET;
        else if ( FrMode1.CrcE )
           P_offset += FR_CRC_ERROR_OFFSET ;
        else if (FrMode1.FbpEmpty)
           P_offset += FR_FBP_EMPTY_OFFSET;

        if (P_Attributes == _TERMINATE)
           _mem(r0,B_FrRxStat[status0][P_offset], _HALF_WORD | STATISTICS_BUS | _TERMINATE) += 1;
        else
           _mem(r0,B_FrRxStat[status0][P_offset], _HALF_WORD | STATISTICS_BUS ) += 1;
     }
   
   /*********************************************************************************
    *
    * Function Name: F_FrRxErrFrameLastPac
    *
    * Description: This function inits various parameters required by
    *              for host termination enquing
    *              (note that in normal flow these fields would be updated
    *               at B_RxLastPacket, however this function handles errored
    *               frames and joins the normal flow in order to enqueue
    *               the faulty frames)
    *
    * Inputs:     FrRcptKey, status0, RcptOffset, DataLength,mode0, FlowNu, FrameLength
    *
    * Outputs:     , DataPtr, FrIwHostKey, IwMasterKey3Tag, Rrp
    *             IWExControlWord
    *
    * Modified and saved registers: RCPT, FrMode1
    *
    *****************************************************************************/
   static func F_FrRxErrFrameLastPac(in FrRcptKey = TagKey2,
                                     in struct S_FrSerialStatusReg0 status0 = r7,
                                     in RcptOffset = r24,
                                     in DataLength = r33,
                                     in FlowNu = r28,
                                     in struct S_FrRcptMode mode0 = r6,
                                     in FrameLength = r18,
                                     in struct S_FrRgpt RGPT = r72,
                                     in P_Phy = r36,
				                     in P_FrRxErrLocation = r10,
                                     inout BufferLength = r29, // must change to diffrent reg
                                     inout struct S_FrRcpt RCPT = r56,
                                     inout struct S_FrRxMode1Field FrMode1 = r1,
                                     out FrIwHostKey = TagKey1)

      {
         // Local register
         register Rbs            =  r10;
         register L2IWTOffset    =  r10;



    if (P_FrRxErrLocation == FR_RX_ERR_MID_FRAME)
	{
         // Hunt mode is set
         RCPT.ModeField |= M_Mask(S_FrRcptMode.FlowHunt);


	  /* save RCPT */
	  _mem(FrRcptKey, B_FrRCPT[status0][RcptOffset],FR_RCPT_WRITE_SIZE,
	       EHP_CPT_BUS|_ORDER_UNLOCK) = *(&RCPT);
	}
    else
	{
	  _task_switch(FrIwHostKey,_ORDER_UNLOCK);
	}

      /* update statistics of errored frames
       * this function updates appropriate error statistics
       * and terminates thread */
      if (status0.U_SerialStatus.Dps.STE)
         {
         F_FrRxUpdateErrStatics(FlowNu*sizeof(S_FrRxStTable),
                                r113, /* this parameter is not required here */
                                status0,
                                FrMode1,
                                FR_NULL_PARAMETER,
                                FR_RX_CHANNEL_STAT,
                                mode0 /* this parameter is not required here */);

      }
      if (FrMode1.GlobalMode.PSTE)
      {
         F_FrRxUpdateErrStatics(P_Phy*sizeof(S_FrRxStTable),
                                FrameLength,
                                status0,
                                FrMode1,
                                FR_NULL_PARAMETER,
                                FR_RX_PHY_STAT,
                                mode0);

         }


      M_FrRxPutFifoBD(RGPT,
                      mode0.IwComFbp,
                      FR_RX_ERR_NO_ORDER,
                      FrIwHostKey,
                      RCPT.BdPtr,
                      _TERMINATE);

      }

   /*****************************************************************************
 *
 * Macro Name:  B_FrRx_StartPos
 *
    * Description: This function is called at a beginning of a receive thread
 *              when Frame relay is run over a POS I/F. macro checks for
    *              errors indication by the serial translating any error to
 *              the appropriate FR relay error. In case this is a dummy
 *              request the thread would be terminated.
 *
 * Inputs:     status1
 *
 * Outputs:    phy, FrMode1, tar0
 *
 * Modified and saved registers: status0
 *****************************************************************************/
   inline func B_FrRx_StartPos(in status1,
                               in status0,
                               out phy,
                               out FrMode1,
                               out tar0,
               			       out DataLength)
      {


         M_PhyNumGet(phy);
         FrMode1 = M_Mask(S_FrRxMode1Field.POS);
         DataLength = status1.DataUnitLength;

         /* checks if there are received errors */
#ifdef _WINPATH_REV_WP3_ENABLE_
         if ( !status0.UpiGSE )
#else
         if ( !status0.U_SerialStatus.Rx.GSE )
#endif
         {
            tar0 = &DATA_TMP_BUF;
            goto Fr_Rx_Start;
         }
         else
         {
            /* checks DUMMY error */
            if ( status0.U_SerialStatus.Rx.DUM )
               _task_switch(status1, _ORDER_UNLOCK|_TERMINATE);



            /* POS SOP/EOP error */
            if ( status0.U_SerialStatus.Rx.SOP_EOP_E )
               FrMode1 |= (M_Mask(S_FrRxMode1Field.SOP_EOP_E)|
                           M_Mask(S_FrRxMode1Field.RxError));


            /* POS parity error */
            if ( status0.U_SerialStatus.Rx.PE )
               FrMode1 |= (M_Mask(S_FrRxMode1Field.PE)|
                           M_Mask(S_FrRxMode1Field.RxError));

            /* PosErr  error */
            if ( status0.PosErr )
               FrMode1 |= (M_Mask(S_FrRxMode1Field.AbrtPosErr)|
                           M_Mask(S_FrRxMode1Field.RxError));


            tar0 = &DATA_TMP_BUF;
            goto Fr_Rx_Start;
         }
      }

   /*****************************************************************************
 *
 * Macro Name:  B_FrRx_StartEmphy
 *
    * Description: This function is called at a beginning of a receive thread
    *              when Frame relay is run over a POS ENPHY I/F. macro realigns
 *              data unit. updates tar0 to begin of data unit and updates the
 *              size of the packet.
 *
 * Inputs:     packet length, status0
 *
 * Outputs:   FrMode1, tar0
 *
 * Modified and saved registers: status1, data unit (r96-r103)
 *****************************************************************************/
   inline func B_FrRx_StartEmphy(in P_Length,
				 in status0,
                                 inout status1,
                                 inout P_PosPacket,
                                 out FrMode1,
                                 out tar0,
				                     out DataLength)
      {
         FrMode1 = M_Mask(S_FrRxMode1Field.POS);
         tar0 = &P_PosPacket;


#ifdef _MODULE_EX_MULTI_PHY_ENABLE_
         status1.DataUnitLength = P_Length;



         /* Emphy data unit is re-aligned according to *
          * standard pos data unit alignment (data unit*
          * begins at r96                              */
         _bf(P_PosPacket,0,32)= _bf(P_PosPacket,96,32);
         _bf(P_PosPacket,32,32)= _bf(P_PosPacket,128,32);
         _bf(P_PosPacket,64,32)= _bf(P_PosPacket,160,32);
         _bf(P_PosPacket,96,32)= _bf(P_PosPacket,192,32);
         _bf(P_PosPacket,128,32)= _bf(P_PosPacket,224,32);
         _bf(P_PosPacket,160,32)= _bf(P_PosPacket,256,32);
         _bf(P_PosPacket,192,32)= _bf(P_PosPacket,288,32);
         _bf(P_PosPacket,224,32)= _bf(P_PosPacket,320,32);


	 if (status0.PosErr)
         {
#pragma outofline;
            FrMode1 |= (M_Mask(S_FrRxMode1Field.AbrtPosErr)|
                        M_Mask(S_FrRxMode1Field.RxError));
         }
#endif // _MODULE_EX_MULTI_PHY_ENABLE_
         DataLength = status1.DataUnitLength;
	 goto Fr_Rx_Start;
      }


   /*****************************************************************************
 *
 * Macro Name:  B_FrRx_StartEmphy2Err
 *
 * Description: This function is called at a beggining of a receive thread
 *              when Frame relay is run over a POS ENPHY2 I/F. macro realignes
 *              data unit. updates tar0 to begin of data unit and updates the
 *              size of the packet. 
 *              this code os for error cases only.
 *
 * Inputs:     packet length, status0
 *
 * Outputs:   FrMode1, tar0
 *
 * Modified and saved registers: status1, data unit (r96-r103)
 *****************************************************************************/
   inline func B_FrRx_StartEmphyErr(in status0,
                                    inout status1,
                                    inout P_PosPacket,
                                    out FrMode1,
                                    out tar0,
				    out DataLength)
      {
         FrMode1 = M_Mask(S_FrRxMode1Field.POS);
         tar0 = &P_PosPacket;

         /* Last bit is set for code compatibility, FR, MFR,HLDC-PPP modules assumes
            that all error notifications arrive copuled with last indication. */
         status0 |= M_Mask(S_PosSerialStatusReg0.Last);
         
         /*  ALL HDLC protocols */
         /*
          * HDLC with Rx_ERR signal:
          * 000 - non octet         
          * 001 - Abort
          * 010 - CRC error
          * 100 - Overrun
          */
         register struct S_Ufe2SerialStatusReg0  Ufe2Status0  = status0;
         
         if(Ufe2Status0.Ufe2Cmd4)
         {/* 100 - Overrun */
            FrMode1 |=  (M_Mask(S_FrRxMode1Field.RxError)|
                         M_Mask(S_FrRxMode1Field.SOP_EOP_E));  

         }
         else if(Ufe2Status0.U_Ufe2Cmd3.Ufe2Cmd3)
         {/* 010 - CRC error */
            FrMode1 |=  (M_Mask(S_FrRxMode1Field.RxError)|
                         M_Mask(S_FrRxMode1Field.CrcE));
            
         }
         else if(!Ufe2Status0.U_SerialStatus_Ufe2.Rx.U_Ufe2Cmd2.Ufe2Cmd2)
         {/* 000 - non octet */
            FrMode1 |= M_Mask(S_FrRxMode1Field.RxError);
         }
         else
         {/* Abort */
            FrMode1 |= (M_Mask(S_FrRxMode1Field.AbrtPosErr)|
                        M_Mask(S_FrRxMode1Field.RxError));
            
         }
         

         status0 &= ~(M_Mask(S_Ufe2SerialStatusReg0.U_SerialStatus_Ufe2.Rx.AtmHashlkup)|
                      M_Mask(S_Ufe2SerialStatusReg0.U_SerialStatus_Ufe2.Rx.U_Ufe2Cmd2.Ufe2Cmd2)   |
                      M_Mask(S_Ufe2SerialStatusReg0.U_SerialStatus_Ufe2.Rx.Ufe2Cmd1)   |
                      M_Mask(S_Ufe2SerialStatusReg0.U_SerialStatus_Ufe2.Rx.Ufe2Cmd0)   |
                      M_Mask(S_Ufe2SerialStatusReg0.U_Ufe2Cmd3.Ufe2Cmd3)  |
                      M_Mask(S_Ufe2SerialStatusReg0.Ufe2Cmd4));         

         DataLength = status1.DataUnitLength;
	 goto Fr_Rx_Start;
      }


   /*****************************************************************************
 *
 * Macro Name:  B_FrRx_StartTdi
 *
 * Description: This fanction is called at a beggining of a receive thread
 *              when Frame relay is run over a TDI I/F. macro checks for
 *              errors indication by the serial transmlating any error to
 *              the appropriate FR relay error. In case this is a dummy
 *              request the thread would be terminated.
 *              (phy register is inted by MCH when this macro is reached
 *               from MCH dps)
 *
 * Inputs:     status1, lable that bypasses phy init
 *
 * Outputs:    phy, FrMode1, tar0
 *
 * Modified and saved registers: status0
 *
 *****************************************************************************/
   inline func B_FrRx_StartTdi(in P_Mid_Func_Lablel,
                               in status1,
                               inout status0,
                               out phy,
                               out FrMode1,
                               out tar0,
                               out DataLength)
      {
         /* update status0[Base] value to point to TDI FR part */
         status0 |= TDI_FR_TA_RAM_OFFSET;

         M_PhyNumGet(phy);
         phy &= 0x0000003F;

      FR_Rx_Start_TDI_MCH:
         FrMode1 = 0;

         tar0 = &DATA_TMP_BUF;
         /* checks if there are received errors */
         if ( status0.U_SerialStatus.Rx.GSE )
         {
#pragma outofline;
           /* checks if there are received errors */
            /* checks DUMMY error */
            if ( status0.U_SerialStatus.Rx.DUM )
               _task_switch(status1, _ORDER_UNLOCK|_TERMINATE);

            /* checks Rx overflow error */
            if ( status0.U_SerialStatus.Rx.OV )
            {
               B_HandleRxOverrun(status0,
                                 status1,
                                 phy);
            }

            /* checks NO error */
            if ( status0.U_SerialStatus.Rx.U_Bit10.NO )
               FrMode1 |= M_Mask(S_FrRxMode1Field.RxError);

            /* checks ABORT error */
            if ( status0.U_SerialStatus.Rx.ABRT )
               FrMode1 |= (M_Mask(S_FrRxMode1Field.AbrtPosErr)|
                           M_Mask(S_FrRxMode1Field.RxError));

          
         }
         /* The Data Length of the chuck at TDI mode is always at the 8 LSB bits of status1 (WP3,WP2,WP1) */
         DataLength = _bf(status1, 24, 8);

      }


   /*****************************************************************************
 *
 * Macro Name:  M_FrRxGetRcpt
 *
 * Description: This macro releases the master key,
 *              fetches the FrRcptKey
 *              Then reads the RCPT table (locking it with the
 *              FrRcptKey. various fields of RCPT are copied to
 *              suitible registers to facilitate further processing.
 *
 * Inputs:     FrMasterKey, FlowNu
 *
 * Outputs     FrRcptKey, FrIwHostKey, IwMasterKey3Tag, RCPT, RcptOffset, mode0
 *
 *
 * Modified  registers: status0
 *
 *****************************************************************************/
   inline func M_FrRxGetRcpt(in    FrMasterKey,
                             in    FlowNu,
                             inout status0,
                             out   FrRcptKey,
                             out   RCPT,
                             out   RcptOffset,
                             out   mode0,
                             out   MaxSDU)
      {
         // Fetch new keys (per flow keys) relase masterkey
         M_OrderRptKey(FrMasterKey,
                       FlowNu,
                       status0,
                       FrRcptKey);

         // Read RCPT

         RcptOffset = FlowNu * sizeof(S_FrRcpt);
         *(&RCPT) = _mem(FrRcptKey,B_FrRCPT[status0][RcptOffset],sizeof(S_FrRcpt),
                         EHP_CPT_BUS|_ORDER_LOCK);


         // Update parameters from RCPT
         // need to load mode register too.
         _bf(status0, 24, 8) = _bf($reg(RCPT.INTM),0,8);
         MaxSDU = _sl6(RGPT.MaxSdu);
      }

   /*****************************************************************************
 *
 * Function Name:  F_FrRxDiscardBuffer
 *
 * Description: This macro handles a faulty frame, where the buffer is discarded
 *              either because the receive queue was full or the buffer was
 *              taken from the IW common FBP.
 *              The macro issues an interrupt, updates statistics
 *              and returns returns buffer and terminates thread.
 *
 * Inputs:  RGPT, status1, mode0, FrMode1, FrIwHostKey, FlowNu, phy, P_IntType, P_StType
 *          P_BdPtr, status0
 *
 * Modified and saved registers:
 *
 *****************************************************************************/
   static func F_FrRxDiscardBuffer(in struct S_FrRgpt RGPT = r72,
                                   in struct S_FrRxSerialStatusReg1 status1 = r8,
                                   in struct S_FrRcptMode mode0 = r6,
                                   in struct S_FrRxMode1Field FrMode1 = r1,
                                   in FrIwHostKey = TagKey1,
                                   in FlowNu = r28,
                                   in phy = r36,
                                   in P_IntType = r15,
                                   in P_StType = r16,
                                   in P_BdPtr = r60,
                                   in struct S_FrSerialStatusReg0 status0 = r7,
                                   in P_FrameErrLen = r112)
      {

         // pssobly need to add check if first buffer
         // if not then I should enqeue instead of discaridng

         // Issue appropriate interrupt
         if ( status0.U_SerialStatus.Dps.OvUnFlowIntm)
         {
            if (P_IntType == RxBN)
            {
               M_Interrupt(status1,
                           PROTOCOL_TYPE_FR,
                           RxBN,
                           FlowNu,
                           NULL_CID);
            }
            else
            {
               M_Interrupt(status1,
                           PROTOCOL_TYPE_FR,
                           RxBsy,
                           FlowNu,
                           NULL_CID);
            }
         }

         /* update statistics */
         if ( status0.U_SerialStatus.Dps.STE )
         {
            M_UpdateRxStatistics(r0,
                                 P_StType,
                                 FlowNu,
                                 status0,
                                 FR_NULL_PARAMETER,
                                 FR_NULL_PARAMETER,
                                 FR_RX_CHANNEL_STAT);
         }
         if (FrMode1.GlobalMode.PSTE)
         {
            M_UpdateRxStatistics(r0,
                                 P_StType,
                                 phy,
                                 status0,
                                 P_FrameErrLen,
                                 FR_NULL_PARAMETER,
                                 FR_RX_PHY_STAT);
         }

         // Return buffer to FBP and terminate thread
      // (The error type field is passed in order to indicate to the macro
         // that this is not a frag error and Key should be released)

         M_FrRxPutFifoBD(RGPT,
                         mode0.IwComFbp,
                      FR_RX_ERR_RELEASE_ORDER_KEY,
                         FrIwHostKey,
                         P_BdPtr,
                         _TERMINATE);


      }



   /*****************************************************************************
 *
 * Macro Name:  M_FrRxPutFifoBD
 *
 * Description: This macro returns the buffer to the appropriate FBP
 *              The IwHostKey might be released if the key was ordered
 *              When Buffer is returned due to frag error the key would only
 *               be released later (if it was ordered).
 *             (In a certain instance the macro also terminates thread)
 *
 * Inputs: RGPT,  P_FBP_Type, P_ERR_Type,P_ReturnBdPtr, FrIwHostKey
 *
 *****************************************************************************/
   inline func M_FrRxPutFifoBD(in RGPT,
                               in P_FBP_Type,
                               in P_OrderMode,
                               in FrIwHostKey,
                               in P_ReturnBdPtr,
                               in P_Attributes)
      {
      register                     NxtBd                         = r10;
      register                     tmpMatrixBuffChainInfoAddress = r10;
      register                     FbpNum                        = r11;
      register                     CurrentBd                     = r12;
      register struct S_MatrixBuffChainInfo MatrixBuffChainInfo  = r16;

         // FBP number is copied to the acc register due to WMM
         // restriction (can not pass both a Key register and a
         // FBP number register)

         // FR FBP
      FbpNum = RGPT.FBPnum;

      if (FEATURE_MATRIX_BUFF_CHAIN && mode0.MTBD)
      {
         CurrentBd = RGPT.FirstBdPtr;

         tmpMatrixBuffChainInfoAddress = RGPT.FirstBdPtr + (S_MatrixBd.U_ProtocolSpecific.MatrixBuffChainInfo)/8;
         MatrixBuffChainInfo =  _mem(r0, tmpMatrixBuffChainInfoAddress, sizeof(S_MatrixBuffChainInfo), FBP_PARM_BUS);

         M_MatrixBufChainCalcSecondBdPtr(MatrixBuffChainInfo,
        		                         RGPT.FirstBdPtr,
        		                         NxtBd);

         /* first is released after this block in any case,
            here we free all the others, and there is a not necessary extra task switch
            at the end (but we save one of writing zero in the last BD NEXT_BD_PTR),
            the plus is that the code is more compact this way */

         do
         {

            _fifo(r0,FbpNum, FBP_PARM_BUS ) = CurrentBd;
            CurrentBd = NxtBd;
            NxtBd = _mem(r0,CurrentBd,4,FBP_PARM_BUS);
         }
         while (CurrentBd != P_ReturnBdPtr);

         }


      if (P_OrderMode == FR_RX_ERR_NO_ORDER )
      {
         _fifo(r0,FbpNum,  P_Attributes | FBP_PARM_BUS ) = P_ReturnBdPtr;
      }
         else
      {
         acc = FbpNum;
         _fifo(FrIwHostKey,acc, FBP_PARM_BUS | _ORDER_UNLOCK | P_Attributes) = P_ReturnBdPtr;
      }

      /* In case the FIFO put operation fails the TA returns to this thread
	      therefore the bellow Task Switch is added*/
      if (P_Attributes == _TERMINATE)
      {
    	  _task_switch(r0,_TERMINATE);
      }
	    


      }

   /*****************************************************************************
 *
 * Macro Name:  B_FrRxChkUpdateStatistics
 *
 * Description: This function is called at the end of frame processing
 *              the function checks if statitstics are enabled.
 *              If statistics are enabled the appropriate statisitc will
 *              be updated and the the thread terminates.
 *              (if statistics are disabled the thread is terminated)
 *
 * Inputs:      status0, FlowNu, FrMode1, mode0, FrameLength, phy
 *
 *****************************************************************************/
   inline func B_FrRxChkUpdateStatistics(in FrMode1,
                                         in mode0,
                                         in phy,
                                         in FlowNu,
                                         in FrameLength,
                                         in status0,
                                         in FcsSize)
      {
         if (status0.U_SerialStatus.Dps.STE)
         {
            // Statistics enabled
            if (!FrMode1.RxError)
            {
               // update flow statistics for a valid frame
               M_FrRxUpdateValidStatics(FlowNu,
                                        status0,
                                        mode0,
                                        FR_NULL_PARAMETER,
                                        FrameLength);
            }
            else
            {
#pragma outofline;
               // update flow statistics for a errored frame
              F_FrRxUpdateErrStatics(FlowNu*sizeof(S_FrRxStTable),
                                     r113, /* this parameter is not required here */
                                     status0,
                                     FrMode1,
                                     FR_NULL_PARAMETER,
                                     FR_RX_CHANNEL_STAT,
                                     mode0 /* this parameter is not required here */);
            }
         }
         if (FrMode1.GlobalMode.PSTE)
         {
            // Statistics enabled
            if (!FrMode1.RxError)
            {
               // update phy statistics for a valid frame
               M_FrRxUpdateValidStatics(phy,
                                        status0,
                                        mode0,
                                        _TERMINATE,
                                        FrameLength);
            }
            else
            {
#pragma outofline;
               // update phy statistics for a errored frame
              F_FrRxUpdateErrStatics(phy*sizeof(S_FrRxStTable),
                                     FrameLength,
                                     status0,
                                     FrMode1,
                                     _TERMINATE,
                                     FR_RX_PHY_STAT,
                                     mode0);
            }
         }
         else
         {
            _task_switch(r0, _TERMINATE);
         }
      }

   /*****************************************************************************
 *
 * Macro Name:  M_FrRxUpdateValidStatics
 *
 * Description: This macro is called at the end of frame processing.
 *              the macro is invoked only for valid frames, it reads
 *              the relevent table with coherency, updates relevent
 *              fields then writes back table.
 *              The macro may also terminate thread according to the
 *              attribute used in the specific insansiation
 *
 *
 * Inputs:     P_TableNu, status0, mode, P_Attributes, P_FrameLen
 *
 * Modified and saved registers:
 *
 *****************************************************************************/
   inline func M_FrRxUpdateValidStatics(in P_TableNu,
                                        in status0,
                                        in mode,
                                        in P_Attributes,
                                        in P_FrameLen)
      {
         // Local registers
         register offset                   = r10;
         register CoherencyTag             = r127;
         register struct S_FrRxStTable RST = r12;


         // Preper coherency tag
         CoherencyTag = P_TableNu <<16;
         CoherencyTag |= (M_Mask(S_Tag.U_TagFields.LabelTag.LabelMode)|
                          (LABEL_TAG_LABEL_FR_RX_STAT<<8));

         // Read Statisicst table with coherency
         offset = P_TableNu*sizeof(S_FrRxStTable);
         *(&RST) = _mem(CoherencyTag, B_FrRxStat[status0][offset],FR_RX_RW_RST_SIZE,
                        STATISTICS_BUS | _LOCK);

         RST.RxFrames +=1;
         RST.RxBytes  += P_FrameLen;
         if (mode.DE)
         {
            RST.RxFramesDE +=1;
            RST.RxBytesDE += P_FrameLen;
         }
         if (mode.FECN)
            RST.RxFramesFECN +=1;

         if (mode.BECN)
            $reg(RST.RxFramesBECN) += 1;

         _mem(CoherencyTag, B_FrRxStat[status0][offset],FR_RX_RW_RST_SIZE,
              STATISTICS_BUS |P_Attributes  | _UNLOCK) = *(&RST);
      }

   /*****************************************************************************
    *
    * Macro Name:  M_FrRxUpdateByteStatics
    *
    * Description: This macro is called by fragmention routines to correct
    *              Frame byte counters with the frag overhead not included
    *              in total frame length (FCS, FR header of each frag)
    *
    * Inputs:     P_TableNu, status0, mode, P_Attributes, P_FrameLen
    *
    * Modified and saved registers:
    *
    *****************************************************************************/
   inline func M_FrRxUpdateByteStatics(in P_TableOffs,
                                       in status0,
                                       in mode,
                                       in P_FrameLen)
      {
         // Local registers
         register offset                   = r10;
         register CoherencyTag             = r127;
         register struct S_FrRxStTable RST = r19;


         // Preper coherency tag
         //CoherencyTag = P_TableNu << 16;
         CoherencyTag = P_TableOffs << 16;
         CoherencyTag |= (M_Mask(S_Tag.U_TagFields.LabelTag.LabelMode)|
                          (LABEL_TAG_LABEL_FR_RX_STAT<<8));

         // Read Statisicst table with coherency
         offset = P_TableOffs*sizeof(S_FrRxStTable);
         *(&RST) = _mem(CoherencyTag, B_FrRxStat[status0][offset],16,
                        STATISTICS_BUS | _LOCK);


         RST.RxBytes  += P_FrameLen;
         if (mode0.DE)
         {
            RST.RxBytesDE +=P_FrameLen;
         }

         _mem(CoherencyTag, B_FrRxStat[status0][offset],16,
              STATISTICS_BUS  | _UNLOCK) = *(&RST);
      }

   /*****************************************************************************
    *
    * Macro Name:  M_FrRxUpdateErrByteCnt
    *
    * Description: This macro updates a total byte counter for all errored
    *              Frames.
    *
    * Inputs:                      status0, P_Offset, P_PacketLen
    *
    * Modified and saved registers:
    *
    *****************************************************************************/
   inline func M_FrRxUpdateErrByteCnt(in status0,
                                      in P_Offset,
                                      in P_PacketLen)
     {
        // Local registers
        register       CoherencyTag = r127;
        register       TotalByteCnt = r12;
        register       TableOffset  = r13;

        CoherencyTag = ((P_Offset)/sizeof(S_FrRxStTable)) << 16 ;
        CoherencyTag |= (M_Mask(S_Tag.U_TagFields.LabelTag.LabelMode)|
                         (LABEL_TAG_LABEL_FR_RX_STAT<<8));


         // Read Statisicst table with coherency
         TableOffset = P_Offset + FR_TOTAL_ERR_BYTE_OFFSET;
         TotalByteCnt = _mem(CoherencyTag, B_FrRxStat[status0][TableOffset],4,
                             STATISTICS_BUS | _LOCK);

	 TotalByteCnt += P_PacketLen;
	 _mem(CoherencyTag, B_FrRxStat[status0][TableOffset],4,
	      STATISTICS_BUS | _UNLOCK) = TotalByteCnt;
     }




   /*****************************************************************************
 *
    * Macro Name:  B_FrRxUpdateRcpt
 *
 * Description: This macro is used at an openning of a new frame, it
 *              updates RCPT Pointers, and status bits and mode0
 *
 * Inputs:  P_FrameType (parameter), TempFragHeader, FBecnDeTmp, RBD
 *          FrFbpCnt, TempIwbt, Rrp, status1, FlowNu,  phy, P_length
 *
 * Modified  registers: RCPT,  mode0, FrMode1
 *
 *****************************************************************************/
   inline func B_FrRxUpdateRcpt(in    P_FrameType,
                                in    TempFragHeader,
                                in    FBecnDeTmp,
                                in    RBD,
                                in    FrFbpCnt,
                                in    TempIwbt,
                                in    RGPT,
                                in    status1,
                                in    FlowNu,
                                in    phy,
                                in    P_length,
                                in    FcsSize,
                                inout RCPT,
                                inout FrMode1,
                                out   mode0,
                                out   Rrp)
      {
      // Local Volatile registers
         register              IwGap          = r10 ;
         register              GapUnALignment = r10;
         register              P_UnusedParam  = r22;   
         
         // Clear FECN, BECN, DE, CR and flowhunt bits in RCPT
         $reg(RCPT.ModeField.FECN) &= ~(M_Mask(S_FrRcptMode.FECN)|
                                        M_Mask(S_FrRcptMode.BECN)|
                                        M_Mask(S_FrRcptMode.DE)|
                                        M_Mask(S_FrRcptMode.CR)|
                                        M_Mask(S_FrRcptMode.FlowHunt)|
                                        M_Mask(S_FrRcptMode.IWF)|
                                        M_Mask(S_FrRcptMode.IwComFbp)|
            M_Mask(S_FrRcptMode.DBT)|
            M_Mask(S_FrRcptMode.MTBD));

     
         
         // Update bit in RCPT
         $reg(RCPT.ModeField.FECN) |= FBecnDeTmp;
         
         
         // Update mode0 and mode field in RCPT
         // (DBT and IwComFbp bits are updated)
         RCPT.ModeField |= TempIwbt;
         mode0 = RCPT.ModeField;
         
         /* Load FCS Size */
         if (!mode0.CRCT)
            FcsSize = 2;
         else
            FcsSize = 4;
         
         // Update First bit (indicating this is the first packet of a frame)
         mode0 |= M_Mask(S_FrRcptMode.F);
         
         
         /* align data pointer to 256 bytes */
         RCPT.DataPtr = RBD.DataPtr;
         RCPT.DataPtr &= IW_DATA_BUFFER_ALIGNMENT;
         
         
         RCPT.CrcTmp = 0xFFFFFFFF;
         // re-init Norf counter to zero
         $reg(RCPT.Norf) &= ~M_Mask(S_FrRcpt.Norf);
         
         /* Load the remaining byte counter in buffer *
          * with buffer size                          */
         IwGap = RCPT.IwGap;
      Rrp = RGPT.RBS;

      /* update DataPtr and Rrp to take Gap into account (also for host for maintaining less differences) */
      Rrp -= IwGap;
            RCPT.DataPtr += IwGap;

      /* update RCPT fields */
         RCPT.BdPtr = RBD.BdPtr;

      RCPT.FbpCounter =  _bf(FrFbpCnt,0,16);
      }



   /*****************************************************************************
 *
 * Macro Name:   B_FrDeleteRxFlow
 *
 * Description:  This macro deletes a FR flow, the macro is intiated by a
 *               host command, maco checks receiver state and if flow to
 *               be deleted is currently serviced. the function would release
 *               any mid frame BD, enter hunt mode if deleted flow is currently
 *               serviced. Set FlowDisabled bit in RCPT and terminate thread.
 *
 * Inputs:       status0 (TCR0),
 *               status1 (TCR1)
 *
 *****************************************************************************/
   inline func B_FrDeleteRxFlow(in status0,
                                in status1,
                                in FcsSize)
      {
         // Local register
         register struct S_FrTcrStatusReg  Tcr0 = STATUS0_REG;
         register struct S_FrTcrStatusReg1 Tcr1 = STATUS1_REG;

         // Local registers
         register DataPtr          = r14;
         register FrameLength      = r18;
         
      // Local registers (non volatile)
         register L2HeaderSize     =  r22;
         register CurrentBdPtr     =  r23;
         register InterworkingType =  r26;

            
         
         FlowNu = Tcr1.FlowNu;
         phy    = Tcr1.PN;

         // Get MasterKey and Rgpt order keys
         M_OrderRgptMasKey(Tcr0,
                           phy,
                           FR_GET_KEY_NO_ORDER,
                           FrRgptKey,
                           FrMasterKey);

         // Fetch Rgpt and Rcpt under order
      RgptOffset = phy*sizeof(S_FrRgpt);
         *(&RGPT) = _mem(FrRgptKey,B_FrRCPT[Tcr0][RgptOffset],sizeof(S_FrRgpt),
                         EHP_CPT_BUS|_ORDER_LOCK);

         // perform this operation so it clears rest of mode1
         FrMode1 =0;
         FrMode1.GlobalMode = RGPT.ModeField;
       
         M_OrderRptKey(FrMasterKey,
                       FlowNu,
                       Tcr0,
                       FrRcptKey);


         RcptOffset = FlowNu * sizeof(S_FrRcpt);
         *(&RCPT) = _mem(FrRcptKey,B_FrRCPT[Tcr0][RcptOffset],32,EHP_CPT_BUS|_ORDER_LOCK);
         
         // mode0 is loaded in order to verify FBP type and Frag mode
         mode0 = RCPT.ModeField;
         
         /* Load FCS Size */
         if (!mode0.CRCT)
            FcsSize = 2;
         else
            FcsSize = 4;
         
         if (!FrMode1.GlobalMode.OutF)
         {
            /****************************************/
            /***  Frame Currently being received  ***/
            /****************************************/
            /* A buffer should be returned if the deleted flow is
               current flow, if flow is hunt mode buffer was
               already released/enqued  */
            if ((FlowNu != RGPT.FlowNu) ||
                (mode0.FlowHunt)) goto FR_Delete_Non_CurrentFlow;

            /****************************************/
            /***   Delete Current (serviced) Flow ***/
            /****************************************/
            // release buffer and enter hunt mode.
            RGPT.FlowNu = phy;
            RgptOffset += S_FrRgpt.FlowNu/8;
            _mem(FrRgptKey,B_FrRCPT[Tcr0][RgptOffset],4,
                 EHP_CPT_BUS|_ORDER_UNLOCK) = *(&(RGPT.FlowNu));

            // Return BD
           FR_Delete_Flow_ReturnBd:


            M_FrRxPutFifoBD(RGPT,
                            mode0.IwComFbp,
                         FR_RX_ERR_NO_ORDER,
                            r0,
                            RCPT.BdPtr,
                            FR_NULL_PARAMETER);


         }
         else
         {
            /*********************************************/
            /** Delete flow currently not serviced     ***/
            /*********************************************/
          FR_Delete_Non_CurrentFlow:
            _task_switch(FrRgptKey,_ORDER_UNLOCK);
            /* A open buffer should be returned if the flow is
               fragmented and not in hunt mode */

         }


         RCPT.ModeField |= (M_Mask(S_FrRcpt.ModeField.FlowDisabled));
         RcptOffset += S_FrRcpt.ModeField/8;
         _mem(FrRcptKey,B_FrRCPT[Tcr0][RcptOffset],4,
              EHP_CPT_BUS |_ORDER_UNLOCK | _TERMINATE)= RCPT.ModeField;

      }// end of B_FrDeleteRxFlow


   
}// End of Fr_rx_func


/*****************************************************************************
 * (C) Copyright Wintegra 2000-2003, All rights reserved.
 *****************************************************************************/

