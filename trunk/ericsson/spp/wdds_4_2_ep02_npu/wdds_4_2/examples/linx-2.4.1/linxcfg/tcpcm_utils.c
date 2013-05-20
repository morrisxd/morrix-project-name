/*
 * Copyright (c) 2006-2007, Enea Software AB
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * Neither the name of Enea Software AB nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * File: tcpcm_utils.c
 */
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include <tcpcm_db_ioctl.h>
#include "tcpcm_utils.h"

#define ptr(p, n) ((unsigned char *)(p) + (n))

static int check_arg(struct linx_con_arg_tcp *arg)
{
        if ((arg->name == NULL) || (*arg->name == '\0'))
                return EINVAL;
        else if ((arg->ipaddr == NULL) || (*arg->ipaddr == '\0'))
                return EINVAL;
        else
                return 0;
}

static size_t sizeof_arg(struct linx_con_arg_tcp *arg)
{
        size_t size;

        size = 0;
        if (arg->name != NULL)
                size += strlen(arg->name);
        size += 1;
        if (arg->ipaddr != NULL)
                size += strlen(arg->ipaddr);
        size += 1;
        return size;
}

int mk_tcpcm_ioctl_create(struct linx_con_arg_tcp *arg, void **v, size_t *sizeof_v)
{
        struct tcpcm_ioctl_create *p;
        size_t size;
        char *feat;

        if (check_arg(arg) != 0)
                return EINVAL;

        size = sizeof(*p) + sizeof_arg(arg);
        p = calloc(1, size);
        if (p == NULL)
                return ENOMEM;

        p->name = sizeof(*p);
        p->name_len = strlen(arg->name);
        memcpy(ptr(p, p->name), arg->name, p->name_len + 1);

	/* not used since linx 2.2 */
        p->remote_addr = 0;
        p->remote_addr_len = 0;

        feat = (arg->features != NULL) ? arg->features : "";
        p->feat = p->remote_addr + p->remote_addr_len + 1;
        p->feat_len = strlen(feat);
        memcpy(ptr(p, p->feat), feat, p->feat_len + 1);
        
	p->use_nagle = arg->use_nagle;
	p->live_tmo = arg->live_tmo;

	/* new since linx 2.2 */
	p->ip_addr = inet_addr(arg->ipaddr);
	p->pad = 0;

        *v = p;
        *sizeof_v = size;
        return 0;
}
