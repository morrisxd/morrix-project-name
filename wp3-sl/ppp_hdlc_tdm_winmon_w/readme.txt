The demo is suited to run on:
WDDS version 4.2.2
Board WDS3 rev B2
WinMon version 4.2 release 5
Last checked on date: 14/02/2012


This package contains the following files:

1. ppp_hdlc_tdm.c        - the application main file
2. ppp_hdlc_tdm.h        - header file which has the structures, macros, globals and prototypes defined/declared
3. ppp_hdlc_tdm_stats.c  - file containing the functions to print various statistics
4. ppp_hdlc_tdm_util.c   - file with many utility functions
5. makefile              - makefile for test compilation
6. readme.txt            - this readme file

Compilation instructions:
1. Unzip
2. Open the makefile given with the test examples and change following parameters.
       WDDI_DIR : give path of the current wddi directory 
3. Compile 
   make TEST=ppp_hdlc_tdm.c
4. Download the compiled executable (in /bin directory) to the board using tftp.


