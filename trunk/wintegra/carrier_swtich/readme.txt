The demo is suited to run on:
WDDS version 5.0 GCA
Board WDS4 
WinMon version 5.1.3
Last checked on date: 02/01/2014


This package contains the following files:

1. carrier_switch.c                - the application main file
2. carrier_switch.h                - header file which has the structures, macros, globals and prototypes defined/declared
3. carrier_switch_menu.c           - file containing the functions for CLI
4. carrier_switch_menu.h           - header file which has the definitions for CLI
5. carrier_switch_api.c            - file containing the functions of main APIs
6. carrier_switch_util.c           - file containing the functions of utilities
7. carrier_switch_util.h           - header file which has the definitions of utilities
8. wt_partition_ecc_util.c  - holds the ram paritition operation functions
9. wt_partition_ecc_util.h  - header file which hold the definition of structure and functions declaration
10. os_timer.c              - timer operation functions for aging feature
11. makefile                - makefile for test compilation
12. readme.txt              - this readme file

Compilation instructions:
1. Unzip
2. Open the makefile given with the test examples and change following parameters.
       WDDI_DIR : give path of the current wddi directory 
3. Compile 
   make TEST=carrier_switch.c
4. Download the compiled executable (in /bin directory) to the board using tftp.

