The demo is suited to run on:
WDDS version 4.3.1
Board WDS3 rev B2
WinMon version 4.2 release 5
Last checked on date: 31/08/2012


This package contains the following files:

1. provider_bridge.c        - the application main file
2. provider_bridge.h        - header file which has the structures, macros, globals and prototypes defined/declared
3. provider_bridge_menu.c   - file containing the functions for CLI
4. provider_bridge_menu.h   - header file which has the definitions for CLI
5. provider_bridge_util.c   - file containing the functions of utilities
6. provider_bridge_uitl.h   - header file which has the definitions of utilities
7. wt_partition_ecc_util.c  - holds the ram paritition operation functions
8. wt_partition_ecc_util.h  - header file which hold the definition of structure and functions declaration
9. os_timer.c               - timer operation functions for aging feature
10. makefile                - makefile for test compilation
11. readme.txt              - this readme file

Compilation instructions:
1. Unzip
2. Open the makefile given with the test examples and change following parameters.
       WDDI_DIR : give path of the current wddi directory 
3. Compile 
   make TEST=provider_bridge.c
4. Download the compiled executable (in /bin directory) to the board using tftp.