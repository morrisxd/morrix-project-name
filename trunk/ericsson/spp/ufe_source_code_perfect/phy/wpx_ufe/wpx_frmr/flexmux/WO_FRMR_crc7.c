/*--------------------------------------------------------------------------*/
/*                                                                          */
/*        Copyright (c) 2009  Omiino Ltd                                    */
/*                                                                          */
/*        All rights reserved.                                              */
/*        This code is provided under license and or Non-disclosure         */
/*        Agreement and must be used solely for the purpose for which it    */
/*        was provided. It must not be passed to any third party without    */
/*        the written permission of Omiino Ltd.                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/

#include "WO_FRMR_private.h"



void OMIINO_FRAMER_CRC7_ShiftByte(U8 *ByteData, U8 NewBit)
{
	U8 TMP_Data;

	/* Implement the cyclic division by "X^7 + X^3 + 1"	*/
	/* using XOR feedback taps				*/

	TMP_Data         = ByteData[7];
	ByteData[7] = ByteData[6];
	ByteData[6] = ByteData[5];
	ByteData[5] = ByteData[4];
	ByteData[4] = ByteData[3] ^ TMP_Data;
	ByteData[3] = ByteData[2];
	ByteData[2] = ByteData[1];
	ByteData[1] = ByteData[0] ^ TMP_Data;
	
	/* Load the next bit into the shift register*/
	ByteData[0] = NewBit;


}




U8 OMIINO_FRAMER_CRC7_Calculate(U8 * pTraceStr)
{
	U8 CurrentByte[8];
	U8 ByteIndex;
	U8 BitIndex;
	U8 BitToLoad = 0;
	U8 CRC7_Value = 0;


	for (BitIndex = 0; BitIndex < 8; BitIndex++) 
	{
		CurrentByte[BitIndex] = 0;
	}


	CurrentByte[7]=1;

	/* Step through remaining 15 path_trace bytes */
	for (ByteIndex = 0; ByteIndex < 15; ByteIndex++)
	{
		for (BitIndex = 0; BitIndex < 8; BitIndex++)
		{
			/* Get the next significant bit of the current byte */
			BitToLoad = ((( pTraceStr[ByteIndex] << BitIndex ) & 0x80 ) / 0x80);

			/* Shift the bit into the register and calculate the new contents.*/
			OMIINO_FRAMER_CRC7_ShiftByte (CurrentByte, BitToLoad);
		}
		
	}
	
	/* Now shift in the trailing seven '0's produced by the multiplication by X^7
	 * Ensure that seven trailling zeroes are added before the contents of BitToLoad were added 7 times
	 */

        BitToLoad=0;


	for (BitIndex = 0; BitIndex < 7; BitIndex++)
	{
		/* Shift in a '0' */
		OMIINO_FRAMER_CRC7_ShiftByte (CurrentByte, BitToLoad); 
	}


	/* A final shift is required to produce the result. Note that the zero	    */
	/* loaded into the shift register does not form part of the 7-digit result. */
	BitToLoad = 0;
	OMIINO_FRAMER_CRC7_ShiftByte (CurrentByte, BitToLoad); 

	/* Finally, convert the contents of the register array to an integer */ 
	for (BitIndex = 7; BitIndex > 0; BitIndex--)
	{
		CRC7_Value = ((2*CRC7_Value) + CurrentByte[BitIndex]);
	}

	return (CRC7_Value |= 0x80);
} 





void OMIINO_FRAMER_PadOutTraceStringWithSpaces(U8 CRC_Index, char * pTraceString )
{
	U8 StringLength = (U8)(strlen(pTraceString));
	U32 i=0;

	if (StringLength < CRC_Index)
	{
		for(i=StringLength; i<CRC_Index; i++)
		{
			pTraceString[i]=' ';
		}
	} /* else it will be truncated */
}



void OMIINO_FRAMER_Add_CRC7_ToTraceString(U8 Mode, char * pTraceString)
{
	U8 CRC_Index=Mode-1;
	OMIINO_FRAMER_PadOutTraceStringWithSpaces(CRC_Index, pTraceString );
	pTraceString[ CRC_Index ] = (char)OMIINO_FRAMER_CRC7_Calculate((U8 *)pTraceString);
	pTraceString[ Mode ] = '\0';
}

