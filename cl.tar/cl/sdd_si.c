#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <vxWorks.h>
#include "ctd_sdh.h"
#include "sdd_dev.h"
#include "sdd_base.h"
#include "sdd_sys.h"
#include "sdd_board.h"
#include "sdd_evt.h"
#include "sdd_si.h"
#include "math.h"

extern void sysIOPortWritePin (char port, UINT8 pin, BOOL high);
extern STATUS sysIOPortReadPin (char port, UINT8 pin);
char *gSddsiDeviceNames[] = {
    "SEEPROM",                  /* DEVID_PWR_SEEPROM   */
    NULL
};

#ifndef WYD_HIT7065
#define WYD_HIT7065
#endif



/*
 * Please refer to "HW-15-14-20-01#SI Design Docuement.doc"
 * section 3: "software io"
 * Author is : Jin, Hai
 *
 * And, must refer to 
 *      "  hiT7080 SI FPGA"
 *
 * In hiT7065 please refer to documentation:


hiT7065 S42024-L5607-A1-01-76D8
SI
HW Architecture Document
AUTHOR: Wang, ShuXun


"
page 26, section "memory map"

Index	Chip Select	Signal Name	Bus Width 	Base Address	Memory Scope	Selected Chip
1.		CS1	CS1	32	0x0000_0000~0x07FF_FFFF	128M Bytes 	SDRAM
2.		CS0	CS0	16	0xFFC0_0000~0xFFFF_FFFF	2M Bytes	Boot Flash
3.		CS7	CS7	8	0xA000_0000	1K Bytes	FPGA
4.		CS3	CS3	8	0x6000_0000	512 Bytes	Reserve
5.		CS5	CS5	8	0x8000_0000	128K Bytes	CY7C009V
6.		CS4	CS4	8	0x7000_0000	512 Bytes	Fusion
7.		CS6	CS6	8	0x9000_0000		ACS8514



"
 * 
 */
UINT32
sdd_si_GetDevBaseAddr (SDD_SI_DEV_TYPE_T devType)
{
    sdd_ASSERT (devType >= SI_SDRAM && devType <= SI_INTER);
    switch (devType)
    {
    case SI_SDRAM:
        return 0x0;
    case SI_BOOTFLASH:
#ifdef WYD_SI_7065        
    return 0xFFc00000;
#else 
    return 0xFFE00000;
#endif
    case SI_FPGA:
        /*
         * TOH fpga
         */
        return 0xA0000000;
    case SI_FUSION:
        return 0x70000000;
    case SI_INTER:
        /*
         * What's this?
         * Good luck , NOT used!!!
         */
        return 0xF0000000;
    default:
        return 0xffffffff;
    }
}

SDD_DEV_ECODE_T
sdd_si_SwitchI2cBus (UINT16 devId)
{
    SDD_DEV_ECODE_T rStatus;
    
    sdd_ASSERT (devId >= DEVID_SI_SEEPROM && devId <= DEVID_SI_M41T11);
    
    if (devId == DEVID_SI_SEEPROM)
    {
        rStatus = sdd_SetPCA9544Channel (I2CDEV_SI_PCA9544, 1);
        if (rStatus != SDD_DEV_SUCCESS)
        {
            SDD_LOG (LOG_SDD_I2C, "sdd_8stm14_SwitchI2cBus SetPCA9544Channel failed! devId:%d rStatus:%d\r\n", devId, rStatus);
        }
    }
    else
    {
        /*
         * here is RTC time
         */
        rStatus = sdd_SetPCA9544Channel (I2CDEV_SI_PCA9544/*yes, same as in hiT7080*/, 4/*yes, pin 4th*/);
        if (rStatus != SDD_DEV_SUCCESS)
        {
            SDD_LOG (LOG_SDD_I2C, "sdd_8stm14_SwitchI2cBus SetPCA9544Channel failed! devId:%d rStatus:%d\r\n", devId, rStatus);
        }
    }
    return SDD_DEV_SUCCESS;
}


/*
 * Please refer to "SLAS405A¨CDECEMBER 2003¨CREVISED AUGUST 2005"
 * page 16.
 */
int
sdd_SetDAC5571FanSpeed (unsigned char addr, unsigned char rpm)
{
    unsigned char data[16];

    data[1] = ((rpm) & 0x0f) << 4; // ???
    data[0] = ((rpm) & 0xf0) >> 4;
    if (ERROR == i2cRawWrite (addr, -1, &data[0], 2 ))
    {
        return 2;
    }

printf ("\r\nsdd_SetDAC5571FanSpeed: data[0]=(%x), data[1]=(%x)", data[0], data[1]);    
        
    return SDD_DEV_SUCCESS;
}

typedef struct {
    unsigned char pin;
    unsigned char cell;
} fanpos;

fanpos g_fan[8] = {
    {3, 0x9a},
    {3, 0x9a},
    {3, 0x98},
    {3, 0x98},
    {4, 0x9a}, 
    {4, 0x9a}, 
    {4, 0x98},
    {4, 0x98}
};

int g_isFanOnBoard [2] = {0, 0};

int g_setdac5571 = 1;
int g_isFanInited = 0;
/*
 * fan ::= {1..8}
 */
