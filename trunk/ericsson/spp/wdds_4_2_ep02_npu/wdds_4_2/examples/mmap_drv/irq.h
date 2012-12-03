#ifndef __ASM_MACH_MIPS_IRQ_H
#define __ASM_MACH_MIPS_IRQ_H

//#define NR_IRQS	128

#define MIPS_CPU_IRQ_BASE  0
#define MIPS_CPU_IRQ_WINPATH  2
#define WINPATH_IRQ_BASE   8

#define IV_BERR1_VEC      0        /* SIU1  (Parameter) bus error */
#define IV_BERR2_VEC      1        /* SIU2  (Packet) bus error */
#define IV_BERR3_VEC      2        /* SIU3  (Global) bus error */
#define IV_BERR3I_VEC     3        /* SIU3i (RIF) bus error */
#define IV_BERR4D_VEC     4        /* SIU4D (Host) bus error */
#define IV_BERR4X_VEC     5        /* SIU4X (ASRAM) bus error */

#define IV_CAM_VEC        6
#define IV_PCE_ACCESS_VEC 7
#define IV_PCE_HASH_VEC   8
#define IV_POLICER_VEC    9

#define IV_BERR_SRIO0_VEC 10
#define IV_BERR_SRIO1_VEC 11
#define IV_BERR_PCIE_VEC  12

#define IV_WFM_DBG1_VEC   13
#define IV_WFM_DBG2_VEC   14
#define IV_WFM_DBG3_VEC   15
#define IV_WFM_DBG4_VEC   16

#define IV_SA_TRS_VEC     17

#define IV_SRIO0_INFO_VEC 24 
#define IV_SRIO1_INFO_VEC 25 
#define IV_PCIE_LINK_VEC  26 
#define IV_PCIE_EXT_VEC   27 

#define IV_EINT1_VEC      28
#define IV_EINT2_VEC      29
#define IV_EINT3_VEC      30
#define IV_EINT4_VEC      31

#define IV_SRIO0_DOOR_VEC 32 
#define IV_SRIO1_DOOR_VEC 33 
#define IV_PCE_PAR_VEC    34
#define IV_SW0_VEC        35

#define IV_SW1_VEC        36
#define IV_SRIO0_MSG_VEC  37 
#define IV_SRIO1_MSG_VEC  38 
#define IV_BRG9_VEC       39

#define IV_BRG10_VEC      40
#define IV_ENET_HOST      41
#define IV_UART_VEC       42
#define IV_IKE_VEC        43

#define IV_SERDES1_VEC    44
#define IV_SERDES2_VEC    45
#define IV_WMMIQO0_VEC    46
#define IV_WMMIQO1_VEC    47

#define IV_WMMIQO2_VEC    48
#define IV_WMMIQO3_VEC    49
#define IV_SW2_VEC        50
#define IV_SW3_VEC        51

#define IV_WMMIQ0_VEC     52
#define IV_WMMIQ1_VEC     53
#define IV_WMMIQ2_VEC     54
#define IV_WMMIQ3_VEC     55

#define IV_WDT0I_VEC      56
#define IV_WDT1I_VEC      57

#define IV_WINPATH_BASE   0
#define IV_WINPATH_MAX    63
#define IV_NULL_VEC       64

#define IV_WIN_IPC_VEC    IV_SW0_VEC          
#define IV_SW_HI_VEC      IV_SW1_VEC    
#define IV_KILL_VEC       IV_SW2_VEC      
#define IV_SW_LO_VEC      IV_SW3_VEC     

/* BUS errors */
#define IV_BERR_PARAM_BUS_VEC  (IV_BERR1_VEC)
#define IV_BERR_PACKET_BUS_VEC (IV_BERR2_VEC)
#define IV_BERR_GLOBAL_BUS_VEC (IV_BERR3_VEC)
#define IV_BERR_RIF_VEC        (IV_BERR3I_VEC)
#define IV_BERR_HOST_BUS_VEC   (IV_BERR4D_VEC)

#define SYS_IRQ_HI 0x030C07FB
#define SYS_IRQ_LO 0xFF001C3F

#define RESERVED_BITS_HI 0xFC000000
#define RESERVED_BITS_LO 0x00FC0000

#define INT_LO_REG_MASK ~(RESERVED_BITS_LO)
#define INT_HI_REG_MASK ~(RESERVED_BITS_HI)

#define SER_IRQ_HI ~(SYS_IRQ_HI | RESERVED_BITS_HI)
#define SER_IRQ_LO ~(SYS_IRQ_LO | RESERVED_BITS_LO)

#define WINPATH_SER_SERVICE_MASK_HI 0x00F00000
//#define WINPATH_SER_SERVICE_MASK_LO 0x0003E3C0

#define WINPATH_SER_OVERFLOW_MASK_HI 0x0003C000
//#define WINPATH_SER_OVERFLOW_MASK_LO 0x00000000

#define IV_SERIAL_QO    IV_WMMIQO3_VEC
#define IV_SERIAL_QS    IV_WMMIQ3_VEC
#define IV_WINK_QO      IV_WMMIQO2_VEC
#define IV_WINK_QS      IV_WMMIQ2_VEC

#define WINPATH_SYS_BUSERR_MASK     (1 << IV_BERR1_VEC| \
                                     1 << IV_BERR2_VEC| \
                                     1 << IV_BERR3_VEC| \
                                     1 << IV_BERR3I_VEC| \
                                     1 << IV_BERR4D_VEC| \
                                     1 << IV_BERR4X_VEC)



#endif /* __ASM_MACH_MIPS_IRQ_H */
