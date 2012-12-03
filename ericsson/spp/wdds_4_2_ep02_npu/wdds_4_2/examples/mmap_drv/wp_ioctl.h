#ifndef __XS_IOCTL_H
#define __XS_IOCTL_H

#define XS_IOCTL_OS_VALUE(x) (0xAF + x)

#define XS_IOCTL_CONSOLE_WRITE           XS_IOCTL_OS_VALUE(7)
#define XS_IOCTL_CONSOLE_READ            XS_IOCTL_OS_VALUE(8)
#define XS_IOCTL_THERMO_READ             XS_IOCTL_OS_VALUE(9)
#define XS_IOCTL_RESET                  XS_IOCTL_OS_VALUE(10)
#define XS_IOCTL_FW_VERSION             XS_IOCTL_OS_VALUE(11)
#define XS_IOCTL_I2C_PROG               XS_IOCTL_OS_VALUE(12)
#define XS_IOCTL_FLASH_PROG             XS_IOCTL_OS_VALUE(13)
#endif // __XS_IOCTL_H