int
sdd_si_InitFans (int fan, int rpm, int pos)
{
    int fanspeed = 3000; /* rpm */
    unsigned char rpmval = 0;
    unsigned char myregister = 0;
    int ret = 0;
    int channel = 0;
    BOOL fanstate = 0;

    fanstate = 1;
    fanspeed = rpm;
    
    /*
     * Single step operation may need no lock, 
     * but there is multiple operation(s) bellow.
     */
    if (PB_OK != (ret = sdd_LockI2cBus (WAIT_FOREVER)))
    {
        return printf ("\r\nsi_initfan: locki2cbus failed(%d)pos(%d)", ret,pos);
    }
    /*
     * Step 1:
     * 
     * switch SI card's pca9544
     */
    channel = (1 == pos)?4:3;
    if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9544Channel (0xe2, channel)))
    {
        sdd_UnlockI2cBus ();
        return printf ("\r\nsi_initfans: si-set9544channel failed(%d)fan(%d)pin(%d)pos(%d)", ret, fan, g_fan[fan-1].pin, pos);
    }


    if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9545Channel (0xe6, g_fan[fan - 1].pin)))
    {
        /*
         * If  we failed here, there maybe fan ABSENT.
         */
        g_isFanOnBoard [pos - 1] = 0;
        sdd_UnlockI2cBus ();
        sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
        return printf ("\r\nsi_initfans: set9545channel failed(%d)fan(%d)pin(%d)pos(%d)", ret, fan, g_fan[fan-1].pin, pos);
    }


    g_isFanOnBoard [pos - 1] = 1;
    
    if (g_setdac5571)
    {
        /*
         * Step 2:
         * 
         * access to DAC5571 controller
         */
        if (SDD_DEV_SUCCESS != (ret = sdd_SetDAC5571FanSpeed (g_fan[fan - 1].cell, (fanspeed * 255)/ 7000)))
        {
            sdd_UnlockI2cBus ();
            sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
            return printf ("\r\nsi_initfan: setdac5571fanspeed faild(%d)pos(%d)addr(%x)", ret,pos, g_fan[fan-1].cell);
        } else {
            // printf ("\r\nsdd_si_initfans: sdd_SetDAC5571FanSpeed OK addr(%x)!!!", g_fan[fan - 1].cell);
        }
    } else {
        printf ("\r\nsdd_si_initfans: SKIP set dac5571, pos(%d)fan(%d) OK", pos, fan);
    }

    /*
     * Step 3:
     *
     * Set FAN rpm speed threshold now
     */
    if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9544Channel (0xe2, channel)))
    {
        sdd_UnlockI2cBus ();
        sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
        return printf ("\r\nsi_initfans: si-set9544channel-2 failed(%d)fan(%d)pin(%d)pos(%d)", ret, fan, g_fan[fan-1].pin, pos);
    }

    if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9545Channel (0xe6, 2)))    // max6639 is in the 2nd pin of 9544
    {
        sdd_UnlockI2cBus ();
        sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
        return printf("\r\nsi_initfan: set rpm threshold set9545channel failed(%d)pos(%d)", ret, pos);
    }
    
    if (fanspeed >0 && fanspeed < 2000)
    {
        rpmval = 0x00;
    } else if (fanspeed >= 2000 && fanspeed <4000) {
        rpmval = 0x01;
    } else if (fanspeed >= 4000 && fanspeed < 8000) {
        rpmval = 0x02;
    } else if (fanspeed >= 8000 && fanspeed < 16000) {
        rpmval = 0x03;
    }

    if (1 == fan || 3 == fan || 2 == fan || 4 == fan)
    {
        myregister = 0x10;
    } else {
        myregister = 0x14;
    }
    
    if (SDD_DEV_SUCCESS != (ret=sdd_SetMAX6639RPMClkFreq (0x5e, myregister, rpmval)))
    {
        sdd_UnlockI2cBus ();
        sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
        return printf ("\r\nsi_initfan: setmax6639 failed(%d)pos(%d)", ret, pos);
    } 

    printf ("\r\nsdd_si_initfans:  sdd_SetMAX6639RPMClkFreq register(%2x), rpmval(%2x)", myregister, rpmval);



    /*
     * we set actual fan speed now...
     */
    if (1 == fan || 2 == fan || 3 == fan || 4 == fan)
    {
        channel = 3;
    } else {
        channel = 4;
    }
    
    if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9545Channel (0xe6, 1)))    // max6639 is in the 2nd pin of 9544
    {
        sdd_UnlockI2cBus ();
        sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
        return printf("\r\nsi_initfan: set polarity&directioni set9545channel failed(%d)pos(%d)", ret, pos);
    }


    if(SDD_DEV_SUCCESS != sdd_SetPCA9555AllPinDirection (0x4e, 0x0000))
    {
        printf ("\r\nsdd_read_fan:set all pin dir failed");
        return PB_ERROR;
    }
    
    if (SDD_DEV_SUCCESS != sdd_SetPCA9555AllPinPolarity (0x4e, 0x0000))
    {
        printf ("\r\nsdd_read_fan:set all pin polarity failed");
        return PB_ERROR;
    }


    fanstate = 1;
    // sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan);
    sdd_UnlockI2cBus ();
    printf ("\r\nsi_initfans: success! pos(%d)fan(%d)", pos, fan);
    
    return 0;
}



extern BOOL gbNERebootState;
static STATUS
sdd_si_ApplyOutBits (SDD_SI_DATA_T * pData)
{
    BOOL blinkOn, bLedTest;
    UINT32 mdoRegAddr;
    UINT32 alarmOutRegAddr;
    UINT8 mdoValue;
    UINT8 alarmOutValue;
    UINT8 port;

    /* get LED blink stage, TRUE if blink on, FALSE if blink off */

    /*
     * led control 
     */
    blinkOn = sdd_GetLedBlinkStage ();
    bLedTest = sdd_GetLedTestStatus ();

    /*
     * Please refer to Hardware documentation - SI function specification:
     *  "S42024-L5607-A1-01-76D8.doc", page 27, "gpio definition"
     * Author is Wang, ShuXun
     */
    if (bLedTest || gbNERebootState == SDD_REBOOT_STATE_START)
    {
        sysIOPortWritePin ('C', 16 - 5, blinkOn);
        sysIOPortWritePin ('C', 16 - 6, blinkOn);
        sysIOPortWritePin ('C', 16 - 7, blinkOn);
        if (gbNERebootState == SDD_REBOOT_STATE_START)
        {
            switch (pData->led_faulty)
            {
            case LED_STATE_ON:
                sysIOPortWritePin ('D', 16 - 4, 1);
                break;
            case LED_STATE_OFF:
                sysIOPortWritePin ('D', 16 - 4, 0);
                break;
            case LED_STATE_BLINK:
                sysIOPortWritePin ('D', 16 - 4, !blinkOn);
                break;
            default:
                break;
            }
        }
        else
        {
            sysIOPortWritePin ('D', 16 - 4, !blinkOn);
        }
        SDD_DBG (("sdd_si_ApplyOutBits led test!\r\n"), SDD_DBG_LED);
    }
    else
    {
        switch (pData->led_faulty)
        {
        case LED_STATE_ON:
            sysIOPortWritePin ('D', 16 - 4, 1);
            break;
        case LED_STATE_OFF:
            sysIOPortWritePin ('D', 16 - 4, 0);
            break;
        case LED_STATE_BLINK:
            sysIOPortWritePin ('D', 16 - 4, !blinkOn);
            break;
        default:
            break;
        }

        switch (pData->led_critical)
        {
        case LED_STATE_ON:
            sysIOPortWritePin ('C', 16 - 6, 0);
            break;
        case LED_STATE_OFF:
            sysIOPortWritePin ('C', 16 - 6, 1);
            break;
        case LED_STATE_BLINK:
            sysIOPortWritePin ('C', 16 - 6, blinkOn);
            break;
        default:
            break;
        }

        switch (pData->led_major)
        {
        case LED_STATE_ON:
            sysIOPortWritePin ('C', 16 - 5, 0);
            break;
        case LED_STATE_OFF:
            sysIOPortWritePin ('C', 16 - 5, 1);
            break;
        case LED_STATE_BLINK:
            sysIOPortWritePin ('C', 16 - 5, blinkOn);
            break;
        default:
            break;
        }

        switch (pData->led_minor)
        {
        case LED_STATE_ON:
            sysIOPortWritePin ('C', 16 - 7, 0);
            break;
        case LED_STATE_OFF:
            sysIOPortWritePin ('C', 16 - 7, 1);
            break;
        case LED_STATE_BLINK:
            sysIOPortWritePin ('C', 16 - 7, blinkOn);
            break;
        default:
            break;
        }
    }
     /****************************end of led confrol**************************/

     /*****************************alarmout & mdo*****************************/
     /*
      * And, must refer to 
      *      "  hiT7080 SI FPGA"
      * page 15, 
      * section "2.2.1.28 mdo"
      */
    mdoRegAddr = sdd_si_GetDevBaseAddr (SI_FPGA) + SDD_SI_MDO_OFFSET;
    mdoValue = SDD_SI_READ8 (mdoRegAddr);
    SDD_DBG (("sdd_SI_PollChanges Get mdo status:0x%x!\r\n", mdoValue), SDD_DBG_SI);
    for (port = 0; port < SDD_SI_MDO_NUM; port++)
    {
        if (pData->mdo[port])
        {
            mdoValue |= (0x01 << port);
        }
        else
        {
            mdoValue &= ~(0x01 << port);
        }
    }
    SDD_DBG (("sdd_SI_PollChanges Set mdo status:0x%x!\r\n", mdoValue), SDD_DBG_SI);
    SDD_SI_WRITE8 (mdoRegAddr, mdoValue);

    /*
     * And, must refer to 
     *      "  hiT7080 SI FPGA"
     * page 14, 
     * section "2.2.1.26 alarm"
     */
    alarmOutRegAddr = sdd_si_GetDevBaseAddr (SI_FPGA) + SDD_SI_ALARMOUT_OFFSET;
    alarmOutValue = SDD_SI_READ8 (alarmOutRegAddr);
    for (port = 0; port < SDD_SI_ALARMOUT_NUM; port++)
    {
        if (pData->alarm_out[port])
        {
            alarmOutValue |= (0x01 << port);
        }
        else
        {
            alarmOutValue &= ~(0x01 << port);
        }
    }
    SDD_SI_WRITE8 (alarmOutRegAddr, alarmOutValue);
     /****************************end of alarm out & mdo**********************/
    return PB_OK;

}


