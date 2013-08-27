/***********************************************************************/
/*                                                                     */
/*  FILE        :fvector.c                                             */
/*  DATE        :Sat, May 03, 2008                                     */
/*  DESCRIPTION :define the fixed vector table.                        */
/*  CPU GROUP   :25                                                    */
/*                                                                     */
/*  This file is generated by Renesas Project Generator (Ver.4.8).     */
/*                                                                     */
/***********************************************************************/

/*******************************************************************
 *  COMPILER for R8C/Tiny
 *  Copyright(c) 2004 Renesas Technology Corp.
 *  And Renesas Solutions Corp.,All Rights Reserved.
 *
 *  resetprg.c : startup file
 *
 *  Function:initialize each function
 *
 * $Date: 2006/09/13 01:35:55 $
 * $Revision: 1.13 $
 *******************************************************************/
#include "cstartdef.h"

#pragma sectaddress     fvector,ROMDATA 0xffdc

//////////////////////////////////////////////////////////////////////////

#pragma interrupt/v _dummy_int  //udi
#pragma interrupt/v _dummy_int  //over_flow
#pragma interrupt/v _dummy_int  //brki
#pragma interrupt/v _dummy_int  //address_match
#pragma interrupt/v _dummy_int  //single_step
#pragma interrupt/v _dummy_int  //wdt
#pragma interrupt/v _dummy_int  //reserved
#pragma interrupt/v _dummy_int  //reserved
#pragma interrupt/v start

#if __WATCH_DOG__ != 0
_asm("    .ofsreg 0FEH");
#else
_asm("    .ofsreg    0FFH");
#endif

_asm("    .id    ""\"#FFFFFFFFFFFFFF\"");

#pragma interrupt _dummy_int()
void _dummy_int(void){}



