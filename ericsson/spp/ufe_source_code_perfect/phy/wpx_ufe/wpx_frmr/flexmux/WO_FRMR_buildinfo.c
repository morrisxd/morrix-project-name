/*
 *        Copyright (c) 2010  Omiino Ltd
 *
 *        All rights reserved.
 *        This code is provided under license and or Non-disclosure Agreement
 *        and must be used solely for the purpose for which it was provided.
 *        It must not be passed to any third party without the written
 *        permission of Omiino Ltd.
 *
 */




#include <string.h>
#include "WO_FRMR_private.h"




void OMIINO_FRAMER_DefineBuildInformation_ProductRelease(void)
{
        const char * ProductReleaseStr="V0.1";

        strncpy(OMIINO_RAM.Driver.ProductReleaseStr,ProductReleaseStr,OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD);
}


void OMIINO_FRAMER_DefineBuildInformation_SoftwareBuildDateTime(void)
{
        const char * SoftwareBuildDateTimeStr="13 September 2012 12:39:32";

        strncpy(OMIINO_RAM.Driver.SoftwareBuildDateTimeStr,SoftwareBuildDateTimeStr,OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD);
}


void OMIINO_FRAMER_DefineBuildInformation_SoftwareBuildVersion(void)
{
        const char * SoftwareBuildVersionStr="00001745";

        strncpy(OMIINO_RAM.Driver.SoftwareBuildVersionStr,SoftwareBuildVersionStr,OMIINO_FRAMER_MAX_CHARS_IN_DRIVER_INFORMATION_FIELD);
}


void OMIINO_FRAMER_DefineBuildInformation(void)
{
        OMIINO_FRAMER_DefineBuildInformation_ProductRelease();
        OMIINO_FRAMER_DefineBuildInformation_SoftwareBuildDateTime();
        OMIINO_FRAMER_DefineBuildInformation_SoftwareBuildVersion();
}