/*===========================================================================*/
/*                      Common functions for external use                    */
/*===========================================================================*/


int
sdd_init_fan (int pos)
{


    if (1 == pos)
    {
        sdd_si_InitFans (1, 3000, 1);
        sdd_si_InitFans (2, 3000, 1);
        sdd_si_InitFans (3, 3000, 1);
        sdd_si_InitFans (4, 3000, 1);
        sdd_si_InitFans (5, 3000, 1);
        sdd_si_InitFans (6, 3000, 1);
        sdd_si_InitFans (7, 3000, 1);
        sdd_si_InitFans (8, 3000, 1);
    }

    if (2 == pos)
    {
        sdd_si_InitFans (1, 3000, 2);
        sdd_si_InitFans (2, 3000, 2);
        sdd_si_InitFans (3, 3000, 2);
        sdd_si_InitFans (4, 3000, 2);
        sdd_si_InitFans (5, 3000, 2);
        sdd_si_InitFans (6, 3000, 2);
        sdd_si_InitFans (7, 3000, 2);
        sdd_si_InitFans (8, 3000, 2);

    }

    if (3 == pos)
    {
        sdd_si_InitFans (1, 3000, 1);
        sdd_si_InitFans (2, 3000, 1);
        sdd_si_InitFans (3, 3000, 1);
        sdd_si_InitFans (4, 3000, 1);
        sdd_si_InitFans (5, 3000, 1);
        sdd_si_InitFans (6, 3000, 1);
        sdd_si_InitFans (7, 3000, 1);
        sdd_si_InitFans (8, 3000, 1);
        
        
        sdd_si_InitFans (1, 3000, 2);
        sdd_si_InitFans (2, 3000, 2);
        sdd_si_InitFans (3, 3000, 2);
        sdd_si_InitFans (4, 3000, 2);
        sdd_si_InitFans (5, 3000, 2);
        sdd_si_InitFans (6, 3000, 2);
        sdd_si_InitFans (7, 3000, 2);
        sdd_si_InitFans (8, 3000, 2);

    }
    return 0;
}

STATUS
sdd_SI_InitCard (SDD_SI_DATA_T * pData, SDD_SLOT_INDEX_T idSlot, SDD_AREA_INDEX_T idArea, BOOL ulOption)
{
    UINT32 port;
    sdd_ASSERT (pData != NULL);

    if (pData->head.state[idArea] != SDD_AREA_STATE_EMPTY)
    {
        return PB_ERROR;
    }

    memset (pData, 0, sizeof (SDD_SI_DATA_T));

    pData->head.idSlot = idSlot;
    pData->head.state[idArea] = SDD_AREA_STATE_INIT;


    /* Initialize default output settings */

    pData->led_faulty = LED_STATE_UNKNOWN;
    pData->led_critical = LED_STATE_UNKNOWN;
    pData->led_major = LED_STATE_UNKNOWN;
    pData->led_minor = LED_STATE_UNKNOWN;

    for (port = 0; port < SDD_SI_ALARMOUT_NUM; port++)
    {
        pData->alarm_out[port] = FALSE;
    }

    for (port = 0; port < SDD_SI_MDO_NUM; port++)
    {
        pData->mdo[port] = FALSE;
    }

    for (port = 0; port < SDD_SI_MDI_NUM; port++)
    {
        pData->mdi[port] = FALSE;
    }
    return PB_OK;
}

STATUS
sdd_SI_ActivateCard (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea, BOOL bWarm)
{

    sdd_ASSERT (pData != NULL);

    if (pData->head.state[idArea] != SDD_AREA_STATE_INIT)
    {
        return PB_ERROR;
    }

    /*=====================================================================*/
    /*                  Initialize data structure                          */
    /*=====================================================================*/

    pData->head.state[idArea] = SDD_AREA_STATE_ACTIVE;


    sdd_init_fan (3);

    taskDelay (sysClkRateGet ()*3);
#if 0    
    SDD_SI_WRITE8 (0xa0000217, 0xff);
    SDD_SI_WRITE8 (0xa0000218, 0xff);
#endif    

    return PB_OK;
}

STATUS
sdd_SI_DeactivateCard (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea)
{
    sdd_ASSERT (pData != NULL);

    if (pData->head.state[idArea] != SDD_AREA_STATE_ACTIVE)
    {
        return PB_ERROR;
    }
    pData->led_critical = LED_STATE_OFF;
    pData->led_major = LED_STATE_OFF;
    pData->led_minor = LED_STATE_OFF;
    sysIOPortWritePin ('C', 16 - 5, 1);
    sysIOPortWritePin ('C', 16 - 6, 1);
    sysIOPortWritePin ('C', 16 - 7, 1);
    pData->head.state[idArea] = SDD_AREA_STATE_INIT;

    return PB_OK;
}

STATUS
sdd_SI_DeinitCard (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea)
{
    sdd_ASSERT (pData != NULL);

    if (pData->head.state[idArea] != SDD_AREA_STATE_INIT)
    {
        return PB_ERROR;
    }

    pData->head.state[idArea] = SDD_AREA_STATE_EMPTY;

    return PB_OK;
}

/*
 * this function is called by sdd_MergeInputData ();
 */
STATUS
sdd_SI_MergeInputData (SDD_SI_DATA_T * pPermData, SDD_SI_DATA_T * pTempData)
{
    sdd_ASSERT (pPermData && pTempData);
    if (pPermData->head.state[SDD_AREA_INDEX_A0] != SDD_AREA_STATE_ACTIVE && pPermData->head.state[SDD_AREA_INDEX_A0] != SDD_AREA_STATE_INIT)
    {
        SDD_DBG (("sdd_SI_MergeInputData! card state is:%d\r\n", pPermData->head.state[SDD_AREA_INDEX_A0]), SDD_DBG_POLL);
        return PB_ERROR;
    }
    pPermData->defect_board_overheat = pTempData->defect_board_overheat;

    memcpy (pPermData->mdi, pTempData->mdi, sizeof (pPermData->mdi));

    pPermData->button_aco = pTempData->button_aco;
    pPermData->button_ledtest = pTempData->button_ledtest;
    pPermData->button_suppress = pTempData->button_suppress;
    return PB_OK;
}

#if 0
static int g_fanI2cAddr[4][2] = {
    {0xe6, 0x07},
    {0xe6, 0x06},
    {0xe6, 0x05},
    {0xe6, 0x04}
};
#endif

int g_testfan = 1;

int report_fan_failur_or_absent (int pos)
{
    return FAN_STATUS_FAIL;
}

int g_fanrpm [8][2];

int
init_rpm (void)
{
    int i = 0;
    int j = 0;
    static int flag = 0;

    if (1 == flag)
    {
        return 0;
    }

    flag = 1;

    for (i = 0; i < 8; i ++)
    {
        for (j = 0; j < 2; j ++)
        {
            g_fanrpm [i][j] = 0;
        }
    }
    
    return 0;
}

int i2cinterval = 3;
int sddi2cinterval2 = 2;
int g_readback = 0;

#ifdef SDD_DEBUG_FAN
#undef SDD_DEBUG_FAN
#endif

/*
 * fan :== {1..8}
 *
 * Return :
 *      {FAN_STATUS_OK, FAN_STATUS_FAIL}
 */
