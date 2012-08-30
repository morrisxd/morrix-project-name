The demo is suited to run on:
WDDS version 4.2.2
Board WDS3 rev B2
WinMon version 4.2 release 3 built Apr 26 2011
Last checked on date: 02/02/2012

This package contains the following files:

1. mlppp_ppp_tdm.c             - the application main file
2. mlppp_ppp_tdm.h             - header file which has the structures, macros, globals and prototypes defined/declared
3. mlppp_ppp_statistics.c      - file containing the functions to print various statistics
4. mlppp_ppp_util.c            - file with many utility functions
5. makefile                    - makefile for test compilation
6. readme.txt                  - this readme file

For compilation instructions refer to the attached application note

Compilation instructions:

Unzip the application and go to mlppp_ppp directory.
a) Open the makefile given with the test examples and change following parameters.

       WDDI_DIR : give path of the current wddi directory 

b) enter the following make command in WinMon prompt. 
   make TEST= mlppp_ppp_tdm.c 
   The executable mlppp_ppp_tdm.exe to be downloaded can be found in mlppp_ppp_tdm/bin directory. 