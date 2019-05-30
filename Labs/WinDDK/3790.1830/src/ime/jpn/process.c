/*++

Copyright (c) 1990-1998 Microsoft Corporation, All Rights Reserved

Module Name:

    PROCESS.C
    
++*/

/**********************************************************************/
/* These tables are for ImeProcessKey(). And IMEProcessKey function   */
/* refer this table and deside the VKey is needed for FAKEIME or not. */
/*                                                                    */
/**********************************************************************/
#include "windows.h"


BYTE bNoComp[] = {      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 10-1F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 20-2F
                        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,        // 30-3F
                        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // 40-4F
                        1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,        // 50-5F
                        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // 60-6F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 70-7F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 80-8F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 90-9F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // A0-AF
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // B0-BF
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // C0-CF
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // D0-DF
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // E0-EF
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       // F0-FF

BYTE bNoCompCtl[] = {   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 10-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 20-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 30-0F
                        0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,        // 40-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 50-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 60-0F
                        0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,        // 70-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 80-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 90-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // A0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // B0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // C0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // D0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // E0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       // F0-0F

BYTE bNoCompSht[] = {   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 10-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 20-0F
                        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,        // 30-0F
                        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // 40-0F
                        1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,        // 50-0F
                        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // 60-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 70-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 80-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 90-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // A0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // B0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // C0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // D0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // E0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       // F0-0F
        
BYTE bNoCompAlt[] = {   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 10-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 20-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 30-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 40-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 50-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 60-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 70-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 80-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 90-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // A0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // B0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // C0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // D0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // E0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       // F0-0F

BYTE bComp[] = {        0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,        // 10-0F
                        1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,        // 20-0F
                        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,        // 30-0F
                        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // 40-0F
                        1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,        // 50-0F
                        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // 60-0F
                        0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,        // 70-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 80-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 90-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // A0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // B0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // C0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // D0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // E0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       // F0-0F

BYTE bCompCtl[] = {     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 10-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 20-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 30-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 40-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 50-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 60-0F
                        0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,        // 70-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 80-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 90-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // A0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // B0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // C0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // D0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // E0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       // F0-0F

BYTE bCompSht[] = {     0,0,0,1,0,0,0,0,1,1,0,0,0,1,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,        // 10-0F
                        1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,        // 20-0F
                        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,        // 30-0F
                        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // 40-0F
                        1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,        // 50-0F
                        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,        // 60-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 70-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 80-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 90-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // A0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // B0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // C0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // D0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // E0-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};       // F0-0F

BYTE bCompAlt[] = {     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,        // 00-0F
                        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};        // 00-0F