FAN_STATUS_T 
sdd_read_fan (int fan, int pos)
{
    int val = 0;
    int ret = 0;
    unsigned char channel = 0;
    int fanstate = 1;
    int rpm = 0;
#ifdef SDD_DEBUG_FAN    
    int myfan = 0;
#endif
    
    if (1 > fan || 8 < fan)
    {
        return -1;
    }

    /*
     * Single step operation may need no lock, 
     * but there is multiple operation(s) bellow.
     */

    /*
     * post initialization
     */
    if (1 == pos)
    {
        channel = 4;
    } else {
        channel = 3;
    }

#ifdef SDD_DEBUG_FAN
    for (myfan = 1; myfan <= 8; myfan ++)
    {
#endif

        
        if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9544Channel (I2CDEV_SI_PCA9544, I2C_EXPANDER_CHANNEL_NULL)))
        {
            printf ("\r\nsdd_read_fan: clear pca9544_1 failed on pos(%d)fan(%d)", pos, fan);
        }

        /*
         * we clear pca9544 here ...
         */
        if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9544Channel (0xe2, I2C_EXPANDER_CHANNEL_NULL)))
        {
            printf ("\r\nsdd_read_fan: clear pca9544 failed on pos(%d)fan(%d)", pos, fan);
        }

        /*
         * and set the channel needed
         */
        if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9544Channel (0xe2, channel)))
        {
            sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
            if (g_testfan)
            {
                printf ("\r\nsdd_read_fan: post-set9544channel failed(%d)fan(%d)pin(%d)", ret, fan, g_fan[fan-1].pin);
            }
            return PB_ERROR/**/;
        }
    taskDelay (sysClkRateGet () /i2cinterval);


        /*
         * Step 1:
         * 
         * The pca9555 is in the 1st channel of the pca9544
         * If we meet error here , perhaps the fan is ABSENT, 
         * not failed.
         * But FAILURE is certain afer this step!!!
         */

        if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9545Channel (0xe6, I2C_EXPANDER_CHANNEL_NULL)))
        {
            g_isFanOnBoard [pos - 1] = 0;
            sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
            // printf ("\r\nsdd_read_fan: clear pca9545 failed on pos(%d)fan(%d)", pos, fan);
            return 0;
        }
        

        if (0 == g_isFanOnBoard [pos - 1])
        {
            printf ("\r\nsdd_read_fan: fan re-inserted on pos(%d)", pos);
            g_isFanOnBoard [pos - 1] = 1;
            sdd_init_fan (pos);
        }
        
        if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9545Channel (0xe6, 1)))
        {
            printf ("\r\nsdd_read_fan: select pca9545 channel failed");
            sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
            return report_fan_failur_or_absent (pos);
        }
        taskDelay (sysClkRateGet () /i2cinterval);

#if 0
        if(SDD_DEV_SUCCESS != sdd_SetPCA9555AllPinDirection (0x4e, 0x0000))
        {
            printf ("\r\nsdd_read_fan:set all pin dir failed");
            return PB_ERROR;
        }
        
        if (SDD_DEV_SUCCESS != sdd_SetPCA9555AllPinPolarity (0x4e, 0x0000))
        {
            printf ("\r\nsdd_read_fan:set all pin polarity failed");
            return PB_ERROR;
        }
#endif

        /*
         * Step 1:
         *
         * then we can set the expected fan now.
         *

         IC  IO/REG  Function
     PCA9555 P0.0    Clear to enable FAN 1 monitor signal to MAX6639 tachometer input channel 1
             P0.1    Clear to enable FAN 2 monitor signal to MAX6639 tachometer input channel 1
             P0.2    Clear to enable FAN 3 monitor signal to MAX6639 tachometer input channel 1
             P0.3    Clear to enable FAN 4 monitor signal to MAX6639 tachometer input channel 1
             P0.4    Clear to enable FAN 5 monitor signal to MAX6639 tachometer input channel 2
             P0.5    Clear to enable FAN 6 monitor signal to MAX6639 tachometer input channel 2
             P0.6    Clear to enable FAN 7 monitor signal to MAX6639 tachometer input channel 2
             P0.7    Clear to enable FAN 8 monitor signal to MAX6639 tachometer input channel 2
     MAX6639 0x20    Get the tachometer 1 channel rotate speed
             0x21    Get the tachometer 2 channel rotate speed

    The address of pca9555 connect to the 9544 above 
    is 0x4e, so we set the pca9555's pin now.

         *
         *
         */
        if (SDD_DEV_SUCCESS != (ret = sdd_SetMAX6639PCA9555PinState(0x4e, 
#ifdef SDD_DEBUG_FAN            
                    myfan - 1, 
#else
                    (fan - 1) % 4, 
#endif
                    1/* "clear to enable FAN n monitor signale to MAX6639 input channel n"*/)))
        {
            if (g_testfan)
            {
#ifdef SDD_DEBUG_FAN            
                printf ("\r\nsdd_readfan: setmax6639pca9555 failed(%d)fan(%d)pos(%d)", ret, myfan, pos);
#else
                printf ("\r\nsdd_readfan: setmax6639pca9555 failed(%d)fan(%d)pos(%d)", ret, fan, pos);
#endif
            }
            sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
            return FAN_STATUS_FAIL;
            // return printf ("\r\nsdd_readfan: setmax6639pca9555 failed(%d)", ret);
        }

#if 1
if (g_readback)
{
        /*
         * This is only for debug
         */

#ifdef SDD_DEBUG_FAN            
        ret = sdd_GetPCA9555PinState (0x4e, myfan - 1, &val);
#else
        ret = sdd_GetPCA9555PinState (0x4e, fan - 1, &val);
#endif
        if (SDD_DEV_SUCCESS != (ret))
        {
            printf ("\r\nsdd_read_fan: GetPCA9555PinState error");
        } else {
#ifdef SDD_DEBUG_FAN            
            printf ("\r\nsdd_read_fan: read pca9555 on pin(%d)(%d)", myfan - 1, val);
#else
            printf ("\r\nsdd_read_fan: read pca9555 on pin(%d)(%d)", fan - 1, val);
#endif
        }
        
        taskDelay (sysClkRateGet () /i2cinterval);
}
#endif        
        

        if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9545Channel (0xe6, I2C_EXPANDER_CHANNEL_NULL)))
        {
            printf ("\r\nsdd_read_fan: clear2 pca9545 failed on pos(%d)fan(%d)", pos, fan);
        }
        /*
         * Step 3:
         *
         * The max6639 is in the 2nt channel of the pca9544
         */
        if (SDD_DEV_SUCCESS != (ret = sdd_SetPCA9545Channel (0xe6, 2)))
        {
            if (g_testfan)
            {
                printf ("\r\nsdd_readfan: set9545channel failed2(%d)", ret);
            }
            sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
            return FAN_STATUS_FAIL;
        }
        taskDelay (sysClkRateGet () /i2cinterval);

        if (g_testfan == 3)
        {
            printf ("\r\nsdd_read_fan: waitting for get max6635 on fan(%d)", fan);
        }
        taskDelay (sysClkRateGet () * sddi2cinterval2);


        /*
         * We read the fan speed now...
         *
    MAX6639 0x20    Get the tachometer 1 channel rotate speed
            0x21    Get the tachometer 2 channel rotate speed
         *
         */
         
#ifdef SDD_DEBUG_FAN            
        if (SDD_DEV_SUCCESS != (ret = sdd_GetMAX6639RPM(0x5e, ((1<=myfan && 4>= myfan)?0x20:0x21), &val)))
#else
        if (SDD_DEV_SUCCESS != (ret = sdd_GetMAX6639RPM(0x5e, ((1<=fan && 4>= fan)?0x20:0x21), &val)))
