# $Id: makefile,v 1.5 2007/11/21 08:30:04 eranka Exp $ */
#****************************************************************************
#* (C) Copyright Wintegra 2001, All rights reserved.
#* WINTEGRA CONFIDENTIAL PROPRIETARY
#* Contains Confidential Proprietary information of Wintegra, Inc.
#* Reverse engineering is prohibited.
#* The copyright notice does not imply publication.
#*****************************************************************************
#*****************************************************************************
#*
#* Module: $RCSfile: makefile,v $
#* Purpose:
#*     <add purpose lines here>
#*
#* Module Originator:  (Login Name: 'george') 
#* Creation Date: Wed Feb 14 10:21:49 2001
#*
#* Change History:
#* +------------+--------------------+----------------------------------------
#* | Date       | By                 | Description
#* +------------+--------------------+----------------------------------------
#* |            |                    |
#* +------------+--------------------+----------------------------------------
#* |            |                    |
#* +------------+--------------------+----------------------------------------
#*
#*****************************************************************************



SOURCES = \
	general_function.dpl \
	iw_buff_chain.dpl \
	sw_time_stamp.dpl \
	cfu_idle.dpl \
        millisecond_timer.dpl 	\
	token_thread.dpl \
	tgu_sgmii_an.dpl \
	tgu_an_restart.dpl



LCMD = -c genfunc.link
#XCFLAGS +=

all: comp

eef: general_function.eef

include ../include/makefile.inc

comp: $(OBJ)

general_function.eef: $(OBJ)
	${WINLNK} ${OBJ} -o $@ ${LFLAGS}
