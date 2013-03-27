/*--------------------------------------------------------------------------*/
/*                                                                          */
/*        Copyright (c) 2010  Omiino Ltd                                    */
/*                                                                          */
/*        All rights reserved.                                              */
/*        This code is provided under license and or Non-disclosure         */
/*        Agreement and must be used solely for the purpose for which it    */
/*        was provided. It must not be passed to any third party without    */
/*        the written permission of Omiino Ltd.                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/



#include "WO_FRMR_private.h"

void OMIINO_AUX_ResponseProgressReport(U8 iDevice, U32 CatalogueID, U32 Iteration, U32 TotalIterations, char * pMessage, U8 MessageLength)
{
    U8 Length;
    Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_PROGRESS_REPORT(CatalogueID, Iteration, TotalIterations, pMessage, MessageLength, OMIINO_RAM.Device[iDevice].LLC.Buffer);
    OMIINO_OMNISPY_SouthboundMessage(iDevice, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
}

void OMIINO_AUX_Response(U8 iDevice, U32 TransactionIdentifier, U8 API_Result)
{
    U8 Length;
    Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_AUX_RESP(TransactionIdentifier, API_Result, OMIINO_RAM.Device[iDevice].LLC.Buffer);
    OMIINO_OMNISPY_SouthboundMessage(iDevice, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
}



void OMIINO_AUX_ResponseWithPayload(U8 iDevice, U32 TransactionIdentifier, U8 API_Result, U32 CatalogueID, char * pData, U8 PayloadLength)
{
    U8 Length;
    Length=OMIINO_LLC_SOUTHBOUND_FORMATTER_AUX_RESP_WithPayload(TransactionIdentifier, API_Result, CatalogueID, pData, PayloadLength, OMIINO_RAM.Device[iDevice].LLC.Buffer);
    OMIINO_OMNISPY_SouthboundMessage(iDevice, OMIINO_RAM.Device[iDevice].LLC.Buffer, Length);
}




