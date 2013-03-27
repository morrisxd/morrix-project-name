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


U32 UtilityGetU32(U8 * pCharArray)
{
    U32 Result=0;

    Result=(pCharArray[0]<<24);
    Result=Result|((pCharArray[1]<<16)&0x00FF0000);
    Result=Result|((pCharArray[2]<<8)&0x0000FF00);
    Result=Result|(pCharArray[3]&0x000000FF);

    return Result;
}

void UtilityPutU32(U32 AnyInt, U8 * pCharArray)
{

    pCharArray[0]=(((AnyInt&0xFF000000)>>24)&0x000000FF);
    pCharArray[1]=(((AnyInt&0x00FF0000)>>16)&0x000000FF);
    pCharArray[2]=(((AnyInt&0x0000FF00)>> 8)&0x000000FF);
    pCharArray[3]= ( AnyInt&0x000000FF     );
}



void UtilityPutU16(U16 AnyShort, U8 * pCharArray)
{

    pCharArray[0]=(((AnyShort&0x0000FF00)>> 8)&0x000000FF);
    pCharArray[1]=( AnyShort&0x000000FF     );
}



U16 UtilityGetU16(U8 * pCharArray)
{
    U16 Result=0;

    Result=(pCharArray[0]<<8);
    Result=Result|(pCharArray[1]&0x000000FF);

    return Result;
}