#endif
        {
            if (g_testfan)
            {
#ifdef SDD_DEBUG_FAN            
                printf ("\r\nsdd_readfan: getmax6639_2 failed(%d)fan(%d)pos(%d)", ret, myfan, pos);
#else
                printf ("\r\nsdd_readfan: getmax6639_2 failed(%d)fan(%d)pos(%d)", ret, fan, pos);
#endif
            }
            sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_eqp_ff);
            return FAN_STATUS_FAIL;
        } else {
            if (g_testfan)
            {
    /*
     * 
     
    FAN1: xxxx RPM
    FAN2: xxxx RPM
    FAN3: xxxx RPM
    FAN4: xxxx RPM
    FAN5: xxxx RPM
    FAN6: xxxx RPM
    FAN7: xxxx RPM
    FAN8: xxxx RPM
      
     */
                rpm = (2000 * 60)/val;
                printf ("\r\nFAN[%d]: %5d RPM  --- RAW(%4d)POS(%s)", 
#ifdef SDD_DEBUG_FAN            
                    myfan, rpm, val, pos);
#else
                    fan, rpm, val, (1==pos)?"LOWER":"UPPER");
#endif
            }
        }
        taskDelay (sysClkRateGet () /i2cinterval);
        

        init_rpm ();
        
        fanstate = 1;
        if (3500 > rpm && 2500 < rpm)
        {
            g_fanrpm[fan-1][pos-1] = 0;
        } else {
            g_fanrpm[fan-1][pos-1] ++;
        }

        if (3 < g_fanrpm[fan-1][pos-1])
        {
            fanstate = 1;
        } else {
            fanstate = 0;
        }
#ifdef SDD_DEBUG_FAN
    }    
#endif    
    sdd_ReportFanInputLoss ((1 == pos)?SDD_SLOT_INDEX_FAN1:SDD_SLOT_INDEX_FAN2,0, SDD_BOARD_TYPE_FAN, fanstate, fan, defect_id_fan_failure);

    return (FAN_STATUS_OK);
}


int showfan (void)
{
    g_testfan = 1;
    return 0;
}

int notshowfan (void)
{
    g_testfan = 0;
    return 0;
}

int g_islock = 1;
int g_locksef = 0;

extern SEM_ID gSDDTaskPollSemID;
/*
 * Code in this function is copied 
 * from the function sdd_MC_PollChanges ()
 * from sdd_mc_7080.c file
 */
int sdd_scan_fan_and_power (SDD_SI_DATA_T * pData, int pos)
{
    // SDD_DEV_ECODE_T ret = 0;
    int fan = 0;

    /*
     * There is NOT any control of fan&power anymore 
     * in MC in  hiT7065.
     * Smart card SI will take this responsibility.
     * So we set gbSddScStatus = 0 here for ever.
     */
    if (1)
    {
        FAN_STATUS_T fanStatus;
        // POWER_STATUS_T pwrStatus;
        /*
         * what is the meaning of sysIOPortReadPin () ?
         * Please refer to the page 25, 
         * "PC9", SC func of hiT7080.
         */

        /*
         * fan 1
         */
        sdd_LockI2cBus (WAIT_FOREVER);         
#ifdef SDD_DEBUG_FAN         
        for (fan = 1; fan <= 1; fan ++)
#else
        for (fan = 1; fan <= 8; fan ++)
#endif
        {
            if (g_islock)
            {
                if (g_locksef)
                semTake(gSDDTaskPollSemID, WAIT_FOREVER);
                sdd_LockI2cBus (WAIT_FOREVER);
            }
            fanStatus = sdd_read_fan (fan, pos);
            if (g_islock)
            {
                sdd_UnlockI2cBus ();
                if (g_locksef)
                semGive (gSDDTaskPollSemID);
            }

#if 0
            if (pData->fan_status[fan] != fanStatus)
            {
                if (fanStatus == FAN_STATUS_FAIL)
                {
                    sdd_ReportUnitFailState (SDD_SI_SLOT_INDEX_FAN1, SDD_AREA_INDEX_A0, SDD_BOARD_TYPE_MC, TRUE);
                }
                else
                {
                    sdd_ReportUnitFailState (SDD_SI_SLOT_INDEX_FAN1, SDD_AREA_INDEX_A0, SDD_BOARD_TYPE_MC, FALSE);
                }
                pData->fan_status[fan] = fanStatus;
            }
#endif            
        }


        sdd_UnlockI2cBus ();
        
        if (g_testfan)
        {
            printf ("\r\n\r\n");
        }
    }

    return 0;
}

int g_sihb = 0;

int sdd_si_start_fan_dog (SDD_SI_DATA_T * pData);


#ifdef SDD_TEMPERATURE
#undef SDD_TEMPERATURE
#endif

int g_skippoll = 0;


