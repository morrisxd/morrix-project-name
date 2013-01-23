The demo is suited to run on:
WDDS version 4.2.2 ver b1574
Board WDS3 rev B2
WinMon version 4.2.4 build Aug 10, 2011
Last checked on date: 01/03/2012


This package contains the following files:

1. bp_cbfc.c            - the application main file
2. bp_cbfc.h            - header file which has the structures, macros, globals and prototypes defined/declared
3. bp_cbfc_stats.c      - file containing the functions to print various statistics
4. bp_cbfc_util.c       - file with many utility functions
5. makefile             - makefile for test compilation
6. readme.txt           - this readme file

Compilation instructions:

Unzip the application and go to bp_cbfc directory.
a) Open the makefile given with the test examples and change following parameters.

       WDDI_DIR : give path of the current wddi directory 

b) enter the following make command in WinMon prompt. 
   make TEST= bp_cbfc.c 
   The executable bp_cbfc.exe to be downloaded can be found in bp_cbfc/bin directory. 

 



