#ifndef __XS_IOCTL_H
#define __XS_IOCTL_H

#define XS_IOCTL_OS_VALUE(x) (0xAF + x)

#define MMAP_IOCTL_SUPPRESS_DUP_SIGNAL   XS_IOCTL_OS_VALUE(1)
#define MMAP_IOCTL_ENABLE_BUS_ERR_SIG    XS_IOCTL_OS_VALUE(3)
#define MMAP_IOCTL_KERNEL_BUS_ERR_REPORT XS_IOCTL_OS_VALUE(4)
#define MMAP_IOCTL_REPLACE_BUS_ERR_KERNEL_REPORT_WITH_SIGNAL XS_IOCTL_OS_VALUE(5)
#define MMAP_IOCTL_READ_BOARD_GEN_CONF   XS_IOCTL_OS_VALUE(6)

#define XS_IOCTL_CONSOLE_WRITE           XS_IOCTL_OS_VALUE(7)
#define XS_IOCTL_CONSOLE_READ            XS_IOCTL_OS_VALUE(8)
#define XS_IOCTL_THERMO_READ             XS_IOCTL_OS_VALUE(9)
#define XS_IOCTL_RESET                   XS_IOCTL_OS_VALUE(10)
#define XS_IOCTL_FW_VERSION              XS_IOCTL_OS_VALUE(11)
#define XS_IOCTL_I2C_PROG                XS_IOCTL_OS_VALUE(12)
#define XS_IOCTL_FLASH_PROG              XS_IOCTL_OS_VALUE(13)
#define XS_IOCTL_ENV_GET                 XS_IOCTL_OS_VALUE(14)
#define XS_IOCTL_REV_GET                 XS_IOCTL_OS_VALUE(15)
#define XS_IOCTL_ENV_GET32               XS_IOCTL_OS_VALUE(16) // Do not use from application
#define XS_IOCTL_READ_WORD               XS_IOCTL_OS_VALUE(17)
#define  XS_IOCTL_I2C_READ               XS_IOCTL_OS_VALUE(18)
#define PCI_CONSOLE_SIZE   256

typedef struct {
    unsigned char buf[PCI_CONSOLE_SIZE];
    int bufLen;
} XS_CONSOLE_BUF_DESC;

typedef struct {
    unsigned char *buf;
    unsigned int   len;
} XS_FILE_DESC;

typedef struct {
    int  devNo;
    unsigned int thermNo;
    unsigned int reg;
    unsigned int value;
} XS_THERMO_DESC;

typedef struct {
    unsigned int Fuse0;
    unsigned int Fuse1;
} XS_REV_DESC;

#endif // __XS_IOCTL_H
