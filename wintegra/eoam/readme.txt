The demo is suited to run on:
WDDS version 4.3.1
Board WDS3 rev B2
WinMon version 4.2 release 5
Last checked on date: 31/08/2012


This package contains the following files:

1. enet_oam.c        - the application main file
2. enet_oam.h        - header file which has the structures, macros, globals and prototypes defined/declared
3. enet_oam_menu.c   - file containing the functions for CLI
4. enet_oam_menu.h   - header file which has the definitions for CLI
5. enet_oam_util.c   - file containing the functions of utilities
6. enet_oam_uitl.h   - header file which has the definitions of utilities
7. makefile          - makefile for test compilation
8. readme.txt        - this readme file

Compilation instructions:
1. Unzip
2. Open the makefile given with the test examples and change following parameters.
       WDDI_DIR : give path of the current wddi directory 
3. Compile 
   make TEST=enet_oam.c
4. Download the compiled executable (in /bin directory) to the board using tftp.