The demo is suited to run on:
WDDS version 4.2.2
Board WDS3 rev B2
WinMon version 4.2 release 5
Last checked on date: 09/02/2012


This package contains the following files:

1. atm_ima_ufe.c               - the application main file
2. atm_ima_ufe.h               - header file which has the structures, macros, globals and prototypes defined/declared
3. atm_ima_ufe_stats.c         - file containing the functions to print various statistics
4. atm_ima_ufe_util.c          - file with many utility functions
5. ufe_utility.h               - file containing declaration of ufe related functions of the demo
6. ufe_utility.c               - file containing implementation of alarm and pm related functions of the demo
7. flexmux_util.c              - file containing implementation of framer related functions of the demo
8. flexmux_util.h              - file containing declaration of framer related functions of the demo
9. makefile                    - makefile for test compilation
10. readme.txt                 - this readme file

Compilation instructions:
1. Unzip
2. Open the makefile given with the test examples and change following parameters.
       WDDI_DIR : give path of the current wddi directory 
3. Compile 
   make TEST=atm_ima_ufe.c
4. Download the compiled executable (in /bin directory) to the board using tftp.