STATUS
sdd_SI_PollChanges (SDD_SI_DATA_T * pData)
{
    UINT32 mdiRegAddr;
    UINT8 mdiValue;
    UINT8 port;
#ifdef SDD_TEMPERATURE    
    UINT8 tempChipNum;
    INT16 clear_defect_overheat = 0;
    unsigned short temp1, temp2 = 0;
    SDD_DEV_ECODE_T val = 0;
#endif
    STATUS bVal;
    STATUS ret = 0;

    if (g_skippoll) return PB_OK;

    if (pData->head.state[SDD_AREA_INDEX_A0] != SDD_AREA_STATE_ACTIVE)
    {
        /* This card is not activated */
        return PB_ERROR;
    }

    if (g_locksef)
    semTake(gSDDTaskPollSemID, WAIT_FOREVER);

#ifdef SDD_TEMPERATURE
if (g_sihb) printf ("\r\nsi_pollchanges: heart beat, slot(%d)", pData->head.idSlot);
    /*************************Get Temperature from fusion fpga*****************************/
    /*
     * But in hiT7065, temperature is read from i2c LM75
     */
#ifdef WYD_HIT7065
#if 1
    /*
     * pca9544 upper
     */
    sdd_SetPCA9544Channel (0xe0, 1);
#endif
    /*
     * now we read LM75 here ...
     */
    val = sdd_GetLM75Temperature (0x92, &temp1);
    if (val) 
    {
        printf ("\r\nsdd_si_poll: read temp1 failed on(%d)", val);
    }

    val = sdd_GetLM75Temperature (0x94, &temp2);
    if (val) 
    {
        printf ("\r\nsdd_si_poll: read temp2 failed on(%d)", val);
    }
    pData->temperature[0] = temp1;
    pData->temperature[1] = temp2;
    if (g_sihb) printf ("\r\nsi_poll: temp1(%3d)temp2(%3d)", temp1, temp2);
    
#else
    pData->temperature[0] = ((((INT16) (SDD_SI_READ8 (0x70000103)) << 4) & 0x0f00) | (INT16) (SDD_SI_READ8 (0x70000104))) - 273 - 14;
    pData->temperature[1] = ((((INT16) (SDD_SI_READ8 (0x70000105)) << 4) & 0x0f00) | (INT16) (SDD_SI_READ8 (0x70000106))) - 273 - 14;
    SDD_DBG (("Get SI temperature from fusion. temp[1]:%d, temp[2]:%d\r\n", pData->temperature[0], pData->temperature[1]), SDD_DBG_TEMP);
#endif    

    /*********************** check Temperature **************************/
    for (tempChipNum = 0; tempChipNum < SDD_SI_TEMP_NUM; tempChipNum++)
    {
        if (pData->temperature[tempChipNum] == SDD_INVALID_TEMPERATURE ||
            pData->threshold_temp_high == SDD_INVALID_TEMPERATURE ||
            pData->threshold_temp_low == SDD_INVALID_TEMPERATURE || pData->threshold_temp_low > pData->threshold_temp_high)
        {
            /* For invalid thresholds and temperature, we report clear defect */
            clear_defect_overheat |= (0x1 << tempChipNum);
        }
        else
        {
            if (pData->temperature[tempChipNum] >= pData->threshold_temp_high && pData->defect_board_overheat == FALSE)
            {
                sdd_ReportBoardTemperatureEvent (pData->head.idSlot, SDD_AREA_INDEX_A0, pData->head.type, TRUE);
                pData->defect_board_overheat = TRUE;
                break;
            }
            else if (pData->temperature[tempChipNum] < pData->threshold_temp_low)
            {
                clear_defect_overheat |= (0x1 << tempChipNum);
            }
        }
    }
    if (clear_defect_overheat == 0x3 && pData->defect_board_overheat == TRUE)
    {
        sdd_ReportBoardTemperatureEvent (pData->head.idSlot, SDD_AREA_INDEX_A0, pData->head.type, FALSE);
        pData->defect_board_overheat = FALSE;
    }
#endif


    /*
     *
     * Get mdi status
     *
     */
    mdiRegAddr = sdd_si_GetDevBaseAddr (SI_FPGA) + SDD_SI_MDI_OFFSET;
    mdiValue = SDD_SI_READ8 (mdiRegAddr);
    for (port = 0; port < SDD_SI_MDI_NUM; port++)
    {
        if (mdiValue & (0x01 << port))
        {
            if (pData->mdi[port] == TRUE)
            {
                sdd_ReportMdiEvent (pData->head.idSlot, pData->head.type, port, FALSE);
                pData->mdi[port] = FALSE;
            }
        }
        else
        {
            if (pData->mdi[port] == FALSE)
            {
                sdd_ReportMdiEvent (pData->head.idSlot, pData->head.type, port, TRUE);
                pData->mdi[port] = TRUE;
            }
        }
    }
    SDD_DBG (("sdd_SI_PollChanges Get mdi status:0x%x!\r\n", mdiValue), SDD_DBG_SI);
    /*
     ****************************Process button events on SI card*********************************
     *
     * Please refer to documentation:
     *
"
Project	00:	hiT7065
Unit	30:	SI
Category	20:	HW Architecture Document
File Name	01:	S42024-L5607-A1-01-76D8.doc
Author    Wang, ShuXun

"     

page 27, section 5.5: GPIO definition.

     */

    /*
     * AOC button
     */
    bVal = sysIOPortReadPin ('B', 32 - 14);
    if (bVal != pData->button_suppress)
    {
        sdd_ReportButtonEvent (pData->head.idSlot, pData->head.type, SDD_BUTTON_EVENT_TYPE_ALARMSUPPRESS, 0, bVal);
        pData->button_suppress = bVal;
    }
    SDD_DBG (("sdd_SI_PollChanges Get button events button_suppress:%d\r\n", bVal), SDD_DBG_SI);


    /*
     * SUP button
     */
    bVal = sysIOPortReadPin ('B', 32 - 15);
    if (bVal != pData->button_aco)
    {
        sdd_ReportButtonEvent (pData->head.idSlot, pData->head.type, SDD_BUTTON_EVENT_TYPE_ALARMCUTOFF, 0, bVal);
        pData->button_aco = bVal;
    }
    SDD_DBG (("sdd_SI_PollChanges Get button events button_aco:%d\r\n", bVal), SDD_DBG_SI);

    /*
     * LED button
     */
    bVal = sysIOPortReadPin ('B', 32 - 16);
    if (bVal != pData->button_ledtest)
    {
        sdd_ReportButtonEvent (pData->head.idSlot, pData->head.type, SDD_BUTTON_EVENT_TYPE_LEDTEST, 0, bVal);
        pData->button_ledtest = bVal;
    }
    SDD_DBG (("sdd_SI_PollChanges Get button events ledtest:%d\r\n", bVal), SDD_DBG_SI);




#if 0        
    sdd_scan_fan_and_power (pData, 1);
    sdd_scan_fan_and_power (pData, 2);
#endif

    ret = sdd_si_ApplyOutBits (pData);
    if (g_locksef)
    semGive (gSDDTaskPollSemID);

    sdd_si_start_fan_dog (pData);

    return ret;

}

SDD_SI_DATA_T * g_sddpData = NULL;

int g_sddseconds = 5;
int skipfandog = 0;

int g_fanpollingcnt = 0;
int sdd_fan_dog (void)
{
    SDD_SI_DATA_T * pData = NULL;

    pData = g_sddpData;
    
    while (1)
    {
        if (skipfandog) continue;
        /*
         * We slow down the interval of polling fan, 
         * once every 30 seconds
         */
        g_fanpollingcnt ++;
        if (0 != (g_fanpollingcnt % g_sddseconds))
        {
            taskDelay (sysClkRateGet ());
            continue;
        }

        sdd_scan_fan_and_power (pData, 1);
        sdd_scan_fan_and_power (pData, 2);
    }
}

extern STATUS vxMemProbe(char * adrs, int mode, int length, char * pVal);

int g_debugfeed = 1;

int sdd_fan_feed_dog (void)
{
    static int i = 0;
    unsigned short val = 0;
    
    while (1)
    {
        taskDelay (sysClkRateGet () / 10);
        i ++;

        if (-1 == vxMemProbe((char *)0xa0000217, VX_READ, 2, (char *)&val))
        {
            printf ("\r\nsdd_fan_feed_dog: memory inaccessable");
            taskDelay (sysClkRateGet () * g_debugfeed);
            continue;
        }

        if (i % 2)
        {
            SDD_SI_WRITE8 (0xa0000217, 0xff);
        } else {
            SDD_SI_WRITE8 (0xa0000217, 0x00);
        }
    }
    
    return 0;
}
    
