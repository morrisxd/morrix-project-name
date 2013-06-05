The demo is suited to run on:
WDDS version 4.2.1 ver 1537
Board WDS3 rev B2
WinMon version 4.2 release 4 built Aug 10 2011
Last checked on date: 07/12/2011
Accompanies : TBD


This package contains the following files:

1. ppp_hdlc_ufe.c        - the application main file
2. ppp_hdlc_ufe.h        - header file which has the structures, macros, globals and prototypes defined/declared
3. ppp_hdlc_ufe_stats.c  - file containing the functions to print various statistics
4. ppp_hdlc_ufe_util.c   - file with many utility functions
5. ufe_utility.c         - file containing implementation of ufe related functions of the demo
6. ufe_utility.h         - file containing declaration of ufe related functions of the demo
7. flexmux_util.c        - file containing implementation of framer related functions of the demo
8. flexmux_util.h        - file containing declaration of framer related functions of the demo
9. makefile              - makefile for test compilation
10. readme.txt            - this readme file

Compilation instructions:

Unzip the application and go to ppp_hdlc_ufe directory.
a) Open the makefile given with the test examples and change following parameters.

       WDDI_DIR : give path of the current wddi directory 

b) enter the following make command in WinMon prompt. 
   make TEST=ppp_hdlc_ufe.c 
   The executable ppp_hdlc_ufe.exe to be downloaded can be found in ppp_hdlc_ufe/bin directory. 

 



