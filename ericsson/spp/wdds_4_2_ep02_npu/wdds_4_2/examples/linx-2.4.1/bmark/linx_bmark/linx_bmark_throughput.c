/*
 * Copyright (C) 2006-2009 by Enea Software AB.
 * All rights reserved.
 *
 * This Example is furnished under a Software License Agreement and
 * may be used only in accordance with the terms of such agreement.
 * No title to and ownership of the Example is hereby transferred.
 *
 * The information in this Example is subject to change
 * without notice and should not be construed as a commitment
 * by Enea Software AB.
 *
 * DISCLAIMER
 * This Example is delivered "AS IS", consequently 
 * Enea Software AB makes no representations or warranties, 
 * expressed or implied, for the Example. 
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

#include <linx.h>
#include <linx_types.h>
#include <linx_ioctl.h>
#include <linx_socket.h>

#include "linx_bmark.h"

/****************************************/

union LINX_SIGNAL {
	LINX_SIGSELECT sigNo;
};

/****************************************/

static void
test_client(LINX * linx,
	    size_t msg_size, int cnt, LINX_SPID slave, int use_linx_api)
{
	int i;
	union LINX_SIGNAL *sig;
	static const LINX_SIGSELECT any_sig[] = { 0 };
	static char buf[65536];	/* buffer used when not using linx api. */

	if (use_linx_api) {

		sig = linx_alloc(linx, msg_size, ECHO_SIG);
		for (i = 0; i < cnt; i++) {
			linx_send(linx, &sig, slave);
			linx_receive(linx, &sig, any_sig);
		}
		linx_free_buf(linx, &sig);

	} else {

		int len;
		struct sockaddr_linx sockaddr;
		socklen_t socklen;
		int sd = linx_get_descriptor(linx);

		sig = (void *)buf;
		socklen = sizeof(struct sockaddr_linx);
		sockaddr.family = AF_LINX;
		sockaddr.spid = slave;

		sig->sigNo = ECHO_SIG;
		for (i = 0; likely(i < cnt); i++) {
			len = sendto(sd, sig, msg_size,
				     0, (struct sockaddr *)&sockaddr, socklen);
			if (unlikely((size_t) len != msg_size || len <= 0)) {
				ERR("Failed to send echo signal. "
				    "sendto returned: %d when asked "
				    "for: %Zd", len, msg_size);
			}

			len = recvfrom(sd,
				       sig,
				       msg_size,
				       0, (struct sockaddr *)&sockaddr,
				       &socklen);

			if (unlikely(len <= 0)) {
				ERR("Failed to receive a echo signal"
				    "(%d, %d)\n", len, errno);
			}
		}
	}
}

int linx_bmark_throughput(const char *path, int cnt,
			  size_t msg_size, LINX_SPID server,
			  int use_linx_api, int tp_instances)
{
	int i;
	struct timeval tv;
	long clk = 0;
	static unsigned int starttimes[PROC_STAT];
	static unsigned int stoptimes[PROC_STAT];
	pid_t pid;

	printf("\n%%%% Running LINX throughput bmark %s linx api %%%%\n",
	       use_linx_api ? "with" : "without");

	printf("Test parameters:\n");
	printf("  Hunt path    : %s\n", path);
	printf("  Loop count   : %d\n", cnt);
	printf("  Message size : %Zd bytes\n", msg_size);
	printf("  Instances    : %u\n", tp_instances);

	fflush(stdout);
	memcpy(starttimes, get_cpu_times(), sizeof(starttimes));
	get_time(&tv);
	for (i = 0; i < tp_instances; i++) {
		pid = fork();
		if (pid < 0)
			ERR("server failed to fork. pid %d", pid);
		if (!pid) {
			LINX_SPID test_slave;
			LINX *linx = linx_open("client", 0, NULL);
			test_slave = create_test_slave(linx, server,
						       use_linx_api);
			test_client(linx, msg_size, cnt, test_slave,
				    use_linx_api);
			destroy_test_slave(linx, test_slave);
			exit(0);
		}
	}

	for (i = 0; i < tp_instances; i++) {
		wait(NULL);
	}

	clk = get_time(&tv);
	memcpy(stoptimes, get_cpu_times(), sizeof(stoptimes));

	/* Measure the time it takes to create the test processes without
	 * doing any test and remove it from the clk time (this oh shall
	 * not be part of the measurement.)
	 */
	get_time(&tv);
	for (i = 0; i < tp_instances; i++) {
		pid = fork();
		if (pid < 0)
			ERR("LINX_bmark_troughput failed to perform second"
			    "fork. pid %d", pid);
		if (!pid) {
			LINX_SPID test_slave;
			LINX *linx = linx_open("client", 0, NULL);
			test_slave = create_test_slave(linx, server,
						       use_linx_api);
			destroy_test_slave(linx, test_slave);
			exit(0);
		}
	}
	for (i = 0; i < tp_instances; i++) {
		wait(NULL);
	}
	clk -= get_time(&tv);

	printf("LINX troughput bmark completed.\n");
	printf("Results:\n");
	{
		long long bits = (long long)msg_size;
		bits *= tp_instances;
		bits *= cnt;
		bits *= 2;
		bits *= 8;

		printf(": Benchmark time %ld us\n", clk);
		printf("  Sent bits   : %lld Bits/direction\n", bits / 2);
		printf("  Performance : %lld Mbps/direction\n",
		       bits / 2 / (long long)clk);
		print_cpu_load(starttimes, stoptimes);
	}
	return 0;
}
