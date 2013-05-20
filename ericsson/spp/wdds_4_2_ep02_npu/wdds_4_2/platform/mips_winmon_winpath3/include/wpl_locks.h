#ifndef _WPL_LOCKS_H__
#define _WPL_LOCKS_H__


/***************************/
/* WPL_Lock/Unlock defines */
/***************************/

#define WPL_TA_SEMAPHORE_RESERVED_BSP      0
#define WPL_TA_SEMAPHORE_RESERVED_WINIPC_1 1
#define WPL_TA_SEMAPHORE_RESERVED_WINIPC_2 2
#define WPL_TA_SEMAPHORE_RESERVED_WINIPC_3 3
#define WPL_TA_SEMAPHORE_RESERVED_WDDI     4
#define WPL_TA_SEMAPHORE_RESERVED_FRAMER   5
#define WPL_TA_SEMAPHORE_UNUSED_2          6
#define WPL_TA_SEMAPHORE_UNUSED_3          7

#define WPL_HW_LOCK 1
#define WPL_SW_LOCK 0

#define WPL_SHARED_LOCK  1
#define WPL_PRIVATE_LOCK 0

/* Under WP3 wddi uses HW lock #4 */
#define WPL_WDDI_LOCK_KEY            WPL_LOCK_KEY_CREATE(WPL_HW_LOCK,      \
                                                         WPL_PRIVATE_LOCK, \
                                                         WPL_TA_SEMAPHORE_RESERVED_WDDI, 0)

/* Services and extensions must use SW keys for (base + id) to work */
#define WPL_SERVICE_LOCK_KEY_BASE    WPL_LOCK_KEY_CREATE(WPL_SW_LOCK,      \
                                                         WPL_PRIVATE_LOCK, \
                                                         0, 78714)
#define WPL_EXTENSION_LOCK_KEY_BASE  WPL_LOCK_KEY_CREATE(WPL_SW_LOCK,      \
                                                         WPL_PRIVATE_LOCK, \
                                                         0, 78734)

WP_U32 WPL_SemInit(WP_SEM_ID *sem_id);
WP_U32 WPL_SemIncrement(WP_SEM_ID *sem_id,WP_U32 incrementCount);
WP_U32 WPL_SemDecrement(WP_SEM_ID *sem_id, WP_U32 decrementCount);
WP_U32 WPL_SemDestroy(WP_SEM_ID *sem_id);


#endif /*_WPL_LOCKS_H__*/
