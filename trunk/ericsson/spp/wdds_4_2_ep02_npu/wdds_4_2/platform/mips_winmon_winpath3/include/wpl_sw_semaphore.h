#ifndef _SW_SEMAPHORE_H__
#define _SW_SEMAPHORE_H__

#define WAIT_FOREVER 0
#define NO_WAIT      1

#define SEM_SUCCESS            0x0
#define SEM_BUSY_ERROR         0x1
#define SEM_WAIT_PARAM_ERROR   0x200

#define SZLONG_LOG 5
#define SZLONG_MASK 31UL
int wpv_swsem_acquire(int *sem, int wait);
int wpv_swsem_release(int *sem);


#endif /*_SW_SEMAPHORE_H__*/
