Trace Buffer

From: Jeff Zhang 
Sent: Thursday, June 07, 2012 23:09
To: Heller Zheng; Morris Deng (Morris_Deng@pmc-sierra.com); Jasse Liu; Jerry Cui
Subject: How to check trace buffer by winsim
Importance: High

 

1. Go to /public/data/projects/winpath/tools/versions/toolchain/110/linux/winsim, copy a winsim to your local folder. Version 110 is good for WP3.

 

2. you need the eef file, go to dps/sources/build/config, copy the eef file. For example: backhaul_winpath3b.cfg.eef

 

3. you need the dump of trace buffer. You can use the trace buffer dumped from bus error or you use winutil tb -dump to get one. Copy the trace buffer dumped from 1st line to last line to a file, eg. Tb.log

 

4. run winsim wp3 +log

 

5. run ld backhaul_winpath3b.cfg.eef

 

6. run tra load tb.log

 

7. run tra disasm    ---- from this step you will get a winsim.txt

 

8. open winsim.txt (WinSim.log)

 

9. from the dps bus error dump you know which TRS has the bus error. Go the last entry of this TRS in winsim.txt

 

10. you will find there is no terminate for this thread because it has a bus error. From the winsim.txt  you see the execution process of each thread. Check the thread you care about and see what is the execution procedure.

 

11. you cant know the value of registers by winsim.txt only if you use _trace_high, or _trace_low to save the value of register in dps code.

 

Thanks,

Jeff