int
sdd_si_start_fan_dog (SDD_SI_DATA_T * pData)
{
    static int flag = 0;

    if (1 == flag)
    {
        return 0;
    }
    flag = 1;
    g_sddpData = pData;
    
    taskSpawn ("sddfandog", 200, 0, 2048*10, \
        (FUNCPTR) sdd_fan_dog, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    printf ("\r\nsdd_si_start_fan_dog: task sdd_fan_dog created");


    taskSpawn ("sddfanfeeddog", 200, 0, 2048*10, \
        (FUNCPTR) sdd_fan_feed_dog, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    printf ("\r\nsdd_si_start_fan_dog: task sdd_fan_feed_dog created");


    return 0;
}


STATUS
sdd_SI_SetLedState (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea, LED_TYPE_T led, UINT16 id, LED_STATE_T state)
{
    if (pData == NULL ||
        (pData->head.state[idArea] != SDD_AREA_STATE_INIT &&
         pData->head.state[idArea] != SDD_AREA_STATE_ACTIVE) || (state != LED_STATE_ON && state != LED_STATE_OFF && state != LED_STATE_BLINK))
    {
        if (NULL != pData){
            printf ("get the current pData->head.state[idArea]:%d\r\n", pData->head.state[idArea]);
        }
        return PB_ERROR;
    }

    if (SDD_AREA_INDEX_A0 == idArea)
    {
        switch (led)
        {
        case LED_TYPE_FAULTY:
            pData->led_faulty = state;
            break;
        case LED_TYPE_CRITICAL:
            pData->led_critical = state;
            break;
        case LED_TYPE_MAJOR:
            pData->led_major = state;
            break;
        case LED_TYPE_MINOR:
            pData->led_minor = state;
            break;
        default:
            return PB_ERROR;
        }
    }

    return PB_OK;
}

STATUS
sdd_SI_GetLedState (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea, LED_TYPE_T led, UINT16 id, LED_STATE_T * pState)
{
    if (pData == NULL || (pData->head.state[idArea] != SDD_AREA_STATE_INIT && pData->head.state[idArea] != SDD_AREA_STATE_ACTIVE))
    {
        return PB_ERROR;
    }

    if (SDD_AREA_INDEX_A0 == idArea)
    {
        switch (led)
        {
        case LED_TYPE_FAULTY:
            *pState = pData->led_faulty;
            break;
        case LED_TYPE_CRITICAL:
            *pState = pData->led_critical;
            break;
        case LED_TYPE_MAJOR:
            *pState = pData->led_major;
            break;
        case LED_TYPE_MINOR:
            *pState = pData->led_minor;
            break;
        default:
            return PB_ERROR;
        }
    }
    return PB_OK;
}

int g_rtcdebug = 0;

STATUS
sdd_SI_GetRtcTime (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea, struct tm * current)
{
    RTC_DATE_T pDate;

    /*verify the mytm */
    if (pData == NULL || current == NULL)
    {
        if (g_rtcdebug) printf ("\r\nsdd_SI_GetRtcTime: paramet NULL");
        return PB_ERR_PARAM;
    }

    memset (&pDate, 0, sizeof (RTC_DATE_T));

    if (g_locksef)
    semTake(gSDDTaskPollSemID, WAIT_FOREVER);

    /*read date from M41T11,and transfer it to the 'time_t' data structure */
    if (sdd_LockI2cBus (WAIT_FOREVER) == PB_OK)
    {
        sdd_si_SwitchI2cBus (DEVID_SI_M41T11);
        if (SDD_DEV_SUCCESS != sdd_GetM41T11Date (I2CDEV_SI_M41T11, &pDate))
        {
            if (g_rtcdebug) printf ("\r\nsdd_SI_GetRtcTime: sdd_GetM41T11Date failed");
            SDD_DBG (("SDD_RtcTimeGet sdd_SetM41T11Date fail\r\n"), SDD_DBG_I2C);
            sdd_UnlockI2cBus ();
            if (g_locksef)
            semGive (gSDDTaskPollSemID);
            return PB_ERR_NAPPL;
        }
        sdd_UnlockI2cBus ();
    }
    else
    {
        if (g_rtcdebug) printf ("\r\nsdd_SI_GetRtcTime: sdd_LockI2cBus  failed");
        SDD_DBG (("SDD_SI_GetRtcTime lock i2cbus failed!\r\n"), SDD_DBG_I2C);
    }
    if (g_locksef)
    semGive (gSDDTaskPollSemID);
    
    current->tm_sec = pDate.second;
    current->tm_min = pDate.minute;
    current->tm_hour = pDate.hour;
    current->tm_mday = pDate.day;
    current->tm_wday = pDate.wday;
    current->tm_mon = pDate.month;
    current->tm_year = pDate.year + 100;

    return PB_OK;
}

STATUS
sdd_SI_SetRtcTime (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea, struct tm * current)
{
    RTC_DATE_T pDate;
    int result = PB_OK;

    if (pData == NULL)
    {
        printf ("\r\nsdd_SI_SetRtcTime : pData NULL");
        return PB_ERR_PARAM;
    }

    pDate.second = current->tm_sec;
    pDate.minute = current->tm_min;
    pDate.hour = current->tm_hour;
    pDate.day = current->tm_mday;
    pDate.wday = current->tm_wday;
    pDate.month = current->tm_mon;
    pDate.year = current->tm_year;

    /*set the datetime */
    if (g_locksef)
    semTake (gSDDTaskPollSemID, WAIT_FOREVER);

    if (sdd_LockI2cBus (WAIT_FOREVER) == PB_OK)
    {
/*
Please refer to documentation:
"
Project	00:	hiT7065
Unit	30:	SI
Category	20:	HW Architecture Document
File Name	01:	S42024-L5607-A1-01-76D8.doc
section 5.6 "i2c structure" --- at the end ...
"



        Index   Device Type Function Description    I2C Address Memory Size
        1.      PCA9544 (Upper) I2C multiplexer 0xE0    256 x 8bit
        2.      PCA9544 (Lower) I2C multiplexer 0xE2    256 x 8bit
        3.      SEEPROM Serial EEPROM   0xA8    1024 x 8bit
        4.      98DX107 SEEPROM Serial EEPROM   0xA0    65536x8bit
        5.      LM75_1  Digital temperature sensor  0x92    256 x 8bit
        6.      LM75_2  Digital temperature sensor  0x94    256 x 8bit
        7.      RTC Timer keeper SRAM   0xD0    256 x 8bit


There is no differece between 7065 and 7080.        
*/        
        sdd_si_SwitchI2cBus (DEVID_SI_M41T11);
        if (SDD_DEV_SUCCESS != sdd_SetM41T11Date (I2CDEV_SI_M41T11, &pDate))
        {
            SDD_DBG (("SDD_RtcTimeSet sdd_SetM41T11Date fail\r\n"), SDD_DBG_I2C);
            printf ("\r\nsdd_SI_SetRtcTime : setm41t11date failed ");
            sdd_UnlockI2cBus ();
            result = PB_ERR_NAPPL;
        }
        sdd_UnlockI2cBus ();
    }
    else
    {
        printf ("\r\nsdd_SI_SetRtcTime switch i2cbus failed");
        SDD_DBG (("SDD_SI_SetRtcTime lock i2cbus failed!\r\n"), SDD_DBG_I2C);
    }
    if (g_locksef)
    semGive (gSDDTaskPollSemID);
    return result;
}

STATUS
sdd_SI_SetAlarmOut (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea, UINT16 id, BOOL bEnable)
{
    if (pData == NULL ||
        (pData->head.state[idArea] != SDD_AREA_STATE_INIT && pData->head.state[idArea] != SDD_AREA_STATE_ACTIVE) || id > SDD_SI_ALARMOUT_NUM)
    {
        return PB_ERROR;
    }

    if (id < 1 || id > SDD_SI_ALARMOUT_NUM) return PB_ERROR;
    pData->alarm_out[id - 1] = bEnable;

    return PB_OK;
}

STATUS
sdd_SI_SetMdo (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea, UINT16 id, BOOL bEnable)
{
    if (pData == NULL ||
        (pData->head.state[idArea] != SDD_AREA_STATE_INIT && pData->head.state[idArea] != SDD_AREA_STATE_ACTIVE) || id > SDD_SI_MDO_NUM)
    {
        return PB_ERROR;
    }
    if (id < 1 || id > SDD_SI_MDO_NUM) return PB_ERROR;
    
    pData->mdo[id - 1] = bEnable;
    return PB_OK;
}

STATUS
sdd_SI_GetMdi (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea, UINT16 id, BOOL * pState)
{
    if (pData == NULL || pState == NULL || pData->head.state[idArea] != SDD_AREA_STATE_ACTIVE || pData->head.flagDevFail || id > SDD_SI_MDI_NUM)
    {
        return PB_ERROR;
    }

    if (id < 1 || id > SDD_SI_MDI_NUM) return PB_ERROR;
    
    *pState = pData->mdi[id - 1];
    
    return PB_OK;
}

STATUS
sdd_SI_SetBoardTempThreshold (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea, INT16 tempHigh, INT16 tempLow)
{
    if (pData == NULL || (pData->head.state[idArea] != SDD_AREA_STATE_INIT && pData->head.state[idArea] != SDD_AREA_STATE_ACTIVE))
    {
        return PB_ERROR;
    }
    SDD_DBG (("SDD Set SI Temp threshold! high:%d low:%d\r\n", tempHigh, tempLow), SDD_DBG_TEMP);
    if (tempLow > tempHigh)
    {
        /* do not accept */
        return PB_ERROR;
    }
    else
    {
        pData->threshold_temp_high = tempHigh;
        pData->threshold_temp_low = tempLow;
    }

    return PB_OK;
}





STATUS
sdd_SI_GetBoardTemperature (SDD_SI_DATA_T * pData, SDD_AREA_INDEX_T idArea, INT16 * pTemper)
{
    sdd_ASSERT (SDD_AREA_INDEX_A0 == idArea);
    if (pData == NULL ||
        pTemper == NULL ||
        pData->head.state[idArea] != SDD_AREA_STATE_ACTIVE || pData->head.flagDevFail || pData->temperature[0] == SDD_INVALID_TEMPERATURE)
    {
        if (NULL != pData) {
            printf ("\r\n125_getboardtemp:pdata(%8p), ptemper(%8p), state(%d)", 
                pData, pTemper, pData->head.state[idArea]);
        }
        
        return PB_ERROR;
    }

    *pTemper = pData->temperature[0];
    SDD_DBG (("sdd_SI_GetBoardTemp value:%d\r\n", pData->temperature[0]), SDD_DBG_TEMP);
    //printf("sdd_SI_GetBoardTemp value:%d\r\n",pData->temperature[0]);
    return PB_OK;
}



void
sdd_SI_DisplayData (SCREEN_T * session, SDD_SI_DATA_T * pData)
{

}


/*
 * $Id: sdd_si.c,v 1.1 2009/04/17 22:53:37 wydeng Exp $
 * $Log: sdd_si.c,v $
 * Revision 1.1  2009/04/17 22:53:37  wydeng
 * sample c source file
 *
 * Revision 1.14  2009/03/23 18:17:05  wydeng
 * -i "7080 R4.0_151"
 * memProbe () in sdd_fan_feed_dog ();
 *
 * Revision 1.13  2009/03/18 18:10:43  wydeng
 * -i "7080 R4.0_151"
 * klocwork
 *
 * Revision 1.12  2009/03/10 10:03:39  wydeng
 * -i "7080 R4.0_151"
 * add polling interval
 *
 * Revision 1.11  2009/03/09 18:27:58  wydeng
 * -i "7080 R4.0_151"
 * add polling interval
 *
 * Revision 1.10  2009/03/05 19:25:39  wydeng
 * -i "7080 R4.0_151"
 * sdd_read_fan ()
 *
 * Revision 1.9  2009/03/02 18:01:49  wydeng
 * -i "7080 R4.0_151"
 * report alarm to FMA
 *
 * Revision 1.8  2009/03/02 15:05:58  wydeng
 * -i "7080 R4.0_151"
 * software feed dog program
 *
 * Revision 1.7  2009/03/01 16:09:49  wydeng
 * -i "7080 R4.0_151"
 * FAN!!!
 *
 * Revision 1.6  2009/02/23 10:35:32  wydeng
 * -i "7080 R4.0_151"
 * remove debug info dull
 *
 * Revision 1.5  2009/02/20 10:12:46  wydeng
 * -i "7080 R4.0_151"
 * disable sdd_read_fan error temproraly
 *
 * Revision 1.4  2009/02/19 18:27:40  wydeng
 * -i "7080 R4.0_151"
 * add card status check before polling
 *
 * Revision 1.3  2009/01/19 16:33:07  wydeng
 * -i "7080 R4.0_151"
 * debug info, sdd_si_SwitchI2cBus ()
 *
 * Revision 1.2  2009/01/12 16:00:06  wbchen
 * -i "7080 R4.0_43" modify code for 7065
 *
 * Revision 1.12.18.18  2009/01/10 20:42:47  wydeng
 * -i "7080 R4.0_151"
 * remove power related code form si code
 *
 * Revision 1.12.18.17  2009/01/07 20:17:23  wydeng
 * -i "7080 R4.0_151"
 * write code for new temperature sensor in SI card
 *
 * Revision 1.12.18.16  2009/01/07 19:54:49  wydeng
 * -i "7080 R4.0_151"
 * write code for new temperature sensor in SI card
 *
 * Revision 1.12.18.15  2009/01/06 21:46:22  wydeng
 * -i "7080 R4.0_151"
 * adjust for setting dac5571 fan controller
 *
 * Revision 1.12.18.14  2009/01/06 21:39:37  wydeng
 * -i "7080 R4.0_151"
 * adjust for setting dac5571 fan controller
 *
 * Revision 1.12.18.13  2009/01/06 21:07:58  wydeng
 * -i "7080 R4.0_151"
 * adjust for setting dac5571 fan controller
 *
 * Revision 1.12.18.12  2009/01/05 14:54:44  wydeng
 * -i "7080 R4.0_151"
 * g_sihb
 *
 * Revision 1.12.18.11  2008/12/30 14:49:42  wydeng
 * -i "7080 R4.0_151"
 * redefinition to SDD_SI_SLOT_INDEX_FAN1, etc ...
 *
 * Revision 1.12.18.10  2008/12/23 20:24:43  wydeng
 * -i "7080 R4.0_151"
 * add sdd_SetDAC5571FanSpeed ();
 *
 * Revision 1.12.18.9  2008/12/22 20:41:15  wydeng
 * -i "7080 R4.0_151"
 * add comments;
 *
 * Revision 1.12.18.8  2008/12/22 20:33:32  wydeng
 * -i "7080 R4.0_151"
 * modify definition of sdd_GetMAX6639RPM()
 *
 * Revision 1.12.18.7  2008/12/22 20:19:03  wydeng
 * -i "7080 R4.0_151"
 * add function sdd_read_fan ();
 *
 * Revision 1.12.18.6  2008/12/20 17:04:51  wydeng
 * -i "7080 R4.0_151"
 * address confirm;
 *
 * Revision 1.12.18.5  2008/12/20 15:19:50  wydeng
 * -i "7080 R4.0_151"
 * writing sdd_si.c code for fan and powers;
 *
 * Revision 1.12.18.4  2008/12/19 11:50:21  wydeng
 * -i "7080 R4.0_151"
 * add comments for address definition;
 *
 * Revision 1.12.18.3  2008/12/19 11:43:46  wydeng
 * -i "7080 R4.0_151"
 * add comments for address definition;
 *
 * Revision 1.12.18.2  2008/12/17 16:39:51  wydeng
 * -i "7080 R4.0_151"
 * add comments for address definition;
 *
 * Revision 1.12.18.1  2008/12/17 14:24:53  wydeng
 * -i "7080 R4.0_151"
 * add comments for address definition;
 *
 * Revision 1.12  2007/11/13 14:25:00  jlgong
 * -i "7080 R4.0_115"
 * plug in/out CC cards, the status of e-sfp is error.
 *
 * Revision 1.11  2007/08/14 14:26:54  zzhang
 * -i"7080_PB001177-01"
 * when si was deativated, light on all led on it.
 *
 * Revision 1.10  2007/08/03 15:58:27  zzhang
 * -i"7080_PB000870-02"
 * get SI temperature from fusion.
 *
 * Revision 1.9  2007/07/19 17:14:27  zzhang
 * -i"7080_PB001177-01"
 * all three leds on si should bink during NE reboot
 *
 * Revision 1.8  2007/05/08 10:34:24  zzhang
 * -i"7080 R4.0_37"
 * Support GE eletronic SFP
 *
 * Revision 1.7  2007/04/03 01:47:42  zzhang
 * -i"7080 R4.0_37"
 * Modify for SC1+1
 *
 * Revision 1.6  2007/02/12 05:23:37  zzhang
 * -i"7080 R4.0_37"
 * Add port mode interface & oa .
 *
 * Revision 1.5  2007/02/05 05:11:17  zzhang
 * -i"7080 R4.0_37"
 * Change RTC interface.
 *
 * Revision 1.4  2007/02/05 02:47:15  zzhang
 * -i"7080 R4.0_37"
 * Change RTC interface.
 *
 * Revision 1.3  2007/02/01 11:43:34  zzhang
 * -i"7080 R4.0_37"
 * ADD SI card.
 **/
