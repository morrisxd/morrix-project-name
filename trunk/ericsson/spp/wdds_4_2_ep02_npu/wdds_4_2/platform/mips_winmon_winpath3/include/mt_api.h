/*****************************************************************************
 * (C) Copyright Wintegra 2010, All rights reserved.
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 * Contains Confidential Proprietary information of Wintegra, Inc.
 * Reverse engineering is prohibited.
 * The copyright notice does not imply publication.
 *****************************************************************************/
/*****************************************************************************
 *
 * Module:  Multithrading Library
 * Purpose:
 *     API for multithreading library
 *
 * Module Originator: Yaron Abershitz
 * Creation Date: January 4, 2010
 *
 *
 *****************************************************************************/

#ifndef __MT_API_H__
#define __MT_API_H__

#ifndef _SIZE_T_
#define _SIZE_T_        __typeof(sizeof(WP_U32))
#endif

#ifndef size_t
#define size_t WP_U32
#endif

typedef int win_pthread_t;
typedef struct{
      unsigned int stacksize;
      unsigned int stackaddr;
}win_pthread_attr_t;


void resume_all(void);

int suspend_all(void);
int enable_multithreading(void);

int win_pthread_create(win_pthread_t *thread,
                       const win_pthread_attr_t *attr,
                       void *(*start_routine)(void*), void *arg);

void win_pthread_exit(void *value_ptr);

win_pthread_t win_pthread_self(void);

int pthread_attr_init(win_pthread_attr_t *attr);

int pthread_attr_destroy(win_pthread_attr_t *attr);

int pthread_attr_getstackaddr(const win_pthread_attr_t *attr, void **addr);

int pthread_attr_getstacksize(const win_pthread_attr_t *attr, size_t *size);

int pthread_attr_setstackaddress(win_pthread_attr_t *attr, void *addr);

int pthread_attr_setstacksize(win_pthread_attr_t *attr, size_t size);
 


#endif /* __MT_API_H__ */


/*****************************************************************************
 * WINTEGRA CONFIDENTIAL PROPRIETARY
 *****************************************************************************/

