# 1 "cah_126e1.c"
 














# 1 "../../../appl/CAH/inc/cah_defs.h" 1
 
















# 1 "../../../appl/CTD/inc/ctd_types.h" 1
 
























# 1 "../../../infra/SYS/target/h/types/vxTypesOld.h" 1
 

 

 





















































 




















# 1 "../../../infra/SYS/target/h/sys/types.h" 1
 

 
 








 

























# 1 "../../../infra/SYS/target/h/types/vxTypes.h" 1
 

 

 












 













































# 1 "../../../infra/SYS/target/h/types/vxANSI.h" 1
 

 

 








 


















 










# 1 "../../../infra/SYS/target/h/types/vxCpu.h" 1
 

 

 








 





























































 













 

 










 






























































































# 208 "../../../infra/SYS/target/h/types/vxCpu.h"



















































# 267 "../../../infra/SYS/target/h/types/vxCpu.h"


# 282 "../../../infra/SYS/target/h/types/vxCpu.h"















# 311 "../../../infra/SYS/target/h/types/vxCpu.h"


# 322 "../../../infra/SYS/target/h/types/vxCpu.h"




























 


















# 44 "../../../infra/SYS/target/h/types/vxANSI.h" 2

# 1 "../../../infra/SYS/target/h/types/vxArch.h" 1
 

 

 



























 




































































# 1 "../../../infra/SYS/target/h/arch/ppc/archPpc.h" 1
 

 

 































	
 



 


 

















 






 
































 






typedef	unsigned int	_RType;		 







































# 1 "../../../infra/SYS/target/h/arch/ppc/ppc603.h" 1
 

 

 
























 









 



















 
































 










 














 








 



 

 










# 150 "../../../infra/SYS/target/h/arch/ppc/archPpc.h" 2















# 1 "../../../infra/SYS/target/h/arch/ppc/toolPpc.h" 1
 

 
 




































 

 





































   











# 532 "../../../infra/SYS/target/h/arch/ppc/toolPpc.h"




# 554 "../../../infra/SYS/target/h/arch/ppc/toolPpc.h"










# 165 "../../../infra/SYS/target/h/arch/ppc/archPpc.h" 2







# 102 "../../../infra/SYS/target/h/types/vxArch.h" 2































 















 










# 45 "../../../infra/SYS/target/h/types/vxANSI.h" 2

# 1 "../../../infra/SYS/target/h/types/vxParams.h" 1
 

 

 










 












































# 46 "../../../infra/SYS/target/h/types/vxANSI.h" 2

# 1 "../../../infra/SYS/target/h/types/vxTypesBase.h" 1
 

 

 



















 
















# 1 "../../../infra/SYS/host/gnu2.96/libinc/stddef.h" 1






 







 

 




 


 





 


# 61 "../../../infra/SYS/host/gnu2.96/libinc/stddef.h"


 





 


















 





 

 





















typedef int ptrdiff_t;









 




 

 


































typedef unsigned int size_t;






















 




 





























 


























typedef short unsigned int wchar_t;
























typedef unsigned int  wint_t;




 

 

# 317 "../../../infra/SYS/host/gnu2.96/libinc/stddef.h"




 













 







# 42 "../../../infra/SYS/target/h/types/vxTypesBase.h" 2


 






struct __timer;































































 






 




















 










































































 






















































































































 






 































# 47 "../../../infra/SYS/target/h/types/vxANSI.h" 2







# 64 "../../../infra/SYS/target/h/types/vxTypes.h" 2



typedef int ssize_t ;




typedef unsigned long time_t ;




typedef long fpos_t ;




typedef signed char int8_t ;




typedef unsigned char uint8_t ;




typedef short int16_t ;




typedef unsigned short uint16_t ;




typedef long int32_t ;




typedef unsigned long uint32_t ;




typedef long long int64_t ;




typedef unsigned long long uint64_t ;



 

typedef unsigned char	uchar_t;
typedef unsigned short	ushort_t;
typedef unsigned int	uint_t;
typedef unsigned long	ulong_t;

typedef	struct	_quad { long val[2]; } quad;
typedef	long	daddr_t;
typedef	char *	caddr_t;
typedef	char *	addr_t;
typedef	long	swblk_t;

 

typedef short		dev_t;
typedef unsigned short	gid_t;
typedef	unsigned long	ino_t;
typedef int		mode_t;
typedef unsigned long	nlink_t;
typedef long		off_t;
typedef int		pid_t;
typedef unsigned short	uid_t;






# 39 "../../../infra/SYS/target/h/sys/types.h" 2







# 80 "../../../infra/SYS/target/h/types/vxTypesOld.h" 2


 

typedef	char		INT8;
typedef	short		INT16;
typedef	int		INT32;
typedef	long long	INT64;

typedef	unsigned char	UINT8;
typedef	unsigned short	UINT16;
typedef	unsigned int	UINT32;
typedef	unsigned long long UINT64;

typedef	unsigned char	UCHAR;
typedef unsigned short	USHORT;
typedef	unsigned int	UINT;
typedef unsigned long	ULONG;

typedef	int		BOOL;
typedef	int		STATUS;
typedef int 		ARGINT;

typedef void		VOID;







typedef int 		(*FUNCPTR) ();	    
typedef void 		(*VOIDFUNCPTR) ();  
typedef double 		(*DBLFUNCPTR) ();   
typedef float 		(*FLTFUNCPTR) ();   



 




typedef union
    {
    long pm_int;
    void *pm_v;
    const void *pm_cv;
    char *pm_c;
    unsigned char *pm_uc;

    signed char *pm_sc;
    const char *pm_cc;
    const unsigned char *pm_cuc;
    const signed char *pm_csc;
    short *pm_s;
    ushort_t *pm_us;
    const short *pm_cs;
    const ushort_t *pm_cus;
    int *pm_i;
    uint_t *pm_ui;
    const int *pm_ci;
    const uint_t *pm_cui;
    long *pm_l;
    ulong_t *pm_ul;
    const long *pm_cl;
    const ulong_t *pm_cul;

    int8_t *pm_i8;
    uint8_t *pm_ui8;
    const int8_t *pm_ci8;
    const uint8_t *pm_cui8;
    int16_t *pm_i16;
    uint16_t *pm_ui16;
    const int16_t *pm_ci16;
    const uint16_t *pm_cui16;
    int32_t *pm_i32;
    uint32_t *pm_ui32;
    const int32_t *pm_ci32;
    const uint32_t *pm_cui32;








    } pointer_mix_t;













 





 














typedef long	fd_mask;





typedef	struct fd_set
    {
    fd_mask	fds_bits[((unsigned int)((( 2048  )+((  (sizeof(fd_mask) * 8 )  )-1)))/(unsigned int)(  (sizeof(fd_mask) * 8 )  )) ];
    } fd_set;







 

typedef	unsigned char	u_char;
typedef	unsigned short	u_short;
typedef	unsigned int	u_int;
typedef	unsigned long	u_long;
typedef	unsigned short	ushort;


 

typedef char		TBOOL;		 


 
























typedef unsigned long INSTR;             
















 

# 292 "../../../infra/SYS/target/h/types/vxTypesOld.h"












# 26 "../../../appl/CTD/inc/ctd_types.h" 2



typedef unsigned int HANDLE;


 



typedef unsigned char UINT1;

typedef char INT1;

typedef unsigned short UINT2;

typedef short INT2;

typedef unsigned long UINT4;

typedef long INT4;

typedef unsigned short BOOLEAN;



# 80 "../../../appl/CTD/inc/ctd_types.h"



 





typedef enum 
{
    DEV_NOT_INIT,
    DEV_FAIL,
    DEV_OK,
    DEV_MAX
}DEVICE_STATUS_Tval;





 
typedef UINT32 INSTANCE_T; 

 

 














































typedef enum
{
    SLOT_NUMBER_NONE    = 0,
    SLOT_NUMBER_LC1     = 1,
    SLOT_NUMBER_LC2     = 2,
    SLOT_NUMBER_LC3     = 3,
    SLOT_NUMBER_LC4     = 4,
    SLOT_NUMBER_LC5     = 5,
    SLOT_NUMBER_LC6     = 6,
    SLOT_NUMBER_HOCC1   = 7,
    SLOT_NUMBER_HOCC2   = 8,
    SLOT_NUMBER_SFC1    = 9,
    SLOT_NUMBER_SFC2    = 10,
    SLOT_NUMBER_CE1     = 11,
    SLOT_NUMBER_CE2     = 12,
    SLOT_NUMBER_CE3     = 13,
    SLOT_NUMBER_CE4     = 14,
    SLOT_NUMBER_CE5     = 15,
    SLOT_NUMBER_CE6     = 16,
    SLOT_NUMBER_SC1     = 17,
    SLOT_NUMBER_SC2     = 18,  
    SLOT_NUMBER_SI      = 19,
    SLOT_NUMBER_SFM1    = 20,
    SLOT_NUMBER_SFM2    = 21,
    SLOT_NUMBER_IO1     = 22,
    SLOT_NUMBER_IO2     = 23,
    SLOT_NUMBER_IO3     = 24,
    SLOT_NUMBER_IO4     = 25,
    SLOT_NUMBER_IO5     = 26,
    SLOT_NUMBER_IO6     = 27,
    SLOT_NUMBER_IO7     = 28,
    SLOT_NUMBER_IO8     = 29,
    SLOT_NUMBER_IO9     = 30,
    SLOT_NUMBER_IO10    = 31,
    SLOT_NUMBER_IO11    = 32,
    SLOT_NUMBER_IO12    = 33,
    SLOT_NUMBER_IO13    = 34,
    SLOT_NUMBER_IO14    = 35,
    SLOT_NUMBER_IO15    = 36,
    SLOT_NUMBER_PWR1    = 37,
    SLOT_NUMBER_PWR2    = 38,
    SLOT_NUMBER_FAN1    = 39,
    SLOT_NUMBER_FAN2    = 40,

    SLOT_NUMBER_MAX     = 40
}SLOT_NUMBER_T;

 




 




 




 




 




typedef enum 
{
    CARDTYPE_UNKNOWN = 0,
    
     
    CARDTYPE_SI,              
    CARDTYPE_SC,              
    CARDTYPE_HOCC,            
    CARDTYPE_LOCC,            
    CARDTYPE_LOCC_PMC,        
    CARDTYPE_LOCC_FPGA,       
    CARDTYPE_1_STM256,        
    CARDTYPE_2_STM64,         
     

    CARDTYPE_1_STM64,         
    CARDTYPE_8_STM16,         
    CARDTYPE_4_STM16,         
    CARDTYPE_2_STM16,         
    CARDTYPE_8_STM4,          
    CARDTYPE_8_STM1O,         
    CARDTYPE_8_STM1E_FUNC,    
    CARDTYPE_8_STM1E_CONN,    
    CARDTYPE_2_STM64M,        
    CARDTYPE_8_STM16M,        
    CARDTYPE_16_STM4M,        
    CARDTYPE_16_STM1EM,       
    CARDTYPE_16_STM1E_CONN,   
    CARDTYPE_12_E3DS3_FUNC,   
    CARDTYPE_12_E3DS3_CONN,   
    CARDTYPE_126_E1_FUNC,     
    CARDTYPE_126_E1_CONN,     
    CARDTYPE_13DBM_OA,        
    CARDTYPE_15DBM_OA,        
    CARDTYPE_18DBM_OA,        
    CARDTYPE_20DB_OA,         
    CARDTYPE_2_OTU,           
    CARDTYPE_1_OTU2,          
    
    CARDTYPE_SDH,             
    CARDTYPE_PDH,             
    CARDTYPE_CC,              
    CARDTYPE_DATA,            
    CARDTYPE_DATA_L2,         
    CARDTYPE_RPR,             
    CARDTYPE_OA,              
    CARDTYPE_OTU,             
    CARDTYPE_MAGUS,           
    
    CARDTYPE_FAN,             
    CARDTYPE_PWR,             
    
    CARD_TYPE_MAX
} CARDTYPE_T;

typedef UINT32 ENTITY_T;

typedef enum
{
    ENTITY_STATE_EMPTY  = 0,
    ENTITY_STATE_INIT   = 1,
    ENTITY_STATE_ACTIVE = 2
} ENTITY_STATE_T;

typedef struct
{
    ENTITY_T   entity;
    BOOL       bPresent;
    CARDTYPE_T cardType;
}ENTITY_INFO_T;

typedef int BOARD_OPTION_T;

 



 











 




 
























 

























































































# 18 "../../../appl/CAH/inc/cah_defs.h" 2

# 1 "../../../appl/CTD/inc/ctd_error.h" 1
 

























typedef enum 
{
    PB_OK = 0,                       
    PB_ERROR,                
    PB_ERR_PARAM,            
    PB_ERR_NAPPL,            
    PB_ERR_NOTIMP,           
    PB_ERR_CONFLICT,         
    PB_ERR_TIMEOUT,          
    PB_ERR_TOOBIG,           
    PB_ERR_TOOMUCH,          
    PB_ERR_WRONGCALL,        
    PB_ERR_VERSION,          
    PB_ERR_NOSUCH,           
    PB_ERR_HARDWARE,         
    PB_ERR_MEMORY,           
    PB_ERR_BUSY,             
    PB_ERR_DRIVER_ERROR,     
    PB_ERR_HANDLE,           
    PB_ERR_BLOCK,            
    PB_ERR_STATE,            
    PB_ERR_COMM,             
    PB_ERR_MAX
} PB_ERROR_CODE_T;

 

 
static __inline const char *CTD_StrError(int code)
{
    switch (code)
    {
        case PB_OK:                    
            return "No error";
            
        case PB_ERROR:                
            return "Unknown Error";
            
        case PB_ERR_PARAM:            
            return "Incorrect Parameter";
            
        case PB_ERR_NAPPL:            
            return "Not applicable";
            
        case PB_ERR_NOTIMP:            
            return "Not Implemented";
            
        case PB_ERR_CONFLICT:        
            return "Conflict to Exist Element";
            
        case PB_ERR_TIMEOUT:        
            return "Operation Timeout";
            
        case PB_ERR_TOOBIG:            
            return "Too Big";
            
        case PB_ERR_TOOMUCH:        
            return "Too Many Elements";
            
        case PB_ERR_WRONGCALL:        
            return "Incorrect Call";
            
        case PB_ERR_VERSION:        
            return "version Mismatch";
            
        case PB_ERR_NOSUCH:            
            return "No Such Element";
            
        case PB_ERR_HARDWARE:        
            return "Hardware Error";
            
        case PB_ERR_MEMORY:            
            return "Out of Memory";
            
        case PB_ERR_BUSY:            
            return "Busy";
            
        case PB_ERR_DRIVER_ERROR:    
            return "Driver Error";
            
        case PB_ERR_HANDLE:            
            return "Invalid Handle";
            
        case PB_ERR_BLOCK:            
            return "Blocked";
            
        case PB_ERR_STATE:            
            return "Invalid state/status";
            
        case PB_ERR_COMM:            
            return "Communication error";
            
        default: 
            return "Unknown error";
    }
}







 















# 19 "../../../appl/CAH/inc/cah_defs.h" 2

# 1 "../../../appl/CTD/inc/ctd_sdh.h" 1
 






















# 1 "../../../appl/CTD/inc/ctd_if.h" 1
 























# 1 "../../../infra/MMG/inc/MIB_access.h" 1
 















































# 1 "../../../infra/SYS/target/h/semLib.h" 1
 

 

 




































# 1 "../../../infra/SYS/target/h/vxWorks.h" 1
 

 

 























































# 71 "../../../infra/SYS/target/h/vxWorks.h"



















 




 




 





 




 





 









 







 






 




























 

























 












 





 

# 1 "../../../infra/SYS/target/h/tool/gnu/toolMacros.h" 1
 

 

 


















 









 












# 218 "../../../infra/SYS/target/h/vxWorks.h" 2




 


















# 42 "../../../infra/SYS/target/h/semLib.h" 2

# 1 "../../../infra/SYS/target/h/vwModNum.h" 1
 

 

 

































































































 












































































































































 










# 43 "../../../infra/SYS/target/h/semLib.h" 2


 






 











 

typedef enum		 
    {
    SEM_EMPTY,			 
    SEM_FULL			 
    } SEM_B_STATE;

typedef struct semaphore *SEM_ID;

 



extern STATUS 	semGive (SEM_ID semId);
extern STATUS 	semTake (SEM_ID semId, int timeout);
extern STATUS 	semFlush (SEM_ID semId);
extern STATUS 	semDelete (SEM_ID semId);
extern int 	semInfo (SEM_ID semId, int idList[], int maxTasks);
extern STATUS 	semBLibInit (void);
extern SEM_ID 	semBCreate (int options, SEM_B_STATE initialState);
extern STATUS 	semCLibInit (void);
extern SEM_ID 	semCCreate (int options, int initialCount);
extern STATUS 	semMLibInit (void);
extern SEM_ID 	semMCreate (int options);
extern STATUS   semMGiveForce (SEM_ID semId);
extern STATUS 	semOLibInit (void);
extern SEM_ID 	semCreate (void);
extern void 	semShowInit (void);
extern STATUS 	semShow (SEM_ID semId, int level);

# 114 "../../../infra/SYS/target/h/semLib.h"








# 49 "../../../infra/MMG/inc/MIB_access.h" 2





 



 
 




















 



 





 










 



 
 
    typedef struct {
        unsigned int flags[4];   
    } MIB_Flags_t;


 
    typedef struct {
        int oid_length;
        int oid_data[30 ];
    } OIDValue;

 
    typedef struct {
        int index;               
        int oid_length;
        int oid_data[30 ];    
    } MIBDid_t;
    typedef MIBDid_t *MIBDID;    

 
    typedef struct {
        unsigned short year;
        unsigned char month;
        unsigned char day;
        unsigned char hour;
        unsigned char min;
        unsigned char sec;
        unsigned char dec_sec;
        unsigned char dir_utc;
        unsigned char hrs_utc;
        unsigned char min_utc;
        unsigned char padd;      
    } DateTime;


 



 





















 





















 
    void MibRegisterCallback(MIBDID did, int (*fptr) (MIBDID, void *, int, void *));
    void MibRegisterCallbackCheck(MIBDID did, int (*fptr_check) (MIBDID, void *, int, void *));
    void MibRegisterCallbackPartial(MIBDID did, int (*fptr_partial) (MIBDID, void *, int, void *), MIB_Flags_t);

 
    void MibUnRegisterCallback(MIBDID did, int (*fptr) (MIBDID, void *, int, void *));
    void MibUnRegisterCallbackCheck(MIBDID did, int (*fptr_check) (MIBDID, void *, int, void *));
    void MibUnRegisterCallbackPartial(MIBDID did, int (*fptr_partial) (MIBDID, void *, int, void *));

 
    int MibLoadAFileByDid(MIBDID did);

 
    int MibLoadTablesSequence(MIBDID * list, int num_entries);

 
    void MibTableChangedFlag(MIBDID did);
    void MibTblGrpChanged(MIBDID did, void *row, int type);
	void MibTblGrpChangedMirrored(MIBDID did, void *row, int type, int saveFlag);

 
    int MibGroupRegisterSema(MIBDID did, SEM_ID sema);

 
    int MibGroupRegisterRemote(MIBDID did, int (*fptr) (MIBDID did, void *row, int type, MIB_Flags_t flags));

 
    int MibRegisterSlotFunc(MIBDID did, int (*fptr) (MIBDID did, void *row), int distributed, int phys_log);

 
 
    int MibSetRemoteIntegerData(MIBDID did, int value);
    int MibGetRemoteIntegerData(MIBDID did);

    int MibSetRemoteChildData(MIBDID did, int oid, int value);
    int MibGetRemoteChildData(MIBDID did, int oid);

 
    int MibAccessHandle(int type, MIBDID did, void *row, int row_len, MIB_Flags_t flags, int slot);
    int MibAccessHandle_NCB(int type, MIBDID did, void *row, int row_len, MIB_Flags_t flags, int slot);
 
void setMibIdApplNo(int mibIdBef,  int applNumberBef, int type);
void clrMibIdApplNo(int mibIdAft, int applNumberAft,   int type);

int MibFlagsCheck(MIB_Flags_t flags_setpartial, MIB_Flags_t flags_callback);

int MibSetCleanFlagToFlash(int TLVtype, int delFlag);

 
int MibGetActiveScSlotNumber(int *pSlotNumber);

    extern MIB_Flags_t mibDefFlags;





# 25 "../../../appl/CTD/inc/ctd_if.h" 2

# 1 "../../../mib/inc/mib/MIB_enums.h" 1



typedef enum {
	PbApplState_initial = 0,
	PbApplState_ready = 1,
	PbApplState_active = 2,
	PbApplState_standby = 3,
	PbApplState_failed = 4,
	PbApplState_diagnostic = 5
} PbApplState_Tval;

typedef enum {
	PbSNCPSwitchReason_clearcmd = 1,
	PbSNCPSwitchReason_lpcmd = 2,
	PbSNCPSwitchReason_forcedswitch = 3,
	PbSNCPSwitchReason_manualswitch = 4,
	PbSNCPSwitchReason_signalfail = 5,
	PbSNCPSwitchReason_signalfailclear = 6,
	PbSNCPSwitchReason_noswitch = 7,
	PbSNCPSwitchReason_signaldeg = 8,
	PbSNCPSwitchReason_signalnormal = 9
} PbSNCPSwitchReason_Tval;

typedef enum {
	PbTMSSquelchMode_clear = 0,
	PbTMSSquelchMode_forceAIS = 1,
	PbTMSSquelchMode_forceDNU = 2,
	PbTMSSquelchMode_forceShutdown = 3
} PbTMSSquelchMode_Tval;

typedef enum {
	EventCategory_management = 1,
	EventCategory_software = 2,
	EventCategory_hardware = 3,
	EventCategory_indeterminate = 4
} EventCategory_Tval;

typedef enum {
	PbXcFrom_cli = 1,
	PbXcFrom_ems = 2,
	PbXcFrom_snm = 3,
	PbXcFrom_gmpls = 4,
	PbXcFrom_rpr = 5,
	PbXcFrom_sncp = 6
} PbXcFrom_Tval;

typedef enum {
	RprTpNumberType_hex11110000 = 286326784,
	RprTpNumberType_hex11120000 = 286392320,
	RprTpNumberType_hex11130000 = 286457856,
	RprTpNumberType_hex11140000 = 286523392,
	RprTpNumberType_hex11210000 = 287375360,
	RprTpNumberType_hex11220000 = 287440896,
	RprTpNumberType_hex11230000 = 287506432,
	RprTpNumberType_hex11240000 = 287571968,
	RprTpNumberType_hex11310000 = 288423936,
	RprTpNumberType_hex11320000 = 288489472,
	RprTpNumberType_hex11330000 = 288555008,
	RprTpNumberType_hex11340000 = 288620544,
	RprTpNumberType_hex11410000 = 289472512,
	RprTpNumberType_hex11420000 = 289538048,
	RprTpNumberType_hex11430000 = 289603584,
	RprTpNumberType_hex11440000 = 289669120
} RprTpNumberType_Tval;

typedef enum {
	RprSvcFailReason_noerr = 0,
	RprSvcFailReason_flowlimit = 1,
	RprSvcFailReason_cannotadd = 2,
	RprSvcFailReason_cannotdel = 3,
	RprSvcFailReason_cannotreach = 4,
	RprSvcFailReason_bandwidth = 5,
	RprSvcFailReason_svctypeconflict = 6,
	RprSvcFailReason_macconflict = 7,
	RprSvcFailReason_cosconflict = 8,
	RprSvcFailReason_autoringenconflict = 9,
	RprSvcFailReason_ringselconflict = 10,
	RprSvcFailReason_wrapenconflict = 11,
	RprSvcFailReason_svcmodeconflict = 12,
	RprSvcFailReason_ttlmodeconflict = 13,
	RprSvcFailReason_manttlvalueconflict = 14,
	RprSvcFailReason_activeconflict = 15,
	RprSvcFailReason_inactive = 16,
	RprSvcFailReason_other = 17
} RprSvcFailReason_Tval;

typedef enum {
	PbMsspAgentState_init = 0,
	PbMsspAgentState_ready = 1,
	PbMsspAgentState_suspend = 2
} PbMsspAgentState_Tval;

typedef enum {
	PbPmReportResp_noerr = 0,
	PbPmReportResp_resource = 1,
	PbPmReportResp_invalid = 2,
	PbPmReportResp_conflict = 3
} PbPmReportResp_Tval;

typedef enum {
	PbMsspLineRequest_signalDegradeRing = 8,
	PbMsspLineRequest_signalFailRing = 11,
	PbMsspLineRequest_clear = 21
} PbMsspLineRequest_Tval;

typedef enum {
	PbEthInterfaceType_lan = 1,
	PbEthInterfaceType_wan = 2,
	PbEthInterfaceType_trunk = 3
} PbEthInterfaceType_Tval;

typedef enum {
	PbTMSSetSa_sa4 = 4,
	PbTMSSetSa_sa5 = 5,
	PbTMSSetSa_sa6 = 6,
	PbTMSSetSa_sa7 = 7,
	PbTMSSetSa_sa8 = 8
} PbTMSSetSa_Tval;

typedef enum {
	RprSpan_east = 1,
	RprSpan_west = 2
} RprSpan_Tval;

typedef enum {
	PbLedColor_off = 0,
	PbLedColor_green = 1,
	PbLedColor_yellow = 2,
	PbLedColor_red = 3,
	PbLedColor_orange = 4,
	PbLedColor_blinkingGreen = 5,
	PbLedColor_blinkingYellow = 6,
	PbLedColor_blinkingRed = 7,
	PbLedColor_blinkingOrange = 8
} PbLedColor_Tval;

typedef enum {
	PbSdhHpSignalLabel_unequipped = 0,
	PbSdhHpSignalLabel_reserved = 1,
	PbSdhHpSignalLabel_tugstructure = 2,
	PbSdhHpSignalLabel_lockedtu = 3,
	PbSdhHpSignalLabel_asynchronousvc3 = 4,
	PbSdhHpSignalLabel_asynchronousvc4 = 18,
	PbSdhHpSignalLabel_atm = 19,
	PbSdhHpSignalLabel_mandqdb = 20,
	PbSdhHpSignalLabel_fddi = 21,
	PbSdhHpSignalLabel_hdlcppp = 22,
	PbSdhHpSignalLabel_sdlsss = 23,
	PbSdhHpSignalLabel_hdlclaps = 24,
	PbSdhHpSignalLabel_sdlsrs = 25,
	PbSdhHpSignalLabel_gig10 = 26,
	PbSdhHpSignalLabel_gfp = 27,
	PbSdhHpSignalLabel_test = 254,
	PbSdhHpSignalLabel_ais = 255
} PbSdhHpSignalLabel_Tval;

typedef enum {
	PbTMSQLMode_passThrough = 1,
	PbTMSQLMode_overWrite = 2
} PbTMSQLMode_Tval;

typedef enum {
	PbTMSSignalStatus_normal = 1,
	PbTMSSignalStatus_failed = 2,
	PbTMSSignalStatus_na = 3,
	PbTMSSignalStatus_holdoff = 4,
	PbTMSSignalStatus_waitToRestore = 5
} PbTMSSignalStatus_Tval;

typedef enum {
	AlarmSeverity_notInAlarm = 0,
	AlarmSeverity_warning = 1,
	AlarmSeverity_minor = 2,
	AlarmSeverity_major = 3,
	AlarmSeverity_critical = 4,
	AlarmSeverity_indeterminate = 5
} AlarmSeverity_Tval;

typedef enum {
	PbTpSegment_pm = 0,
	PbTpSegment_tcm1 = 1,
	PbTpSegment_tcm2 = 2,
	PbTpSegment_tcm3 = 3,
	PbTpSegment_tcm4 = 4,
	PbTpSegment_tcm5 = 5,
	PbTpSegment_tcm6 = 6,
	PbTpSegment_sm = 7,
	PbTpSegment_total = 8
} PbTpSegment_Tval;

typedef enum {
	PbRspType_failed = 1,
	PbRspType_okay = 2
} PbRspType_Tval;

typedef enum {
	PbCpgCardStatus_none = 0,
	PbCpgCardStatus_normal = 1,
	PbCpgCardStatus_fail = 2,
	PbCpgCardStatus_absent = 3
} PbCpgCardStatus_Tval;

typedef enum {
	PbOnOff_on = 1,
	PbOnOff_off = 2
} PbOnOff_Tval;

typedef enum {
	IANAifType_other = 1,
	IANAifType_regular1822 = 2,
	IANAifType_hdh1822 = 3,
	IANAifType_ddnX25 = 4,
	IANAifType_rfc877x25 = 5,
	IANAifType_ethernetCsmacd = 6,
	IANAifType_iso88023Csmacd = 7,
	IANAifType_iso88024TokenBus = 8,
	IANAifType_iso88025TokenRing = 9,
	IANAifType_iso88026Man = 10,
	IANAifType_starLan = 11,
	IANAifType_proteon10Mbit = 12,
	IANAifType_proteon80Mbit = 13,
	IANAifType_hyperchannel = 14,
	IANAifType_fddi = 15,
	IANAifType_lapb = 16,
	IANAifType_sdlc = 17,
	IANAifType_ds1 = 18,
	IANAifType_e1 = 19,
	IANAifType_basicISDN = 20,
	IANAifType_primaryISDN = 21,
	IANAifType_propPointToPointSerial = 22,
	IANAifType_ppp = 23,
	IANAifType_softwareLoopback = 24,
	IANAifType_eon = 25,
	IANAifType_ethernet3Mbit = 26,
	IANAifType_nsip = 27,
	IANAifType_slip = 28,
	IANAifType_ultra = 29,
	IANAifType_ds3 = 30,
	IANAifType_sip = 31,
	IANAifType_frameRelay = 32,
	IANAifType_rs232 = 33,
	IANAifType_para = 34,
	IANAifType_arcnet = 35,
	IANAifType_arcnetPlus = 36,
	IANAifType_atm = 37,
	IANAifType_miox25 = 38,
	IANAifType_sonet = 39,
	IANAifType_x25ple = 40,
	IANAifType_iso88022llc = 41,
	IANAifType_localTalk = 42,
	IANAifType_smdsDxi = 43,
	IANAifType_frameRelayService = 44,
	IANAifType_v35 = 45,
	IANAifType_hssi = 46,
	IANAifType_hippi = 47,
	IANAifType_modem = 48,
	IANAifType_aal5 = 49,
	IANAifType_sonetPath = 50,
	IANAifType_sonetVT = 51,
	IANAifType_smdsIcip = 52,
	IANAifType_propVirtual = 53,
	IANAifType_propMultiplexor = 54,
	IANAifType_ieee80212 = 55,
	IANAifType_fibreChannel = 56,
	IANAifType_hippiInterface = 57,
	IANAifType_frameRelayInterconnect = 58,
	IANAifType_aflane8023 = 59,
	IANAifType_aflane8025 = 60,
	IANAifType_cctEmul = 61,
	IANAifType_fastEther = 62,
	IANAifType_isdn = 63,
	IANAifType_v11 = 64,
	IANAifType_v36 = 65,
	IANAifType_g703at64k = 66,
	IANAifType_g703at2mb = 67,
	IANAifType_qllc = 68,
	IANAifType_fastEtherFX = 69,
	IANAifType_channel = 70,
	IANAifType_ieee80211 = 71,
	IANAifType_ibm370parChan = 72,
	IANAifType_escon = 73,
	IANAifType_dlsw = 74,
	IANAifType_isdns = 75,
	IANAifType_isdnu = 76,
	IANAifType_lapd = 77,
	IANAifType_ipSwitch = 78,
	IANAifType_rsrb = 79,
	IANAifType_atmLogical = 80,
	IANAifType_ds0 = 81,
	IANAifType_ds0Bundle = 82,
	IANAifType_bsc = 83,
	IANAifType_async = 84,
	IANAifType_cnr = 85,
	IANAifType_iso88025Dtr = 86,
	IANAifType_eplrs = 87,
	IANAifType_arap = 88,
	IANAifType_propCnls = 89,
	IANAifType_hostPad = 90,
	IANAifType_termPad = 91,
	IANAifType_frameRelayMPI = 92,
	IANAifType_x213 = 93,
	IANAifType_adsl = 94,
	IANAifType_radsl = 95,
	IANAifType_sdsl = 96,
	IANAifType_vdsl = 97,
	IANAifType_iso88025CRFPInt = 98,
	IANAifType_myrinet = 99,
	IANAifType_voiceEM = 100,
	IANAifType_voiceFXO = 101,
	IANAifType_voiceFXS = 102,
	IANAifType_voiceEncap = 103,
	IANAifType_voiceOverIp = 104,
	IANAifType_atmDxi = 105,
	IANAifType_atmFuni = 106,
	IANAifType_atmIma = 107,
	IANAifType_pppMultilinkBundle = 108,
	IANAifType_ipOverCdlc = 109,
	IANAifType_ipOverClaw = 110,
	IANAifType_stackToStack = 111,
	IANAifType_virtualIpAddress = 112,
	IANAifType_mpc = 113,
	IANAifType_ipOverAtm = 114,
	IANAifType_iso88025Fiber = 115,
	IANAifType_tdlc = 116,
	IANAifType_gigabitEthernet = 117,
	IANAifType_hdlc = 118,
	IANAifType_lapf = 119,
	IANAifType_v37 = 120,
	IANAifType_x25mlp = 121,
	IANAifType_x25huntGroup = 122,
	IANAifType_transpHdlc = 123,
	IANAifType_interleave = 124,
	IANAifType_fast = 125,
	IANAifType_ip = 126,
	IANAifType_docsCableMaclayer = 127,
	IANAifType_docsCableDownstream = 128,
	IANAifType_docsCableUpstream = 129,
	IANAifType_a12MppSwitch = 130,
	IANAifType_tunnel = 131,
	IANAifType_coffee = 132,
	IANAifType_ces = 133,
	IANAifType_atmSubInterface = 134,
	IANAifType_l2vlan = 135,
	IANAifType_l3ipvlan = 136,
	IANAifType_l3ipxvlan = 137,
	IANAifType_digitalPowerline = 138,
	IANAifType_mediaMailOverIp = 139,
	IANAifType_dtm = 140,
	IANAifType_dcn = 141,
	IANAifType_ipForward = 142,
	IANAifType_msdsl = 143,
	IANAifType_ieee1394 = 144,
	IANAifType_if_gsn = 145,
	IANAifType_dvbRccMacLayer = 146,
	IANAifType_dvbRccDownstream = 147,
	IANAifType_dvbRccUpstream = 148,
	IANAifType_atmVirtual = 149,
	IANAifType_mplsTunnel = 150,
	IANAifType_srp = 151,
	IANAifType_voiceOverAtm = 152,
	IANAifType_voiceOverFrameRelay = 153,
	IANAifType_idsl = 154,
	IANAifType_compositeLink = 155,
	IANAifType_ss7SigLink = 156,
	IANAifType_propWirelessP2P = 157,
	IANAifType_frForward = 158,
	IANAifType_rfc1483 = 159,
	IANAifType_usb = 160,
	IANAifType_ieee8023adLag = 161,
	IANAifType_bgppolicyaccounting = 162,
	IANAifType_frf16MfrBundle = 163,
	IANAifType_h323Gatekeeper = 164,
	IANAifType_h323Proxy = 165,
	IANAifType_mpls = 166,
	IANAifType_mfSigLink = 167,
	IANAifType_hdsl2 = 168,
	IANAifType_shdsl = 169,
	IANAifType_ds1FDL = 170,
	IANAifType_pos = 171,
	IANAifType_dvbAsiIn = 172,
	IANAifType_dvbAsiOut = 173,
	IANAifType_plc = 174,
	IANAifType_nfas = 175,
	IANAifType_tr008 = 176,
	IANAifType_gr303RDT = 177,
	IANAifType_gr303IDT = 178,
	IANAifType_isup = 179,
	IANAifType_propDocsWirelessMaclayer = 180,
	IANAifType_propDocsWirelessDownstream = 181,
	IANAifType_propDocsWirelessUpstream = 182,
	IANAifType_hiperlan2 = 183,
	IANAifType_propBWAp2Mp = 184,
	IANAifType_sonetOverheadChannel = 185,
	IANAifType_digitalWrapperOverheadChannel = 186,
	IANAifType_aal2 = 187,
	IANAifType_radioMAC = 188,
	IANAifType_atmRadio = 189,
	IANAifType_imt = 190,
	IANAifType_mvl = 191,
	IANAifType_reachDSL = 192,
	IANAifType_frDlciEndPt = 193,
	IANAifType_atmVciEndPt = 194,
	IANAifType_opticalChannel = 195,
	IANAifType_opticalTransport = 196,
	IANAifType_propAtm = 197,
	IANAifType_voiceOverCable = 198,
	IANAifType_infiniband = 199,
	IANAifType_teLink = 200,
	IANAifType_q2931 = 201,
	IANAifType_virtualTg = 202,
	IANAifType_sipTg = 203,
	IANAifType_sipSig = 204,
	IANAifType_docsCableUpstreamChannel = 205,
	IANAifType_econet = 206,
	IANAifType_pon155 = 207,
	IANAifType_pon622 = 208,
	IANAifType_bridge = 209,
	IANAifType_linegroup = 210,
	IANAifType_voiceEMFGD = 211,
	IANAifType_voiceFGDEANA = 212,
	IANAifType_voiceDID = 213,
	IANAifType_mpegTransport = 214,
	IANAifType_sixToFour = 215,
	IANAifType_gtp = 216,
	IANAifType_pdnEtherLoop1 = 217,
	IANAifType_pdnEtherLoop2 = 218,
	IANAifType_opticalChannelGroup = 219,
	IANAifType_homepna = 220,
	IANAifType_gfp = 221,
	IANAifType_ciscoISLvlan = 222,
	IANAifType_actelisMetaLOOP = 223,
	IANAifType_fcipLink = 224,
	IANAifType_rpr = 225,
	IANAifType_qam = 226,
	IANAifType_lmp = 227,
	IANAifType_cblVectaStar = 228,
	IANAifType_docsCableMCmtsDownstream = 229,
	IANAifType_adsl2 = 230,
	IANAifType_macSecControlledIF = 231,
	IANAifType_macSecUncontrolledIF = 232,
	IANAifType_aviciOpticalEther = 233,
	IANAifType_atmbond = 234,
	IANAifType_voiceFGDOS = 235,
	IANAifType_mocaVersion1 = 236,
	IANAifType_ieee80216WMAN = 237,
	IANAifType_adsl2plus = 238,
	IANAifType_dvbRcsMacLayer = 239,
	IANAifType_dvbTdm = 240,
	IANAifType_dvbRcsTdma = 241,
	IANAifType_x86Laps = 242,
	IANAifType_wwanPP = 243,
	IANAifType_wwanPP2 = 244,
	IANAifType_voiceEBS = 245,
	IANAifType_ifPwType = 246,
	IANAifType_ilan = 247,
	IANAifType_pip = 248,
	IANAifType_aluELP = 249
} IANAifType_Tval;

typedef enum {
	PbMsspSwitchStatus_idle = 0,
	PbMsspSwitchStatus_bridge = 1,
	PbMsspSwitchStatus_bridgeAndSwitch = 2,
	PbMsspSwitchStatus_passthough = 3
} PbMsspSwitchStatus_Tval;

typedef enum {
	PbMSPRestoreMode_revertive = 1,
	PbMSPRestoreMode_nonrevertive = 2
} PbMSPRestoreMode_Tval;

typedef enum {
	PbXcAction_up = 0,
	PbXcAction_down = 1,
	PbXcAction_delete = 2
} PbXcAction_Tval;

typedef enum {
	PbSdhLpSignalLabel_unequipped = 0,
	PbSdhLpSignalLabel_reserved = 1,
	PbSdhLpSignalLabel_async = 2,
	PbSdhLpSignalLabel_bitsync = 3,
	PbSdhLpSignalLabel_bytesync = 4,
	PbSdhLpSignalLabel_ext = 5,
	PbSdhLpSignalLabel_test = 6,
	PbSdhLpSignalLabel_ais = 7
} PbSdhLpSignalLabel_Tval;

typedef enum {
	PbPortOperationStatus_up = 1,
	PbPortOperationStatus_down = 2,
	PbPortOperationStatus_absent = 3,
	PbPortOperationStatus_na = 4
} PbPortOperationStatus_Tval;

typedef enum {
	PbLacpPortClass_lAN = 0,
	PbLacpPortClass_wAN = 1
} PbLacpPortClass_Tval;

typedef enum {
	RprXcTableClass_oldTable = 0,
	RprXcTableClass_newTable = 1
} RprXcTableClass_Tval;

typedef enum {
	PbAlarmGroupType_psuA = 1,
	PbAlarmGroupType_psuB = 2,
	PbAlarmGroupType_fan1 = 3,
	PbAlarmGroupType_fan2 = 4,
	PbAlarmGroupType_tms_slot_a1 = 5,
	PbAlarmGroupType_tms_slot_a2 = 6,
	PbAlarmGroupType_almevt_logger = 7,
	PbAlarmGroupType_slot_statistics = 8,
	PbAlarmGroupType_ds3 = 9,
	PbAlarmGroupType_sonet_line = 10,
	PbAlarmGroupType_sonet_path = 11,
	PbAlarmGroupType_fdr = 12,
	PbAlarmGroupType_node = 13,
	PbAlarmGroupType_dget_agent = 14,
	PbAlarmGroupType_dget_mgr = 15,
	PbAlarmGroupType_fan3 = 16,
	PbAlarmGroupType_optics = 17,
	PbAlarmGroupType_slot_intf = 18,
	PbAlarmGroupType_path_aps = 19,
	PbAlarmGroupType_power_circuit = 20,
	PbAlarmGroupType_line_aps = 21,
	PbAlarmGroupType_optical_signaling = 22
} PbAlarmGroupType_Tval;

typedef enum {
	PBSNCPIntrusive_sncp_i = 0,
	PBSNCPIntrusive_sncp_n = 1
} PBSNCPIntrusive_Tval;

typedef enum {
	PbSNCPRestoreMode_revertive = 1,
	PbSNCPRestoreMode_nonrevertive = 2
} PbSNCPRestoreMode_Tval;

typedef enum {
	PbXcType_add = 1,
	PbXcType_drop = 2,
	PbXcType_passthrough = 3
} PbXcType_Tval;

typedef enum {
	PbUpDown_up = 1,
	PbUpDown_down = 2
} PbUpDown_Tval;

typedef enum {
	PbMsspTrafficOrder_none = 0,
	PbMsspTrafficOrder_high = 1,
	PbMsspTrafficOrder_low = 2
} PbMsspTrafficOrder_Tval;

typedef enum {
	PbXcFailReason_noerr = 0,
	PbXcFailReason_resource = 1,
	PbXcFailReason_block = 2,
	PbXcFailReason_invalid = 3,
	PbXcFailReason_conflict = 4,
	PbXcFailReason_map = 5,
	PbXcFailReason_interface = 6,
	PbXcFailReason_locc = 7
} PbXcFailReason_Tval;

typedef enum {
	IANAtunnelType_other = 1,
	IANAtunnelType_direct = 2,
	IANAtunnelType_gre = 3,
	IANAtunnelType_minimal = 4,
	IANAtunnelType_l2tp = 5,
	IANAtunnelType_pptp = 6,
	IANAtunnelType_l2f = 7,
	IANAtunnelType_udp = 8,
	IANAtunnelType_atmp = 9,
	IANAtunnelType_msdp = 10,
	IANAtunnelType_sixToFour = 11,
	IANAtunnelType_sixOverFour = 12,
	IANAtunnelType_isatap = 13,
	IANAtunnelType_teredo = 14
} IANAtunnelType_Tval;

typedef enum {
	PbEpgFunctionCardStatus_unknown = 0,
	PbEpgFunctionCardStatus_latchoff = 1,
	PbEpgFunctionCardStatus_latchon = 2,
	PbEpgFunctionCardStatus_cardfail = 3
} PbEpgFunctionCardStatus_Tval;

typedef enum {
	RouteStatus_inactive = 1,
	RouteStatus_active = 2,
	RouteStatus_invalid = 3
} RouteStatus_Tval;

typedef enum {
	PbTMSMonitorMode_auto = 0,
	PbTMSMonitorMode_yes = 1,
	PbTMSMonitorMode_no = 2
} PbTMSMonitorMode_Tval;

typedef enum {
	RprXcFailReason_noerr = 0,
	RprXcFailReason_resource = 1,
	RprXcFailReason_block = 2,
	RprXcFailReason_invalid = 3,
	RprXcFailReason_conflict = 4,
	RprXcFailReason_map = 5,
	RprXcFailReason_interface = 6,
	RprXcFailReason_locc = 7
} RprXcFailReason_Tval;

typedef enum {
	PbActualPortType_none = 0,
	PbActualPortType_e1 = 1,
	PbActualPortType_ds1 = 2,
	PbActualPortType_e3 = 3,
	PbActualPortType_ds3 = 4,
	PbActualPortType_stm1 = 5,
	PbActualPortType_stm4 = 6
} PbActualPortType_Tval;

typedef enum {
	TruthValue_true = 1,
	TruthValue_false = 2
} TruthValue_Tval;

typedef enum {
	RprRingletID_ringlet0 = 0,
	RprRingletID_ringlet1 = 1
} RprRingletID_Tval;

typedef enum {
	PbMsspExternalCommand_exerciseRing = 3,
	PbMsspExternalCommand_manualSwitchRing = 6,
	PbMsspExternalCommand_forcedSwitchRing = 13,
	PbMsspExternalCommand_lockoutProtection = 15,
	PbMsspExternalCommand_clear = 21,
	PbMsspExternalCommand_noCommand = 23
} PbMsspExternalCommand_Tval;

typedef enum {
	PbPmMonitor_notMonitored = 1,
	PbPmMonitor_monitored = 2,
	PbPmMonitor_autoReport = 3
} PbPmMonitor_Tval;

typedef enum {
	PbOkNotOk_failed = 1,
	PbOkNotOk_okay = 2
} PbOkNotOk_Tval;

typedef enum {
	PbMspChNumberType_nullchannel = 0,
	PbMspChNumberType_workchannel1 = 1,
	PbMspChNumberType_workchannel2 = 2,
	PbMspChNumberType_workchannel3 = 3,
	PbMspChNumberType_workchannel4 = 4,
	PbMspChNumberType_workchannel5 = 5,
	PbMspChNumberType_workchannel6 = 6,
	PbMspChNumberType_workchannel7 = 7,
	PbMspChNumberType_workchannel8 = 8,
	PbMspChNumberType_workchannel9 = 9,
	PbMspChNumberType_workchannel10 = 10,
	PbMspChNumberType_workchannel11 = 11,
	PbMspChNumberType_workchannel12 = 12,
	PbMspChNumberType_workchannel13 = 13,
	PbMspChNumberType_workchannel14 = 14,
	PbMspChNumberType_extrafficchannel = 15
} PbMspChNumberType_Tval;

typedef enum {
	PbMSPExCmdRslt_cmdRsltSucc = 1,
	PbMSPExCmdRslt_cmdRsltLowPriority = 2,
	PbMSPExCmdRslt_cmdRsltSwithToActive = 3,
	PbMSPExCmdRslt_cmdSwitchFail = 4
} PbMSPExCmdRslt_Tval;

typedef enum {
	PbTMSOutActType_auto = 0,
	PbTMSOutActType_squelch = 1
} PbTMSOutActType_Tval;

typedef enum {
	PBMSPChPriState_unknown = 0,
	PBMSPChPriState_prihigh = 1,
	PBMSPChPriState_prilow = 2
} PBMSPChPriState_Tval;

typedef enum {
	PbPmTpStatusType_active = 1,
	PbPmTpStatusType_inactive = 2
} PbPmTpStatusType_Tval;

typedef enum {
	PbSncpFrom_cli = 1,
	PbSncpFrom_ems = 2,
	PbSncpFrom_snm = 3,
	PbSncpFrom_gmpls = 4
} PbSncpFrom_Tval;

typedef enum {
	RprXcMsgStatus_removed = 0,
	RprXcMsgStatus_disabled = 1,
	RprXcMsgStatus_ok = 2,
	RprXcMsgStatus_failed = 3
} RprXcMsgStatus_Tval;

typedef enum {
	PbUserAccessLevel_superUser = 1,
	PbUserAccessLevel_administrative = 2,
	PbUserAccessLevel_readOnly = 3
} PbUserAccessLevel_Tval;

typedef enum {
	PbXcCapacity_vc11 = 1,
	PbXcCapacity_vc12 = 2,
	PbXcCapacity_vc2 = 3,
	PbXcCapacity_vc3 = 4,
	PbXcCapacity_vc4 = 5,
	PbXcCapacity_vc44c = 6,
	PbXcCapacity_vc416c = 7,
	PbXcCapacity_vc464c = 8,
	PbXcCapacity_e1 = 9
} PbXcCapacity_Tval;

typedef enum {
	PbPortStatus_up = 1,
	PbPortStatus_down = 2
} PbPortStatus_Tval;

typedef enum {
	TypeMibUpDownLoadStatus_inactive = 0,
	TypeMibUpDownLoadStatus_uploadPreparing = 1,
	TypeMibUpDownLoadStatus_uploadPrepared = 2,
	TypeMibUpDownLoadStatus_uploadPrepareFailed = 3,
	TypeMibUpDownLoadStatus_uploadPrepareTimeout = 4,
	TypeMibUpDownLoadStatus_uploading = 5,
	TypeMibUpDownLoadStatus_uploaded = 6,
	TypeMibUpDownLoadStatus_uploadCanceled = 7,
	TypeMibUpDownLoadStatus_uploadingTimeout = 8,
	TypeMibUpDownLoadStatus_uploadingFailed = 9,
	TypeMibUpDownLoadStatus_downloadPreparing = 21,
	TypeMibUpDownLoadStatus_downloadPrepared = 22,
	TypeMibUpDownLoadStatus_downloadPrepareFailed = 23,
	TypeMibUpDownLoadStatus_downloadPrepareTimeout = 24,
	TypeMibUpDownLoadStatus_downloading = 25,
	TypeMibUpDownLoadStatus_downloaded = 26,
	TypeMibUpDownLoadStatus_downloadCanceled = 27,
	TypeMibUpDownLoadStatus_downloadingTimeout = 28,
	TypeMibUpDownLoadStatus_downloadingFailed = 29
} TypeMibUpDownLoadStatus_Tval;

typedef enum {
	PbMsspNodeState_idle = 0,
	PbMsspNodeState_passthough = 1,
	PbMsspNodeState_switching = 2,
	PbMsspNodeState_isolated = 3
} PbMsspNodeState_Tval;

typedef enum {
	PbSNCPExCmdRslt_cmdRsltSucc = 1,
	PbSNCPExCmdRslt_cmdRsltLowPriority = 2,
	PbSNCPExCmdRslt_cmdRsltSwithToActive = 3,
	PbSNCPExCmdRslt_cmdSwitchFail = 4,
	PbSNCPExCmdRslt_cmdSwitchTimeout = 5,
	PbSNCPExCmdRslt_cmdWaitCcRsp = 6,
	PbSNCPExCmdRslt_cmdBadCommand = 7
} PbSNCPExCmdRslt_Tval;

typedef enum {
	AlarmTypeID_spi_los = 1,
	AlarmTypeID_rs_oof = 2,
	AlarmTypeID_rs_lof = 3,
	AlarmTypeID_rs_deg = 4,
	AlarmTypeID_rs_tim = 5,
	AlarmTypeID_rs_exc = 6,
	AlarmTypeID_rs_dcf = 7,
	AlarmTypeID_rs_tca = 8,
	AlarmTypeID_ms_rdi = 9,
	AlarmTypeID_ms_rei = 10,
	AlarmTypeID_ms_ais = 11,
	AlarmTypeID_ms_deg = 12,
	AlarmTypeID_ms_exc = 13,
	AlarmTypeID_ms_dcf = 14,
	AlarmTypeID_ms_fop = 15,
	AlarmTypeID_ms_tca = 17,
	AlarmTypeID_au_lop = 18,
	AlarmTypeID_au_ais = 19,
	AlarmTypeID_au_tca = 20,
	AlarmTypeID_hp_tim = 21,
	AlarmTypeID_hp_uneq = 22,
	AlarmTypeID_hp_rdi = 23,
	AlarmTypeID_hp_plm = 24,
	AlarmTypeID_hp_deg = 25,
	AlarmTypeID_hp_lom = 26,
	AlarmTypeID_hp_exc = 27,
	AlarmTypeID_hp_tca = 28,
	AlarmTypeID_hpv_lom = 29,
	AlarmTypeID_hpv_sqm = 30,
	AlarmTypeID_hpv_loa = 31,
	AlarmTypeID_tu_lop = 32,
	AlarmTypeID_tu_lom = 33,
	AlarmTypeID_tu_ais = 34,
	AlarmTypeID_tu_tca = 35,
	AlarmTypeID_lp_tim = 36,
	AlarmTypeID_lp_uneq = 37,
	AlarmTypeID_lp_rdi = 38,
	AlarmTypeID_lp_plm = 39,
	AlarmTypeID_lp_deg = 40,
	AlarmTypeID_lp_exc = 41,
	AlarmTypeID_lp_rfi = 42,
	AlarmTypeID_lpv_lom = 43,
	AlarmTypeID_lpv_sqm = 44,
	AlarmTypeID_lpv_loa = 45,
	AlarmTypeID_lan_los = 46,
	AlarmTypeID_lan_bbr = 47,
	AlarmTypeID_lan_bpr = 48,
	AlarmTypeID_set_lti = 49,
	AlarmTypeID_set_lto = 50,
	AlarmTypeID_set_trf = 51,
	AlarmTypeID_set_deg = 52,
	AlarmTypeID_set_ssmbm = 53,
	AlarmTypeID_env_mdi = 54,
	AlarmTypeID_env_oh = 55,
	AlarmTypeID_eqp_uf = 56,
	AlarmTypeID_eqp_ur = 57,
	AlarmTypeID_eqp_pf = 58,
	AlarmTypeID_wan_los = 59,
	AlarmTypeID_wan_bbr = 60,
	AlarmTypeID_wan_bpr = 61,
	AlarmTypeID_tc_lcd = 62,
	AlarmTypeID_vp_ais = 63,
	AlarmTypeID_vp_rdi = 64,
	AlarmTypeID_vp_loc = 65,
	AlarmTypeID_vc_ais = 66,
	AlarmTypeID_vc_rdi = 67,
	AlarmTypeID_vc_loc = 68,
	AlarmTypeID_ppi_los = 69,
	AlarmTypeID_spi_dcf = 70,
	AlarmTypeID_in_cer = 71,
	AlarmTypeID_in_clr = 72,
	AlarmTypeID_out_cer = 73,
	AlarmTypeID_out_clr = 74,
	AlarmTypeID_lp_tca = 75,
	AlarmTypeID_set_scf = 76,
	AlarmTypeID_eqp_ff = 77,
	AlarmTypeID_spi_oop_tca = 78,
	AlarmTypeID_spi_iop_tca = 79,
	AlarmTypeID_spi_lb_tca = 80,
	AlarmTypeID_spi_lt_tca = 81,
	AlarmTypeID_spi_ta = 82,
	AlarmTypeID_eqp_utm = 83,
	AlarmTypeID_spi_lcd_slow = 84,
	AlarmTypeID_spi_lcd_fast = 85,
	AlarmTypeID_hpv_csf = 86,
	AlarmTypeID_hpv_lfd = 87,
	AlarmTypeID_lpv_csf = 88,
	AlarmTypeID_lpv_lfd = 89,
	AlarmTypeID_ppi_tca = 90,
	AlarmTypeID_ppi_nslip = 91,
	AlarmTypeID_ppi_pslip = 92,
	AlarmTypeID_rs_tca_es15m = 93,
	AlarmTypeID_rs_tca_es24h = 94,
	AlarmTypeID_rs_tca_bbe15m = 95,
	AlarmTypeID_rs_tca_bbe24h = 96,
	AlarmTypeID_rs_tca_ses15m = 97,
	AlarmTypeID_rs_tca_ses24h = 98,
	AlarmTypeID_rs_tca_uas15m = 99,
	AlarmTypeID_rs_tca_uas24h = 100,
	AlarmTypeID_rs_tca_uap15m = 101,
	AlarmTypeID_rs_tca_uap24h = 102,
	AlarmTypeID_rs_tca_cses15m = 103,
	AlarmTypeID_rs_tca_cses24h = 104,
	AlarmTypeID_rs_tca_ofs15m = 105,
	AlarmTypeID_rs_tca_ofs24h = 106,
	AlarmTypeID_ms_tca_es15mn = 107,
	AlarmTypeID_ms_tca_es15mf = 108,
	AlarmTypeID_ms_tca_es24hn = 109,
	AlarmTypeID_ms_tca_es24hf = 110,
	AlarmTypeID_ms_tca_bbe15mn = 111,
	AlarmTypeID_ms_tca_bbe15mf = 112,
	AlarmTypeID_ms_tca_bbe24hn = 113,
	AlarmTypeID_ms_tca_bbe24hf = 114,
	AlarmTypeID_ms_tca_ses15mn = 115,
	AlarmTypeID_ms_tca_ses15mf = 116,
	AlarmTypeID_ms_tca_ses24hn = 117,
	AlarmTypeID_ms_tca_ses24hf = 118,
	AlarmTypeID_ms_tca_uas15mn = 119,
	AlarmTypeID_ms_tca_uas15mf = 120,
	AlarmTypeID_ms_tca_uas24hn = 121,
	AlarmTypeID_ms_tca_uas24hf = 122,
	AlarmTypeID_ms_tca_uap15mn = 123,
	AlarmTypeID_ms_tca_uap15mf = 124,
	AlarmTypeID_ms_tca_uap24hn = 125,
	AlarmTypeID_ms_tca_uap24hf = 126,
	AlarmTypeID_ms_tca_cses15mn = 127,
	AlarmTypeID_ms_tca_cses15mf = 128,
	AlarmTypeID_ms_tca_cses24hn = 129,
	AlarmTypeID_ms_tca_cses24hf = 130,
	AlarmTypeID_hp_tca_es15mn = 131,
	AlarmTypeID_hp_tca_es15mf = 132,
	AlarmTypeID_hp_tca_es24hn = 133,
	AlarmTypeID_hp_tca_es24hf = 134,
	AlarmTypeID_hp_tca_bbe15mn = 135,
	AlarmTypeID_hp_tca_bbe15mf = 136,
	AlarmTypeID_hp_tca_bbe24hn = 137,
	AlarmTypeID_hp_tca_bbe24hf = 138,
	AlarmTypeID_hp_tca_ses15mn = 139,
	AlarmTypeID_hp_tca_ses15mf = 140,
	AlarmTypeID_hp_tca_ses24hn = 141,
	AlarmTypeID_hp_tca_ses24hf = 142,
	AlarmTypeID_hp_tca_uas15mn = 143,
	AlarmTypeID_hp_tca_uas15mf = 144,
	AlarmTypeID_hp_tca_uas24hn = 145,
	AlarmTypeID_hp_tca_uas24hf = 146,
	AlarmTypeID_hp_tca_uap15mn = 147,
	AlarmTypeID_hp_tca_uap15mf = 148,
	AlarmTypeID_hp_tca_uap24hn = 149,
	AlarmTypeID_hp_tca_uap24hf = 150,
	AlarmTypeID_hp_tca_cses15mn = 151,
	AlarmTypeID_hp_tca_cses15mf = 152,
	AlarmTypeID_hp_tca_cses24hn = 153,
	AlarmTypeID_hp_tca_cses24hf = 154,
	AlarmTypeID_hp_tca_pjcp15m = 155,
	AlarmTypeID_hp_tca_pjcp24h = 156,
	AlarmTypeID_hp_tca_pjcn15m = 157,
	AlarmTypeID_hp_tca_pjcn24h = 158,
	AlarmTypeID_lp_tca_es15mn = 159,
	AlarmTypeID_lp_tca_es15mf = 160,
	AlarmTypeID_lp_tca_es24hn = 161,
	AlarmTypeID_lp_tca_es24hf = 162,
	AlarmTypeID_lp_tca_bbe15mn = 163,
	AlarmTypeID_lp_tca_bbe15mf = 164,
	AlarmTypeID_lp_tca_bbe24hn = 165,
	AlarmTypeID_lp_tca_bbe24hf = 166,
	AlarmTypeID_lp_tca_ses15mn = 167,
	AlarmTypeID_lp_tca_ses15mf = 168,
	AlarmTypeID_lp_tca_ses24hn = 169,
	AlarmTypeID_lp_tca_ses24hf = 170,
	AlarmTypeID_lp_tca_uas15mn = 171,
	AlarmTypeID_lp_tca_uas15mf = 172,
	AlarmTypeID_lp_tca_uas24hn = 173,
	AlarmTypeID_lp_tca_uas24hf = 174,
	AlarmTypeID_lp_tca_uap15mn = 175,
	AlarmTypeID_lp_tca_uap15mf = 176,
	AlarmTypeID_lp_tca_uap24hn = 177,
	AlarmTypeID_lp_tca_uap24hf = 178,
	AlarmTypeID_lp_tca_cses15mn = 179,
	AlarmTypeID_lp_tca_cses15mf = 180,
	AlarmTypeID_lp_tca_cses24hn = 181,
	AlarmTypeID_lp_tca_cses24hf = 182,
	AlarmTypeID_lp_tca_pjcp15m = 183,
	AlarmTypeID_lp_tca_pjcp24h = 184,
	AlarmTypeID_lp_tca_pjcn15m = 185,
	AlarmTypeID_lp_tca_pjcn24h = 186,
	AlarmTypeID_env_mdi1 = 187,
	AlarmTypeID_env_mdi2 = 188,
	AlarmTypeID_env_mdi3 = 189,
	AlarmTypeID_env_mdi4 = 190,
	AlarmTypeID_rpr_katoe = 191,
	AlarmTypeID_rpr_katow = 192,
	AlarmTypeID_rpr_dmac = 193,
	AlarmTypeID_rpr_eres0 = 194,
	AlarmTypeID_rpr_eres1 = 195,
	AlarmTypeID_rpr_lrnc = 196,
	AlarmTypeID_rpr_mmac = 197,
	AlarmTypeID_rpr_mstn = 198,
	AlarmTypeID_rpr_mscbe = 199,
	AlarmTypeID_rpr_mscbw = 200,
	AlarmTypeID_rpr_pmcfg = 201,
	AlarmTypeID_rpr_tpincs = 202,
	AlarmTypeID_rpr_tpinst = 203,
	AlarmTypeID_oa_tf = 211,
	AlarmTypeID_oa_los = 212,
	AlarmTypeID_oa_bias = 214,
	AlarmTypeID_oa_lth = 215,
	AlarmTypeID_eqp_mismatch = 216,
	AlarmTypeID_neg_slip_tca_15m = 217,
	AlarmTypeID_neg_slip_tca_24h = 218,
	AlarmTypeID_pos_slip_tca_15m = 219,
	AlarmTypeID_pos_slip_tca_24h = 220,
	AlarmTypeID_set_timedeg = 221,
	AlarmTypeID_sfp_failure = 222,
	AlarmTypeID_ecc1_failure = 223,
	AlarmTypeID_sfp_mismatch = 224,
	AlarmTypeID_ppi_ais = 225,
	AlarmTypeID_oa_output_power_high = 226,
	AlarmTypeID_oa_gain_high = 227,
	AlarmTypeID_ecc2_failure = 228,
	AlarmTypeID_memory_card_failure = 229,
	AlarmTypeID_ppi_lof = 230,
	AlarmTypeID_ppi_lom = 231,
	AlarmTypeID_ppi_rdi = 232,
	AlarmTypeID_hpvc_tlct = 234,
	AlarmTypeID_lpvc_tlct = 235,
	AlarmTypeID_ppi_tca_es15mn = 236,
	AlarmTypeID_ppi_tca_es15mf = 237,
	AlarmTypeID_ppi_tca_es24hn = 238,
	AlarmTypeID_ppi_tca_es24hf = 239,
	AlarmTypeID_ppi_tca_bbe15mn = 240,
	AlarmTypeID_ppi_tca_bbe15mf = 241,
	AlarmTypeID_ppi_tca_bbe24hn = 242,
	AlarmTypeID_ppi_tca_bbe24hf = 243,
	AlarmTypeID_ppi_tca_ses15mn = 244,
	AlarmTypeID_ppi_tca_ses15mf = 245,
	AlarmTypeID_ppi_tca_ses24hn = 246,
	AlarmTypeID_ppi_tca_ses24hf = 247,
	AlarmTypeID_ppi_tca_uas15mn = 248,
	AlarmTypeID_ppi_tca_uas15mf = 249,
	AlarmTypeID_ppi_tca_uas24hn = 250,
	AlarmTypeID_ppi_tca_uas24hf = 251,
	AlarmTypeID_loss_of_power_input = 253,
	AlarmTypeID_env_mdi5 = 254,
	AlarmTypeID_env_mdi6 = 255,
	AlarmTypeID_env_mdi7 = 256,
	AlarmTypeID_env_mdi8 = 257,
	AlarmTypeID_sfp_absent = 258,
	AlarmTypeID_memory_card_absent = 259,
	AlarmTypeID_oa_pf = 260,
	AlarmTypeID_sc_sw_version_mismatch = 286,
	AlarmTypeID_hcoc9_dcf = 287,
	AlarmTypeID_hcoc27_dcf = 288,
	AlarmTypeID_unsuccessful_logins_exceeded = 289,
	AlarmTypeID_log_occupancy_threshold = 290,
	AlarmTypeID_log_is_full = 291,
	AlarmTypeID_unauthorized_access = 292,
	AlarmTypeID_communication_failure = 293,
	AlarmTypeID_no_free_low_order_capacity = 294,
	AlarmTypeID_low_order_capacity_mismatch = 295,
	AlarmTypeID_pll_unlock = 296,
	AlarmTypeID_account_expired_disabled = 297,
	AlarmTypeID_vcxo_failure = 298,
	AlarmTypeID_eq_fal = 299,
	AlarmTypeID_tr_cim = 300,
	AlarmTypeID_otu_lfa = 301,
	AlarmTypeID_otu_lma = 302,
	AlarmTypeID_sm_tim = 303,
	AlarmTypeID_sm_iae = 304,
	AlarmTypeID_sm_bdi = 305,
	AlarmTypeID_opu_ptm = 306,
	AlarmTypeID_loss_of_optical_input = 307,
	AlarmTypeID_ld_fail = 308,
	AlarmTypeID_optical_output_fail = 309,
	AlarmTypeID_wavelength_control = 310,
	AlarmTypeID_dcn_2m_los = 311,
	AlarmTypeID_flash_util_over = 312,
	AlarmTypeID_cfcard_util_over = 313,
	AlarmTypeID_signal_error_fail = 314,
	AlarmTypeID_latch_open = 315,
	AlarmTypeID_otu_deg = 316,
	AlarmTypeID_odu2_loflom = 317,
	AlarmTypeID_odu2_ais = 318,
	AlarmTypeID_odu2_lck = 319,
	AlarmTypeID_odu2_oci = 320,
	AlarmTypeID_odu2_tim = 321,
	AlarmTypeID_odu2_bdi = 322,
	AlarmTypeID_odu2_deg = 323,
	AlarmTypeID_odu2_plm = 324,
	AlarmTypeID_odu2_msim = 325,
	AlarmTypeID_odu1_loflom = 326,
	AlarmTypeID_odu1_ais = 327,
	AlarmTypeID_odu1_lck = 328,
	AlarmTypeID_odu1_oci = 329,
	AlarmTypeID_odu1_tim = 330,
	AlarmTypeID_odu1_bdi = 331,
	AlarmTypeID_odu1_deg = 332,
	AlarmTypeID_odu1_plm = 333,
	AlarmTypeID_hpvc_plct = 334,
	AlarmTypeID_lpvc_plct = 335,
	AlarmTypeID_hpvc_plcr = 336,
	AlarmTypeID_lpvc_plcr = 337,
	AlarmTypeID_eqp_fan_failure = 338
} AlarmTypeID_Tval;

typedef enum {
	RprXcCapacity_vc11 = 1,
	RprXcCapacity_vc12 = 2,
	RprXcCapacity_vc2 = 3,
	RprXcCapacity_vc3 = 4,
	RprXcCapacity_vc4 = 5,
	RprXcCapacity_vc44c = 6,
	RprXcCapacity_vc416c = 7,
	RprXcCapacity_vc464c = 8,
	RprXcCapacity_e1 = 9
} RprXcCapacity_Tval;

typedef enum {
	PbTimeZone_eniwetok = 0,
	PbTimeZone_midwayIsland = 1,
	PbTimeZone_hawaii = 2,
	PbTimeZone_alaska = 3,
	PbTimeZone_pacificTime = 4,
	PbTimeZone_arizona = 5,
	PbTimeZone_mountainTime = 10,
	PbTimeZone_centralAmerica = 15,
	PbTimeZone_centralTime = 20,
	PbTimeZone_mexico = 25,
	PbTimeZone_saskatchewan = 30,
	PbTimeZone_bogota = 35,
	PbTimeZone_easternTime = 40,
	PbTimeZone_indiana = 45,
	PbTimeZone_atlantic = 50,
	PbTimeZone_caracas = 55,
	PbTimeZone_santiago = 56,
	PbTimeZone_newfoundland = 60,
	PbTimeZone_brasilia = 65,
	PbTimeZone_buenosAires = 70,
	PbTimeZone_greenland = 73,
	PbTimeZone_midAtlantic = 75,
	PbTimeZone_azores = 80,
	PbTimeZone_capeVerdeIslands = 83,
	PbTimeZone_casablanca = 85,
	PbTimeZone_greenwich = 90,
	PbTimeZone_amsterdam = 95,
	PbTimeZone_belgrade = 100,
	PbTimeZone_brussels = 105,
	PbTimeZone_sarajevo = 110,
	PbTimeZone_westCentralAfrica = 115,
	PbTimeZone_athens = 120,
	PbTimeZone_bucharest = 125,
	PbTimeZone_cairo = 130,
	PbTimeZone_harare = 135,
	PbTimeZone_helsinki = 140,
	PbTimeZone_jerusalem = 145,
	PbTimeZone_baghdad = 150,
	PbTimeZone_kuwait = 155,
	PbTimeZone_moscow = 158,
	PbTimeZone_nairobi = 160,
	PbTimeZone_tehran = 165,
	PbTimeZone_abuDhabi = 170,
	PbTimeZone_baku = 175,
	PbTimeZone_kabul = 180,
	PbTimeZone_ekaterinburg = 185,
	PbTimeZone_islamabad = 190,
	PbTimeZone_calcutta = 195,
	PbTimeZone_kathmandu = 198,
	PbTimeZone_almaty = 200,
	PbTimeZone_astana = 201,
	PbTimeZone_sri = 202,
	PbTimeZone_rangoony = 205,
	PbTimeZone_bangkok = 207,
	PbTimeZone_krasnoyarsk = 208,
	PbTimeZone_beijing = 210,
	PbTimeZone_irkutsk = 212,
	PbTimeZone_kualaLumpur = 215,
	PbTimeZone_perth = 220,
	PbTimeZone_taipei = 225,
	PbTimeZone_osaka = 230,
	PbTimeZone_seoulr = 235,
	PbTimeZone_yakutsk = 240,
	PbTimeZone_adelaidei = 245,
	PbTimeZone_darwin = 250,
	PbTimeZone_brisbane = 255,
	PbTimeZone_canberra = 260,
	PbTimeZone_guam = 265,
	PbTimeZone_hobart = 270,
	PbTimeZone_vladivostok = 275,
	PbTimeZone_magadan = 280,
	PbTimeZone_auckland = 285,
	PbTimeZone_fujiIslands = 290,
	PbTimeZone_nuku = 300
} PbTimeZone_Tval;

typedef enum {
	PbSecondaryStatus_none = 1,
	PbSecondaryStatus_bsl_down = 2,
	PbSecondaryStatus_bsl_adminDown = 3,
	PbSecondaryStatus_bsl_HW_adminDown = 4,
	PbSecondaryStatus_bsl_HW_adminTest = 5,
	PbSecondaryStatus_bsl_HW_down = 6,
	PbSecondaryStatus_bsl_notPresent = 7,
	PbSecondaryStatus_sl_adminDown = 8,
	PbSecondaryStatus_sl_adminTest = 9,
	PbSecondaryStatus_hw_adminDown = 10,
	PbSecondaryStatus_hw_adminTest = 11,
	PbSecondaryStatus_hw_down = 12,
	PbSecondaryStatus_nc_HW_removed = 13,
	PbSecondaryStatus_nc_HW_adminDown = 14,
	PbSecondaryStatus_nc_HW_adminTest = 15,
	PbSecondaryStatus_nc_HW_down = 16,
	PbSecondaryStatus_nc_SL_adminDown = 17,
	PbSecondaryStatus_nc_SL_adminTest = 18,
	PbSecondaryStatus_nc_SL_down = 19,
	PbSecondaryStatus_nc_funcAdminDown = 20,
	PbSecondaryStatus_hw_inactive = 21
} PbSecondaryStatus_Tval;

typedef enum {
	PbLatchState_on = 1,
	PbLatchState_off = 2
} PbLatchState_Tval;

typedef enum {
	PbTMSOutState_normal = 0,
	PbTMSOutState_ais = 1,
	PbTMSOutState_dnu = 2,
	PbTMSOutState_shutdown = 3,
	PbTMSOutState_na = 4
} PbTMSOutState_Tval;

typedef enum {
	RprProtectionStatus_noRequest = 1,
	RprProtectionStatus_waitToRestore = 2,
	RprProtectionStatus_manualSwitch = 4,
	RprProtectionStatus_signalDegraded = 8,
	RprProtectionStatus_signalFailed = 16,
	RprProtectionStatus_forcedSwitch = 32
} RprProtectionStatus_Tval;

typedef enum {
	PbSlotNumberType_none = 0,
	PbSlotNumberType_lc1 = 1,
	PbSlotNumberType_lc2 = 2,
	PbSlotNumberType_lc3 = 3,
	PbSlotNumberType_lc4 = 4,
	PbSlotNumberType_lc5 = 5,
	PbSlotNumberType_lc6 = 6,
	PbSlotNumberType_hocc1 = 7,
	PbSlotNumberType_hocc2 = 8,
	PbSlotNumberType_sfc1 = 9,
	PbSlotNumberType_sfc2 = 10,
	PbSlotNumberType_ce1 = 11,
	PbSlotNumberType_ce2 = 12,
	PbSlotNumberType_ce3 = 13,
	PbSlotNumberType_ce4 = 14,
	PbSlotNumberType_ce5 = 15,
	PbSlotNumberType_ce6 = 16,
	PbSlotNumberType_sc1 = 17,
	PbSlotNumberType_sc2 = 18,
	PbSlotNumberType_si = 19,
	PbSlotNumberType_sfm1 = 20,
	PbSlotNumberType_sfm2 = 21,
	PbSlotNumberType_io1 = 22,
	PbSlotNumberType_io2 = 23,
	PbSlotNumberType_io3 = 24,
	PbSlotNumberType_io4 = 25,
	PbSlotNumberType_io5 = 26,
	PbSlotNumberType_io6 = 27,
	PbSlotNumberType_io7 = 28,
	PbSlotNumberType_io8 = 29,
	PbSlotNumberType_io9 = 30,
	PbSlotNumberType_io10 = 31,
	PbSlotNumberType_io11 = 32,
	PbSlotNumberType_io12 = 33,
	PbSlotNumberType_io13 = 34,
	PbSlotNumberType_io14 = 35,
	PbSlotNumberType_io15 = 36,
	PbSlotNumberType_power1 = 37,
	PbSlotNumberType_power2 = 38,
	PbSlotNumberType_fan1 = 39,
	PbSlotNumberType_fan2 = 40
} PbSlotNumberType_Tval;

typedef enum {
	PbMsspNodeSide_west = 0,
	PbMsspNodeSide_east = 1
} PbMsspNodeSide_Tval;

typedef enum {
	PbSNCPChannelState_active = 1,
	PbSNCPChannelState_standby = 2
} PbSNCPChannelState_Tval;

typedef enum {
	SdhFtpCmndLastState_notApplicable = 1,
	SdhFtpCmndLastState_success = 2,
	SdhFtpCmndLastState_failed = 3,
	SdhFtpCmndLastState_inProgress = 4
} SdhFtpCmndLastState_Tval;

typedef enum {
	PbLinkGeneralType_networkLink = 1,
	PbLinkGeneralType_offNetworkLink = 2
} PbLinkGeneralType_Tval;

typedef enum {
	PbBoardFunction_none = 0,
	PbBoardFunction_empty = 1,
	PbBoardFunction_main = 2305,
	PbBoardFunction_hocc = 2306,
	PbBoardFunction_locc = 2307,
	PbBoardFunction_e1x126 = 2308,
	PbBoardFunction_e1x63 = 2309,
	PbBoardFunction_stm_1ex8 = 2310,
	PbBoardFunction_e3ds3x12 = 2312,
	PbBoardFunction_stm_41x8 = 2320,
	PbBoardFunction_stm_1x8 = 2321,
	PbBoardFunction_stm_16x4 = 2322,
	PbBoardFunction_stm_16x8 = 2323,
	PbBoardFunction_e3ds3px12 = 2324,
	PbBoardFunction_stm_64x2 = 2325,
	PbBoardFunction_stm_64x1 = 2326,
	PbBoardFunction_oaBooster13dbm = 2336,
	PbBoardFunction_oaBooster15dbm = 2337,
	PbBoardFunction_oaBooster18dbm = 2338,
	PbBoardFunction_oaPreamp20db = 2340,
	PbBoardFunction_tengetx1 = 2342,
	PbBoardFunction_tengetx1_geax46 = 2343,
	PbBoardFunction_getx8 = 2344,
	PbBoardFunction_gex6_fegeax4 = 2345,
	PbBoardFunction_gex2_ferprx12 = 2352,
	PbBoardFunction_gex4_feax12 = 2353,
	PbBoardFunction_fetx12 = 2354,
	PbBoardFunction_fc2gx8 = 2355,
	PbBoardFunction_stm_256x1 = 2357,
	PbBoardFunction_fan = 2358,
	PbBoardFunction_si_without_eow = 2368,
	PbBoardFunction_stm_16x2 = 2369,
	PbBoardFunction_otu_2x1 = 2400,
	PbBoardFunction_tengex1_gex10l2 = 2440,
	PbBoardFunction_stm_16x8m = 2441,
	PbBoardFunction_stm_41x16m = 2448,
	PbBoardFunction_stm_64x2m = 2449,
	PbBoardFunction_otu_2x2 = 2450,
	PbBoardFunction_chassis = 4232,
	PbBoardFunction_cc = 4240,
	PbBoardFunction_dataFabric_controller = 4241,
	PbBoardFunction_dataFabric = 4242,
	PbBoardFunction_si = 4243,
	PbBoardFunction_power = 4244,
	PbBoardFunction_chassis_fan_airFilter = 4245,
	PbBoardFunction_fanTray = 4246,
	PbBoardFunction_fanCircuitBoard = 4247,
	PbBoardFunction_backplane = 4248,
	PbBoardFunction_e1x21p_75 = 4352,
	PbBoardFunction_e1x21p_120 = 4353,
	PbBoardFunction_stm_1ex16 = 4354,
	PbBoardFunction_stm_1ex8p = 4356,
	PbBoardFunction_e3ds3x6p = 4358,
	PbBoardFunction_gex20 = 4359,
	PbBoardFunction_tengex2 = 4360,
	PbBoardFunction_gex10_eos = 4361,
	PbBoardFunction_tengex2_eos = 4368,
	PbBoardFunction_gmpls_load = 983040
} PbBoardFunction_Tval;

typedef enum {
	PbYesNo_yes = 1,
	PbYesNo_no = 2
} PbYesNo_Tval;

typedef enum {
	PBMSPSignalState_signalunknown = 0,
	PBMSPSignalState_signalfailhigh = 1,
	PBMSPSignalState_signalfaillow = 2,
	PBMSPSignalState_signaldeglhigh = 3,
	PBMSPSignalState_signaldeglow = 4,
	PBMSPSignalState_signalnormal = 5
} PBMSPSignalState_Tval;

typedef enum {
	PbMSPSignalDirection_unidirection = 1,
	PbMSPSignalDirection_bidirection = 2
} PbMSPSignalDirection_Tval;

typedef enum {
	PbEpgCmdResult_ok = 0,
	PbEpgCmdResult_failed = 1
} PbEpgCmdResult_Tval;

typedef enum {
	TMSIfSdhQlType_sdh = 1,
	TMSIfSdhQlType_sonet = 2
} TMSIfSdhQlType_Tval;

typedef enum {
	PbChassisType_mw863 = 1,
	PbChassisType_mw808 = 2,
	PbChassisType_mw8100 = 3,
	PbChassisType_mw801 = 4,
	PbChassisType_hiT7035 = 5,
	PbChassisType_hiT7080 = 7,
	PbChassisType_hiT7065 = 8,
	PbChassisType_all = 255
} PbChassisType_Tval;

typedef enum {
	PbMsspSwStatus_idle = 0,
	PbMsspSwStatus_bridge = 1,
	PbMsspSwStatus_bridgeAndSwitch = 2,
	PbMsspSwStatus_passthough = 3
} PbMsspSwStatus_Tval;

typedef enum {
	PbTMSQl_inv0 = 0,
	PbTMSQl_inv1 = 1,
	PbTMSQl_prc = 2,
	PbTMSQl_inv3 = 3,
	PbTMSQl_ssut = 4,
	PbTMSQl_inv5 = 5,
	PbTMSQl_inv6 = 6,
	PbTMSQl_inv7 = 7,
	PbTMSQl_ssul = 8,
	PbTMSQl_inv9 = 9,
	PbTMSQl_inv10 = 10,
	PbTMSQl_sec = 11,
	PbTMSQl_inv12 = 12,
	PbTMSQl_inv13 = 13,
	PbTMSQl_inv14 = 14,
	PbTMSQl_dnu = 15,
	PbTMSQl_failed = 16,
	PbTMSQl_nsupp = 17,
	PbTMSQl_unc = 18
} PbTMSQl_Tval;

typedef enum {
	TypeMibUpgFailedIndex_noError = 0,
	TypeMibUpgFailedIndex_unKnown = 1,
	TypeMibUpgFailedIndex_writeFlashFailed = 2,
	TypeMibUpgFailedIndex_readFlashFailed = 3,
	TypeMibUpgFailedIndex_flashFull = 4,
	TypeMibUpgFailedIndex_flashError = 5,
	TypeMibUpgFailedIndex_writeCfFailed = 6,
	TypeMibUpgFailedIndex_readCfFailed = 7,
	TypeMibUpgFailedIndex_cfFull = 8,
	TypeMibUpgFailedIndex_cfError = 9,
	TypeMibUpgFailedIndex_copyAscFlashFailed = 10,
	TypeMibUpgFailedIndex_copyAscCfFailed = 11,
	TypeMibUpgFailedIndex_copySscFlashFailed = 12,
	TypeMibUpgFailedIndex_copySscCfFailed = 13,
	TypeMibUpgFailedIndex_spawnTaskFailed = 14,
	TypeMibUpgFailedIndex_paraInvalid = 15,
	TypeMibUpgFailedIndex_writeRamdscFailed = 16,
	TypeMibUpgFailedIndex_readRamdscFailed = 17,
	TypeMibUpgFailedIndex_setMibFailed = 18,
	TypeMibUpgFailedIndex_getMibFailed = 19,
	TypeMibUpgFailedIndex_openDirFailed = 20,
	TypeMibUpgFailedIndex_createDirFailed = 21,
	TypeMibUpgFailedIndex_peerScOnline = 22,
	TypeMibUpgFailedIndex_peerScOffline = 23,
	TypeMibUpgFailedIndex_synRamdscFailed = 24,
	TypeMibUpgFailedIndex_synFlashFailed = 25,
	TypeMibUpgFailedIndex_synCfFailed = 26,
	TypeMibUpgFailedIndex_msgCreateFailed = 27,
	TypeMibUpgFailedIndex_msgSendFailed = 28,
	TypeMibUpgFailedIndex_cmndStatusInvalid = 29,
	TypeMibUpgFailedIndex_sscCopyTimeout = 30,
	TypeMibUpgFailedIndex_createSemFailed = 31,
	TypeMibUpgFailedIndex_mibHeadFileInvalid = 32,
	TypeMibUpgFailedIndex_mibNeFileCheckSumInvalid = 33,
	TypeMibUpgFailedIndex_mibGmFileCheckSumInvalid = 34,
	TypeMibUpgFailedIndex_lcsFileInvalid = 35,
	TypeMibUpgFailedIndex_dirCopyFailed = 36,
	TypeMibUpgFailedIndex_dirDelFailed = 37,
	TypeMibUpgFailedIndex_zipFileNotExist = 38,
	TypeMibUpgFailedIndex_tarFileFailed = 39,
	TypeMibUpgFailedIndex_zipFileFailed = 40,
	TypeMibUpgFailedIndex_unzipFileFailed = 41,
	TypeMibUpgFailedIndex_gsemTakeFailed = 42,
	TypeMibUpgFailedIndex_gSaveFlagSetFailed = 43,
	TypeMibUpgFailedIndex_dirRnameLoadTmpFailed = 44,
	TypeMibUpgFailedIndex_dirRnameTmpLoadFailed = 45,
	TypeMibUpgFailedIndex_dirRnameTmpBkupFailed = 46,
	TypeMibUpgFailedIndex_dirRnameBkupActFailed = 47,
	TypeMibUpgFailedIndex_mibIncSaveFailed = 48,
	TypeMibUpgFailedIndex_mibAllSaveFailed = 49,
	TypeMibUpgFailedIndex_mibSaveSwitchSetFailed = 50,
	TypeMibUpgFailedIndex_scSwitch = 51,
	TypeMibUpgFailedIndex_upgCommandRunning = 52,
	TypeMibUpgFailedIndex_cfaSoftCopying = 53,
	TypeMibUpgFailedIndex_psmMibCheckingFuncInvalid = 54,
	TypeMibUpgFailedIndex_lcmMibCheckingFuncInvalid = 55,
	TypeMibUpgFailedIndex_globleHeadFileInvalid = 56,
	TypeMibUpgFailedIndex_neHeadfileFlagSetFailed = 57,
	TypeMibUpgFailedIndex_psmHeadfileFlagSetFailed = 58,
	TypeMibUpgFailedIndex_uploadPrepareTimeout = 71,
	TypeMibUpgFailedIndex_uploadPrepareFailed = 72,
	TypeMibUpgFailedIndex_uploadMidTimeout = 73,
	TypeMibUpgFailedIndex_uploadMidFailed = 74,
	TypeMibUpgFailedIndex_uploadCanceled = 75,
	TypeMibUpgFailedIndex_downloadPrepareTimeout = 81,
	TypeMibUpgFailedIndex_downloadPrepareFailed = 82,
	TypeMibUpgFailedIndex_downloadMidTimeout = 83,
	TypeMibUpgFailedIndex_downloadMidFailed = 84,
	TypeMibUpgFailedIndex_downloadCanceled = 85,
	TypeMibUpgFailedIndex_switchAscTimeout = 91,
	TypeMibUpgFailedIndex_switchAscFailed = 92,
	TypeMibUpgFailedIndex_switchAscCfFailed = 93,
	TypeMibUpgFailedIndex_synTimeout = 101,
	TypeMibUpgFailedIndex_synCanceled = 102,
	TypeMibUpgFailedIndex_synFailed = 103,
	TypeMibUpgFailedIndex_clearTimeout = 111,
	TypeMibUpgFailedIndex_clearFailed = 112,
	TypeMibUpgFailedIndex_ascMibClearSetFailed = 113,
	TypeMibUpgFailedIndex_sscMibClearSetFailed = 114,
	TypeMibUpgFailedIndex_mibCopyTimeout = 121,
	TypeMibUpgFailedIndex_mibCopyFailed = 122,
	TypeMibUpgFailedIndex_gmplsCopyFailed = 123,
	TypeMibUpgFailedIndex_lcsynTimeout = 131,
	TypeMibUpgFailedIndex_lcsSynFailed = 132,
	TypeMibUpgFailedIndex_lcsDownTimeout = 141,
	TypeMibUpgFailedIndex_lcsDownFailed = 142
} TypeMibUpgFailedIndex_Tval;

typedef enum {
	PBSNCPAction_normal = 0,
	PBSNCPAction_recfgw = 1,
	PBSNCPAction_recfgp = 2,
	PBSNCPAction_setmib = 3
} PBSNCPAction_Tval;

typedef enum {
	PBMSPType_msponevsn = 1,
	PBMSPType_msponeplusone = 2
} PBMSPType_Tval;

typedef enum {
	PbAlmClassType_ne = 1,
	PbAlmClassType_slot = 2,
	PbAlmClassType_port = 3,
	PbAlmClassType_tp = 4
} PbAlmClassType_Tval;

typedef enum {
	RprSvcStatus_ok = 0,
	RprSvcStatus_failed = 1,
	RprSvcStatus_removed = 2
} RprSvcStatus_Tval;

typedef enum {
	PbLowHigh_low = 1,
	PbLowHigh_high = 2
} PbLowHigh_Tval;

typedef enum {
	PBSNCPSignalState_normal = 1,
	PBSNCPSignalState_failed = 2,
	PBSNCPSignalState_degard = 3
} PBSNCPSignalState_Tval;

typedef enum {
	PbSNCPSignalDirection_source = 1,
	PbSNCPSignalDirection_sink = 2
} PbSNCPSignalDirection_Tval;

typedef enum {
	PbCircuitDirection_bidirection = 1,
	PbCircuitDirection_unidirection = 2,
	PbCircuitDirection_multicast = 3
} PbCircuitDirection_Tval;

typedef enum {
	RprRingType_closedRing = 1,
	RprRingType_openRing = 2
} RprRingType_Tval;

typedef enum {
	RprOamRinglet_default = 1,
	RprOamRinglet_ringlet0 = 2,
	RprOamRinglet_ringlet1 = 3,
	RprOamRinglet_reverseRinglet = 4
} RprOamRinglet_Tval;

typedef enum {
	PbColdRebootNodeState_false = 0,
	PbColdRebootNodeState_truewithfpga = 1,
	PbColdRebootNodeState_truewithoutfpga = 2
} PbColdRebootNodeState_Tval;

typedef enum {
	RprServiceType_p2p = 1,
	RprServiceType_mp2mp = 2,
	RprServiceType_local = 3
} RprServiceType_Tval;

typedef enum {
	TypeSoftUpgFailedIndex_noError = 0,
	TypeSoftUpgFailedIndex_unKnown = 1,
	TypeSoftUpgFailedIndex_commandTimeout = 2,
	TypeSoftUpgFailedIndex_wrongNodeType = 3,
	TypeSoftUpgFailedIndex_redunLoadError = 4,
	TypeSoftUpgFailedIndex_writeFlashFailed = 5,
	TypeSoftUpgFailedIndex_readFlashFailed = 6,
	TypeSoftUpgFailedIndex_flashFull = 7,
	TypeSoftUpgFailedIndex_flashError = 8,
	TypeSoftUpgFailedIndex_writeCfFailed = 9,
	TypeSoftUpgFailedIndex_readCfFailed = 10,
	TypeSoftUpgFailedIndex_cfFull = 11,
	TypeSoftUpgFailedIndex_cfError = 12,
	TypeSoftUpgFailedIndex_copyAscFlashFailed = 13,
	TypeSoftUpgFailedIndex_copyAscCfFailed = 14,
	TypeSoftUpgFailedIndex_copySscFlashFailed = 15,
	TypeSoftUpgFailedIndex_copySscCfFailed = 16,
	TypeSoftUpgFailedIndex_spawnTaskFailed = 17,
	TypeSoftUpgFailedIndex_parameterInvalid = 18,
	TypeSoftUpgFailedIndex_writeRamdscFailed = 19,
	TypeSoftUpgFailedIndex_readRamdscFailed = 20,
	TypeSoftUpgFailedIndex_setUpgMibFailed = 21,
	TypeSoftUpgFailedIndex_getUpgMibFailed = 22,
	TypeSoftUpgFailedIndex_openUpgDirFailed = 23,
	TypeSoftUpgFailedIndex_createUpgDirFailed = 24,
	TypeSoftUpgFailedIndex_peerScOnline = 25,
	TypeSoftUpgFailedIndex_peerScOffline = 26,
	TypeSoftUpgFailedIndex_synRamdscFailed = 27,
	TypeSoftUpgFailedIndex_synFlashFailed = 28,
	TypeSoftUpgFailedIndex_synCfFailed = 29,
	TypeSoftUpgFailedIndex_msgCreateFailed = 30,
	TypeSoftUpgFailedIndex_msgSendFailed = 31,
	TypeSoftUpgFailedIndex_cmndStatusInvalid = 32,
	TypeSoftUpgFailedIndex_sscCopyTimeout = 33,
	TypeSoftUpgFailedIndex_softInvalid = 34,
	TypeSoftUpgFailedIndex_downRamFailed = 35,
	TypeSoftUpgFailedIndex_downCfFailed = 36,
	TypeSoftUpgFailedIndex_dirRnameLoadTmpFailed = 37,
	TypeSoftUpgFailedIndex_dirRnameTmpLoadFailed = 38,
	TypeSoftUpgFailedIndex_dirRnameTmpBkupFailed = 39,
	TypeSoftUpgFailedIndex_dirRnameBkupActFailed = 40,
	TypeSoftUpgFailedIndex_cfUnSurported = 41,
	TypeSoftUpgFailedIndex_ramUnSurported = 42,
	TypeSoftUpgFailedIndex_delDirFailed = 43,
	TypeSoftUpgFailedIndex_copyDirFailed = 44,
	TypeSoftUpgFailedIndex_copyFileFailed = 45,
	TypeSoftUpgFailedIndex_openFileFailed = 46,
	TypeSoftUpgFailedIndex_scSwitch = 47,
	TypeSoftUpgFailedIndex_srootFileInvalid = 48,
	TypeSoftUpgFailedIndex_crootFileInvalid = 49,
	TypeSoftUpgFailedIndex_lrootFileInvalid = 50,
	TypeSoftUpgFailedIndex_addHostServerFailed = 51,
	TypeSoftUpgFailedIndex_delHostServerFailed = 52,
	TypeSoftUpgFailedIndex_dirNameParseFailed = 53,
	TypeSoftUpgFailedIndex_softVersionInvalid = 54,
	TypeSoftUpgFailedIndex_createConnectFailed = 55,
	TypeSoftUpgFailedIndex_openDataPipeFailed = 56,
	TypeSoftUpgFailedIndex_headFileStructureInvalid = 57,
	TypeSoftUpgFailedIndex_fileTypeInvalid = 58,
	TypeSoftUpgFailedIndex_getReleaseDataFailed = 59,
	TypeSoftUpgFailedIndex_diskStatusInvalid = 60,
	TypeSoftUpgFailedIndex_diskSpaceNotEnough = 61,
	TypeSoftUpgFailedIndex_getFileSizeFailed = 62,
	TypeSoftUpgFailedIndex_createFileFailed = 63,
	TypeSoftUpgFailedIndex_writeFileFailed = 64,
	TypeSoftUpgFailedIndex_fileWithoutCrc = 65,
	TypeSoftUpgFailedIndex_getFileNameFailed = 66,
	TypeSoftUpgFailedIndex_crcBufferInvalid = 67,
	TypeSoftUpgFailedIndex_crcChecksumInvalid = 68,
	TypeSoftUpgFailedIndex_extractFileFailed = 69,
	TypeSoftUpgFailedIndex_getTransDataFailed = 70,
	TypeSoftUpgFailedIndex_writeTransDataFailed = 71,
	TypeSoftUpgFailedIndex_mfmCommandRunning = 72,
	TypeSoftUpgFailedIndex_cfaSoftCopying = 73,
	TypeSoftUpgFailedIndex_versionQueryFailed = 74,
	TypeSoftUpgFailedIndex_fileNotExiste = 75,
	TypeSoftUpgFailedIndex_ftpConnectionFailed = 101,
	TypeSoftUpgFailedIndex_ftpConnectionTimeout = 102,
	TypeSoftUpgFailedIndex_downloadTimeout = 103,
	TypeSoftUpgFailedIndex_downloadCanceled = 104,
	TypeSoftUpgFailedIndex_downloadFailed = 105,
	TypeSoftUpgFailedIndex_backupTimeout = 111,
	TypeSoftUpgFailedIndex_backupCanceled = 112,
	TypeSoftUpgFailedIndex_backupFailed = 113,
	TypeSoftUpgFailedIndex_synchronizeTimeout = 121,
	TypeSoftUpgFailedIndex_synchronizeCanceled = 122,
	TypeSoftUpgFailedIndex_synchronizeFailed = 123,
	TypeSoftUpgFailedIndex_switchTimeout = 131,
	TypeSoftUpgFailedIndex_switchFailed = 132,
	TypeSoftUpgFailedIndex_switchCfFailed = 133
} TypeSoftUpgFailedIndex_Tval;

typedef enum {
	PbEpgSwitchRequest_clear = 1,
	PbEpgSwitchRequest_lp = 2,
	PbEpgSwitchRequest_fs_w = 3,
	PbEpgSwitchRequest_fs_p = 4,
	PbEpgSwitchRequest_uf_p = 5,
	PbEpgSwitchRequest_uf_w = 6,
	PbEpgSwitchRequest_lis_w = 7,
	PbEpgSwitchRequest_lis_p = 8,
	PbEpgSwitchRequest_ms_w = 9,
	PbEpgSwitchRequest_ms_p = 10,
	PbEpgSwitchRequest_wtr = 11,
	PbEpgSwitchRequest_nr = 12
} PbEpgSwitchRequest_Tval;

typedef enum {
	PbTMSStClkType_twoMHz = 1,
	PbTMSStClkType_twoMbUnFramed = 2,
	PbTMSStClkType_twoMbFramed = 3
} PbTMSStClkType_Tval;

typedef enum {
	PbInSignalType_normal = 1,
	PbInSignalType_ais = 3
} PbInSignalType_Tval;

typedef enum {
	PbGmplsSncpDirection_forward = 1,
	PbGmplsSncpDirection_reverse = 2
} PbGmplsSncpDirection_Tval;

typedef enum {
	RprServiceCos_classC = 0,
	RprServiceCos_classBEir = 1,
	RprServiceCos_classBCir = 2,
	RprServiceCos_classA = 3
} RprServiceCos_Tval;

typedef enum {
	AlarmCategory_communication = 1,
	AlarmCategory_qos = 2,
	AlarmCategory_equipment = 3,
	AlarmCategory_processerror = 4,
	AlarmCategory_environment = 5,
	AlarmCategory_security = 6
} AlarmCategory_Tval;

typedef enum {
	RprXcRowStatus_free = 0,
	RprXcRowStatus_used = 1
} RprXcRowStatus_Tval;

typedef enum {
	PbUsageStatus_free = 0,
	PbUsageStatus_sdh_occupied = 1,
	PbUsageStatus_pdh_data_occupied = 2,
	PbUsageStatus_gmpls_reserved = 3,
	PbUsageStatus_unavailable = 4
} PbUsageStatus_Tval;

typedef enum {
	PbBoardStyle_empty = 0,
	PbBoardStyle_unknown = 1,
	PbBoardStyle_main = 2305,
	PbBoardStyle_hocc = 2306,
	PbBoardStyle_locc_pmc = 2307,
	PbBoardStyle_e1x126 = 2308,
	PbBoardStyle_e1px63 = 2309,
	PbBoardStyle_stm_1ex8 = 2310,
	PbBoardStyle_e3ds3x12 = 2312,
	PbBoardStyle_stm_41x8 = 2320,
	PbBoardStyle_stm_1x8 = 2321,
	PbBoardStyle_stm_16x4 = 2322,
	PbBoardStyle_stm_16x8 = 2323,
	PbBoardStyle_e3ds3px12 = 2324,
	PbBoardStyle_stm_64x2 = 2325,
	PbBoardStyle_stm_64x1 = 2326,
	PbBoardStyle_oaBooster13dbm = 2336,
	PbBoardStyle_oaBooster15dbm = 2337,
	PbBoardStyle_oaBooster18dbm = 2338,
	PbBoardStyle_oaPreamp20db = 2340,
	PbBoardStyle_tengetx1 = 2342,
	PbBoardStyle_tengetx1_geax46 = 2343,
	PbBoardStyle_getx8 = 2344,
	PbBoardStyle_gex6_fegeax4 = 2345,
	PbBoardStyle_gex2_ferprx12 = 2352,
	PbBoardStyle_gex4_feax12 = 2353,
	PbBoardStyle_fetx12 = 2354,
	PbBoardStyle_fc2gx8 = 2355,
	PbBoardStyle_locc = 2356,
	PbBoardStyle_stm_256x1 = 2357,
	PbBoardStyle_fan = 2358,
	PbBoardStyle_si_without_eow = 2368,
	PbBoardStyle_stm_16x2 = 2369,
	PbBoardStyle_otu_2x1 = 2400,
	PbBoardStyle_tengex1_gex10l2 = 2440,
	PbBoardStyle_stm_16x8m = 2441,
	PbBoardStyle_stm_41x16m = 2448,
	PbBoardStyle_stm_64x2m = 2449,
	PbBoardStyle_otu_2x2 = 2450,
	PbBoardStyle_chassis = 4232,
	PbBoardStyle_cc = 4240,
	PbBoardStyle_dataFabric_controller = 4241,
	PbBoardStyle_dataFabric = 4242,
	PbBoardStyle_si = 4243,
	PbBoardStyle_power = 4244,
	PbBoardStyle_chassis_fan_airFilter = 4245,
	PbBoardStyle_fanTray = 4246,
	PbBoardStyle_fanCircuitBoard = 4247,
	PbBoardStyle_backplane = 4248,
	PbBoardStyle_e1x21p_75 = 4352,
	PbBoardStyle_e1x21p_120 = 4353,
	PbBoardStyle_stm_1ex16 = 4354,
	PbBoardStyle_stm_1ex8p = 4356,
	PbBoardStyle_e3ds3x6p = 4358,
	PbBoardStyle_gex20 = 4359,
	PbBoardStyle_tengex2 = 4360,
	PbBoardStyle_gex10_eos = 4361,
	PbBoardStyle_tengex2_eos = 4368,
	PbBoardStyle_gmpls_load = 983040
} PbBoardStyle_Tval;

typedef enum {
	TypeUpgradeCmnd_inactive = 0,
	TypeUpgradeCmnd_activeScDownload = 1,
	TypeUpgradeCmnd_activeScSwitch = 2,
	TypeUpgradeCmnd_activeScBackup = 3,
	TypeUpgradeCmnd_synchronizeBackup = 4,
	TypeUpgradeCmnd_standbyScSwitch = 5
} TypeUpgradeCmnd_Tval;

typedef enum {
	PbMSPCurrentReq_reqlockout = 1,
	PbMSPCurrentReq_reqforcedswitch = 2,
	PbMSPCurrentReq_reqsfhigh = 3,
	PbMSPCurrentReq_reqsflow = 4,
	PbMSPCurrentReq_reqsdhigh = 5,
	PbMSPCurrentReq_reqsdlow = 6,
	PbMSPCurrentReq_reqmannualswitch = 7,
	PbMSPCurrentReq_reqwaittorestore = 8,
	PbMSPCurrentReq_reqexercise = 9,
	PbMSPCurrentReq_reqreverse = 10,
	PbMSPCurrentReq_reqnorevert = 11,
	PbMSPCurrentReq_reqnorequest = 12
} PbMSPCurrentReq_Tval;

typedef enum {
	PbEnableDisable_enable = 1,
	PbEnableDisable_disable = 2
} PbEnableDisable_Tval;

typedef enum {
	Mssp4fLineRequest_signalDegradeRing = 8,
	Mssp4fLineRequest_signalFailRing = 11,
	Mssp4fLineRequest_clear = 21
} Mssp4fLineRequest_Tval;

typedef enum {
	Mssp4fSwitchCommand_noRequest = 0,
	Mssp4fSwitchCommand_reverseRequestRing = 1,
	Mssp4fSwitchCommand_reverseRequestSpan = 2,
	Mssp4fSwitchCommand_exerciserRing = 3,
	Mssp4fSwitchCommand_exerciserSpan = 4,
	Mssp4fSwitchCommand_waitToRestore = 5,
	Mssp4fSwitchCommand_manualSwitchRing = 6,
	Mssp4fSwitchCommand_manualSwitchSpan = 7,
	Mssp4fSwitchCommand_signalDegradeRing = 8,
	Mssp4fSwitchCommand_signalDegradeSpan = 9,
	Mssp4fSwitchCommand_signalDegradeProtection = 10,
	Mssp4fSwitchCommand_signalFailRing = 11,
	Mssp4fSwitchCommand_signalFailSpan = 12,
	Mssp4fSwitchCommand_forcedSwitchRing = 13,
	Mssp4fSwitchCommand_forcedSwitchSpan = 14,
	Mssp4fSwitchCommand_lockoutProtection = 15
} Mssp4fSwitchCommand_Tval;

typedef enum {
	PbEthPortClass_lAN = 1,
	PbEthPortClass_wAN = 2,
	PbEthPortClass_tRUNK = 3
} PbEthPortClass_Tval;

typedef enum {
	AlarmState_set = 1,
	AlarmState_clear = 2
} AlarmState_Tval;

typedef enum {
	SnmpSecurityLevel_noAuthNoPriv = 1,
	SnmpSecurityLevel_authNoPriv = 2,
	SnmpSecurityLevel_authPriv = 3
} SnmpSecurityLevel_Tval;

typedef enum {
	PbMsspRequestSource_kByte = 0,
	PbMsspRequestSource_line = 1,
	PbMsspRequestSource_external = 2
} PbMsspRequestSource_Tval;

typedef enum {
	PbSNCPCurrentReq_reqClear = 1,
	PbSNCPCurrentReq_reqlockout = 2,
	PbSNCPCurrentReq_reqforcedswitchtop = 3,
	PbSNCPCurrentReq_reqforcedswitchtow = 4,
	PbSNCPCurrentReq_reqsignalfail = 5,
	PbSNCPCurrentReq_reqmannualswitchtop = 6,
	PbSNCPCurrentReq_reqmannualswitchtow = 7,
	PbSNCPCurrentReq_reqwaittorestore = 8,
	PbSNCPCurrentReq_reqnorequest = 9,
	PbSNCPCurrentReq_reqsignalDeg = 10
} PbSNCPCurrentReq_Tval;

typedef enum {
	PbXcStatus_removed = 0,
	PbXcStatus_disabled = 1,
	PbXcStatus_ok = 2,
	PbXcStatus_failed = 3
} PbXcStatus_Tval;

typedef enum {
	EventTypeID_usr_login = 1,
	EventTypeID_usr_logout = 2,
	EventTypeID_usr_autologout = 3,
	EventTypeID_usr_pwd_change = 4,
	EventTypeID_unauthorized_login = 5,
	EventTypeID_alarm_log_cleared = 6,
	EventTypeID_event_log_cleared = 7,
	EventTypeID_auto_acceptance = 8,
	EventTypeID_card_booted = 9,
	EventTypeID_card_reset = 10,
	EventTypeID_card_disabled = 11,
	EventTypeID_card_removed = 12,
	EventTypeID_sfp_changed = 13,
	EventTypeID_protection_switch_completed = 14,
	EventTypeID_protection_switch_back_completed = 15,
	EventTypeID_software_download_started = 16,
	EventTypeID_software_download_completed = 17,
	EventTypeID_software_upgrade_started = 18,
	EventTypeID_software_upgrade_completed = 19,
	EventTypeID_mib_upload_started = 20,
	EventTypeID_mib_upload_completed = 21,
	EventTypeID_mib_download_started = 22,
	EventTypeID_mib_download_completed = 23,
	EventTypeID_mib_backup_started = 24,
	EventTypeID_mib_backup_completed = 25,
	EventTypeID_mib_restore_started = 26,
	EventTypeID_mib_restore_completed = 27,
	EventTypeID_system_reset = 28,
	EventTypeID_system_start = 29,
	EventTypeID_tms_status = 30,
	EventTypeID_tms_config = 31,
	EventTypeID_port_down = 32,
	EventTypeID_port_up = 33,
	EventTypeID_msspring_state_change = 34,
	EventTypeID_msspring_squelch = 35,
	EventTypeID_cc_switch = 36,
	EventTypeID_tca = 37,
	EventTypeID_flash_full = 38,
	EventTypeID_dcc_dll_status = 39,
	EventTypeID_mib_check_failed = 40,
	EventTypeID_mib_illegal_operation = 41,
	EventTypeID_system_memory_usage = 42,
	EventTypeID_sncp_switch_to_protect = 43,
	EventTypeID_sncp_switch_to_work = 44,
	EventTypeID_card_deleted = 45,
	EventTypeID_system_time_changed = 46,
	EventTypeID_positive_bit_slip = 47,
	EventTypeID_negative_bit_slip = 48,
	EventTypeID_latch_switch_opened = 49,
	EventTypeID_sncp_switch = 50,
	EventTypeID_tms_systiming_status = 51,
	EventTypeID_tms_systiming_config = 52,
	EventTypeID_epg_protectstate_change = 53,
	EventTypeID_rpr_topo_changed = 54,
	EventTypeID_mib_clear_started = 57,
	EventTypeID_mib_clear_completed = 58,
	EventTypeID_boot_image_is_not_identical = 60,
	EventTypeID_rstp_topology_changed = 61,
	EventTypeID_rstp_enabled = 62,
	EventTypeID_rstp_disabled = 63,
	EventTypeID_lcasso_changed = 64,
	EventTypeID_lcassk_changed = 65,
	EventTypeID_mib_file_is_failed = 66,
	EventTypeID_free_space_is_not_enough_in_cf = 67,
	EventTypeID_space_is_released_in_cf = 68,
	EventTypeID_ntp_function_changed = 69,
	EventTypeID_connect_to_ntp_server_failed = 70,
	EventTypeID_connect_to_ntp_server_successfully = 71,
	EventTypeID_primary_load_copy_succeeded = 72,
	EventTypeID_primary_load_copy_failed = 73,
	EventTypeID_lan_port_down_on_csf_alarm = 74,
	EventTypeID_lan_port_recovered_on_csf_alarm_cleared = 75,
	EventTypeID_lan_port_down_on_tlct_alarm = 76,
	EventTypeID_lan_port_recovered_on_tlct_alarm_cleared = 77,
	EventTypeID_lan_port_recovered_on_atls_disable = 78,
	EventTypeID_provision_ssm_mode_changed = 79,
	EventTypeID_ecc_dll_status = 80,
	EventTypeID_software_image_loaded_location = 81,
	EventTypeID_mib_loaded_location = 82,
	EventTypeID_eow_software_download_started = 83,
	EventTypeID_eow_software_download_completed = 84,
	EventTypeID_eow_software_upgrade_started = 85,
	EventTypeID_eow_software_upgrade_completed = 86,
	EventTypeID_eow_box_present_status = 87,
	EventTypeID_eow_box_linkup_status = 88,
	EventTypeID_software_synchronization_started = 89,
	EventTypeID_software_synchronization_statement = 90,
	EventTypeID_mib_syn_started = 91,
	EventTypeID_mib_syn_statement = 92,
	EventTypeID_mib_copy_started = 93,
	EventTypeID_mib_copy_statement = 94,
	EventTypeID_card_mismatch = 95,
	EventTypeID_lcs_download_started = 96,
	EventTypeID_lcs_download_statement = 97,
	EventTypeID_lcs_syn_started = 98,
	EventTypeID_lcs_syn_statement = 99,
	EventTypeID_software_is_different = 101,
	EventTypeID_wr_error = 102,
	EventTypeID_card_inserted = 103,
	EventTypeID_sft_unstable_state = 104,
	EventTypeID_sft_is_switching = 105,
	EventTypeID_sft_switching_complete_by_present = 106,
	EventTypeID_sft_switching_complete_by_external_command = 107,
	EventTypeID_sft_switching_complete_by_unit_failure = 108,
	EventTypeID_sft_switching_complete_by_latch_open = 109,
	EventTypeID_sft_switching_failed = 110,
	EventTypeID_sft_file_lost = 111,
	EventTypeID_sft_ohw_error = 112,
	EventTypeID_sft_pull_ool = 113,
	EventTypeID_sft_gmpls_switch = 114,
	EventTypeID_ms_got_write_authority = 115,
	EventTypeID_ms_release_write_authority = 116,
	EventTypeID_unauthorized_access = 117,
	EventTypeID_si_ens_state = 118,
	EventTypeID_si_erm_state = 119,
	EventTypeID_mstp_port_change = 120,
	EventTypeID_mstp_rstp_enable = 121,
	EventTypeID_initialization_failure = 122,
	EventTypeID_ocxo_failure = 123,
	EventTypeID_vcxo_failure = 124,
	EventTypeID_operation_mode_switch = 125,
	EventTypeID_j0_mode_changed = 126,
	EventTypeID_j0_value_changed = 127,
	EventTypeID_boot_image_upgrade_completed = 128,
	EventTypeID_boot_image_upgrade_failure = 129,
	EventTypeID_pll_cannot_lock = 130,
	EventTypeID_user_enabled = 131,
	EventTypeID_user_disabled = 132,
	EventTypeID_lcs_status_changed = 133,
	EventTypeID_epg_switching_failure = 134,
	EventTypeID_rpr_hardware_passthrough_setup = 135,
	EventTypeID_rpr_hardware_passthrough_removed = 136,
	EventTypeID_account_expired = 137,
	EventTypeID_cpu_util_over = 138,
	EventTypeID_mem_util_over = 139,
	EventTypeID_gmpls_switch_success = 140,
	EventTypeID_queue_over_flow = 141,
	EventTypeID_dcn_mgmt_ethport_down = 142,
	EventTypeID_dcn_mgmt_ethport_up = 143,
	EventTypeID_mib_preserve_clear_started = 144,
	EventTypeID_mib_preserve_clear_completed = 145
} EventTypeID_Tval;

typedef enum {
	TmsRefSourceNum_reference1 = 1,
	TmsRefSourceNum_reference2 = 2,
	TmsRefSourceNum_reference3 = 3,
	TmsRefSourceNum_reference4 = 4
} TmsRefSourceNum_Tval;

typedef enum {
	PbSignalType_normal = 1,
	PbSignalType_unequipped = 2,
	PbSignalType_ais = 3,
	PbSignalType_rdi = 5
} PbSignalType_Tval;

typedef enum {
	PBSNCPAddRslt_succ = 1,
	PBSNCPAddRslt_workTPError = 2,
	PBSNCPAddRslt_workTPDuplicate = 3,
	PBSNCPAddRslt_protTPDuplicate = 4,
	PBSNCPAddRslt_protTPError = 5,
	PBSNCPAddRslt_msProtTP = 6,
	PBSNCPAddRslt_pcmTimeout = 7,
	PBSNCPAddRslt_noFreeTbl = 8,
	PBSNCPAddRslt_ccFail = 9,
	PBSNCPAddRslt_ccTimeout = 10,
	PBSNCPAddRslt_waitCCReq = 11,
	PBSNCPAddRslt_waitCCResult = 12,
	PBSNCPAddRslt_waitPCMResult = 13,
	PBSNCPAddRslt_pcmFail = 14,
	PBSNCPAddRslt_waitSPAReq = 15,
	PBSNCPAddRslt_waitSPAResult = 16,
	PBSNCPAddRslt_spaFail = 17,
	PBSNCPAddRslt_spaTimeout = 18
} PBSNCPAddRslt_Tval;

typedef enum {
	PbMsspTrafficDirection_rx = 0,
	PbMsspTrafficDirection_tx = 1
} PbMsspTrafficDirection_Tval;

typedef enum {
	PbCmndState_none = 1,
	PbCmndState_running = 2,
	PbCmndState_complete = 3,
	PbCmndState_failed = 4
} PbCmndState_Tval;

typedef enum {
	PbTIModeType_mode16Byte = 1,
	PbTIModeType_mode64Byte = 2,
	PbTIModeType_modeFixedOneByte = 3,
	PbTIModeType_modeProvisionedOneByte = 4,
	PbTIModeType_modeAutoTrace = 5
} PbTIModeType_Tval;

typedef enum {
	PbCFCardState_notapplicable = 0,
	PbCFCardState_mountedandnormal = 1,
	PbCFCardState_removed = 2,
	PbCFCardState_mountedbutfailed = 3,
	PbCFCardState_mountednofreespace = 4
} PbCFCardState_Tval;

typedef enum {
	PbSlotType_none = 0,
	PbSlotType_mc = 1,
	PbSlotType_lc = 2,
	PbSlotType_ti = 5,
	PbSlotType_to = 6,
	PbSlotType_power = 7,
	PbSlotType_fan = 8,
	PbSlotType_si = 10,
	PbSlotType_io = 12,
	PbSlotType_cc = 13,
	PbSlotType_sfc = 14,
	PbSlotType_sfm = 15,
	PbSlotType_ce = 16
} PbSlotType_Tval;

typedef enum {
	PbPmUapType_nearend = 1,
	PbPmUapType_farend = 2
} PbPmUapType_Tval;

typedef enum {
	PbSFFTranscCode_unknown = 0,
	PbSFFTranscCode_infiniband_1x_SX = 1,
	PbSFFTranscCode_infiniband_1x_LX = 2,
	PbSFFTranscCode_infiniband_1x_COP_ACT = 3,
	PbSFFTranscCode_infiniband_1x_COP_PSV = 4,
	PbSFFTranscCode_escon_MMF_1310NM_LED = 5,
	PbSFFTranscCode_escon_SMF_1310NM_LASER = 6,
	PbSFFTranscCode_sonet_oc48_SR = 7,
	PbSFFTranscCode_sonet_oc48_SR1 = 8,
	PbSFFTranscCode_sonet_oc48_IR = 9,
	PbSFFTranscCode_sonet_oc48_IR1 = 10,
	PbSFFTranscCode_sonet_oc48_IR2 = 11,
	PbSFFTranscCode_sonet_oc48_LR = 12,
	PbSFFTranscCode_sonet_oc48_LR1 = 13,
	PbSFFTranscCode_sonet_oc48_LR2 = 14,
	PbSFFTranscCode_sonet_oc48_LR3 = 15,
	PbSFFTranscCode_sonet_oc12_SR = 16,
	PbSFFTranscCode_sonet_oc12_SR1 = 17,
	PbSFFTranscCode_sonet_oc12_IR = 18,
	PbSFFTranscCode_sonet_oc12_IR1 = 19,
	PbSFFTranscCode_sonet_oc12_IR2 = 20,
	PbSFFTranscCode_sonet_oc12_LR = 21,
	PbSFFTranscCode_sonet_oc12_LR1 = 22,
	PbSFFTranscCode_sonet_oc12_LR2 = 23,
	PbSFFTranscCode_sonet_oc12_LR3 = 24,
	PbSFFTranscCode_sonet_oc3_SR = 25,
	PbSFFTranscCode_sonet_oc3_SR1 = 26,
	PbSFFTranscCode_sonet_oc3_IR = 27,
	PbSFFTranscCode_sonet_oc3_IR1 = 28,
	PbSFFTranscCode_sonet_oc3_IR2 = 29,
	PbSFFTranscCode_sonet_oc3_LR = 30,
	PbSFFTranscCode_sonet_oc3_LR1 = 31,
	PbSFFTranscCode_sonet_oc3_LR2 = 32,
	PbSFFTranscCode_sonet_oc3_LR3 = 33,
	PbSFFTranscCode_ethernet_BASE_PX = 34,
	PbSFFTranscCode_ethernet_BASE_BX10 = 35,
	PbSFFTranscCode_ethernet_100BASE_FX = 36,
	PbSFFTranscCode_ethernet_100BASE_LX_LX10 = 37,
	PbSFFTranscCode_ethernet_1000BASE_T = 38,
	PbSFFTranscCode_ethernet_1000BASE_CX = 39,
	PbSFFTranscCode_ethernet_1000BASE_LX = 40,
	PbSFFTranscCode_ethernet_1000BASE_SX = 41,
	PbSFFTranscCode_fibre_CHANNEL = 42,
	PbSFFTranscCode_max = 43
} PbSFFTranscCode_Tval;

typedef enum {
	StorageType_other = 1,
	StorageType_volatile = 2,
	StorageType_nonVolatile = 3,
	StorageType_permanent = 4,
	StorageType_readOnly = 5
} StorageType_Tval;

typedef enum {
	PbCommunityStringAccessLevel_readOnly = 1,
	PbCommunityStringAccessLevel_readWrite = 2
} PbCommunityStringAccessLevel_Tval;

typedef enum {
	PbDpnpSwitchEvent_w2p = 1,
	PbDpnpSwitchEvent_w2m = 2,
	PbDpnpSwitchEvent_p2w = 3,
	PbDpnpSwitchEvent_p2m = 4,
	PbDpnpSwitchEvent_m2w = 5,
	PbDpnpSwitchEvent_m2p = 6
} PbDpnpSwitchEvent_Tval;

typedef enum {
	PBSNCPWorkorProtToCC_protection = 0,
	PBSNCPWorkorProtToCC_working = 1,
	PBSNCPWorkorProtToCC_none = 255
} PBSNCPWorkorProtToCC_Tval;

typedef enum {
	PbTPType_ospiTTP = 1,
	PbTPType_rsDatacomCTP = 2,
	PbTPType_rsTTP = 3,
	PbTPType_msCTP = 4,
	PbTPType_msDatacomCTP = 5,
	PbTPType_msTTP = 6,
	PbTPType_au4CTP = 7,
	PbTPType_vc4TTP = 8,
	PbTPType_tu12CTP = 9,
	PbTPType_vc12TTP = 10,
	PbTPType_ppiTTP = 11,
	PbTPType_vc4xvTTP = 12,
	PbTPType_vc12xvTTP = 13,
	PbTPType_au416cCTP = 14,
	PbTPType_vc416cTTP = 15,
	PbTPType_au44cCTP = 16,
	PbTPType_vc44cTTP = 17,
	PbTPType_au3CTP = 18,
	PbTPType_tu3CTP = 19,
	PbTPType_hovc3TTP = 20,
	PbTPType_lovc3TTP = 21,
	PbTPType_tu11CTP = 22,
	PbTPType_vc11TTP = 23,
	PbTPType_lanTTP = 24,
	PbTPType_wanTTP = 25,
	PbTPType_au464cCTP = 26,
	PbTPType_vc464cTTP = 27,
	PbTPType_tcCTP = 28,
	PbTPType_vpCTP = 29,
	PbTPType_vcCTP = 30,
	PbTPType_vc3xvTTP = 31,
	PbTPType_vc4NIMTTP = 32,
	PbTPType_vc12NIMTTP = 33,
	PbTPType_vc416cNIMTTP = 34,
	PbTPType_vc44cNIMTTP = 35,
	PbTPType_hovc3NIMTTP = 36,
	PbTPType_lovc3NIMTTP = 37,
	PbTPType_vc11NIMTTP = 38,
	PbTPType_vc464cNIMTTP = 39,
	PbTPType_au4256cCTP = 40,
	PbTPType_vc4256cTTP = 41,
	PbTPType_vc4256cNIMTTP = 42
} PbTPType_Tval;

typedef enum {
	PbMSPExternalCmd_cmdClear = 1,
	PbMSPExternalCmd_cmdlockout = 2,
	PbMSPExternalCmd_cmdforcedswitch = 3,
	PbMSPExternalCmd_cmdmannualswitch = 4,
	PbMSPExternalCmd_cmdexercise = 5,
	PbMSPExternalCmd_cmdnocomand = 6
} PbMSPExternalCmd_Tval;

typedef enum {
	TMSStClkOhm_seventyfive = 1,
	TMSStClkOhm_onehundredtwenty = 2
} TMSStClkOhm_Tval;

typedef enum {
	PbPmTableType_interval = 1,
	PbPmTableType_intervalFe = 2,
	PbPmTableType_day = 3,
	PbPmTableType_dayFe = 4
} PbPmTableType_Tval;

typedef enum {
	PbPortType_lan = 1,
	PbPortType_console = 2,
	PbPortType_stm1 = 3,
	PbPortType_stm4 = 4,
	PbPortType_stm16 = 5,
	PbPortType_stm64 = 6,
	PbPortType_fe = 7,
	PbPortType_ge = 8,
	PbPortType_e1 = 9,
	PbPortType_e3ds3 = 10,
	PbPortType_ti = 11,
	PbPortType_to = 12,
	PbPortType_vstm1 = 13,
	PbPortType_vstm4 = 14,
	PbPortType_vstm16 = 15,
	PbPortType_vlan = 16,
	PbPortType_vwan = 17,
	PbPortType_e1ds1 = 18,
	PbPortType_ivstm1 = 19,
	PbPortType_rprclient = 20,
	PbPortType_rprline = 21,
	PbPortType_oa = 22,
	PbPortType_stm1or4 = 23,
	PbPortType_stm256 = 24,
	PbPortType_tenge = 25,
	PbPortType_fiber_channel = 26,
	PbPortType_vstm64 = 27,
	PbPortType_otu2 = 28,
	PbPortType_na = 29
} PbPortType_Tval;

typedef enum {
	PbSncpCircuitDirection_bidirection = 1,
	PbSncpCircuitDirection_unidirection = 2
} PbSncpCircuitDirection_Tval;

typedef enum {
	PbPmPortStatusType_normal = 1,
	PbPmPortStatusType_working = 2,
	PbPmPortStatusType_protect = 3
} PbPmPortStatusType_Tval;

typedef enum {
	PbTpMaintenanceType_normal = 1,
	PbTpMaintenanceType_lck = 5,
	PbTpMaintenanceType_oci = 6,
	PbTpMaintenanceType_ais = 7
} PbTpMaintenanceType_Tval;

typedef enum {
	PbMSPSwitchReason_clearcmd = 1,
	PbMSPSwitchReason_lpcmd = 2,
	PbMSPSwitchReason_forcedswitch = 3,
	PbMSPSwitchReason_manualswitch = 4,
	PbMSPSwitchReason_signalfail = 5,
	PbMSPSwitchReason_signalfailclear = 6,
	PbMSPSwitchReason_signaldeg = 7,
	PbMSPSwitchReason_signaldegclear = 8,
	PbMSPSwitchReason_noswitch = 9
} PbMSPSwitchReason_Tval;

typedef enum {
	PbGmplsCcDirection_forward = 1,
	PbGmplsCcDirection_reverse = 2
} PbGmplsCcDirection_Tval;

typedef enum {
	EthPortClass_lan = 0,
	EthPortClass_wan = 1,
	EthPortClass_trunk = 2
} EthPortClass_Tval;

typedef enum {
	TypeMibCmnd_inactive = 0,
	TypeMibCmnd_activeScMibUpload = 1,
	TypeMibCmnd_activeScMibDownload = 2,
	TypeMibCmnd_activeScMibSwitch = 3,
	TypeMibCmnd_standbyScMibSwitch = 4,
	TypeMibCmnd_synchronizeMibBackup = 5,
	TypeMibCmnd_activeScMibClear = 6,
	TypeMibCmnd_activeScMibCopy = 7,
	TypeMibCmnd_downloadLicense = 8,
	TypeMibCmnd_synchronizeLicense = 9,
	TypeMibCmnd_activeScMibPreserveClear = 10
} TypeMibCmnd_Tval;

typedef enum {
	RowStatus_active = 1,
	RowStatus_notInService = 2,
	RowStatus_notReady = 3,
	RowStatus_createAndGo = 4,
	RowStatus_createAndWait = 5,
	RowStatus_destroy = 6
} RowStatus_Tval;

typedef enum {
	PbSNCPExternalCmd_cmdClear = 1,
	PbSNCPExternalCmd_cmdlockout = 2,
	PbSNCPExternalCmd_cmdforcedswitchtop = 3,
	PbSNCPExternalCmd_cmdforcedswitchtow = 4,
	PbSNCPExternalCmd_cmdmannualswitchtop = 5,
	PbSNCPExternalCmd_cmdmannualswitchtow = 6,
	PbSNCPExternalCmd_cmdnocomand = 7
} PbSNCPExternalCmd_Tval;

typedef enum {
	RprProtectionType_steering = 1,
	RprProtectionType_wrap = 2,
	RprProtectionType_disable = 3
} RprProtectionType_Tval;

typedef enum {
	PbNodeMode_standalone = 1,
	PbNodeMode_subservient = 2
} PbNodeMode_Tval;

typedef enum {
	PbAcDc_ac = 1,
	PbAcDc_dc = 2
} PbAcDc_Tval;

typedef enum {
	PbMsspRequest_noRequest = 0,
	PbMsspRequest_reverseRequestRing = 1,
	PbMsspRequest_reverseRequestSpan = 2,
	PbMsspRequest_exerciserRing = 3,
	PbMsspRequest_exerciserSpan = 4,
	PbMsspRequest_waitToRestore = 5,
	PbMsspRequest_manualSwitchRing = 6,
	PbMsspRequest_manualSwitchSpan = 7,
	PbMsspRequest_signalDegradeRing = 8,
	PbMsspRequest_signalDegradeSpan = 9,
	PbMsspRequest_signalDegradeProtection = 10,
	PbMsspRequest_signalFailRing = 11,
	PbMsspRequest_signalFailSpan = 12,
	PbMsspRequest_forcedSwitchRing = 13,
	PbMsspRequest_forcedSwitchSpan = 14,
	PbMsspRequest_lockoutProtection = 15
} PbMsspRequest_Tval;

typedef enum {
	PbConnectorType_unknown = 0,
	PbConnectorType_sc = 1,
	PbConnectorType_fiber_1_copper = 2,
	PbConnectorType_fiber_2_copper = 3,
	PbConnectorType_bnctnc = 4,
	PbConnectorType_fiber_coax = 5,
	PbConnectorType_fiber_jack = 6,
	PbConnectorType_lc = 7,
	PbConnectorType_mtrj = 8,
	PbConnectorType_mu = 9,
	PbConnectorType_sg = 10,
	PbConnectorType_optical_pigtail = 11,
	PbConnectorType_hssdc = 32,
	PbConnectorType_copper_pigtail = 33,
	PbConnectorType_unspecified = 256
} PbConnectorType_Tval;

typedef enum {
	PbBoardProperty_none = 0,
	PbBoardProperty_all = 1,
	PbBoardProperty_hiT7065sc = 2,
	PbBoardProperty_hiT7065cc = 4,
	PbBoardProperty_hiT7065lc = 8
} PbBoardProperty_Tval;

 
# 26 "../../../appl/CTD/inc/ctd_if.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_sonet.h" 1



 




 



 
extern MIBDid_t pbSonetSdh_Did;

   


 



 
 
extern MIBDid_t pbPortConfigTable_Did;

   
typedef enum {
	pbPortMode_auto=1,
	pbPortMode_mon=2,
	pbPortMode_nmon=3
} pbPortMode_Tval;

typedef enum {
	pbPdhPortMap_async=1,
	pbPdhPortMap_byte=2,
	pbPdhPortMap_none=3
} pbPdhPortMap_Tval;

typedef enum {
	pbPortLoopback_none=1,
	pbPortLoopback_outward=2,
	pbPortLoopback_inward=3
} pbPortLoopback_Tval;

typedef enum {
	pbFecMode_correction=1,
	pbFecMode_disable=2
} pbFecMode_Tval;

typedef enum {
	pbImpedance_ohm75=0,
	pbImpedance_ohm120=1,
	pbImpedance_ohm100=2,
	pbImpedance_ohm110=3
} pbImpedance_Tval;

typedef enum {
	pbCodeType_hdb3=0,
	pbCodeType_ami=1,
	pbCodeType_bypass=2,
	pbCodeType_cdrbypass=3,
	pbCodeType_b3zs=4
} pbCodeType_Tval;

typedef enum {
	pbHaulType_e175ohm=0,
	pbHaulType_e1120ohm=1,
	pbHaulType_dsx1133ft=2,
	pbHaulType_dsx1266ft=3,
	pbHaulType_dsx1399ft=4,
	pbHaulType_dsx1533ft=5,
	pbHaulType_dsx1655ft=6,
	pbHaulType_j10655ft=7,
	pbHaulType_ds10=8,
	pbHaulType_ds175=9,
	pbHaulType_ds115=10,
	pbHaulType_ds1225=11,
	pbHaulType_ds111xx=12
} pbHaulType_Tval;

typedef enum {
	pbRetimingStatus_disable=0,
	pbRetimingStatus_enable=1
} pbRetimingStatus_Tval;

typedef enum {
	pbPortAlsMode_no_command=0,
	pbPortAlsMode_manual_restart=1,
	pbPortAlsMode_manual_test=2
} pbPortAlsMode_Tval;

typedef enum {
	expectedFrameStructure_none=0,
	expectedFrameStructure_e1_framed=1,
	expectedFrameStructure_e1_unframed=2,
	expectedFrameStructure_e3_G751=3,
	expectedFrameStructure_e3_G832=4,
	expectedFrameStructure_ds3_M13=5,
	expectedFrameStructure_ds3_CBit=6,
	expectedFrameStructure_e3_unframed=7,
	expectedFrameStructure_ds3_unframed=8
} expectedFrameStructure_Tval;

typedef enum {
	crcMultiframeDetector_activated=1,
	crcMultiframeDetector_deactivated=2
} crcMultiframeDetector_Tval;

typedef enum {
	portClassification_nongmpls=0,
	portClassification_inni=1,
	portClassification_enni=2,
	portClassification_uni10=3,
	portClassification_uni20=4
} portClassification_Tval;

typedef enum {
	portOperationLock_none=1,
	portOperationLock_nms=2,
	portOperationLock_gmpls=3
} portOperationLock_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	PbPortType_Tval pbPortType;                                          
	int		pbPortNo;                                                       
	char		pbPortLabel[64];   
	pbPortMode_Tval pbPortMode;   
	pbPdhPortMap_Tval pbPdhPortMap;   
	PbPortStatus_Tval pbPortAdminStatus;   
	PbEnableDisable_Tval pbPortMapRefresh;   
	TruthValue_Tval pbPortAutoLaserShutdown;   
	pbPortLoopback_Tval pbPortLoopback;   
	pbFecMode_Tval pbFecMode;   
	PbActualPortType_Tval pbActualPortType;   
	pbImpedance_Tval pbImpedance;   
	pbCodeType_Tval pbCodeType;   
	pbHaulType_Tval pbHaulType;   
	pbRetimingStatus_Tval pbRetimingStatus;   
	pbPortAlsMode_Tval pbPortAlsMode;   
	TruthValue_Tval pbPortAlarmDisable;   
	expectedFrameStructure_Tval expectedFrameStructure;
	crcMultiframeDetector_Tval crcMultiframeDetector;
	portClassification_Tval portClassification;
	PbUsageStatus_Tval portUsageStatus;
	int		portMappingString_len;
	char		portMappingString[384];
	portOperationLock_Tval portOperationLock;
}MIBpbPortConfigTable_t;































 



 
 
extern MIBDid_t pbPortStatusTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	PbPortType_Tval pbPortType;                               
	int		pbPortNo;                                            
	int		pbPortStatus;   
	PbPortOperationStatus_Tval pbPortOperStatus;   
	int		pbLaserIBias;   
	int		pbLaserRecvPwr;   
	int		pbLaserTransPwr;   
	int		pbLaserTemp;   
	TruthValue_Tval pbCompliance8472;   
	int		pbnNominalMBps;   
	int		pbnWavelength;   
	int		pbnLinkLength9125;   
	int		pbnLinkLength50125;   
	int		pbnLinkLength62125;   
	int		pbnLinkLengthCopper;   
	char		pbsVendorName[20];   
	char		pbsVendorPN[20];   
	char		pbsVendorSN[20];   
	char		pbsManuDate[16];   
	PbSFFTranscCode_Tval pbnTransCode;   
	char		pbsVendorOUI[16];   
	PbConnectorType_Tval pbnConnectorType;   
	int		pbnClLaserTemp;
	int		pbnTECCurrent;
	char		pbsSAGPartNumber[20];
}MIBpbPortStatusTable_t;

































 



 
 
extern MIBDid_t pbPortHoTpReserveTable_Did;

   
typedef enum {
	tpOperationLock_none=1,
	tpOperationLock_nms=2,
	tpOperationLock_gmpls=3
} tpOperationLock_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                           
	PbPortType_Tval pbPortType;                                   
	int		pbPortNo;                                                
	int		vc4ReservationStatus_len;
	char		vc4ReservationStatus[256];   
	int		vc4ReservationOperate_len;
	char		vc4ReservationOperate[257];   
	tpOperationLock_Tval tpOperationLock;   
}MIBpbPortHoTpReserveTable_t;













 



 
 
extern MIBDid_t pbTPConfigTable_Did;

   
typedef enum {
	pbSdMode_poisson=1,
	pbSdMode_bursty=2
} pbSdMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	PbPortType_Tval pbPortType;                                       
	int		pbPortNo;                                                    
	PbTPType_Tval pbTpType;                                              
	int		pbTpIndex;                                                      
	TruthValue_Tval pbMonitored;   
	PbSignalType_Tval pbOutSignalType;
	PbInSignalType_Tval pbInSignalType;   
	int		pbTransPsl;   
	int		pbExpPsl;   
	TruthValue_Tval pbSLEnable;   
	int		pbSLAccept;   
	TruthValue_Tval pbTtiEnabled;   
	TruthValue_Tval pbTimAisDis;
	PbTIModeType_Tval pbTtiTransmitMode;   
	PbTIModeType_Tval pbTtiReceiveMode;   
	int		pbTtiOctetTransmitted_len;
	char		pbTtiOctetTransmitted[65];   
	int		pbTtiOctetExpected_len;
	char		pbTtiOctetExpected[65];   
	int		pbTtiOctetAccepted_len;
	char		pbTtiOctetAccepted[65];   
	pbSdMode_Tval pbSdMode;   
	TruthValue_Tval pbBurstyEnable;   
	int		pbBurstyPeriod;   
	int		pbBurstyPercent;   
	TruthValue_Tval pbDegEnable;   
	int		pbDegThreshold;   
	TruthValue_Tval pbExcEnable;   
	int		pbExcThreshold;   
	TruthValue_Tval pbAISEnable;   
	TruthValue_Tval pbRDIEnable;   
	PbPmMonitor_Tval pmNearEnd15minMonitored;
	PbPmMonitor_Tval pmFarEnd15minMonitored;
	PbPmMonitor_Tval pmNearEnd24hrMonitored;
	PbPmMonitor_Tval pmFarEnd24hrMonitored;
}MIBpbTPConfigTable_t;








































 



 
 
extern MIBDid_t pbOTUPortConfigTable_Did;

   
typedef enum {
	pbOTUPortMode_auto=1,
	pbOTUPortMode_mon=2,
	pbOTUPortMode_nmon=3
} pbOTUPortMode_Tval;

typedef enum {
	pbOTUPortLoopback_none=1,
	pbOTUPortLoopback_outward=2,
	pbOTUPortLoopback_inward=3
} pbOTUPortLoopback_Tval;

typedef enum {
	pbInsFecErrRate_errE3=3,
	pbInsFecErrRate_errE4=4,
	pbInsFecErrRate_errE5=5
} pbInsFecErrRate_Tval;

typedef enum {
	pbOTUFecMode_rsfec=0,
	pbOTUFecMode_ufec=1
} pbOTUFecMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                       
	PbPortType_Tval pbPortType;                                               
	int		pbPortNo;                                                            
	char		pbOTUPortLabel[65];   
	pbOTUPortMode_Tval pbOTUPortMode;   
	TruthValue_Tval pbOTUPortAlarmDisable;   
	pbOTUPortLoopback_Tval pbOTUPortLoopback;   
	TruthValue_Tval pbFecEnable;   
	TruthValue_Tval pbInsFecErrEnable;   
	pbInsFecErrRate_Tval pbInsFecErrRate;   
	int		pbWaveLengthId;   
	TruthValue_Tval pbSMTimAlmEnable;   
	TruthValue_Tval pbSMTimActEnable;   
	int		pbSMTxTti_len;
	char		pbSMTxTti[65];   
	int		pbSMExpTti_len;
	char		pbSMExpTti[65];   
	TruthValue_Tval pbPTAlmEnable;   
	int		pbTxPt;   
	int		pbExpPt;   
	TruthValue_Tval pbCIDAlmEnable;   
	int		pbTxChannelId;   
	TruthValue_Tval pbAutoAdjustManulStopEnable;   
	int		pbPortPathMapping_len;
	char		pbPortPathMapping[9];   
	pbOTUFecMode_Tval pbOTUFecMode;   
	PbPortStatus_Tval pbOTUPortAdminStatus;   
	PbUsageStatus_Tval pbOTUPortUsageStatus;   
}MIBpbOTUPortConfigTable_t;
































 



 
 
extern MIBDid_t pbPRBSPortHisTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	int		pbPortNo;                                                    
	PbPortType_Tval pbPortType;                                       
	unsigned int	pbPRBSHisErrorSeconds;   
	int		pbPRBSHisMeanError;
	char		pbPRBSTrapTimeStamp[15];   
	PbActualPortType_Tval pbPRBSHisActualPortType;   
}MIBpbPRBSPortHisTable_t;














 



 
 
extern MIBDid_t pbPRBSPortCurTable_Did;

   
typedef enum {
	pbPRBSPattern_na=0,
	pbPRBSPattern_power15=1,
	pbPRBSPattern_power20=2,
	pbPRBSPattern_power23=3
} pbPRBSPattern_Tval;

typedef enum {
	pbPRBSStatus_na=0,
	pbPRBSStatus_syncing=1,
	pbPRBSStatus_syncSucc=2,
	pbPRBSStatus_syncFail=3,
	pbPRBSStatus_syncError=4
} pbPRBSStatus_Tval;

typedef enum {
	prbsMode_normal=0,
	prbsMode_invert=1
} prbsMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                         
	int		pbPortNo;                                              
	PbPortType_Tval pbPortType;                                 
	TruthValue_Tval pbPRBSEnable;   
	pbPRBSPattern_Tval pbPRBSPattern;   
	unsigned int	pbPRBSErrorBlocks;   
	unsigned int	pbPRBSErrorSeconds;   
	pbPRBSStatus_Tval pbPRBSStatus;   
	PbActualPortType_Tval pbPRBSActualPortType;   
	prbsMode_Tval prbsMode;   
}MIBpbPRBSPortCurTable_t;

















 



 
 
extern MIBDid_t pbOTUPortStatusTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;           
	PbPortType_Tval pbPortType;                   
	int		pbPortNo;                                
	int		pbSMRxTti_len;
	char		pbSMRxTti[65];   
	int		pbRxPt;   
	int		pbRxChannelId;   
}MIBpbOTUPortStatusTable_t;













 



 
 
extern MIBDid_t pbOTUTPConfigTable_Did;

   
typedef enum {
	pbOTUTpType_none=0,
	pbOTUTpType_otu1=1,
	pbOTUTpType_otu2=2,
	pbOTUTpType_odu21=3,
	pbOTUTpType_otu3=4,
	pbOTUTpType_odu31=5,
	pbOTUTpType_odu32=6,
	pbOTUTpType_odu321=7,
	pbOTUTpType_otu4=8,
	pbOTUTpType_odu41=9,
	pbOTUTpType_odu42=10,
	pbOTUTpType_odu421=11,
	pbOTUTpType_odu43=12,
	pbOTUTpType_odu431=13,
	pbOTUTpType_odu432=14,
	pbOTUTpType_odu4321=15
} pbOTUTpType_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                     
	PbPortType_Tval pbPortType;                                             
	int		pbPortNo;                                                          
	int		pbOTUTpChannelId;                                                     
	PbTpSegment_Tval pbOTUTpSegment;                                           
	pbOTUTpType_Tval pbOTUTpType;   
	TruthValue_Tval pbOTUMonitored;   
	TruthValue_Tval pbOTUTimAlmEnable;   
	TruthValue_Tval pbOTUTimActEnable;   
	int		pbOTUTxTti_len;
	char		pbOTUTxTti[65];   
	int		pbOTUExpTti_len;
	char		pbOTUExpTti[65];   
	int		pbOTURxTti_len;
	char		pbOTURxTti[65];   
	TruthValue_Tval pbOTUDegEnable;   
	int		pbOTUDegThreshold;   
	int		pbOTUDegIntervals;   
	PbTpMaintenanceType_Tval pbOTUSendMaintenanceType;   
	PbTpMaintenanceType_Tval pbOTURecvMaintenanceType;   
	TruthValue_Tval pbOTUPlmAlmEnable;   
	TruthValue_Tval pbOTUMsimAlmEnable;   
	TruthValue_Tval pbOTUPtAlmEnable;   
	int		pbOTUTxPt;   
	int		pbOTUExpPt;   
	int		pbOTURxPt;   
	PbPmMonitor_Tval pmOTUNearEnd15minMonitored;   
	PbPmMonitor_Tval pmOTUFarEnd15minMonitored;   
	PbPmMonitor_Tval pmOTUNearEnd24hrMonitored;   
	PbPmMonitor_Tval pmOTUFarEnd24hrMonitored;   
}MIBpbOTUTPConfigTable_t;


































 



 
extern MIBDid_t pbSonetModule_Did;

   



# 27 "../../../appl/CTD/inc/ctd_if.h" 2













 



typedef struct
{
        UINT32  ulPortId;       
        PbPortType_Tval ulPortType;

         

        UINT32  ulTug3Num;
        
        UINT16 ausTug3Mapping[192 ];
}PORT_TUG3_MAPPING_T;  

 
typedef struct
{
        UINT32  ulPortId;       
        PbPortType_Tval ulPortType;

        UINT32  ulTug3Num;

        UINT16 ausTug3Mapping[1];    
}MSG_PORT_TUG3_MAPPING_T;  

 






















 
















 














 





















typedef struct
{
        UINT32  nPortId;     

         
        PbPortOperationStatus_Tval eLaserOn;    

 
        
        int         nLaserIBias;
        int         nLaserRecvPwr;
        int         nLaserTransPwr;
        int         nLaserTemp;

  

	TruthValue_Tval pbCompliance8472;   
	int		pbnNominalMBps;   
	int		pbnWavelength;   
	int		pbnLinkLength9125;   
	int		pbnLinkLength50125;   
	int		pbnLinkLength62125;   
	int		pbnLinkLengthCopper;   
	char		pbsVendorName[20];   
	char		pbsVendorPN[20];   
	char		pbsVendorSN[20];   
	char		pbsManuDate[16];   
	PbSFFTranscCode_Tval pbnTransCode;   
	char		pbsVendorOUI[16];   
	PbConnectorType_Tval pbnConnectorType;  
    int     pbnClLaserTemp;  
    int     pbnTECCurrent;   
    char    pbsSagPN[20];  
}__attribute__ ((packed))  PORT_STATUS_T;  

 


 



























typedef struct
{
    UINT8  slotNo;
    UINT8  portType;
    UINT8  portNo;
    UINT16 mappingStrLen;
    UINT8  mappingStr[0];
}__attribute__ ((packed))  PORT_MAPPING_T;

typedef struct
{
    UINT8  slotNo;
    UINT8  portType;
    UINT8  portNo;
    UINT16 mappingStrLen;    
}__attribute__ ((packed))  CTD_MAPPING_T;

 






 














typedef struct
{
    UINT32 SlotNo;
    UINT32 BoardStyle;
}__attribute__ ((packed))  SLOT_ATTRIB;

typedef struct
{
    UINT32 PortNo_Min;  
	UINT32 PortNo_Max;  
}__attribute__ ((packed))  PORT_RANGE;

typedef struct
{
    SLOT_ATTRIB slotAttrib[13];  
}__attribute__ ((packed))  PdhCardFuncToIoSuit;

typedef struct
{
    SLOT_ATTRIB slotAttrib[2]; 
    PORT_RANGE portRange;
}__attribute__ ((packed))  PdhCardIoToFuncSuit;

typedef struct
{
    UINT32 number;             
    SLOT_ATTRIB cardInfo[12];   
    PORT_RANGE portRange[12];   
}__attribute__ ((packed))  PdhCardRelation;

 
# 288 "../../../appl/CTD/inc/ctd_if.h"


typedef struct
{    
    UINT8  portType;
    UINT8  portNo;
    UINT8  actualPortType;
    UINT16 mappingStrLen;
     
}__attribute__ ((packed))  ONE_PORT_MAPPING_T;

typedef struct
{
    UINT8 ucSlotNo;
    UINT8 ucPortNum;
     
}__attribute__ ((packed))  ONE_SLOT_MAPPING_T;

typedef struct
{
    UINT8 ucSlotNum;
         
}__attribute__ ((packed))  NE_MAPPING_T;

 


 

 
typedef void (*funcDCCPortStateChangeNew)(UINT32 inst, int port, int state);

PB_ERROR_CODE_T CTD_GetPdhIoCardInfoFromFuncCard(UINT32 funcCardSlotNo, UINT32 funcCardBoardStlye, PdhCardRelation *pPdhRelatedCard);
PB_ERROR_CODE_T CTD_GetPdhFuncCardInfoFromIoCard(UINT32 ioCardSlotNo, UINT32 ioCardBoardStlye, PdhCardRelation *pPdhRelatedCard);


PB_ERROR_CODE_T CTD_ifm_clrNeMappingSeachFlag(NE_MAPPING_T* pNeMapping);
PB_ERROR_CODE_T CTD_ifm_GetNextMappingfromNe(NE_MAPPING_T* pNeMapping, UINT32* pSlotNo, UINT32* pPortType, UINT32* pPortNo, UINT32 *pMappingStrLen,
                                 UINT8 *pMappingStr);
PB_ERROR_CODE_T CTD_ifm_clrSlotMappingSeachFlag(ONE_SLOT_MAPPING_T* pSlotMapping);
PB_ERROR_CODE_T CTD_ifm_GetNextMappingfromSlot(ONE_SLOT_MAPPING_T* pSlotMapping, UINT32* pSlotNo, UINT32* pPortType, UINT32* pPortNo, UINT32 *pMappingStrLen,
                                 UINT8 *pMappingStr);
PB_ERROR_CODE_T CTD_ifm_GetSlotNumberFromNeMapping(NE_MAPPING_T *pNeMapping, UINT32 *pSlotNumber);
PB_ERROR_CODE_T CTD_ifm_GetPortNumberFromNeMapping(UINT32 slotNo, NE_MAPPING_T *pNeMapping, UINT32 *pPortNumber);
PB_ERROR_CODE_T CTD_ifm_GetPortMappingFromNeMapping(UINT32 slotNo, UINT32 portType, UINT32 portNo, NE_MAPPING_T *pNeMapping,  
                              UINT32 *pPortMappingLen, UINT8 *pMappingStr);
PB_ERROR_CODE_T CTD_ifm_GetPortNumberFromSlotMapping(ONE_SLOT_MAPPING_T *pSlotMapping,UINT32 *pSlotNo, UINT32 *pPortNumber);
PB_ERROR_CODE_T CTD_ifm_GetPortMappingFromSlotMapping(UINT32 slotNo, UINT32 portType, UINT32 portNo, ONE_SLOT_MAPPING_T *pSlotMapping,  
                              UINT32 *pPortMappingLen, UINT8 *pMappingStr);











 




























































































# 24 "../../../appl/CTD/inc/ctd_sdh.h" 2

# 1 "../../../appl/CTD/inc/ctd_sd.h" 1
 
































typedef INT16 TEMPERATURE_T;

typedef enum {
  TEMPERATURE_STATE_NORMAL,
  TEMPERATURE_STATE_OVERHEAT,
  TEMPERATURE_STATE_UNDERHEAT
} TEMPERATURE_STATE_T;

typedef enum
{
  LED_TYPE_CRITICAL=0,
  LED_TYPE_MAJOR=1,
  LED_TYPE_MINOR=2,
  LED_TYPE_ACTIVE=3,
  LED_TYPE_FAULTY=4,
  LED_TYPE_FAN=5,
  LED_TYPE_POWER=6,
  LED_TYPE_PORTLINK=7,
  LED_TYPE_PORTALARM=8,
  LED_TYPE_CARDMISMATCH=9,
  LED_TYPE_STANDBY=10,
  LED_TYPE_CFCARD=11,
  LED_TYPE_USRLINK=12
} LED_TYPE_T;

typedef enum
{
  LED_STATE_UNKNOWN = -1,  
  LED_STATE_ON = 0,
  LED_STATE_OFF = 1,
  LED_STATE_BLINK = 2,
} LED_STATE_T;

typedef int BOARD_STYLE_T;

typedef enum {
  SDD_BOARD_ABSENT = 0,
  SDD_BOARD_PRESENT = 1,
  SDD_BOARD_UNKNOWN = 2
} BOARD_PRESENCE_T;

typedef enum {
  SDD_REBOOT_STATE_START    = 0,
  SDD_REBOOT_STATE_FINISH   = 1,
  SDD_REBOOT_STATE_UNKNOWN  = 2
} SDD_REBOOT_STATE_T;

typedef enum
{
  BOARD_EVENT_REMOVE = 0,
  BOARD_EVENT_INSERT = 1,
  BOARD_EVENT_LATCH_ATTACH = 2,
  BOARD_EVENT_LATCH_DETATCH = 3
} BOARD_EVENT_T;

 
typedef enum
{
    SDD_RESET_UNKNOWN       = 0,
    SDD_RESET_WARM   = 1,    
    SDD_RESET_COLD         = 2,    
    SDD_RESET_NE_WARM    = 3,    
    SDD_RESET_NE_COLD      = 4,     
    SDD_RESET_BOARD_PLUG	= 5		 
     
} SDD_RESET_REASON;

 
typedef enum
{
    SDD_BOARD_TYPE_EMPTY        = 0,
    SDD_BOARD_TYPE_MC           = 1,
    SDD_BOARD_TYPE_CC           = 2,
    SDD_BOARD_TYPE_CHASSIS      = 3,
    SDD_BOARD_TYPE_8STM16       = 4,
    SDD_BOARD_TYPE_8STM14       = 5,
    SDD_BOARD_TYPE_2STM64       = 6,
    SDD_BOARD_TYPE_8GET         = 7,
    SDD_BOARD_TYPE_10GEA        = 8,
    SDD_BOARD_TYPE_10GET        = 9,
    SDD_BOARD_TYPE_RPR          = 10,
    SDD_BOARD_TYPE_8STM1E       = 11,
    SDD_BOARD_TYPE_8STM1EIO     = 12,
    SDD_BOARD_TYPE_12E3DS3      = 13,
    SDD_BOARD_TYPE_12E3DS3IO    = 14,
    SDD_BOARD_TYPE_126E1        = 15,
    SDD_BOARD_TYPE_126E1IO      = 16,
    SDD_BOARD_TYPE_SI           = 17,
    SDD_BOARD_TYPE_OA           = 18,
    SDD_BOARD_TYPE_PWR          = 19,
    SDD_BOARD_TYPE_FAN          = 20,
    SDD_BOARD_TYPE_1OTU         = 21,
    SDD_BOARD_TYPE_10GEL2       = 22,
    SDD_BOARD_TYPE_8STM16M      = 23,
    SDD_BOARD_TYPE_16STM14M     = 24,
    SDD_BOARD_TYPE_2STM64M      = 25,
    SDD_BOARD_TYPE_EOS          = 26,
    SDD_BOARD_TYPE_GMPLS        = 27,
    SDD_BOARD_TYPE_DATAFABRIC_CONTROLLER        = 28,
    SDD_BOARD_TYPE_DATAFABRIC   = 29,
    SDD_BOARD_TYPE_BACKPLANE    = 30,
    SDD_BOARD_TYPE_TENGE        = 31,

    SDD_BOARD_TYPE_10GEL2_46    = 32,    

    SDD_BOARD_TYPE_MAX
} SDD_BOARD_TYPE_T;

typedef enum
{
    SDD_BUTTON_EVENT_TYPE_LEDTEST,
    SDD_BUTTON_EVENT_TYPE_ALARMCUTOFF,
    SDD_BUTTON_EVENT_TYPE_ALARMSUPPRESS
}SDD_BUTTON_EVENT_TYPE;

typedef enum 
{
  BUTTON_STATE_DOWN,
  BUTTON_STATE_UP,
} BUTTON_STATE_T;

typedef enum
{
  POWER_FAIL,
  POWER_OK,
  POWER_PRESENT,
  POWER_ABSENT
} POWER_STATUS_T;

typedef enum{
    FAN_STATUS_OK,
    FAN_STATUS_FAIL,
    FAN_STATUS_ABSENT,
    FAN_STATUS_PRESENT
}FAN_STATUS_T;

typedef enum
{
    TIMING_LINE_NULL = 0,
    TIMING_LINE_1    = 1,        
    TIMING_LINE_2    = 2            
        
}TIMING_LINE_T;

typedef enum 
{
    none=0,
    manre=1,
    mantest=2
} ALS_MODE_STATUS_T;

typedef enum
{
    PORT_STATE_UNKNOWN = -1,
    PORT_STATE_DIS,
    PORT_STATE_EN
} PORTADMIN_STATE_T;

typedef enum
{
    ALS_FUNC_UNKNOWN = -1,
    ALS_FUNC_DIS,
    ALS_FUNC_EN
}ALS_FUNC_STATE_T;

typedef struct
{
    TIMING_LINE_T port1_Line;
    TIMING_LINE_T port2_Line;
    TIMING_LINE_T port3_Line;
    TIMING_LINE_T port4_Line;
    TIMING_LINE_T port5_Line;
    TIMING_LINE_T port6_Line;
    TIMING_LINE_T port7_Line;
    TIMING_LINE_T port8_Line;
} TIMING_REF_T;


typedef enum
{
    LED_TEST_KNOWN = -1,
    LED_TEST_BEGIN = 0,
    LED_TEST_PROC  = 1,
    LED_TEST_END   = 2
} SDD_LED_TEST_STATE_T;

 
typedef void (*SDD_BOARD_EVENT_CBACK)(BOARD_EVENT_T event,
                                    SLOT_NUMBER_T slot,
                                    BOARD_STYLE_T cardType);

typedef void (*SDD_LATCH_EVENT_CBACK)(BOARD_EVENT_T event,
                                    SLOT_NUMBER_T slot,
                                    BOARD_STYLE_T cardType);

typedef void (*SDD_ENTITY_UPDATE_CBACK)(INT16 entityNum,
                                       ENTITY_INFO_T *pInfo);


typedef void (*SDD_ONE_SECOND_TIMER_CBACK)(UINT32 counter);

typedef void (*SDD_BUTTON_EVENT_CBACK)(SDD_BUTTON_EVENT_TYPE buttonType,
                                      BUTTON_STATE_T oldState,
                                      BUTTON_STATE_T newState);                                      







 
































































# 25 "../../../appl/CTD/inc/ctd_sdh.h" 2


 


typedef unsigned int  HW_PORT_TYPE;      
































 


typedef unsigned int  HW_ACT_PORT_TYPE;      













 


typedef UINT32 SDH_TP_TYPE;   











































 


typedef UINT8 MAGUS_TP_TYPE;







 
typedef enum 
{
    TTI_LEN_16BYTE        = 0x1,
    TTI_LEN_64BYTE        = 0x2,
    TTI_LEN_FIXED1B       = 0x3,
    TTI_LEN_PROVISIONED1B = 0x4,
}TTI_LEN_T;




typedef int ePort_MAPPING_MODE;




typedef enum 
{
    PORT_LOOP_NONE     = 0x1,       
    PORT_LOOP_INWARD   = 0x2,       
    PORT_LOOP_OUTWARD  = 0x3        
} PORT_LOOPBACK_T;

typedef enum
{
    TTI_TYPE_TRANSMIT   = 0x0,
    TTI_TYPE_ACCEPTED   = 0x1,
    TTI_TYPE_CAPTURED   = 0x2,
    TTI_TYPE_EXPECTED   = 0x3
} TTI_TYPE_T;

typedef char TTI_STRING[64];

typedef enum
{
    TIM_DISABLED    = 0x0,
    TIM_ENABLED     = 0x1
} eTIM_MODE;

typedef enum 
{
    PSL_TYPE_TRANSMIT   = 0x0,
    PSL_TYPE_ACCEPTED   = 0x1,
    PSL_TYPE_CAPTURED   = 0x2,
    PSL_TYPE_EXPECTED   = 0x3
} PSL_TYPE_T;

typedef enum 
{
    PORT_MODE_INVALID   = 0x0,   
    PORT_MODE_AUTO      = 0x1,   
    PORT_MODE_MON       = 0x2,   
    PORT_MODE_NMON      = 0x3    
} PORT_MODE_T;

 
typedef enum 
{
    ALARM_SOURCE    = 0x0,
    ALARM_SINK      = 0x1
}ALARM_DIRECTION_T;

 
typedef struct
{
    UINT32  bip;
    UINT32  fc;
    BOOL    b_lof;
    BOOL    b_ses;
} RS_PMPARA_T;

typedef struct
{
    UINT32  bip;      
    UINT32  rei;      
    UINT32  fc;       
    UINT32  fefc;     
    BOOL    b_ses;    
    BOOL    b_fses;   
} MSHPLP_PMPARA_T;

typedef struct
{
    UINT32 npjc;
    UINT32 ppjc;
} AUTU_PMPARA_T;

typedef struct
{
    UINT32  bip;      
    UINT32  bei;      
    UINT32  fc;       
    UINT32  fefc;     
} OTU_PMPARA_T;

typedef struct
{
    UINT32  fec0;      
    UINT32  fec1;      
    UINT32  fecall;       
    UINT32  fecbyte;      
    UINT32  fecun;      
    UINT32  fc;     
} FEC_PMPARA_T;

typedef struct 
{
    int   nLaserIBias;
    float nVcc;                              
    int   nLaserTemp;
    int   nLaserTransPwr;
    int   nLaserRecvPwr;
    int   bCompliance8472;                    
    int   pbnNominalMBps;                     
    int   pbnWavelength;                      
    int   pbnLinkLength9125;                  
    int   pbnLinkLength50125;                 
    int   pbnLinkLength62125;                 
    int   pbnLinkLengthCopper;                
    char  pbsVendorName[20];                  
    char  pbsVendorPN[20];                    
    char  pbsVendorSN[20];                    
    char  pbsManuDate[16];                    
    PbSFFTranscCode_Tval pbnTransCode;        
    char  pbsVendorOUI[16];                   
    PbConnectorType_Tval pbnConnectorType;    
    int   pbnClLaserTemp;                    
    int   pbnTECCurrent;                     
    char  pbsSagPN[20];                      
} LASER_PARAMETER_T;

 
typedef enum 
{
    PDH_MAP_E1_VC12_TU12    = 0x0,
    PDH_MAP_DS1_VC11_TU11   = 0x1,
    PDH_MAP_DS1_VC11_TU12   = 0x2,
    PDH_MAP_E3_LOVC3_TU3    = 0x3,
    PDH_MAP_E3_HOVC3_AU3    = 0x4,
    PDH_MAP_DS3_LOVC3_TU3   = 0x5,
    PDH_MAP_DS3_HOVC3_AU3   = 0x6
} PDH_MAPPING_T;

typedef enum
{
    LINE_CODING_HDB3    = 0x0,
    LINE_CODING_CMI     = 0x1,
    LINE_CODING_AMI     = 0x2,
    LINE_CODING_B3ZS    = 0x3,
    LINE_CODING_B6ZS    = 0x4,
    LINE_CODING_B8ZS    = 0x5
}LINE_CODING_T;

typedef enum
{
    OHM_75_R  = 0x0,
    OHM_120_R = 0x1,
    OHM_100_R = 0x2,
    OHM_110_R = 0x3,
}PDH_OHM_TYPE_T;

typedef enum 
{
    PDH_MAP_OPTION_ASYNC    = 0x0,
    PDH_MAP_OPTION_BITSYNC  = 0x1,
    PDH_MAP_OPTION_BYTESYNC = 0x2
}PDH_MAP_OPTION_T;

 
typedef enum 
{
    E1_SA_BIT_4     = 0x0,
    E1_SA_BIT_5     = 0x1,
    E1_SA_BIT_6     = 0x2,
    E1_SA_BIT_7     = 0x3,
    E1_SA_BIT_8     = 0x4
}E1_SA_BIT_T;

typedef enum 
{
    PORT_STATE_ABSENT   = 0x0,
    PORT_STATE_UP       = 0x1,
    PORT_STATE_DOWN     = 0x2
} PORT_STATE_T;

typedef enum 
{
    LATCH_STATE_UNKNOWN     = 0x0,
    LATCH_STATE_CLOSE       = 0x1,
    LATCH_STATE_OPEN        = 0x2,
    LATCH_STATE_CARDFAIL    = 0x3,
    LATCH_STATE_CARD_REMOVE = 0x4,
    LATCH_STATE_CARD_INSERT = 0x5
}LATCH_STATE_T;

typedef enum
{
    EOW_TYPE_NULL   = 0x0,
    EOW_TYPE_E1     = 0x1,
    EOW_TYPE_E2     = 0x2,
    EOW_TYPE_F1     = 0x3
}EOW_TYPE_T;

typedef enum
{ 
    PDH_MODE_DS3  = 0x0,
    PDH_MODE_E3   = 0x1,
    PDH_MODE_E1   = 0x2,
    PDH_MODE_DS1  = 0x3
}PDH_PORT_MODE_T;

typedef enum
{
    E1_STRUCTURE_INVALID   = 0x0,
    E1_STRUCTURE_FRAMED    = 0x1,
    E1_STRUCTURE_UNFRAMED  = 0x2,
    E3_STRUCTURE_G751      = 0x3,
    E3_STRUCTURE_G832      = 0x4,
    E3_STRUCTURE_UNFRAMED  = 0x5,
    DS3_STRUCTURE_M13      = 0x6,
    DS3_STRUCTURE_CBIT     = 0x7,
    DS3_STRUCTURE_UNFRAMED = 0x8,
}PDH_FRAME_MODE_T;

 
typedef enum 
{
    PRBS_STATUS_NA          = 0x0,
    PRBS_STATUS_SYNCING     = 0x1,
    PRBS_STATUS_SYNC_SUCC   = 0x2,
    PRBS_STATUS_SYNC_FAIL   = 0x3,
    PRBS_STATUS_SYNC_ERROR  = 0x4
} PRBS_STATUS_T;

typedef enum 
{
    PRBS_MODE_NA      = 0x0,
    PRBS_MODE_POWER15 = 0x1,
    PRBS_MODE_POWER20 = 0x2,
    PRBS_MODE_POWER23 = 0x3,
} PRBS_MODE_T;

typedef enum 
{
    SIGNAL_TYPE_NORMAL  = 0x1,      
    SIGNAL_TYPE_UNEQ    = 0x2,      
    SIGNAL_TYPE_AIS     = 0x3,      
    SIGNAL_TYPE_RDI     = 0x4,       
     
    SIGNAL_TYPE_PPIAIS  = 0x8
} SIGNAL_TYPE_T;

typedef enum 
{
    ALARM_INSERT_AUTO   = 0x0,
    ALARM_INSERT_FORCE  = 0x1,
}ALARM_INSERT_WAY_T;

typedef enum
{
    DCC_TYPE_NONE    = 0x0,
    DCC_TYPE_RS      = 0x1,
    DCC_TYPE_MS      = 0x2,
    DCC_TYPE_BOTH    = 0x3,
} DCC_TYPE_T;

typedef enum
{
    CTD_BOARD_EPGTYPE_ERROR         = 0x0,
    CTD_BOARD_EPGTYPE_NONE          = 0x1,
    CTD_BOARD_EPGTYPE_1TON_WORK     = 0x2,
    CTD_BOARD_EPGTYPE_1TON_PROT     = 0x3,
    CTD_BOARD_EPGTYPE_1PLUS1_WORK   = 0x4,
    CTD_BOARD_EPGTYPE_1PLUS1_PROT   = 0x5,
    CTD_BOARD_EPGTYPE_1TO1_WORK     = 0x6,
    CTD_BOARD_EPGTYPE_1TO1_PROT     = 0x7
} CTD_BOARD_EPGTYPE;

 
typedef enum
{
    CTD_LOCC_NONE    = 0x0,
    CTD_LOCC_20G     = 0x1,
    CTD_LOCC_40G     = 0x2,
}CTD_LOCC_MODE_T;

 
typedef enum
{
    CTD_CARD_UNKNOWN    = 0x0,
    CTD_CARD_ACTIVE     = 0x1,
    CTD_CARD_STANDBY    = 0x2,
}CTD_CARD_STATE_T;

 
typedef enum
{
    CTD_FEC_ERR_NONE    = 0x0,
    CTD_FEC_ERR_E3      = 0x3,
    CTD_FEC_ERR_E4      = 0x4,
    CTD_FEC_ERR_E5      = 0x5,
}CTD_FEC_INSERT_RATE_T;

 












 









 











typedef struct 
{
    char   *name;
    UINT32  style;           
    UINT32  option;          
    UINT32  totalPortNum;    
    UINT32  portIdRange[(29 ) ][2];   



    UINT32  dataPortNum[2];  
} CTD_BOARD_INFO_T;

 













































 
 






 


 













typedef UINT16 TU_TP_T;

 


 





























typedef UINT32 AU_TP_T;

 



 







typedef UINT32 COMPACT_TP_T;






 






 




 









 


typedef struct
{
     
    SDH_TP_TYPE eTpType;

     
    UINT32 E: 4,
           D: 4,
           C: 4,
           B: 4,
           A: 4,
           K: 4,
           L: 4,
           M: 4;        
}__attribute__ ((packed))  TP_INDEX_T;  

 


 
















typedef UINT16 SDH_TUG3_MAPPING_T;



typedef UINT8 SDH_TUG2_MAPPING_T;




 


typedef UINT16 SDH_VC3H_MAPPING_T;

 







 









typedef struct
{
    



    BOOL b_clr;
    SDH_TUG3_MAPPING_T tug3[3];
} SDH_VC4_MAPPING_T;

typedef struct
{
    



    BOOL b_au4;

    union
    {
         
        SDH_VC4_MAPPING_T vc4;
 
         
        SDH_VC3H_MAPPING_T vc3h[3];
    } hovc;
} SDH_AUG1_MAPPING_T;

typedef struct
{
    



    BOOL b_4c;
 
    SDH_AUG1_MAPPING_T aug1[4];
} SDH_AUG4_MAPPING_T;

typedef struct
{
    



    BOOL b_16c;
 
    SDH_AUG4_MAPPING_T aug4[4];
} SDH_AUG16_MAPPING_T;

typedef struct
{
    



    BOOL b_64c;

    SDH_AUG16_MAPPING_T aug16[4];
} SDH_AUG64_MAPPING_T;

typedef struct
{
    



    BOOL b_256c;

    SDH_AUG64_MAPPING_T aug64[4];
} SDH_AUG256_MAPPING_T;

extern void CTD_InitModule();
extern UINT32 CTD_GetPortTotalNumber(SLOT_NUMBER_T slot);
extern UINT32 CTD_GetPortNumber(SLOT_NUMBER_T slot, HW_PORT_TYPE type);
extern UINT32 CTD_GetPortBegin(SLOT_NUMBER_T slot, HW_PORT_TYPE type);
extern UINT32 CTD_GetBoardStyle(SLOT_NUMBER_T slot);
extern UINT32 CTD_GetBoardOption(SLOT_NUMBER_T slot);
extern PB_ERROR_CODE_T CTD_SetBoardInfo(SLOT_NUMBER_T slot, UINT32 ulStyle, UINT32 ulOption);
extern PB_ERROR_CODE_T CTD_GetPortType(SLOT_NUMBER_T slot, UINT8 *pucTypeNum, HW_PORT_TYPE *pePortType);
extern SLOT_NUMBER_T CTD_InstanceToSlot(INSTANCE_T inst);
extern INSTANCE_T CTD_SlotToInstance(SLOT_NUMBER_T slot);
extern UINT32 CTD_GetRelativePortId(SLOT_NUMBER_T slot, UINT32 port);
extern UINT32 CTD_GetAbsolutePortId(SLOT_NUMBER_T slot, HW_PORT_TYPE pType, UINT32 port);
extern CTD_BOARD_EPGTYPE CTD_GetBoardEpgType(SLOT_NUMBER_T slot);
extern CARDTYPE_T CTD_BoardStyleToCardType(PbBoardStyle_Tval boardStyle);
extern PORT_LOOPBACK_T CTD_TransToCdhLoopType(pbPortLoopback_Tval  eLoopbackType);
extern PB_ERROR_CODE_T CTD_TransToCdhMap(PORT_MAPPING_T *pTug3Map, void *pCdhMap);
extern PB_ERROR_CODE_T CTD_TransToCdhTp(const TP_INDEX_T *pTpIndex, COMPACT_TP_T *pTpId);
extern SDH_TP_TYPE CTD_GetTpType(const TP_INDEX_T *pTpIndex);
extern TTI_LEN_T CTD_TransToCdhTtiMode(PbTIModeType_Tval mode);
extern SIGNAL_TYPE_T CTD_TransToCdhSigType(PbSignalType_Tval signalType);
extern LATCH_STATE_T CTD_TransToCdhLatchState(PbEpgFunctionCardStatus_Tval eState);
extern PbEpgFunctionCardStatus_Tval CTD_TransToExLatchState(LATCH_STATE_T eState);
extern PRBS_MODE_T CTD_TransToCdhPrbsMode(pbPRBSPattern_Tval prbsPattern);
extern PbPortOperationStatus_Tval CTD_TransToExPortState(PORT_STATE_T cdhPortState);
extern const char* CTD_GetLedStateName(LED_STATE_T state);
extern PDH_FRAME_MODE_T CTD_TransToCdhFrameMode(expectedFrameStructure_Tval eFrameMode);
extern UINT32 CTD_TransToCdhPortType(HW_ACT_PORT_TYPE ePortType);
extern PORT_LOOPBACK_T CTD_TransToOtuLoopType(pbOTUPortLoopback_Tval eLoopbackType);
extern CTD_FEC_INSERT_RATE_T CTD_TransToOtuFecErrRate(pbInsFecErrRate_Tval InsFecErrRate);
extern CTD_CARD_STATE_T CTD_GetXcState();
extern PB_ERROR_CODE_T CTD_SetXcState(BOOL bCardState);
extern const char* CTD_GetCardName(SLOT_NUMBER_T slot);

extern CTD_BOARD_INFO_T ctd_boardInfo[];








 













































































































# 20 "../../../appl/CAH/inc/cah_defs.h" 2

# 1 "../../../appl/DBG/inc/Dbg_ei.h" 1
 


















# 1 "../../../infra/SYS/target/h/stdio.h" 1
 

 
 

 



















































# 1 "../../../infra/SYS/target/h/classLib.h" 1
 

 

 

























# 1 "../../../infra/SYS/target/h/objLib.h" 1
 

 

 




































# 1 "../../../infra/SYS/target/h/errno.h" 1
 

 

 







 
































 








































 



 

	 












	 


















 






 



 






 



 


 




 




extern int *	__errno(void);





# 175 "../../../infra/SYS/target/h/errno.h"














# 42 "../../../infra/SYS/target/h/objLib.h" 2




 









 

typedef struct obj_core  *OBJ_ID;		 


 















# 91 "../../../infra/SYS/target/h/objLib.h"


 



extern STATUS 	objShow (OBJ_ID objId, int showType);














# 31 "../../../infra/SYS/target/h/classLib.h" 2

# 1 "../../../infra/SYS/target/h/memLib.h" 1
 

 

 






















































 







 



typedef struct mem_part *PART_ID;

 

typedef struct
    {

    unsigned long numBytesFree,	    
		  numBlocksFree,    
		  maxBlockSizeFree, 
		  numBytesAlloc,    
		  numBlocksAlloc;   

    }  MEM_PART_STATS;


 

 



 




 




 






 









 

 

extern PART_ID memSysPartId;

 

extern FUNCPTR  smMemPartOptionsSetRtn;
extern FUNCPTR  smMemPartFindMaxRtn;
extern FUNCPTR  smMemPartReallocRtn;
extern FUNCPTR  smMemPartShowRtn;

 



extern STATUS 	memInit (char *pPool, unsigned poolSize);
extern STATUS 	memPartLibInit (char *pPool, unsigned poolSize);
extern PART_ID 	memPartCreate (char *pPool, unsigned poolSize);
extern void 	memPartInit (PART_ID partId, char *pPool, unsigned poolSize);
extern STATUS 	memPartAddToPool (PART_ID partId, char *pPool,
				  unsigned poolSize);
extern void 	memAddToPool (char *pPool, unsigned poolSize);
extern void *	memPartAlloc (PART_ID partId, unsigned nBytes);
extern void *   memPartAlignedAlloc (PART_ID partId, unsigned nBytes,
				     unsigned alignment);
extern void *	memalign (unsigned alignment, unsigned size);
extern void *   valloc (unsigned size);
extern STATUS 	memPartFree (PART_ID partId, char *pBlock);
extern STATUS 	memPartOptionsSet (PART_ID partId, unsigned options);
extern int 	memFindMax (void);
extern int 	memPartFindMax (PART_ID partId);
extern void *	memPartRealloc (PART_ID partId, char *pBlock, unsigned nBytes);
extern void 	memOptionsSet (unsigned options);
extern STATUS 	cfree (char *pBlock);
extern void 	memShowInit (void);
extern void 	memShow (int type);
extern STATUS 	memPartShow (PART_ID partId, int type);
extern STATUS   memPartInfoGet (PART_ID	partId, MEM_PART_STATS * ppartStats);

# 186 "../../../infra/SYS/target/h/memLib.h"









# 32 "../../../infra/SYS/target/h/classLib.h" 2


 






typedef struct obj_class *CLASS_ID;		 

extern CLASS_ID classClassId;			 

 



extern STATUS 	classLibInit (void);
extern STATUS 	classMemPartIdSet (CLASS_ID classId, PART_ID memPartId);
extern void 	classShowInit (void);
extern STATUS 	classShow (CLASS_ID classId, int level);

















# 58 "../../../infra/SYS/target/h/stdio.h" 2


# 1 "../../../infra/SYS/target/h/private/objLibP.h" 1
 

 

 


























typedef struct obj_core		 
    {
    struct obj_class *pObjClass;	 
    } OBJ_CORE;






 



extern OBJ_ID 	objCreate (CLASS_ID classId, ...);
extern STATUS 	objInit (CLASS_ID classId, OBJ_ID objId, ...);
extern STATUS 	objDelete (OBJ_ID objId);
extern STATUS 	objDestroy (OBJ_ID objId, BOOL dealloc, int timeout);
extern STATUS 	objFree (CLASS_ID classId, char *pObject);
extern STATUS 	objTerminate (OBJ_ID objId);
extern void *	objAlloc (CLASS_ID classId);
extern void *	objAllocExtra (CLASS_ID classId, unsigned nExtraBytes,
			       void ** ppExtra);
extern void 	objCoreInit (OBJ_CORE *pObjCore, CLASS_ID pObjClass);
extern void 	objCoreTerminate (OBJ_CORE *pObjCore);

# 71 "../../../infra/SYS/target/h/private/objLibP.h"









# 60 "../../../infra/SYS/target/h/stdio.h" 2

# 1 "../../../infra/SYS/target/h/private/classLibP.h" 1
 

 
 























 









typedef struct obj_class	 
    {
    OBJ_CORE		objCore;	 
    struct mem_part	*objPartId;	 
    unsigned		objSize;	 
    unsigned		objAllocCnt;	 
    unsigned		objFreeCnt;	 
    unsigned		objInitCnt;	 
    unsigned		objTerminateCnt; 
    int			coreOffset;	 
    FUNCPTR		createRtn;	 
    FUNCPTR		initRtn;	 
    FUNCPTR		destroyRtn;	 
    FUNCPTR		showRtn;	 
    FUNCPTR		instRtn;	 
    } OBJ_CLASS;


 



extern CLASS_ID	classCreate (unsigned objectSize, int coreOffset,
			     FUNCPTR createRtn, FUNCPTR initRtn,
			     FUNCPTR destroyRtn);
extern STATUS	classInit (OBJ_CLASS *pObjClass, unsigned objectSize,
			   int coreOffset, FUNCPTR createRtn, FUNCPTR initRtn,
			   FUNCPTR destroyRtn);
extern STATUS	classDestroy (CLASS_ID classId);
extern STATUS	classInstConnect (CLASS_ID classId, FUNCPTR instRtn);
extern STATUS	classShowConnect (CLASS_ID classId, FUNCPTR showRtn);
extern STATUS   classInstrument ( OBJ_CLASS * pObjClass, 
				  OBJ_CLASS * pObjInstClass );


# 82 "../../../infra/SYS/target/h/private/classLibP.h"









   






# 61 "../../../infra/SYS/target/h/stdio.h" 2



# 1 "../../../infra/SYS/host/gnu2.96/libinc/stdarg.h" 1
 








































# 1 "../../../infra/SYS/host/gnu2.96/libinc/va-ppc.h" 1
 

# 68 "../../../infra/SYS/host/gnu2.96/libinc/va-ppc.h"


 
 







 



 

typedef struct __va_list_tag {
  unsigned char gpr;		 


  unsigned char fpr;		 


  char *overflow_arg_area;	 

  char *reg_save_area;		 
} __va_list[1], __gnuc_va_list[1];









 



 


typedef struct {
  long   __gp_save[8];		 
  double __fp_save[8];		 
} __va_regsave_t;

 
 









 









 























 













# 267 "../../../infra/SYS/host/gnu2.96/libinc/va-ppc.h"



 






# 330 "../../../infra/SYS/host/gnu2.96/libinc/va-ppc.h"


# 42 "../../../infra/SYS/host/gnu2.96/libinc/stdarg.h" 2

# 131 "../../../infra/SYS/host/gnu2.96/libinc/stdarg.h"












 
 













# 175 "../../../infra/SYS/host/gnu2.96/libinc/stdarg.h"


 




 

 

 

typedef __gnuc_va_list va_list;
























# 64 "../../../infra/SYS/target/h/stdio.h" 2



# 77 "../../../infra/SYS/target/h/stdio.h"


 

struct __sbuf 			 
    {
    uchar_t * _base;		 
    int	      _size;		 
    };

typedef	struct __sFILE
    {
    OBJ_CORE		objCore;	 
    uchar_t *		_p;		 
    int			_r;		 
    int			_w;		 
    short		_flags;		 
    short		_file;		 
    struct __sbuf	_bf;		 
    int			_lbfsize;	 
    struct __sbuf	_ub;		 
    uchar_t *		_up;		 
    int			_ur;		 
    uchar_t		_ubuf[3];	 
    uchar_t		_nbuf[1];	 
    struct __sbuf	_lb;		 
    int			_blksize;	 
    int			_offset;	 
    int			taskId;		 
    } FILE;

 

extern CLASS_ID fpClassId;		 

 

























































extern void	clearerr (FILE *);
extern int	fclose (FILE *);
extern int	feof (FILE *);
extern int	ferror (FILE *);
extern int	fflush (FILE *);
extern int	fgetc (FILE *);
extern int	fgetpos (FILE *, fpos_t *);
extern char *	fgets (char *, size_t, FILE *);
extern FILE *	fopen (const char *, const char *);
extern int	fprintf (FILE *, const char *, ...);
extern int	fputc (int, FILE *);
extern int	fputs (const char *, FILE *);
extern int	fread (void *, size_t, size_t, FILE *);
extern FILE *	freopen (const char *, const char *, FILE *);
extern int	fscanf (FILE *, const char *, ...);
extern int	fseek (FILE *, long, int);
extern int	fsetpos (FILE *, const fpos_t *);
extern long	ftell (FILE *);
extern int	fwrite (const void *, size_t, size_t, FILE *);
extern int	getc (FILE *);
extern int	getchar (void);
extern char *	gets (char *);
extern void	perror (const char *);
extern int	printf (const char *, ...);
extern int	putc (int, FILE *);
extern int	putchar (int);
extern int	puts (const char *);
extern int	remove (const char *);
extern int	rename  (const char *, const char *);
extern void	rewind (FILE *);
extern int	scanf (const char *, ...);
extern void	setbuf (FILE *, char *);
extern int	setvbuf (FILE *, char *, int, size_t);
extern int	sprintf (char *, const char *, ...);
extern int	sscanf (const char *, const char *, ...);
extern FILE *	tmpfile (void);
extern char *	tmpnam (char *);
extern int	ungetc (int, FILE *);
extern int	vfprintf (FILE *, const char *, va_list);
extern int	vprintf (const char *, va_list);
extern int	vsprintf (char *, const char *, va_list);
extern int	__srget (FILE *);		 
extern int	__swbuf (int, FILE *);		 

 

extern FILE *	fdopen (int, const char *);
extern int	fileno (FILE *);

 

 



extern int	fdprintf (int fd, const char *fmt, ...);
extern int	vfdprintf (int fd, const char *fmt, va_list ap);
extern int	printErr (const char *fmt, ...);
extern int	getw (FILE *);
extern int	putw (int, FILE *);
extern void	setbuffer (FILE *, char *, int);
extern int	setlinebuf (FILE *);
extern FILE *   stdioFp (int std);
extern STATUS   stdioShow (FILE * fp, int level);
extern STATUS   stdioShowInit (void);
extern STATUS   stdioInit (void);



# 310 "../../../infra/SYS/target/h/stdio.h"



 


extern FILE **	__stdin(void);		 
extern FILE **	__stdout(void);		 
extern FILE **	__stderr(void);		 











 










































# 20 "../../../appl/DBG/inc/Dbg_ei.h" 2


# 1 "../../../infra/UTL/DBG/tcl8.0/generic/tcl.h" 1
 


















 

























 










# 77 "../../../infra/UTL/DBG/tcl8.0/generic/tcl.h"


 




# 94 "../../../infra/UTL/DBG/tcl8.0/generic/tcl.h"


 




# 115 "../../../infra/UTL/DBG/tcl8.0/generic/tcl.h"


 












 













# 154 "../../../infra/UTL/DBG/tcl8.0/generic/tcl.h"


 






















 












# 202 "../../../infra/UTL/DBG/tcl8.0/generic/tcl.h"


 









    typedef void *ClientData;






 


















typedef struct Tcl_Interp {
    char *result;		 

    void (*freeProc)  (char *blockPtr)  ;
				 






    int errorLine;               


} Tcl_Interp;

typedef struct Tcl_AsyncHandler_ *Tcl_AsyncHandler;
typedef struct Tcl_Channel_ *Tcl_Channel;
typedef struct Tcl_Command_ *Tcl_Command;
typedef struct Tcl_Event Tcl_Event;
typedef struct Tcl_Pid_ *Tcl_Pid;
typedef struct Tcl_RegExp_ *Tcl_RegExp;
typedef struct Tcl_TimerToken_ *Tcl_TimerToken;
typedef struct Tcl_Trace_ *Tcl_Trace;
typedef struct Tcl_Var_ *Tcl_Var;

 




























 



typedef enum {TCL_INT, TCL_DOUBLE, TCL_EITHER} Tcl_ValueType;
typedef struct Tcl_Value {
    Tcl_ValueType type;		 

    long intValue;		 
    double doubleValue;		 
} Tcl_Value;

 





struct Tcl_Obj;

 



typedef int (Tcl_AppInitProc)  (Tcl_Interp *interp)  ;
typedef int (Tcl_AsyncProc)  (ClientData clientData,
	Tcl_Interp *interp, int code)  ;
typedef void (Tcl_ChannelProc)  (ClientData clientData, int mask)  ;
typedef void (Tcl_CloseProc)  (ClientData data)  ;
typedef void (Tcl_CmdDeleteProc)  (ClientData clientData)  ;
typedef int (Tcl_CmdProc)  (ClientData clientData,
	Tcl_Interp *interp, int argc, char *argv[])  ;
typedef void (Tcl_CmdTraceProc)  (ClientData clientData,
	Tcl_Interp *interp, int level, char *command, Tcl_CmdProc *proc,
	ClientData cmdClientData, int argc, char *argv[])  ;
typedef void (Tcl_DupInternalRepProc)  (struct Tcl_Obj *srcPtr, 
        struct Tcl_Obj *dupPtr)  ;
typedef int (Tcl_EventProc)  (Tcl_Event *evPtr, int flags)  ;
typedef void (Tcl_EventCheckProc)  (ClientData clientData,
	int flags)  ;
typedef int (Tcl_EventDeleteProc)  (Tcl_Event *evPtr,
        ClientData clientData)  ;
typedef void (Tcl_EventSetupProc)  (ClientData clientData,
	int flags)  ;
typedef void (Tcl_ExitProc)  (ClientData clientData)  ;
typedef void (Tcl_FileProc)  (ClientData clientData, int mask)  ;
typedef void (Tcl_FileFreeProc)  (ClientData clientData)  ;
typedef void (Tcl_FreeInternalRepProc)  (struct Tcl_Obj *objPtr)  ;
typedef void (Tcl_FreeProc)  (char *blockPtr)  ;
typedef void (Tcl_IdleProc)  (ClientData clientData)  ;
typedef void (Tcl_InterpDeleteProc)  (ClientData clientData,
	Tcl_Interp *interp)  ;
typedef int (Tcl_MathProc)  (ClientData clientData,
	Tcl_Interp *interp, Tcl_Value *args, Tcl_Value *resultPtr)  ;
typedef void (Tcl_NamespaceDeleteProc)  (ClientData clientData)  ;
typedef int (Tcl_ObjCmdProc)  (ClientData clientData,
	Tcl_Interp *interp, int objc, struct Tcl_Obj * const  objv[])  ;
typedef int (Tcl_PackageInitProc)  (Tcl_Interp *interp)  ;
typedef void (Tcl_TcpAcceptProc)  (ClientData callbackData,
        Tcl_Channel chan, char *address, int port)  ;
typedef void (Tcl_TimerProc)  (ClientData clientData)  ;
typedef int (Tcl_SetFromAnyProc)  (Tcl_Interp *interp,
	struct Tcl_Obj *objPtr)  ;
typedef void (Tcl_UpdateStringProc)  (struct Tcl_Obj *objPtr)  ;
typedef char *(Tcl_VarTraceProc)  (ClientData clientData,
	Tcl_Interp *interp, char *part1, char *part2, int flags)  ;

 





typedef struct Tcl_ObjType {
    char *name;			 
    Tcl_FreeInternalRepProc *freeIntRepProc;
				 


    Tcl_DupInternalRepProc *dupIntRepProc;
    				 

    Tcl_UpdateStringProc *updateStringProc;
    				 

    Tcl_SetFromAnyProc *setFromAnyProc;
    				 



} Tcl_ObjType;

 





typedef struct Tcl_Obj {
    int refCount;		 
    char *bytes;		 










    int length;			 

    Tcl_ObjType *typePtr;	 



    union {			 
	long longValue;		 
	double doubleValue;	 
	void  *otherValuePtr;	 
	struct {		 
	    void  *ptr1;
	    void  *ptr2;
	} twoPtrValue;
    } internalRep;
} Tcl_Obj;

 










extern  void		Tcl_IncrRefCount  (Tcl_Obj *objPtr)  ;
extern  void		Tcl_DecrRefCount  (Tcl_Obj *objPtr)  ;
extern  int		Tcl_IsShared  (Tcl_Obj *objPtr)  ;

















 





extern  Tcl_Obj *	Tcl_NewBooleanObj  (int boolValue)  ;
extern  Tcl_Obj *	Tcl_NewDoubleObj  (double doubleValue)  ;
extern  Tcl_Obj *	Tcl_NewIntObj  (int intValue)  ;
extern  Tcl_Obj *	Tcl_NewListObj  (int objc,
			    Tcl_Obj * const  objv[])  ;
extern  Tcl_Obj *	Tcl_NewLongObj  (long longValue)  ;
extern  Tcl_Obj *	Tcl_NewObj  (void)  ;
extern  Tcl_Obj *	Tcl_NewStringObj  (char *bytes,
			    int length)  ;

# 485 "../../../infra/UTL/DBG/tcl8.0/generic/tcl.h"


 





typedef struct Tcl_Namespace {
    char *name;                  



    char *fullName;              

    ClientData clientData;       

    Tcl_NamespaceDeleteProc* deleteProc;
                                 

    struct Tcl_Namespace* parentPtr;
                                 


} Tcl_Namespace;

 





















typedef struct Tcl_CallFrame {
    Tcl_Namespace *nsPtr;
    int dummy1;
    int dummy2;
    char *dummy3;
    char *dummy4;
    char *dummy5;
    int dummy6;
    char *dummy7;
    char *dummy8;
    int dummy9;
    char* dummy10;
} Tcl_CallFrame;

 













     
typedef struct Tcl_CmdInfo {
    int isNativeObjectProc;	  



    Tcl_ObjCmdProc *objProc;	  
    ClientData objClientData;	  
    Tcl_CmdProc *proc;		  
    ClientData clientData;	  
    Tcl_CmdDeleteProc *deleteProc;
                                  

    ClientData deleteData;	  

    Tcl_Namespace *namespacePtr;  




} Tcl_CmdInfo;

 






typedef struct Tcl_DString {
    char *string;		 

    int length;			 

    int spaceAvl;		 

    char staticSpace[200 ];
				 

} Tcl_DString;





 




 



 







 






 








 








 















 









 





extern  char *		Tcl_Alloc  (unsigned int size)  ;
extern  void		Tcl_Free  (char *ptr)  ;
extern  char *		Tcl_Realloc  (char *ptr,
			    unsigned int size)  ;

# 698 "../../../infra/UTL/DBG/tcl8.0/generic/tcl.h"
















 









 





typedef struct Tcl_HashEntry {
    struct Tcl_HashEntry *nextPtr;	 


    struct Tcl_HashTable *tablePtr;	 
    struct Tcl_HashEntry **bucketPtr;	 


    ClientData clientData;		 

    union {				 
	char *oneWordValue;		 
	int words[1];			 



	char string[4];			 


    } key;				 
} Tcl_HashEntry;

 






typedef struct Tcl_HashTable {
    Tcl_HashEntry **buckets;		 


    Tcl_HashEntry *staticBuckets[4 ];
					 

    int numBuckets;			 

    int numEntries;			 

    int rebuildSize;			 

    int downShift;			 


    int mask;				 

    int keyType;			 





    Tcl_HashEntry *(*findProc)  (struct Tcl_HashTable *tablePtr, 
	    const  char *key)  ;
    Tcl_HashEntry *(*createProc)  (struct Tcl_HashTable *tablePtr,
	    const  char *key, int *newPtr)  ;
} Tcl_HashTable;

 




typedef struct Tcl_HashSearch {
    Tcl_HashTable *tablePtr;		 
    int nextIndex;			 

    Tcl_HashEntry *nextEntryPtr;	 

} Tcl_HashSearch;

 






 









 









 











 









struct Tcl_Event {
    Tcl_EventProc *proc;	 
    struct Tcl_Event *nextPtr;	 
};

 



typedef enum {
    TCL_QUEUE_TAIL, TCL_QUEUE_HEAD, TCL_QUEUE_MARK
} Tcl_QueuePosition;

 







 






typedef struct Tcl_Time {
    long sec;			 
    long usec;			 
} Tcl_Time;

 








 










 



typedef int	(Tcl_DriverBlockModeProc)  (
		    ClientData instanceData, int mode)  ;
typedef int	(Tcl_DriverCloseProc)  (ClientData instanceData,
		    Tcl_Interp *interp)  ;
typedef int	(Tcl_DriverInputProc)  (ClientData instanceData,
		    char *buf, int toRead, int *errorCodePtr)  ;
typedef int	(Tcl_DriverOutputProc)  (ClientData instanceData,
		    char *buf, int toWrite, int *errorCodePtr)  ;
typedef int	(Tcl_DriverSeekProc)  (ClientData instanceData,
		    long offset, int mode, int *errorCodePtr)  ;
typedef int	(Tcl_DriverSetOptionProc)  (
		    ClientData instanceData, Tcl_Interp *interp,
	            char *optionName, char *value)  ;
typedef int	(Tcl_DriverGetOptionProc)  (
		    ClientData instanceData, Tcl_Interp *interp,
		    char *optionName, Tcl_DString *dsPtr)  ;
typedef void	(Tcl_DriverWatchProc)  (
		    ClientData instanceData, int mask)  ;
typedef int	(Tcl_DriverGetHandleProc)  (
		    ClientData instanceData, int direction,
		    ClientData *handlePtr)  ;

 



typedef enum Tcl_EolTranslation {
    TCL_TRANSLATE_AUTO,			 
    TCL_TRANSLATE_CR,			 
    TCL_TRANSLATE_LF,			 
    TCL_TRANSLATE_CRLF			 
} Tcl_EolTranslation;

 







typedef struct Tcl_ChannelType {
    char *typeName;			 


    Tcl_DriverBlockModeProc *blockModeProc;
    					 

    Tcl_DriverCloseProc *closeProc;	 

    Tcl_DriverInputProc *inputProc;	 

    Tcl_DriverOutputProc *outputProc;	 

    Tcl_DriverSeekProc *seekProc;	 

    Tcl_DriverSetOptionProc *setOptionProc;
    					 
    Tcl_DriverGetOptionProc *getOptionProc;
    					 
    Tcl_DriverWatchProc *watchProc;	 

    Tcl_DriverGetHandleProc *getHandleProc;
					 

} Tcl_ChannelType;

 









 



typedef enum Tcl_PathType {
    TCL_PATH_ABSOLUTE,
    TCL_PATH_RELATIVE,
    TCL_PATH_VOLUME_RELATIVE
} Tcl_PathType;

 



extern  void		Tcl_AddErrorInfo  (Tcl_Interp *interp,
			    char *message)  ;
extern  void		Tcl_AddObjErrorInfo  (Tcl_Interp *interp,
			    char *message, int length)  ;
extern  void		Tcl_AllowExceptions  (Tcl_Interp *interp)  ;
extern  int		Tcl_AppendAllObjTypes  (
			    Tcl_Interp *interp, Tcl_Obj *objPtr)  ;
extern  void		Tcl_AppendElement  (Tcl_Interp *interp,
			    char *string)  ;
extern  void		Tcl_AppendResult  
			    ( Tcl_Interp *   interp , ...)   ;
extern  void		Tcl_AppendToObj  (Tcl_Obj *objPtr,
			    char *bytes, int length)  ;
extern  void		Tcl_AppendStringsToObj  
			    ( Tcl_Obj *   interp , ...)   ;
extern  int		Tcl_AppInit  (Tcl_Interp *interp)  ;
extern  Tcl_AsyncHandler	Tcl_AsyncCreate  (Tcl_AsyncProc *proc,
			    ClientData clientData)  ;
extern  void		Tcl_AsyncDelete  (Tcl_AsyncHandler async)  ;
extern  int		Tcl_AsyncInvoke  (Tcl_Interp *interp,
			    int code)  ;
extern  void		Tcl_AsyncMark  (Tcl_AsyncHandler async)  ;
extern  int		Tcl_AsyncReady  (void)  ;
extern  void		Tcl_BackgroundError  (Tcl_Interp *interp)  ;
extern  char		Tcl_Backslash  (const  char *src,
			    int *readPtr)  ;
extern  int		Tcl_BadChannelOption  (Tcl_Interp *interp,
			    char *optionName, char *optionList)  ;
extern  void		Tcl_CallWhenDeleted  (Tcl_Interp *interp,
			    Tcl_InterpDeleteProc *proc,
			    ClientData clientData)  ;
extern  void		Tcl_CancelIdleCall  (Tcl_IdleProc *idleProc,
			    ClientData clientData)  ;



extern  int		Tcl_Close  (Tcl_Interp *interp,
        		    Tcl_Channel chan)  ;
extern  int		Tcl_CommandComplete  (char *cmd)  ;
extern  char *		Tcl_Concat  (int argc, char **argv)  ;
extern  Tcl_Obj *	Tcl_ConcatObj  (int objc,
			    Tcl_Obj * const  objv[])  ;
extern  int		Tcl_ConvertCountedElement  (const  char *src,
			    int length, char *dst, int flags)  ;
extern  int		Tcl_ConvertElement  (const  char *src,
			    char *dst, int flags)  ;
extern  int		Tcl_ConvertToType  (Tcl_Interp *interp,
			    Tcl_Obj *objPtr, Tcl_ObjType *typePtr)  ;
extern  int		Tcl_CreateAlias  (Tcl_Interp *slave,
			    char *slaveCmd, Tcl_Interp *target,
        		    char *targetCmd, int argc, char **argv)  ;
extern  int		Tcl_CreateAliasObj  (Tcl_Interp *slave,
			    char *slaveCmd, Tcl_Interp *target,
        		    char *targetCmd, int objc,
		            Tcl_Obj * const  objv[])  ;
extern  Tcl_Channel	Tcl_CreateChannel  (
    			    Tcl_ChannelType *typePtr, char *chanName,
                            ClientData instanceData, int mask)  ;
extern  void		Tcl_CreateChannelHandler  (
			    Tcl_Channel chan, int mask,
                            Tcl_ChannelProc *proc, ClientData clientData)  ;
extern  void		Tcl_CreateCloseHandler  (
			    Tcl_Channel chan, Tcl_CloseProc *proc,
                            ClientData clientData)  ;
extern  Tcl_Command	Tcl_CreateCommand  (Tcl_Interp *interp,
			    char *cmdName, Tcl_CmdProc *proc,
			    ClientData clientData,
			    Tcl_CmdDeleteProc *deleteProc)  ;
extern  void		Tcl_CreateEventSource  (
			    Tcl_EventSetupProc *setupProc,
			    Tcl_EventCheckProc *checkProc,
			    ClientData clientData)  ;
extern  void		Tcl_CreateExitHandler  (Tcl_ExitProc *proc,
			    ClientData clientData)  ;
extern  void		Tcl_CreateFileHandler  (
    			    int fd, int mask, Tcl_FileProc *proc,
			    ClientData clientData)  ;
extern  Tcl_Interp *	Tcl_CreateInterp  (void)  ;
extern  void		Tcl_CreateMathFunc  (Tcl_Interp *interp,
			    char *name, int numArgs, Tcl_ValueType *argTypes,
			    Tcl_MathProc *proc, ClientData clientData)  ;
extern  Tcl_Command	Tcl_CreateObjCommand  (
			    Tcl_Interp *interp, char *cmdName,
			    Tcl_ObjCmdProc *proc, ClientData clientData,
			    Tcl_CmdDeleteProc *deleteProc)  ;
extern  Tcl_Interp *	Tcl_CreateSlave  (Tcl_Interp *interp,
		            char *slaveName, int isSafe)  ;
extern  Tcl_TimerToken	Tcl_CreateTimerHandler  (int milliseconds,
			    Tcl_TimerProc *proc, ClientData clientData)  ;
extern  Tcl_Trace	Tcl_CreateTrace  (Tcl_Interp *interp,
			    int level, Tcl_CmdTraceProc *proc,
			    ClientData clientData)  ;
extern  char *		Tcl_DbCkalloc  (unsigned int size,
			    char *file, int line)  ;
extern  int		Tcl_DbCkfree  (char *ptr,
			    char *file, int line)  ;
extern  char *		Tcl_DbCkrealloc  (char *ptr,
			    unsigned int size, char *file, int line)  ;
extern  void		Tcl_DbDecrRefCount  (Tcl_Obj *objPtr,
			    char *file, int line)  ;
extern  void		Tcl_DbIncrRefCount  (Tcl_Obj *objPtr,
			    char *file, int line)  ;
extern  int		Tcl_DbIsShared  (Tcl_Obj *objPtr,
			    char *file, int line)  ;
extern  Tcl_Obj *	Tcl_DbNewBooleanObj  (int boolValue,
                            char *file, int line)  ;
extern  Tcl_Obj *	Tcl_DbNewDoubleObj  (double doubleValue,
                            char *file, int line)  ;
extern  Tcl_Obj *	Tcl_DbNewListObj  (int objc,
			    Tcl_Obj * const  objv[], char *file, int line)  ;
extern  Tcl_Obj *	Tcl_DbNewLongObj  (long longValue,
                            char *file, int line)  ;
extern  Tcl_Obj *	Tcl_DbNewObj  (char *file, int line)  ;
extern  Tcl_Obj *	Tcl_DbNewStringObj  (char *bytes,
			    int length, char *file, int line)  ;
extern  void		Tcl_DeleteAssocData  (Tcl_Interp *interp,
                            char *name)  ;
extern  int		Tcl_DeleteCommand  (Tcl_Interp *interp,
			    char *cmdName)  ;
extern  int		Tcl_DeleteCommandFromToken  (
			    Tcl_Interp *interp, Tcl_Command command)  ;
extern  void		Tcl_DeleteChannelHandler  (
    			    Tcl_Channel chan, Tcl_ChannelProc *proc,
                            ClientData clientData)  ;
extern  void		Tcl_DeleteCloseHandler  (
			    Tcl_Channel chan, Tcl_CloseProc *proc,
                            ClientData clientData)  ;
extern  void		Tcl_DeleteEvents  (
			    Tcl_EventDeleteProc *proc,
                            ClientData clientData)  ;
extern  void		Tcl_DeleteEventSource  (
			    Tcl_EventSetupProc *setupProc,
			    Tcl_EventCheckProc *checkProc,
			    ClientData clientData)  ;
extern  void		Tcl_DeleteExitHandler  (Tcl_ExitProc *proc,
			    ClientData clientData)  ;
extern  void		Tcl_DeleteFileHandler  (int fd)  ;
extern  void		Tcl_DeleteHashEntry  (
			    Tcl_HashEntry *entryPtr)  ;
extern  void		Tcl_DeleteHashTable  (
			    Tcl_HashTable *tablePtr)  ;
extern  void		Tcl_DeleteInterp  (Tcl_Interp *interp)  ;
extern  void		Tcl_DeleteTimerHandler  (
			    Tcl_TimerToken token)  ;
extern  void		Tcl_DeleteTrace  (Tcl_Interp *interp,
			    Tcl_Trace trace)  ;
extern  void		Tcl_DetachPids  (int numPids, Tcl_Pid *pidPtr)  ;
extern  void		Tcl_DontCallWhenDeleted  (
			    Tcl_Interp *interp, Tcl_InterpDeleteProc *proc,
			    ClientData clientData)  ;
extern  int		Tcl_DoOneEvent  (int flags)  ;
extern  void		Tcl_DoWhenIdle  (Tcl_IdleProc *proc,
			    ClientData clientData)  ;
extern  char *		Tcl_DStringAppend  (Tcl_DString *dsPtr,
			    const  char *string, int length)  ;
extern  char *		Tcl_DStringAppendElement  (
			    Tcl_DString *dsPtr, const  char *string)  ;
extern  void		Tcl_DStringEndSublist  (Tcl_DString *dsPtr)  ;
extern  void		Tcl_DStringFree  (Tcl_DString *dsPtr)  ;
extern  void		Tcl_DStringGetResult  (Tcl_Interp *interp,
			    Tcl_DString *dsPtr)  ;
extern  void		Tcl_DStringInit  (Tcl_DString *dsPtr)  ;
extern  void		Tcl_DStringResult  (Tcl_Interp *interp,
			    Tcl_DString *dsPtr)  ;
extern  void		Tcl_DStringSetLength  (Tcl_DString *dsPtr,
			    int length)  ;
extern  void		Tcl_DStringStartSublist  (
			    Tcl_DString *dsPtr)  ;
extern  Tcl_Obj *	Tcl_DuplicateObj  (Tcl_Obj *objPtr)  ;
extern  int		Tcl_Eof  (Tcl_Channel chan)  ;
extern  char *		Tcl_ErrnoId  (void)  ;
extern  char *		Tcl_ErrnoMsg  (int err)  ;
extern  int		Tcl_Eval  (Tcl_Interp *interp,
			    char *string)  ;
extern  int		Tcl_EvalFile  (Tcl_Interp *interp,
			    char *fileName)  ;
extern  void		Tcl_EventuallyFree  (ClientData clientData,
			    Tcl_FreeProc *freeProc)  ;
extern  int		Tcl_EvalObj  (Tcl_Interp *interp,
			    Tcl_Obj *objPtr)  ;
extern  void		Tcl_Exit  (int status)  ;
extern  int		Tcl_ExposeCommand  (Tcl_Interp *interp,
        		    char *hiddenCmdToken, char *cmdName)  ;
extern  int		Tcl_ExprBoolean  (Tcl_Interp *interp,
			    char *string, int *ptr)  ;
extern  int		Tcl_ExprBooleanObj  (Tcl_Interp *interp,
			    Tcl_Obj *objPtr, int *ptr)  ;
extern  int		Tcl_ExprDouble  (Tcl_Interp *interp,
			    char *string, double *ptr)  ;
extern  int		Tcl_ExprDoubleObj  (Tcl_Interp *interp,
			    Tcl_Obj *objPtr, double *ptr)  ;
extern  int		Tcl_ExprLong  (Tcl_Interp *interp,
			    char *string, long *ptr)  ;
extern  int		Tcl_ExprLongObj  (Tcl_Interp *interp,
			    Tcl_Obj *objPtr, long *ptr)  ;
extern  int		Tcl_ExprObj  (Tcl_Interp *interp,
			    Tcl_Obj *objPtr, Tcl_Obj **resultPtrPtr)  ;
extern  int		Tcl_ExprString  (Tcl_Interp *interp,
			    char *string)  ;
extern  void		Tcl_Finalize  (void)  ;
extern  void		Tcl_FindExecutable  (char *argv0)  ;
extern  Tcl_HashEntry *	Tcl_FirstHashEntry  (
			    Tcl_HashTable *tablePtr,
			    Tcl_HashSearch *searchPtr)  ;
extern  int		Tcl_Flush  (Tcl_Channel chan)  ;
extern  void		TclFreeObj  (Tcl_Obj *objPtr)  ;
extern  void		Tcl_FreeResult  (Tcl_Interp *interp)  ;
extern  int		Tcl_GetAlias  (Tcl_Interp *interp,
       			    char *slaveCmd, Tcl_Interp **targetInterpPtr,
                            char **targetCmdPtr, int *argcPtr,
			    char ***argvPtr)  ;
extern  int		Tcl_GetAliasObj  (Tcl_Interp *interp,
       			    char *slaveCmd, Tcl_Interp **targetInterpPtr,
                            char **targetCmdPtr, int *objcPtr,
			    Tcl_Obj ***objv)  ;
extern  ClientData	Tcl_GetAssocData  (Tcl_Interp *interp,
                            char *name, Tcl_InterpDeleteProc **procPtr)  ;
extern  int		Tcl_GetBoolean  (Tcl_Interp *interp,
			    char *string, int *boolPtr)  ;
extern  int		Tcl_GetBooleanFromObj  (
			    Tcl_Interp *interp, Tcl_Obj *objPtr,
			    int *boolPtr)  ;
extern  Tcl_Channel	Tcl_GetChannel  (Tcl_Interp *interp,
	        	    char *chanName, int *modePtr)  ;
extern  int		Tcl_GetChannelBufferSize  (
    			    Tcl_Channel chan)  ;
extern  int		Tcl_GetChannelHandle  (Tcl_Channel chan,
	        	    int direction, ClientData *handlePtr)  ;
extern  ClientData	Tcl_GetChannelInstanceData  (
    			    Tcl_Channel chan)  ;
extern  int		Tcl_GetChannelMode  (Tcl_Channel chan)  ;
extern  char *		Tcl_GetChannelName  (Tcl_Channel chan)  ;
extern  int		Tcl_GetChannelOption  (Tcl_Interp *interp,
			    Tcl_Channel chan, char *optionName,
			    Tcl_DString *dsPtr)  ;
extern  Tcl_ChannelType * Tcl_GetChannelType  (Tcl_Channel chan)  ;
extern  int		Tcl_GetCommandInfo  (Tcl_Interp *interp,
			    char *cmdName, Tcl_CmdInfo *infoPtr)  ;
extern  char *		Tcl_GetCommandName  (Tcl_Interp *interp,
			    Tcl_Command command)  ;
extern  char *		Tcl_GetCwd  (char *buf, int len)  ;
extern  int		Tcl_GetDouble  (Tcl_Interp *interp,
			    char *string, double *doublePtr)  ;
extern  int		Tcl_GetDoubleFromObj  (
			    Tcl_Interp *interp, Tcl_Obj *objPtr,
			    double *doublePtr)  ;
extern  int		Tcl_GetErrno  (void)  ;
extern  int		Tcl_GetErrorLine  (Tcl_Interp *interp)  ;
extern  char *		Tcl_GetHostName  (void)  ;
extern  int		Tcl_GetIndexFromObj  (Tcl_Interp *interp,
			    Tcl_Obj *objPtr, char **tablePtr, char *msg,
			    int flags, int *indexPtr)  ;
extern  int		Tcl_GetInt  (Tcl_Interp *interp,
			    char *string, int *intPtr)  ;
extern  int		Tcl_GetInterpPath  (Tcl_Interp *askInterp,
			    Tcl_Interp *slaveInterp)  ;
extern  int		Tcl_GetIntFromObj  (Tcl_Interp *interp,
			    Tcl_Obj *objPtr, int *intPtr)  ;
extern  int		Tcl_GetLongFromObj  (Tcl_Interp *interp,
			    Tcl_Obj *objPtr, long *longPtr)  ;
extern  Tcl_Interp *	Tcl_GetMaster  (Tcl_Interp *interp)  ;
extern  Tcl_Obj *	Tcl_GetObjResult  (Tcl_Interp *interp)  ;
extern  Tcl_ObjType *	Tcl_GetObjType  (char *typeName)  ;
extern  int		Tcl_GetOpenFile  (Tcl_Interp *interp,
			    char *string, int write, int checkUsage,
			    ClientData *filePtr)  ;
extern  Tcl_Command	Tcl_GetOriginalCommand  (
			    Tcl_Command command)  ;
extern  Tcl_PathType	Tcl_GetPathType  (char *path)  ;
extern  int		Tcl_Gets  (Tcl_Channel chan,
        		    Tcl_DString *dsPtr)  ;
extern  int		Tcl_GetsObj  (Tcl_Channel chan,
        		    Tcl_Obj *objPtr)  ;
extern  int		Tcl_GetServiceMode  (void)  ;
extern  Tcl_Interp *	Tcl_GetSlave  (Tcl_Interp *interp,
			    char *slaveName)  ;
extern  Tcl_Channel	Tcl_GetStdChannel  (int type)  ;
extern  char *		Tcl_GetStringFromObj  (Tcl_Obj *objPtr,
			    int *lengthPtr)  ;
extern  char *		Tcl_GetStringResult  (Tcl_Interp *interp)  ;
extern  char *		Tcl_GetVar  (Tcl_Interp *interp,
			    char *varName, int flags)  ;
extern  char *		Tcl_GetVar2  (Tcl_Interp *interp,
			    char *part1, char *part2, int flags)  ;
extern  int		Tcl_GlobalEval  (Tcl_Interp *interp,
			    char *command)  ;
extern  int		Tcl_GlobalEvalObj  (Tcl_Interp *interp,
			    Tcl_Obj *objPtr)  ;
extern  char *		Tcl_HashStats  (Tcl_HashTable *tablePtr)  ;
extern  int		Tcl_HideCommand  (Tcl_Interp *interp,
		            char *cmdName, char *hiddenCmdToken)  ;
extern  int		Tcl_Init  (Tcl_Interp *interp)  ;
extern  void		Tcl_InitHashTable  (Tcl_HashTable *tablePtr,
			    int keyType)  ;
extern  void		Tcl_InitMemory  (Tcl_Interp *interp)  ;
extern  int		Tcl_InputBlocked  (Tcl_Channel chan)  ;
extern  int		Tcl_InputBuffered  (Tcl_Channel chan)  ;
extern  int		Tcl_InterpDeleted  (Tcl_Interp *interp)  ;
extern  int		Tcl_IsSafe  (Tcl_Interp *interp)  ;
extern  void		Tcl_InvalidateStringRep  (
			    Tcl_Obj *objPtr)  ;
extern  char *		Tcl_JoinPath  (int argc, char **argv,
			    Tcl_DString *resultPtr)  ;
extern  int		Tcl_LinkVar  (Tcl_Interp *interp,
			    char *varName, char *addr, int type)  ;
extern  int		Tcl_ListObjAppendList  (
			    Tcl_Interp *interp, Tcl_Obj *listPtr, 
			    Tcl_Obj *elemListPtr)  ;
extern  int		Tcl_ListObjAppendElement  (
			    Tcl_Interp *interp, Tcl_Obj *listPtr,
			    Tcl_Obj *objPtr)  ;
extern  int		Tcl_ListObjGetElements  (
			    Tcl_Interp *interp, Tcl_Obj *listPtr,
			    int *objcPtr, Tcl_Obj ***objvPtr)  ;
extern  int		Tcl_ListObjIndex  (Tcl_Interp *interp,
			    Tcl_Obj *listPtr, int index, 
			    Tcl_Obj **objPtrPtr)  ;
extern  int		Tcl_ListObjLength  (Tcl_Interp *interp,
			    Tcl_Obj *listPtr, int *intPtr)  ;
extern  int		Tcl_ListObjReplace  (Tcl_Interp *interp,
			    Tcl_Obj *listPtr, int first, int count,
			    int objc, Tcl_Obj * const  objv[])  ;
extern  void		Tcl_Main  (int argc, char **argv, int fd,
			    Tcl_AppInitProc *appInitProc)  ;
 
extern  int		Tcl_MakeSafe  (Tcl_Interp *interp)  ;
extern  Tcl_Channel	Tcl_MakeTcpClientChannel  (
    			    ClientData tcpSocket)  ;
extern  char *		Tcl_Merge  (int argc, char **argv)  ;
extern  Tcl_HashEntry *	Tcl_NextHashEntry  (
			    Tcl_HashSearch *searchPtr)  ;
extern  void		Tcl_NotifyChannel  (Tcl_Channel channel,
			    int mask)  ;
extern  Tcl_Obj *	Tcl_ObjGetVar2  (Tcl_Interp *interp,
			    Tcl_Obj *part1Ptr, Tcl_Obj *part2Ptr,
			    int flags)  ;
extern  Tcl_Obj *	Tcl_ObjSetVar2  (Tcl_Interp *interp,
			    Tcl_Obj *part1Ptr, Tcl_Obj *part2Ptr,
			    Tcl_Obj *newValuePtr, int flags)  ;
extern  Tcl_Channel	Tcl_OpenCommandChannel  (
    			    Tcl_Interp *interp, int argc, char **argv,
			    int flags)  ;
extern  Tcl_Channel	Tcl_OpenFileChannel  (Tcl_Interp *interp,
        		    char *fileName, char *modeString,
                            int permissions)  ;
extern  Tcl_Channel	Tcl_OpenTcpClient  (Tcl_Interp *interp,
			    int port, char *address, char *myaddr,
		            int myport, int async)  ;
extern  Tcl_Channel	Tcl_OpenTcpServer  (Tcl_Interp *interp,
		            int port, char *host,
        		    Tcl_TcpAcceptProc *acceptProc,
			    ClientData callbackData)  ;
extern  char *		Tcl_ParseVar  (Tcl_Interp *interp,
			    char *string, char **termPtr)  ;
extern  int		Tcl_PkgProvide  (Tcl_Interp *interp,
			    char *name, char *version)  ;
extern  char *		Tcl_PkgRequire  (Tcl_Interp *interp,
			    char *name, char *version, int exact)  ;
extern  char *		Tcl_PosixError  (Tcl_Interp *interp)  ;
extern  void		Tcl_Preserve  (ClientData data)  ;
extern  void		Tcl_PrintDouble  (Tcl_Interp *interp,
			    double value, char *dst)  ;
extern  int		Tcl_PutEnv  (const  char *string)  ;
extern  void		Tcl_QueueEvent  (Tcl_Event *evPtr,
			    Tcl_QueuePosition position)  ;
extern  int		Tcl_Read  (Tcl_Channel chan,
	        	    char *bufPtr, int toRead)  ;
extern  void		Tcl_ReapDetachedProcs  (void)  ;
extern  int		Tcl_RecordAndEval  (Tcl_Interp *interp,
			    char *cmd, int flags)  ;
extern  int		Tcl_RecordAndEvalObj  (Tcl_Interp *interp,
			    Tcl_Obj *cmdPtr, int flags)  ;
extern  Tcl_RegExp	Tcl_RegExpCompile  (Tcl_Interp *interp,
			    char *string)  ;
extern  int		Tcl_RegExpExec  (Tcl_Interp *interp,
			    Tcl_RegExp regexp, char *string, char *start)  ;
extern  int		Tcl_RegExpMatch  (Tcl_Interp *interp,
			    char *string, char *pattern)  ;
extern  void		Tcl_RegExpRange  (Tcl_RegExp regexp,
			    int index, char **startPtr, char **endPtr)  ;
extern  void		Tcl_RegisterChannel  (Tcl_Interp *interp,
	        	    Tcl_Channel chan)  ;
extern  void		Tcl_RegisterObjType  (
			    Tcl_ObjType *typePtr)  ;
extern  void		Tcl_Release  (ClientData clientData)  ;
extern  void		Tcl_RestartIdleTimer  (void)  ;
extern  void		Tcl_ResetResult  (Tcl_Interp *interp)  ;

extern  int		Tcl_ScanCountedElement  (const  char *string,
			    int length, int *flagPtr)  ;
extern  int		Tcl_ScanElement  (const  char *string,
			    int *flagPtr)  ;
extern  int		Tcl_Seek  (Tcl_Channel chan,
        		    int offset, int mode)  ;
extern  int		Tcl_ServiceAll  (void)  ;
extern  int		Tcl_ServiceEvent  (int flags)  ;
extern  void		Tcl_SetAssocData  (Tcl_Interp *interp,
                            char *name, Tcl_InterpDeleteProc *proc,
                            ClientData clientData)  ;
extern  void		Tcl_SetBooleanObj  (Tcl_Obj *objPtr, 
			    int boolValue)  ;
extern  void		Tcl_SetChannelBufferSize  (
			    Tcl_Channel chan, int sz)  ;
extern  int		Tcl_SetChannelOption  (
			    Tcl_Interp *interp, Tcl_Channel chan,
	        	    char *optionName, char *newValue)  ;
extern  int		Tcl_SetCommandInfo  (Tcl_Interp *interp,
			    char *cmdName, Tcl_CmdInfo *infoPtr)  ;
extern  void		Tcl_SetDoubleObj  (Tcl_Obj *objPtr, 
			    double doubleValue)  ;
extern  void		Tcl_SetErrno  (int err)  ;
extern  void		Tcl_SetErrorCode  
    			    ( Tcl_Interp *   arg1 , ...)   ;
extern  void		Tcl_SetIntObj  (Tcl_Obj *objPtr, 
			    int intValue)  ;
extern  void		Tcl_SetListObj  (Tcl_Obj *objPtr, 
			    int objc, Tcl_Obj * const  objv[])  ;
extern  void		Tcl_SetLongObj  (Tcl_Obj *objPtr, 
			    long longValue)  ;
extern  void		Tcl_SetMaxBlockTime  (Tcl_Time *timePtr)  ;
extern  void		Tcl_SetObjErrorCode  (Tcl_Interp *interp,
			    Tcl_Obj *errorObjPtr)  ;
extern  void		Tcl_SetObjLength  (Tcl_Obj *objPtr,
			    int length)  ;
extern  void		Tcl_SetObjResult  (Tcl_Interp *interp,
			    Tcl_Obj *resultObjPtr)  ;
extern  void		Tcl_SetPanicProc  (void (*proc)
			     ( char *    format , ...)   )  ;
extern  int		Tcl_SetRecursionLimit  (Tcl_Interp *interp,
			    int depth)  ;
extern  void		Tcl_SetResult  (Tcl_Interp *interp,
			    char *string, Tcl_FreeProc *freeProc)  ;
extern  int		Tcl_SetServiceMode  (int mode)  ;
extern  void		Tcl_SetStdChannel  (Tcl_Channel channel,
			    int type)  ;
extern  void		Tcl_SetStringObj  (Tcl_Obj *objPtr, 
			    char *bytes, int length)  ;
extern  void		Tcl_SetTimer  (Tcl_Time *timePtr)  ;
extern  char *		Tcl_SetVar  (Tcl_Interp *interp,
			    char *varName, char *newValue, int flags)  ;
extern  char *		Tcl_SetVar2  (Tcl_Interp *interp,
			    char *part1, char *part2, char *newValue,
			    int flags)  ;
extern  char *		Tcl_SignalId  (int sig)  ;
extern  char *		Tcl_SignalMsg  (int sig)  ;
extern  void		Tcl_Sleep  (int ms)  ;
extern  void		Tcl_SourceRCFile  (Tcl_Interp *interp)  ;
extern  int		Tcl_SplitList  (Tcl_Interp *interp,
			    char *list, int *argcPtr, char ***argvPtr)  ;
extern  void		Tcl_SplitPath  (char *path,
			    int *argcPtr, char ***argvPtr)  ;
extern  void		Tcl_StaticPackage  (Tcl_Interp *interp,
			    char *pkgName, Tcl_PackageInitProc *initProc,
			    Tcl_PackageInitProc *safeInitProc)  ;
extern  int		Tcl_StringMatch  (char *string,
			    char *pattern)  ;
extern  int		Tcl_Tell  (Tcl_Channel chan)  ;

extern  int		Tcl_TraceVar  (Tcl_Interp *interp,
			    char *varName, int flags, Tcl_VarTraceProc *proc,
			    ClientData clientData)  ;
extern  int		Tcl_TraceVar2  (Tcl_Interp *interp,
			    char *part1, char *part2, int flags,
			    Tcl_VarTraceProc *proc, ClientData clientData)  ;
extern  char *		Tcl_TranslateFileName  (Tcl_Interp *interp,
			    char *name, Tcl_DString *bufferPtr)  ;
extern  int		Tcl_Ungets  (Tcl_Channel chan, char *str,
			    int len, int atHead)  ;
extern  void		Tcl_UnlinkVar  (Tcl_Interp *interp,
			    char *varName)  ;
extern  int		Tcl_UnregisterChannel  (Tcl_Interp *interp,
			    Tcl_Channel chan)  ;
extern  int		Tcl_UnsetVar  (Tcl_Interp *interp,
			    char *varName, int flags)  ;
extern  int		Tcl_UnsetVar2  (Tcl_Interp *interp,
			    char *part1, char *part2, int flags)  ;
extern  void		Tcl_UntraceVar  (Tcl_Interp *interp,
			    char *varName, int flags, Tcl_VarTraceProc *proc,
			    ClientData clientData)  ;
extern  void		Tcl_UntraceVar2  (Tcl_Interp *interp,
			    char *part1, char *part2, int flags,
			    Tcl_VarTraceProc *proc, ClientData clientData)  ;
extern  void		Tcl_UpdateLinkedVar  (Tcl_Interp *interp,
			    char *varName)  ;
extern  int		Tcl_UpVar  (Tcl_Interp *interp,
			    char *frameName, char *varName,
			    char *localName, int flags)  ;
extern  int		Tcl_UpVar2  (Tcl_Interp *interp,
			    char *frameName, char *part1, char *part2,
			    char *localName, int flags)  ;
extern  int		Tcl_VarEval  
    			    ( Tcl_Interp *   interp , ...)   ;
extern  ClientData	Tcl_VarTraceInfo  (Tcl_Interp *interp,
			    char *varName, int flags,
			    Tcl_VarTraceProc *procPtr,
			    ClientData prevClientData)  ;
extern  ClientData	Tcl_VarTraceInfo2  (Tcl_Interp *interp,
			    char *part1, char *part2, int flags,
			    Tcl_VarTraceProc *procPtr,
			    ClientData prevClientData)  ;
extern  int		Tcl_WaitForEvent  (Tcl_Time *timePtr)  ;
extern  Tcl_Pid		Tcl_WaitPid  (Tcl_Pid pid, int *statPtr, 
			    int options)  ;
extern  int		Tcl_Write  (Tcl_Channel chan,
			    char *s, int slen)  ;
extern  void		Tcl_WrongNumArgs  (Tcl_Interp *interp,
			    int objc, Tcl_Obj * const  objv[], char *message)  ;



# 22 "../../../appl/DBG/inc/Dbg_ei.h" 2

# 1 "../../../infra/SYS/target/h/tickLib.h" 1
 

 

 




















 



extern void 	tickAnnounce (void);
extern void 	tickSet (ULONG ticks);
extern ULONG 	tickGet (void);














# 23 "../../../appl/DBG/inc/Dbg_ei.h" 2


 





typedef INT2 DEBUG_LEVEL;

 




extern DEBUG_LEVEL DBG_SetDebugLevel(DEBUG_LEVEL level);

 




extern DEBUG_LEVEL DBG_GetDebugLevel();

 











extern STATUS DBG_debug(int argc, char **argv);

 

















 




















extern UINT4 DBG_ReadRegister(UINT1 * pAddress, INT4 num);
extern void DBG_WriteRegister(UINT1 * pAddress, UINT1 value);



 





 






typedef struct {
    char levelName[16 ];
    UINT8  initVal;          
    char levelDesc[256 ];
}DBG_LEVEL_INFO_T;

typedef struct {
    char module[16 ];
    DBG_LEVEL_INFO_T levels[32 ];
}DBG_INFO_T;

 
typedef struct {
    UINT32 level[253];
    UINT32 warmFlag;   
    UINT32 nvramMaxLen;
    UINT32 nvramLen;
    char nvram[1];   
}DBG_NVRAM_T;
extern DBG_NVRAM_T* gpDbgNvram;

typedef UINT16 DBG_MODULE_T;

 





void DBG_Init(void);

 









DBG_MODULE_T DBG_RegisterModule(DBG_INFO_T *pInfo);

 









void DBG_SetLevel(char* module, char* level, BOOL set);
 








void DBG_SetAllLevels(char* module, BOOL set);
 










void DBG_SetLevelByNum(DBG_MODULE_T module, UINT16 level, BOOL set);

 




void DBG_ShowModules(void);

 






void DBG_ShowModuleInfo(char* module, BOOL full);

 








BOOL DBG_GetLevelByName(char* module, char* level);
 









BOOL DBG_GetLevel(DBG_MODULE_T module, UINT16 level);
 











BOOL DBG_GetLevel2And(DBG_MODULE_T module, UINT16 level1, UINT16 level2);
 











BOOL DBG_GetLevel2Or(DBG_MODULE_T module, UINT16 level1, UINT16 level2);

 
void DBG_NvResetLevel(void);
 
int DBG_NvResetBuffer(UINT32 base, UINT32 bufLen);
 
void DBG_NvPrintBuffer(void);
 

void DBG_NvPrintBufferBack(void);
void DBG_NvSetLevel(char* module, char* level, BOOL set);
void DBG_NvSetAllLevels(char* module, BOOL set);
void DBG_NvSetLevelByNum(DBG_MODULE_T module, UINT16 level, BOOL set);
void DBG_NvShowModules(void);
void DBG_NvShowModuleInfo(char* module, BOOL full);
BOOL DBG_NvGetLevelByName(char* module, char* level);
BOOL DBG_NvGetLevel(DBG_MODULE_T module, UINT16 level);
BOOL DBG_NvGetLevel2And(DBG_MODULE_T module, UINT16 level1, UINT16 level2);
BOOL DBG_NvGetLevel2Or(DBG_MODULE_T module, UINT16 level1, UINT16 level2);






  














extern int DBG_TclAppInit(Tcl_Interp *interp);




# 21 "../../../appl/CAH/inc/cah_defs.h" 2

# 1 "../../../appl/CTD/inc/ctd_drv.h" 1
 

















# 1 "../../../appl/CTD/inc/ctd_tm.h" 1
 
 
 
 
 
 
 
 
 
 
 
 







# 1 "../../../appl/CMM/inc/cm_command.h" 1
 











 




# 1 "../../../include/cmnd.h" 1



# 1 "../../../include/config_limits.h" 1



#ident  "$Header: /home/cvsroot1/LT/cah_126e1.i,v 1.1 2009/04/15 20:25:11 wydeng Exp $"


 



 

 







 


 


 



 



 






# 4 "../../../include/cmnd.h" 2


 



 

















































































































 


 








 




 


typedef struct {
    unsigned short slot;         
    unsigned short pslot;        
    unsigned int applNumber; 
    unsigned int instance;
} applId_tt;

typedef struct {
    unsigned char packet_type;   
    unsigned char packet_flags;  
    unsigned short packet_sequence;   
    applId_tt dest;   
    applId_tt src;  

    int command;      
    int length;                  
    unsigned short reSendSynMsgMax; 

    unsigned short  sendSynMsgTimerLen;   

    unsigned short  wCheckSum;   
    unsigned char aExt[2];
    void *handle;   
     
} command_tt;



 







# 1 "../../../include/cmnds/cmnd_common.h" 1



#ident  "%full_filespec: cmnd_common.h#11:incl:1 %"

 















































 














 




typedef struct _CM_PHYCARDPARA_T {
    unsigned int style;
    unsigned int function;
    unsigned int hwVersion;
    unsigned int option;
    unsigned int slot;
    char   serialNum[18 ];
    char   manuDate[9 ];
    char   prodCode[11 ]; 
    char   oemCode[11 ];
    char   simCode[26 ];
    char   bootDate[41 ];
    char   bootVersion[17 ];
    char   hardVersion[17 ];
    char   softVersion[33 ];
    char   fusionVersion[17 ];
    char   fpgaVersion[(17  * 15) ];	
} CM_PHYCARDPARA_T;

typedef struct _CM_INFRA_BOOT_ACK_T
{
    int    m_boot; 
}CM_INFRA_BOOT_ACK_T;

typedef struct _CM_LOAD_OBJ_T
{
    unsigned int slot;
}CM_LOAD_OBJ_T;

typedef struct cmndFmtApplState {
    command_tt head;
    int state;
    int applOptions;
} cmndFmtApplState_t;

 



























# 146 "../../../include/cmnds/cmnd_common.h"


# 157 "../../../include/cmnds/cmnd_common.h"



# 180 "../../../include/cmnd.h" 2




# 18 "../../../appl/CMM/inc/cm_command.h" 2



# 1 "../../../include/dtl_ei.h" 1
 






































typedef UINT16 DTL_MODULE_T;

typedef enum
{
    DTL_NOTHING_OPEN = 0,       
    DTL_REALTIME_PRINT,         
    DTL_RAM_SAVE,               
    DTL_FLASH_SAVE,              
    DTL_FLASH_SAVE_DIRECTLY,    
                                
}DTL_LEVEL_MODE_T;

typedef struct
{
    char                 levelName[32 ];
    DTL_LEVEL_MODE_T     levelControl;         
    UINT16               levelCount;            
    char                 levelDesc[128 ];
}DTL_LEVEL_INFO_T;

typedef struct 
{
    char                module[32 ];
    DTL_LEVEL_INFO_T    levels[32 ];
}DTL_MOD_INFO_T;

















extern DTL_MODULE_T  DTL_RegisterModule(DTL_MOD_INFO_T *pInfo, BOOL timeStampEnable);
extern void          DTL_PrintProc(DTL_MODULE_T module, UINT8 level, char* fileName, int line,  char *fmt, ...);
extern void          DTL_LoadModule(void);







# 21 "../../../appl/CMM/inc/cm_command.h" 2



# 1 "../../../include/cmnds/cmnd_coll.h" 1
 
 
 
 
 
 
 
 



# 1 "../../../include/globals.h" 1
#ident  "%full_filespec: globals.h#17:incl:1 %"







# 1 "../../../include/tmd.h" 1
 
 
 
 
 
 
 
 
 



















































































































































































 
# 1 "../../../infra/SYS/target/h/sys/socket.h" 1
 

 

 

 


































 











































 



 








 















 














 






struct opthdr {
	long 	level;			 
	long	name;			 
	long	len;			 
};




 


struct	linger {
	int	l_onoff;		 
	int	l_linger;		 
};

 




 





































 



struct sockaddr {
	u_char	sa_len;			 
	u_char	sa_family;		 
	char	sa_data[14];		 
};

typedef struct sockaddr SOCKADDR;

 



struct sockproto {
	u_short	sp_family;		 
	u_short	sp_protocol;		 
};

 

































 










# 290 "../../../infra/SYS/target/h/sys/socket.h"

 



















 




extern UINT32 somaxconn;

 



struct msghdr {
	caddr_t	msg_name;		 
	u_int	msg_namelen;		 
	struct	iovec *msg_iov;		 
	u_int	msg_iovlen;		 
	caddr_t	msg_control;		 
	u_int	msg_controllen;		 
 


	int	msg_flags;		 
};
















 





struct cmsghdr {
	u_int	cmsg_len;		 
	int	cmsg_level;		 
	int	cmsg_type;		 
 
};

 



 






 







 











# 190 "../../../include/tmd.h" 2

# 1 "../../../infra/SYS/target/h/sys/times.h" 1
 

 

 





























 




struct timeval
    {
    long tv_sec;	 
    long tv_usec;	 
    };

struct timezone
    {
    int	tz_minuteswest;	 
    int	tz_dsttime;	 
    };












# 191 "../../../include/tmd.h" 2

# 1 "../../../infra/SYS/target/h/msgQLib.h" 1
 

 

 































 





 







 




 

typedef struct msg_q *MSG_Q_ID;	 

typedef struct			 
    {
    int     numMsgs;		 
    int     numTasks;		 

    int     sendTimeouts;	 
    int     recvTimeouts;	 

    int     options;		 
    int     maxMsgs;		 
    int     maxMsgLength;	 

    int     taskIdListMax;	 
    int *   taskIdList;		 

    int     msgListMax;		 
    char ** msgPtrList;		 
    int *   msgLenList;		 

    } MSG_Q_INFO;

 

 










 



extern STATUS 	msgQLibInit (void);
extern MSG_Q_ID msgQCreate (int maxMsgs, int maxMsgLength, int options);
extern STATUS 	msgQDelete (MSG_Q_ID msgQId);
extern STATUS 	msgQSend (MSG_Q_ID msgQId, char *buffer, UINT nBytes,
			  int timeout, int priority);
extern int 	msgQReceive (MSG_Q_ID msgQId, char *buffer, UINT maxNBytes,
			     int timeout);
extern STATUS 	msgQInfoGet (MSG_Q_ID msgQId, MSG_Q_INFO *pInfo);
extern int 	msgQNumMsgs (MSG_Q_ID msgQId);
extern void 	msgQShowInit (void);
extern STATUS 	msgQShow (MSG_Q_ID msgQId, int level);

# 122 "../../../infra/SYS/target/h/msgQLib.h"







# 192 "../../../include/tmd.h" 2



# 1 "../../../include/support.h" 1


#ident  "%full_filespec: support.h#5:incl:1 %"

# 1 "../../../include/tmd.h" 1
 
 
 
 
 
 
 
 
 














































































































































































# 564 "../../../include/tmd.h"


# 5 "../../../include/support.h" 2






    typedef struct {
        int waiterCount;
        void *semaphore;
    } *PDQsema;



    extern char *Strerror(int error);
    extern STATUS *Strerror_r(int error, char *buffer);
    extern char *Strdup(const char *src);







    extern void *Int_Malloc(char *file, int line, size_t len);
    extern void Int_FFree(char *file, int line, void *ptr);
    extern void Int_Mem_Add_User(char *file, int line, void *mem);








    extern void *Calloc(size_t nmemb, size_t size);
    extern void *Realloc(void *ptr, size_t newSize);


    extern char *Strncpy(char *s1, const char *s2, size_t n);
    extern int StrEQ(const char *s1, const char *s2);
    extern int StrcaseEQ(const char *s1, const char *s2);
    extern int StrIsNumber(const char *str);

    extern PDQsema pdqSemCreate(void);
    extern void pdqSemTake(PDQsema sema);
    extern void pdqSemGive(PDQsema sema);






# 195 "../../../include/tmd.h" 2


# 1 "../../../infra/inc/bspType.h" 1

# 23 "../../../infra/inc/bspType.h"




# 197 "../../../include/tmd.h" 2

# 1 "../../../infra/SYS/target/h/dirent.h" 1
 

 

 




























# 1 "../../../infra/SYS/target/h/limits.h" 1
 

 

 





















 








# 1 "../../../infra/SYS/target/h/tool/gnu/limits.h" 1



 



 



 




 





 



 












 

 




 



 








 



 













 




 








 






# 36 "../../../infra/SYS/target/h/limits.h" 2


































# 34 "../../../infra/SYS/target/h/dirent.h" 2







 

struct dirent		 
    {
    char	d_name [99   + 1];	 
    };


 
 

typedef struct		 
    {
    int		  dd_fd;		 
    int		  dd_cookie;		 
    struct dirent dd_dirent;		 
    } DIR;

 





 



extern DIR *	opendir (char *dirName);
extern STATUS 	closedir (DIR *pDir);
extern struct 	dirent *readdir (DIR *pDir);
extern void 	rewinddir (DIR *pDir);















# 198 "../../../include/tmd.h" 2














 
 
void SFT_ApplSwRegister(void);
 
 













typedef PB_ERROR_CODE_T (*AMM_AppNotifyRegister)(int flag);

typedef void (*auxClockHookFunc_t)(int user_para);


void tmd_ReportApplInitState(int status);
void  Amm_ApplNotifyRegister(AMM_AppNotifyRegister *fptr);
void Amm_SwRegister(void);
void  Amm_ApplInit(void);
void Amm_ApplWaitStatus(BOOL bSwFlag);
void Amm_FdrCycleChange(int dwCycleTime);
int AMM_Select(int width, fd_set *pReadFds, fd_set *pWriteFds,fd_set *pExceptFds, struct timeval *pTimeOut);


 
    int tmd_GetFDforAppl(int applNumber);
    int tmd_GetApplIDforAppl(char *taskName);

 
    int gettimeofday(struct timeval *tv, struct timezone *tz);
    int settimeofday(const struct timeval *tv, const struct timezone *tz);

 
 
    int AMM_SpawnAppl(char *name, int priority, int options, int stackSize, int (*entryPt) (void), int applNum,
                      char *applName, unsigned int logflags, int cfgOptions);

 
 
 
    int AMM_SpawnTask(char *name, int priority, int options, int stackSize, int (*entryPt) (void), int taskNum, int arg1,
                      int arg2, int arg3, int arg4, int arg5, int arg6, int arg7);

     
     
    int AMM_SpawnApplTask(char *name, int priority, int options, int stackSize, int (*entryPt) (void), int applNum, char *applName,
                  unsigned int logflags, int cfgOptions, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7);

 















 
    struct acb *tmd_RegisterApplication(int applNum, char *applName, unsigned int logflags, int cfgOptions);

 
    void tmd_TaskEnd(void);
    void tmd_RemoveRegistration(int applNumber);

 
    int tmd_GenerateAlarm(int alarm_interface_index, int slot_num, int port_num, int ifAlarmEnabled, int alarm_type_id,
                           int new_alarm_status, int interface_type, int instances[]);
    int tmd_GenerateEvent(int event_type_id, char *event_detail);



    int SIPADDR(int slot);
    int PSIPADDR(int slot);

 
    unsigned int FindBoardProperties(void);
    int BoardInit(int boardtype);

 



 















 
# 338 "../../../include/tmd.h"






# 354 "../../../include/tmd.h"



 
typedef enum{	
		SEND_OK = 0,		
		QUEUE_FULL=1,
		LINK_BREAKDOWN=2,
		NET_CONGESTION =3,
		DEST_APP_NOTRESP=4,
		PROTO_ERROR=5,
		OTHER_ERROR=6,
                MS_STATE_UNKNOWN=7,
                DMM_CMND_NULL=8,
                DMM_SRC_INVALID=9,
                DMM_DEST_INVALID= 10,
                DMM_MSG_LEN_ERROR=11,
                DMM_NOT_READY=12,
                DMM_SLOT_TRANSFER_ERROR=13,
                DMM_SYN_INNER_DEST_NCK=14,
                DMM_SYN_NCK_BY_DMM =15,
                DMM_SYN_NCK_BY_APPL=16
}DMM_ERR_CODE_t;



 void *msg_q_Receive(int qid, int timeout);
 
 command_tt *DMM_ReadPipe(int fd);
 
 command_tt *DMM_ReadQueue(int qid, int timeout);


 command_tt *DMM_CreateMsgExt(char *file, int line,int length);
 
 void DMM_ReleaseMsgExt(char *file, int line,command_tt *pCommand);




 
 command_tt * DMM_ReadCommandForAppl ();
 int DMM_CreateApplQueue(int size);
 
 int DMM_CreateQueue(int size);
 int DMM_ReleaseQueue( int qid);
 
 int DMM_CreateApplPipe(int size);
 
 int DMM_ReleasePipe(int fd);


 DMM_ERR_CODE_t DMM_SendCommand (command_tt *command);
 DMM_ERR_CODE_t DMM_SendUnAckedCommand(command_tt *command);
 DMM_ERR_CODE_t DMM_SendWRes(command_tt *command, int retries, int timeout, command_tt **ppResMsg);
   DMM_ERR_CODE_t DMM_SendSynAck(command_tt  *pInCmd,command_tt *pAckCmd,BOOL bRecvSucc);
 DMM_ERR_CODE_t DMM_SendBcCmd(command_tt *pCommand);
 command_tt * DMM_Recv (int timeout);
 command_tt * DMM_ReadCommandForAppl ();
 int  DMM_GetPhysicalSlot(int iLogicSlot);
 int   DMM_GetLogicalSlot(int iPhysicalSlot);
 int DMM_GetMatePSlot(void );
DMM_ERR_CODE_t DmmGetLinkState(unsigned short wDestPslot);


 
 int DMM_GetMatePSlot(void );

 int DMM_GetLocalPSlot(void );
 int DMM_GetLocalLSlot(void );
BOOL DMM_SetSpcMsMap(UINT16  wActivePSlot,BOOL bStdbyIsOnline);
BOOL DMM_GetPSlotOnLine(UINT16 wPSlotNo);
BOOL DMM_GetLSlotOnLine(UINT16 wLSlotNo);

int DMM_CreateSpcApplQ(UINT32 dwHighQSize,UINT32 dwLowQSize,UINT32 dwExtQSize);
UINT32 DMM_GetMsgType(const command_tt *pCommand);

void  DMM_SetCmdUrgent(command_tt *pCommand);
BOOL DMM_IsSmartCard(int PhySlot);


BOOL msTimer_Start(auxClockHookFunc_t FuncPtr,int user_para,UINT32 MsLen);
BOOL taskDelayMs(UINT32 DelayLen);



 

extern FILE *NfsOpenFile(const char *pFileName, const char *pMode);
extern STATUS NfsRemoveFile(const char *pFileName);
extern STATUS NfsCreatePath(const char *pDirName);
extern STATUS NfsDelPath(const char *pDirName);
extern DIR *NfsOpenDir(const char *pDirName);

 



extern int RFileWrite(char *pFileName,char *pMem,int WriteLen,int WriteMode);
extern int RFileRead(char *pFileName,char *pMem,int ReadLen);



 
 int tmd_SendQueueCmnd(int qid, command_tt *command);
 
 int tmd_SendPipeCmnd(int fd, command_tt *command);

 
 int tmd_IsrSendQueueCmnd(int qid, command_tt *command);
 
 int tmd_IsrSendCommand(command_tt *command);
 STATUS tmd_PipeDrv (void);

 BOOL tmd_RemapSlot(int LSlotIndex,int PSlotIndex);
 int tmd_StackCheck(int taskNum);
 int tmd_GetQueueSize(int qid);
 int tmd_GetNumLowCmnds(int qid);
 int tmd_GetNumHiCmnds(int qid);
 int tmd_GetMaxLowCmnds(int qid);
 int tmd_GetMaxHiCmnds(int qid);
 int tmd_GetNumRxMsgs(int qid);

 int tmd_GetNumLowCmnds(int qid);
 int tmd_ClrQueueDbg(int qid);
 int tmd_GetSlotMac(int slot, char *mac);

 STATUS tmd_RemapProxeeSlot(int slot, int proxy);

 int tmd_MsgSlotCheck(int slot);


extern int tmd_GetSlotIfName(int slot, char *name_buffer);
extern int tmd_GetSlotTxMsgNum(int slot);
extern int tmd_GetSlotRxMsgNum(int slot);

 int tmd_GetSlotIfName(int slot, char *name_buffer);
 int tmd_GetSlotTxMsgNum(int slot);
 int tmd_GetSlotRxMsgNum(int slot);

 void tmd_ResetSlotMsgCnts(int slot);


 int Zip(char* pstSrcName, char* pszArchFileName);
 int Zip_PSM(char* pstSrcName, char* pszArchFileName);
 int unZip(char* pstSrcName, char* pszArchFileName);
 int Zip_Buffer(char* bufferData, unsigned int bufferLen, char* pszArchFileName);





 


    void SetUseEther(int i);

    void *tmd_CreatePrivateArea(unsigned int size);
    int tmd_EnablePrivateArea(unsigned int appl);
    int tmd_DisablePrivateArea(unsigned int appl);    
    
 
 
 
    void initializeMirroringServices(void);

 
    STATUS isMemLocMirrored(void *address);
    STATUS registerMirroredMemLoc(void *address, short numbytes, unsigned short initialValue);
    STATUS setMemLoc_16bit(void *address, short accessType, unsigned short bitMask, unsigned short bitPattern);
    STATUS setMemLoc_16bit_Isr(void *address, short accessType, unsigned short bitMask, unsigned short bitPattern);
    unsigned short getMemLoc_16bit(void *address);
    unsigned short getMemLoc_16bit_Isr(void *address);

 
    STATUS is_I2C_Register_Shared(int address);
    STATUS register_Shared_16bit_I2C_Register(int address, unsigned short initialValue, char setInitValNow);
    STATUS set_shared_16bit_I2C_register(int address, short accessType, unsigned short bitMask,
                                         unsigned short bitPattern);
    int get_shared_16bit_I2C_register(int address, unsigned short *data);

void AMM_SlotWdgTimeoutProc(int slot);
    
 


 


    void registerDynamicDumps(void *area, int len, char *name);

    void tmd_prepare_card_for_impending_shutdown(void);













unsigned int  CRC32(unsigned int crc, const unsigned char *buf, unsigned int len);






# 9 "../../../include/globals.h" 2

# 1 "../../../include/acb.h" 1





# 1 "../../../infra/SYS/target/h/taskLib.h" 1
 

 

 





































































































































# 1 "../../../infra/SYS/target/h/qLib.h" 1
 

 

 
































# 1 "../../../infra/SYS/target/h/qClass.h" 1
 

 

 




























 

typedef struct q_class		 
    {
    FUNCPTR createRtn;		 
    FUNCPTR initRtn;		 
    FUNCPTR deleteRtn;		 
    FUNCPTR terminateRtn;	 
    FUNCPTR putRtn;		 
    FUNCPTR getRtn;		 
    FUNCPTR removeRtn;		 
    FUNCPTR resortRtn;		 
    FUNCPTR advanceRtn;		 
    FUNCPTR getExpiredRtn;	 
    FUNCPTR keyRtn;		 
    FUNCPTR calibrateRtn;	 
    FUNCPTR infoRtn;		 
    FUNCPTR eachRtn;		 
    struct q_class *valid;	 
    } Q_CLASS;

typedef Q_CLASS *Q_CLASS_ID;	 

 

 


























# 38 "../../../infra/SYS/target/h/qLib.h" 2


 





 

extern  Q_CLASS_ID		qFifoClassId;
extern  Q_CLASS_ID		qPriListClassId;
extern  Q_CLASS_ID		qPriListFromTailClassId;
extern  Q_CLASS_ID		qPriDeltaClassId;
extern  Q_CLASS_ID		qPriHeapClassId;
extern  Q_CLASS_ID		qPriBMapClassId;

 








 

 








typedef struct		 
    {
    UINT     qPriv1;			 
    UINT     qPriv2;			 
    UINT     qPriv3;			 
    UINT     qPriv4;			 
    } Q_NODE;


 








typedef struct		 
    {
    Q_NODE  *pFirstNode;		 
    UINT     qPriv1;			 
    UINT     qPriv2;			 
    Q_CLASS *pQClass;			 
    } Q_HEAD;





 


 
















 













 

















 












 













 














 




















 
















 













 



















 

























extern Q_HEAD *	qCreate (Q_CLASS *pQClass, ...);
extern Q_NODE *	qEach (Q_HEAD *pQHead, FUNCPTR routine, int routineArg);
extern Q_NODE *	qFirst (Q_HEAD *pQHead);
extern Q_NODE *	qGet (Q_HEAD *pQHead);
extern Q_NODE *	qGetExpired (Q_HEAD *pQHead);
extern STATUS 	qDelete (Q_HEAD *pQHead);
extern STATUS 	qInit (Q_HEAD *pQHead, Q_CLASS *pQClass, ...);
extern STATUS 	qTerminate (Q_HEAD *pQHead);
extern ULONG 	qKey (Q_HEAD *pQHead, Q_NODE *pQNode, int keyType);
extern int 	qInfo (Q_HEAD *pQHead, Q_NODE *nodeArray [ ], int maxNodes);
extern void 	qAdvance (Q_HEAD *pQHead);
extern void 	qCalibrate (Q_HEAD *pQHead, ULONG keyDelta);
extern void 	qPut (Q_HEAD *pQHead, Q_NODE *pQNode, ULONG key);
extern STATUS 	qRemove (Q_HEAD *pQHead, Q_NODE *pQNode);
extern void 	qResort (Q_HEAD *pQHead, Q_NODE *pQNode, ULONG newKey);

# 331 "../../../infra/SYS/target/h/qLib.h"









# 139 "../../../infra/SYS/target/h/taskLib.h" 2

# 1 "../../../infra/SYS/target/h/regs.h" 1
 

 

 




























































# 1 "../../../infra/SYS/target/h/arch/ppc/regsPpc.h" 1
 

 
 





















typedef struct		 
    {
    _RType gpr[32 ];	 
    _RType msr;			 
    _RType lr;			 
    _RType ctr;			 
    _RType pc;			 
    UINT32 cr;			 
    UINT32 xer;			 





    UINT32 pad;			 

    } REG_SET;

 



















































































































# 168 "../../../infra/SYS/target/h/arch/ppc/regsPpc.h"







# 66 "../../../infra/SYS/target/h/regs.h" 2








































typedef struct regindex
    {
    char	*regName;	 
    int		regOff;		 







    } REG_INDEX;







# 140 "../../../infra/SYS/target/h/taskLib.h" 2

# 1 "../../../infra/SYS/target/h/excLib.h" 1
 

 

 






































































# 1 "../../../infra/SYS/target/h/arch/ppc/excPpcLib.h" 1
 

 

 


































 















# 72 "../../../infra/SYS/target/h/arch/ppc/excPpcLib.h"












typedef struct
    {
    UINT32	valid;		 

    UINT32	vecOff;		 
    _RType	cia;		 

    _RType	msr;		 




    _RType	dar;		 


    UINT32      cr;		 
    UINT32	xer;		 

# 115 "../../../infra/SYS/target/h/arch/ppc/excPpcLib.h"

    UINT32	fpcsr;		 
    UINT32	dsisr;		 



    } EXC_INFO;











 

# 189 "../../../infra/SYS/target/h/arch/ppc/excPpcLib.h"

















						 

# 219 "../../../infra/SYS/target/h/arch/ppc/excPpcLib.h"


# 234 "../../../infra/SYS/target/h/arch/ppc/excPpcLib.h"





















# 278 "../../../infra/SYS/target/h/arch/ppc/excPpcLib.h"




# 310 "../../../infra/SYS/target/h/arch/ppc/excPpcLib.h"


 







# 339 "../../../infra/SYS/target/h/arch/ppc/excPpcLib.h"




 

extern FUNCPTR  excExcepHook;    

 



extern STATUS		excConnect (VOIDFUNCPTR *, VOIDFUNCPTR);
extern STATUS  		excIntConnect (VOIDFUNCPTR *, VOIDFUNCPTR);
extern void		excVecSet (FUNCPTR *, FUNCPTR);
extern FUNCPTR		excVecGet (FUNCPTR *);
extern void		excVecBaseSet (FUNCPTR *);
extern FUNCPTR *	excVecBaseGet (void);











# 387 "../../../infra/SYS/target/h/arch/ppc/excPpcLib.h"









# 76 "../../../infra/SYS/target/h/excLib.h" 2










































typedef struct  excfaultTab
    {
    int faultType;		 
    int subtype;		 
    int signal;			 
    int code;			 
    } EXC_FAULT_TAB;

 


extern STATUS 	excShowInit (void);
extern void 	excHookAdd (FUNCPTR excepHook);
extern STATUS 	excJobAdd (VOIDFUNCPTR func, int arg1, int arg2, int arg3,
			   int arg4, int arg5, int arg6);














# 141 "../../../infra/SYS/target/h/taskLib.h" 2

# 1 "../../../infra/SYS/target/h/private/eventP.h" 1
 

 

 


















































































 

 







 





































 



typedef unsigned short event_t;



# 1 "../../../infra/SYS/target/h/private/funcBindP.h" 1
 

 

 

























































 

extern FUNCPTR     _func_ioTaskStdSet;
extern FUNCPTR     _func_bdall;
extern FUNCPTR     _func_dspTaskRegsShow;
extern  VOIDFUNCPTR _func_dspRegsListHook;	 
extern  FUNCPTR	   _func_dspMregsHook;		 
extern FUNCPTR     _func_excBaseHook;
extern FUNCPTR     _func_excInfoShow;
extern FUNCPTR     _func_excIntHook;
extern FUNCPTR     _func_excJobAdd;
extern FUNCPTR     _func_excPanicHook;
extern FUNCPTR     _func_fastUdpErrorNotify;
extern FUNCPTR     _func_fastUdpInput;
extern FUNCPTR     _func_fastUdpPortTest;
extern FUNCPTR     _func_fclose;
extern FUNCPTR     _func_fppTaskRegsShow;
extern FUNCPTR     _func_altivecProbe;
extern FUNCPTR     _func_altivecTaskRegsGet;
extern FUNCPTR     _func_altivecTaskRegsShow;
extern FUNCPTR     _func_speTaskRegsShow;
extern FUNCPTR     _func_ftpLs;
extern FUNCPTR     _func_netLsByName;
extern FUNCPTR     _func_printErr;
extern FUNCPTR     _func_logMsg;
extern FUNCPTR     _func_memalign;
extern FUNCPTR     _func_pthread_setcanceltype;
extern FUNCPTR     _func_selPtyAdd;
extern FUNCPTR     _func_selPtyDelete;
extern FUNCPTR     _func_selTyAdd;
extern FUNCPTR     _func_selTyDelete;
extern FUNCPTR     _func_selWakeupAll;
extern FUNCPTR     _func_selWakeupListInit;
extern FUNCPTR     _func_selWakeupListTerm;
extern VOIDFUNCPTR _func_sigExcKill;
extern FUNCPTR     _func_sigprocmask;
extern FUNCPTR     _func_sigTimeoutRecalc;
extern FUNCPTR     _func_smObjObjShow;
extern FUNCPTR     _func_spy;
extern FUNCPTR     _func_spyStop;
extern FUNCPTR     _func_spyClkStart;
extern FUNCPTR     _func_spyClkStop;
extern FUNCPTR     _func_spyReport;
extern FUNCPTR     _func_spyTask;
extern FUNCPTR     _func_sseTaskRegsShow;
extern FUNCPTR     _func_symFindByValueAndType;    
extern FUNCPTR     _func_symFindByValue;           
extern FUNCPTR     _func_symFindSymbol;
extern FUNCPTR     _func_symNameGet;
extern FUNCPTR     _func_symValueGet;
extern FUNCPTR     _func_symTypeGet;
extern FUNCPTR     _func_taskCreateHookAdd;
extern FUNCPTR     _func_taskDeleteHookAdd;
extern FUNCPTR     _func_valloc;
extern FUNCPTR     _func_remCurIdGet;
extern FUNCPTR     _func_remCurIdSet;

extern FUNCPTR	   _dbgDsmInstRtn;


extern BOOL        fastUdpInitialized;


extern BOOL	   ftpErrorSuppress;
extern BOOL	   _procNumWasSet;
extern int	   m2If64BitCounters;

extern VOIDFUNCPTR _func_evtLogO;
extern VOIDFUNCPTR _func_evtLogOIntLock;

extern VOIDFUNCPTR _func_evtLogM0;
extern VOIDFUNCPTR _func_evtLogM1;
extern VOIDFUNCPTR _func_evtLogM2;
extern VOIDFUNCPTR _func_evtLogM3;

extern VOIDFUNCPTR _func_evtLogT0;
extern VOIDFUNCPTR _func_evtLogT0_noInt;
extern VOIDFUNCPTR _func_evtLogT1;
extern VOIDFUNCPTR _func_evtLogT1_noTS;
extern VOIDFUNCPTR _func_evtLogTSched;

extern VOIDFUNCPTR _func_evtLogString;
extern FUNCPTR     _func_evtLogPoint;
extern FUNCPTR	   _func_evtLogReserveTaskName;

# 156 "../../../infra/SYS/target/h/private/funcBindP.h"


extern FUNCPTR     _func_tmrStamp;
extern FUNCPTR     _func_tmrStampLock;
extern FUNCPTR     _func_tmrFreq;
extern FUNCPTR     _func_tmrPeriod;
extern FUNCPTR     _func_tmrConnect;
extern FUNCPTR     _func_tmrEnable;
extern FUNCPTR     _func_tmrDisable;

extern VOIDFUNCPTR _func_trgCheck;
extern UINT32 evtAction;
extern UINT32 wvEvtClass;
extern UINT32 trgEvtClass;

extern BOOL   wvInstIsOn;              
extern BOOL   wvObjIsEnabled;          






# 144 "../../../infra/SYS/target/h/private/eventP.h" 2














































 











# 210 "../../../infra/SYS/target/h/private/eventP.h"


 

 










 












 
















 

















 


















 












# 322 "../../../infra/SYS/target/h/private/eventP.h"

 












# 353 "../../../infra/SYS/target/h/private/eventP.h"

 










# 377 "../../../infra/SYS/target/h/private/eventP.h"

 

















 
















 








# 432 "../../../infra/SYS/target/h/private/eventP.h"


 
















 
















 








# 489 "../../../infra/SYS/target/h/private/eventP.h"

 








# 511 "../../../infra/SYS/target/h/private/eventP.h"

 








# 532 "../../../infra/SYS/target/h/private/eventP.h"

 








# 555 "../../../infra/SYS/target/h/private/eventP.h"

 








# 576 "../../../infra/SYS/target/h/private/eventP.h"

 








# 597 "../../../infra/SYS/target/h/private/eventP.h"

 
















 








# 632 "../../../infra/SYS/target/h/private/eventP.h"

 








# 651 "../../../infra/SYS/target/h/private/eventP.h"

 








# 670 "../../../infra/SYS/target/h/private/eventP.h"

 








# 688 "../../../infra/SYS/target/h/private/eventP.h"


















 














































 

 


     





     





     




 


     











     





     





     










     











     






     





      





     

 


     










     







     






     








     






     







     







     







     








     








     









     









     








     








     








     








     






     






     







     






     









     






     









     










     







     






     






     







     





     







     








     







     






     








     






     





     





 


     






     





     





     





     








     







     





     




     





     





     





     





     





     





     





     




     





     





     





     





     





     





     





     





     





     





     





     





     





     





     




     




 


     







     






     







     







     




     





     





     





     





     






     






 




     





     




     























 














	 


















 





























# 142 "../../../infra/SYS/target/h/taskLib.h" 2

# 1 "../../../infra/SYS/target/h/private/semLibP.h" 1
 

 

 





























# 1 "../../../infra/SYS/target/h/private/eventLibP.h" 1
 

 

 























extern VOIDFUNCPTR eventEvtRtn;  

 

typedef struct eventsCb
    {
    UINT32 wanted;	 
    volatile UINT32 received;	 
    UINT8  options;	 
    UINT8  sysflags;	 
    UINT8  pad[2];	 
    } EVENTS;		 

 



 



					 


 

typedef struct eventsResourceCb
    {
    UINT32 registered;	 
    int    taskId;	 
    UINT8  options;	 
    UINT8  pad[3];	 
    } EVENTS_RSRC;	 





 














extern void 	eventLibInit	(void);
extern void 	eventInit	(EVENTS_RSRC * pEvRsrc);
extern void 	eventTerminate 	(const EVENTS_RSRC * pEvRsrc);
extern void 	semEvLibInit	(void);
extern void 	msgQEvLibInit	(void);
extern STATUS 	eventStart	(OBJ_ID objId, EVENTS_RSRC * pEvRsrc,
				 FUNCPTR isRsrcFree, UINT32 events,
				 UINT8 options);
extern STATUS	eventRsrcSend	(int taskId, UINT32 events);
extern STATUS	eventRsrcShow	(EVENTS_RSRC * pEvRsrc);
extern STATUS   eventTaskShow	(EVENTS	* pEvents);

# 106 "../../../infra/SYS/target/h/private/eventLibP.h"
















# 35 "../../../infra/SYS/target/h/private/semLibP.h" 2






 













typedef struct semaphore  
    {
    OBJ_CORE	objCore;	 
    UINT8	semType;	 
    UINT8	options;	 
    UINT16	recurse;	 
    Q_HEAD	qHead;		 
    union
	{
	UINT		 count;	 
	struct windTcb	*owner;	 
	} state;
    EVENTS_RSRC	events;		 

    } SEMAPHORE;




 












 
















 

extern OBJ_CLASS	semClass;		 
extern OBJ_CLASS	semInstClass;		 
extern CLASS_ID		semClassId;		 
extern CLASS_ID		semInstClassId;		 
extern FUNCPTR		semGiveTbl [];		 
extern FUNCPTR		semTakeTbl [];		 
extern FUNCPTR		semFlushTbl [];		 
extern FUNCPTR		semGiveDeferTbl [];	 
extern FUNCPTR		semFlushDeferTbl [];	 
extern int		semMGiveKernWork;	 

extern FUNCPTR  semSmShowRtn;	 
extern FUNCPTR  semSmInfoRtn;	 

 



extern STATUS	semLibInit (void);
extern STATUS	semTerminate (SEM_ID semId);
extern STATUS	semDestroy (SEM_ID semId, BOOL dealloc);
extern STATUS	semGiveDefer (SEM_ID semId);
extern STATUS	semFlushDefer (SEM_ID semId);
extern STATUS	semInvalid (SEM_ID semId);
extern STATUS	semIntRestrict (SEM_ID semId);
extern STATUS	semQInit (SEMAPHORE *pSemaphore, int options);
extern STATUS	semQFlush (SEM_ID semId);
extern void	semQFlushDefer (SEM_ID semId);
extern STATUS	semBInit (SEMAPHORE *pSem,int options,SEM_B_STATE initialState);
extern STATUS	semBCoreInit (SEMAPHORE *pSemaphore, int options,
			      SEM_B_STATE initialState);
extern STATUS	semBGive (SEM_ID semId);
extern STATUS	semBTake (SEM_ID semId, int timeout);
extern void	semBGiveDefer (SEM_ID semId);
extern STATUS	semMInit (SEMAPHORE *pSem, int options);
extern STATUS	semMCoreInit (SEMAPHORE *pSemaphore, int options);
extern STATUS	semMGive (SEM_ID semId);
extern STATUS	semMTake (SEM_ID semId, int timeout);
extern STATUS	semMGiveKern (SEM_ID semId);
extern STATUS	semMPendQPut (SEM_ID semId, int timeout);
extern STATUS	semCInit (SEMAPHORE *pSem,int options,int initialCount);
extern STATUS	semCCoreInit (SEMAPHORE *pSemaphore, int options,
			      int initialCount);
extern STATUS	semCGive (SEM_ID semId);
extern STATUS	semCTake (SEM_ID semId, int timeout);
extern void	semCGiveDefer (SEM_ID semId);
extern STATUS	semOTake (SEM_ID semId);

# 184 "../../../infra/SYS/target/h/private/semLibP.h"


# 200 "../../../infra/SYS/target/h/private/semLibP.h"












# 143 "../../../infra/SYS/target/h/taskLib.h" 2



# 1 "../../../infra/SYS/target/h/stdlib.h" 1
 

 

 
















































# 64 "../../../infra/SYS/target/h/stdlib.h"


 















typedef struct { int quot; int rem; } div_t ;




typedef struct { long quot; long rem; } ldiv_t ;



typedef struct {		 
	unsigned char __state;
	unsigned short __wchar;
	} _Mbsave;



extern void	abort (void);
extern int	abs (int __i);
extern int	atexit (void (*__func)(void));
extern double	atof (const char *__s);
extern int	atoi (const char *__s);
extern long	atol (const char *__s);
extern void *	bsearch (const void *__key, const void *__base,
		         size_t __nelem, size_t __size,
		         int  (*__cmp)(const void *__ck, const void *__ce));
extern div_t	div (int __numer, int __denom);
extern long	labs (long __i);
extern ldiv_t	ldiv (long __numer, long __denom);
extern int	mblen (const char *__s, size_t __n);
extern size_t	mbstowcs (wchar_t *__wcs, const char *__s, size_t __n);
extern int	mbtowc (wchar_t *__pwc, const char *__s, size_t __n);
extern void	qsort (void *__base, size_t __nelem, size_t __size,
		       int  (*__cmp)(const void *__e1, const void *__e2));
extern int	rand (void);
extern void *	srand (unsigned int __seed);
extern double	strtod (const char *__s, char **__endptr);
extern long	strtol (const char *__s, char **__endptr, int __base);
extern unsigned long strtoul (const char *__s, char **__endptr, int __base);
extern int	system (const char *__s);
extern size_t	wcstombs (char *__s, const wchar_t *__wcs, size_t __n);
extern int	wctomb (char *__s, wchar_t __wchar);

extern void *	calloc (size_t __nelem, size_t __size);
extern void	exit (int __status);
extern void	free (void *__ptr);
extern char *	getenv (const char *__name);
extern void *	malloc (size_t __size);
extern void *	realloc (void *__ptr, size_t __size);



extern void     div_r (int numer, int denom, div_t * divStructPtr);
extern void     ldiv_r (long numer, long denom, ldiv_t * divStructPtr);



# 177 "../../../infra/SYS/target/h/stdlib.h"









# 146 "../../../infra/SYS/target/h/taskLib.h" 2



 












 







 



















 







typedef struct wdb_info		 
    {
    int			wdbState;	 
    REG_SET *		wdbRegisters;	 
    struct
        {
	void *	wdb1;
	void *	wdb2;
	} wdbEvtList;			 
    int			bpAddr;		 
					 
    int			taskBpAddr;	 
    int			taskPc;		 
    int			taskFp;		 
    int			taskSp;		 
    VOIDFUNCPTR		wdbExitHook;	 
  } WDB_INFO;

typedef struct windTcb		 
    {
    Q_NODE		qNode;		 
    Q_NODE		tickNode;	 
    Q_NODE		activeNode;	 

    OBJ_CORE		objCore;	 
    char *		name;		 
    int			options;	 
    UINT		status;		 
    UINT		priority;	 
    UINT		priNormal;	 
    UINT		priMutexCnt;	 
    struct semaphore *	pPriMutex;	 

    UINT		lockCnt;	 
    UINT		tslice;		 

    UINT16		swapInMask;	 
    UINT16		swapOutMask;	 

    Q_HEAD *		pPendQ;		 

    UINT		safeCnt;	 
    Q_HEAD		safetyQHead;	 

    FUNCPTR		entry;		 

    char *		pStackBase;	 
    char *		pStackLimit;	 
    char *		pStackEnd;	 

    int			errorStatus;	 
    int			exitCode;	 

    struct sigtcb *	pSignalInfo;	 
    struct selContext *	pSelectContext;	 

    UINT		taskTicks;	 
    UINT		taskIncTicks;	 

    struct taskVar *	pTaskVar;	 
    struct rpcModList *	pRPCModList;	 
    struct fpContext *	pFpContext;	 

    struct __sFILE *	taskStdFp[3];	 
    int			taskStd[3];	 

    char **		ppEnviron;	 
    int                 envTblSize;      
    int                 nEnvVarEntries;  
    struct sm_obj_tcb *	pSmObjTcb;	 
    int			windxLock;	 
    void *		pComLocal;	 
    REG_SET *		pExcRegSet;	 
    EVENTS		events;		 
    WDB_INFO *		pWdbInfo;	 
    void *		pPthread;	 
    int			reserved1;	 
    int			compiler1;	 
    int			spare1;		 
    int			spare2;		 
    int			spare3;		 
    int			spare4;		 

    




     

# 296 "../../../infra/SYS/target/h/taskLib.h"
















    EXC_INFO            excInfo;         
    UINT                pCoprocCtx;      

     

    REG_SET             regs;            



















# 348 "../../../infra/SYS/target/h/taskLib.h"










# 369 "../../../infra/SYS/target/h/taskLib.h"


















# 396 "../../../infra/SYS/target/h/taskLib.h"


    struct dspContext *	pDspContext;	 
    WDB_INFO		wdbInfo;	 
    } WIND_TCB;

typedef struct 			 
    {
    int			td_id;		 
    char *		td_name;	 
    int			td_priority;	 
    int			td_status;	 
    int			td_options;	 
    FUNCPTR		td_entry;	 
    char *		td_sp;		 
    char *		td_pStackBase;	 
    char *		td_pStackLimit;	 
    char *		td_pStackEnd;	 
    int			td_stackSize;	 
    int			td_stackCurrent; 
    int			td_stackHigh;	 
    int			td_stackMargin;	 
    int			td_errorStatus;	 
    int			td_delay;	 
    EVENTS		td_events;	 
    } TASK_DESC;





 


















 


















 













 










# 499 "../../../infra/SYS/target/h/taskLib.h"

 














 

extern CLASS_ID	taskClassId;		 
extern CLASS_ID taskInstClassId;  	 
extern WIND_TCB *taskIdCurrent;		 
extern BOOL     taskPriRangeCheck;       

 



extern STATUS 	taskLibInit (void);





extern int 	taskSpawn (char *name, int priority, int options, int stackSize,
		      	   FUNCPTR entryPt, int arg1, int arg2, int arg3,
		      	   int arg4, int arg5, int arg6, int arg7,
		      	   int arg8, int arg9, int arg10);


extern STATUS 	taskInit (WIND_TCB *pTcb, char *name, int priority, int options,
			  char *pStackBase, int stackSize, FUNCPTR entryPt,
			  int arg1, int arg2, int arg3, int arg4, int arg5,
			  int arg6, int arg7, int arg8, int arg9, int arg10);
extern STATUS 	taskActivate (int tid);
extern STATUS 	taskDelete (int tid);
extern STATUS 	taskDeleteForce (int tid);
extern STATUS 	taskSuspend (int tid);
extern STATUS 	taskResume (int tid);
extern STATUS 	taskRestart (int tid);
extern STATUS 	taskPrioritySet (int tid, int newPriority);
extern STATUS 	taskPriorityGet (int tid, int *pPriority);
extern STATUS 	taskLock (void);
extern STATUS 	taskUnlock (void);
extern STATUS 	taskSafe (void);
extern STATUS 	taskUnsafe (void);
extern STATUS 	taskDelay (int ticks);
extern STATUS 	taskOptionsSet (int tid, int mask, int newOptions);
extern STATUS 	taskOptionsGet (int tid, int *pOptions);
extern char *	taskName (int tid);
extern int 	taskNameToId (char *name);
extern STATUS 	taskIdVerify (int tid);
extern int 	taskIdSelf (void);
extern int 	taskIdDefault (int tid);
extern BOOL 	taskIsReady (int tid);
extern BOOL 	taskIsSuspended (int tid);
extern WIND_TCB *taskTcb (int tid);
extern int 	taskIdListGet (int idList [ ], int maxTasks);
extern STATUS 	taskInfoGet (int tid, TASK_DESC *pTaskDesc);
extern STATUS 	taskStatusString (int tid, char *pString);
extern STATUS 	taskOptionsString (int tid, char *pString);
extern STATUS 	taskRegsGet (int tid, REG_SET *pRegs);
extern STATUS 	taskRegsSet (int tid, REG_SET *pRegs);
extern void 	taskRegsShow (int tid);
extern void *	taskStackAllot (int tid, unsigned nBytes);
extern void 	taskShowInit (void);
extern STATUS 	taskShow (int tid, int level);

# 614 "../../../infra/SYS/target/h/taskLib.h"









# 6 "../../../include/acb.h" 2


# 1 "../../../infra/SYS/target/h/lstLib.h" 1
 

 

 


































 

typedef struct node		 
    {
    struct node *next;		 
    struct node *previous;	 
    } NODE;


 

typedef struct			 
    {
    NODE node;			 
    int count;			 
    } LIST;

 


 




extern void	lstLibInit (void);
extern NODE *	lstFirst (LIST *pList);
extern NODE *	lstGet (LIST *pList);
extern NODE *	lstLast (LIST *pList);
extern NODE *	lstNStep (NODE *pNode, int nStep);
extern NODE *	lstNext (NODE *pNode);
extern NODE *	lstNth (LIST *pList, int nodenum);
extern NODE *	lstPrevious (NODE *pNode);
extern int 	lstCount (LIST *pList);
extern int 	lstFind (LIST *pList, NODE *pNode);
extern void 	lstAdd (LIST *pList, NODE *pNode);
extern void 	lstConcat (LIST *pDstList, LIST *pAddList);
extern void 	lstDelete (LIST *pList, NODE *pNode);
extern void 	lstExtract (LIST *pSrcList, NODE *pStartNode, NODE *pEndNode,
	  		    LIST *pDstList);
extern void 	lstFree (LIST *pList);
extern void 	lstInit (LIST *pList);
extern void 	lstInsert (LIST *pList, NODE *pPrev, NODE *pNode);

# 104 "../../../infra/SYS/target/h/lstLib.h"









# 8 "../../../include/acb.h" 2










struct acb {
    applId_tt applId;
    char applname[23  + 1];
     

    int fd;                      
    int qid;                     

    int          fd_qid;  
    unsigned int logFlags;
    unsigned int cfg_options;
    unsigned int swping_counter;
    unsigned int swping_max;
    unsigned int waiting;
    unsigned int malloc_memory;  
    unsigned int malloc_buffs;   
    unsigned int Malloc_buffs;   
    unsigned int Malloc_memory;  
    int tid;                     
    void *applPrivate;           
    void *PrivateAreaAddr;       
    int   PrivateAreaSize;            
    int   PrivateAreaOpen;          
    int          tasksSpawned;      
};


struct taskMemStat
{ 
 int taskId;
 int AppNo;
 char taskName[12 ];
 unsigned int dynMemReqSum;  
 unsigned int dynMemUsingNow;  
 unsigned int dynMemTUsingNow;   
 unsigned int  fixedMemReqSum;  
 unsigned int fixedMemUsingNow;   
 unsigned int fixedMemTUsingNow;  
 
};

struct MemUsedInTaskInit
{ 
   char mod_or_app_Name[12 ];
  unsigned int malloc_size_used_in_load;  
  unsigned int malloc_size_used_in_active;  
};


struct taskCB {
    int taskNum;
    char taskname[10  + 1];
    void *taskPrivate;
    char            *mibAccessPtr;  
};


extern struct taskCB *taskCB;
extern struct acb *acb;
extern struct acb *applicationACBs[];

extern struct taskMemStat gTaskMemStat[];



# 10 "../../../include/globals.h" 2


# 1 "../../../include/boot_info.h" 1




# 1 "../../../infra/SYS/target/h/netinet/in.h" 1
 

 


































 




















# 1 "../../../infra/SYS/target/h/net/if.h" 1
 

 

 


































 






























# 1 "../../../infra/SYS/target/h/net/if_arp.h" 1
 

 

 


































 



















 









struct	arphdr {
	u_short	ar_hrd;		 


	u_short	ar_pro;		 
	u_char	ar_hln;		 
	u_char	ar_pln;		 
	u_short	ar_op;		 






 









};

 


struct arpreq {
	struct	sockaddr arp_pa;		 
	struct	sockaddr arp_ha;		 
	int	arp_flags;			 
};
 
















# 71 "../../../infra/SYS/target/h/net/if.h" 2

# 1 "../../../infra/SYS/target/h/net/route.h" 1
 

 

 


































 




















# 1 "../../../infra/SYS/target/h/net/socketvar.h" 1
 

 
 













 





































# 1 "../../../infra/SYS/target/h/selectLib.h" 1
 

 

 





































 





typedef enum
    {
    SELREAD,
    SELWRITE
    } SELECT_TYPE;

# 1 "../../../infra/SYS/target/h/private/selectLibP.h" 1
 

 

 
















typedef struct selWkNode
    {
    NODE		linkedListHooks; 
    BOOL		dontFree;	 
    int			taskId;		 
    int			fd;		 
    SELECT_TYPE		type;		 
    } SEL_WAKEUP_NODE;

typedef struct
    {
    SEMAPHORE		listMutex;	 
    SEL_WAKEUP_NODE	firstNode;	 
    LIST		wakeupList;	 
    } SEL_WAKEUP_LIST;

typedef struct selContext
    {
    SEMAPHORE	wakeupSem;		 
    BOOL     	pendedOnSelect;		 

    fd_set     *pReadFds;		 
    fd_set     *pWriteFds;		 

     

    fd_set     *pOrigReadFds;		 
    fd_set     *pOrigWriteFds;		 
    int		width;			 

     











    } SEL_CONTEXT;


 



extern void	selectInit		(int numFiles);
extern void	selTaskDeleteHookAdd 	(void);















# 55 "../../../infra/SYS/target/h/selectLib.h" 2


 



extern SELECT_TYPE  selWakeupType 	(SEL_WAKEUP_NODE *pWakeupNode);
extern STATUS 	    selNodeAdd 	      	(SEL_WAKEUP_LIST *pWakeupList,
			    		 SEL_WAKEUP_NODE *pWakeupNode);
extern STATUS 	    selNodeDelete 	(SEL_WAKEUP_LIST *pWakeupList,
			       		 SEL_WAKEUP_NODE *pWakeupNode);
extern int 	    selWakeupListLen 	(SEL_WAKEUP_LIST *pWakeupList);
extern void 	    selWakeup 		(SEL_WAKEUP_NODE *pWakeupNode);
extern void 	    selWakeupAll 	(SEL_WAKEUP_LIST *pWakeupList, 
					 SELECT_TYPE type);
extern void 	    selWakeupListInit 	(SEL_WAKEUP_LIST *pWakeupList);
extern void 	    selWakeupListTerm 	(SEL_WAKEUP_LIST *pWakeupList);
extern int	    select 		(int width, fd_set *pReadFds, 
					 fd_set *pWriteFds, fd_set *pExceptFds,
					 struct timeval *pTimeOut);

# 88 "../../../infra/SYS/target/h/selectLib.h"







# 56 "../../../infra/SYS/target/h/net/socketvar.h" 2

# 1 "../../../infra/SYS/target/h/net/uio.h" 1
 

 

 


































 





















struct iovec {
	caddr_t	iov_base;
	int	iov_len;
};

enum	uio_rw { UIO_READ, UIO_WRITE };

 
enum uio_seg {
	UIO_USERSPACE,		 
	UIO_SYSSPACE,		 
	UIO_USERISPACE		 
};

struct uio {
	struct	iovec *uio_iov;
	int	uio_iovcnt;
	off_t	uio_offset;
	int	uio_resid;
	enum	uio_seg uio_segflg;
	enum	uio_rw uio_rw;



};















# 57 "../../../infra/SYS/target/h/net/socketvar.h" 2

# 1 "../../../infra/SYS/target/h/net/mbuf.h" 1
 

 

 


































 































# 1 "../../../infra/SYS/target/h/net/unixLib.h" 1
 

 
 
























 

















 































            
 



extern int 	splnet (void);
extern int 	splimp (void);
extern void 	splx (int x);
extern void 	panic (char *msg);
extern void 	wakeup (SEM_ID semId);
extern void 	ksleep (SEM_ID semId);
extern char *	_netMalloc (int bufSize, UCHAR type, int canWait); 
extern void	_netFree (char * pBuf); 
extern void * 	hashinit (int 	elements, int type, u_long * hashmask);

# 106 "../../../infra/SYS/target/h/net/unixLib.h"








# 72 "../../../infra/SYS/target/h/net/mbuf.h" 2



# 1 "../../../infra/SYS/target/h/string.h" 1
 

 

 























# 39 "../../../infra/SYS/target/h/string.h"




extern void *	memchr (const void *__s, int __c, size_t __n);
extern int 	memcmp (const void *__s1, const void *__s2, size_t __n);
extern void *	memcpy (void *__s1, const void *__s2, size_t __n);
extern void *	memmove (void *__s1, const void *__s2, size_t __n);
extern void *	memset (void *__s, int __c, size_t __n);

extern char *	strcat (char *__s1, const char *__s2);
extern char *	strchr (const char *__s, int __c);
extern int 	strcmp (const char *__s1, const char *__s2);
extern int 	strcoll (const char *__s1, const char *__s2);
extern char *	strcpy (char *__s1, const char *__s2);
extern size_t 	strcspn (const char *__s1, const char *__s2);
extern size_t 	strlen (const char *__s);
extern char *	strncat (char *__s1, const char *__s2, size_t __n);
extern int 	strncmp (const char *__s1, const char *__s2, size_t __n);
extern char *	strncpy (char *__s1, const char *__s2, size_t __n);
extern char *	strpbrk (const char *__s1, const char *__s2);
extern char *	strrchr (const char *__s, int __c);
extern size_t 	strspn (const char *__s1, const char *__s2);
extern char *	strstr (const char *__s1, const char *__s2);
extern char *	strtok (char *__s, const char *__sep);
extern size_t 	strxfrm (char *__s1, const char *__s2, size_t __n);
extern char *	strerror(int __errcode);


extern char *	strtok_r (char *__s, const char *__sep, char **__ppLast);



extern int	strerror_r (int __errcode, char *__buf);
extern void 	bcopy (const char *source, char *dest, int nbytes);
extern void 	bcopyBytes (char *source, char *dest, int nbytes);
extern void 	bcopyWords (char *source, char *dest, int nwords);
extern void 	bcopyLongs (char *source, char *dest, int nlongs);
extern void 	bfill (char *buf, int nbytes, int ch);
extern void 	bfillBytes (char *buf, int nbytes, int ch);
extern void 	bzero (char *buffer, int nbytes);
extern int 	bcmp (char *buf1, char *buf2, int nbytes);
extern void 	binvert (char *buf, int nbytes);
extern void 	bswap (char *buf1, char *buf2, int nbytes);
extern void 	uswab (char *source, char *destination, int nbytes);
extern void 	swab (char *source, char *dest, int nbytes);
extern char *	index (const char *s, int c);
extern char *	rindex (const char *s, int c);


# 137 "../../../infra/SYS/target/h/string.h"







# 75 "../../../infra/SYS/target/h/net/mbuf.h" 2

# 1 "../../../infra/SYS/target/h/netBufLib.h" 1
 

 

 

































 













	
	
	








 

# 92 "../../../infra/SYS/target/h/netBufLib.h"


























# 144 "../../../infra/SYS/target/h/netBufLib.h"







 













 
 
 

 
















# 204 "../../../infra/SYS/target/h/netBufLib.h"











 











 







































 























 






 




 

 









 




 



 








 



 



 



 





    











                    











                    



                    








 









typedef struct
    {
    int		mBlkNum;		 
    int		clBlkNum;		 
    char * 	memArea;		 
    int		memSize;		 
    } M_CL_CONFIG;

 







typedef struct clDesc
    {
    int		clSize;			 
    int		clNum;			 
    char *	memArea;		 
    int		memSize;		 
    } CL_DESC; 

 

typedef struct clBuff
    {
    struct clBuff *	pClNext; 	 
    } CL_BUF;

typedef CL_BUF * CL_BUF_ID; 

 

typedef struct clPool
    {
    int			clSize;		 
    int			clLg2;		 
    int			clNum; 		 
    int			clNumFree; 	 
    int			clUsage;	 
    CL_BUF_ID		pClHead;	 
    struct netPool *	pNetPool;	 
    } CL_POOL; 

typedef CL_POOL * CL_POOL_ID; 

 

typedef struct mHdr
    {
    struct mBlk *	mNext;		 
    struct mBlk *	mNextPkt;	 
    char *		mData;		 
    int			mLen;		 
    UCHAR		mType;		 
    UCHAR		mFlags;		 
    USHORT		reserved; 
    } M_BLK_HDR;

 

typedef struct	pktHdr
    {
    struct ifnet *	rcvif;		 
    int			len;		 
    } M_PKT_HDR;

typedef union clBlkList
    {
    struct clBlk * 	pClBlkNext;	 
    char * 		pClBuf;		 
    } CL_BLK_LIST;
    
 

typedef struct clBlk
    {
    CL_BLK_LIST 	clNode;		 
    UINT		clSize;		 
    int			clRefCnt;	 
    FUNCPTR		pClFreeRtn;	 
    int			clFreeArg1;	 
    int			clFreeArg2;	 
    int			clFreeArg3;	 
    struct netPool *	pNetPool;	 
    } CL_BLK;

 

typedef struct mBlk
    {
    M_BLK_HDR 	mBlkHdr; 		 
    M_PKT_HDR	mBlkPktHdr;		 
    CL_BLK *	pClBlk;			 
    } M_BLK;

 

typedef struct mbstat
    {
    ULONG	mNum;			 
    ULONG	mDrops;			 
    ULONG	mWait;			 
    ULONG	mDrain;			 
    ULONG	mTypes[256];		 
    } M_STAT;

typedef M_BLK * 	M_BLK_ID;
typedef CL_BLK *	CL_BLK_ID;
typedef struct netPool 	NET_POOL;
typedef struct poolFunc POOL_FUNC;
typedef NET_POOL * 	NET_POOL_ID;

struct	poolFunc			 
    {
     
    STATUS	(*pInitRtn) (NET_POOL_ID pNetPool, M_CL_CONFIG * pMclBlkConfig,
                             CL_DESC * pClDescTbl, int clDescTblNumEnt,
			     BOOL fromKheap);

     
    void	(*pMblkFreeRtn) (NET_POOL_ID pNetPool, M_BLK_ID pMblk);

     
    void	(*pClBlkFreeRtn) (CL_BLK_ID pClBlk);

     
    void	(*pClFreeRtn) (NET_POOL_ID pNetPool, char * pClBuf);

     
    M_BLK_ID 	(*pMblkClFreeRtn) (NET_POOL_ID pNetPool, M_BLK_ID pMblk);

     
    M_BLK_ID	(*pMblkGetRtn) (NET_POOL_ID pNetPool, int canWait, UCHAR type);

     
    CL_BLK_ID	(*pClBlkGetRtn) (NET_POOL_ID pNetPool, int canWait);
    
     
    char *	(*pClGetRtn) (NET_POOL_ID pNetPool, CL_POOL_ID pClPool);

     
    STATUS	(*pMblkClGetRtn) (NET_POOL_ID pNetPool, M_BLK_ID pMblk,
                                  int bufSize, int canWait, BOOL bestFit);

     
    CL_POOL_ID	(*pClPoolIdGetRtn) (NET_POOL_ID pNetPool, int	bufSize,
                                    BOOL bestFit);
    };

struct netPool				 
    {
    M_BLK_ID	pmBlkHead;		 
    CL_BLK_ID	pClBlkHead;		 
    int		mBlkCnt;		 
    int		mBlkFree;		 


    int		clMask;			 
    int		clLg2Max;		 
    int		clSizeMax;		 
    int		clLg2Min;		 
    int		clSizeMin;		 
    CL_POOL * 	clTbl [((16   - 6  )  + 1) ];	 
    M_STAT *	pPoolStat;		 
    POOL_FUNC *	pFuncTbl;		 
    };

                     
                    
extern  STATUS 		netBufLibInit (void);
extern  STATUS 		netPoolInit (NET_POOL_ID pNetPool,
                                     M_CL_CONFIG * pMclBlkConfig,
                                     CL_DESC * pClDescTbl, int clDescTblNumEnt,
                                     POOL_FUNC * pFuncTbl);
extern  STATUS		netPoolDelete (NET_POOL_ID);
extern  void		netMblkFree (NET_POOL_ID pNetPool, M_BLK_ID pMblk);
extern  void		netClBlkFree (NET_POOL_ID pNetPool, CL_BLK_ID pClBlk);
extern  void 		netClFree (NET_POOL_ID pNetPool, UCHAR * pClBuf);
extern  M_BLK_ID 	netMblkClFree (M_BLK_ID pMblk);
extern  void		netMblkClChainFree (M_BLK_ID pMblk);
extern  M_BLK_ID 	netMblkGet (NET_POOL_ID pNetPool, int canWait,
                                    UCHAR type);
extern  CL_BLK_ID	netClBlkGet (NET_POOL_ID pNetPool, int canWait);
extern  char *	 	netClusterGet (NET_POOL_ID pNetPool,
                                       CL_POOL_ID pClPool);
extern  STATUS 	 	netMblkClGet (NET_POOL_ID pNetPool, M_BLK_ID pMblk,
                                      int bufSize, int canWait, BOOL bestFit);
extern  M_BLK_ID 	netTupleGet (NET_POOL_ID pNetPool, int bufSize,
                                     int canWait, UCHAR type, BOOL bestFit);

extern  M_BLK_ID 	netTupleGet2 (NET_POOL_ID, int, int);

extern  CL_BLK_ID  	netClBlkJoin (CL_BLK_ID pClBlk, char * pClBuf,
                                      int size, FUNCPTR pFreeRtn, int arg1,
                                      int arg2, int arg3);
extern  M_BLK_ID  	netMblkClJoin (M_BLK_ID pMblk, CL_BLK_ID pClBlk);
extern  CL_POOL_ID 	netClPoolIdGet (NET_POOL_ID pNetPool, int bufSize,
                                        BOOL bestFit);

extern  int 		netMblkToBufCopy (M_BLK_ID pMblk, char * pBuf,
                                          FUNCPTR pCopyRtn);
extern  int		netMblkOffsetToBufCopy (M_BLK_ID pMblk, int offset,
                                                char * pBuf, int len,
                                                FUNCPTR	pCopyRtn);
extern  M_BLK_ID 	netMblkDup (M_BLK_ID pSrcMblk, M_BLK_ID	pDestMblk);
extern  M_BLK_ID 	netMblkChainDup (NET_POOL_ID pNetPool, M_BLK_ID	pMblk,
                                         int offset, int len, int canWait);











# 76 "../../../infra/SYS/target/h/net/mbuf.h" 2






 























 



 


 













 





 





 





 





 






 








# 176 "../../../infra/SYS/target/h/net/mbuf.h"
# 193 "../../../infra/SYS/target/h/net/mbuf.h"


 










 








 















 

 












 






















        




    
 















 

extern  struct mbstat 	mbstat;			 
extern  M_CL_CONFIG	mClBlkConfig;		 
extern  CL_DESC 		clDescTbl [];		 
extern  int		clDescTblNumEnt; 	 
extern  M_CL_CONFIG	sysMclBlkConfig;	 
extern  CL_DESC 		sysClDescTbl [];	 
extern  int		sysClDescTblNumEnt; 	 


extern  NET_POOL_ID	_pNetDpool;		 
extern  NET_POOL_ID	_pNetSysPool;		 


 

extern int max_linkhdr;		 
extern int max_protohdr;	 
extern int max_hdr;		 
extern int max_datalen;		 

extern struct	mBlk  * netMblkClFree( struct mBlk  * ) ;
extern struct	mBlk  *m_getclr (int, UCHAR, int, BOOL);
extern struct	mBlk  *m_prepend (struct mBlk  *, int, int);
extern struct	mBlk  *m_pullup (struct mBlk  *, int);
extern void	m_adj (struct mBlk  *, int);
extern void	netMblkClChainFree( struct mBlk  * ) ;
extern void 	m_cat (struct mBlk  *, struct mBlk  *);
extern struct 	mBlk  *m_devget (char *, int, int, struct ifnet *,
				void (*copy)());
struct mBlk  *	mBufClGet (int, UCHAR, register  int, BOOL); 
struct mBlk  * 	mHdrClGet (int, UCHAR, register  int, BOOL); 

extern  struct mBlk  * bcopy_to_mbufs (u_char * buf, int totlen, int off0,
                                     struct ifnet * ifp, int width);
extern  struct mBlk  * build_cluster (u_char * buf, int totlen,
                                    struct ifnet * ifp, u_char ctype,
                                    u_char * pRefcnt, FUNCPTR freeRtn,
                                    int arg1, int arg2, int arg3);
extern  int	copyFromMbufs (char * pIobuf, struct mBlk  * pMbuf, int width);






# 58 "../../../infra/SYS/target/h/net/socketvar.h" 2






 






struct socket
    {
    short	so_type;		 
    short	so_options;		 
    short	so_linger;		 
    short	so_state;		 
    caddr_t	so_pcb;			 
    struct	protosw *so_proto;	 

     



    int		so_userArg;		 
    SEM_ID	so_timeoSem;		 
    int		so_fd;			 

     











    struct	socket *so_head;	 
    struct	socket *so_q0;		 
    struct	socket *so_q;		 
    short	so_q0len;		 
    short	so_qlen;		 
    short	so_qlimit;		 
    short	so_timeo;		 
    u_short	so_error;		 
    short	so_pgrp;		 
    u_long	so_oobmark;		 

     

    struct	sockbuf
	{
	u_long		sb_cc;		 
	u_long		sb_hiwat;	 
	u_long		sb_mbcnt;	 
	u_long		sb_mbmax;	 
	u_long		sb_lowat;	 
	struct		mBlk  *sb_mb;	 
	struct		proc *sb_sel;	 
	short		sb_timeo;	 
	short		sb_flags;	 

	SEM_ID		sb_Sem;		 
	int		sb_want;	 
	} so_rcv, so_snd;










    void	(*so_upcall) (struct socket *so, caddr_t arg, int waitf);
    caddr_t	so_upcallarg;		 
    BOOL 	selectFlag; 		 
    SEL_WAKEUP_LIST so_selWakeupList;  
    int         vsid;                  
    };






 















 



 









 



 





 







 







 







 








 















extern  VOIDFUNCPTR sowakeupHook;

extern u_long	sb_max;

 


 
extern	char netio[], netcon[], netcls[];

 


extern int soo_ioctl (struct socket * so, int cmd, caddr_t data);
extern int soo_select (struct socket *so, SEL_WAKEUP_NODE *wakeupNode);
extern int soo_unselect (struct socket *so, SEL_WAKEUP_NODE *wakeupNode); 

struct mBlk ;
struct sockaddr;

extern void	sbappend (struct sockbuf *sb, struct mBlk  *m);
extern int	sbappendaddr (struct sockbuf *sb, struct sockaddr *asa,
	    struct mBlk  *m0, struct mBlk  *control);
extern int	sbappendcontrol (struct sockbuf *sb, struct mBlk  *m0,
	    struct mBlk  *control);
extern void	sbappendrecord (struct sockbuf *sb, struct mBlk  *m0);
extern void	sbcheck (struct sockbuf *sb);
extern void	sbcompress (struct sockbuf *sb, struct mBlk  *m, struct mBlk  *n);
extern void	sbdrop (struct sockbuf *sb, int len);
extern void	sbdroprecord (struct sockbuf *sb);
extern void	sbflush (struct sockbuf *sb);
extern void	sbinsertoob (struct sockbuf *sb, struct mBlk  *m0);
extern void	sbrelease (struct sockbuf *sb);
extern int	sbreserve (struct sockbuf *sb, u_long cc);
extern void	sbwait (struct sockbuf *sb);
extern void	sbwakeup(struct socket *so, struct sockbuf *sb,
			 SELECT_TYPE wakeupType);
extern int	sb_lock (struct sockbuf *sb);
extern int	soabort (struct socket *so);
extern int	soaccept (struct socket *so, struct mBlk  *nam);
extern int	sobind (struct socket *so, struct mBlk  *nam);
extern void	socantrcvmore (struct socket *so);
extern void	socantsendmore (struct socket *so);
extern int	soclose (struct socket *so);
extern int	soconnect (struct socket *so, struct mBlk  *nam);
extern int	soconnect2 (struct socket *so1, struct socket *so2);
extern int	socreate (int dom, struct socket **aso, int type, int proto);
extern int	sodisconnect (struct socket *so);
extern void	sofree (struct socket *so);
extern int	sogetopt (struct socket *so, int level, int optname,
	    struct mBlk  **mp);
extern void	sohasoutofband (struct socket *so);
extern void	soisconnected (struct socket *so);
extern void	soisconnecting (struct socket *so);
extern void	soisdisconnected (struct socket *so);
extern void	soisdisconnecting (struct socket *so);
extern int	solisten (struct socket *so, int backlog);
extern struct socket *
	sonewconn1 (struct socket *head, int connstatus);
extern void	soqinsque (struct socket *head, struct socket *so, int q);
extern int	soqremque (struct socket *so, int q);
extern int	soreceive (struct socket *so, struct mBlk  **paddr, 
			   struct uio *pUio, struct mBlk  **mp0, struct mBlk  **
			   controlp, int *flagsp);
extern int	soreserve (struct socket *so, u_long sndcc, u_long rcvcc);
extern void	sorflush (struct socket *so);
extern int	sosend (struct socket *so, struct mBlk  *addr, struct uio *uio,
	    struct mBlk  *top, struct mBlk  *control, int flags);
extern int	sosetopt (struct socket *so, int level, int optname,
	    struct mBlk  *m0);
extern int	soshutdown (struct socket *so, int how);
extern void	sowakeup (struct socket *so, struct sockbuf *sb, 
			  SELECT_TYPE wakeupType);






# 61 "../../../infra/SYS/target/h/net/route.h" 2

# 1 "../../../infra/SYS/target/h/net/radix.h" 1
 

 

 


































 






















 



struct radix_node {
	struct	radix_mask *rn_mklist;	 
	struct	radix_node *rn_p;	 
	short	rn_b;			 
	char	rn_bmask;		 
	u_char	rn_flags;		 




	union {
		struct {			 
			caddr_t	rn_Key;	 
			caddr_t	rn_Mask;	 
			struct	radix_node *rn_Dupedkey;
		} rn_leaf;
		struct {			 
			int	rn_Off;		 
			struct	radix_node *rn_L; 
			struct	radix_node *rn_R; 
		}rn_node;
	}		rn_u;





};








 



extern struct radix_mask {
	short	rm_b;			 
	char	rm_unused;		 
	u_char	rm_flags;		 
	struct	radix_mask *rm_mklist;	 
	union	{
		caddr_t	rmu_mask;		 
		struct	radix_node *rmu_leaf;	 
	}	rm_rmu;
	int	rm_refs;		 
} *rn_mkfreelist;













struct radix_node_head {
	struct	radix_node *rnh_treetop;
	int	rnh_addrsize;		 
	int	rnh_pktsize;		 
	struct	radix_node *(*rnh_addaddr)	 
		(void *v, void *mask,
		     struct radix_node_head *head, struct radix_node nodes[]);
	struct	radix_node *(*rnh_addpkt)	 
		(void *v, void *mask,
		     struct radix_node_head *head, struct radix_node nodes[]);
	struct	radix_node *(*rnh_deladdr)	 
		(void *v, void *mask, struct radix_node_head *head);
	struct	radix_node *(*rnh_delpkt)	 
		(void *v, void *mask, struct radix_node_head *head);
	struct	radix_node *(*rnh_matchaddr)	 
		(void *v, struct radix_node_head *head, int);
        struct  radix_node *(*rnh_lookup)        
                (void *v, void *mask, struct radix_node_head *head);
	struct	radix_node *(*rnh_matchpkt)	 
		(void *v, struct radix_node_head *head);
	int	(*rnh_walktree)			 
		(struct radix_node_head *head, int (*f)(), void *w);
	struct	radix_node rnh_nodes[3];	 
};















void	 rn_init (void);

int	 rn_inithead (struct radix_node_head **, int);
int	 rn_destroyhead (struct radix_node_head *);
int	 rn_refines (void *, void *);
int	 rn_walksubtree (struct radix_node_head *, void *, void *,
                         int (*)(), void *);
int	 rn_walktree (struct radix_node_head *, int (*)(), void *);
struct radix_node
	 *rn_addmask (void *, int, int),
	 *rn_addroute (void *, void *, struct radix_node_head *,
			struct radix_node [2]),
	 *rn_delete (void *, void *, struct radix_node_head *),
	 *rn_insert (void *, struct radix_node_head *, int *,
			struct radix_node [2]),
	 *rn_match (void *, struct radix_node_head *, int),
	 *rn_newpair (void *, int, struct radix_node[2]),
	 *rn_search (void *, struct radix_node *),
	 *rn_search_m (void *, struct radix_node *, void *);







# 62 "../../../infra/SYS/target/h/net/route.h" 2







 






 




struct route
    {
    struct	rtentry *ro_rt;
    struct	sockaddr ro_dst;
    };

 



struct rt_metrics {
	u_long	rmx_locks;	 
	u_long	rmx_mtu;	 
	u_long	rmx_expire;	 
	u_long	rmx_recvpipe;	 
	u_long	rmx_sendpipe;	 
	u_long	rmx_ssthresh;	 
	u_long	rmx_rtt;	 
	u_long	rmx_rttvar;	 
	u_short	rmx_hopcount;	 
        u_short rmx_mss; 	 

         

        long 	value1;
        long 	value2;
        long 	value3;
        long 	value4;
        long 	value5;
        long 	routeTag;
        u_char  weight;
};

 






struct sockaddr_gate
    {
    u_char         gate_len;
    u_char         gate_family;
    u_char         routeProto;
    u_char         weight;
    u_long         gate_addr;
    long           value1;
    long           value2;
    long           value3;
    long           value4;
    long           value5;
    long           routeTag;
    };

 







 










struct rtentry {
	struct	radix_node rt_nodes[2];	 


	struct	sockaddr *rt_gateway;	 
	short	rt_flags;		 
	short	rt_refcnt;		 
	u_long	rt_use;			 
	struct	ifnet *rt_ifp;		 
	struct	ifaddr *rt_ifa;		 
	struct	sockaddr *rt_genmask;	 
	caddr_t	rt_llinfo;		 
	struct	rtentry *rt_gwroute;	 
        int	rt_mod;			  
	struct	rt_metrics rt_rmx;	 
        struct  rtentry *rt_parent;      
};

 



struct ortentry {
	u_long	rt_hash;		 
	struct	sockaddr rt_dst;	 
	struct	sockaddr rt_gateway;	 
	short	rt_flags;		 
	short	rt_refcnt;		 
	u_long	rt_use;			 
	struct	ifnet *rt_ifp;		 
        int	rt_mod;			 
};
























 

struct	rtstat
    {
    short	rts_badredirect;	 
    short	rts_dynamic;		 
    short	rts_newgateway;		 
    short	rts_unreach;		 
    short	rts_wildcard;		 
    };

 


struct rt_msghdr {
	u_short	rtm_msglen;	 
	u_char	rtm_version;	 
	u_char	rtm_type;	 
	u_short	rtm_index;	 
	int	rtm_flags;	 
	int	rtm_addrs;	 
	pid_t	rtm_pid;	 
	int	rtm_seq;	 
	int	rtm_errno;	 
	int	rtm_use;	 
	u_long	rtm_inits;	 
	struct	rt_metrics rtm_rmx;  
};


















# 271 "../../../infra/SYS/target/h/net/route.h"











# 294 "../../../infra/SYS/target/h/net/route.h"


 











 












 










struct rt_addrinfo {
	int	rti_addrs;
	struct	sockaddr *rti_info[8 ];
};

struct route_cb {
	int	ip_count;
	int	ns_count;
	int	iso_count;
	int	any_count;
};











extern	struct	rtstat	rtstat;
extern	struct	radix_node_head *rt_tables[];
extern  int	rtmodified; 




int	 route_init (void);

void	 rt_maskedcopy (struct sockaddr *,
	    struct sockaddr *, struct sockaddr *);
int	 rt_setgate (struct rtentry *,
	    struct sockaddr *, struct sockaddr *);
void	 rtable_init (void **);
void	 rtalloc (struct route *);
struct rtentry *
	 rtalloc1 (struct sockaddr *, int, int);
void	 rtfree (struct rtentry *);
int	 rtinit (struct ifaddr *, int, int);
int	 rtioctl (int, caddr_t);
int	 rtredirect (struct sockaddr *, struct sockaddr *,
	    struct sockaddr *, int, struct sockaddr *, struct rtentry **);
int	 rtrequest (int, struct sockaddr *,
	    struct sockaddr *, struct sockaddr *, int, struct rtentry **);
int	 rtrequest11 (int, struct sockaddr *,
	    struct sockaddr *, struct sockaddr *, int, struct rtentry **);






# 72 "../../../infra/SYS/target/h/net/if.h" 2

# 1 "../../../infra/SYS/target/h/ifLib.h" 1
 

 

 




















 


extern STATUS   ifAddrAdd (char *interfaceName, char *interfaceAddress,
                           char *broadcastAddress, int subnetMask);
extern STATUS 	ifAddrSet (char *interfaceName, char *interfaceAddress);
extern STATUS 	ifAddrDelete (char *interfaceName, char *interfaceAddress);
extern STATUS 	ifAddrGet (char *interfaceName, char *interfaceAddress);
extern STATUS 	ifBroadcastSet (char *interfaceName, char *broadcastAddress);
extern STATUS 	ifBroadcastGet (char *interfaceName, char *broadcastAddress);
extern STATUS 	ifDstAddrSet (char *interfaceName, char *dstAddress);
extern STATUS 	ifDstAddrGet (char *interfaceName, char *dstAddress);
extern STATUS 	ifMaskSet (char *interfaceName, int netMask);
extern STATUS 	ifMaskGet (char *interfaceName, int *netMask);
extern STATUS 	ifFlagChange (char *interfaceName, int flags, BOOL on);
extern STATUS 	ifFlagSet (char *interfaceName, int flags);
extern STATUS 	ifFlagGet (char *interfaceName, int *flags);
extern STATUS 	ifMetricSet (char *interfaceName, int metric);
extern STATUS 	ifMetricGet (char *interfaceName, int *pMetric);
extern int 	ifRouteDelete (char *ifName, int unit);
extern struct 	ifnet *ifunit (char *ifname);
extern struct	ifnet *ifIndexToIfpNoLock (int ifIndex);
extern struct	ifnet *ifIndexToIfp (int ifIndex);
extern unsigned short	ifNameToIfIndex (char *ifname);
extern STATUS	ifIndexToIfName (unsigned short ifIndex, char *ifname);
extern STATUS	ifUnnumberedSet(char *, char *, char *, char *);
extern int 	ifAllRoutesDelete (char *, int);
# 78 "../../../infra/SYS/target/h/ifLib.h"






# 73 "../../../infra/SYS/target/h/net/if.h" 2






 
























struct	ifqueue {
	struct	mBlk  *ifq_head;
	struct	mBlk  *ifq_tail;
	int	ifq_len;
	int	ifq_maxlen;
	int	ifq_drops;
};

struct mBlk ;
struct proc;
struct rtentry;
struct socket;
struct ether_header;

 



struct	if_data {
	 
	u_char	ifi_type;		 
	u_char	ifi_addrlen;		 
	u_char	ifi_hdrlen;		 
	u_long	ifi_mtu;		 
	u_long	ifi_metric;		 
	u_long	ifi_baudrate;		 
	 
	u_long	ifi_ipackets;		 
	u_long	ifi_ierrors;		 
	u_long	ifi_opackets;		 
	u_long	ifi_oerrors;		 
	u_long	ifi_collisions;		 
	u_long	ifi_ibytes;		 
	u_long	ifi_obytes;		 
	u_long	ifi_imcasts;		 
	u_long	ifi_omcasts;		 
	u_long	ifi_iqdrops;		 
	u_long	ifi_noproto;		 
	u_long  ifi_lastchange;		 
};

 





struct ifnet {
	char	*if_name;		 
	struct	ifnet *if_next;		 
	struct	ifaddr *if_addrlist;	 
	int	if_pcount;		 
	caddr_t	if_bpf;			 
	u_short	if_index;		 
	short	if_unit;		 
	short	if_timer;		 
	short	if_flags;		 
	struct	if_data if_data;	 
	struct	mBlk *	pInmMblk; 	 
	int	(*if_init) 
		(int unit);		 
	int	(*if_output)		 
		(struct ifnet *, struct mBlk  *, struct sockaddr *, 
		 struct rtentry *);	
	int	(*if_start) 		 
    		(struct ifnet *);
	int	(*if_ioctl)		 
		(struct ifnet *, int cmd, caddr_t data);
	int	(*if_reset)		 
		(int unit);	
	int	(*if_resolve)();	 
	void	(*if_watchdog)		 
		(int unit);
	struct	ifqueue if_snd;		 
        void * 	pCookie; 		 



};














































 







 








# 254 "../../../infra/SYS/target/h/net/if.h"








 






# 277 "../../../infra/SYS/target/h/net/if.h"
# 291 "../../../infra/SYS/target/h/net/if.h"


# 303 "../../../infra/SYS/target/h/net/if.h"


# 313 "../../../infra/SYS/target/h/net/if.h"




 





struct ifaddr {
	struct	sockaddr *ifa_addr;	 
	struct	sockaddr *ifa_dstaddr;	 

	struct	sockaddr *ifa_netmask;	 
	struct	ifnet *ifa_ifp;		 
	struct	ifaddr *ifa_next;	 
	void	(*ifa_rtrequest)();	 
	u_short	ifa_flags;		 
	short	ifa_refcnt;		 
	int	ifa_metric;		 
};


 



struct if_msghdr {
	u_short	ifm_msglen;	 
	u_char	ifm_version;	 
	u_char	ifm_type;	 
	int	ifm_addrs;	 
	int	ifm_flags;	 
	u_short	ifm_index;	 
	struct	if_data ifm_data; 
};

 



struct ifa_msghdr {
	u_short	ifam_msglen;	 
	u_char	ifam_version;	 
	u_char	ifam_type;	 
	int	ifam_addrs;	 
	int	ifam_flags;	 
	u_short	ifam_index;	 
	int	ifam_metric;	 
};

 





struct	ifreq {

	char	ifr_name[16 ];		 
	union {
		struct	sockaddr ifru_addr;
		struct	sockaddr ifru_dstaddr;
		struct	sockaddr ifru_broadaddr;
		short	ifru_flags;
		int	ifru_metric;
		caddr_t	ifru_data;
	} ifr_ifru;






};

struct ifaliasreq {
	char	ifra_name[16 ];		 
	struct	sockaddr ifra_addr;
	struct	sockaddr ifra_broadaddr;
	struct	sockaddr ifra_mask;
};

 



struct	ifmreq {

	char	ifr_name[16 ];		 
	int	ifr_tableLen;
	char 	*ifr_table;
};

 





struct	ifconf {
	int	ifc_len;		 
	union {
		caddr_t	ifcu_buf;
		struct	ifreq *ifcu_req;
	} ifc_ifcu;


};

 




typedef struct mtuQuery_t 
	{
	UCHAR family;
	UINT32 dstIpAddr;
	UINT32 mtu;
	} MTU_QUERY;











extern struct ifnet * 	ifnet;		 
extern struct ifqueue 	rawintrq;	 

extern STATUS		if_attach (struct ifnet *);
extern void		if_down (struct ifnet *);
extern void		if_qflush (struct ifqueue *);


extern void		if_slowtimo (void);




extern void		if_up (struct ifnet *);
extern int		ifconf (int, caddr_t);
extern void		ifinit (void);
extern int		ifioctl (struct socket *, u_long, caddr_t);
extern int		ifpromisc (struct ifnet *, int);
extern struct ifaddr *	ifa_ifwithaddr (struct sockaddr *);
extern struct ifaddr *	ifa_ifwithaf (int);
extern struct ifaddr *	ifa_ifwithdstaddr (struct sockaddr *);
extern struct ifaddr *	ifa_ifwithnet (struct sockaddr *);
extern struct ifaddr *	ifaof_ifpforaddr (struct sockaddr *, struct ifnet *);
extern struct ifaddr *	ifa_ifwithroute(int, struct sockaddr *, 
					struct sockaddr*);
extern void		ifafree (struct ifaddr *);
extern void		link_rtrequest (int, struct rtentry *,
                                        struct sockaddr *);

struct port_enable_info{
	int port;
	int disable;
	int protocol;	 
	char description[64];
	int hitcount;
	struct port_enable_info *pnext;
};

typedef struct portUsing_hash{
	int count;
	struct port_enable_info *pnext;
}portUsing_Hash;	 






# 59 "../../../infra/SYS/target/h/netinet/in.h" 2







 




 





















 









 


struct in_addr {
	u_long s_addr;
};

 










































 


struct sockaddr_in {
	u_char	sin_len;
	u_char	sin_family;
	u_short	sin_port;
	struct	in_addr sin_addr;
	char	sin_zero[8];
};

 





struct sockaddr_rt {
	u_char	srt_len;
	u_char	srt_family;
        u_char  srt_proto;
        u_char  srt_tos;          
	struct	in_addr srt_addr;
	char	sin_zero[8];
        };












 






struct ip_opts {
	struct	in_addr ip_dst;		 
	char	opts[40];		 
};

 



















 






 









struct ip_mreq {
 struct in_addr imr_multiaddr;  
   union {
     struct in_addr u_imr_interface; 
     int       u_imr_index;       
 } imr_arg;
};







 








# 286 "../../../infra/SYS/target/h/netinet/in.h"

 



















 
















# 353 "../../../infra/SYS/target/h/netinet/in.h"


extern	struct domain inetdomain;
extern  BOOL inet_netmatch(struct sockaddr_in *sin1, struct sockaddr_in *sin2);
extern  void in_makeaddr_b(u_long net, u_long host, struct in_addr * pAddr);
extern 	u_long in_lnaof();
extern	void in_ifaddr_remove();
extern	int in_broadcast (struct in_addr, struct ifnet *);
extern	int in_canforward (struct in_addr);
extern	int in_cksum (struct mBlk  *, int);
extern	int in_localaddr (struct in_addr);
extern	u_long in_netof (struct in_addr);
extern	void in_socktrim (struct sockaddr_in *);
extern  struct in_ifaddr *in_iaonnetof(u_long net);






# 5 "../../../include/boot_info.h" 2


# 1 "../../../infra/inc/bspType.h" 1

# 23 "../../../infra/inc/bspType.h"




# 7 "../../../include/boot_info.h" 2



 





 










 













 
 


 



























 










 
extern void tmd_init(void *bp);

 


struct sirocco_bootp_req_info {
    char serial_number[12];
    int slot_number;
    short int board_style;
    short int card_function;
};

 


struct sirocco_bootp_resp_info {
    struct in_addr scm_addr;
    struct in_addr scm_mask;
    struct in_addr sca_addr;
    struct in_addr sca_mask;
    struct in_addr gate_addr;
};

union sirocco_bootp_vend_info {
    struct sirocco_bootp_req_info req_info;
    struct sirocco_bootp_resp_info resp_info;
};





extern int Inital_load_complete;
extern unsigned int InitialTextLoadAddr;
extern unsigned int TextLoadAddr;


 
int getSlotNumber(void);


# 12 "../../../include/globals.h" 2







extern int sysShutdown;
extern int sysPoweroff;

typedef enum {
    INIT_ERROR_NONE,
    INIT_ERROR_MINOR,
    INIT_ERROR_MAJOR,
    INIT_ERROR_FATAL
} InitErrorEnum;

struct globals {
     



    char externalDevName[10 ];   
    unsigned long externalIPAddress;     
    unsigned long externalNetmask;   
    unsigned long externalBroadcast;     
    unsigned long gatewayIpAddress;  
    unsigned long nodeIPAddress;     
    unsigned long nodeIPNetmask;     
    char serialNumber[12 ];   
    unsigned long physicalSlot;  
    unsigned long logicalSlot;   

    int cardFunction;            
    int boardStyle;              
    int cardState;               
    int warmStart;               
    int coldStart;               
    
    struct acb **applicationACBs;

    InitErrorEnum ErrorLevel;    

    int activeInterface;         
    char actIfName[10 ];     
    unsigned long activeIpAddr;  
    char loadPath[100 ];  
    int chassisType;             

    void *moduleInfo;            
    void *logQueue;              
    unsigned int *sizes;         
    unsigned int *npool;         
    unsigned int *nused;         
    unsigned long memRatio;

    unsigned long externalIPAddressSec;
    unsigned long externalNetmaskSec;
    int ethernetEnable;

     



    unsigned long GmplsIPAddress; 
    unsigned long GmplsIPNetmask; 

};



 





 










 















extern struct globals globals;

extern short getPortNumber(void);






 









typedef unsigned char Uint8;
typedef signed char Int8;

typedef unsigned short Uint16;
typedef short Int16;

typedef unsigned int Uint32;
typedef int Int32;


typedef unsigned char Boolean;



# 12 "../../../include/cmnds/cmnd_coll.h" 2


 

# 1 "../../../include/fdr_services.h" 1



# 1 "../../../include/globals.h" 1
#ident  "%full_filespec: globals.h#17:incl:1 %"

# 148 "../../../include/globals.h"

# 4 "../../../include/fdr_services.h" 2






 
 
 
 




   
typedef struct
{
    short proxy_slot_list[40 +1];      
} fdrProxyList_t;

 
 
 
 
 
 
 
 
struct cmndSlotUpdate
{
    command_tt head;
    int reg_flags;                       
    int physical_slot;                   
    int board_function;                  

    int card_function;                   
    int card_style;                      
    int card_state;                      
    int last_card_style;                 
    int isCardSmart;                     
    int proxy_slot;                      

    int response_required;               
    int logical_slot;                    
    unsigned int sparedslots_bitmask;    
};
typedef struct cmndSlotUpdate cmndSlotUpdate_t;

 
 
 

 
 
 
 
 












 
 
 
 
 









extern STATUS fdrcoll_Register_Notification(UINT32 physical_slot, int reg_flags);
extern STATUS fdrcoll_UnRegister_Notification(UINT32 physical_slot,
                                              int reg_flags);
 
 
 
 
 
extern int fdr_poll_slot_state(UINT32 phys_slot);
extern int fdr_poll_slot_style(UINT32 phys_slot);
extern int fdr_poll_last_slot_style(UINT32 phys_slot);
extern int fdr_poll_slot_function(UINT32 phys_slot);
extern int fdr_poll_slot_operational_function(UINT32 phys_slot);
extern int fdr_poll_slot_running_function(UINT32 phys_slot);
extern int fdr_poll_proxy_slot(UINT32 phys_slot);
extern int fdr_poll_slot_is_card_smart(UINT32 phys_slot);
extern int fdr_get_logical_slot_of_physical_slot(UINT32 phys_slot);
extern int fdr_get_physical_slot_of_logical_slot(UINT32 logical_slot);
extern int fdr_fetch_proxy_slots_for_card(UINT32 phys_slot,
                                          fdrProxyList_t *proxylist);
extern int fdr_poll_all_info_for_slot(UINT32 pslot, cmndSlotUpdate_t *csu);

 
 
 
 
 
extern int fdrcoll_get_local_card_state(void);
extern STATUS fdrcoll_Register_Mapping_Notification(void);
extern void fdr_transmit_slot_notification_response(cmndSlotUpdate_t *msg);
extern unsigned int fdrcoll_what_was_last_Eval_broadcast_key(void);
extern int fdr_poll_slot_is_soft_synchronized(UINT32 phys_slot);


 

   










# 16 "../../../include/cmnds/cmnd_coll.h" 2







 
 
 

 
    typedef struct {
        command_tt head;
        applId_tt applId;
        unsigned int new_state;
        unsigned int failure_mode;
        unsigned int slot_event_key;
    } cmndFmtFDRcommand_t;

    typedef struct {
        command_tt head;
        unsigned int card_state;
        unsigned int slot_event_key;

    } cmndFdrCardStateChange_t;

    typedef struct {
        command_tt head;
        int board_type;
        int board_function;
        int card_state;
        int phys_slot;
        unsigned int slot_event_key;
    } cmndFdrQuery_t;

    typedef struct {
        command_tt head;
        unsigned int slot_event_key;
        char i2cAddr[30];
        int phys_slot;
        unsigned int entity_id_number;
        short entity_index;
    } cmndFdrBackplaneQuery_t;

    typedef struct {
        command_tt head;
        int physical_slot;
        int prior_card_type;
    } cmndFdrRemoveIntf_t;

    typedef struct {
        command_tt head;
        int physical_slot;
        char i2cAddr[30];
        unsigned int slot_event_key;

    } cmndFdrDumbCardQuery_t;

 
    typedef struct {
        command_tt head;
        int notified_card_state;
    } cmndFdrMapNotify_t;

    typedef struct {
        command_tt head;
        int applnum;
        char applname[30];
        int notification;
        int status;
        int flag_fdr_reports;
        int options;
    } cmndFdrCollReg_t;

 







 
 
 




































 





 
 
 
 





 






 








# 24 "../../../appl/CMM/inc/cm_command.h" 2

 







enum
{
    CMM_LEVEL_ERR = 0,
    CMM_LEVEL_CMC,
    CMM_LEVEL_SLOT,
    CMM_LEVEL_STATE,
    CMM_LEVEL_ALMEVT
};

enum
{
    CMM_LEVEL_CAI = 0,
    CMM_LEVEL_CMI
};

enum
{
    CMM_LEVEL_CAD = 0,
    CAD_LEVEL_ALMEVT,
    CAD_LEVEL_ERR
};
enum
{
    CMND_CM_DATA_INIT_COMPLETE = 0x3800 ,
    CMND_CM_CARDPROV,
    CMND_CM_CARDSTATECHG,
    CMND_CM_READYTOINIT,
    CMND_CM_READYTOACT,
    CMND_CM_PREPARETOACT,
    CMND_CM_DEINIT,
    CMND_CM_DEACT,
    CMND_CM_INITDRV,
    CMND_CM_RESTOREAPP,
    CMND_CM_INITAPP,
    CMND_CM_ACTDRV,
    CMND_CM_ACTAPP,
    CMND_CM_INITDRVOK,
    CMND_CM_INITDRVFAIL,
    CMND_CM_INITAPPOK,
    CMND_CM_INITAPPFAIL,
    CMND_CM_ACTDRVOK,
    CMND_CM_ACTDRVFAIL,
    CMND_CM_ACTAPPOK,
    CMND_CM_ACTAPPFAIL,
    CMND_CM_CHECKSTATE,          
    CMND_CM_CHECKSTATE_ACK,          
    CMND_CM_DRVSTAE,             
    CMND_CM_INSERT,
    CMND_CM_REMOVE,
    CMND_CM_FAIL,
    CMND_CM_FAILCLR,
    CMND_CM_ACTIVECC,
    CMND_CM_PHYINSERT,
    CMND_CM_APP_QUERY_MIBCARD,
    CMND_CM_MIBCARD,
    CMND_CM_MIBCARD_ACK,
    CMND_CM_CARDSTATE,
    CMND_CM_CARDSTATE_ACK,
    CMND_CM_MIBCARD_INIT_FINISH,
    CMND_CM_CMI_STARTED,
    CMND_CM_MIBCARD_ACT_FINISH,
    CMND_CM_RESTORE_CARD,
    CMND_CM_ADD_CARD,
    CMND_CM_DEL_CARD,
    CMND_CM_CARD_DOWN,
    CMND_CM_CARD_UP,
    CMND_CM_DELCARD,
    CMND_CM_RESETCARD,
    CMND_CM_REBOOT,
    CMND_CM_HALT,
    CMND_CM_HALT_ACK,
    CMND_CM_LATCH_STATE,
    CMND_CM_ACTIVECCDOWN,
    CMND_CM_ACTIVECCUP,
    CMND_CM_REPORT_CARD_DEFECT,
    CMND_CM_REPORT_CARD_PRESENCE,
    CMND_CM_DISABLECARD,
    CMND_CM_ENABLECARD,
    CMND_CM_TASK_READY,
    CMND_CM_TASK_READY_ACK,
    CMND_CM_SLOT_UPDATE,
    CMND_CM_CC_CAPABILITY_QUERY,   
    CMND_CM_CC_CAPABILITY,         
    CMND_CM_CC_CAPABILITY_ACK,     
    CMND_CM_CPC_LOCC_CAPACITY,      
    CMND_CM_CPC_HOCC_PHYSTATE,     
    CMND_CM_CPC_HOCC_PHYSTATE_ACK,  
    CMND_CM_CC_QUERY_ACTIVECC,         
    CMND_CM_APP_QUERY_CARDSTATE,        
    CMND_CM_APP_QUERY_CARDSTATE_RESPD,        
    CMND_CM_CARD_REBOOT,
    CMND_CM_RESETCPU,                         
    CMND_CM_SET_TEMPTH,          
    CMND_CM_QUERY_LATCHSTATE,
    CMND_CM_POLLING_STATE,          
    CMND_CM_MIBCARD_CHECKSTATE_FINISHED,    
    CMND_CM_UNITFAIL,                     
    CMND_CM_PCM_NOVALIDMIBCC,         
    CMND_CM_SWFPGAVS,              
    CMND_CM_VCXOOCXOFAIL,         
    CMND_CM_CHECKMIBACTFINISH,     
    CMND_CM_SETSILED,     
    CMND_CM_SHUTDOWNLAMP,     
    CMND_CM_SETLAMP,     
    CMND_CM_STOPSLAVECCCHECK,     
    CMND_CM_HOCC_POWER_FAIL,       
    CMND_CM_QUERY_TEMP_THRESHOLD,  
    CMND_CM_TEMP_THRESHOLD_ACK,    
    CMND_CM_SET_CARDTYPE,     
    CMND_CM_SET_HOCC_AIS,
    CMND_CM_SET_PORT_STATUS,
    CMND_CM_ACTIVE_HOCC_COLD_REBOOT,
    CMND_CM_ALL_LCCARD_ACTIVE,
    CMND_CM_FAN_PWR_INFO_QUERY,
    CMND_CM_FAN_PWR_INFO_RESP,
    CMND_CM_FAN_FAILED
};

typedef enum _CM_CARDBASESTATE_T
{
    CM_BASSTATE_NONE=0,          
    CM_BASSTATE_UP = 1,          
    CM_BASSTATE_DOWN = 2,        
    CM_BASSTATE_UNKNOWN = 3      
} CM_CARDBASESTATE_T;

typedef struct _CM_CARDINFO_T
{
    UINT32 slot;     
    UINT32 style;    
    UINT32 option;   
    BOOL   warm;     
    BOOL   cancfg;   
    BOOL   suspended;  
} CM_CARDINFO_T;

struct CM_INFO_CC_QURY_CPC_T
{
    UINT32 slot;
};

struct CM_RESET_CPU_T
{
    UINT32 Pslot;
};

struct CM_POLL_STATE_T
{
    UINT32 Pslot;
};


typedef struct _CM_ACTIVECC_DATA_T
{
    UINT32                ActiveCC;
} CM_ACTIVECC_DATA_T;

typedef struct _CM_HOCC_PHYSTATE_T
{
    SLOT_NUMBER_T slot;
    BOOL ispresent;
}CM_HOCC_PHYSTATE_T;

typedef struct _CM_CPC_CCPHYSTATE_T
{
    command_tt            head;   
    CM_HOCC_PHYSTATE_T    info;   
} CM_CPC_CCPHYSTATE_T;

typedef struct _CM_CCS_CPC_READY_T
{
    command_tt            head;   
    UINT32                slot;   
} CM_CCS_CPC_READY_T;


typedef struct _CM_CARDSTATE_DATA_T
{
    CM_CARDINFO_T         card;
    UINT32                cmd;
} CM_CARDSTATE_DATA_T;

typedef struct _CM_CARDSTATE_ACK_DATA_T
{
    CM_CARDINFO_T         card;
    UINT32                cmd;
    PB_ERROR_CODE_T       result;
} CM_CARDSTATE_ACK_DATA_T;

typedef struct _CM_MIBCARD_DATA_T
{
    CM_CARDINFO_T         card[SLOT_NUMBER_MAX ];
} CM_MIBCARD_DATA_T;


typedef struct _CM_CARDSTATE_ARRY_T
{
    CM_CARDSTATE_DATA_T         state[SLOT_NUMBER_MAX ];
} CM_CARDSTATE_ARRY_T;

typedef struct _CM_REPORT_CARD_DEFECT_DATA_T
{
    UINT32 slot;
    BOOL ur;
    BOOL miscard;
    BOOL uf;
    BOOL initing;
    BOOL misslot;
} CM_REPORT_CARD_DEFECT_DATA_T;

typedef struct _CM_HALT_DATA_T
{
    BOOL       warm;
    UINT32      type;    
} CM_HALT_DATA_T;

typedef enum
{
    TMS_NO_FAULTY_ALARM = 0,
    TMS_VCXO_FAIL,
    TMS_OCXO_FAIL,
    TMS_HOCC_POWER_FAIL
}CM_VCXOOCXO_FAULTY_TYPE;
 
typedef struct
{
    UINT8                   slot;
    CM_VCXOOCXO_FAULTY_TYPE    faultyType;
    BOOL                       vcxofail;
}CM_RECV_VOCXO_MSG_T;


 
 
typedef struct
{
    command_tt              head;
    UINT8                   slot;
    CM_VCXOOCXO_FAULTY_TYPE    faultyType;
    BOOL                       vcxofail;
} CM_VCXOOCXO_FAIL_IND_MSG_T;

typedef struct
{
    UINT8           high;
    UINT8           low;
    UINT8           slot;
}CM_TEMP_THRESHOLD_T;


 
typedef struct
{
    command_tt              head;
    UINT8           high;
    UINT8           low;
    UINT8           slot;
} CM_TEMP_THRESHOLD_MSG_T;

 
typedef struct _CM_ACTIVECC_T
{
    command_tt            head;     
    UINT32                ActiveCC; 
                                    
} CM_ACTIVECC_T;

 
typedef struct _CM_CARDSTATE_T
{
    command_tt            head;   

    CM_CARDINFO_T         card;   
    UINT32                cmd;    
                                  
                                  
                                  
                                  
                                  
} CM_CARDSTATE_T;

 
typedef struct _CM_CARDSTATE_ACK_T
{
    command_tt            head;  
  
    CM_CARDINFO_T         card;  
    UINT32                cmd;   
    PB_ERROR_CODE_T       result; 
} CM_CARDSTATE_ACK_T;

 
typedef struct _CM_APP_QUERY_CARDSTATE_T
{
    command_tt            head;
} CM_APP_QUERY_CARDSTATE_T;

 
typedef struct _CM_APP_QUERY_CARDSTATE_REPD_T
{
    command_tt            head;
    CM_CARDSTATE_ARRY_T         state;  
}CM_APP_QUERY_CARDSTATE_REPD_T;

 
typedef struct _CM_MIBCARD_T
{
    command_tt            head;
    CM_CARDINFO_T         card[SLOT_NUMBER_MAX ];  
} CM_MIBCARD_T;

 
typedef struct _CM_MIBCARD_ACK_T
{
    command_tt            head;
} CM_MIBCARD_ACK_T;

 
typedef struct _CM_APP_QUERY_MIBCARD_T
{
    command_tt            head;
} CM_APP_QUERY_MIBCARD_T;


typedef struct _CM_REPORT_CARD_DEFECT_T
{
    command_tt head;

    UINT32 slot;
    BOOL ur;
    BOOL miscard;
    BOOL uf;
    BOOL initing;
    BOOL misslot;
} CM_REPORT_CARD_DEFECT_T;



typedef struct _CM_HALT_T
{
    command_tt head;
    BOOL       warm;
    UINT32      type;  
} CM_HALT_T;

typedef struct _CM_LATCH_T
{
    command_tt          head;
    UINT32                 slot;
    UINT32                 state;
} CM_LATCH_T;

typedef enum{
    NE_HOCC_CAPABILITY_UNKNOW = 0,      
    NE_HOCC_CAPABILITY_340G   = 1,      
    NE_HOCC_CAPABILITY_680G  = 2       
}eNeHOCCCapability;

typedef enum{
    NE_LOCC_CAPABILITY_UNKNOW = 0,      
    NE_LOCC_CAPABILITY_NONE     =1,     
    NE_LOCC_CAPABILITY_20G    = 2,      
    NE_LOCC_CAPABILITY_40G  = 3       
}eNeLOCCCapability;

typedef struct _CM_CC_CAPABILITY_T
{
    command_tt            head;
    eNeHOCCCapability       eHOCCCapability;
    eNeLOCCCapability       eLOCCCapability;
} CM_CC_CAPABILITY_T;

typedef struct _CM_REBOOTCARD_T
{
    command_tt          head;
    UINT32 slot;
    BOOL   warm;
} CM_REBOOTCARD_T;






# 20 "../../../appl/CTD/inc/ctd_tm.h" 2

# 1 "../../../mib/inc/MIB.h" 1







 








 

 
 
 
 
 
 
 
 
 
 
 



# 21 "../../../appl/CTD/inc/ctd_tm.h" 2

# 1 "../../../mib/inc/MIB_cookie.h" 1
 





# 1 "../../../mib/inc/mib/MIB_base.h" 1



 




 



 
extern MIBDid_t iso_Did;

   


 



 
extern MIBDid_t org_Did;

   


 



 
extern MIBDid_t dod_Did;

   


 



 
extern MIBDid_t internet_Did;

   


 



 
extern MIBDid_t mgmt_Did;

   


 



 
extern MIBDid_t private_Did;

   


 



 
extern MIBDid_t snmpV2_Did;

   


 



 
extern MIBDid_t snmpModules_Did;

   


 



 
extern MIBDid_t enterprises_Did;

   



# 7 "../../../mib/inc/MIB_cookie.h" 2


# 1 "../../../mib/inc/mib/MIB_rfc1213.h" 1



 




 



 
extern MIBDid_t mib_2_Did;

   


 



 
extern MIBDid_t at_Did;

   


 



 
extern MIBDid_t ip_Did;

   
typedef enum {
	ipForwarding_forwarding=1,
	ipForwarding_not_forwarding=2
} ipForwarding_Tval;

typedef struct
{
	ipForwarding_Tval ipForwarding;
	int		ipDefaultTTL;
	unsigned int	ipInReceives;
	unsigned int	ipInHdrErrors;
	unsigned int	ipInAddrErrors;
	unsigned int	ipForwDatagrams;
	unsigned int	ipInUnknownProtos;
	unsigned int	ipInDiscards;
	unsigned int	ipInDelivers;
	unsigned int	ipOutRequests;
	unsigned int	ipOutDiscards;
	unsigned int	ipOutNoRoutes;
	int		ipReasmTimeout;
	unsigned int	ipReasmReqds;
	unsigned int	ipReasmOKs;
	unsigned int	ipReasmFails;
	unsigned int	ipFragOKs;
	unsigned int	ipFragFails;
	unsigned int	ipFragCreates;
	unsigned int	ipRoutingDiscards;
}MIBip_t;

























 



 
extern MIBDid_t icmp_Did;

   
typedef struct
{
	unsigned int	icmpInMsgs;
	unsigned int	icmpInErrors;
	unsigned int	icmpInDestUnreachs;
	unsigned int	icmpInTimeExcds;
	unsigned int	icmpInParmProbs;
	unsigned int	icmpInSrcQuenchs;
	unsigned int	icmpInRedirects;
	unsigned int	icmpInEchos;
	unsigned int	icmpInEchoReps;
	unsigned int	icmpInTimestamps;
	unsigned int	icmpInTimestampReps;
	unsigned int	icmpInAddrMasks;
	unsigned int	icmpInAddrMaskReps;
	unsigned int	icmpOutMsgs;
	unsigned int	icmpOutErrors;
	unsigned int	icmpOutDestUnreachs;
	unsigned int	icmpOutTimeExcds;
	unsigned int	icmpOutParmProbs;
	unsigned int	icmpOutSrcQuenchs;
	unsigned int	icmpOutRedirects;
	unsigned int	icmpOutEchos;
	unsigned int	icmpOutEchoReps;
	unsigned int	icmpOutTimestamps;
	unsigned int	icmpOutTimestampReps;
	unsigned int	icmpOutAddrMasks;
	unsigned int	icmpOutAddrMaskReps;
}MIBicmp_t;































 



 
extern MIBDid_t tcp_Did;

   
typedef enum {
	tcpRtoAlgorithm_other=1,
	tcpRtoAlgorithm_constant=2,
	tcpRtoAlgorithm_rsre=3,
	tcpRtoAlgorithm_vanj=4
} tcpRtoAlgorithm_Tval;

typedef struct
{
	tcpRtoAlgorithm_Tval tcpRtoAlgorithm;
	int		tcpRtoMin;
	int		tcpRtoMax;
	int		tcpMaxConn;
	unsigned int	tcpActiveOpens;
	unsigned int	tcpPassiveOpens;
	unsigned int	tcpAttemptFails;
	unsigned int	tcpEstabResets;
	unsigned int	tcpCurrEstab;
	unsigned int	tcpInSegs;
	unsigned int	tcpOutSegs;
	unsigned int	tcpRetransSegs;
	unsigned int	tcpInErrs;
	unsigned int	tcpOutRsts;
}MIBtcp_t;



















 



 
extern MIBDid_t udp_Did;

   
typedef struct
{
	unsigned int	udpInDatagrams;
	unsigned int	udpNoPorts;
	unsigned int	udpInErrors;
	unsigned int	udpOutDatagrams;
}MIBudp_t;









 



 
extern MIBDid_t egp_Did;

   
typedef struct
{
	unsigned int	egpInMsgs;
	unsigned int	egpInErrors;
	unsigned int	egpOutMsgs;
	unsigned int	egpOutErrors;
	int		egpAs;
}MIBegp_t;










 



 
extern MIBDid_t transmission_Did;

   


 



 
 
extern MIBDid_t egpNeighTable_Did;

   
typedef enum {
	egpNeighState_idle=1,
	egpNeighState_acquisition=2,
	egpNeighState_down=3,
	egpNeighState_up=4,
	egpNeighState_cease=5
} egpNeighState_Tval;

typedef enum {
	egpNeighMode_active=1,
	egpNeighMode_passive=2
} egpNeighMode_Tval;

typedef enum {
	egpNeighEventTrigger_start=1,
	egpNeighEventTrigger_stop=2
} egpNeighEventTrigger_Tval;

typedef struct
{
	unsigned int	egpNeighAddr;                                    
	egpNeighState_Tval egpNeighState;
	int		egpNeighAs;
	unsigned int	egpNeighInMsgs;
	unsigned int	egpNeighInErrs;
	unsigned int	egpNeighOutMsgs;
	unsigned int	egpNeighOutErrs;
	unsigned int	egpNeighInErrMsgs;
	unsigned int	egpNeighOutErrMsgs;
	unsigned int	egpNeighStateUps;
	unsigned int	egpNeighStateDowns;
	int		egpNeighIntervalHello;
	int		egpNeighIntervalPoll;
	egpNeighMode_Tval egpNeighMode;
	egpNeighEventTrigger_Tval egpNeighEventTrigger;
}MIBegpNeighTable_t;






















 



 
 
extern MIBDid_t udpTable_Did;

   
typedef struct
{
	unsigned int	udpLocalAddress;                     
	int		udpLocalPort;                                
}MIBudpTable_t;









 



 
 
extern MIBDid_t tcpConnTable_Did;

   
typedef enum {
	tcpConnState_closed=1,
	tcpConnState_listen=2,
	tcpConnState_synSent=3,
	tcpConnState_synReceived=4,
	tcpConnState_established=5,
	tcpConnState_finWait1=6,
	tcpConnState_finWait2=7,
	tcpConnState_closeWait=8,
	tcpConnState_lastAck=9,
	tcpConnState_closing=10,
	tcpConnState_timeWait=11,
	tcpConnState_deleteTCB=12
} tcpConnState_Tval;

typedef struct
{
	unsigned int	tcpConnLocalAddress;                         
	int		tcpConnLocalPort;                                    
	unsigned int	tcpConnRemAddress;                           
	int		tcpConnRemPort;                                      
	tcpConnState_Tval tcpConnState;
}MIBtcpConnTable_t;












 



 
 
extern MIBDid_t ipAddrTable_Did;

   
typedef struct
{
	unsigned int	ipAdEntAddr;                                 
	int		ipAdEntIfIndex;
	unsigned int	ipAdEntNetMask;
	unsigned int	ipAdEntBcastAddr;
	int		ipAdEntReasmMaxSize;
}MIBipAddrTable_t;












 



 
 
extern MIBDid_t ipRouteTable_Did;

   
typedef enum {
	ipRouteType_other=1,
	ipRouteType_invalid=2,
	ipRouteType_direct=3,
	ipRouteType_indirect=4
} ipRouteType_Tval;

typedef enum {
	ipRouteProto_other=1,
	ipRouteProto_local=2,
	ipRouteProto_netmgmt=3,
	ipRouteProto_icmp=4,
	ipRouteProto_egp=5,
	ipRouteProto_ggp=6,
	ipRouteProto_hello=7,
	ipRouteProto_rip=8,
	ipRouteProto_is_is=9,
	ipRouteProto_es_is=10,
	ipRouteProto_ciscoIgrp=11,
	ipRouteProto_bbnSpfIgp=12,
	ipRouteProto_ospf=13,
	ipRouteProto_bgp=14
} ipRouteProto_Tval;

typedef struct
{
	unsigned int	ipRouteDest;                       
	int		ipRouteIfIndex;
	int		ipRouteMetric1;
	int		ipRouteMetric2;
	int		ipRouteMetric3;
	int		ipRouteMetric4;
	unsigned int	ipRouteNextHop;
	ipRouteType_Tval ipRouteType;
	ipRouteProto_Tval ipRouteProto;
	int		ipRouteAge;
	unsigned int	ipRouteMask;
	int		ipRouteMetric5;
	OIDValue	ipRouteInfo;
}MIBipRouteTable_t;




















 



 
 
extern MIBDid_t ipNetToMediaTable_Did;

   
typedef enum {
	ipNetToMediaType_other=1,
	ipNetToMediaType_invalid=2,
	ipNetToMediaType_dynamic=3,
	ipNetToMediaType_static=4
} ipNetToMediaType_Tval;

typedef struct
{
	int		ipNetToMediaIfIndex;                                         
	unsigned int	ipNetToMediaNetAddress;                              
	int		ipNetToMediaPhysAddress_len;
	char		ipNetToMediaPhysAddress[255];
	ipNetToMediaType_Tval ipNetToMediaType;
}MIBipNetToMediaTable_t;











 



 
 
extern MIBDid_t atTable_Did;

   
typedef struct
{
	int		atIfIndex;                               
	int		atNetAddress_len;
	char		atNetAddress[8];                        
	int		atPhysAddress_len;
	char		atPhysAddress[255];
}MIBatTable_t;











# 9 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_rfc1907.h" 1



 




 



 
extern MIBDid_t snmpMIB_Did;

   


 



 
extern MIBDid_t snmpMIBObjects_Did;

   


 



 
extern MIBDid_t snmpMIBConformance_Did;

   


 



 
extern MIBDid_t snmpMIBCompliances_Did;

   


 



 
extern MIBDid_t snmpMIBGroups_Did;

   


 



 
extern MIBDid_t snmpTrap_Did;

   
typedef struct
{
	OIDValue	snmpTrapOID;
	OIDValue	snmpTrapEnterprise;
}MIBsnmpTrap_t;







 



 
extern MIBDid_t snmpTraps_Did;

   


 



 
extern MIBDid_t snmpSet_Did;

   
typedef struct
{
	unsigned int	snmpSetSerialNo;
}MIBsnmpSet_t;






 



 
extern MIBDid_t system_Did;

   
typedef struct
{
	char		sysDescr[255];
	OIDValue	sysObjectID;
	unsigned int	sysUpTime;
	char		sysContact[255];
	char		sysName[255];
	char		sysLocation[255];
	int		sysServices;
	unsigned int	sysORLastChange;
}MIBsystem_t;













 



 
extern MIBDid_t snmp_Did;

   
typedef enum {
	snmpEnableAuthenTraps_enabled=1,
	snmpEnableAuthenTraps_disabled=2
} snmpEnableAuthenTraps_Tval;

typedef struct
{
	unsigned int	snmpInPkts;
	unsigned int	snmpInBadVersions;
	unsigned int	snmpOutPkts;
	unsigned int	snmpInBadCommunityNames;
	unsigned int	snmpInBadCommunityUses;
	unsigned int	snmpInASNParseErrs;
	snmpEnableAuthenTraps_Tval snmpEnableAuthenTraps;
	unsigned int	snmpOutTraps;
	unsigned int	snmpOutGetResponses;
	unsigned int	snmpOutSetRequests;
	unsigned int	snmpOutGetNexts;
	unsigned int	snmpOutGetRequests;
	unsigned int	snmpOutGenErrs;
	unsigned int	snmpOutBadValues;
	unsigned int	snmpOutNoSuchNames;
	unsigned int	snmpOutTooBigs;
	unsigned int	snmpInTraps;
	unsigned int	snmpInGetResponses;
	unsigned int	snmpInSetRequests;
	unsigned int	snmpInGetNexts;
	unsigned int	snmpInGetRequests;
	unsigned int	snmpInTotalSetVars;
	unsigned int	snmpInTotalReqVars;
	unsigned int	snmpInGenErrs;
	unsigned int	snmpInReadOnlys;
	unsigned int	snmpInBadValues;
	unsigned int	snmpInNoSuchNames;
	unsigned int	snmpInTooBigs;
	unsigned int	snmpSilentDrops;
	unsigned int	snmpProxyDrops;
}MIBsnmp_t;



































 



 
 
extern MIBDid_t sysORTable_Did;

   
typedef struct
{
	int		sysORIndex;                          
	OIDValue	sysORID;
	char		sysORDescr[255];
	unsigned int	sysORUpTime;
}MIBsysORTable_t;












# 10 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_IANAifType.h" 1



 




 



 
extern MIBDid_t ianaifType_Did;

   



# 11 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_rfc2233.h" 1



 




 



 
extern MIBDid_t interfaces_Did;

   
typedef struct
{
	int		ifNumber;
}MIBinterfaces_t;






 



 
extern MIBDid_t ifMIB_Did;

   


 



 
extern MIBDid_t ifMIBObjects_Did;

   
typedef struct
{
	unsigned int	ifTableLastChange;
	unsigned int	ifStackLastChange;
}MIBifMIBObjects_t;







 



 
extern MIBDid_t ifConformance_Did;

   


 



 
extern MIBDid_t ifGroups_Did;

   


 



 
extern MIBDid_t ifCompliances_Did;

   


 



 
 
extern MIBDid_t ifXTable_Did;

   
typedef enum {
	ifLinkUpDownTrapEnable_enabled=1,
	ifLinkUpDownTrapEnable_disabled=2
} ifLinkUpDownTrapEnable_Tval;

typedef struct
{
	int		ifIndex;                                                           
	char		ifName[255];
	unsigned int	ifInMulticastPkts;
	unsigned int	ifInBroadcastPkts;
	unsigned int	ifOutMulticastPkts;
	unsigned int	ifOutBroadcastPkts;
	unsigned int	ifHCInOctets[2];
	unsigned int	ifHCInUcastPkts[2];
	unsigned int	ifHCInMulticastPkts[2];
	unsigned int	ifHCInBroadcastPkts[2];
	unsigned int	ifHCOutOctets[2];
	unsigned int	ifHCOutUcastPkts[2];
	unsigned int	ifHCOutMulticastPkts[2];
	unsigned int	ifHCOutBroadcastPkts[2];
	ifLinkUpDownTrapEnable_Tval ifLinkUpDownTrapEnable;
	unsigned int	ifHighSpeed;
	TruthValue_Tval ifPromiscuousMode;
	TruthValue_Tval ifConnectorPresent;
	char		ifAlias[64];
	unsigned int	ifCounterDiscontinuityTime;
}MIBifXTable_t;



























 



 
 
extern MIBDid_t ifTestTable_Did;

   
typedef enum {
	ifTestStatus_notInUse=1,
	ifTestStatus_inUse=2
} ifTestStatus_Tval;

typedef enum {
	ifTestResult_none=1,
	ifTestResult_success=2,
	ifTestResult_inProgress=3,
	ifTestResult_notSupported=4,
	ifTestResult_unAbleToRun=5,
	ifTestResult_aborted=6,
	ifTestResult_failed=7
} ifTestResult_Tval;

typedef struct
{
	int		ifIndex;                               
	unsigned int	ifTestId;
	ifTestStatus_Tval ifTestStatus;
	OIDValue	ifTestType;
	ifTestResult_Tval ifTestResult;
	OIDValue	ifTestCode;
	int		ifTestOwner_len;
	char		ifTestOwner[255];
}MIBifTestTable_t;














 



 
 
extern MIBDid_t ifRcvAddressTable_Did;

   
typedef enum {
	ifRcvAddressType_other=1,
	ifRcvAddressType_volatile=2,
	ifRcvAddressType_nonVolatile=3
} ifRcvAddressType_Tval;

typedef struct
{
	int		ifIndex;                                             
	int		ifRcvAddressAddress_len;
	char		ifRcvAddressAddress[255];                           
	RowStatus_Tval ifRcvAddressStatus;
	ifRcvAddressType_Tval ifRcvAddressType;
}MIBifRcvAddressTable_t;











 



 
 
extern MIBDid_t ifStackTable_Did;

   
typedef struct
{
	int		ifStackHigherLayer;                                
	int		ifStackLowerLayer;                                 
	RowStatus_Tval ifStackStatus;
}MIBifStackTable_t;










 



 
 
extern MIBDid_t ifTable_Did;

   
typedef enum {
	ifAdminStatus_up=1,
	ifAdminStatus_down=2,
	ifAdminStatus_testing=3
} ifAdminStatus_Tval;

typedef enum {
	ifOperStatus_up=1,
	ifOperStatus_down=2,
	ifOperStatus_testing=3,
	ifOperStatus_unknown=4,
	ifOperStatus_dormant=5,
	ifOperStatus_notPresent=6,
	ifOperStatus_lowerLayerDown=7
} ifOperStatus_Tval;

typedef struct
{
	int		ifIndex;                                         
	char		ifDescr[255];
	IANAifType_Tval ifType;
	int		ifMtu;
	unsigned int	ifSpeed;
	int		ifPhysAddress_len;
	char		ifPhysAddress[255];
	ifAdminStatus_Tval ifAdminStatus;
	ifOperStatus_Tval ifOperStatus;
	unsigned int	ifLastChange;
	unsigned int	ifInOctets;
	unsigned int	ifInUcastPkts;
	unsigned int	ifInNUcastPkts;
	unsigned int	ifInDiscards;
	unsigned int	ifInErrors;
	unsigned int	ifInUnknownProtos;
	unsigned int	ifOutOctets;
	unsigned int	ifOutUcastPkts;
	unsigned int	ifOutNUcastPkts;
	unsigned int	ifOutDiscards;
	unsigned int	ifOutErrors;
	unsigned int	ifOutQLen;
	OIDValue	ifSpecific;
}MIBifTable_t;






























# 12 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_rfc3411.h" 1



 




 



 
extern MIBDid_t snmpFrameworkMIB_Did;

   


 



 
extern MIBDid_t snmpFrameworkAdmin_Did;

   


 



 
extern MIBDid_t snmpFrameworkMIBObjects_Did;

   


 



 
extern MIBDid_t snmpFrameworkMIBConformance_Did;

   


 



 
extern MIBDid_t snmpFrameworkMIBCompliances_Did;

   


 



 
extern MIBDid_t snmpFrameworkMIBGroups_Did;

   


 



 
extern MIBDid_t snmpEngine_Did;

   
typedef struct
{
	int		snmpEngineID_len;
	char		snmpEngineID[32];
	int		snmpEngineBoots;
	int		snmpEngineTime;
	int		snmpEngineMaxMessageSize;
}MIBsnmpEngine_t;









 



 
extern MIBDid_t snmpAuthProtocols_Did;

   


 



 
extern MIBDid_t snmpPrivProtocols_Did;

   



# 13 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_rfc3412.h" 1



 




 



 
extern MIBDid_t snmpMPDMIB_Did;

   


 



 
extern MIBDid_t snmpMPDAdmin_Did;

   


 



 
extern MIBDid_t snmpMPDMIBObjects_Did;

   


 



 
extern MIBDid_t snmpMPDMIBConformance_Did;

   


 



 
extern MIBDid_t snmpMPDMIBCompliances_Did;

   


 



 
extern MIBDid_t snmpMPDMIBGroups_Did;

   


 



 
extern MIBDid_t snmpMPDStats_Did;

   
typedef struct
{
	unsigned int	snmpUnknownSecurityModels;
	unsigned int	snmpInvalidMsgs;
	unsigned int	snmpUnknownPDUHandlers;
}MIBsnmpMPDStats_t;









# 14 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_rfc3413t.h" 1



 




 



 
extern MIBDid_t snmpTargetMIB_Did;

   


 



 
extern MIBDid_t snmpTargetObjects_Did;

   
typedef struct
{
	unsigned int	snmpTargetSpinLock;
	unsigned int	snmpUnavailableContexts;
	unsigned int	snmpUnknownContexts;
}MIBsnmpTargetObjects_t;








 



 
extern MIBDid_t snmpTargetConformance_Did;

   


 



 
extern MIBDid_t snmpTargetCompliances_Did;

   


 



 
extern MIBDid_t snmpTargetGroups_Did;

   


 



 
 
extern MIBDid_t snmpTargetAddrTable_Did;

   
typedef struct
{
	char		snmpTargetAddrName[32];                                         
	OIDValue	snmpTargetAddrTDomain;
	int		snmpTargetAddrTAddress_len;
	char		snmpTargetAddrTAddress[255];
	int		snmpTargetAddrTimeout;
	int		snmpTargetAddrRetryCount;
	int		snmpTargetAddrTagList_len;
	char		snmpTargetAddrTagList[255];
	char		snmpTargetAddrParams[32];
	StorageType_Tval snmpTargetAddrStorageType;
	RowStatus_Tval snmpTargetAddrRowStatus;
}MIBsnmpTargetAddrTable_t;
















 



 
 
extern MIBDid_t snmpTargetParamsTable_Did;

   
typedef struct
{
	char		snmpTargetParamsName[32];                                               
	int		snmpTargetParamsMPModel;
	int		snmpTargetParamsSecurityModel;
	char		snmpTargetParamsSecurityName[255];
	SnmpSecurityLevel_Tval snmpTargetParamsSecurityLevel;
	StorageType_Tval snmpTargetParamsStorageType;
	RowStatus_Tval snmpTargetParamsRowStatus;
}MIBsnmpTargetParamsTable_t;















# 15 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_rfc3413n.h" 1



 




 



 
extern MIBDid_t snmpNotificationMIB_Did;

   


 



 
extern MIBDid_t snmpNotifyObjects_Did;

   


 



 
extern MIBDid_t snmpNotifyConformance_Did;

   


 



 
extern MIBDid_t snmpNotifyCompliances_Did;

   


 



 
extern MIBDid_t snmpNotifyGroups_Did;

   


 



 
 
extern MIBDid_t snmpNotifyTable_Did;

   
typedef enum {
	snmpNotifyType_trap=1,
	snmpNotifyType_inform=2
} snmpNotifyType_Tval;

typedef struct
{
	char		snmpNotifyName[32];                                     
	int		snmpNotifyTag_len;
	char		snmpNotifyTag[255];
	snmpNotifyType_Tval snmpNotifyType;
	StorageType_Tval snmpNotifyStorageType;
	RowStatus_Tval snmpNotifyRowStatus;
}MIBsnmpNotifyTable_t;












 



 
 
extern MIBDid_t snmpNotifyFilterProfileTable_Did;

   
typedef struct
{
	char		snmpTargetParamsName[32];                                                     
	char		snmpNotifyFilterProfileName[32];
	StorageType_Tval snmpNotifyFilterProfileStorType;
	RowStatus_Tval snmpNotifyFilterProfileRowStatus;
}MIBsnmpNotifyFilterProfileTable_t;











 



 
 
extern MIBDid_t snmpNotifyFilterTable_Did;

   
typedef enum {
	snmpNotifyFilterType_included=1,
	snmpNotifyFilterType_excluded=2
} snmpNotifyFilterType_Tval;

typedef struct
{
	char		snmpNotifyFilterProfileName[32];                                    
	OIDValue	snmpNotifyFilterSubtree;                                         
	int		snmpNotifyFilterMask_len;
	char		snmpNotifyFilterMask[16];
	snmpNotifyFilterType_Tval snmpNotifyFilterType;
	StorageType_Tval snmpNotifyFilterStorageType;
	RowStatus_Tval snmpNotifyFilterRowStatus;
}MIBsnmpNotifyFilterTable_t;














# 16 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_rfc3414.h" 1



 




 



 
extern MIBDid_t snmpUsmMIB_Did;

   


 



 
extern MIBDid_t usmMIBObjects_Did;

   


 



 
extern MIBDid_t usmMIBConformance_Did;

   


 



 
extern MIBDid_t usmMIBCompliances_Did;

   


 



 
extern MIBDid_t usmMIBGroups_Did;

   


 



 
extern MIBDid_t usmStats_Did;

   
typedef struct
{
	unsigned int	usmStatsUnsupportedSecLevels;
	unsigned int	usmStatsNotInTimeWindows;
	unsigned int	usmStatsUnknownUserNames;
	unsigned int	usmStatsUnknownEngineIDs;
	unsigned int	usmStatsWrongDigests;
	unsigned int	usmStatsDecryptionErrors;
}MIBusmStats_t;











 



 
extern MIBDid_t usmUser_Did;

   
typedef struct
{
	unsigned int	usmUserSpinLock;
}MIBusmUser_t;






 



 
 
extern MIBDid_t usmUserTable_Did;

   
typedef struct
{
	int		usmUserEngineID_len;
	char		usmUserEngineID[32];                                        
	char		usmUserName[32];                                            
	char		usmUserSecurityName[255];
	OIDValue	usmUserCloneFrom;
	OIDValue	usmUserAuthProtocol;
	int		usmUserAuthKeyChange_len;
	char		usmUserAuthKeyChange[255];
	int		usmUserOwnAuthKeyChange_len;
	char		usmUserOwnAuthKeyChange[255];
	OIDValue	usmUserPrivProtocol;
	int		usmUserPrivKeyChange_len;
	char		usmUserPrivKeyChange[255];
	int		usmUserOwnPrivKeyChange_len;
	char		usmUserOwnPrivKeyChange[255];
	int		usmUserPublic_len;
	char		usmUserPublic[32];
	StorageType_Tval usmUserStorageType;
	RowStatus_Tval usmUserStatus;
}MIBusmUserTable_t;




















 



 
extern MIBDid_t usmNoPrivProtocol_Did;

   


 



 
extern MIBDid_t usmDESPrivProtocol_Did;

   


 



 
extern MIBDid_t usmNoAuthProtocol_Did;

   


 



 
extern MIBDid_t usmHMACMD5AuthProtocol_Did;

   


 



 
extern MIBDid_t usmHMACSHAAuthProtocol_Did;

   



# 17 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_rfc3415.h" 1



 




 



 
extern MIBDid_t snmpVacmMIB_Did;

   


 



 
extern MIBDid_t vacmMIBObjects_Did;

   


 



 
extern MIBDid_t vacmMIBConformance_Did;

   


 



 
extern MIBDid_t vacmMIBCompliances_Did;

   


 



 
extern MIBDid_t vacmMIBGroups_Did;

   


 



 
 
extern MIBDid_t vacmContextTable_Did;

   
typedef struct
{
	char		vacmContextName[32];                        
}MIBvacmContextTable_t;








 



 
 
extern MIBDid_t vacmSecurityToGroupTable_Did;

   
typedef struct
{
	int		vacmSecurityModel;                                                         
	char		vacmSecurityName[32];                                                     
	char		vacmGroupName[32];
	StorageType_Tval vacmSecurityToGroupStorageType;
	RowStatus_Tval vacmSecurityToGroupStatus;
}MIBvacmSecurityToGroupTable_t;












 



 
 
extern MIBDid_t vacmAccessTable_Did;

   
typedef enum {
	vacmAccessContextMatch_exact=1,
	vacmAccessContextMatch_prefix=2
} vacmAccessContextMatch_Tval;

typedef struct
{
	char		vacmGroupName[32];                                            
	char		vacmAccessContextPrefix[32];                                  
	int		vacmAccessSecurityModel;                                       
	SnmpSecurityLevel_Tval vacmAccessSecurityLevel;                     
	vacmAccessContextMatch_Tval vacmAccessContextMatch;
	char		vacmAccessReadViewName[32];
	char		vacmAccessWriteViewName[32];
	char		vacmAccessNotifyViewName[32];
	StorageType_Tval vacmAccessStorageType;
	RowStatus_Tval vacmAccessStatus;
}MIBvacmAccessTable_t;

















 



 
extern MIBDid_t vacmMIBViews_Did;

   
typedef struct
{
	unsigned int	vacmViewSpinLock;
}MIBvacmMIBViews_t;






 



 
 
extern MIBDid_t vacmViewTreeFamilyTable_Did;

   
typedef enum {
	vacmViewTreeFamilyType_included=1,
	vacmViewTreeFamilyType_excluded=2
} vacmViewTreeFamilyType_Tval;

typedef struct
{
	char		vacmViewTreeFamilyViewName[32];                                         
	OIDValue	vacmViewTreeFamilySubtree;                                           
	int		vacmViewTreeFamilyMask_len;
	char		vacmViewTreeFamilyMask[16];
	vacmViewTreeFamilyType_Tval vacmViewTreeFamilyType;
	StorageType_Tval vacmViewTreeFamilyStorageType;
	RowStatus_Tval vacmViewTreeFamilyStatus;
}MIBvacmViewTreeFamilyTable_t;














# 18 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_rfc3584.h" 1



 




 



 
extern MIBDid_t snmpCommunityMIB_Did;

   


 



 
extern MIBDid_t snmpCommunityMIBObjects_Did;

   
typedef struct
{
	unsigned int	snmpTrapAddress;
	int		snmpTrapCommunity_len;
	char		snmpTrapCommunity[255];
}MIBsnmpCommunityMIBObjects_t;







 



 
extern MIBDid_t snmpCommunityMIBConformance_Did;

   


 



 
extern MIBDid_t snmpCommunityMIBCompliances_Did;

   


 



 
extern MIBDid_t snmpCommunityMIBGroups_Did;

   


 



 
 
extern MIBDid_t snmpCommunityTable_Did;

   
typedef struct
{
	char		snmpCommunityIndex[32];                                               
	int		snmpCommunityName_len;
	char		snmpCommunityName[255];
	char		snmpCommunitySecurityName[32];
	int		snmpCommunityContextEngineID_len;
	char		snmpCommunityContextEngineID[32];
	char		snmpCommunityContextName[32];
	int		snmpCommunityTransportTag_len;
	char		snmpCommunityTransportTag[255];
	StorageType_Tval snmpCommunityStorageType;
	RowStatus_Tval snmpCommunityStatus;
}MIBsnmpCommunityTable_t;















 



 
 
extern MIBDid_t snmpTargetAddrExtTable_Did;

   
typedef struct
{
	char		snmpTargetAddrName[32];                             
	int		snmpTargetAddrTMask_len;
	char		snmpTargetAddrTMask[255];
	int		snmpTargetAddrMMS;
}MIBsnmpTargetAddrExtTable_t;











# 19 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_rfc2742.h" 1



 




 



 
extern MIBDid_t agentxMIB_Did;

   


 



 
extern MIBDid_t agentxObjects_Did;

   


 



 
extern MIBDid_t agentxConformance_Did;

   


 



 
extern MIBDid_t agentxMIBGroups_Did;

   


 



 
extern MIBDid_t agentxMIBCompliances_Did;

   


 



 
extern MIBDid_t agentxGeneral_Did;

   
typedef struct
{
	int		agentxDefaultTimeout;
	int		agentxMasterAgentXVer;
}MIBagentxGeneral_t;







 



 
extern MIBDid_t agentxConnection_Did;

   
typedef struct
{
	unsigned int	agentxConnTableLastChange;
}MIBagentxConnection_t;






 



 
extern MIBDid_t agentxSession_Did;

   
typedef struct
{
	unsigned int	agentxSessionTableLastChange;
}MIBagentxSession_t;






 



 
extern MIBDid_t agentxRegistration_Did;

   
typedef struct
{
	unsigned int	agentxRegistrationTableLastChange;
}MIBagentxRegistration_t;






 



 
 
extern MIBDid_t agentxRegistrationTable_Did;

   
typedef struct
{
	unsigned int	agentxConnIndex;                             
	unsigned int	agentxSessionIndex;                          
	unsigned int	agentxRegIndex;                              
	int		agentxRegContext_len;
	char		agentxRegContext[255];
	OIDValue	agentxRegStart;
	unsigned int	agentxRegRangeSubId;
	unsigned int	agentxRegUpperBound;
	unsigned int	agentxRegPriority;
	int		agentxRegTimeout;
	TruthValue_Tval agentxRegInstance;
}MIBagentxRegistrationTable_t;

















 



 
 
extern MIBDid_t agentxSessionTable_Did;

   
typedef enum {
	agentxSessionAdminStatus_up=1,
	agentxSessionAdminStatus_down=2
} agentxSessionAdminStatus_Tval;

typedef struct
{
	unsigned int	agentxConnIndex;                                       
	unsigned int	agentxSessionIndex;                                    
	OIDValue	agentxSessionObjectID;
	char		agentxSessionDescr[255];
	agentxSessionAdminStatus_Tval agentxSessionAdminStatus;
	unsigned int	agentxSessionOpenTime;
	int		agentxSessionAgentXVer;
	int		agentxSessionTimeout;
}MIBagentxSessionTable_t;















 



 
 
extern MIBDid_t agentxConnectionTable_Did;

   
typedef struct
{
	unsigned int	agentxConnIndex;                                           
	unsigned int	agentxConnOpenTime;
	OIDValue	agentxConnTransportDomain;
	int		agentxConnTransportAddress_len;
	char		agentxConnTransportAddress[255];
}MIBagentxConnectionTable_t;












# 20 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_photon_reg.h" 1



 




 



 
extern MIBDid_t enterpriseRoot_Did;

   


 



 
extern MIBDid_t enterpriseCommonRoot_Did;

   


 



 
extern MIBDid_t enterpriseReg_Did;

   


 



 
extern MIBDid_t enterpriseGeneric_Did;

   


 



 
extern MIBDid_t enterpriseProducts_Did;

   


 



 
extern MIBDid_t enterpriseCaps_Did;

   


 



 
extern MIBDid_t enterpriseReqs_Did;

   


 



 
extern MIBDid_t enterpriseExpr_Did;

   


 



 
extern MIBDid_t productSeries1Products_Did;

   


 



 
extern MIBDid_t productSeries1Common_Did;

   


 



 
extern MIBDid_t enterpriseModules_Did;

   


 



 
extern MIBDid_t enterpriseNetworkingProducts_Did;

   


 



 
extern MIBDid_t product1Reg_Did;

   


 



 
extern MIBDid_t enterpriseGlobalRegModule_Did;

   



# 21 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_photon_products.h" 1



 




 



 
extern MIBDid_t productSeries1CmnConf_Did;

   


 



 
extern MIBDid_t productSeries1CmnObjs_Did;

   


 



 
extern MIBDid_t productSeries1CmnEvents_Did;

   
typedef struct
{
	unsigned int	pbTrapTimeStamp;
}MIBproductSeries1CmnEvents_t;






 



 
extern MIBDid_t productSeries1CmnTCs_Did;

   


 



 
extern MIBDid_t productSeries1TrapGroup_Did;

   


 



 
extern MIBDid_t pbNode_Did;

   


 



 
extern MIBDid_t pbEvents_Did;

   


 



 
extern MIBDid_t pbCircuitBasedTransport_Did;

   


 



 
extern MIBDid_t pbTMS_Did;

   


 



 
extern MIBDid_t pbOspf_Did;

   


 



 
extern MIBDid_t pbMoln_Did;

   


 



 
extern MIBDid_t pbDcc_Did;

   


 



 
extern MIBDid_t pbEthernet_Did;

   


 



 
extern MIBDid_t pbDiagnostics_Did;

   


 



 
extern MIBDid_t pbOpticsMonitor_Did;

   


 



 
extern MIBDid_t pbPowerCircuitMonitor_Did;

   


 



 
extern MIBDid_t pbOr_Did;

   


 



 
extern MIBDid_t pbTrunk_Did;

   


 



 
extern MIBDid_t pbXcExtend_Did;

   


 



 
extern MIBDid_t pbRpr_Did;

   


 



 
extern MIBDid_t pbOAObjects_Did;

   


 



 
extern MIBDid_t pbOsi_Did;

   


 



 
extern MIBDid_t pbIpTunnel_Did;

   


 



 
extern MIBDid_t pbProtection_Did;

   


 



 
extern MIBDid_t pbSecurity_Did;

   


 



 
extern MIBDid_t pbDigitalTransport_Did;

   


 



 
extern MIBDid_t productSeries1MibModule_Did;

   



# 22 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_global_tc.h" 1



 




 



 
extern MIBDid_t enterpriseGlobalTCModule_Did;

   



# 23 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_oa.h" 1



 




 



 
 
extern MIBDid_t pbOATable_Did;

   
typedef enum {
	pbOAModuleConfiguration_singlePump=0,
	pbOAModuleConfiguration_dualPump=1
} pbOAModuleConfiguration_Tval;

typedef enum {
	pbOAModuleType_amplet=0,
	pbOAModuleType_boosterAmplifier=1,
	pbOAModuleType_lineAmplifier=2,
	pbOAModuleType_preAmplifier=3
} pbOAModuleType_Tval;

typedef enum {
	pbOAOperationMode_constantCurrent=0,
	pbOAOperationMode_constantGain=1,
	pbOAOperationMode_constantPower=2
} pbOAOperationMode_Tval;

typedef enum {
	pbOAAutoShutdown_disable=0,
	pbOAAutoShutdown_enable=1
} pbOAAutoShutdown_Tval;

typedef enum {
	pbOARealOperationMode_constantCurrent=0,
	pbOARealOperationMode_constantGain=1,
	pbOARealOperationMode_constantPower=2
} pbOARealOperationMode_Tval;

typedef enum {
	pbOARealAutoShutdown_disable=0,
	pbOARealAutoShutdown_enable=1
} pbOARealAutoShutdown_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbOASlotNo;                                            
	int		pbOAPortNo;                                                             
	pbOAModuleConfiguration_Tval pbOAModuleConfiguration;   
	pbOAModuleType_Tval pbOAModuleType;   
	char		pbOAHWVersion[10];   
	char		pbOASWVersion[10];   
	char		pbOASerialNumber[10];   
	int		pbOAInputPower;   
	int		pbOACurrentOutputPower;   
	int		pbOACurrentModuleGain;   
	int		pbOAPump1Power;   
	int		pbOAPump2Power;   
	int		pbOATemperature;   
	int		pbOAPump1DriveCurrent;   
	int		pbOAPump2DriveCurrent;   
	int		pbOAPowerSupplyVoltage;   
	pbOAOperationMode_Tval pbOAOperationMode;   
	int		pbOAExpectedOutputPower;   
	int		pbOAExpectedModuleGain;   
	int		pbOAInputThreshold;   
	int		pbOAOutputThreshold;   
	int		pbOAShutdownThreshold;   
	int		pbOALowTThreshold;   
	int		pbOAHighTThreshold;   
	pbOAAutoShutdown_Tval pbOAAutoShutdown;   
	int		pbOAMaxoutputpower;   
	int		pbOAMaxPump1Current;   
	int		pbOAMaxPump2Current;   
	pbOARealOperationMode_Tval pbOARealOperationMode;   
	int		pbOARealExpectedOutputPower;   
	int		pbOARealExpectedModuleGain;   
	int		pbOARealInputThreshold;   
	int		pbOARealOutputThreshold;   
	int		pbOARealShutdownThreshold;   
	int		pbOARealLowTThreshold;   
	int		pbOARealHighTThreshold;   
	pbOARealAutoShutdown_Tval pbOARealAutoShutdown;   
	int		pbOARealCoolPumpTemp;   
}MIBpbOATable_t;













































 



 
extern MIBDid_t enterpriseOAModule_Did;

   



# 24 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_node.h" 1



 




 



 
extern MIBDid_t addressGroup_Did;

   
typedef enum {
	pbEthernetEnable_disable=1,
	pbEthernetEnable_enable=2
} pbEthernetEnable_Tval;

typedef enum {
	pbMgmtPort1Status_down=0,
	pbMgmtPort1Status_up=1
} pbMgmtPort1Status_Tval;

typedef enum {
	pbMgmtPort2Status_down=0,
	pbMgmtPort2Status_up=1
} pbMgmtPort2Status_Tval;

typedef struct
{
	unsigned int	pbIpAddress;   
	unsigned int	pbIpMask;   
	unsigned int	pbGatewayAddress;   
	unsigned int	pbEthernetAddress;   
	unsigned int	pbEthernetMask;   
	int		pbSystemMacAddress_len;
	char		pbSystemMacAddress[6];   
	unsigned int	pbSlaveEthernetAddress;   
	unsigned int	pbEthernetAddressSecondary;   
	unsigned int	pbEthernetMaskSecondary;   
	pbEthernetEnable_Tval pbEthernetEnable;   
	unsigned int	pbGMPLSIPAddress;   
	unsigned int	pbGMPLSIPMask;
	pbMgmtPort1Status_Tval pbMgmtPort1Status;
	pbMgmtPort2Status_Tval pbMgmtPort2Status;
}MIBaddressGroup_t;



















 



 
extern MIBDid_t systemGroup_Did;

   
typedef struct
{
	PbNodeMode_Tval pbMode;   
	char		pbLabel[255];   
	char		pbLocation[255];   
	char		pbDescription[255];   
	char		pbContact[255];   
	char		pbSerialNumber[255];   
	char		pbRelease[20];   
	char		pbBootImageVersion[20];   
	char		pbBackplaneHwVersion[17];   
	char		pbBackplaneCode[64];   
}MIBsystemGroup_t;















 



 
extern MIBDid_t ledGroup_Did;

   
typedef struct
{
	TruthValue_Tval pbLedTest;   
}MIBledGroup_t;






 



 
 
extern MIBDid_t pbSlotTable_Did;

   
typedef enum {
	pbCardState_none=0,
	pbCardState_provisioning=1,
	pbCardState_initializing=2,
	pbCardState_faulty=3,
	pbCardState_operational=4,
	pbCardState_absent=5,
	pbCardState_disabled=6,
	pbCardState_mismatch=7,
	pbCardState_available=8,
	pbCardState_active=9,
	pbCardState_standby=10,
	pbCardState_mirroring=11,
	pbCardState_tracking=12,
	pbCardState_reset=13,
	pbCardState_max=14
} pbCardState_Tval;

typedef enum {
	pbCardDesiredState_auto=1,
	pbCardDesiredState_active=2,
	pbCardDesiredState_standby=3
} pbCardDesiredState_Tval;

typedef enum {
	pbSlotResetDuration_permanent=1,
	pbSlotResetDuration_transient=2
} pbSlotResetDuration_Tval;

typedef enum {
	pbDiagMode_none=1,
	pbDiagMode_once=2,
	pbDiagMode_always=3,
	pbDiagMode_ess=4,
	pbDiagMode_burnIn=5
} pbDiagMode_Tval;

typedef enum {
	pbForceResetSlot_none=1,
	pbForceResetSlot_warm=2,
	pbForceResetSlot_cold=3
} pbForceResetSlot_Tval;

typedef enum {
	pbLatchState_na=0,
	pbLatchState_off=1,
	pbLatchState_on=2
} pbLatchState_Tval;

typedef enum {
	slotMaxDCCNumber_ports_8=0,
	slotMaxDCCNumber_ports_16=1
} slotMaxDCCNumber_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                  
	PbSlotType_Tval pbSlotType;   
	PbBoardStyle_Tval pbBoardType;   
	PbBoardFunction_Tval pbBoardFunction;   
	pbCardState_Tval pbCardState;   
	pbCardDesiredState_Tval pbCardDesiredState;   
	char		pbSlotNumberDisplay[10];   
	TruthValue_Tval pbSlotReset;   
	int		pbSlotNoFailuresTotal;   
	TruthValue_Tval pbSlotResetNoFailures;   
	pbSlotResetDuration_Tval pbSlotResetDuration;   
	char		pbSlotName[255];   
	char		pbSlotDescr[255];   
	TruthValue_Tval pbSlotIfCardDumb;   
	TruthValue_Tval pbSlotCrashDumpAvail;   
	PbBoardStyle_Tval pbSlotLastBoardType;   
	TruthValue_Tval pbSlotBoardTypeDifference;   
	TruthValue_Tval pbSlotExplicitDeletion;   
	PbBoardStyle_Tval pbProvisionedCardType;   
	char		pbSlotCardSpecificInfo[255];   
	int		pbSlotTemperatureThreshold;   
	int		pbSlotTemperatureHysteresis;   
	int		pbSlotTemperatureCleanThreshold;   
	PbBoardFunction_Tval pbSlotLastBoardFunction;   
	pbDiagMode_Tval pbDiagMode;   
	TruthValue_Tval pbDiagActive;   
	PbSlotNumberType_Tval pbLogicalSlotNumber;   
	TruthValue_Tval pbSlotDisable;   
	pbForceResetSlot_Tval pbForceResetSlot;   
	PbLedColor_Tval pbSlotFailureLedColor;   
	PbLedColor_Tval pbSlotActiveLedColor;   
	char		pbBoardSerialNumber[255];   
	int		pbBoardOption;   
	char		pbCode[64];   
	pbLatchState_Tval pbLatchState;   
	TruthValue_Tval pbSlotAlarmDisable;   
	PbBoardFunction_Tval provisionedCardFunction;
	int		slotConfigOption;   
	char		cardSoftwareVersion[33];   
	char		cardBootVersion[21];   
	char		cardBootDatetime[255];   
	char		cardHardwareVersion[17];   
	char		cardFusionVersion[17];   
	char		cardFpgaVersion[255];   
	int		macAddress_len;
	char		macAddress[6];
	slotMaxDCCNumber_Tval slotMaxDCCNumber;
	int		slotMapValue;
}MIBpbSlotTable_t;






















































 



 
extern MIBDid_t pbMibSaveRestoreGroup_Did;

   
typedef enum {
	pbConfigType_ascii=1,
	pbConfigType_binary=2
} pbConfigType_Tval;

typedef struct
{
	PbOnOff_Tval pbAutoSaver;   
	int		pbAutoSaverFirstTimer;   
	int		pbAutoSaverLastTimer;   
	pbConfigType_Tval pbConfigType;   
	TruthValue_Tval pbSaveMib;   
	TruthValue_Tval pbClearMibSet;   
}MIBpbMibSaveRestoreGroup_t;











 



 
extern MIBDid_t pbInterfaceGroup_Did;

   
typedef struct
{
	TruthValue_Tval pbAutoLinkStarted;   
}MIBpbInterfaceGroup_t;






 



 
extern MIBDid_t pbUpgradeGroup_Did;

   
typedef struct
{
	char		pbUpgradeCurrRelease[30];   
	char		pbUpgradeBackupRelease[30];   
	unsigned int	pbUpgradeHostIP;   
	char		pbUpgradeHostUsername[20];   
	char		pbUpgradeHostPassword[20];   
	char		pbUpgradeHostFileName[255];   
	TypeUpgradeCmnd_Tval pbUpgradeCommand;   
	PbCmndState_Tval pbUpgradeCmndState;   
	char		pbUpgradeStateDescr[255];   
	TruthValue_Tval pbUpgradeLoadCancel;   
	TypeUpgradeCmnd_Tval nodeUpgradeLastCommand;   
	TypeSoftUpgFailedIndex_Tval nodeUpgradeFailedIndex;   
	int		nodeUpgradeSwitchPhySlot;   
}MIBpbUpgradeGroup_t;


















 



 
extern MIBDid_t pbConfigCmndGroup_Did;

   
typedef enum {
	nodeMibNmsOperateArea_none=0,
	nodeMibNmsOperateArea_flash=1,
	nodeMibNmsOperateArea_cf=2,
	nodeMibNmsOperateArea_ramdisc=3
} nodeMibNmsOperateArea_Tval;

typedef enum {
	pbBackupMIBState_invalid=0,
	pbBackupMIBState_valid=1
} pbBackupMIBState_Tval;

typedef struct
{
	TypeMibCmnd_Tval nodeMibCmnd;   
	PbCmndState_Tval nodeMibCmndState;   
	TruthValue_Tval nodeMibCmndCancel;   
	nodeMibNmsOperateArea_Tval nodeMibNmsOperateArea;   
	TypeMibUpDownLoadStatus_Tval nodeMibUpDownLoadStatus;   
	TypeMibCmnd_Tval nodeMibLastCommand;   
	TypeMibUpgFailedIndex_Tval nodeMibFailedIndex;   
	pbBackupMIBState_Tval pbBackupMIBState;   
	DateTime	pbMIBOperationDateTimeStamp;   
	TruthValue_Tval nodeMIBFilesNoCheck;   
}MIBpbConfigCmndGroup_t;















 



 
extern MIBDid_t pbStatsGroup_Did;

   
typedef struct
{
	int		pbNumberOfReboots;   
	DateTime	pbNodeDownDTS;   
}MIBpbStatsGroup_t;







 



 
extern MIBDid_t alarmGroup_Did;

   
typedef struct
{
	PbEnableDisable_Tval pbNodeAlarmEnabling;   
	TruthValue_Tval pbAISEnabled;   
	TruthValue_Tval pbRDIEnabled;   
	TruthValue_Tval pbAlarmSuppress;   
	TruthValue_Tval pbAlarmCutOff;   
	int		pbDataExcDegTpNum;   
	TruthValue_Tval pbSEFEnabled;
}MIBalarmGroup_t;












 



 
extern MIBDid_t pbNodeGroup_Did;

   
typedef enum {
	pbNodeType_hiT7065=11
} pbNodeType_Tval;

typedef struct
{
	pbNodeType_Tval pbNodeType;   
	char		pbNodeSoftwareVersion[255];   
	int		pbNodeIntSoftwareVersion;   
	int		pbDatabaseVersion;   
}MIBpbNodeGroup_t;









 



 
extern MIBDid_t nodeControlGroup_Did;

   
typedef struct
{
	TruthValue_Tval pbWarmRebootNode;   
	PbColdRebootNodeState_Tval pbColdRebootNode;   
	TruthValue_Tval pbResetNode;   
	TruthValue_Tval pbNodeShutDown;   
	TruthValue_Tval pbNodePowerOff;   
}MIBnodeControlGroup_t;










 



 
extern MIBDid_t ntpGroup_Did;

   
typedef struct
{
	TruthValue_Tval pbNTPEnabled;   
	unsigned int	pbNTPIpAddress;   
	int		pbNTPRequestPeriods;   
}MIBntpGroup_t;








 



 
extern MIBDid_t pbDateTimeStampGroup_Did;

   
typedef struct
{
	DateTime	pbDateTimeStampSet;   
	DateTime	pbDateTimeStampGet;   
	PbTimeZone_Tval pbTimeZone;   
}MIBpbDateTimeStampGroup_t;








 



 
extern MIBDid_t pbCFCardGroup_Did;

   
typedef enum {
	pbCFCardMIBValidity_cfcf=1,
	pbCFCardMIBValidity_cfmc=2,
	pbCFCardMIBValidity_mccf=3,
	pbCFCardMIBValidity_mcmc=4,
	pbCFCardMIBValidity_cffailed=5
} pbCFCardMIBValidity_Tval;

typedef struct
{
	PbCFCardState_Tval pbCFCardStatus;   
	pbCFCardMIBValidity_Tval pbCFCardMIBValidity;   
	PbCFCardState_Tval pbStandbyCFCardStatus;   
}MIBpbCFCardGroup_t;








 



 
extern MIBDid_t flashSpaceGroup_Did;

   
typedef struct
{
	int		boardFlashFreeSpace;   
	int		cfCardFreeSpace;   
}MIBflashSpaceGroup_t;







 



 
 
extern MIBDid_t pbExtensionShelfTable_Did;

   
typedef struct
{
	int		pbExtensionShelfID;                                                   
	unsigned int	pbMainShelfIP;   
	char		pbMainShelfReadString[255];   
	char		pbMainShelfWriteString[255];   
	int		pbMainShelfSlot;   
	int		pbMainShelfPort;   
	unsigned int	pbExtesionShelfIP;   
	char		pbExtesionShelfReadString[255];   
	char		pbExtesionShelfWriteString[255];   
	int		pbExtesionShelfSlot;   
	int		pbExtesionShelfPort;   
	char		pbExtesionShelfLabel[255];   
	char		pbExtesionShelfDesc[255];   
	RowStatus_Tval pbExtesionShelfRowStatus;   
}MIBpbExtensionShelfTable_t;





















 



 
extern MIBDid_t nodePropertyGroup_Did;

   
typedef enum {
	nodeHoccCapability_capability_340g=1,
	nodeHoccCapability_capability_680g=2
} nodeHoccCapability_Tval;

typedef enum {
	nodeLoccCapability_capability_none=1,
	nodeLoccCapability_capability_20g=2,
	nodeLoccCapability_capability_40g=3
} nodeLoccCapability_Tval;

typedef struct
{
	nodeHoccCapability_Tval nodeHoccCapability;   
	nodeLoccCapability_Tval nodeLoccCapability;   
}MIBnodePropertyGroup_t;







 



 
extern MIBDid_t eowFunctionGroup_Did;

   


 



 
extern MIBDid_t gmplsLicenseGroup_Did;

   
typedef enum {
	gmplsLicenseStatus_unknown=1,
	gmplsLicenseStatus_license_not_exist=2,
	gmplsLicenseStatus_license_invalid=3,
	gmplsLicenseStatus_ospf_only=4,
	gmplsLicenseStatus_gmpls=5
} gmplsLicenseStatus_Tval;

typedef struct
{
	gmplsLicenseStatus_Tval gmplsLicenseStatus;
	TruthValue_Tval gmplsLicenseSyncFlag;
}MIBgmplsLicenseGroup_t;







 



 
extern MIBDid_t e2romConfigGroup_Did;

   


 



 
extern MIBDid_t systemMonitorGroup_Did;

   


 



 
extern MIBDid_t cpuUtilEventGroup_Did;

   
typedef struct
{
	int		cpuUtilThreshold;   
	TruthValue_Tval cpuUtilEventEnable;   
}MIBcpuUtilEventGroup_t;







 



 
extern MIBDid_t memUtilEventGroup_Did;

   
typedef struct
{
	int		memUtilThreshold;   
	TruthValue_Tval memUtilEventEnable;   
}MIBmemUtilEventGroup_t;







 



 
extern MIBDid_t cfCardUtilEventGroup_Did;

   
typedef struct
{
	int		cfCardUtilThreshold;   
	TruthValue_Tval cfCardUtilEventEnable;   
}MIBcfCardUtilEventGroup_t;







 



 
extern MIBDid_t flashUtilEventGroup_Did;

   
typedef struct
{
	int		flashUtilThreshold;   
	TruthValue_Tval flashUtilEventEnable;   
}MIBflashUtilEventGroup_t;







 



 
 
extern MIBDid_t pbE2RomConfigTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;             
	int		pbE2RomPageNo;                             
	int		pbE2RomTlvData_len;
	char		pbE2RomTlvData[253];
}MIBpbE2RomConfigTable_t;










 



 
 
extern MIBDid_t eowOhccTable_Did;

   
typedef struct
{
	int		eowOhccDestination;                                   
	int		eowOhccSource;   
	TruthValue_Tval eowOhccRingManaged;   
}MIBeowOhccTable_t;










 



 
 
extern MIBDid_t siGlobalTable_Did;

   
typedef enum {
	siClipper_clip0=0,
	siClipper_clip4=4,
	siClipper_clip8=8,
	siClipper_clip16=16,
	siClipper_clip32=32
} siClipper_Tval;

typedef enum {
	siEnsThreshold_thresh0=0,
	siEnsThreshold_thresh1=1,
	siEnsThreshold_thresh3=3,
	siEnsThreshold_thresh5=5,
	siEnsThreshold_thresh7=7,
	siEnsThreshold_thresh255=255
} siEnsThreshold_Tval;

typedef enum {
	siErmAtt_is6dB=0,
	siErmAtt_is9dB=1
} siErmAtt_Tval;

typedef enum {
	siWorkingMode_normalMode=1,
	siWorkingMode_boxMode=2
} siWorkingMode_Tval;

typedef enum {
	siUpdateDsp_inactive=0,
	siUpdateDsp_update=1,
	siUpdateDsp_cancel=2
} siUpdateDsp_Tval;

typedef enum {
	siUpdateDspState_successfully=0,
	siUpdateDspState_failed=1,
	siUpdateDspState_updating=2,
	siUpdateDspState_unknown=3
} siUpdateDspState_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                 
	int		ifIdx;                                            
	char		siName[33];   
	char		siPhoneNum[4];   
	siClipper_Tval siClipper;   
	siEnsThreshold_Tval siEnsThreshold;   
	int		siErmThreshold;   
	siErmAtt_Tval siErmAtt;   
	int		siErmStep;   
	siWorkingMode_Tval siWorkingMode;   
	siUpdateDsp_Tval siUpdateDsp;   
	siUpdateDspState_Tval siUpdateDspState;   
}MIBsiGlobalTable_t;



















 



 
 
extern MIBDid_t siChannelManageTable_Did;

   
typedef enum {
	siChannelRingManageDelta_greaterThan12dB=0,
	siChannelRingManageDelta_lessThan6dB=1
} siChannelRingManageDelta_Tval;

typedef enum {
	siChannelRingManageSwitch_dBMode=0,
	siChannelRingManageSwitch_ooMode=1
} siChannelRingManageSwitch_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                   
	int		ifIdx;                                                           
	int		siChannelID;                                                        
	int		siChannelGroup;                                                     
	char		siChannelName[16];   
	TruthValue_Tval siChannelEnable;   
	TruthValue_Tval siChannelNoiseDetect;   
	TruthValue_Tval siChannelNoiseState;   
	TruthValue_Tval siChannelRingManage;   
	TruthValue_Tval siChannelRingManageState;   
	siChannelRingManageDelta_Tval siChannelRingManageDelta;   
	siChannelRingManageSwitch_Tval siChannelRingManageSwitch;   
	int		siChannelGroupSetting;   
}MIBsiChannelManageTable_t;




















 



 
extern MIBDid_t pbACOGroup_Did;

   
typedef struct
{
	PbOnOff_Tval pbAudibleACO;   
	PbOnOff_Tval pbVisualACO;   
	PbEnableDisable_Tval pbExtAudible;   
	PbEnableDisable_Tval pbExtVisual;   
}MIBpbACOGroup_t;









 



 
 
extern MIBDid_t pbIoaTable_Did;

   
typedef struct
{
	int		pbIoaIndexNo;                                       
	char		pbIoaLabel[32];   
	TruthValue_Tval pbIoaEnableStatus;   
	TruthValue_Tval pbIoaStatus;   
}MIBpbIoaTable_t;











 



 
extern MIBDid_t alarmRuleSwitchGroup_Did;

   
typedef struct
{
	PbEnableDisable_Tval lofMonitoringSwitch;
	PbEnableDisable_Tval lomMonitoringSwitch;
	PbEnableDisable_Tval lopMonitoringSwitch;
}MIBalarmRuleSwitchGroup_t;








 



 
 
extern MIBDid_t upgradeSoftwareInfoTable_Did;

   
typedef enum {
	upgradeSoftwareIndication_boardfirst=1,
	upgradeSoftwareIndication_boardsecond=2,
	upgradeSoftwareIndication_cffirst=3,
	upgradeSoftwareIndication_cfsecond=4
} upgradeSoftwareIndication_Tval;

typedef enum {
	upgradeSoftwareRole_active=1,
	upgradeSoftwareRole_backup=2
} upgradeSoftwareRole_Tval;

typedef struct
{
	PbSlotNumberType_Tval upgradeSoftwareSlotNumber;                         
	upgradeSoftwareIndication_Tval upgradeSoftwareIndication;                
	upgradeSoftwareRole_Tval upgradeSoftwareRole;   
	char		upgradeSoftwareName[33];   
	DateTime	upgradeSoftwareDatetime;   
}MIBupgradeSoftwareInfoTable_t;












 



 
 
extern MIBDid_t pbPhysicalAdjacencyTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbLocalSlotNumber;                                             
	int		pbLocalPortNumber;                                                              
	PbUpDown_Tval pbRemoteNodeMolnAdjacencyStatus;   
	unsigned int	pbRemoteNodeIPAddress;   
	int		pbRemoteSlotNumber;   
	int		pbRemotePortNumber;   
	PbUpDown_Tval pbLinkOperationalStatus;   
	char		pbInterfaceDescr[64];   
	PbLinkGeneralType_Tval pbLinkGeneralType;   
	RowStatus_Tval pbLinkRowStatus;   
}MIBpbPhysicalAdjacencyTable_t;

















 



 
 
extern MIBDid_t pbAutoLinkTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbLocalSlot;                       
	int		pbLocalPort;                                        
	unsigned int	pbRemoteIPAddress;   
	int		pbRemoteSlot;   
	int		pbRemotePort;   
}MIBpbAutoLinkTable_t;












 



 
 
extern MIBDid_t pbLedTable_Did;

   
typedef enum {
	pbLedColor_off=0,
	pbLedColor_green=1,
	pbLedColor_yellow=2,
	pbLedColor_red=3,
	pbLedColor_orange=4,
	pbLedColor_blinkingGreen=5,
	pbLedColor_blinkingYellow=6,
	pbLedColor_blinkingRed=7,
	pbLedColor_blinkingOrange=8
} pbLedColor_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbLedSlotNo;             
	int		pbLedIndexNo;                             
	char		pbLedLabel[32];   
	pbLedColor_Tval pbLedColor;   
}MIBpbLedTable_t;











 



 
extern MIBDid_t pbNodeModule_Did;

   



# 25 "../../../mib/inc/MIB_cookie.h" 2


# 1 "../../../mib/inc/mib/MIB_pb_rpr.h" 1



 




 



 
extern MIBDid_t rprGeneral_Did;

   


 



 
extern MIBDid_t rprProtocols_Did;

   


 



 
extern MIBDid_t rprService_Did;

   


 



 
extern MIBDid_t rprWan_Did;

   


 



 
extern MIBDid_t rprHwPassThrough_Did;

   


 



 
 
extern MIBDid_t rprHwpXcTable_Did;

   
typedef enum {
	rprHwpXcCircuitDirection_bidirection=1,
	rprHwpXcCircuitDirection_unidirection=2,
	rprHwpXcCircuitDirection_multicast=3
} rprHwpXcCircuitDirection_Tval;

typedef enum {
	rprHwpXcCcsRpmMsgType_client=1,
	rprHwpXcCcsRpmMsgType_ems=2,
	rprHwpXcCcsRpmMsgType_snm=3,
	rprHwpXcCcsRpmMsgType_gmpls=4,
	rprHwpXcCcsRpmMsgType_rpr=5
} rprHwpXcCcsRpmMsgType_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                   
	RprXcCapacity_Tval rprHwpXcCapacity;                                     
	PbSlotNumberType_Tval rprHwpXcSrcSlot;                                   
	int		rprHwpXcSrcPort;                                                    
	RprTpNumberType_Tval rprHwpXcSource;                                     
	PbSlotNumberType_Tval rprHwpXcDstSlot;                                   
	int		rprHwpXcDstPort;                                                    
	RprTpNumberType_Tval rprHwpXcDestination;                                
	PbPortType_Tval rprHwpXcSrcPortType;   
	PbPortType_Tval rprHwpXcDstPortType;   
	int		rprHwpXcConnectId;   
	rprHwpXcCircuitDirection_Tval rprHwpXcCircuitDirection;   
	rprHwpXcCcsRpmMsgType_Tval rprHwpXcCcsRpmMsgType;   
	RprXcMsgStatus_Tval rprHwpXcCcsRpmMsgState;   
	int		rprHwpXcCcsRpmMsgPriority;   
	PbBoardStyle_Tval rprHwpXcSrcSlotCard;   
	PbBoardStyle_Tval rprHwpXcDstSlotCard;   
	RprXcFailReason_Tval rprHwpXcReason;   
	RprXcRowStatus_Tval rprHwpXcRowStatus;   
	RprXcTableClass_Tval rprHwpXcTableClass;   
}MIBrprHwpXcTable_t;



























 



 
 
extern MIBDid_t pbRprWanConfigTable_Did;

   
typedef enum {
	pbRprWanPortXVType_vc4=0,
	pbRprWanPortXVType_vc12=1
} pbRprWanPortXVType_Tval;

typedef enum {
	pbRprWanGFPFCS_disable=0,
	pbRprWanGFPFCS_enable=1
} pbRprWanGFPFCS_Tval;

typedef enum {
	pbRprWanGFPFCSAutoDetect_disable=0,
	pbRprWanGFPFCSAutoDetect_enable=1
} pbRprWanGFPFCSAutoDetect_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                       
	int		pbPortNo;                                                            
	char		pbRprWanPortLabel[255];   
	int		pbRprWanPortCapacity;   
	TruthValue_Tval pbRprWanPortMonitored;   
	pbRprWanPortXVType_Tval pbRprWanPortXVType;   
	TruthValue_Tval pbRprWanLCASSupport;   
	TruthValue_Tval pbRprWanPortCapacityTrigger;   
	pbRprWanGFPFCS_Tval pbRprWanGFPFCS;   
	pbRprWanGFPFCSAutoDetect_Tval pbRprWanGFPFCSAutoDetect;   
	TruthValue_Tval pbRprWanFlowControl;   
}MIBpbRprWanConfigTable_t;


















 



 
extern MIBDid_t pbRprLCASBase_Did;

   


 



 
 
extern MIBDid_t pbRprSOLCASConfigTable_Did;

   
typedef enum {
	pbRprSOLCASActive_none=0,
	pbRprSOLCASActive_active=1,
	pbRprSOLCASActive_deactive=2
} pbRprSOLCASActive_Tval;

typedef enum {
	pbRprSOLCASState_fixed=0,
	pbRprSOLCASState_add=1,
	pbRprSOLCASState_normal=2,
	pbRprSOLCASState_eos=3,
	pbRprSOLCASState_idle=5,
	pbRprSOLCASState_dnu=15
} pbRprSOLCASState_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                     
	int		pbPortNo;                                          
	PbTPType_Tval pbTpType;                                 
	int		pbTpIndex;                                         
	pbRprSOLCASActive_Tval pbRprSOLCASActive;   
	int		pbRprSOLCASWanPort;   
	pbRprSOLCASState_Tval pbRprSOLCASState;   
}MIBpbRprSOLCASConfigTable_t;














 



 
 
extern MIBDid_t pbRprSKLCASConfigTable_Did;

   
typedef enum {
	pbRprSKLCASState_fixed=0,
	pbRprSKLCASState_idle=1,
	pbRprSKLCASState_ok=2,
	pbRprSKLCASState_fail=3
} pbRprSKLCASState_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                     
	int		pbPortNo;                                          
	PbTPType_Tval pbTpType;                                 
	int		pbTpIndex;                                         
	int		pbRprSKLCASWanPort;   
	pbRprSKLCASState_Tval pbRprSKLCASState;   
}MIBpbRprSKLCASConfigTable_t;













 



 
 
extern MIBDid_t rprServiceTable_Did;

   
typedef enum {
	rprSvcClassifyType_port=0,
	rprSvcClassifyType_portVlan=1,
	rprSvcClassifyType_portVlanVpri=2
} rprSvcClassifyType_Tval;

typedef enum {
	rprSvcMode_basic=1,
	rprSvcMode_share=2,
	rprSvcMode_cross=3
} rprSvcMode_Tval;

typedef enum {
	rprSvcTtlMode_auto=1,
	rprSvcTtlMode_autoAdvanced=2,
	rprSvcTtlMode_manualCloseRing=3,
	rprSvcTtlMode_manualAll=4,
	rprSvcTtlMode_manualMac=5,
	rprSvcTtlMode_manualMacAdv=6
} rprSvcTtlMode_Tval;

typedef enum {
	rprGmiiSel_gmii0=0,
	rprGmiiSel_gmii1=1
} rprGmiiSel_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                             
	int		rprIfIndex;                                                
	int		rprServiceId;                                                 
	int		rprLocalEthPortNo;                                            
	int		rprLocalVID;                                                  
	int		rprLocalVPrio;                                                
	rprSvcClassifyType_Tval rprSvcClassifyType;                        
	RprServiceType_Tval rprServiceType;   
	rprSvcMode_Tval rprSvcMode;   
	char		rprRmtStationMac[20];   
	RprRingletID_Tval rprSvcRingletSel;   
	RprRingletID_Tval rprSvcRealRingletSel;   
	TruthValue_Tval rprSvcAutoRingletSelEn;   
	RprServiceCos_Tval rprSvcCos;   
	rprSvcTtlMode_Tval rprSvcTtlMode;   
	int		rprSvcTtl0;   
	int		rprSvcTtl1;   
	int		rprSvcRealTtl0;   
	int		rprSvcRealTtl1;   
	TruthValue_Tval rprSvcWe;   
	TruthValue_Tval rprSvcSteered;   
	int		rprSvcRateProfIndex;   
	int		rprSvcTokenRate;   
	int		rprSvcBurstSize;   
	int		rprSvcMaxDelay;   
	int		rprSvcMaxJitter;   
	int		rprSvcWeight;   
	rprGmiiSel_Tval rprGmiiSel;   
	TruthValue_Tval rprDoubleVlanEn;   
	int		rprDoubleVID;   
	int		rprDoubleVPrio;   
	TruthValue_Tval rprSvcLoop;   
	TruthValue_Tval rprSvcActive;   
	RprSvcStatus_Tval rprSvcCurStatus;   
	RprSvcFailReason_Tval rprSvcReason;   
	char		rprSvcLabel[32];   
	int		rprFlowRuleID;   
	int		rprSvcMulBrdCastID;
	TruthValue_Tval rprSvcMulBrdCastEn;
	RowStatus_Tval rprSvcRowStatus;   
}MIBrprServiceTable_t;















































 



 
 
extern MIBDid_t rprRateProfileTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                     
	int		rprIfIndex;                                        
	int		rprRateProfNo;                                        
	int		rprTokenRate;   
	int		rprBurstSize;   
	int		rprExcessBurstSize;   
}MIBrprRateProfileTable_t;













 



 
 
extern MIBDid_t pbRprMultiCastTable_Did;

   
typedef enum {
	rprMultiCastGMRP_yes=0,
	rprMultiCastGMRP_no=1
} rprMultiCastGMRP_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                 
	int		rprIfIndex;                                                    
	int		rprServiceId;                                                  
	char		rprMultiCastMacAddress[20];                                      
	int		rprMultiCastDoubleVlanId;   
	char		rprMultiCastPortsList[64];   
	rprMultiCastGMRP_Tval rprMultiCastGMRP;   
	RowStatus_Tval rprMultiCastRowStatus;   
}MIBpbRprMultiCastTable_t;















 



 
 
extern MIBDid_t pbRprAclTable_Did;

   
typedef enum {
	rprAclMacType_destinationMac=1,
	rprAclMacType_sourceMac=2
} rprAclMacType_Tval;

typedef enum {
	rprAclMode_filter=1,
	rprAclMode_forward=2
} rprAclMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                     
	int		rprIfIndex;                                        
	int		rprServiceId;                                      
	char		rprAclMacAddress[20];                                
	rprAclMacType_Tval rprAclMacType;   
	int		rprAclDoubleVlanId;   
	char		rprAclPortsList[64];   
	rprAclMode_Tval rprAclMode;   
	RowStatus_Tval rprAclRowStatus;   
	int		rprAclTblIdx;   
}MIBpbRprAclTable_t;

















 



 
 
extern MIBDid_t pbRprMultiBroadCastTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                 
	int		rprIfIndex;                                                                    
	int		rprMultiBroadCastId;                                                              
	int		rprMultiBroadCastTimeInterval;   
	int		rprMultiBroadCastPacketsInterval;   
}MIBpbRprMultiBroadCastTable_t;












 



 
 
extern MIBDid_t rprTopoImageTable_Did;

   
typedef enum {
	rprTopoImageCapability_default=0,
	rprTopoImageCapability_jbStrNspconsfair=1,
	rprTopoImageCapability_regWrpNspconsfair=2,
	rprTopoImageCapability_jbWrpNspconsfair=3,
	rprTopoImageCapability_regStrSpconsfair=4,
	rprTopoImageCapability_jbStrSpconsfair=5,
	rprTopoImageCapability_regWrpSpconsfair=6,
	rprTopoImageCapability_jbWrpSpconsfair=7
} rprTopoImageCapability_Tval;

typedef enum {
	rprTopoImageStatus_reachableRinglet0=1,
	rprTopoImageStatus_reachableRinglet1=2,
	rprTopoImageStatus_reachableRinglet0Ringlet1=3,
	rprTopoImageStatus_wrapActiveWest=4,
	rprTopoImageStatus_wrapActiveEast=8,
	rprTopoImageStatus_receivedBadFcs=16,
	rprTopoImageStatus_receivBadFcsReachableR0R1=19,
	rprTopoImageStatus_receivedMultichokeFairness=32
} rprTopoImageStatus_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                 
	int		rprIfIndex;                                                                    
	char		rprTopoImageMacAddress[20];                                                      
	char		rprTopoImageSecMacAddress1[20];   
	char		rprTopoImageSecMacAddress2[20];   
	int		rprTopoImageStationIfIndex;   
	char		rprTopoImageStationName[255];   
	rprTopoImageCapability_Tval rprTopoImageCapability;   
	int		rprTopoImageRinglet0Hops;   
	int		rprTopoImageRinglet0ReservedRate;   
	int		rprTopoImageRinglet1Hops;   
	int		rprTopoImageRinglet1ReservedRate;   
	RprProtectionStatus_Tval rprTopoImageWestProtectionStatus;   
	int		rprTopoImageWestWeight;   
	RprProtectionStatus_Tval rprTopoImageEastProtectionStatus;   
	int		rprTopoImageEastWeight;   
	rprTopoImageStatus_Tval rprTopoImageStatus;   
	char		rprTopoImageRemoteNEAddress[20];   
	int		rprTopoImageRemoteSlotNumber;   
}MIBrprTopoImageTable_t;


























 



 
 
extern MIBDid_t rprFairnessTable_Did;

   
typedef enum {
	rprFairnessRinglet_ringlet0=1,
	rprFairnessRinglet_ringlet1=2
} rprFairnessRinglet_Tval;

typedef enum {
	rprFairnessResetWaterMarks_idle=1,
	rprFairnessResetWaterMarks_resetWaterMarks=2
} rprFairnessResetWaterMarks_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                           
	int		rprIfIndex;                                                              
	rprFairnessRinglet_Tval rprFairnessRinglet;                                      
	int		rprFairnessRingletWeight;   
	int		rprFairnessReservedRate;   
	int		rprFairnessMaxAllowed;   
	int		rprFairnessAgeCoef;   
	int		rprFairnessLpCoef;   
	int		rprFairnessAdvertisementRatio;   
	int		rprFairnessMcffReportCoef;   
	int		rprFairnessActiveWeightsCoef;   
	int		rprFairnessSTQHighThreshold;   
	int		rprFairnessSTQMedThreshold;   
	int		rprFairnessSTQLowThreshold;   
	int		rprFairnessRateHighThreshold;   
	int		rprFairnessRateLowThreshold;   
	rprFairnessResetWaterMarks_Tval rprFairnessResetWaterMarks;   
	int		rprFairnessSTQHighWaterMark;   
	int		rprFairnessSTQLowWaterMark;   
	int		rprFairnessRampUpCoef;   
	int		rprFairnessRampDnCoef;   
}MIBrprFairnessTable_t;




























 



 
 
extern MIBDid_t rprOamTable_Did;

   
typedef enum {
	rprOamActionType_echo=1,
	rprOamActionType_flush=2
} rprOamActionType_Tval;

typedef enum {
	rprOamClassOfService_classA=1,
	rprOamClassOfService_classB=2,
	rprOamClassOfService_classC=3
} rprOamClassOfService_Tval;

typedef enum {
	rprOamControl_idle=1,
	rprOamControl_active=2,
	rprOamControl_abort=3
} rprOamControl_Tval;

typedef enum {
	rprOamResponseStatus_unknown=1,
	rprOamResponseStatus_inProcess=2,
	rprOamResponseStatus_error=3,
	rprOamResponseStatus_success=4
} rprOamResponseStatus_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                           
	int		rprIfIndex;                                              
	rprOamActionType_Tval rprOamActionType;   
	char		rprOamDestAddress[20];   
	RprOamRinglet_Tval rprOamRequestRinglet;   
	RprOamRinglet_Tval rprOamResponseRinglet;   
	rprOamClassOfService_Tval rprOamClassOfService;   
	char		rprOamUserData[20];   
	TruthValue_Tval rprOamProtected;   
	int		rprOamRequestCount;   
	int		rprOamTimeout;   
	rprOamControl_Tval rprOamControl;   
	int		rprOamResponseCount;   
	int		rprOamAvResponseTime;   
	rprOamResponseStatus_Tval rprOamResponseStatus;   
}MIBrprOamTable_t;






















 



 
 
extern MIBDid_t rprTopoRingInfoTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                 
	int		rprIfIndex;                                                    
	TruthValue_Tval rprRingMultiChoke0;   
	TruthValue_Tval rprRingMultiChoke1;   
	int		rprRingMtuSize;   
	int		rprRingTotalHopsTx0;   
	int		rprRingTotalHopsTx1;   
	char		rprRingLastNeighborMac0[20];   
	char		rprRingLastNeighborMac1[20];   
	int		rprRingCheckSum;   
	int		rprRingNeighborCheckSum0;   
	int		rprRingNeighborCheckSum1;   
	TruthValue_Tval rprRingCheckSumValid;   
	TruthValue_Tval rprRingNeighborCksValid0;   
	TruthValue_Tval rprRingNeighborCksValid1;   
}MIBrprTopoRingInfoTable_t;






















 



 
 
extern MIBDid_t rprIfTable_Did;

   
typedef enum {
	rprIfFairnessMode_aggressive=1,
	rprIfFairnessMode_conservative=2,
	rprIfFairnessMode_others=3
} rprIfFairnessMode_Tval;

typedef enum {
	rprIfMacOperModes_default=0,
	rprIfMacOperModes_strctorderNdrpbadfcs=1,
	rprIfMacOperModes_regorderDrpbadfcs=2,
	rprIfMacOperModes_strctorderDrpbadfcs=3
} rprIfMacOperModes_Tval;

typedef enum {
	rprIfRingOperModes_default=0,
	rprIfRingOperModes_jbStrClsring=1,
	rprIfRingOperModes_regWrpClsring=2,
	rprIfRingOperModes_jbWrpClsring=3,
	rprIfRingOperModes_regStrOpenring=4,
	rprIfRingOperModes_jbStrOpenring=5,
	rprIfRingOperModes_regWrpOpenring=6,
	rprIfRingOperModes_jbWrpOpenring=7
} rprIfRingOperModes_Tval;

typedef enum {
	rprIfCurrentStatus_ok=0,
	rprIfCurrentStatus_neighborInconsistency=1,
	rprIfCurrentStatus_duplicateMac=2,
	rprIfCurrentStatus_neiborInconsistDMAC=3,
	rprIfCurrentStatus_exceedMaxStations=4
} rprIfCurrentStatus_Tval;

typedef enum {
	rprIfMplsMode_mpls=0,
	rprIfMplsMode_qinQ=1,
	rprIfMplsMode_transparent=2
} rprIfMplsMode_Tval;

typedef enum {
	rprIfHwPassThrough_disable=1,
	rprIfHwPassThrough_enable=2,
	rprIfHwPassThrough_force=3
} rprIfHwPassThrough_Tval;

typedef enum {
	rprIfBandMode_auto=1,
	rprIfBandMode_manual=2
} rprIfBandMode_Tval;

typedef enum {
	rprIfCardProtection_disable=1,
	rprIfCardProtection_enable=2,
	rprIfCardProtection_force=3
} rprIfCardProtection_Tval;

typedef enum {
	rprIfL2RateLimitMode_advanced=1,
	rprIfL2RateLimitMode_basic=2
} rprIfL2RateLimitMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                 
	int		rprIfIndex;                                                    
	int		rprIfStationsOnRing;   
	TruthValue_Tval rprIfReversionMode;   
	int		rprIfProtectionWTR;   
	int		rprIfProtectionFastTimer;   
	int		rprIfProtectionSlowTimer;   
	int		rprIfAtdTimer;   
	int		rprIfKeepaliveTimeout;   
	rprIfFairnessMode_Tval rprIfFairnessMode;   
	int		rprIfPtqSize;   
	int		rprIfStqSize;   
	int		rprIfSTQFullThreshold;   
	int		rprIfIdleThreshold;   
	int		rprIfSesThreshold;   
	RprProtectionType_Tval rprIfProtectionMode;   
	TruthValue_Tval rprIfJumboFramePreferred;   
	rprIfMacOperModes_Tval rprIfMacOperModes;   
	rprIfRingOperModes_Tval rprIfRingOperModes;   
	rprIfCurrentStatus_Tval rprIfCurrentStatus;   
	int		rprIfLinkRate;   
	rprIfMplsMode_Tval rprIfMplsMode;   
	int		rprIfMplsTypeTag;   
	TruthValue_Tval rprIfVlanPrioEn;   
	char		rprIfMyMacAddress[20];   
	TruthValue_Tval rprIfPassThroughEn;   
	rprIfHwPassThrough_Tval rprIfHwPassThrough;   
	TruthValue_Tval rprIfHwPassThroughStatus;   
	int		rprIfResRateR0;   
	int		rprIfResRateR1;   
	int		rprIfA0RateR0;   
	int		rprIfA0RateR1;   
	int		rprIfA1RateR0;   
	int		rprIfA1RateR1;   
	int		rprIfBcirRateR0;   
	int		rprIfBcirRateR1;   
	int		rprIfRealA0RateR0;   
	int		rprIfRealA0RateR1;   
	int		rprIfRealA1RateR0;   
	int		rprIfRealA1RateR1;   
	int		rprIfRealBcirRateR0;   
	int		rprIfRealBcirRateR1;   
	TruthValue_Tval rprIfAlarmMon;   
	char		rprIfStationName[128];   
	TruthValue_Tval rprIfInterWorking;   
	rprIfBandMode_Tval rprIfBandMode;   
	TruthValue_Tval rprIfMulBrdCastEn;
	rprIfCardProtection_Tval rprIfCardProtection;   
	TruthValue_Tval rprIfCardProtectionState;   
	rprIfL2RateLimitMode_Tval rprIfL2RateLimitMode;   
	char		rprIfCrossPortsList[64];   
	TruthValue_Tval rprIfLogSave;   
	TruthValue_Tval rprIfL2CosFlowControl;   
}MIBrprIfTable_t;




























































 



 
 
extern MIBDid_t rprSpanTable_Did;

   
typedef enum {
	rprSpanCurrentStatus_ok=0,
	rprSpanCurrentStatus_kaNmcNsdNsf=1,
	rprSpanCurrentStatus_nkaMcNsdNsf=2,
	rprSpanCurrentStatus_kaMcNsdNsf=3,
	rprSpanCurrentStatus_nkaNmcSdNsf=4,
	rprSpanCurrentStatus_kaNmcSdNsf=5,
	rprSpanCurrentStatus_nkaMcSdNsf=6,
	rprSpanCurrentStatus_kaMcSdNsf=7,
	rprSpanCurrentStatus_nkaNmcNsdSf=8,
	rprSpanCurrentStatus_kaNmcNsdSf=9,
	rprSpanCurrentStatus_nkaMcNsdSf=10,
	rprSpanCurrentStatus_kaMcNsdSf=11,
	rprSpanCurrentStatus_nkaNmcSdSf=12,
	rprSpanCurrentStatus_kaNmcSdSf=13,
	rprSpanCurrentStatus_nkaMcSdSf=14,
	rprSpanCurrentStatus_kaMcSdSf=15
} rprSpanCurrentStatus_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                               
	int		rprIfIndex;                                                                  
	RprSpan_Tval rprSpanId;                                                           
	int		rprSpanTotalRingletReservedRate;   
	rprSpanCurrentStatus_Tval rprSpanCurrentStatus;   
	int		rprSpanWanPortNo;   
	int		rprSpanHopsToWrap;   
}MIBrprSpanTable_t;














 



 
 
extern MIBDid_t rprSpanProtectionTable_Did;

   
typedef enum {
	rprSpanProtectionCommand_idle=1,
	rprSpanProtectionCommand_manualSwitch=2,
	rprSpanProtectionCommand_forcedSwitch=3
} rprSpanProtectionCommand_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                       
	int		rprIfIndex;                                                                          
	RprSpan_Tval rprSpanId;                                                                   
	TruthValue_Tval rprSpanProtectionNeighborValid;   
	int		rprSpanProtectionHoldOffTimer;   
	rprSpanProtectionCommand_Tval rprSpanProtectionCommand;   
	unsigned int	rprSpanProtectionCount;   
	unsigned int	rprSpanProtectionDuration;   
	unsigned int	rprSpanProtectionLastActivationTime;   
	TruthValue_Tval rprSpanProtectionCountReset;   
}MIBrprSpanProtectionTable_t;

















 



 
 
extern MIBDid_t rprCardProtectionTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                         
	int		rprIfIndex;                                            
	int		rprCardProtectionId;                                      
	int		rprCpfWorkSlot;   
	int		rprCpfProtSlot;   
	int		rprCpfActiveSlot;   
	int		rprCpfCoordSlot;   
	int		rprCpfCoordProtGroup;   
	TruthValue_Tval rprCpfEnable;   
}MIBrprCardProtectionTable_t;
















 



 
extern MIBDid_t pbRprModule_Did;

   



# 27 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_sdh_pm.h" 1



 




 



 
extern MIBDid_t pbSdhPmUpLoadInfo_Did;

   


 



 
extern MIBDid_t pbSdhMostRecentIndex_Did;

   


 



 
extern MIBDid_t pbSdhRecentUapPerfMon_Did;

   


 



 
extern MIBDid_t pbSdhUapPerfMon_Did;

   


 



 
extern MIBDid_t pbSdhRecentPerfMon_Did;

   


 



 
extern MIBDid_t pbSdhCurPerfMon_Did;

   
typedef struct
{
	int		pbEthPmMonTimeIntervalSet;   
	PbEnableDisable_Tval pbPdhPmSlipEventEnable;   
}MIBpbSdhCurPerfMon_t;







 



 
extern MIBDid_t pbPMFilterConfigGroup_Did;

   
typedef struct
{
	TruthValue_Tval pbPmDeleteAllFilter;
	PbSlotNumberType_Tval pbPmDeleteSlotNumber;
	TruthValue_Tval pbPmDeleteSlotFilter;
	unsigned int	pbPmDeleteFilterTypeId;
	TruthValue_Tval pbPmDeleteFilterType;
}MIBpbPMFilterConfigGroup_t;










 



 
 
extern MIBDid_t pbPMFilterTable_Did;

   
typedef struct
{
	unsigned int	pbPmFilterType;                    
	PbSlotNumberType_Tval pbSlotNumber;             
	PbPortType_Tval pbPortType;                     
	int		pbPortNo;                                  
	PbTPType_Tval pbTpType;                         
	int		pbTpIndex;                                 
	PbPmTableType_Tval pbTableType;                 
	unsigned int	pbPmEthId;                         
	unsigned int	pbPmReserved;
}MIBpbPMFilterTable_t;
















 



 
 
extern MIBDid_t pbSdhCurPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	PbPortType_Tval pbPortType;                                       
	int		pbPortNo;                                                    
	PbTPType_Tval pbTpType;                                           
	int		pbTpIndex;                                                   
	PbPmTableType_Tval pbTableType;                                      
	PbActualPortType_Tval pbPmActualPortType;   
	PbPmMonitor_Tval pbSdhPmMonitor;   
	unsigned int	pbEsPmCounter;   
	TruthValue_Tval pbEsPmThresholdEnable;   
	int		pbEsPmThresholdLow;   
	int		pbEsPmThresholdHigh;   
	unsigned int	pbBbePmCounter;   
	TruthValue_Tval pbBbePmThresholdEnable;   
	int		pbBbePmThresholdLow;   
	int		pbBbePmThresholdHigh;   
	unsigned int	pbSesPmCounter;   
	TruthValue_Tval pbSesPmThresholdEnable;   
	int		pbSesPmThresholdLow;   
	int		pbSesPmThresholdHigh;   
	unsigned int	pbUasPmCounter;   
	TruthValue_Tval pbUasPmThresholdEnable;   
	int		pbUasPmThresholdLow;   
	int		pbUasPmThresholdHigh;   
	unsigned int	pbCsesPmCounter;   
	TruthValue_Tval pbCsesPmThresholdEnable;   
	int		pbCsesPmThresholdLow;   
	int		pbCsesPmThresholdHigh;   
	unsigned int	pbFcPmCounter;   
	TruthValue_Tval pbFcPmThresholdEnable;   
	int		pbFcPmThresholdLow;   
	int		pbFcPmThresholdHigh;   
	unsigned int	pbOfsPmCounter;
	TruthValue_Tval pbOfsPmThresholdEnable;   
	int		pbOfsPmThresholdLow;   
	int		pbOfsPmThresholdHigh;   
	unsigned int	pbPpjcPmCounter;   
	TruthValue_Tval pbPpjcPmThresholdEnable;   
	int		pbPpjcPmThresholdLow;   
	int		pbPpjcPmThresholdHigh;   
	unsigned int	pbNpjcPmCounter;   
	TruthValue_Tval pbNpjcPmThresholdEnable;   
	int		pbNpjcPmThresholdLow;   
	int		pbNpjcPmThresholdHigh;   
	TruthValue_Tval pbPmTableReset;   
	char		pbPmTimeStart[16];   
	unsigned int	pbPmTimeElapse;   
	PbPmPortStatusType_Tval pbPmProtStatus;   
	PbPmTpStatusType_Tval pbPmTpStatus;   
	TruthValue_Tval pbPmSuspect;   
}MIBpbSdhCurPMTable_t;

























































 



 
 
extern MIBDid_t pbEthCurPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                                   
	int		pbPortNo;                                                                                        
	PbPmTableType_Tval pbEthTableType;                                                                       
	EthPortClass_Tval pbEthPmPortType;                                                                    
	PbPmMonitor_Tval pbEthPmMonitor;   
	char		pbEthPmTimeStart[16];   
	unsigned int	pbEthPmTimeElapse;   
	TruthValue_Tval pbEthPmTimeSuspect;   
	TruthValue_Tval pbEthPmTableReset;   
	unsigned int	pbEthInUnicastPktsHigh;   
	unsigned int	pbEthInUnicastPktsLow;   
	unsigned int	pbEthInMulcastPktsHigh;   
	unsigned int	pbEthInMulcastPktsLow;   
	unsigned int	pbEthInBrdcastPktsHigh;   
	unsigned int	pbEthInBrdcastPktsLow;   
	unsigned int	pbEthInAllBadPktsHigh;   
	unsigned int	pbEthInAllBadPktsLow;   
	unsigned int	pbEthInAllBadBytesHigh;   
	unsigned int	pbEthInAllBadBytesLow;   
	unsigned int	pbEthInAllGoodPktsHigh;   
	unsigned int	pbEthInAllGoodPktsLow;   
	unsigned int	pbEthInAllGoodBytesHigh;   
	unsigned int	pbEthInAllGoodBytesLow;   
	unsigned int	pbEthInOverSizePktsHigh;   
	unsigned int	pbEthInOverSizePktsLow;   
	unsigned int	pbEthInUnderSizePktsHigh;   
	unsigned int	pbEthInUnderSizePktsLow;   
	unsigned int	pbEthInCRCErrPktsHigh;   
	unsigned int	pbEthInCRCErrPktsLow;   
	unsigned int	pbEthOutUnicastPktsHigh;   
	unsigned int	pbEthOutUnicastPktsLow;   
	unsigned int	pbEthOutMulcastPktsHigh;   
	unsigned int	pbEthOutMulcastPktsLow;   
	unsigned int	pbEthOutBrdcastPktsHigh;   
	unsigned int	pbEthOutBrdcastPktsLow;   
	unsigned int	pbEthOutAllGoodPktsHigh;   
	unsigned int	pbEthOutAllGoodPktsLow;   
	unsigned int	pbEthOutAllGoodBytesHigh;   
	unsigned int	pbEthOutAllGoodBytesLow;   
	TruthValue_Tval pbEthBbrEnable;   
	int		pbEthBbrThresholdValue;   
	TruthValue_Tval pbEthBprEnable;   
	int		pbEthBprThresholdValue;   
	unsigned int	pbEthCurInAllBytesHigh;   
	unsigned int	pbEthCurInAllBytesLow;   
	unsigned int	pbEthCurInAllPktsHigh;   
	unsigned int	pbEthCurInAllPktsLow;   
	unsigned int	pbEthCurInAllGoodPktsHigh;   
	unsigned int	pbEthCurInAllGoodPktsLow;   
	unsigned int	pbEthCurInAllBadPktsHigh;   
	unsigned int	pbEthCurInAllBadPktsLow;   
	unsigned int	pbEthCurInUnicastPktsHigh;   
	unsigned int	pbEthCurInUnicastPktsLow;   
	unsigned int	pbEthCurInMulcastPktsHigh;   
	unsigned int	pbEthCurInMulcastPktsLow;   
	unsigned int	pbEthCurInBrdcastPktsHigh;   
	unsigned int	pbEthCurInBrdcastPktsLow;   
	unsigned int	pbEthCurInDropPktsHigh;   
	unsigned int	pbEthCurInDropPktsLow;   
	unsigned int	pbEthCurInUndersizeGoodPktsHigh;   
	unsigned int	pbEthCurInUndersizeGoodPktsLow;   
	unsigned int	pbEthCurInOversizeGoodPktsHigh;   
	unsigned int	pbEthCurInOversizeGoodPktsLow;   
	unsigned int	pbEthCurIn64PktsHigh;   
	unsigned int	pbEthCurIn64PktsLow;   
	unsigned int	pbEthCurIn65To127PktsHigh;   
	unsigned int	pbEthCurIn65To127PktsLow;   
	unsigned int	pbEthCurIn128To255PktsHigh;   
	unsigned int	pbEthCurIn128To255PktsLow;   
	unsigned int	pbEthCurIn256To511PktsHigh;   
	unsigned int	pbEthCurIn256To511PktsLow;   
	unsigned int	pbEthCurIn512To1023PktsHigh;   
	unsigned int	pbEthCurIn512To1023PktsLow;   
	unsigned int	pbEthCurIn1024To1518PktsHigh;   
	unsigned int	pbEthCurIn1024To1518PktsLow;   
	unsigned int	pbEthCurInPausePktsHigh;   
	unsigned int	pbEthCurInPausePktsLow;   
	unsigned int	pbEthCurInFragmentPktsHigh;   
	unsigned int	pbEthCurInFragmentPktsLow;   
	unsigned int	pbEthCurInJabberPktsHigh;   
	unsigned int	pbEthCurInJabberPktsLow;   
	unsigned int	pbEthCurInAlignmentErrorPktsHigh;   
	unsigned int	pbEthCurInAlignmentErrorPktsLow;   
	unsigned int	pbEthCurOutAllBytesHigh;   
	unsigned int	pbEthCurOutAllBytesLow;   
	unsigned int	pbEthCurOutAllPktsHigh;   
	unsigned int	pbEthCurOutAllPktsLow;   
	unsigned int	pbEthCurOutUnicastPktsHigh;   
	unsigned int	pbEthCurOutUnicastPktsLow;   
	unsigned int	pbEthCurOutMulcastPktsHigh;   
	unsigned int	pbEthCurOutMulcastPktsLow;   
	unsigned int	pbEthCurOutBrdcastPktsHigh;   
	unsigned int	pbEthCurOutBrdcastPktsLow;   
	unsigned int	pbEthCurOutGoodPausePktsHigh;   
	unsigned int	pbEthCurOutGoodPausePktsLow;   
	unsigned int	pbEthCurOutCollisionPktsHigh;   
	unsigned int	pbEthCurOutCollisionPktsLow;   
	unsigned int	pbCurOutInvalidSymbolsHigh;
	unsigned int	pbCurOutInvalidSymbolsLow;
	unsigned int	pbCurOutSuperblockCrcErrorHigh;
	unsigned int	pbCurOutSuperblockCrcErrorLow;
	unsigned int	pbCurOutSuperblockCorrectableCrcErrorHigh;
	unsigned int	pbCurOutSuperblockCorrectableCrcErrorLow;
}MIBpbEthCurPMTable_t;














































































































 



 
 
extern MIBDid_t pbxSecEthPmMonTable_Did;

   
typedef enum {
	pbxSecEthPmReportStatus_notReport=1,
	pbxSecEthPmReportStatus_reportOnce=2
} pbxSecEthPmReportStatus_Tval;

typedef enum {
	pbxSecEthPmMonitorStatus_notMonitor=1,
	pbxSecEthPmMonitorStatus_monitor=2
} pbxSecEthPmMonitorStatus_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                 
	int		pbPortNo;                                                      
	EthPortClass_Tval pbxSecEthPmPortType;                                 
	pbxSecEthPmReportStatus_Tval pbxSecEthPmReportStatus;   
	pbxSecEthPmMonitorStatus_Tval pbxSecEthPmMonitorStatus;   
	unsigned int	pbxSecEthPmFtpHostIP;   
	char		pbxSecEthPmFtpPath[64];   
	char		pbxSecEthPmFtpFileName[13];   
	char		pbxSecEthPmFtpUsername[32];   
	char		pbxSecEthPmFtpPassword[32];   
	char		pbxSecEthPmRequestId[40];   
	PbPmReportResp_Tval pbxSecEthPmRespMsg;   
}MIBpbxSecEthPmMonTable_t;



















 



 
 
extern MIBDid_t pbRprClientCurPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                             
	int		rprIfIndex;                                                                                
	PbPmTableType_Tval pbRprClientTableType;                                                           
	PbPmMonitor_Tval pbRprClientPmMonitor;   
	char		pbRprClientPmTimeStart[16];   
	unsigned int	pbRprClientPmTimeElapse;   
	TruthValue_Tval pbRprClientPmTimeSuspect;   
	TruthValue_Tval pbRprClientPmTableReset;   
	unsigned int	pbRprClientInAllFramesHigh;   
	unsigned int	pbRprClientInAllFramesLow;   
	unsigned int	pbRprClientOutAllFramesHigh;   
	unsigned int	pbRprClientOutAllFramesLow;   
	unsigned int	pbRprClientOutPassFramesHigh;   
	unsigned int	pbRprClientOutPassFramesLow;   
	unsigned int	pbRprClientOutDropFramesHigh;   
	unsigned int	pbRprClientOutDropFramesLow;   
	unsigned int	pbRprClientOutErrLenFramesHigh;   
	unsigned int	pbRprClientOutErrLenFramesLow;   
	unsigned int	pbRprClientOutErrCrcFramesHigh;   
	unsigned int	pbRprClientOutErrCrcFramesLow;   
	unsigned int	pbRprClientInUcastClassAFramesHigh;   
	unsigned int	pbRprClientInUcastClassAFramesLow;   
	unsigned int	pbRprClientInUcastClassAOctetsHigh;   
	unsigned int	pbRprClientInUcastClassAOctetsLow;   
	unsigned int	pbRprClientInUcastClassBCirFramesHigh;   
	unsigned int	pbRprClientInUcastClassBCirFramesLow;   
	unsigned int	pbRprClientInUcastClassBCirOctetsHigh;   
	unsigned int	pbRprClientInUcastClassBCirOctetsLow;   
	unsigned int	pbRprClientInUcastClassBEirFramesHigh;   
	unsigned int	pbRprClientInUcastClassBEirFramesLow;   
	unsigned int	pbRprClientInUcastClassBEirOctetsHigh;   
	unsigned int	pbRprClientInUcastClassBEirOctetsLow;   
	unsigned int	pbRprClientInUcastClassCFramesHigh;   
	unsigned int	pbRprClientInUcastClassCFramesLow;   
	unsigned int	pbRprClientInUcastClassCOctetsHigh;   
	unsigned int	pbRprClientInUcastClassCOctetsLow;   
	unsigned int	pbRprClientInMcastClassAFramesHigh;   
	unsigned int	pbRprClientInMcastClassAFramesLow;   
	unsigned int	pbRprClientInMcastClassAOctetsHigh;   
	unsigned int	pbRprClientInMcastClassAOctetsLow;   
	unsigned int	pbRprClientInMcastClassBCirFramesHigh;   
	unsigned int	pbRprClientInMcastClassBCirFramesLow;   
	unsigned int	pbRprClientInMcastClassBCirOctetsHigh;   
	unsigned int	pbRprClientInMcastClassBCirOctetsLow;   
	unsigned int	pbRprClientInMcastClassBEirFramesHigh;   
	unsigned int	pbRprClientInMcastClassBEirFramesLow;   
	unsigned int	pbRprClientInMcastClassBEirOctetsHigh;   
	unsigned int	pbRprClientInMcastClassBEirOctetsLow;   
	unsigned int	pbRprClientInMcastClassCFramesHigh;   
	unsigned int	pbRprClientInMcastClassCFramesLow;   
	unsigned int	pbRprClientInMcastClassCOctetsHigh;   
	unsigned int	pbRprClientInMcastClassCOctetsLow;   
	unsigned int	pbRprClientOutUcastClassAFramesHigh;   
	unsigned int	pbRprClientOutUcastClassAFramesLow;   
	unsigned int	pbRprClientOutUcastClassAOctetsHigh;   
	unsigned int	pbRprClientOutUcastClassAOctetsLow;   
	unsigned int	pbRprClientOutUcastClassBCirFramesHigh;   
	unsigned int	pbRprClientOutUcastClassBCirFramesLow;   
	unsigned int	pbRprClientOutUcastClassBCirOctetsHigh;   
	unsigned int	pbRprClientOutUcastClassBCirOctetsLow;   
	unsigned int	pbRprClientOutUcastClassBEirFramesHigh;   
	unsigned int	pbRprClientOutUcastClassBEirFramesLow;   
	unsigned int	pbRprClientOutUcastClassBEirOctetsHigh;   
	unsigned int	pbRprClientOutUcastClassBEirOctetsLow;   
	unsigned int	pbRprClientOutUcastClassCFramesHigh;   
	unsigned int	pbRprClientOutUcastClassCFramesLow;   
	unsigned int	pbRprClientOutUcastClassCOctetsHigh;   
	unsigned int	pbRprClientOutUcastClassCOctetsLow;   
	unsigned int	pbRprClientOutMcastClassAFramesHigh;   
	unsigned int	pbRprClientOutMcastClassAFramesLow;   
	unsigned int	pbRprClientOutMcastClassAOctetsHigh;   
	unsigned int	pbRprClientOutMcastClassAOctetsLow;   
	unsigned int	pbRprClientOutMcastClassBCirFramesHigh;   
	unsigned int	pbRprClientOutMcastClassBCirFramesLow;   
	unsigned int	pbRprClientOutMcastClassBCirOctetsHigh;   
	unsigned int	pbRprClientOutMcastClassBCirOctetsLow;   
	unsigned int	pbRprClientOutMcastClassBEirFramesHigh;   
	unsigned int	pbRprClientOutMcastClassBEirFramesLow;   
	unsigned int	pbRprClientOutMcastClassBEirOctetsHigh;   
	unsigned int	pbRprClientOutMcastClassBEirOctetsLow;   
	unsigned int	pbRprClientOutMcastClassCFramesHigh;   
	unsigned int	pbRprClientOutMcastClassCFramesLow;   
	unsigned int	pbRprClientOutMcastClassCOctetsHigh;   
	unsigned int	pbRprClientOutMcastClassCOctetsLow;   
}MIBpbRprClientCurPMTable_t;



























































































 



 
 
extern MIBDid_t pbRprSpanCurPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                         
	int		rprIfIndex;                                                                            
	RprSpan_Tval rprSpanId;                                                                     
	PbPmTableType_Tval pbRprSpanTableType;                                                         
	PbPmMonitor_Tval pbRprSpanPmMonitor;   
	char		pbRprSpanPmTimeStart[16];   
	unsigned int	pbRprSpanPmTimeElapse;   
	TruthValue_Tval pbRprSpanPmTimeSuspect;   
	TruthValue_Tval pbRprSpanPmTableReset;   
	unsigned int	pbRprSpanInAllFramesHigh;   
	unsigned int	pbRprSpanInAllFramesLow;   
	unsigned int	pbRprSpanInWrapFramesHigh;   
	unsigned int	pbRprSpanInWrapFramesLow;   
	unsigned int	pbRprSpanInDataFramesHigh;   
	unsigned int	pbRprSpanInDataFramesLow;   
	unsigned int	pbRprSpanInCtrlFramesHigh;   
	unsigned int	pbRprSpanInCtrlFramesLow;   
	unsigned int	pbRprSpanInFaFramesHigh;   
	unsigned int	pbRprSpanInFaFramesLow;   
	unsigned int	pbRprSpanOutAllFramesHigh;   
	unsigned int	pbRprSpanOutAllFramesLow;   
	unsigned int	pbRprSpanOutAddFramesHigh;   
	unsigned int	pbRprSpanOutAddFramesLow;   
	unsigned int	pbRprSpanOutTransitFramesHigh;   
	unsigned int	pbRprSpanOutTransitFramesLow;   
	unsigned int	pbRprSpanOutWrapFramesHigh;   
	unsigned int	pbRprSpanOutWrapFramesLow;   
	unsigned int	pbRprSpanOutTpFramesHigh;   
	unsigned int	pbRprSpanOutTpFramesLow;   
	unsigned int	pbRprSpanOutFaFramesHigh;   
	unsigned int	pbRprSpanOutFaFramesLow;   
	unsigned int	pbRprSpanOutOamFramesHigh;   
	unsigned int	pbRprSpanOutOamFramesLow;   
	unsigned int	pbRprSpanErrHecFramesHigh;   
	unsigned int	pbRprSpanErrHecFramesLow;   
	unsigned int	pbRprSpanErrParityFramesHigh;   
	unsigned int	pbRprSpanErrParityFramesLow;   
	unsigned int	pbRprSpanErrFcsFramesHigh;   
	unsigned int	pbRprSpanErrFcsFramesLow;   
	unsigned int	pbRprSpanErrEdgeFramesHigh;   
	unsigned int	pbRprSpanErrEdgeFramesLow;   
	unsigned int	pbRprSpanErrSelfSourceFramesHigh;   
	unsigned int	pbRprSpanErrSelfSourceFramesLow;   
	unsigned int	pbRprSpanInUcastClassAFramesHigh;   
	unsigned int	pbRprSpanInUcastClassAFramesLow;   
	unsigned int	pbRprSpanInUcastClassAOctetsHigh;   
	unsigned int	pbRprSpanInUcastClassAOctetsLow;   
	unsigned int	pbRprSpanInUcastClassBCirFramesHigh;   
	unsigned int	pbRprSpanInUcastClassBCirFramesLow;   
	unsigned int	pbRprSpanInUcastClassBCirOctetsHigh;   
	unsigned int	pbRprSpanInUcastClassBCirOctetsLow;   
	unsigned int	pbRprSpanInUcastClassBEirFramesHigh;   
	unsigned int	pbRprSpanInUcastClassBEirFramesLow;   
	unsigned int	pbRprSpanInUcastClassBEirOctetsHigh;   
	unsigned int	pbRprSpanInUcastClassBEirOctetsLow;   
	unsigned int	pbRprSpanInUcastClassCFramesHigh;   
	unsigned int	pbRprSpanInUcastClassCFramesLow;   
	unsigned int	pbRprSpanInUcastClassCOctetsHigh;   
	unsigned int	pbRprSpanInUcastClassCOctetsLow;   
	unsigned int	pbRprSpanInMcastClassAFramesHigh;   
	unsigned int	pbRprSpanInMcastClassAFramesLow;   
	unsigned int	pbRprSpanInMcastClassAOctetsHigh;   
	unsigned int	pbRprSpanInMcastClassAOctetsLow;   
	unsigned int	pbRprSpanInMcastClassBCirFramesHigh;   
	unsigned int	pbRprSpanInMcastClassBCirFramesLow;   
	unsigned int	pbRprSpanInMcastClassBCirOctetsHigh;   
	unsigned int	pbRprSpanInMcastClassBCirOctetsLow;   
	unsigned int	pbRprSpanInMcastClassBEirFramesHigh;   
	unsigned int	pbRprSpanInMcastClassBEirFramesLow;   
	unsigned int	pbRprSpanInMcastClassBEirOctetsHigh;   
	unsigned int	pbRprSpanInMcastClassBEirOctetsLow;   
	unsigned int	pbRprSpanInMcastClassCFramesHigh;   
	unsigned int	pbRprSpanInMcastClassCFramesLow;   
	unsigned int	pbRprSpanInMcastClassCOctetsHigh;   
	unsigned int	pbRprSpanInMcastClassCOctetsLow;   
	unsigned int	pbRprSpanOutUcastClassAFramesHigh;   
	unsigned int	pbRprSpanOutUcastClassAFramesLow;   
	unsigned int	pbRprSpanOutUcastClassAOctetsHigh;   
	unsigned int	pbRprSpanOutUcastClassAOctetsLow;   
	unsigned int	pbRprSpanOutUcastClassBCirFramesHigh;   
	unsigned int	pbRprSpanOutUcastClassBCirFramesLow;   
	unsigned int	pbRprSpanOutUcastClassBCirOctetsHigh;   
	unsigned int	pbRprSpanOutUcastClassBCirOctetsLow;   
	unsigned int	pbRprSpanOutUcastClassBEirFramesHigh;   
	unsigned int	pbRprSpanOutUcastClassBEirFramesLow;   
	unsigned int	pbRprSpanOutUcastClassBEirOctetsHigh;   
	unsigned int	pbRprSpanOutUcastClassBEirOctetsLow;   
	unsigned int	pbRprSpanOutUcastClassCFramesHigh;   
	unsigned int	pbRprSpanOutUcastClassCFramesLow;   
	unsigned int	pbRprSpanOutUcastClassCOctetsHigh;   
	unsigned int	pbRprSpanOutUcastClassCOctetsLow;   
	unsigned int	pbRprSpanOutMcastClassAFramesHigh;   
	unsigned int	pbRprSpanOutMcastClassAFramesLow;   
	unsigned int	pbRprSpanOutMcastClassAOctetsHigh;   
	unsigned int	pbRprSpanOutMcastClassAOctetsLow;   
	unsigned int	pbRprSpanOutMcastClassBCirFramesHigh;   
	unsigned int	pbRprSpanOutMcastClassBCirFramesLow;   
	unsigned int	pbRprSpanOutMcastClassBCirOctetsHigh;   
	unsigned int	pbRprSpanOutMcastClassBCirOctetsLow;   
	unsigned int	pbRprSpanOutMcastClassBEirFramesHigh;   
	unsigned int	pbRprSpanOutMcastClassBEirFramesLow;   
	unsigned int	pbRprSpanOutMcastClassBEirOctetsHigh;   
	unsigned int	pbRprSpanOutMcastClassBEirOctetsLow;   
	unsigned int	pbRprSpanOutMcastClassCFramesHigh;   
	unsigned int	pbRprSpanOutMcastClassCFramesLow;   
	unsigned int	pbRprSpanOutMcastClassCOctetsHigh;   
	unsigned int	pbRprSpanOutMcastClassCOctetsLow;   
}MIBpbRprSpanCurPMTable_t;


















































































































 



 
 
extern MIBDid_t pbPdhCurSlipTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                             
	PbPortType_Tval pbPortType;                                     
	int		pbPortNo;                                                  
	PbPmTableType_Tval pbSlipTableType;                                
	PbPmMonitor_Tval pbSlipMonitor;   
	unsigned int	pbPSlipCounter;   
	TruthValue_Tval pbPSlipThresholdEnable;   
	int		pbPSlipThreshold;   
	unsigned int	pbNSlipCounter;   
	TruthValue_Tval pbNSlipThresholdEnable;   
	int		pbNSlipThreshold;   
	TruthValue_Tval pbSlipTableReset;   
	char		pbSlipTimeStart[16];   
	unsigned int	pbSlipTimeElapse;   
	TruthValue_Tval pbSlipSuspect;   
}MIBpbPdhCurSlipTable_t;






















 



 
 
extern MIBDid_t pbPmMonitoredNumberTable_Did;

   
typedef struct
{
	unsigned int	pbSdhFtpCmndHostIP;                                
	int		pbPmMonitoredAll;   
	int		pbPmMonitoredCtp;   
	int		pbPmMonitoredTtp;   
	int		pbPmMonitoredEth;   
	int		pbPmMonitoredRprClient;   
	int		pbPmMonitoredRprSpan;   
	int		pbPmMonitoredAtm;   
	int		pbPmMonitoredPdhSlip;   
	int		pbPmMonitoredPdhCrc;   
	int		pbPmMonitoredFec;   
	int		pbPmMonitoredOtu;
}MIBpbPmMonitoredNumberTable_t;



















 



 
 
extern MIBDid_t pbxSecEthPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	int		pbPortNo;                                                    
	EthPortClass_Tval pbxSecEthCurPmPortType;                         
	RowStatus_Tval pbxSecEthPmEnableStatus;   
	char		pbxSecEthPmTimeStart[16];   
	TruthValue_Tval pbxSecEthPmStartFlag;   
}MIBpbxSecEthPMTable_t;













 



 
 
extern MIBDid_t pbPdhCurPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                     
	PbPortType_Tval pbPortType;                                             
	int		pbPortNo;                                                          
	PbPmTableType_Tval pbPdhTableType;                                         
	PbPmMonitor_Tval pbPdhPmMonitor;   
	unsigned int	pbPdhEsPmCounter;   
	TruthValue_Tval pbPdhEsPmThresholdEnable;   
	int		pbPdhEsPmThresholdLow;   
	int		pbPdhEsPmThresholdHigh;   
	unsigned int	pbPdhBbePmCounter;   
	TruthValue_Tval pbPdhBbePmThresholdEnable;   
	int		pbPdhBbePmThresholdLow;   
	int		pbPdhBbePmThresholdHigh;   
	unsigned int	pbPdhSesPmCounter;   
	TruthValue_Tval pbPdhSesPmThresholdEnable;   
	int		pbPdhSesPmThresholdLow;   
	int		pbPdhSesPmThresholdHigh;   
	unsigned int	pbPdhUasPmCounter;   
	TruthValue_Tval pbPdhUasPmThresholdEnable;   
	int		pbPdhUasPmThresholdLow;   
	int		pbPdhUasPmThresholdHigh;   
	unsigned int	pbPdhCsesPmCounter;   
	TruthValue_Tval pbPdhCsesPmThresholdEnable;   
	int		pbPdhCsesPmThresholdLow;   
	int		pbPdhCsesPmThresholdHigh;   
	unsigned int	pbPdhFcPmCounter;   
	TruthValue_Tval pbPdhFcPmThresholdEnable;   
	int		pbPdhFcPmThresholdLow;   
	int		pbPdhFcPmThresholdHigh;   
	TruthValue_Tval pbPdhPmTableReset;   
	char		pbPdhPmTimeStart[16];   
	unsigned int	pbPdhPmTimeElapse;   
	TruthValue_Tval pbPdhPmSuspect;   
}MIBpbPdhCurPMTable_t;








































 



 
 
extern MIBDid_t pbEthVlanPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                           
	int		pbPortNo;                                                                
	PbPmTableType_Tval pbEthVlanTableType;                                           
	EthPortClass_Tval pbEthPmPortType;                                            
	int		pbEthVlanNo;                                                             
	PbPmMonitor_Tval pbEthVlanPmMonitor;   
	char		pbEthVlanPmTimeStart[16];   
	unsigned int	pbEthVlanPmTimeElapse;   
	TruthValue_Tval pbEthVlanPmTimeSuspect;   
	TruthValue_Tval pbEthVlanPmTableReset;   
	unsigned int	pbEthVlanInPktsHigh;   
	unsigned int	pbEthVlanInPktsLow;   
	unsigned int	pbEthVlanInBytesHigh;   
	unsigned int	pbEthVlanInBytesLow;   
	unsigned int	pbEthVlanDropPktsHigh;   
	unsigned int	pbEthVlanDropPktsLow;   
	unsigned int	pbEthVlanRcvUnicastFramesHigh;   
	unsigned int	pbEthVlanRcvUnicastFramesLow;   
	unsigned int	pbEthVlanRcvTotalFramesHigh;   
	unsigned int	pbEthVlanRcvTotalFramesLow;   
	unsigned int	pbEthVlanRcvUnicastBytesHigh;   
	unsigned int	pbEthVlanRcvUnicastBytesLow;   
	unsigned int	pbEthVlanRcvTotalBytesHigh;   
	unsigned int	pbEthVlanRcvTotalBytesLow;   
	RowStatus_Tval pbEthVlanPmEnableStatus;   
}MIBpbEthVlanPMTable_t;
































 



 
 
extern MIBDid_t pbxSecEthVlanPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                       
	int		pbPortNo;                                                            
	EthPortClass_Tval pbEthPmPortType;                                        
	int		pbEthVlanNo;                                                         
	RowStatus_Tval pbxSecEthVlanPmEnableStatus;   
	char		pbxSecEthVlanPmTimeStart[16];   
	TruthValue_Tval pbxSecEthVlanPmStartFlag;   
}MIBpbxSecEthVlanPMTable_t;














 



 
 
extern MIBDid_t pbEthTcaPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                 
	int		pbPortNo;                                                      
	EthPortClass_Tval pbEthPmPortType;                                  
	TruthValue_Tval pbEthPmBbrEnable;   
	int		pbEthPmBbrThresholdValue;   
	TruthValue_Tval pbEthPmBprEnable;   
	int		pbEthPmBprThresholdValue;   
}MIBpbEthTcaPMTable_t;














 



 
 
extern MIBDid_t pbFecCurPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	PbPortType_Tval pbPortType;                                       
	int		pbPortNo;                                                    
	PbPmTableType_Tval pbFecTableType;                                   
	PbPmMonitor_Tval pbFecPmMonitor;   
	unsigned int	pbFecTecPmCounterHigh;   
	unsigned int	pbFecTecPmCounterLow;   
	unsigned int	pbFecZecPmCounterHigh;   
	unsigned int	pbFecZecPmCounterLow;   
	unsigned int	pbFecOecPmCounterHigh;   
	unsigned int	pbFecOecPmCounterLow;   
	unsigned int	pbFecFcPmCounter;
	TruthValue_Tval pbFecPmTableReset;   
	char		pbFecPmTimeStart[16];   
	unsigned int	pbFecPmTimeElapse;   
	TruthValue_Tval pbFecPmSuspect;   
	unsigned int	pbFecUncorrectedBlkHigh;
	unsigned int	pbFecUncorrectedBlkLow;
}MIBpbFecCurPMTable_t;

























 



 
 
extern MIBDid_t pbOtuCurPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	PbPortType_Tval pbPortType;                               
	int		pbPortNo;                                            
	PbPmTableType_Tval pbOtuTableType;                        
	unsigned int	pbChannelID;                                 
	unsigned int	pbSegmentNum;                                
	PbPmMonitor_Tval pbOtuPmMonitor;
	unsigned int	pbOtuEsPmCounter;
	unsigned int	pbOtuBbePmCounter;
	unsigned int	pbOtuSesPmCounter;
	unsigned int	pbOtuUasPmCounter;
	unsigned int	pbOtuCsesPmCounter;
	unsigned int	pbOtuFcPmCounter;
	unsigned int	pbOtuOfsPmCounter;
	unsigned int	pbOtuIaesPmCounter;
	unsigned int	pbOtuBiaesPmCounter;
	TruthValue_Tval pbOtuPmTableReset;
	char		pbOtuPmTimeStart[16];
	unsigned int	pbOtuPmTimeElapse;
	TruthValue_Tval pbOtuPmSuspect;
}MIBpbOtuCurPMTable_t;



























 



 
 
extern MIBDid_t pbSdhRecentPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                           
	PbPortType_Tval pbPortType;                                   
	int		pbPortNo;                                                
	PbTPType_Tval pbTpType;                                       
	int		pbTpIndex;                                               
	PbPmTableType_Tval pbRecentTableType;                            
	int		pbRecentTimesIndex;                                         
	unsigned int	pbRecentEsPmCounter;   
	unsigned int	pbRecentBbePmCounter;   
	unsigned int	pbRecentSesPmCounter;   
	unsigned int	pbRecentUasPmCounter;   
	unsigned int	pbRecentCsesPmCounter;   
	unsigned int	pbRecentFcPmCounter;   
	unsigned int	pbRecentOfsPmCounter;   
	unsigned int	pbRecentPpjcPmCounter;   
	unsigned int	pbRecentNpjcPmCounter;   
	char		pbRecentPmTimeEnd[16];   
	TruthValue_Tval pbSuspect;   
	TruthValue_Tval pbComplete;   
}MIBpbSdhRecentPMTable_t;


























 



 
 
extern MIBDid_t pbEthRecentPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                   
	int		pbPortNo;                                                                        
	PbPmTableType_Tval pbEthRecentTableType;                                                 
	EthPortClass_Tval pbEthRecentPmPortType;                                                 
	int		pbEthRecentTimesIndex;                                                              
	char		pbEthRecentPmTimeEnd[16];   
	TruthValue_Tval pbEthRecentPmTimeSuspect;   
	unsigned int	pbEthRecentInUnicastPktsHigh;   
	unsigned int	pbEthRecentInUnicastPktsLow;   
	unsigned int	pbEthRecentInMulcastPktsHigh;   
	unsigned int	pbEthRecentInMulcastPktsLow;   
	unsigned int	pbEthRecentInBrdcastPktsHigh;   
	unsigned int	pbEthRecentInBrdcastPktsLow;   
	unsigned int	pbEthRecentInAllBadPktsHigh;   
	unsigned int	pbEthRecentInAllBadPktsLow;   
	unsigned int	pbEthRecentInAllBadBytesHigh;   
	unsigned int	pbEthRecentInAllBadBytesLow;   
	unsigned int	pbEthRecentInAllGoodPktsHigh;   
	unsigned int	pbEthRecentInAllGoodPktsLow;   
	unsigned int	pbEthRecentInAllGoodBytesHigh;   
	unsigned int	pbEthRecentInAllGoodBytesLow;   
	unsigned int	pbEthRecentInOverSizePktsHigh;   
	unsigned int	pbEthRecentInOverSizePktsLow;   
	unsigned int	pbEthRecentInUnderSizePktsHigh;   
	unsigned int	pbEthRecentInUnderSizePktsLow;   
	unsigned int	pbEthRecentInCRCErrPktsHigh;   
	unsigned int	pbEthRecentInCRCErrPktsLow;   
	unsigned int	pbEthRecentOutUnicastPktsHigh;   
	unsigned int	pbEthRecentOutUnicastPktsLow;   
	unsigned int	pbEthRecentOutMulcastPktsHigh;   
	unsigned int	pbEthRecentOutMulcastPktsLow;   
	unsigned int	pbEthRecentOutBrdcastPktsHigh;   
	unsigned int	pbEthRecentOutBrdcastPktsLow;   
	unsigned int	pbEthRecentOutAllGoodPktsHigh;   
	unsigned int	pbEthRecentOutAllGoodPktsLow;   
	unsigned int	pbEthRecentOutAllGoodBytesHigh;   
	unsigned int	pbEthRecentOutAllGoodBytesLow;   
	unsigned int	pbEthRcntInAllBytesHigh;   
	unsigned int	pbEthRcntInAllBytesLow;   
	unsigned int	pbEthRcntInAllPktsHigh;   
	unsigned int	pbEthRcntInAllPktsLow;   
	unsigned int	pbEthRcntInAllGoodPktsHigh;   
	unsigned int	pbEthRcntInAllGoodPktsLow;   
	unsigned int	pbEthRcntInAllBadPktsHigh;   
	unsigned int	pbEthRcntInAllBadPktsLow;   
	unsigned int	pbEthRcntInUnicastPktsHigh;   
	unsigned int	pbEthRcntInUnicastPktsLow;   
	unsigned int	pbEthRcntInMulcastPktsHigh;   
	unsigned int	pbEthRcntInMulcastPktsLow;   
	unsigned int	pbEthRcntInBrdcastPktsHigh;   
	unsigned int	pbEthRcntInBrdcastPktsLow;   
	unsigned int	pbEthRcntInDropPktsHigh;   
	unsigned int	pbEthRcntInDropPktsLow;   
	unsigned int	pbEthRcntInUndersizeGoodPktsHigh;   
	unsigned int	pbEthRcntInUndersizeGoodPktsLow;   
	unsigned int	pbEthRcntInOversizeGoodPktsHigh;   
	unsigned int	pbEthRcntInOversizeGoodPktsLow;   
	unsigned int	pbEthRcntIn64PktsHigh;   
	unsigned int	pbEthRcntIn64PktsLow;   
	unsigned int	pbEthRcntIn65To127PktsHigh;   
	unsigned int	pbEthRcntIn65To127PktsLow;   
	unsigned int	pbEthRcntIn128To255PktsHigh;   
	unsigned int	pbEthRcntIn128To255PktsLow;   
	unsigned int	pbEthRcntIn256To511PktsHigh;   
	unsigned int	pbEthRcntIn256To511PktsLow;   
	unsigned int	pbEthRcntIn512To1023PktsHigh;   
	unsigned int	pbEthRcntIn512To1023PktsLow;   
	unsigned int	pbEthRcntIn1024To1518PktsHigh;   
	unsigned int	pbEthRcntIn1024To1518PktsLow;   
	unsigned int	pbEthRcntInPausePktsHigh;   
	unsigned int	pbEthRcntInPausePktsLow;   
	unsigned int	pbEthRcntInFragmentPktsHigh;   
	unsigned int	pbEthRcntInFragmentPktsLow;   
	unsigned int	pbEthRcntInJabberPktsHigh;   
	unsigned int	pbEthRcntInJabberPktsLow;   
	unsigned int	pbEthRcntInAlignmentErrorPktsHigh;   
	unsigned int	pbEthRcntInAlignmentErrorPktsLow;   
	unsigned int	pbEthRcntOutAllBytesHigh;   
	unsigned int	pbEthRcntOutAllBytesLow;   
	unsigned int	pbEthRcntOutAllPktsHigh;   
	unsigned int	pbEthRcntOutAllPktsLow;   
	unsigned int	pbEthRcntOutUnicastPktsHigh;   
	unsigned int	pbEthRcntOutUnicastPktsLow;   
	unsigned int	pbEthRcntOutMulcastPktsHigh;   
	unsigned int	pbEthRcntOutMulcastPktsLow;   
	unsigned int	pbEthRcntOutBrdcastPktsHigh;   
	unsigned int	pbEthRcntOutBrdcastPktsLow;   
	unsigned int	pbEthRcntOutGoodPausePktsHigh;   
	unsigned int	pbEthRcntOutGoodPausePktsLow;   
	unsigned int	pbEthRcntOutCollisionPktsHigh;   
	unsigned int	pbEthRcntOutCollisionPktsLow;   
}MIBpbEthRecentPMTable_t;


































































































 



 
 
extern MIBDid_t pbRprClientRecentPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                                         
	int		rprIfIndex;                                                                                            
	PbPmTableType_Tval pbRprClientRecentTableType;                                                                 
	int		pbRprClientTableRecentTimesIndex;                                                                         
	char		pbRprClientRecentPmTimeEnd[16];   
	TruthValue_Tval pbRprClientRecentPmTimeSuspect;   
	unsigned int	pbRprClientRecentInAllFramesHigh;   
	unsigned int	pbRprClientRecentInAllFramesLow;   
	unsigned int	pbRprClientRecentOutAllFramesHigh;   
	unsigned int	pbRprClientRecentOutAllFramesLow;   
	unsigned int	pbRprClientRecentOutPassFramesHigh;   
	unsigned int	pbRprClientRecentOutPassFramesLow;   
	unsigned int	pbRprClientRecentOutDropFramesHigh;   
	unsigned int	pbRprClientRecentOutDropFramesLow;   
	unsigned int	pbRprClientRecentOutErrLenFramesHigh;   
	unsigned int	pbRprClientRecentOutErrLenFramesLow;   
	unsigned int	pbRprClientRecentOutErrCrcFramesHigh;   
	unsigned int	pbRprClientRecentOutErrCrcFramesLow;   
	unsigned int	pbRprClientRecentInUcastClassAFramesHigh;   
	unsigned int	pbRprClientRecentInUcastClassAFramesLow;   
	unsigned int	pbRprClientRecentInUcastClassAOctetsHigh;   
	unsigned int	pbRprClientRecentInUcastClassAOctetsLow;   
	unsigned int	pbRprClientRecentInUcastClassBCirFramesHigh;   
	unsigned int	pbRprClientRecentInUcastClassBCirFramesLow;   
	unsigned int	pbRprClientRecentInUcastClassBCirOctetsHigh;   
	unsigned int	pbRprClientRecentInUcastClassBCirOctetsLow;   
	unsigned int	pbRprClientRecentInUcastClassBEirFramesHigh;   
	unsigned int	pbRprClientRecentInUcastClassBEirFramesLow;   
	unsigned int	pbRprClientRecentInUcastClassBEirOctetsHigh;   
	unsigned int	pbRprClientRecentInUcastClassBEirOctetsLow;   
	unsigned int	pbRprClientRecentInUcastClassCFramesHigh;   
	unsigned int	pbRprClientRecentInUcastClassCFramesLow;   
	unsigned int	pbRprClientRecentInUcastClassCOctetsHigh;   
	unsigned int	pbRprClientRecentInUcastClassCOctetsLow;   
	unsigned int	pbRprClientRecentInMcastClassAFramesHigh;   
	unsigned int	pbRprClientRecentInMcastClassAFramesLow;   
	unsigned int	pbRprClientRecentInMcastClassAOctetsHigh;   
	unsigned int	pbRprClientRecentInMcastClassAOctetsLow;   
	unsigned int	pbRprClientRecentInMcastClassBCirFramesHigh;   
	unsigned int	pbRprClientRecentInMcastClassBCirFramesLow;   
	unsigned int	pbRprClientRecentInMcastClassBCirOctetsHigh;   
	unsigned int	pbRprClientRecentInMcastClassBCirOctetsLow;   
	unsigned int	pbRprClientRecentInMcastClassBEirFramesHigh;   
	unsigned int	pbRprClientRecentInMcastClassBEirFramesLow;   
	unsigned int	pbRprClientRecentInMcastClassBEirOctetsHigh;   
	unsigned int	pbRprClientRecentInMcastClassBEirOctetsLow;   
	unsigned int	pbRprClientRecentInMcastClassCFramesHigh;   
	unsigned int	pbRprClientRecentInMcastClassCFramesLow;   
	unsigned int	pbRprClientRecentInMcastClassCOctetsHigh;   
	unsigned int	pbRprClientRecentInMcastClassCOctetsLow;   
	unsigned int	pbRprClientRecentOutUcastClassAFramesHigh;   
	unsigned int	pbRprClientRecentOutUcastClassAFramesLow;   
	unsigned int	pbRprClientRecentOutUcastClassAOctetsHigh;   
	unsigned int	pbRprClientRecentOutUcastClassAOctetsLow;   
	unsigned int	pbRprClientRecentOutUcastClassBCirFramesHigh;   
	unsigned int	pbRprClientRecentOutUcastClassBCirFramesLow;   
	unsigned int	pbRprClientRecentOutUcastClassBCirOctetsHigh;   
	unsigned int	pbRprClientRecentOutUcastClassBCirOctetsLow;   
	unsigned int	pbRprClientRecentOutUcastClassBEirFramesHigh;   
	unsigned int	pbRprClientRecentOutUcastClassBEirFramesLow;   
	unsigned int	pbRprClientRecentOutUcastClassBEirOctetsHigh;   
	unsigned int	pbRprClientRecentOutUcastClassBEirOctetsLow;   
	unsigned int	pbRprClientRecentOutUcastClassCFramesHigh;   
	unsigned int	pbRprClientRecentOutUcastClassCFramesLow;   
	unsigned int	pbRprClientRecentOutUcastClassCOctetsHigh;   
	unsigned int	pbRprClientRecentOutUcastClassCOctetsLow;   
	unsigned int	pbRprClientRecentOutMcastClassAFramesHigh;   
	unsigned int	pbRprClientRecentOutMcastClassAFramesLow;   
	unsigned int	pbRprClientRecentOutMcastClassAOctetsHigh;   
	unsigned int	pbRprClientRecentOutMcastClassAOctetsLow;   
	unsigned int	pbRprClientRecentOutMcastClassBCirFramesHigh;   
	unsigned int	pbRprClientRecentOutMcastClassBCirFramesLow;   
	unsigned int	pbRprClientRecentOutMcastClassBCirOctetsHigh;   
	unsigned int	pbRprClientRecentOutMcastClassBCirOctetsLow;   
	unsigned int	pbRprClientRecentOutMcastClassBEirFramesHigh;   
	unsigned int	pbRprClientRecentOutMcastClassBEirFramesLow;   
	unsigned int	pbRprClientRecentOutMcastClassBEirOctetsHigh;   
	unsigned int	pbRprClientRecentOutMcastClassBEirOctetsLow;   
	unsigned int	pbRprClientRecentOutMcastClassCFramesHigh;   
	unsigned int	pbRprClientRecentOutMcastClassCFramesLow;   
	unsigned int	pbRprClientRecentOutMcastClassCOctetsHigh;   
	unsigned int	pbRprClientRecentOutMcastClassCOctetsLow;   
}MIBpbRprClientRecentPMTable_t;

























































































 



 
 
extern MIBDid_t pbRprSpanRecentPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                                     
	int		rprIfIndex;                                                                                        
	RprSpan_Tval rprSpanId;                                                                                 
	PbPmTableType_Tval pbRprSpanRecentTableType;                                                               
	int		pbRprSpanTableRecentTimesIndex;                                                                       
	char		pbRprSpanRecentPmTimeEnd[16];   
	TruthValue_Tval pbRprSpanRecentPmTimeSuspect;   
	unsigned int	pbRprSpanRecentInAllFramesHigh;   
	unsigned int	pbRprSpanRecentInAllFramesLow;   
	unsigned int	pbRprSpanRecentInWrapFramesHigh;   
	unsigned int	pbRprSpanRecentInWrapFramesLow;   
	unsigned int	pbRprSpanRecentInDataFramesHigh;   
	unsigned int	pbRprSpanRecentInDataFramesLow;   
	unsigned int	pbRprSpanRecentInCtrlFramesHigh;   
	unsigned int	pbRprSpanRecentInCtrlFramesLow;   
	unsigned int	pbRprSpanRecentInFaFramesHigh;   
	unsigned int	pbRprSpanRecentInFaFramesLow;   
	unsigned int	pbRprSpanRecentOutAllFramesHigh;   
	unsigned int	pbRprSpanRecentOutAllFramesLow;   
	unsigned int	pbRprSpanRecentOutAddFramesHigh;   
	unsigned int	pbRprSpanRecentOutAddFramesLow;   
	unsigned int	pbRprSpanRecentOutTransitFramesHigh;   
	unsigned int	pbRprSpanRecentOutTransitFramesLow;   
	unsigned int	pbRprSpanRecentOutWrapFramesHigh;   
	unsigned int	pbRprSpanRecentOutWrapFramesLow;   
	unsigned int	pbRprSpanRecentOutTpFramesHigh;   
	unsigned int	pbRprSpanRecentOutTpFramesLow;   
	unsigned int	pbRprSpanRecentOutFaFramesHigh;   
	unsigned int	pbRprSpanRecentOutFaFramesLow;   
	unsigned int	pbRprSpanRecentOutOamFramesHigh;   
	unsigned int	pbRprSpanRecentOutOamFramesLow;   
	unsigned int	pbRprSpanRecentErrHecFramesHigh;   
	unsigned int	pbRprSpanRecentErrHecFramesLow;   
	unsigned int	pbRprSpanRecentErrParityFramesHigh;   
	unsigned int	pbRprSpanRecentErrParityFramesLow;   
	unsigned int	pbRprSpanRecentErrFcsFramesHigh;   
	unsigned int	pbRprSpanRecentErrFcsFramesLow;   
	unsigned int	pbRprSpanRecentErrEdgeFramesHigh;   
	unsigned int	pbRprSpanRecentErrEdgeFramesLow;   
	unsigned int	pbRprSpanRecentErrSelfSourceFramesHigh;   
	unsigned int	pbRprSpanRecentErrSelfSourceFramesLow;   
	unsigned int	pbRprSpanRecentInUcastClassAFramesHigh;   
	unsigned int	pbRprSpanRecentInUcastClassAFramesLow;   
	unsigned int	pbRprSpanRecentInUcastClassAOctetsHigh;   
	unsigned int	pbRprSpanRecentInUcastClassAOctetsLow;   
	unsigned int	pbRprSpanRecentInUcastClassBCirFramesHigh;   
	unsigned int	pbRprSpanRecentInUcastClassBCirFramesLow;   
	unsigned int	pbRprSpanRecentInUcastClassBCirOctetsHigh;   
	unsigned int	pbRprSpanRecentInUcastClassBCirOctetsLow;   
	unsigned int	pbRprSpanRecentInUcastClassBEirFramesHigh;   
	unsigned int	pbRprSpanRecentInUcastClassBEirFramesLow;   
	unsigned int	pbRprSpanRecentInUcastClassBEirOctetsHigh;   
	unsigned int	pbRprSpanRecentInUcastClassBEirOctetsLow;   
	unsigned int	pbRprSpanRecentInUcastClassCFramesHigh;   
	unsigned int	pbRprSpanRecentInUcastClassCFramesLow;   
	unsigned int	pbRprSpanRecentInUcastClassCOctetsHigh;   
	unsigned int	pbRprSpanRecentInUcastClassCOctetsLow;   
	unsigned int	pbRprSpanRecentInMcastClassAFramesHigh;   
	unsigned int	pbRprSpanRecentInMcastClassAFramesLow;   
	unsigned int	pbRprSpanRecentInMcastClassAOctetsHigh;   
	unsigned int	pbRprSpanRecentInMcastClassAOctetsLow;   
	unsigned int	pbRprSpanRecentInMcastClassBCirFramesHigh;   
	unsigned int	pbRprSpanRecentInMcastClassBCirFramesLow;   
	unsigned int	pbRprSpanRecentInMcastClassBCirOctetsHigh;   
	unsigned int	pbRprSpanRecentInMcastClassBCirOctetsLow;   
	unsigned int	pbRprSpanRecentInMcastClassBEirFramesHigh;   
	unsigned int	pbRprSpanRecentInMcastClassBEirFramesLow;   
	unsigned int	pbRprSpanRecentInMcastClassBEirOctetsHigh;   
	unsigned int	pbRprSpanRecentInMcastClassBEirOctetsLow;   
	unsigned int	pbRprSpanRecentInMcastClassCFramesHigh;   
	unsigned int	pbRprSpanRecentInMcastClassCFramesLow;   
	unsigned int	pbRprSpanRecentInMcastClassCOctetsHigh;   
	unsigned int	pbRprSpanRecentInMcastClassCOctetsLow;   
	unsigned int	pbRprSpanRecentOutUcastClassAFramesHigh;   
	unsigned int	pbRprSpanRecentOutUcastClassAFramesLow;   
	unsigned int	pbRprSpanRecentOutUcastClassAOctetsHigh;   
	unsigned int	pbRprSpanRecentOutUcastClassAOctetsLow;   
	unsigned int	pbRprSpanRecentOutUcastClassBCirFramesHigh;   
	unsigned int	pbRprSpanRecentOutUcastClassBCirFramesLow;   
	unsigned int	pbRprSpanRecentOutUcastClassBCirOctetsHigh;   
	unsigned int	pbRprSpanRecentOutUcastClassBCirOctetsLow;   
	unsigned int	pbRprSpanRecentOutUcastClassBEirFramesHigh;   
	unsigned int	pbRprSpanRecentOutUcastClassBEirFramesLow;   
	unsigned int	pbRprSpanRecentOutUcastClassBEirOctetsHigh;   
	unsigned int	pbRprSpanRecentOutUcastClassBEirOctetsLow;   
	unsigned int	pbRprSpanRecentOutUcastClassCFramesHigh;   
	unsigned int	pbRprSpanRecentOutUcastClassCFramesLow;   
	unsigned int	pbRprSpanRecentOutUcastClassCOctetsHigh;   
	unsigned int	pbRprSpanRecentOutUcastClassCOctetsLow;   
	unsigned int	pbRprSpanRecentOutMcastClassAFramesHigh;   
	unsigned int	pbRprSpanRecentOutMcastClassAFramesLow;   
	unsigned int	pbRprSpanRecentOutMcastClassAOctetsHigh;   
	unsigned int	pbRprSpanRecentOutMcastClassAOctetsLow;   
	unsigned int	pbRprSpanRecentOutMcastClassBCirFramesHigh;   
	unsigned int	pbRprSpanRecentOutMcastClassBCirFramesLow;   
	unsigned int	pbRprSpanRecentOutMcastClassBCirOctetsHigh;   
	unsigned int	pbRprSpanRecentOutMcastClassBCirOctetsLow;   
	unsigned int	pbRprSpanRecentOutMcastClassBEirFramesHigh;   
	unsigned int	pbRprSpanRecentOutMcastClassBEirFramesLow;   
	unsigned int	pbRprSpanRecentOutMcastClassBEirOctetsHigh;   
	unsigned int	pbRprSpanRecentOutMcastClassBEirOctetsLow;   
	unsigned int	pbRprSpanRecentOutMcastClassCFramesHigh;   
	unsigned int	pbRprSpanRecentOutMcastClassCFramesLow;   
	unsigned int	pbRprSpanRecentOutMcastClassCOctetsHigh;   
	unsigned int	pbRprSpanRecentOutMcastClassCOctetsLow;   
}MIBpbRprSpanRecentPMTable_t;
















































































































 



 
 
extern MIBDid_t pbPdhRecentSlipTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                             
	PbPortType_Tval pbPortType;                                     
	int		pbPortNo;                                                  
	PbPmTableType_Tval pbRecentSlipTableType;                          
	int		pbRecentSlipTimesIndex;                                       
	unsigned int	pbRecentPSlipCounter;   
	unsigned int	pbRecentNSlipCounter;   
	char		pbSlipTimeEnd[16];   
	TruthValue_Tval pbRecentSlipSuspect;   
}MIBpbPdhRecentSlipTable_t;
















 



 
 
extern MIBDid_t pbxSecEthRecentPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                   
	int		pbPortNo;                                                                        
	EthPortClass_Tval pbxSecEthRecentPmPortType;                                             
	int		pbxSecEthTimesIndex;                                                                
	TruthValue_Tval pbxSecEthRecentPmStartFlag;   
	unsigned int	pbxSecEthRecentInUnicastPkts;   
	unsigned int	pbxSecEthRecentInMulcastPkts;   
	unsigned int	pbxSecEthRecentInBrdcastPkts;   
	unsigned int	pbxSecEthRecentInAllBadPkts;   
	unsigned int	pbxSecEthRecentInAllBadBytes;   
	unsigned int	pbxSecEthRecentInAllGoodPkts;   
	unsigned int	pbxSecEthRecentInAllGoodBytes;   
	unsigned int	pbxSecEthRecentInOverSizePkts;   
	unsigned int	pbxSecEthRecentInUnderSizePkts;   
	unsigned int	pbxSecEthRecentInCRCErrPkts;   
	unsigned int	pbxSecEthRecentOutUnicastPkts;   
	unsigned int	pbxSecEthRecentOutMulcastPkts;   
	unsigned int	pbxSecEthRecentOutBrdcastPkts;   
	unsigned int	pbxSecEthRecentOutAllGoodPkts;   
	unsigned int	pbxSecEthRecentOutAllGoodBytes;   
	int		pbxSecEthFrmStartIndex;   
	unsigned int	pbxSecEthRcntInAllBytes;   
	unsigned int	pbxSecEthRcntInAllPkts;   
	unsigned int	pbxSecEthRcntInAllGoodPkts;   
	unsigned int	pbxSecEthRcntInAllBadPkts;   
	unsigned int	pbxSecEthRcntInUnicastPkts;   
	unsigned int	pbxSecEthRcntInMulcastPkts;   
	unsigned int	pbxSecEthRcntInBrdcastPkts;   
	unsigned int	pbxSecEthRcntInDropPkts;   
	unsigned int	pbxSecEthRcntInUndersizeGoodPkts;   
	unsigned int	pbxSecEthRcntInOversizeGoodPkts;   
	unsigned int	pbxSecEthRcntIn64Pkts;   
	unsigned int	pbxSecEthRcntIn65To127Pkts;   
	unsigned int	pbxSecEthRcntIn128To255Pkts;   
	unsigned int	pbxSecEthRcntIn256To511Pkts;   
	unsigned int	pbxSecEthRcntIn512To1023Pkts;   
	unsigned int	pbxSecEthRcntIn1024To1518Pkts;   
	unsigned int	pbxSecEthRcntInPausePkts;   
	unsigned int	pbxSecEthRcntInFragmentPkts;   
	unsigned int	pbxSecEthRcntInJabberPkts;   
	unsigned int	pbxSecEthRcntInAlignmentErrorPkts;   
	unsigned int	pbxSecEthRcntOutAllBytes;   
	unsigned int	pbxSecEthRcntOutAllPkts;   
	unsigned int	pbxSecEthRcntOutUnicastPkts;   
	unsigned int	pbxSecEthRcntOutMulcastPkts;   
	unsigned int	pbxSecEthRcntOutBrdcastPkts;   
	unsigned int	pbxSecEthRcntOutGoodPausePkts;   
	unsigned int	pbxSecEthRcntOutCollisionPkts;   
}MIBpbxSecEthRecentPMTable_t;























































 



 
 
extern MIBDid_t pbPdhRecentPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                 
	PbPortType_Tval pbPortType;                                         
	int		pbPortNo;                                                      
	PbPmTableType_Tval pbPdhRecentTableType;                               
	int		pbPdhRecentTimesIndex;                                            
	unsigned int	pbPdhRecentEsPmCounter;   
	unsigned int	pbPdhRecentBbePmCounter;   
	unsigned int	pbPdhRecentSesPmCounter;   
	unsigned int	pbPdhRecentUasPmCounter;   
	unsigned int	pbPdhRecentCsesPmCounter;   
	unsigned int	pbPdhRecentFcPmCounter;   
	char		pbPdhRecentPmTimeEnd[16];   
	TruthValue_Tval pbPdhSuspect;   
	TruthValue_Tval pbPdhComplete;   
}MIBpbPdhRecentPMTable_t;





















 



 
 
extern MIBDid_t pbEthVlanRecentPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                       
	int		pbPortNo;                                                                            
	PbPmTableType_Tval pbEthVlanRecentTableType;                                                 
	EthPortClass_Tval pbEthPmPortType;                                                        
	int		pbEthVlanNo;                                                                         
	int		pbEthVlanRecentTimesIndex;                                                              
	char		pbEthVlanRecentPmTimeEnd[16];   
	TruthValue_Tval pbEthVlanRecentPmTimeSuspect;   
	unsigned int	pbEthVlanRecentInPktsHigh;   
	unsigned int	pbEthVlanRecentInPktsLow;   
	unsigned int	pbEthVlanRecentInBytesHigh;   
	unsigned int	pbEthVlanRecentInBytesLow;   
	unsigned int	pbEthVlanRecentDropPktsHigh;   
	unsigned int	pbEthVlanRecentDropPktsLow;   
	unsigned int	pbEthVlanRecentRcvUnicastFramesHigh;   
	unsigned int	pbEthVlanRecentRcvUnicastFramesLow;   
	unsigned int	pbEthVlanRecentRcvTotalFramesHigh;   
	unsigned int	pbEthVlanRecentRcvTotalFramesLow;   
	unsigned int	pbEthVlanRecentRcvUnicastBytesHigh;   
	unsigned int	pbEthVlanRecentRcvUnicastBytesLow;   
	unsigned int	pbEthVlanRecentRcvTotalBytesHigh;   
	unsigned int	pbEthVlanRecentRcvTotalBytesLow;   
}MIBpbEthVlanRecentPMTable_t;





























 



 
 
extern MIBDid_t pbxSecEthVlanRecentPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                       
	int		pbPortNo;                                                                            
	EthPortClass_Tval pbEthPmPortType;                                                        
	int		pbEthVlanNo;                                                                         
	int		pbxSecEthVlanTimesIndex;                                                                
	TruthValue_Tval pbxSecEthVlanRecentPmStartFlag;   
	unsigned int	pbxSecEthVlanRecentInPkts;   
	unsigned int	pbxSecEthVlanRecentInBytes;   
	unsigned int	pbxSecEthVlanRecentDropPkts;   
	unsigned int	pbxSecEthVlanRecentRcvUnicastFrames;   
	unsigned int	pbxSecEthVlanRecentRcvTotalFrames;   
	unsigned int	pbxSecEthVlanRecentRcvUnicastBytes;   
	unsigned int	pbxSecEthVlanRecentRcvTotalBytes;   
	int		pbxSecEthVlanFrmStartIndex;   
}MIBpbxSecEthVlanRecentPMTable_t;





















 



 
 
extern MIBDid_t pbFecRecentPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                       
	PbPortType_Tval pbPortType;                                               
	int		pbPortNo;                                                            
	PbPmTableType_Tval pbFecRecentTableType;                                     
	int		pbFecRecentTimesIndex;                                                  
	unsigned int	pbFecRecentTecPmCounterHigh;   
	unsigned int	pbFecRecentTecPmCounterLow;   
	unsigned int	pbFecRecentZecPmCounterHigh;   
	unsigned int	pbFecRecentZecPmCounterLow;   
	unsigned int	pbFecRecentOecPmCounterHigh;   
	unsigned int	pbFecRecentOecPmCounterLow;   
	unsigned int	pbFecRecentFcPmCounter;   
	char		pbFecRecentPmTimeEnd[16];   
	TruthValue_Tval pbFecSuspect;   
	TruthValue_Tval pbFecComplete;   
	unsigned int	pbFecRecUncorrectedBlkHigh;
	unsigned int	pbFecRecUncorrectedBlkLow;
}MIBpbFecRecentPMTable_t;
























 



 
 
extern MIBDid_t pbOtuRecentPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                   
	PbPortType_Tval pbPortType;                                           
	int		pbPortNo;                                                        
	unsigned int	pbOtuRecentChannelID;                                    
	unsigned int	pbOtuRecentSegmentNum;                                   
	PbPmTableType_Tval pbOtuRecentTableType;                              
	int		pbOtuRecentTimesIndex;                                           
	unsigned int	pbOtuRecentEsPmCounter;
	unsigned int	pbOtuRecentBbePmCounter;
	unsigned int	pbOtuRecentSesPmCounter;
	unsigned int	pbOtuRecentUasPmCounter;
	unsigned int	pbOtuRecentCsesPmCounter;
	unsigned int	pbOtuRecentFcPmCounter;
	unsigned int	pbOtuRecentOfsPmCounter;
	unsigned int	pbOtuRecentIaesPmCounter;
	unsigned int	pbOtuRecentBiaesPmCounter;
	char		pbOtuRecentPmTimeEnd[16];
	TruthValue_Tval pbOtuSuspect;
	TruthValue_Tval pbOtuComplete;
}MIBpbOtuRecentPMTable_t;


























 



 
 
extern MIBDid_t pbSdhUapPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;               
	PbPortType_Tval pbPortType;                       
	int		pbPortNo;                                    
	PbTPType_Tval pbTpType;                           
	int		pbTpIndex;                                   
	PbPmUapType_Tval pbPmUapType;                        
	TruthValue_Tval pbIsAtUap;   
	unsigned int	pbUAPCounter;   
	char		pbUapTimeStart[16];   
	unsigned int	pbUapTimeElapse;   
	TruthValue_Tval pbUAPReset;   
}MIBpbSdhUapPMTable_t;


















 



 
 
extern MIBDid_t pbPdhUapPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                     
	PbPortType_Tval pbPortType;                             
	int		pbPortNo;                                          
	PbPmUapType_Tval pbPdhUapType;                             
	TruthValue_Tval pbPdhIsAtUap;   
	unsigned int	pbPdhUAPCounter;   
	char		pbPdhUapTimeStart[16];   
	unsigned int	pbPdhUapTimeElapse;   
	TruthValue_Tval pbPdhUAPReset;   
}MIBpbPdhUapPMTable_t;
















 



 
 
extern MIBDid_t pbOtuUapPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                     
	PbPortType_Tval pbPortType;                             
	int		pbPortNo;                                          
	unsigned int	pbUapChannelID;                            
	unsigned int	pbUapSegmentNum;                           
	PbPmUapType_Tval pbOtuUapType;                          
	TruthValue_Tval pbOtuIsAtUap;
	unsigned int	pbOtuUAPCounter;
	char		pbOtuUapTimeStart[16];
	unsigned int	pbOtuUapTimeElapse;
	TruthValue_Tval pbOtuUAPReset;
}MIBpbOtuUapPMTable_t;


















 



 
 
extern MIBDid_t pbSdhRecentUapPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                         
	PbPortType_Tval pbPortType;                                 
	int		pbPortNo;                                              
	PbTPType_Tval pbTpType;                                     
	int		pbTpIndex;                                             
	PbPmUapType_Tval pbPmRecUapType;                               
	int		pbUapTimesIndex;                                          
	char		pbRecentUapTimeStart[16];   
	char		pbRecentUapTimeEnd[16];   
}MIBpbSdhRecentUapPMTable_t;
















 



 
 
extern MIBDid_t pbPdhRecentUapPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	PbPortType_Tval pbPortType;                                       
	int		pbPortNo;                                                    
	PbPmUapType_Tval pbPdhRecUapType;                                    
	int		pbPdhUapTimesIndex;                                             
	char		pbPdhRecentUapTimeStart[16];   
	char		pbPdhRecentUapTimeEnd[16];   
}MIBpbPdhRecentUapPMTable_t;














 



 
 
extern MIBDid_t pbOtuRecentUapPMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	PbPortType_Tval pbPortType;                                       
	int		pbPortNo;                                                    
	unsigned int	pbOtuRecUapChannelID;                                
	unsigned int	pbOtuRecUapSegmentNum;                               
	PbPmUapType_Tval pbOtuRecUapType;                                 
	int		pbOtuUapTimesIndex;                                          
	char		pbOtuRecentUapTimeStart[16];
	char		pbOtuRecentUapTimeEnd[16];
}MIBpbOtuRecentUapPMTable_t;
















 



 
 
extern MIBDid_t pbSdhMostRecentIndexTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                     
	PbPortType_Tval pbPortType;                                             
	int		pbPortNo;                                                          
	PbTPType_Tval pbTpType;                                                 
	int		pbTpIndex;                                                         
	PbPmUapType_Tval pbPmRecIntervalTblType;                                   
	int		pbRecentIntervalTblMRI;   
	TruthValue_Tval pbRecentIntervalUpMaxIndex;   
	int		pbRecentUapTblMRI;   
	TruthValue_Tval pbRecentUapUpMaxIndex;   
}MIBpbSdhMostRecentIndexTable_t;

















 



 
 
extern MIBDid_t pbEthMostRecentIndexTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                           
	int		pbPortNo;                                                                
	PbPmTableType_Tval pbEthIndexTableType;                                          
	EthPortClass_Tval pbEthIndexPmPortType;                                          
	int		pbEthRecentIntervalTblMRI;   
	TruthValue_Tval pbEthRecentIntervalUpMaxIndex;   
}MIBpbEthMostRecentIndexTable_t;













 



 
 
extern MIBDid_t pbRprClientMostRecentIndexTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                       
	int		rprIfIndex;                                                                             
	PbPmTableType_Tval pbRprClientIndexTableType;                                                
	int		pbRprClientRecentIntervalTblMRI;   
	TruthValue_Tval pbRprClientRecentIntervalUpMaxIndex;   
}MIBpbRprClientMostRecentIndexTable_t;












 



 
 
extern MIBDid_t pbRprSpanMostRecentIndexTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                   
	int		rprIfIndex;                                                                      
	RprSpan_Tval rprSpanId;                                                                  
	PbPmTableType_Tval pbRprSpanIndexTableType;                                              
	int		pbRprSpanRecentIntervalTblMRI;   
	TruthValue_Tval pbRprSpanRecentIntervalUpMaxIndex;   
}MIBpbRprSpanMostRecentIndexTable_t;













 



 
 
extern MIBDid_t pbSlipMostRecentIndexTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                     
	PbPortType_Tval pbPortType;                             
	int		pbPortNo;                                          
	int		pbSlipRecentTblMRI;   
}MIBpbSlipMostRecentIndexTable_t;











 



 
 
extern MIBDid_t pbxSecEthIndexTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                   
	int		pbPortNo;                                                                        
	EthPortClass_Tval pbxSecEthIndexPortType;                                                
	int		pbxSecEthRecentIntervalTblMRI;   
	TruthValue_Tval pbxSecEthRecentIntervalUpMaxIndex;   
	int		pbxSecEthRecentIndex;   
}MIBpbxSecEthIndexTable_t;













 



 
 
extern MIBDid_t pbPdhMostRecentIndexTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                           
	PbPortType_Tval pbPortType;                                                   
	int		pbPortNo;                                                                
	PbPmUapType_Tval pbPdhRecIntervalTblType;                                        
	int		pbPdhRecentIntervalTblMRI;   
	TruthValue_Tval pbPdhRecentIntervalUpMaxIndex;   
	int		pbPdhRecentUapTblMRI;   
	TruthValue_Tval pbPdhRecentUapUpMaxIndex;   
}MIBpbPdhMostRecentIndexTable_t;















 



 
 
extern MIBDid_t pbEthVlanMostRecentIndexTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                   
	int		pbPortNo;                                                                        
	PbPmTableType_Tval pbEthVlanIndexTableType;                                              
	EthPortClass_Tval pbEthPmPortType;                                                       
	int		pbEthVlanNo;                                                                        
	int		pbEthVlanRecentIntervalTblMRI;   
	TruthValue_Tval pbEthVlanRecentIntervalUpMaxIndex;   
}MIBpbEthVlanMostRecentIndexTable_t;














 



 
 
extern MIBDid_t pbxSecEthVlanIndexTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                                           
	int		pbPortNo;                                                                                
	EthPortClass_Tval pbEthPmPortType;                                                            
	int		pbEthVlanNo;                                                                             
	int		pbxSecEthVlanRecentIntervalTblMRI;   
	TruthValue_Tval pbxSecEthVlanRecentIntervalUpMaxIndex;   
	int		pbxSecEthVlanRecentIndex;   
}MIBpbxSecEthVlanIndexTable_t;














 



 
 
extern MIBDid_t pbFecMostRecentIndexTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                           
	PbPortType_Tval pbPortType;                                                   
	int		pbPortNo;                                                                
	PbPmUapType_Tval pbFecRecIntervalTblType;                                        
	int		pbFecRecentIntervalTblMRI;   
	TruthValue_Tval pbFecRecentIntervalUpMaxIndex;   
}MIBpbFecMostRecentIndexTable_t;













 



 
 
extern MIBDid_t pbOtuMostRecentIndexTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                           
	PbPortType_Tval pbPortType;                                                   
	int		pbPortNo;                                                                
	unsigned int	pbOtuRecChannelID;                                               
	unsigned int	pbOtuRecSegmentNum;                                              
	PbPmUapType_Tval pbOtuRecIntervalTblType;                                     
	int		pbOtuRecentIntervalTblMRI;
	TruthValue_Tval pbOtuRecentIntervalUpMaxIndex;
	int		pbOtuRecentUapTblMRI;
	TruthValue_Tval pbOtuRecentUapUpMaxIndex;
}MIBpbOtuMostRecentIndexTable_t;

















 



 
 
extern MIBDid_t pbSdhPMUpLoadTable_Did;

   
typedef struct
{
	unsigned int	pbSdhFtpCmndHostIP;                                 
	char		pbSdhFtpCmndPath[64];   
	char		pbSdhFtpFileName[13];   
	char		pbSdhFtpCmndUsername[32];   
	char		pbSdhFtpCmndPassword[32];   
	SdhFtpCmndLastState_Tval pbSdhFtpCmndLastState;   
	unsigned int	pbAdjustTimeDircetion;   
	unsigned int	pbAdjustTimeDays;   
	unsigned int	pbAdjustTimeHours;   
	unsigned int	pbAdjustTimeMins;   
	unsigned int	pbAdjustTimeSecs;   
	RowStatus_Tval pbUpLoadStatus;   
}MIBpbSdhPMUpLoadTable_t;



















 



 
 
extern MIBDid_t pbxSecEthUploadTable_Did;

   
typedef struct
{
	unsigned int	pbxSecEthFtpHostIP;                               
	char		pbxSecEthFtpFileName[20];   
	char		pbxSecEthFtpUsername[32];   
	char		pbxSecEthFtpPassword[32];   
}MIBpbxSecEthUploadTable_t;











 



 
extern MIBDid_t pbSonetSDHPMModule_Did;

   



# 28 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_events.h" 1



 




 



 
 
extern MIBDid_t pbActiveAlarmTable_Did;

   
typedef struct
{
	unsigned int	pbActiveAlarmIndex;                                             
	char		pbActiveAlarmCode[255];   
	AlarmSeverity_Tval pbActiveAlarmSeverity;   
	char		pbActiveAlarmSource[255];   
	DateTime	pbActiveAlarmDTS;   
	DateTime	pbActiveAlarmClearDTS;   
	PbSlotNumberType_Tval pbActiveAlarmSlotNum;   
	int		pbActiveAlarmPortNum;   
	PbPortType_Tval pbActiveAlarmPortType;   
	char		pbActiveAlarmDesc[255];   
	AlarmTypeID_Tval pbActiveAlarmTypeID;   
	AlarmCategory_Tval pbActiveAlarmCategory;   
	PbTPType_Tval pbActiveAlarmPathType;   
	int		pbActiveAlarmSeqNum;   
	PbYesNo_Tval pbActiveAlarmAcknowledge;   
	AlarmState_Tval pbActiveAlarmState;   
	PbBoardStyle_Tval pbActiveAlarmBoardStyle;   
	PbPortType_Tval pbActiveAlarmActualPortType;   
}MIBpbActiveAlarmTable_t;

























 



 
 
extern MIBDid_t pbAlarmTypeCfgTable_Did;

   
typedef struct
{
	AlarmTypeID_Tval pbAlarmTypeID;                              
	char		pbAlarmTypeCode[255];   
	char		pbAlarmTypeDesc[255];   
	AlarmCategory_Tval pbAlarmTypeCategory;   
	AlarmSeverity_Tval pbAlarmTypeSeverity;   
	TruthValue_Tval pbAlarmTypeInhibit;
	TruthValue_Tval pbAlarmTypeMDO1;   
	TruthValue_Tval pbAlarmTypeMDO2;   
	int		pbAlarmTypeTrapID;   
	TruthValue_Tval pbAlarmTypeMDO3;   
	TruthValue_Tval pbAlarmTypeMDO4;   
	PbAlmClassType_Tval pbAlmClassID;   
	TruthValue_Tval pbAlarmTypeMDO5;
	TruthValue_Tval pbAlarmTypeMDO6;
	TruthValue_Tval pbAlarmTypeMDO7;
	TruthValue_Tval pbAlarmTypeMDO8;
}MIBpbAlarmTypeCfgTable_t;























 



 
 
extern MIBDid_t pbCurrentEventsTable_Did;

   
typedef struct
{
	int		pbCurrentEventIndex;                                          
	EventTypeID_Tval pbCurrentEventID;   
	DateTime	pbCurrentEventDTS;   
	char		pbCurrentEventName[255];   
	EventCategory_Tval pbCurrentEventCategory;   
	char		pbCurrentEventDetails[255];   
}MIBpbCurrentEventsTable_t;













 



 
 
extern MIBDid_t pbEventTypeCfgTable_Did;

   
typedef struct
{
	EventTypeID_Tval pbEventTypeID;                              
	EventCategory_Tval pbEventTypeCategory;   
	char		pbEventTypeName[255];   
	TruthValue_Tval pbEventTypeSendTrap;   
}MIBpbEventTypeCfgTable_t;











 



 
extern MIBDid_t pbLogSizeGroup_Did;

   
typedef struct
{
	int		pbAlarmLogSize;
	int		pbEventLogSize;
}MIBpbLogSizeGroup_t;







 



 
extern MIBDid_t alarmPersistentTime_Did;

   
typedef struct
{
	int		alarmDetectionPersistentTime;   
	int		alarmClearingPersistentTime;   
}MIBalarmPersistentTime_t;







 



 
extern MIBDid_t logPolicyGroup_Did;

   
typedef enum {
	eventLogPolicy_fifo=1,
	eventLogPolicy_rejectNewEvent=2
} eventLogPolicy_Tval;

typedef struct
{
	eventLogPolicy_Tval eventLogPolicy;   
}MIBlogPolicyGroup_t;






 



 
extern MIBDid_t pbAlarmStatGroup_Did;

   
typedef struct
{
	int		pbNumberOfCriticalAlarms;   
	int		pbNumberOfMajorAlarms;   
	int		pbNumberOfMinorAlarms;   
	int		pbNumberOfWarningAlarms;   
	int		pbNumberOfIndeterminateAlarms;   
	int		pbNumberOfEvents;   
}MIBpbAlarmStatGroup_t;











 



 
extern MIBDid_t pbLogClearGroup_Did;

   
typedef struct
{
	TruthValue_Tval pbClearAlarmLog;   
	TruthValue_Tval pbClearEventsLog;   
}MIBpbLogClearGroup_t;







 



 
extern MIBDid_t pbEventsModule_Did;

   



# 29 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_xc.h" 1



 




 



 
 
extern MIBDid_t pbSdhXcTable_Did;

   
typedef struct
{
	PbXcCapacity_Tval pbXConConnectCapacity;                                         
	PbSlotNumberType_Tval pbXConConnectSrcSlot;                                      
	int		pbXConConnectSrcPort;                                                       
	PbPortType_Tval pbXConConnectSrcPortType;                                     
	int		pbXConConnectSource;                                                        
	PbSlotNumberType_Tval pbXConConnectDstSlot;                                      
	int		pbXConConnectDstPort;                                                       
	PbPortType_Tval pbXConConnectDstPortType;                                     
	int		pbXConConnectDestination;                                                   
	int		pbXConConnectId;   
	char		pbXConConnectCircuitId[64];   
	PbXcStatus_Tval pbXConConnectStatus;   
	PbXcFailReason_Tval pbXConConnectReason;   
	PbXcType_Tval pbXConConnectType;   
	PbCircuitDirection_Tval pbXConConnectCircuitDirection;   
	PbXcAction_Tval pbXConConnectAction;   
	PbXcFrom_Tval pbXConFrom;   
	RowStatus_Tval pbXConRowStatus;   
	char		pbXConConnectReqNo[40];   
	int		pbXCGmplsVcNumber;   
	char		pbXCGmplsCallName[255];   
	PbGmplsCcDirection_Tval pbGmplsCcDirection;   
}MIBpbSdhXcTable_t;





























 



 
extern MIBDid_t pbXCModule_Did;

   



# 30 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_xc_context.h" 1



 




 



 
extern MIBDid_t pbXCExtendObjects_Did;

   


 



 
 
extern MIBDid_t pbSdhXcNewContextTable_Did;

   
typedef enum {
	pbXConContextHOCCAu3Num_none=0,
	pbXConContextHOCCAu3Num_init=1,
	pbXConContextHOCCAu3Num_sys=2,
	pbXConContextHOCCAu3Num_card=4,
	pbXConContextHOCCAu3Num_map=8,
	pbXConContextHOCCAu3Num_poh=16,
	pbXConContextHOCCAu3Num_connect=32,
	pbXConContextHOCCAu3Num_context=64,
	pbXConContextHOCCAu3Num_device=128,
	pbXConContextHOCCAu3Num_ccprot=256,
	pbXConContextHOCCAu3Num_sncp=512,
	pbXConContextHOCCAu3Num_msp=1024,
	pbXConContextHOCCAu3Num_msspr=2048,
	pbXConContextHOCCAu3Num_ep=4096,
	pbXConContextHOCCAu3Num_loccprot=8192,
	pbXConContextHOCCAu3Num_pco=16384,
	pbXConContextHOCCAu3Num_ccs=32768,
	pbXConContextHOCCAu3Num_cca=65536,
	pbXConContextHOCCAu3Num_gmpls=131072,
	pbXConContextHOCCAu3Num_all=1048575
} pbXConContextHOCCAu3Num_Tval;

typedef enum {
	pbXConContextHOCCAu3Step_none=0,
	pbXConContextHOCCAu3Step_message=1,
	pbXConContextHOCCAu3Step_module=2,
	pbXConContextHOCCAu3Step_manager=3,
	pbXConContextHOCCAu3Step_ucsl=4,
	pbXConContextHOCCAu3Step_ldsl=5,
	pbXConContextHOCCAu3Step_pdsl=6,
	pbXConContextHOCCAu3Step_driver=7,
	pbXConContextHOCCAu3Step_system=8,
	pbXConContextHOCCAu3Step_simulator=9
} pbXConContextHOCCAu3Step_Tval;

typedef struct
{
	int		pbXConContextLOCCBusNum;                                         
	int		pbXConContextLOCCAuNum;                                          
	pbXConContextHOCCAu3Num_Tval pbXConContextHOCCAu3Num;
	pbXConContextHOCCAu3Step_Tval pbXConContextHOCCAu3Step;
	int		pbXConContextHOCCAu3Count;
}MIBpbSdhXcNewContextTable_t;












 



 
 
extern MIBDid_t pbSdhXcNewLoMapTable_Did;

   
typedef enum {
	gmplsReservationStatus_unreserved=0,
	gmplsReservationStatus_reserved=1
} gmplsReservationStatus_Tval;

typedef enum {
	gmplsReservationOperate_unreserve=0,
	gmplsReservationOperate_reserve=1
} gmplsReservationOperate_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbXConLoMapLOCCSlot;                        
	PbPortType_Tval pbXConLoMapLOCCPortType;                          
	int		pbXConLoMapLOCCPort;                                         
	int		pbXConLoMapHOCCIndex;
	PbSlotNumberType_Tval pbXConLoMapLCSlot;
	PbPortType_Tval pbXConLoMapLCPortType;
	int		pbXConLoMapLCPort;
	int		pbXConLoMapLCTP;
	gmplsReservationStatus_Tval gmplsReservationStatus;
	gmplsReservationOperate_Tval gmplsReservationOperate;
}MIBpbSdhXcNewLoMapTable_t;

















 



 
extern MIBDid_t pbXcMigrationGroup_Did;

   
typedef enum {
	pbXcMigrationCommand_none=0,
	pbXcMigrationCommand_movePathToGmpls=1,
	pbXcMigrationCommand_confirmPathToGmpls=2,
	pbXcMigrationCommand_abortPathToGmpls=3
} pbXcMigrationCommand_Tval;

typedef enum {
	pbXcMigrationStatus_unknown=0,
	pbXcMigrationStatus_successful=1,
	pbXcMigrationStatus_failed=2
} pbXcMigrationStatus_Tval;

typedef struct
{
	int		pbForwardXcId;
	int		pbReverseXcId;
	pbXcMigrationCommand_Tval pbXcMigrationCommand;
	pbXcMigrationStatus_Tval pbXcMigrationStatus;
}MIBpbXcMigrationGroup_t;









 



 
extern MIBDid_t pbXCContextModule_Did;

   



# 31 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_tms.h" 1



 




 



 
 
extern MIBDid_t pbTMSRefSourceTable_Did;

   
typedef struct
{
	int		pbTMSRefSourceIndex;                                            
	PbSlotNumberType_Tval pbTMSRefSourceSlot;   
	int		pbTMSRefSourcePort;   
	PbYesNo_Tval pbTMSRefSourceMonitored;   
	PbTMSSignalStatus_Tval pbTMSSignalStatus;   
	int		pbTMSRefSourcePriority;   
	PbYesNo_Tval pbTMSRefSourceLockOut;   
	PbTMSQl_Tval pbTMSRefSourceQlIn;   
	PbTMSQl_Tval pbTMSRefSourceQl;   
	PbTMSQLMode_Tval pbTMSQlProvionedMode;   
	PbTMSQl_Tval pbTMSQlProvioned;   
	PbYesNo_Tval pbTMSRefSourceValid;   
	int		pbTMSRefSourceWTR;   
	TMSIfSdhQlType_Tval tMSIfSdhQLType;   
	PbTMSQl_Tval tMSRefSourceQLOut;   
	PbYesNo_Tval tMSForceDnu;   
	PbYesNo_Tval tMSSupportQlOut;   
	PbPortType_Tval tMSPortType;   
	int		tmsRefSourceFDUpband;   
	int		tmsRefSourceFDLowband;   
	int		tmsRefSourceFDValue;   
	int		tmsStoredDrvClkNum;   
}MIBpbTMSRefSourceTable_t;





























 



 
extern MIBDid_t pbTMSSysTimingGroup_Did;

   
typedef enum {
	pbTMSTimingState_locked=1,
	pbTMSTimingState_freerunning=2,
	pbTMSTimingState_holdover=3
} pbTMSTimingState_Tval;

typedef enum {
	pbTMSSysTimingMode_autoselection=1,
	pbTMSSysTimingMode_freerunning=2,
	pbTMSSysTimingMode_holdover=3
} pbTMSSysTimingMode_Tval;

typedef enum {
	pbTMSSwitchRequestStatus_inactive=0,
	pbTMSSwitchRequestStatus_ok=1,
	pbTMSSwitchRequestStatus_other_fail=2,
	pbTMSSwitchRequestStatus_dis_fail=3,
	pbTMSSwitchRequestStatus_lock_fail=4,
	pbTMSSwitchRequestStatus_fs_fail=5,
	pbTMSSwitchRequestStatus_sf_fail=6,
	pbTMSSwitchRequestStatus_ql_dnu=7,
	pbTMSSwitchRequestStatus_ql_low=8
} pbTMSSwitchRequestStatus_Tval;

typedef struct
{
	int		pbTMSSwitchRequest;   
	int		pbTMSActiveTimingSource;   
	pbTMSTimingState_Tval pbTMSTimingState;   
	pbTMSSysTimingMode_Tval pbTMSSysTimingMode;   
	pbTMSSwitchRequestStatus_Tval pbTMSSwitchRequestStatus;   
	DateTime	pbTMSHoldoverDTS;   
	int		pbTMSStaClkOutSource;   
	PbYesNo_Tval pbTMSQlEnabled;   
	PbTMSQl_Tval pbTMSExpectedSMMB;   
	PbYesNo_Tval pbTMSStClkAutoLoopPrevention;   
	int		tmsSysSrcFDThreshold;   
	int		tmsSysFDValue;   
}MIBpbTMSSysTimingGroup_t;

















 



 
 
extern MIBDid_t pbTMSStClkInTable_Did;

   
typedef struct
{
	TmsRefSourceNum_Tval pbTMSStClkInIndex;                          
	PbTMSStClkType_Tval pbTMSStClkInType;   
	PbTMSMonitorMode_Tval pbTMSStClkInMonitored;   
	PbTMSSetSa_Tval pbTMSStClkInSaBit;   
	TMSStClkOhm_Tval tMSStClkInOhm;   
}MIBpbTMSStClkInTable_t;












 



 
 
extern MIBDid_t pbTMSStClkOutTable_Did;

   
typedef struct
{
	TmsRefSourceNum_Tval pbTMSStClkOutIndex;                               
	PbTMSStClkType_Tval pbTMSStClkOutType;   
	PbYesNo_Tval pbTMSStClkOutMonitored;   
	PbTMSSetSa_Tval pbTMSStClkOutSaBit;   
	PbTMSQl_Tval pbTMSStClkOutQlThreshold;   
	PbTMSSquelchMode_Tval pbTMSStClkOutSquelchMode;   
	TMSStClkOhm_Tval tMSStClkOutOhm;   
	PbTMSQl_Tval pbTMSStClkOutQlIn;   
	PbTMSQl_Tval pbTMSStClkOutQl;   
	PbTMSOutActType_Tval tmsStClkOutputMode;   
	PbTMSOutState_Tval pbTMSStClkOutState;   
}MIBpbTMSStClkOutTable_t;


















 



 
extern MIBDid_t pbTMSModule_Did;

   



# 32 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_ethernet.h" 1



 




 



 
 
extern MIBDid_t pbEthGlobalPortTable_Did;

   
typedef enum {
	pbPortTagProtocol_iEEE802=0,
	pbPortTagProtocol_doubleTag=1,
	pbPortTagProtocol_tunneling=2
} pbPortTagProtocol_Tval;

typedef enum {
	pbEthACLType_filter=1,
	pbEthACLType_forward=2
} pbEthACLType_Tval;

typedef enum {
	pbEthBroadcastEnableHol_disable=0,
	pbEthBroadcastEnableHol_enable=1
} pbEthBroadcastEnableHol_Tval;

typedef enum {
	pbEthQoSIEEE802Version_version2004=0,
	pbEthQoSIEEE802Version_version2005=1
} pbEthQoSIEEE802Version_Tval;

typedef enum {
	pbEthChangeCoSMode_onlyCos=0,
	pbEthChangeCoSMode_cosAndPriority=1
} pbEthChangeCoSMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                 
	pbPortTagProtocol_Tval pbPortTagProtocol;   
	int		pbAgingTime;   
	int		pbIPGConfig;   
	int		pbVlanTagProtocolType_len;
	char		pbVlanTagProtocolType[2];   
	int		pbMTUSize;   
	pbEthACLType_Tval pbEthACLType;   
	pbEthBroadcastEnableHol_Tval pbEthBroadcastEnableHol;   
	int		pbEthTunnelingRxTPID_len;
	char		pbEthTunnelingRxTPID[2];   
	pbEthQoSIEEE802Version_Tval pbEthQoSIEEE802Version;   
	pbEthChangeCoSMode_Tval pbEthChangeCoSMode;
	unsigned int	pbEthLabelBase;   
	int		pbEthWredDropProbability;   
}MIBpbEthGlobalPortTable_t;




















 



 
 
extern MIBDid_t pbEthConfigTable_Did;

   
typedef enum {
	pbEthPortType_ge=8,
	pbEthPortType_fe=7,
	pbEthPortType_tenge=25,
	pbEthPortType_fiber_channel=26
} pbEthPortType_Tval;

typedef enum {
	pbEthPortAdminStatus_up=1,
	pbEthPortAdminStatus_down=0
} pbEthPortAdminStatus_Tval;

typedef enum {
	pbPortLinkStatus_up=1,
	pbPortLinkStatus_down=0
} pbPortLinkStatus_Tval;

typedef enum {
	pbEthPortAutoNego_enable=1,
	pbEthPortAutoNego_disable=0
} pbEthPortAutoNego_Tval;

typedef enum {
	pbEthPortFlowControl_enable=1,
	pbEthPortFlowControl_disable=0
} pbEthPortFlowControl_Tval;

typedef enum {
	pbEthPortSpeed_s10M=1,
	pbEthPortSpeed_s100M=2,
	pbEthPortSpeed_s1G=3,
	pbEthPortSpeed_s10G=4,
	pbEthPortSpeed_fc1G=5,
	pbEthPortSpeed_fc2G=6,
	pbEthPortSpeed_fc4G=7
} pbEthPortSpeed_Tval;

typedef enum {
	pbEthPortDuplex_full=1,
	pbEthPortDuplex_half=0
} pbEthPortDuplex_Tval;

typedef enum {
	pbUnTagMode_untag=0,
	pbUnTagMode_tag=1
} pbUnTagMode_Tval;

typedef enum {
	pbEthPortMode_auto=1,
	pbEthPortMode_mon=2,
	pbEthPortMode_nmon=3
} pbEthPortMode_Tval;

typedef enum {
	pbPortDefaultVlanPriority_be=0,
	pbPortDefaultVlanPriority_bk=1,
	pbPortDefaultVlanPriority_sp=2,
	pbPortDefaultVlanPriority_ee=3,
	pbPortDefaultVlanPriority_cl=4,
	pbPortDefaultVlanPriority_vi=5,
	pbPortDefaultVlanPriority_vo=6,
	pbPortDefaultVlanPriority_nc=7
} pbPortDefaultVlanPriority_Tval;

typedef enum {
	pbLanTxPriAlg_wRR=0,
	pbLanTxPriAlg_sP1_WRR3=1,
	pbLanTxPriAlg_sP2_WRR2=2,
	pbLanTxPriAlg_sP=3,
	pbLanTxPriAlg_wFQ=4
} pbLanTxPriAlg_Tval;

typedef enum {
	pbEthPortLoopback_none=0,
	pbEthPortLoopback_incoming=1,
	pbEthPortLoopback_outgoing=2
} pbEthPortLoopback_Tval;

typedef enum {
	pbEthTxEnableHol_disable=0,
	pbEthTxEnableHol_enable=1
} pbEthTxEnableHol_Tval;

typedef enum {
	pbEthAlsMode_no_command=0,
	pbEthAlsMode_manual_restart=1,
	pbEthAlsMode_manual_test=2
} pbEthAlsMode_Tval;

typedef enum {
	ethMDIorMDIX_autoCrossover=0,
	ethMDIorMDIX_mdix=1,
	ethMDIorMDIX_mdi=2
} ethMDIorMDIX_Tval;

typedef enum {
	pbLanChangeCoS_be=0,
	pbLanChangeCoS_bk=1,
	pbLanChangeCoS_sp=2,
	pbLanChangeCoS_ee=3,
	pbLanChangeCoS_cl=4,
	pbLanChangeCoS_vi=5,
	pbLanChangeCoS_vo=6,
	pbLanChangeCoS_nc=7,
	pbLanChangeCoS_not_change=8
} pbLanChangeCoS_Tval;

typedef enum {
	pbLanQinQStatus_disable=0,
	pbLanQinQStatus_enable=1
} pbLanQinQStatus_Tval;

typedef enum {
	pbLanQinQNewVlanPriority_be=0,
	pbLanQinQNewVlanPriority_bk=1,
	pbLanQinQNewVlanPriority_sp=2,
	pbLanQinQNewVlanPriority_ee=3,
	pbLanQinQNewVlanPriority_cl=4,
	pbLanQinQNewVlanPriority_vi=5,
	pbLanQinQNewVlanPriority_vo=6,
	pbLanQinQNewVlanPriority_nc=7
} pbLanQinQNewVlanPriority_Tval;

typedef enum {
	pbEthPortClassifier_non_gmpls=0,
	pbEthPortClassifier_i_nni=1,
	pbEthPortClassifier_e_nni=2,
	pbEthPortClassifier_uni10=3,
	pbEthPortClassifier_uni20=4
} pbEthPortClassifier_Tval;

typedef enum {
	pbEthPortWorkMode_ethernetMode=0,
	pbEthPortWorkMode_mplsMode=1
} pbEthPortWorkMode_Tval;

typedef enum {
	pbSFPWorkMode_fx=0,
	pbSFPWorkMode_t=1,
	pbSFPWorkMode_fx100=2,
	pbSFPWorkMode_xfp10G=3
} pbSFPWorkMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                       
	int		pbPortNo;                                                            
	pbEthPortType_Tval pbEthPortType;   
	pbEthPortAdminStatus_Tval pbEthPortAdminStatus;   
	pbPortLinkStatus_Tval pbPortLinkStatus;   
	pbEthPortAutoNego_Tval pbEthPortAutoNego;   
	pbEthPortFlowControl_Tval pbEthPortFlowControl;   
	pbEthPortSpeed_Tval pbEthPortSpeed;   
	pbEthPortDuplex_Tval pbEthPortDuplex;   
	char		pbEthPortVlan[255];   
	int		pbEthPortPvid;   
	pbUnTagMode_Tval pbUnTagMode;   
	int		pbPortInputRateLimit;   
	pbEthPortMode_Tval pbEthPortMode;   
	int		pbEthPortBroadCastRateLimit;   
	pbPortDefaultVlanPriority_Tval pbPortDefaultVlanPriority;   
	char		pbEthPortLabel[20];   
	pbLanTxPriAlg_Tval pbLanTxPriAlg;   
	char		pbLanTaggedVlan[255];   
	pbEthPortLoopback_Tval pbEthPortLoopback;   
	pbEthTxEnableHol_Tval pbEthTxEnableHol;   
	TruthValue_Tval pbEthAutoLaserShutdown;   
	pbEthAlsMode_Tval pbEthAlsMode;   
	TruthValue_Tval pbEthPortAlarmDisable;   
	TruthValue_Tval ethPortOnCSF;   
	ethMDIorMDIX_Tval ethMDIorMDIX;   
	pbLanChangeCoS_Tval pbLanChangeCoS;   
	pbLanQinQStatus_Tval pbLanQinQStatus;   
	int		pbLanQinQNewVlan;   
	pbLanQinQNewVlanPriority_Tval pbLanQinQNewVlanPriority;   
	int		pbLanTPID_len;
	char		pbLanTPID[2];   
	char		pbLanMirrorPort[8];   
	pbEthPortClassifier_Tval pbEthPortClassifier;
	pbEthPortWorkMode_Tval pbEthPortWorkMode;   
	TruthValue_Tval pbEthEnableWred;   
	pbSFPWorkMode_Tval pbSFPWorkMode;   
}MIBpbEthConfigTable_t;











































 



 
 
extern MIBDid_t pbEthWanConfigTable_Did;

   
typedef enum {
	pbWanDefaultVlanPriority_be=0,
	pbWanDefaultVlanPriority_bk=1,
	pbWanDefaultVlanPriority_sp=2,
	pbWanDefaultVlanPriority_ee=3,
	pbWanDefaultVlanPriority_cl=4,
	pbWanDefaultVlanPriority_vi=5,
	pbWanDefaultVlanPriority_vo=6,
	pbWanDefaultVlanPriority_nc=7
} pbWanDefaultVlanPriority_Tval;

typedef enum {
	pbWanChangeCoS_be=0,
	pbWanChangeCoS_bk=1,
	pbWanChangeCoS_sp=2,
	pbWanChangeCoS_ee=3,
	pbWanChangeCoS_cl=4,
	pbWanChangeCoS_vi=5,
	pbWanChangeCoS_vo=6,
	pbWanChangeCoS_nc=7,
	pbWanChangeCoS_not_change=8
} pbWanChangeCoS_Tval;

typedef enum {
	pbWanTagMode_untag=0,
	pbWanTagMode_tag=1
} pbWanTagMode_Tval;

typedef enum {
	pbWanFlowControl_enable=1,
	pbWanFlowControl_disable=0
} pbWanFlowControl_Tval;

typedef enum {
	pbWanTxPriAlg_wRR=0,
	pbWanTxPriAlg_sP1_WRR3=1,
	pbWanTxPriAlg_sP2_WRR2=2,
	pbWanTxPriAlg_sP=3,
	pbWanTxPriAlg_wFQ=4
} pbWanTxPriAlg_Tval;

typedef enum {
	pbWanTxEnableHol_disable=0,
	pbWanTxEnableHol_enable=1
} pbWanTxEnableHol_Tval;

typedef enum {
	pbWanQinQStatus_disable=0,
	pbWanQinQStatus_enable=1
} pbWanQinQStatus_Tval;

typedef enum {
	pbWanQinQNewVlanPriority_be=0,
	pbWanQinQNewVlanPriority_bk=1,
	pbWanQinQNewVlanPriority_sp=2,
	pbWanQinQNewVlanPriority_ee=3,
	pbWanQinQNewVlanPriority_cl=4,
	pbWanQinQNewVlanPriority_vi=5,
	pbWanQinQNewVlanPriority_vo=6,
	pbWanQinQNewVlanPriority_nc=7
} pbWanQinQNewVlanPriority_Tval;

typedef enum {
	pbWanLinkStatus_down=0,
	pbWanLinkStatus_up=1
} pbWanLinkStatus_Tval;

typedef enum {
	pbWanPortWorkMode_ethernetMode=0,
	pbWanPortWorkMode_mplsMode=1
} pbWanPortWorkMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                 
	int		pbPortNo;                                                      
	char		pbWanLabel[20];   
	char		pbWanVlanList[255];   
	char		pbWanTaggedVlanList[255];   
	pbWanDefaultVlanPriority_Tval pbWanDefaultVlanPriority;   
	pbWanChangeCoS_Tval pbWanChangeCoS;   
	int		pbWanPvid;   
	pbWanTagMode_Tval pbWanTagMode;   
	pbWanFlowControl_Tval pbWanFlowControl;   
	pbWanTxPriAlg_Tval pbWanTxPriAlg;   
	pbWanTxEnableHol_Tval pbWanTxEnableHol;   
	pbWanQinQStatus_Tval pbWanQinQStatus;   
	int		pbWanQinQNewVlan;   
	pbWanQinQNewVlanPriority_Tval pbWanQinQNewVlanPriority;   
	int		pbWanTPID_len;
	char		pbWanTPID[2];   
	char		pbWanMirrorPort[8];   
	pbWanLinkStatus_Tval pbWanLinkStatus;   
	pbWanPortWorkMode_Tval pbWanPortWorkMode;   
	TruthValue_Tval pbWanEnableWred;   
}MIBpbEthWanConfigTable_t;



























 



 
 
extern MIBDid_t pbEthStaticMACTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	char		pbMACEntryMacAddr[20];                                 
	int		pbMACEntryVid;                                          
	char		pbMACEntryPort[8];   
	TruthValue_Tval pbMACEntryTagged;   
	int		pbMACEntryIndex;   
	RowStatus_Tval pbMACEntryRowStatus;   
}MIBpbEthStaticMACTable_t;














 



 
 
extern MIBDid_t pbEthStaticMulticastTable_Did;

   
typedef enum {
	pbStaticMulticastForGmrp_notForGmrp=0,
	pbStaticMulticastForGmrp_forGmrp=1
} pbStaticMulticastForGmrp_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                     
	char		pbStaticMulticastMacAddr[20];                                        
	int		pbStaticMulticastVlan;                                                
	char		pbStaticMulticastPortList[64];   
	int		pbStaticMulticastIndex;   
	RowStatus_Tval pbStaticMulticastRowStatus;   
	pbStaticMulticastForGmrp_Tval pbStaticMulticastForGmrp;   
}MIBpbEthStaticMulticastTable_t;














 



 
 
extern MIBDid_t pbVlanPortCoSTable_Did;

   
typedef enum {
	pbEthVlanCoS_be=0,
	pbEthVlanCoS_bk=1,
	pbEthVlanCoS_sp=2,
	pbEthVlanCoS_ee=3,
	pbEthVlanCoS_cl=4,
	pbEthVlanCoS_vi=5,
	pbEthVlanCoS_vo=6,
	pbEthVlanCoS_nc=7,
	pbEthVlanCoS_none=8
} pbEthVlanCoS_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                   
	int		pbPortNo;                                                        
	PbEthPortClass_Tval pbEthPortCls;                                        
	int		pbEthVlan;                                                          
	int		pbEthRateLimitValue;   
	pbEthVlanCoS_Tval pbEthVlanCoS;   
	TruthValue_Tval pbEthVlanDisable;   
	char		pbEthRedirectionPort[8];   
	int		pbVlanPortCoSIndex;   
	RowStatus_Tval pbEthVlanPortCoSRowStatus;   
	int		pbEthVlanPolicingID;   
	int		pbEthVlanColorID;   
}MIBpbVlanPortCoSTable_t;



















 



 
 
extern MIBDid_t pbEthVlanNameTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                     
	int		pbEthNamedVlan;                                                       
	char		pbEthVlanName[30];   
	RowStatus_Tval pbEthVlanRowStatus;   
	TruthValue_Tval pbEthVlanEnableMacLearning;   
}MIBpbEthVlanNameTable_t;












 



 
 
extern MIBDid_t pbPortQueueTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                         
	int		pbPortNo;                                              
	PbEthInterfaceType_Tval pbEthPortStyle;                        
	int		pbEthTxQueNo;                                             
	int		pbEthPortQueueWeight;   
	int		pbEthQueueWredID;   
}MIBpbPortQueueTable_t;













 



 
 
extern MIBDid_t pbEthACLTable_Did;

   
typedef enum {
	pbEthACLItemType_filter=1,
	pbEthACLItemType_forward=2
} pbEthACLItemType_Tval;

typedef enum {
	pbEthACLCoS_be=0,
	pbEthACLCoS_bk=1,
	pbEthACLCoS_sp=2,
	pbEthACLCoS_ee=3,
	pbEthACLCoS_cl=4,
	pbEthACLCoS_vi=5,
	pbEthACLCoS_vo=6,
	pbEthACLCoS_nc=7,
	pbEthACLCoS_none=8
} pbEthACLCoS_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                             
	char		pbEthACLMacAddr[20];                                         
	int		pbEthACLVid;                                                  
	pbEthACLItemType_Tval pbEthACLItemType;   
	int		pbEthACLRateLimit;   
	pbEthACLCoS_Tval pbEthACLCoS;   
	int		pbEthACLIndex;   
	RowStatus_Tval pbEthACLEntryRowStatus;   
}MIBpbEthACLTable_t;















 



 
 
extern MIBDid_t pbEthStatusTable_Did;

   
typedef enum {
	pbEthStatusPortStyle_lan=1,
	pbEthStatusPortStyle_wan=2
} pbEthStatusPortStyle_Tval;

typedef enum {
	pbEthStatusPortType_ge=8,
	pbEthStatusPortType_fe=7,
	pbEthStatusPortType_tenge=25,
	pbEthStatusPortType_fiber_channel=26
} pbEthStatusPortType_Tval;

typedef enum {
	pbEthStatusPortOperStatus_pbPortOperationStatus_up=1,
	pbEthStatusPortOperStatus_pbPortOperationStatus_down=2,
	pbEthStatusPortOperStatus_pbPortOperationStatus_absent=3,
	pbEthStatusPortOperStatus_pbPortOperationStatus_na=4
} pbEthStatusPortOperStatus_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                       
	int		pbPortNo;                                                            
	pbEthStatusPortStyle_Tval pbEthStatusPortStyle;   
	pbEthStatusPortType_Tval pbEthStatusPortType;   
	pbEthStatusPortOperStatus_Tval pbEthStatusPortOperStatus;   
	int		pbEthStatusLaserIBias;   
	int		pbEthStatusLaserRecvPwr;   
	int		pbEthStatusLaserTransPwr;   
	int		pbEthStatusLaserTemp;   
	TruthValue_Tval pbEthStatusCompliance8472;   
	int		pbEthStatusNominalMBps;   
	int		pbEthStatusWavelength;   
	int		pbEthStatusLinkLength9125;   
	int		pbEthStatusLinkLength50125;   
	int		pbEthStatusLinkLength62125;   
	int		pbEthStatusLinkLengthCopper;   
	char		pbEthStatusVendorName[20];   
	char		pbEthStatusVendorPN[20];   
	char		pbEthStatusVendorSN[20];   
	char		pbEthStatusManuDate[16];   
	int		pbEthStatusTransCode;   
	char		pbEthStatusVendorOUI[16];   
	PbConnectorType_Tval pbEthStatusConnectorType;   
	char		pbEthsSAGPartNumber[20];   
	int		pbEthnClLaserTemp;   
	int		pbEthnTECCurrent;   
}MIBpbEthStatusTable_t;

































 



 
 
extern MIBDid_t pbVlanPortQinQTable_Did;

   
typedef enum {
	pbEthQinQNewVlanPriority_be=0,
	pbEthQinQNewVlanPriority_bk=1,
	pbEthQinQNewVlanPriority_sp=2,
	pbEthQinQNewVlanPriority_ee=3,
	pbEthQinQNewVlanPriority_cl=4,
	pbEthQinQNewVlanPriority_vi=5,
	pbEthQinQNewVlanPriority_vo=6,
	pbEthQinQNewVlanPriority_nc=7
} pbEthQinQNewVlanPriority_Tval;

typedef enum {
	pbEthQinQNewVlanPriMode_userDefined=0,
	pbEthQinQNewVlanPriMode_innerTag=1,
	pbEthQinQNewVlanPriMode_ipds=2
} pbEthQinQNewVlanPriMode_Tval;

typedef enum {
	pbEthQinQVlanSwapMode_vlanId=0,
	pbEthQinQVlanSwapMode_tpid=1,
	pbEthQinQVlanSwapMode_both=2
} pbEthQinQVlanSwapMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                     
	int		pbPortNo;                                                          
	PbEthPortClass_Tval pbEthPortCls;                                       
	int		pbEthVlan;                                                         
	int		pbEthQinQNewVlanId;   
	pbEthQinQNewVlanPriority_Tval pbEthQinQNewVlanPriority;   
	int		pbVlanPortQinQIndex;   
	RowStatus_Tval pbEthVlanPortQinQRowStatus;   
	pbEthQinQNewVlanPriMode_Tval pbEthQinQNewVlanPriMode;   
	TruthValue_Tval pbEthQinQVlanSwapEnable;   
	int		pbEthQinQVlanSwapVid;   
	pbEthQinQVlanSwapMode_Tval pbEthQinQVlanSwapMode;   
}MIBpbVlanPortQinQTable_t;



















 



 
 
extern MIBDid_t pbEthProtPktACLTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                 
	char		pbEthACLMacAddr[20];                                          
	char		pbEthProtPktACLPort[64];
	int		pbEthProtPktACLIndex;
	RowStatus_Tval pbEthProtPktACLRowStatus;
}MIBpbEthProtPktACLTable_t;












 



 
 
extern MIBDid_t pbEthReservedTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                             
	int		pbEthReserveIndex;                                         
	char		pbEthReservedCfg[255];
	RowStatus_Tval pbEthReservedRowStatus;
}MIBpbEthReservedTable_t;











 



 
extern MIBDid_t pbIGMPSnooping_Did;

   


 



 
 
extern MIBDid_t pbLagConfigTable_Did;

   
typedef enum {
	pbLagType_lan=0,
	pbLagType_wan=1
} pbLagType_Tval;

typedef enum {
	pbLagFlowControl_disable=0,
	pbLagFlowControl_enable=1
} pbLagFlowControl_Tval;

typedef enum {
	pbLagTagMode_untag=0,
	pbLagTagMode_tag=1
} pbLagTagMode_Tval;

typedef enum {
	pbLagDefaultVlanPriority_be=0,
	pbLagDefaultVlanPriority_bk=1,
	pbLagDefaultVlanPriority_sp=2,
	pbLagDefaultVlanPriority_ee=3,
	pbLagDefaultVlanPriority_cl=4,
	pbLagDefaultVlanPriority_vi=5,
	pbLagDefaultVlanPriority_vo=6,
	pbLagDefaultVlanPriority_nc=7
} pbLagDefaultVlanPriority_Tval;

typedef enum {
	pbLagTxPriAlg_wRR=0,
	pbLagTxPriAlg_sP1_WRR3=1,
	pbLagTxPriAlg_sP2_WRR2=2,
	pbLagTxPriAlg_sP=3,
	pbLagTxPriAlg_wFQ=4
} pbLagTxPriAlg_Tval;

typedef enum {
	pbLagTxEnableHol_disable=0,
	pbLagTxEnableHol_enable=1
} pbLagTxEnableHol_Tval;

typedef enum {
	pbLagChangeCoS_be=0,
	pbLagChangeCoS_bk=1,
	pbLagChangeCoS_sp=2,
	pbLagChangeCoS_ee=3,
	pbLagChangeCoS_cl=4,
	pbLagChangeCoS_vi=5,
	pbLagChangeCoS_vo=6,
	pbLagChangeCoS_nc=7,
	pbLagChangeCoS_not_change=8
} pbLagChangeCoS_Tval;

typedef enum {
	pbLagQinQStatus_disable=0,
	pbLagQinQStatus_enable=1
} pbLagQinQStatus_Tval;

typedef enum {
	pbLagQinQNewVlanPriority_be=0,
	pbLagQinQNewVlanPriority_bk=1,
	pbLagQinQNewVlanPriority_sp=2,
	pbLagQinQNewVlanPriority_ee=3,
	pbLagQinQNewVlanPriority_cl=4,
	pbLagQinQNewVlanPriority_vi=5,
	pbLagQinQNewVlanPriority_vo=6,
	pbLagQinQNewVlanPriority_nc=7
} pbLagQinQNewVlanPriority_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                 
	int		pbLagId;                                                          
	pbLagType_Tval pbLagType;   
	pbLagFlowControl_Tval pbLagFlowControl;   
	int		pbLagPvid;   
	char		pbLagVlan[255];   
	pbLagTagMode_Tval pbLagTagMode;   
	int		pbLagInputRateLimit;   
	int		pbLagBroadCastRateLimit;   
	pbLagDefaultVlanPriority_Tval pbLagDefaultVlanPriority;   
	pbLagTxPriAlg_Tval pbLagTxPriAlg;   
	pbLagTxEnableHol_Tval pbLagTxEnableHol;   
	pbLagChangeCoS_Tval pbLagChangeCoS;   
	pbLagQinQStatus_Tval pbLagQinQStatus;   
	int		pbLagQinQNewVlan;   
	pbLagQinQNewVlanPriority_Tval pbLagQinQNewVlanPriority;   
	char		pbLagPortList[255];   
	TruthValue_Tval pbLagEnableWred;   
}MIBpbLagConfigTable_t;

























 



 
extern MIBDid_t pbLacpConfig_Did;

   


 



 
 
extern MIBDid_t pbEthStaticMplsTable_Did;

   
typedef enum {
	pbEthInPortType_lan=0,
	pbEthInPortType_wan=1
} pbEthInPortType_Tval;

typedef enum {
	pbEthOutPortType_lan=0,
	pbEthOutPortType_wan=1
} pbEthOutPortType_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                   
	unsigned int	pbEthLspLabel;                                              
	char		pbEthInPort[8];   
	pbEthInPortType_Tval pbEthInPortType;   
	char		pbEthPort[8];   
	pbEthOutPortType_Tval pbEthOutPortType;   
	int		pbEthStaticMplsPolicingID;   
	int		pbEthStaticMplsColorID;   
	RowStatus_Tval pbEthStaticMplsStatus;   
	char		pbEthLspName[64];   
}MIBpbEthStaticMplsTable_t;

















 



 
 
extern MIBDid_t pbEthPolicingTable_Did;

   
typedef enum {
	pbEthPolicingMode_rfc2698=0,
	pbEthPolicingMode_mef10_cf0=1,
	pbEthPolicingMode_mef10_cf1=2
} pbEthPolicingMode_Tval;

typedef enum {
	pbEthPolicingColorAware_unaware=0,
	pbEthPolicingColorAware_aware=1
} pbEthPolicingColorAware_Tval;

typedef enum {
	pbEthPolicingDiscardColor_noColor=0,
	pbEthPolicingDiscardColor_red=1,
	pbEthPolicingDiscardColor_yellow=2,
	pbEthPolicingDiscardColor_all=3
} pbEthPolicingDiscardColor_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                   
	int		pbEthPolicingIndex;                                                 
	char		pbEthPolicingName[32];   
	pbEthPolicingMode_Tval pbEthPolicingMode;   
	unsigned int	pbEthPolicingEIRPIR;   
	unsigned int	pbEthPolicingCIR;   
	unsigned int	pbEthPolicingEBSPBS;   
	unsigned int	pbEthPolicingCBS;   
	pbEthPolicingColorAware_Tval pbEthPolicingColorAware;   
	pbEthPolicingDiscardColor_Tval pbEthPolicingDiscardColor;   
	RowStatus_Tval pbEthPolicingRowStatus;   
	int		pbEthPolicingCount;   
}MIBpbEthPolicingTable_t;



















 



 
 
extern MIBDid_t pbEthColorTable_Did;

   
typedef enum {
	pbEthPrio0Color_green=0,
	pbEthPrio0Color_yellow=1,
	pbEthPrio0Color_red=2
} pbEthPrio0Color_Tval;

typedef enum {
	pbEthPrio1Color_green=0,
	pbEthPrio1Color_yellow=1,
	pbEthPrio1Color_red=2
} pbEthPrio1Color_Tval;

typedef enum {
	pbEthPrio2Color_green=0,
	pbEthPrio2Color_yellow=1,
	pbEthPrio2Color_red=2
} pbEthPrio2Color_Tval;

typedef enum {
	pbEthPrio3Color_green=0,
	pbEthPrio3Color_yellow=1,
	pbEthPrio3Color_red=2
} pbEthPrio3Color_Tval;

typedef enum {
	pbEthPrio4Color_green=0,
	pbEthPrio4Color_yellow=1,
	pbEthPrio4Color_red=2
} pbEthPrio4Color_Tval;

typedef enum {
	pbEthPrio5Color_green=0,
	pbEthPrio5Color_yellow=1,
	pbEthPrio5Color_red=2
} pbEthPrio5Color_Tval;

typedef enum {
	pbEthPrio6Color_green=0,
	pbEthPrio6Color_yellow=1,
	pbEthPrio6Color_red=2
} pbEthPrio6Color_Tval;

typedef enum {
	pbEthPrio7Color_green=0,
	pbEthPrio7Color_yellow=1,
	pbEthPrio7Color_red=2
} pbEthPrio7Color_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	int		pbEthColorIndex;                                        
	pbEthPrio0Color_Tval pbEthPrio0Color;   
	pbEthPrio1Color_Tval pbEthPrio1Color;   
	pbEthPrio2Color_Tval pbEthPrio2Color;   
	pbEthPrio3Color_Tval pbEthPrio3Color;   
	pbEthPrio4Color_Tval pbEthPrio4Color;   
	pbEthPrio5Color_Tval pbEthPrio5Color;   
	pbEthPrio6Color_Tval pbEthPrio6Color;   
	pbEthPrio7Color_Tval pbEthPrio7Color;   
	RowStatus_Tval pbEthColorRowStatus;   
	int		pbEthColorCount;   
}MIBpbEthColorTable_t;



















 



 
 
extern MIBDid_t pbEthWredTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                     
	int		pbEthWredIndex;                                       
	int		pbEthWredLowThr0;   
	int		pbEthWredUpThr0;   
	int		pbEthWredLowThr1;   
	int		pbEthWredUpThr1;   
	int		pbEthWredLowThr2;   
	int		pbEthWredUpThr2;   
	RowStatus_Tval pbEthWredRowStatus;   
	int		pbEthWredCount;   
}MIBpbEthWredTable_t;

















 



 
 
extern MIBDid_t pbEthPortBasedACLTable_Did;

   
typedef enum {
	pbEthPortBasedACLItemType_filter=1,
	pbEthPortBasedACLItemType_forward=2
} pbEthPortBasedACLItemType_Tval;

typedef enum {
	pbEthPortBasedACLCoS_be=0,
	pbEthPortBasedACLCoS_bk=1,
	pbEthPortBasedACLCoS_sp=2,
	pbEthPortBasedACLCoS_ee=3,
	pbEthPortBasedACLCoS_cl=4,
	pbEthPortBasedACLCoS_vi=5,
	pbEthPortBasedACLCoS_vo=6,
	pbEthPortBasedACLCoS_nc=7,
	pbEthPortBasedACLCoS_none=8
} pbEthPortBasedACLCoS_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                               
	int		pbPortNo;                                                                    
	PbEthPortClass_Tval pbEthPortCls;                                                 
	char		pbEthPortBasedACLMacAddr[20];                                                  
	int		pbEthPortBasedACLVid;                                                           
	pbEthPortBasedACLItemType_Tval pbEthPortBasedACLItemType;   
	int		pbEthPortBasedACLRateLimit;   
	pbEthPortBasedACLCoS_Tval pbEthPortBasedACLCoS;   
	int		pbEthPortBasedACLIndex;   
	RowStatus_Tval pbEthPortBasedACLEntryRowStatus;   
}MIBpbEthPortBasedACLTable_t;

















 



 
 
extern MIBDid_t pbLacpGlobalCfgTable_Did;

   
typedef enum {
	pbLacpEnable_enable=1,
	pbLacpEnable_disable=2
} pbLacpEnable_Tval;

typedef enum {
	pbLacpHashMode_macipport=1,
	pbLacpHashMode_mac=2,
	pbLacpHashMode_ipport=3
} pbLacpHashMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;               
	pbLacpEnable_Tval pbLacpEnable;   
	char		pbLacpSystemID[255];   
	int		pbLacpSystemPri;   
	pbLacpHashMode_Tval pbLacpHashMode;   
}MIBpbLacpGlobalCfgTable_t;












 



 
 
extern MIBDid_t pbLacpAgConfigTable_Did;

   
typedef enum {
	pbLacpAgMode_dynamic=1,
	pbLacpAgMode_static=0
} pbLacpAgMode_Tval;

typedef enum {
	pbLacpAgRowState_active=1,
	pbLacpAgRowState_inactive=0
} pbLacpAgRowState_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                 
	int		pbLacpAgNo;                                       
	char		pbLacpAgName[32];   
	char		pbLacpAgMac[32];   
	pbLacpAgMode_Tval pbLacpAgMode;   
	pbLacpAgRowState_Tval pbLacpAgRowState;   
}MIBpbLacpAgConfigTable_t;













 



 
 
extern MIBDid_t pbLacpAgInformTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                             
	int		pbLacpAgNo;                                                
	int		pbLacpAgOperKey;   
	char		pbLacpPortList[128];   
	char		pbLacpPartnerMac[128];   
	int		pbLacpPartnerSysPri;   
	int		pbLacpPartnerAgOperKey;   
	int		pbLacpPortTypeInAg;   
	int		pbLacpPortSpeedInAg;   
}MIBpbLacpAgInformTable_t;
















 



 
 
extern MIBDid_t pbLacpApConfigTable_Did;

   
typedef enum {
	pbLacpTimeoutMode_fast=1,
	pbLacpTimeoutMode_slow=0
} pbLacpTimeoutMode_Tval;

typedef enum {
	pbLacpPortIndiv_individual=1,
	pbLacpPortIndiv_aggregable=2
} pbLacpPortIndiv_Tval;

typedef enum {
	pbLacpActiveMode_active=1,
	pbLacpActiveMode_passive=0
} pbLacpActiveMode_Tval;

typedef enum {
	pbLacpPortBpduMode_standardMode=0,
	pbLacpPortBpduMode_tagMode=1
} pbLacpPortBpduMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                     
	PbLacpPortClass_Tval pbLacpPortType;                       
	int		pbPortNo;                                          
	pbLacpTimeoutMode_Tval pbLacpTimeoutMode;   
	pbLacpPortIndiv_Tval pbLacpPortIndiv;   
	pbLacpActiveMode_Tval pbLacpActiveMode;   
	int		pbLacpDesigAggor;   
	int		pbLacpPortPri;   
	pbLacpPortBpduMode_Tval pbLacpPortBpduMode;   
}MIBpbLacpApConfigTable_t;
















 



 
 
extern MIBDid_t pbLacpApInformTable_Did;

   
typedef enum {
	pbLacpPortLinkState_up=1,
	pbLacpPortLinkState_down=2
} pbLacpPortLinkState_Tval;

typedef enum {
	pbLacpPartnerPortMode_active=1,
	pbLacpPartnerPortMode_passive=0
} pbLacpPartnerPortMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                             
	PbLacpPortClass_Tval pbLacpPortType;                            
	int		pbPortNo;                                                  
	pbLacpPortLinkState_Tval pbLacpPortLinkState;   
	int		pbLacpPortBelongAg;   
	char		pbLacpPortSync[32];   
	int		pbLacpPortOperKey;   
	int		pbLacpPartnerPortNo;   
	pbLacpPartnerPortMode_Tval pbLacpPartnerPortMode;   
	int		pbLacpPartnerPortPri;   
	int		pbLacpPartnerOperKey;   
	char		pbLacpPartnerPortState[32];   
}MIBpbLacpApInformTable_t;



















 



 
 
extern MIBDid_t pbIgmpSnoopingGlobalTable_Did;

   
typedef enum {
	pbIgmpSnoopingEnable_enable=1,
	pbIgmpSnoopingEnable_disable=2
} pbIgmpSnoopingEnable_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	pbIgmpSnoopingEnable_Tval pbIgmpSnoopingEnable;   
	int		pbIgmpSnoopingAgingTime;   
}MIBpbIgmpSnoopingGlobalTable_t;










 



 
 
extern MIBDid_t pbVlanIgmpSnoopingTable_Did;

   
typedef enum {
	pbVlanIgmpSnoopingEnable_enable=1,
	pbVlanIgmpSnoopingEnable_disable=2
} pbVlanIgmpSnoopingEnable_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                       
	int		pbIgmpVlanNumber;                                                       
	pbVlanIgmpSnoopingEnable_Tval pbVlanIgmpSnoopingEnable;   
	RowStatus_Tval pbVlanIgmpSnoopingRowStatus;   
}MIBpbVlanIgmpSnoopingTable_t;











 



 
 
extern MIBDid_t pbIgmpSnoopingIpGroupTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                     
	int		pbIgmpVlanNumber;                                  
	unsigned int	pbGroupIp;                                    
	char		pbIgmpMacAddr[20];   
	char		pbIgmpIpMemberPort[128];   
}MIBpbIgmpSnoopingIpGroupTable_t;












 



 
 
extern MIBDid_t pbIgmpSnoopingMacGroupTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	int		pbIgmpVlanNumber;                                    
	char		pbIgmpMacAddr[20];                                  
	char		pbIgmpMacMemberPort[128];   
}MIBpbIgmpSnoopingMacGroupTable_t;











 



 
 
extern MIBDid_t pbIgmpSnoopingMulRouterTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	int		pbIgmpVlanNumber;                                    
	char		pbIgmpMulRouterPort[128];   
}MIBpbIgmpSnoopingMulRouterTable_t;










 



 
extern MIBDid_t pbEthernetModule_Did;

   



# 33 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_gvrp.h" 1



 




 



 
extern MIBDid_t pbGvrpBase_Did;

   


 



 
 
extern MIBDid_t pbGvrpBridgeTable_Did;

   
typedef enum {
	pbGvrpEnable_disabled=0,
	pbGvrpEnable_enabled=1
} pbGvrpEnable_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	char		pbGvrpBridgeAddress[32];   
	pbGvrpEnable_Tval pbGvrpEnable;   
	int		pbGvrpJoinTimer;   
	int		pbGvrpLeaveTimer;   
	int		pbGvrpLeaveallTimer;   
	char		pbGvrpVlanList[255];
	int		pbGvrpMaxDVlanNum;
}MIBpbGvrpBridgeTable_t;















 



 
 
extern MIBDid_t pbGvrpPortTable_Did;

   
typedef enum {
	pbGvrpPortType_lan=1,
	pbGvrpPortType_wan=2,
	pbGvrpPortType_trunk=3
} pbGvrpPortType_Tval;

typedef enum {
	pbGvrpPortEnable_disabled=0,
	pbGvrpPortEnable_enabled=1
} pbGvrpPortEnable_Tval;

typedef enum {
	pbGvrpPortRegAdminCtrl_normal=0,
	pbGvrpPortRegAdminCtrl_fixed=1,
	pbGvrpPortRegAdminCtrl_forbidden=2
} pbGvrpPortRegAdminCtrl_Tval;

typedef enum {
	pbGvrpPortApplAdminCtrl_normal=0,
	pbGvrpPortApplAdminCtrl_nonparticipant=1
} pbGvrpPortApplAdminCtrl_Tval;

typedef enum {
	pbGvrpBlockPortEnable_disabled=0,
	pbGvrpBlockPortEnable_enabled=1
} pbGvrpBlockPortEnable_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	pbGvrpPortType_Tval pbGvrpPortType;                                  
	int		pbGvrpPortNo;                                                   
	pbGvrpPortEnable_Tval pbGvrpPortEnable;   
	pbGvrpPortRegAdminCtrl_Tval pbGvrpPortRegAdminCtrl;   
	pbGvrpPortApplAdminCtrl_Tval pbGvrpPortApplAdminCtrl;   
	pbGvrpBlockPortEnable_Tval pbGvrpBlockPortEnable;   
}MIBpbGvrpPortTable_t;














 



 
 
extern MIBDid_t pbGvrpDVlanTable_Did;

   
typedef enum {
	pbGvrpDPortType_lan=1,
	pbGvrpDPortType_wan=2,
	pbGvrpDPortType_trunk=3
} pbGvrpDPortType_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;               
	pbGvrpDPortType_Tval pbGvrpDPortType;                
	int		pbGvrpDPortNo;                                  
	char		pbGvrpDVlanList[255];   
}MIBpbGvrpDVlanTable_t;











 



 
extern MIBDid_t pbGvrpModule_Did;

   



# 34 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_gmrp.h" 1



 




 



 
extern MIBDid_t pbGmrpBase_Did;

   


 



 
 
extern MIBDid_t pbGmrpBridgeTable_Did;

   
typedef enum {
	pbGmrpEnable_disabled=0,
	pbGmrpEnable_enabled=1
} pbGmrpEnable_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	char		pbGmrpBridgeAddress[32];   
	pbGmrpEnable_Tval pbGmrpEnable;   
	int		pbGmrpJoinTimer;   
	int		pbGmrpLeaveTimer;   
	int		pbGmrpLeaveallTimer;   
	int		pbGmrpMaxDMacNum;   
}MIBpbGmrpBridgeTable_t;














 



 
 
extern MIBDid_t pbGmrpPortTable_Did;

   
typedef enum {
	pbGmrpPortType_lan=1,
	pbGmrpPortType_wan=2,
	pbGmrpPortType_trunk=3
} pbGmrpPortType_Tval;

typedef enum {
	pbGmrpPortEnable_disabled=0,
	pbGmrpPortEnable_enabled=1
} pbGmrpPortEnable_Tval;

typedef enum {
	pbGmrpPortRegAdminCtrl_normal=0,
	pbGmrpPortRegAdminCtrl_fixed=1,
	pbGmrpPortRegAdminCtrl_forbidden=2
} pbGmrpPortRegAdminCtrl_Tval;

typedef enum {
	pbGmrpPortApplAdminCtrl_normal=0,
	pbGmrpPortApplAdminCtrl_nonparticipant=1
} pbGmrpPortApplAdminCtrl_Tval;

typedef enum {
	pbGmrpForwardAllEnable_disabled=0,
	pbGmrpForwardAllEnable_enabled=1
} pbGmrpForwardAllEnable_Tval;

typedef enum {
	pbGmrpBlockPortEnable_disabled=0,
	pbGmrpBlockPortEnable_enabled=1
} pbGmrpBlockPortEnable_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	pbGmrpPortType_Tval pbGmrpPortType;                                  
	int		pbGmrpPortNo;                                                   
	pbGmrpPortEnable_Tval pbGmrpPortEnable;   
	pbGmrpPortRegAdminCtrl_Tval pbGmrpPortRegAdminCtrl;   
	pbGmrpPortApplAdminCtrl_Tval pbGmrpPortApplAdminCtrl;   
	pbGmrpForwardAllEnable_Tval pbGmrpForwardAllEnable;   
	pbGmrpBlockPortEnable_Tval pbGmrpBlockPortEnable;   
}MIBpbGmrpPortTable_t;















 



 
 
extern MIBDid_t pbVlanGmrpTable_Did;

   
typedef enum {
	pbVlanGmrpEnable_disable=0,
	pbVlanGmrpEnable_enable=1
} pbVlanGmrpEnable_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	int		pbGmrpVlanNumber;                                       
	pbVlanGmrpEnable_Tval pbVlanGmrpEnable;   
	RowStatus_Tval pbVlanGmrpRowStatus;   
}MIBpbVlanGmrpTable_t;











 



 
 
extern MIBDid_t pbGmrpDMacTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                         
	int		pbGmrpVlanId;                                             
	char		pbGmrpMacAddr[20];                                       
	char		pbGmrpMacMemberPort1[255];   
	char		pbGmrpMacMemberPort2[255];   
}MIBpbGmrpDMacTable_t;












 



 
extern MIBDid_t pbGmrpModule_Did;

   



# 35 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_eos.h" 1



 




 



 
extern MIBDid_t eosMapper_Did;

   


 



 
 
extern MIBDid_t vcgConfigTable_Did;

   
typedef enum {
	vcgXVType_vc4=0,
	vcgXVType_vc3=1,
	vcgXVType_vc12=2
} vcgXVType_Tval;

typedef enum {
	vcgPauseTransparent_disable=0,
	vcgPauseTransparent_enable=1
} vcgPauseTransparent_Tval;

typedef enum {
	vcgGfpMode_gfp_f=0,
	vcgGfpMode_gfp_t=1
} vcgGfpMode_Tval;

typedef enum {
	vcgGfpType_gfp_NullExternsion=0,
	vcgGfpType_gfp_LinearFrames=1
} vcgGfpType_Tval;

typedef enum {
	vcgGfpFCS_disable=0,
	vcgGfpFCS_enable=1
} vcgGfpFCS_Tval;

typedef enum {
	vcgGfpFcsAutoDetect_disable=0,
	vcgGfpFcsAutoDetect_enable=1
} vcgGfpFcsAutoDetect_Tval;

typedef enum {
	vcgPortMonMode_auto=1,
	vcgPortMonMode_mon=2,
	vcgPortMonMode_nmon=3
} vcgPortMonMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	int		vcgNo;                                                  
	char		vcgLabel[100];   
	vcgXVType_Tval vcgXVType;   
	char		vcgChannelId[50];   
	TruthValue_Tval vcgLCASSupport;   
	TruthValue_Tval vcgCapacityTrigger;   
	vcgPauseTransparent_Tval vcgPauseTransparent;   
	int		vcgLcasHoldOffTime;   
	vcgGfpMode_Tval vcgGfpMode;   
	vcgGfpType_Tval vcgGfpType;   
	vcgGfpFCS_Tval vcgGfpFCS;   
	vcgGfpFcsAutoDetect_Tval vcgGfpFcsAutoDetect;   
	TruthValue_Tval vcgGfpCsfAutoMode;   
	vcgPortMonMode_Tval vcgPortMonMode;
	TruthValue_Tval vcgPortAlarmDisable;
	int		vcgPLCTThreshold;   
	int		vcgPLCRThreshold;   
}MIBvcgConfigTable_t;

























 



 
 
extern MIBDid_t eosGlobalConfigTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;         
	int		eosIPGConfig;   
	int		eosMTUSize;   
	int		eosEslConfig;   
}MIBeosGlobalConfigTable_t;











 



 
 
extern MIBDid_t pbSOLCASConfigTable_Did;

   
typedef enum {
	pbSOLCASActive_none=0,
	pbSOLCASActive_active=1,
	pbSOLCASActive_deactive=2
} pbSOLCASActive_Tval;

typedef enum {
	pbSOLCASState_fixed=0,
	pbSOLCASState_add=1,
	pbSOLCASState_normal=2,
	pbSOLCASState_eos=3,
	pbSOLCASState_idle=5,
	pbSOLCASState_dnu=15
} pbSOLCASState_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;               
	int		pbPortNo;                                    
	PbTPType_Tval pbTpType;                           
	int		pbTpIndex;                                   
	pbSOLCASActive_Tval pbSOLCASActive;   
	int		pbSOLCASVCGPort;   
	pbSOLCASState_Tval pbSOLCASState;   
}MIBpbSOLCASConfigTable_t;














 



 
 
extern MIBDid_t pbSKLCASConfigTable_Did;

   
typedef enum {
	pbSKLCASState_ok=0,
	pbSKLCASState_fail=1
} pbSKLCASState_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;               
	int		pbPortNo;                                    
	PbTPType_Tval pbTpType;                           
	int		pbTpIndex;                                   
	int		pbSKLCASVCGPort;   
	pbSKLCASState_Tval pbSKLCASState;   
}MIBpbSKLCASConfigTable_t;













 



 
extern MIBDid_t enterpriseEOSModule_Did;

   



# 36 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_rstp.h" 1



 




 



 
extern MIBDid_t pbRstpBase_Did;

   


 



 
 
extern MIBDid_t pbRstpBridgeTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                 
	int		pbRstpBridgeAddress_len;
	char		pbRstpBridgeAddress[6];   
	int		pbRstpBridgePriority;   
	int		pbRstpBridgeMaxAge;   
	int		pbRstpBridgeHelloTime;   
	int		pbRstpBridgeForwardDelay;   
	int		pbRstpTxHoldCount;   
	TruthValue_Tval pbRstpBEGIN;   
	TruthValue_Tval pbRstpEventSwitch;   
}MIBpbRstpBridgeTable_t;
















 



 
 
extern MIBDid_t pbRstpPortTable_Did;

   
typedef enum {
	pbRstpPortType_lan=1,
	pbRstpPortType_wan=2,
	pbRstpPortType_trunk=3
} pbRstpPortType_Tval;

typedef enum {
	pbRstpPortEnable_enabled=1,
	pbRstpPortEnable_disabled=2
} pbRstpPortEnable_Tval;

typedef enum {
	pbRstpPortAdminPointToPoint_forceTrue=0,
	pbRstpPortAdminPointToPoint_forceFalse=1,
	pbRstpPortAdminPointToPoint_auto=2
} pbRstpPortAdminPointToPoint_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                       
	pbRstpPortType_Tval pbRstpPortType;                                          
	int		pbRstpPortNo;                                                           
	int		pbRstpPortPriority;   
	pbRstpPortEnable_Tval pbRstpPortEnable;   
	int		pbRstpPortPathCost;   
	TruthValue_Tval pbRstpPortAdminEdgePort;   
	pbRstpPortAdminPointToPoint_Tval pbRstpPortAdminPointToPoint;   
}MIBpbRstpPortTable_t;















 



 
 
extern MIBDid_t pbRstpBridgeStateTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                             
	char		pbRstpRPVector1[75];   
	char		pbRstpRPVector2[75];   
	int		pbRstpRootMaxAge;   
	int		pbRstpRootHelloTime;   
	int		pbRstpRootForwardDelay;   
	int		pbRstpRootPortId;   
}MIBpbRstpBridgeStateTable_t;














 



 
 
extern MIBDid_t pbRstpPortStateTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	int		pbRstpPortType;                                      
	int		pbRstpPortNo;                                        
	int		pbRstpPEnable;   
	char		pbRstpPPVector1[75];   
	char		pbRstpPPVector2[75];   
	char		pbRstpDsgPVector1[75];   
	char		pbRstpDsgPVector2[75];   
	char		pbRstpMsgPVector1[75];   
	char		pbRstpMsgPVector2[75];   
	char		pbRstpPTimes[30];   
	char		pbRstpDsgTimes[30];   
	char		pbRstpMsgTimes[30];   
	int		pbRstpPForward;   
	int		pbRstpPLearn;   
	int		pbRstpPInfoIs;   
	int		pbRstpPRole;   
	int		pbRstpPSelectedRole;   
}MIBpbRstpPortStateTable_t;

























 



 
extern MIBDid_t enterpriseRstpMIB_Did;

   



# 37 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_mstp.h" 1



 




 



 
extern MIBDid_t pbMstpBase_Did;

   


 



 
extern MIBDid_t pbMstpBridgeBase_Did;

   


 



 
extern MIBDid_t pbMstpPortBase_Did;

   


 



 
extern MIBDid_t pbMstpBridgeStateBase_Did;

   


 



 
extern MIBDid_t pbMstpPortStateBase_Did;

   


 



 
 
extern MIBDid_t pbCistPortStateTable_Did;

   
typedef enum {
	pbCistPState_psDisable=0,
	pbCistPState_psBlocking=1,
	pbCistPState_psLearning=2,
	pbCistPState_psForwarding=3
} pbCistPState_Tval;

typedef enum {
	pbCistPRole_prDisable=0,
	pbCistPRole_prRoot=1,
	pbCistPRole_prDesignated=2,
	pbCistPRole_prAlternate=3,
	pbCistPRole_prBackup=4,
	pbCistPRole_prMaster=5
} pbCistPRole_Tval;

typedef enum {
	pbCistPPState_forceTrue=0,
	pbCistPPState_forceFalse=1
} pbCistPPState_Tval;

typedef enum {
	pbPortBpduRule_stp=0,
	pbPortBpduRule_rstp_mstp=1
} pbPortBpduRule_Tval;

typedef enum {
	pbPortOperEdge_normalPort=0,
	pbPortOperEdge_edgePort=1
} pbPortOperEdge_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                   
	PbEthInterfaceType_Tval pbMstPortType;                
	int		pbPortNo;                                        
	PbEnableDisable_Tval pbCistPortEnable;   
	char		pbCistPPVector1[75];   
	char		pbCistPPVector2[75];   
	char		pbCistPPVector3[75];   
	char		pbCistDPVector1[75];   
	char		pbCistDPVector2[75];   
	char		pbCistDPVector3[75];   
	char		pbCistMPVector1[75];   
	char		pbCistMPVector2[75];   
	char		pbCistMPVector3[75];   
	char		pbCistPTimes[30];   
	char		pbCistDTimes[30];   
	char		pbCistMTimes[30];   
	pbCistPState_Tval pbCistPState;   
	int		pbCistPInfoIs;   
	pbCistPRole_Tval pbCistPRole;   
	pbCistPPState_Tval pbCistPPState;
	int		pbMstRcvdInternal;   
	pbPortBpduRule_Tval pbPortBpduRule;
	pbPortOperEdge_Tval pbPortOperEdge;
}MIBpbCistPortStateTable_t;






























 



 
 
extern MIBDid_t pbMstiPortStateTable_Did;

   
typedef enum {
	pbMstiPState_psDisable=0,
	pbMstiPState_psBlocking=1,
	pbMstiPState_psLearning=2,
	pbMstiPState_psForwarding=3
} pbMstiPState_Tval;

typedef enum {
	pbMstiPRole_prDisable=0,
	pbMstiPRole_prRoot=1,
	pbMstiPRole_prDesignated=2,
	pbMstiPRole_prAlternate=3,
	pbMstiPRole_prBackup=4,
	pbMstiPRole_prMaster=5
} pbMstiPRole_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                       
	PbEthInterfaceType_Tval pbMstPortType;                    
	int		pbPortNo;                                            
	int		pbMstiNumber;                                        
	char		pbMstiPPVector1[75];   
	char		pbMstiPPVector2[75];   
	char		pbMstiDPVector1[75];   
	char		pbMstiDPVector2[75];   
	char		pbMstiMPVector1[75];   
	char		pbMstiMPVector2[75];   
	char		pbMstiPTimes[30];   
	char		pbMstiDTimes[30];   
	char		pbMstiMTimes[30];   
	pbMstiPState_Tval pbMstiPState;   
	int		pbMstiPInfoIs;   
	pbMstiPRole_Tval pbMstiPRole;   
	int		pbMstiPSelectedRole;   
}MIBpbMstiPortStateTable_t;
























 



 
 
extern MIBDid_t pbCistBridgeStateTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	int		pbMstBaseNumPorts;   
	char		pbCistRPVector1[75];   
	char		pbCistRPVector2[75];   
	char		pbCistRPVector3[75];   
	int		pbMstRootMaxAge;
	int		pbMstRootMsgAge;   
	int		pbMstRootHelloTime;   
	int		pbMstRootForwardDelay;   
	int		pbCistRootRemainingHops;   
	int		pbCistRootPortId;   
	int		pbMstCfgDigest_len;
	char		pbMstCfgDigest[16];   
}MIBpbCistBridgeStateTable_t;



















 



 
 
extern MIBDid_t pbMstiBridgeStateTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	int		pbMstiNumber;                                                
	char		pbMstiRPVector1[75];   
	char		pbMstiRPVector2[75];   
	int		pbMstiRootRemainingHops;   
	int		pbMstiRootPortId;   
}MIBpbMstiBridgeStateTable_t;













 



 
 
extern MIBDid_t pbCistPortTable_Did;

   
typedef enum {
	pbMstPortAdminPointToPoint_forceTrue=0,
	pbMstPortAdminPointToPoint_forceFalse=1,
	pbMstPortAdminPointToPoint_auto=2
} pbMstPortAdminPointToPoint_Tval;

typedef enum {
	pbMstPortBpduMode_standardMode=0,
	pbMstPortBpduMode_tagMode=1
} pbMstPortBpduMode_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                                     
	PbEthInterfaceType_Tval pbMstPortType;                                  
	int		pbPortNo;                                                          
	int		pbMstPort;   
	int		pbCistPortPriority;   
	int		pbCistExPortPathCost;   
	int		pbCistInPortPathCost;   
	TruthValue_Tval pbMstPortAdminEdgePort;   
	pbMstPortAdminPointToPoint_Tval pbMstPortAdminPointToPoint;   
	PbEnableDisable_Tval pbMstPortEnable;   
	TruthValue_Tval pbMstPortCheck;
	pbMstPortBpduMode_Tval pbMstPortBpduMode;
}MIBpbCistPortTable_t;



















 



 
 
extern MIBDid_t pbMstiPortTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                     
	PbEthInterfaceType_Tval pbMstPortType;                  
	int		pbPortNo;                                          
	int		pbMstiNumber;                                      
	int		pbMstiPortPriority;   
	int		pbMstiPortPathCost;   
}MIBpbMstiPortTable_t;













 



 
 
extern MIBDid_t pbCistBridgeTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                               
	int		pbMstBaseBridgeAddress_len;
	char		pbMstBaseBridgeAddress[6];   
	int		pbCistPriority;   
	int		pbMstForceVersion;   
	int		pbMstBridgeMaxAge;   
	int		pbMstBridgeHelloTime;   
	int		pbMstBridgeForwardDelay;   
	int		pbMstTxHoldCount;   
	int		pbMstMaxHops;   
	TruthValue_Tval pbMstMstpBEGIN;   
	char		pbMstMstpConfigName[33];   
	int		pbMstMstpRevisonLevel;   
	TruthValue_Tval pbMstEventSwitch;
}MIBpbCistBridgeTable_t;




















 



 
 
extern MIBDid_t pbMstiBridgeTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                 
	int		pbMstiNumber;                                     
	char		pbMstiVlanNumber[255];   
	int		pbMstiPriority;   
}MIBpbMstiBridgeTable_t;











 



 
extern MIBDid_t enterpriseMstpMIB_Did;

   



# 38 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_dcc.h" 1



 




 



 
 
extern MIBDid_t pbIpRouteTable_Did;

   
typedef enum {
	pbIpRouteProto_other=1,
	pbIpRouteProto_local=2,
	pbIpRouteProto_netmgmt=3,
	pbIpRouteProto_icmp=4,
	pbIpRouteProto_egp=5,
	pbIpRouteProto_ggp=6,
	pbIpRouteProto_hello=7,
	pbIpRouteProto_rip=8,
	pbIpRouteProto_is_is=9,
	pbIpRouteProto_es_is=10,
	pbIpRouteProto_ciscoIgrp=11,
	pbIpRouteProto_bbnSpfIgp=12,
	pbIpRouteProto_ospf=13,
	pbIpRouteProto_bgp=14
} pbIpRouteProto_Tval;

typedef enum {
	pbIpRouteType_other=1,
	pbIpRouteType_invalid=2,
	pbIpRouteType_direct=3,
	pbIpRouteType_indirect=4
} pbIpRouteType_Tval;

typedef struct
{
	unsigned int	pbIpRouteDest;                            
	unsigned int	pbIpRouteMask;                            
	pbIpRouteProto_Tval pbIpRouteProto;                    
	int		pbIpRouteIfIndex;   
	unsigned int	pbIpRouteNextHop;   
	pbIpRouteType_Tval pbIpRouteType;   
	int		pbIpRouteAge;   
	int		pbIpRouteMetric1;   
	int		pbIpRouteMetric2;   
	int		pbIpRouteMetric3;   
	int		pbIpRouteMetric4;   
	int		pbIpRouteMetric5;   
}MIBpbIpRouteTable_t;



















 



 
 
extern MIBDid_t dccCfgTable_Did;

   
typedef enum {
	mode_ms=0,
	mode_rs=1,
	mode_hcoc9=2,
	mode_hcoc27=3,
	mode_ecc=4
} mode_Tval;

typedef enum {
	state_disable=1,
	state_enable=2,
	state_transparent=3
} state_Tval;

typedef enum {
	protocol_ppp=1,
	protocol_osi=2,
	protocol_iptunnel=3
} protocol_Tval;

typedef enum {
	linkStatus_down=1,
	linkStatus_up=2
} linkStatus_Tval;

typedef enum {
	mspStatus_nonprotected=1,
	mspStatus_protected=2
} mspStatus_Tval;

typedef enum {
	crcMode_crc16=1,
	crcMode_crc32=2
} crcMode_Tval;

typedef enum {
	multilink_open=1,
	multilink_close=2
} multilink_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;               
	int		pbPortNo;                                    
	mode_Tval mode;                                      
	state_Tval state;   
	protocol_Tval protocol;   
	unsigned int	dccBundleNumber;   
	linkStatus_Tval linkStatus;   
	mspStatus_Tval mspStatus;   
	int		destSlot;   
	int		destPort;   
	int		mtu;
	crcMode_Tval crcMode;   
	int		hcocIndex;   
	multilink_Tval multilink;   
}MIBdccCfgTable_t;





















 



 
 
extern MIBDid_t ipInIpCfgTable_Did;

   
typedef struct
{
	unsigned int	ipInIpIfIpAddr;                               
	unsigned int	ipInIpIfMask;                                 
	unsigned int	ipInIpDestIpAddr;                             
	unsigned int	ipInIpSourcIpAddr;   
	unsigned int	ipInIpIfIndex;   
	unsigned int	ipInIpIfMtu;   
	RowStatus_Tval ipInIpRowStatus;
	unsigned int	ipInIpPeerIfIpAddr;   
}MIBipInIpCfgTable_t;















 



 
 
extern MIBDid_t mpBundleCfgTable_Did;

   
typedef enum {
	bundleTOS_enable=1,
	bundleTOS_disable=2
} bundleTOS_Tval;

typedef struct
{
	unsigned int	bundlePeerIpAddr;                         
	unsigned int	bundleIndex;   
	int		bundleStatus;   
	int		bundleNumber;   
	RowStatus_Tval bundleRowStatus;   
	bundleTOS_Tval bundleTOS;
}MIBmpBundleCfgTable_t;













 



 
 
extern MIBDid_t pbStaticIpRouteTable_Did;

   
typedef enum {
	pbStaticIpRouteProto_local=1
} pbStaticIpRouteProto_Tval;

typedef enum {
	pbStaticIpRouteMetricType_none=0,
	pbStaticIpRouteMetricType_aseType1Metric=1,
	pbStaticIpRouteMetricType_aseType2Metric=2
} pbStaticIpRouteMetricType_Tval;

typedef struct
{
	unsigned int	pbStaticIpRouteDest;                                          
	unsigned int	pbStaticIpRouteMask;                                          
	pbStaticIpRouteProto_Tval pbStaticIpRouteProto;                            
	int		pbStaticIpRouteIfIndex;   
	unsigned int	pbStaticIpRouteNextHop;   
	RouteStatus_Tval pbStaticIpRouteAdminStatus;   
	RouteStatus_Tval pbStaticIpRouteOperStatus;   
	pbStaticIpRouteMetricType_Tval pbStaticIpRouteMetricType;   
	int		pbStaticIpRouteCost;   
}MIBpbStaticIpRouteTable_t;
















 



 
 
extern MIBDid_t tracertCfgTable_Did;

   
typedef enum {
	tracertStart_start=1,
	tracertStart_stop=2
} tracertStart_Tval;

typedef struct
{
	unsigned int	tracertPeerIpAddr;                       
	unsigned int	tracertTimeout;
	RowStatus_Tval tracertRowStatus;
	tracertStart_Tval tracertStart;
}MIBtracertCfgTable_t;











 



 
 
extern MIBDid_t tracertResultTable_Did;

   
typedef struct
{
	int		tracertTtlNumber;                                      
	int		tracertTripTime1;
	int		tracertTripTime2;
	int		tracertTripTime3;
	unsigned int	tracertPeerIpAddrDis;
	RowStatus_Tval tracertRstRowStatus;
}MIBtracertResultTable_t;













 



 
 
extern MIBDid_t pingCfgTable_Did;

   
typedef enum {
	pingStart_start=1,
	pingStart_stop=2
} pingStart_Tval;

typedef struct
{
	unsigned int	pingTargetIpAddr;                      
	int		pingDataSize;
	int		pingCounts;
	int		pingTimeout;
	RowStatus_Tval pingRowStatus;
	pingStart_Tval pingStart;
}MIBpingCfgTable_t;













 



 
 
extern MIBDid_t pingResultTable_Did;

   
typedef struct
{
	int		pingIndex;                                           
	unsigned int	pingTargetIpAddrDis;
	int		pingDataSizeDis;
	int		pingTripTime;
	int		pingTtlnumber;
	int		pingResult;
	RowStatus_Tval pingRstRowStatus;
}MIBpingResultTable_t;














 



 
 
extern MIBDid_t dccReservePortTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;         
	int		phyPortNum;                            
	int		logPortNum;
}MIBdccReservePortTable_t;










 



 
extern MIBDid_t pbDccModule_Did;

   



# 39 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_iptunnel.h" 1



 




 



 
 
extern MIBDid_t pbIpTunnelCfgTable_Did;

   
typedef enum {
	pbSdhServerType_ms=0,
	pbSdhServerType_rs=1,
	pbSdhServerType_hcoc9=2,
	pbSdhServerType_hcoc27=3
} pbSdhServerType_Tval;

typedef enum {
	pbLapdNetworkSide_user=0,
	pbLapdNetworkSide_network=1
} pbLapdNetworkSide_Tval;

typedef enum {
	pbLapdAITS_unconfirmed=0,
	pbLapdAITS_acknowledged=1
} pbLapdAITS_Tval;

typedef enum {
	pbIpTunnelByPass_nobypass=0,
	pbIpTunnelByPass_bypass=1
} pbIpTunnelByPass_Tval;

typedef enum {
	pbIpTunnelDstMode_ms=0,
	pbIpTunnelDstMode_rs=1
} pbIpTunnelDstMode_Tval;

typedef enum {
	pbIpTunnelStatus_deactive=0,
	pbIpTunnelStatus_active=1
} pbIpTunnelStatus_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                             
	int		pbPortNo;                                                  
	pbSdhServerType_Tval pbSdhServerType;                           
	pbLapdNetworkSide_Tval pbLapdNetworkSide;   
	int		pbLapdT200;   
	int		pbLapdT203;   
	int		pbLapdN200;   
	pbLapdAITS_Tval pbLapdAITS;   
	int		pbLapdWindow;   
	unsigned int	pbIpTunnelSrcIpAddress;   
	unsigned int	pbIpTunnelDesIpAddress;   
	pbIpTunnelByPass_Tval pbIpTunnelByPass;   
	int		pbIpTunnelDstSlot;   
	int		pbIpTunnelDstPort;   
	pbIpTunnelDstMode_Tval pbIpTunnelDstMode;   
	pbIpTunnelStatus_Tval pbIpTunnelStatus;   
}MIBpbIpTunnelCfgTable_t;























 



 
extern MIBDid_t pbIpTunnelModule_Did;

   



# 40 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_osi.h" 1



 




 



 
 
extern MIBDid_t pbOsiConfigTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                   
	int		pbPortNo;                                        
	int		pbOsiNetworkSide;   
	int		pbOsiT200;   
	int		pbOsiT203;   
	int		pbOsiN200;   
	int		pbOsiAITS;   
	int		pbOsiWindow;   
	int		pbOsiConfigTimer;   
	int		pbOsiTTL;   
	char		pbOsiLocalNasp[41];   
	char		pbOsiRemoteNasp[41];   
	unsigned int	pbOsiSrcIpAddress;   
	unsigned int	pbOsiDesIpAddress;   
	int		pbOsiIpIfMtu;   
}MIBpbOsiConfigTable_t;






















 



 
 
extern MIBDid_t osiTunnelConfigTable_Did;

   
typedef enum {
	osiTunnelStatus_deactive=0,
	osiTunnelStatus_active=1
} osiTunnelStatus_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbSlotNumber;                           
	int		pbPortNo;                                                
	int		pbSdhServerType;                                            
	int		osiTunnelNetworkSide;   
	int		osiTunnelT200;   
	int		osiTunnelT203;   
	int		osiTunnelN200;   
	int		osiTunnelAITS;   
	int		osiTunnelWindow;   
	int		osiTunnelConfigTimer;   
	int		osiTunnelTTL;   
	char		osiTunnelLocalNasp[41];   
	char		osiTunnelRemoteNasp[41];   
	unsigned int	osiTunnelSrcIpAddress;   
	unsigned int	osiTunnelDesIpAddress;   
	int		osiTunnelIpIfMtu;   
	osiTunnelStatus_Tval osiTunnelStatus;
	unsigned int	osiTunnelIndex;   
}MIBosiTunnelConfigTable_t;

























 



 
extern MIBDid_t pbOsiModule_Did;

   



# 41 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_protection.h" 1



 




 



 
extern MIBDid_t epgGroup_Did;

   


 



 
extern MIBDid_t msspringGroup_Did;

   


 



 
extern MIBDid_t mspGroup_Did;

   


 



 
extern MIBDid_t sncpGroup_Did;

   


 



 
extern MIBDid_t mssp4fGroup_Did;

   


 



 
 
extern MIBDid_t mssp4fConfigTable_Did;

   
typedef enum {
	mssp4fProtectMode_normal=1,
	mssp4fProtectMode_transoceanic=2
} mssp4fProtectMode_Tval;

typedef enum {
	mssp4fRingType_none=0,
	mssp4fRingType_stm_1=1,
	mssp4fRingType_stm_4=4,
	mssp4fRingType_stm_16=16,
	mssp4fRingType_stm_64=64
} mssp4fRingType_Tval;

typedef struct
{
	unsigned int	mssp4fRingId;                                                     
	mssp4fProtectMode_Tval mssp4fProtectMode;   
	char		mssp4fRingName[64];   
	unsigned int	mssp4fGroupId;   
	unsigned int	mssp4fWaitToRestore;   
	mssp4fRingType_Tval mssp4fRingType;   
	char		mssp4fWestWorkChannelNutInfo[64];   
	char		mssp4fEastWorkChannelNutInfo[64];   
	TruthValue_Tval mssp4fConfigEnabled;   
	TruthValue_Tval mssp4fConfigDeleted;   
	TruthValue_Tval mssp4fRingSuspended;   
	TruthValue_Tval mssp4fAutoSquelchEnabled;   
	TruthValue_Tval mssp4fAutoMappingEnabled;   
	PbSlotNumberType_Tval mssp4fWestWorkSlotNum;   
	PbPortType_Tval mssp4fWestWorkPortType;   
	unsigned int	mssp4fWestWorkPortNum;   
	PbSlotNumberType_Tval mssp4fWestProtSlotNum;   
	PbPortType_Tval mssp4fWestProtPortType;   
	unsigned int	mssp4fWestProtPortNum;   
	PbSlotNumberType_Tval mssp4fEastWorkSlotNum;   
	PbPortType_Tval mssp4fEastWorkPortType;   
	unsigned int	mssp4fEastWorkPortNum;   
	PbSlotNumberType_Tval mssp4fEastProtSlotNum;   
	PbPortType_Tval mssp4fEastProtPortType;   
	unsigned int	mssp4fEastProtPortNum;   
	PbSlotNumberType_Tval mssp4fPeerWestWorkSlotNum;
	PbPortType_Tval mssp4fPeerWestWorkPortType;
	unsigned int	mssp4fPeerWestWorkPortNum;
	PbSlotNumberType_Tval mssp4fPeerWestProtSlotNum;
	PbPortType_Tval mssp4fPeerWestProtPortType;
	unsigned int	mssp4fPeerWestProtPortNum;
	PbSlotNumberType_Tval mssp4fPeerEastWorkSlotNum;
	PbPortType_Tval mssp4fPeerEastWorkPortType;
	unsigned int	mssp4fPeerEastWorkPortNum;
	PbSlotNumberType_Tval mssp4fPeerEastProtSlotNum;
	PbPortType_Tval mssp4fPeerEastProtPortType;
	unsigned int	mssp4fPeerEastProtPortNum;
}MIBmssp4fConfigTable_t;












































 



 
 
extern MIBDid_t mssp4fTopologyTable_Did;

   
typedef struct
{
	unsigned int	mssp4fRingId;                          
	unsigned int	mssp4fSumOfNodes;
	unsigned int	mssp4fNodeId0;
	unsigned int	mssp4fNodeId1;
	unsigned int	mssp4fNodeId2;
	unsigned int	mssp4fNodeId3;
	unsigned int	mssp4fNodeId4;
	unsigned int	mssp4fNodeId5;
	unsigned int	mssp4fNodeId6;
	unsigned int	mssp4fNodeId7;
	unsigned int	mssp4fNodeId8;
	unsigned int	mssp4fNodeId9;
	unsigned int	mssp4fNodeId10;
	unsigned int	mssp4fNodeId11;
	unsigned int	mssp4fNodeId12;
	unsigned int	mssp4fNodeId13;
	unsigned int	mssp4fNodeId14;
	unsigned int	mssp4fNodeId15;
	unsigned int	mssp4fNodeIp0;
	unsigned int	mssp4fNodeIp1;
	unsigned int	mssp4fNodeIp2;
	unsigned int	mssp4fNodeIp3;
	unsigned int	mssp4fNodeIp4;
	unsigned int	mssp4fNodeIp5;
	unsigned int	mssp4fNodeIp6;
	unsigned int	mssp4fNodeIp7;
	unsigned int	mssp4fNodeIp8;
	unsigned int	mssp4fNodeIp9;
	unsigned int	mssp4fNodeIp10;
	unsigned int	mssp4fNodeIp11;
	unsigned int	mssp4fNodeIp12;
	unsigned int	mssp4fNodeIp13;
	unsigned int	mssp4fNodeIp14;
	unsigned int	mssp4fNodeIp15;
}MIBmssp4fTopologyTable_t;









































 



 
 
extern MIBDid_t mssp4fCommandTable_Did;

   
typedef enum {
	mssp4fCommandCurrentSide_west=0,
	mssp4fCommandCurrentSide_east=1
} mssp4fCommandCurrentSide_Tval;

typedef enum {
	mssp4fCommand_exerciseRing=3,
	mssp4fCommand_exerciseSpan=4,
	mssp4fCommand_manualSwitchRing=6,
	mssp4fCommand_manualSwitchSpan=7,
	mssp4fCommand_forcedSwitchRing=13,
	mssp4fCommand_forcedSwitchSpan=14,
	mssp4fCommand_lockoutProtection=15,
	mssp4fCommand_clear=21,
	mssp4fCommand_noCommand=23
} mssp4fCommand_Tval;

typedef struct
{
	unsigned int	mssp4fRingId;                                          
	mssp4fCommandCurrentSide_Tval mssp4fCommandCurrentSide;             
	mssp4fCommand_Tval mssp4fCommand;
	char		mssp4fLastCommandStatus[64];   
}MIBmssp4fCommandTable_t;











 



 
 
extern MIBDid_t mssp4fNodeStatusTable_Did;

   
typedef enum {
	mssp4fNodeAgentState_init=0,
	mssp4fNodeAgentState_ready=1,
	mssp4fNodeAgentState_suspend=2
} mssp4fNodeAgentState_Tval;

typedef enum {
	mssp4fNodeState_idle=0,
	mssp4fNodeState_passthough=1,
	mssp4fNodeState_switching=2,
	mssp4fNodeState_isolated=3
} mssp4fNodeState_Tval;

typedef enum {
	mssp4fNodeCurrentSide_west=0,
	mssp4fNodeCurrentSide_east=1
} mssp4fNodeCurrentSide_Tval;

typedef enum {
	mssp4fNodeCurrentReq_idle=0,
	mssp4fNodeCurrentReq_bridge=1,
	mssp4fNodeCurrentReq_bridgeAndSwitch=2,
	mssp4fNodeCurrentReq_passthrough=3
} mssp4fNodeCurrentReq_Tval;

typedef enum {
	mssp4fNodeCurrentSrc_kByte=0,
	mssp4fNodeCurrentSrc_line=1,
	mssp4fNodeCurrentSrc_external=2
} mssp4fNodeCurrentSrc_Tval;

typedef enum {
	mssp4fNodeKExpressMode_autoLocal=0,
	mssp4fNodeKExpressMode_autoExpress=1,
	mssp4fNodeKExpressMode_forceExpress=2,
	mssp4fNodeKExpressMode_forceLocal=3
} mssp4fNodeKExpressMode_Tval;

typedef struct
{
	unsigned int	mssp4fRingId;                                        
	mssp4fNodeAgentState_Tval mssp4fNodeAgentState;
	mssp4fNodeState_Tval mssp4fNodeState;   
	mssp4fNodeCurrentSide_Tval mssp4fNodeCurrentSide;
	mssp4fNodeCurrentReq_Tval mssp4fNodeCurrentReq;
	mssp4fNodeCurrentSrc_Tval mssp4fNodeCurrentSrc;
	mssp4fNodeKExpressMode_Tval mssp4fNodeKExpressMode;
	Mssp4fSwitchCommand_Tval mssp4fWestSwitchCommand;
	Mssp4fLineRequest_Tval mssp4fWestLineReq;
	Mssp4fSwitchCommand_Tval mssp4fWestLastSwitchCmd;
	int		mssp4fWestSwitchStatus_len;
	char		mssp4fWestSwitchStatus[64];   
	unsigned int	mssp4fWestK1byteInput;
	unsigned int	mssp4fWestK2byteInput;
	unsigned int	mssp4fWestK1byteOutput;
	unsigned int	mssp4fWestK2byteOutput;
	Mssp4fSwitchCommand_Tval mssp4fEastSwitchCommand;
	Mssp4fLineRequest_Tval mssp4fEastLineReq;
	Mssp4fSwitchCommand_Tval mssp4fEastLastSwitchCmd;
	int		mssp4fEastSwitchStatus_len;
	char		mssp4fEastSwitchStatus[64];   
	unsigned int	mssp4fEastK1byteInput;
	unsigned int	mssp4fEastK2byteInput;
	unsigned int	mssp4fEastK1byteOutput;
	unsigned int	mssp4fEastK2byteOutput;
	unsigned int	mssp4fPassNodeId;
}MIBmssp4fNodeStatusTable_t;































 



 
 
extern MIBDid_t mssp4fAutoMappingTable_Did;

   
typedef struct
{
	unsigned int	mssp4fRingId;                                                
	unsigned int	mssp4fNodeId;                                                
	int		mssp4fWestWorkMappingString_len;
	char		mssp4fWestWorkMappingString[192];
	int		mssp4fEastWorkMappingString_len;
	char		mssp4fEastWorkMappingString[192];
	RowStatus_Tval mssp4fAutomappingRowStatus;
}MIBmssp4fAutoMappingTable_t;












 



 
 
extern MIBDid_t mssp4fXcTable_Did;

   
typedef enum {
	mssp4fXcSide_west=0,
	mssp4fXcSide_east=1,
	mssp4fXcSide_both=2
} mssp4fXcSide_Tval;

typedef enum {
	mssp4fXcOrder_au4clear=0,
	mssp4fXcOrder_au44c=1,
	mssp4fXcOrder_au416c=2,
	mssp4fXcOrder_au464c=3
} mssp4fXcOrder_Tval;

typedef enum {
	mssp4fXcType_adddrop=0,
	mssp4fXcType_passthrough=1,
	mssp4fXcType_nonrelated=2,
	mssp4fXcType_invalid=3
} mssp4fXcType_Tval;

typedef struct
{
	unsigned int	mssp4fRingId;                          
	unsigned int	mssp4fNodeId;                          
	mssp4fXcSide_Tval mssp4fXcSide;                     
	unsigned int	mssp4fXcTimeSlot;                      
	mssp4fXcOrder_Tval mssp4fXcOrder;
	mssp4fXcType_Tval mssp4fXcType;
}MIBmssp4fXcTable_t;













 



 
 
extern MIBDid_t pbSncpConfigTable_Did;

   
typedef struct
{
	PbXcCapacity_Tval pbSncpTPType;                                            
	PbSlotNumberType_Tval pbSncpWorkChSlotNum;                                 
	PbPortType_Tval pbSncpWorkChPortType;                                   
	int		pbSncpWorkChPortNum;                                                  
	int		pbSncpWorkSource;                                                     
	PbSlotNumberType_Tval pbSncpProtectChSlotNum;                              
	PbPortType_Tval pbSncpProtectChPortType;                                
	int		pbSncpProtectChPortNum;                                               
	int		pbSncpProtectSource;                                                  
	PbSlotNumberType_Tval pbSncpSrcOrDstSlotNum;                               
	PbPortType_Tval pbSncpSrcOrDstPortType;                                    
	int		pbSncpSrcOrDstPortNum;                                                
	int		pbSncpSrcOrDstSource;                                                 
	PbSNCPSignalDirection_Tval pbSncpSignalDircetion;                          
	int		pbSncpGroupId;   
	PbSNCPChannelState_Tval pbSncpWorkChState;   
	PbSNCPExternalCmd_Tval pbSncpExternalCmd;   
	PbSNCPExCmdRslt_Tval pbSncpExCmdRslt;   
	PbSNCPCurrentReq_Tval pbSncpCurSwitchReq;   
	PbSNCPSwitchReason_Tval pbSncpSwitchReason;   
	unsigned int	pbSncpHoldOffTime;   
	PbSNCPRestoreMode_Tval pbSncpRestoreMode;   
	unsigned int	pbSncpWTR;   
	PBSNCPSignalState_Tval pbSncpWorkSignalState;   
	PBSNCPSignalState_Tval pbSncpProtectSignalState;   
	RowStatus_Tval pbSncpRowStatus;   
	char		pbSncpLable[64];   
	PBSNCPAddRslt_Tval pbSncpAddRslt;   
	unsigned int	pbDNIId;   
	PBSNCPIntrusive_Tval pbSncpIntrusive;   
	int		pbSncpWorkGmplsVcNumber;   
	char		pbSncpWorkGmplsCallName[255];   
	int		pbSncpProtectGmplsVcNumber;   
	char		pbSncpProtectGmplsCallName[255];   
	PBSNCPWorkorProtToCC_Tval pbSncpWorkorProtToCC;   
	PbSncpCircuitDirection_Tval pbSncpDirection;   
	PbSncpFrom_Tval pbSncpFrom;   
	PbGmplsSncpDirection_Tval pbGmplsSncpDirection;   
	PBSNCPAction_Tval pbSncpAction;   
}MIBpbSncpConfigTable_t;














































 



 
 
extern MIBDid_t pbSncpStateTable_Did;

   
typedef struct
{
	int		pbSncpStateLineId;                                                    
	char		pbSncpDetailedStateString1[255];   
	char		pbSncpDetailedStateString2[255];   
	char		pbSncpDetailedStateString3[255];   
	char		pbSncpDetailedStateString4[255];   
}MIBpbSncpStateTable_t;












 



 
 
extern MIBDid_t pbDpnpConfigTable_Did;

   
typedef struct
{
	PbXcCapacity_Tval pbDpnpTPType;                                         
	PbSlotNumberType_Tval pbDpnpDstSlotNum;                                 
	PbPortType_Tval pbDpnpDstPortType;                                      
	int		pbDpnpDstPortNum;                                                  
	int		pbDpnpDstSource;                                                   
	PbSlotNumberType_Tval pbVSncpWorkChSlotNum;                             
	PbPortType_Tval pbVSncpWorkChPortType;                                  
	int		pbVSncpWorkChPortNum;                                              
	int		pbVSncpWorkSource;                                                 
	PbSlotNumberType_Tval pbVSncpProtectChSlotNum;                          
	PbPortType_Tval pbVSncpProtectChPortType;                               
	int		pbVSncpProtectChPortNum;                                           
	int		pbVSncpProtectSource;                                              
	PbSlotNumberType_Tval pbVMsspringWorkChSlotNum;                         
	PbPortType_Tval pbVMsspringWorkChPortType;                              
	int		pbVMsspringWorkChPortNum;                                          
	int		pbVMsspringWorkSource;                                             
	PbSNCPSignalDirection_Tval pbDpnpSignalDircetion;                       
	int		pbDpnpGroupId;
	PbSNCPChannelState_Tval pbDpnpWorkChState;
	PbSNCPExternalCmd_Tval pbDpnpExternalCmd;
	PbSNCPExCmdRslt_Tval pbDpnpExCmdRslt;
	PbSNCPCurrentReq_Tval pbDpnpCurSwitchReq;
	PbSNCPSwitchReason_Tval pbDpnpSwitchReason;
	unsigned int	pbDpnpHoldOffTime;
	PbSNCPRestoreMode_Tval pbDpnpRestoreMode;
	unsigned int	pbDpnpWTR;
	PBSNCPSignalState_Tval pbDpnpWorkSignalState;
	PBSNCPSignalState_Tval pbDpnpProtectSignalState;
	PBSNCPSignalState_Tval pbDpnpVMsspringSignalState;
	RowStatus_Tval pbDpnpRowStatus;
	char		pbDpnpLable[64];
	PBSNCPAddRslt_Tval pbDpnpAddRslt;
	unsigned int	pbDpnpDNIId;
	PBSNCPIntrusive_Tval pbDpnpIntrusive;
	PbSncpCircuitDirection_Tval pbDpnpDirection;
	PbSncpFrom_Tval pbDpnpFrom;
	PbDpnpSwitchEvent_Tval pbDpnpSwitchEvent;
	PbGmplsSncpDirection_Tval pbGmplsDpnpDirection;
}MIBpbDpnpConfigTable_t;














































 



 
 
extern MIBDid_t pbDpnpStateTable_Did;

   
typedef struct
{
	int		pbDpnpStateLineId;                                                 
	char		pbDpnpDetailedStateString1[255];
	char		pbDpnpDetailedStateString2[255];
	char		pbDpnpDetailedStateString3[255];
	char		pbDpnpDetailedStateString4[255];
}MIBpbDpnpStateTable_t;












 



 
extern MIBDid_t pbSncpExtendParaGroup_Did;

   
typedef struct
{
	unsigned int	pbLoSncpWTR;
	unsigned int	pbLoSncpHoldOffTime;
}MIBpbSncpExtendParaGroup_t;







 



 
 
extern MIBDid_t pbMspConfigTable_Did;

   
typedef struct
{
	PbPortType_Tval pbMspGroupActPortType;                                
	PbSlotNumberType_Tval pbMspProtectChSlotNum;                             
	int		pbMspProtectChPortNum;                                              
	PbPortType_Tval pbMspProtectChPortType;
	PBMSPSignalState_Tval pbMspProtectChSignalState;
	int		pbMspGroupId;   
	PBMSPType_Tval pbMspType;   
	PbMSPSignalDirection_Tval pbMspSignalDircetion;   
	PbMSPRestoreMode_Tval pbMspRestoreMode;   
	TruthValue_Tval pbMspExTrafficEnable;
	TruthValue_Tval pbMspEnable;
	PbMSPExternalCmd_Tval pbMspExternalCmd;   
	PbMspChNumberType_Tval pbMspExtCmdParam;
	PbMSPExCmdRslt_Tval pbMspExCmdRslt;   
	PbMspChNumberType_Tval pbMspSwitchState;
	PbMSPCurrentReq_Tval pbMspCurSwitchReq;   
	PbMSPSwitchReason_Tval pbMspSwitchReason;   
	unsigned int	pbMspWTR;   
	int		pbMspWorkChNum;
	PbSlotNumberType_Tval pbMspWorkCh1SlotNum;
	int		pbMspWorkCh1PortNum;
	PbPortType_Tval pbMspWorkCh1PortType;
	PBMSPSignalState_Tval pbMspWorkCh1SignalState;
	PBMSPChPriState_Tval pbMspWorkCh1PriState;
	PbSlotNumberType_Tval pbMspWorkCh2SlotNum;
	int		pbMspWorkCh2PortNum;
	PbPortType_Tval pbMspWorkCh2PortType;
	PBMSPSignalState_Tval pbMspWorkCh2SignalState;
	PBMSPChPriState_Tval pbMspWorkCh2PriState;
	PbSlotNumberType_Tval pbMspWorkCh3SlotNum;
	int		pbMspWorkCh3PortNum;
	PbPortType_Tval pbMspWorkCh3PortType;
	PBMSPSignalState_Tval pbMspWorkCh3SignalState;
	PBMSPChPriState_Tval pbMspWorkCh3PriState;
	PbSlotNumberType_Tval pbMspWorkCh4SlotNum;
	int		pbMspWorkCh4PortNum;
	PbPortType_Tval pbMspWorkCh4PortType;
	PBMSPSignalState_Tval pbMspWorkCh4SignalState;
	PBMSPChPriState_Tval pbMspWorkCh4PriState;
	PbSlotNumberType_Tval pbMspWorkCh5SlotNum;
	int		pbMspWorkCh5PortNum;
	PbPortType_Tval pbMspWorkCh5PortType;
	PBMSPSignalState_Tval pbMspWorkCh5SignalState;
	PBMSPChPriState_Tval pbMspWorkCh5PriState;
	PbSlotNumberType_Tval pbMspWorkCh6SlotNum;
	int		pbMspWorkCh6PortNum;
	PbPortType_Tval pbMspWorkCh6PortType;
	PBMSPSignalState_Tval pbMspWorkCh6SignalState;
	PBMSPChPriState_Tval pbMspWorkCh6PriState;
	PbSlotNumberType_Tval pbMspWorkCh7SlotNum;
	int		pbMspWorkCh7PortNum;
	PbPortType_Tval pbMspWorkCh7PortType;
	PBMSPSignalState_Tval pbMspWorkCh7SignalState;
	PBMSPChPriState_Tval pbMspWorkCh7PriState;
	PbSlotNumberType_Tval pbMspWorkCh8SlotNum;
	int		pbMspWorkCh8PortNum;
	PbPortType_Tval pbMspWorkCh8PortType;
	PBMSPSignalState_Tval pbMspWorkCh8SignalState;
	PBMSPChPriState_Tval pbMspWorkCh8PriState;
	PbSlotNumberType_Tval pbMspWorkCh9SlotNum;
	int		pbMspWorkCh9PortNum;
	PbPortType_Tval pbMspWorkCh9PortType;
	PBMSPSignalState_Tval pbMspWorkCh9SignalState;
	PBMSPChPriState_Tval pbMspWorkCh9PriState;
	PbSlotNumberType_Tval pbMspWorkCh10SlotNum;
	int		pbMspWorkCh10PortNum;
	PbPortType_Tval pbMspWorkCh10PortType;
	PBMSPSignalState_Tval pbMspWorkCh10SignalState;
	PBMSPChPriState_Tval pbMspWorkCh10PriState;
	PbSlotNumberType_Tval pbMspWorkCh11SlotNum;
	int		pbMspWorkCh11PortNum;
	PbPortType_Tval pbMspWorkCh11PortType;
	PBMSPSignalState_Tval pbMspWorkCh11SignalState;
	PBMSPChPriState_Tval pbMspWorkCh11PriState;
	PbSlotNumberType_Tval pbMspWorkCh12SlotNum;
	int		pbMspWorkCh12PortNum;
	PbPortType_Tval pbMspWorkCh12PortType;
	PBMSPSignalState_Tval pbMspWorkCh12SignalState;
	PBMSPChPriState_Tval pbMspWorkCh12PriState;
	PbSlotNumberType_Tval pbMspWorkCh13SlotNum;
	int		pbMspWorkCh13PortNum;
	PbPortType_Tval pbMspWorkCh13PortType;
	PBMSPSignalState_Tval pbMspWorkCh13SignalState;
	PBMSPChPriState_Tval pbMspWorkCh13PriState;
	PbSlotNumberType_Tval pbMspWorkCh14SlotNum;
	int		pbMspWorkCh14PortNum;
	PbPortType_Tval pbMspWorkCh14PortType;
	PBMSPSignalState_Tval pbMspWorkCh14SignalState;
	PBMSPChPriState_Tval pbMspWorkCh14PriState;
	TruthValue_Tval pbMspK2Bit5Mismatch;   
	TruthValue_Tval pbMspK1Bit1to4Mismatch;   
	TruthValue_Tval pbMspK1Bit5to8Mismatch;   
	int		pbMspReceiveK1;   
	int		pbMspReceiveK2;   
	int		pbMspSendoutK1;   
	int		pbMspSendoutK2;   
	RowStatus_Tval pbMspRowStatus;   
	TruthValue_Tval pbMspFreeze;
	char		pbMspLable[255];   
}MIBpbMspConfigTable_t;










































































































 



 
 
extern MIBDid_t pbMSMConfigTable_Did;

   
typedef enum {
	pbMSMProtectMode_none=1,
	pbMSMProtectMode_msspring=2,
	pbMSMProtectMode_ulsr=3
} pbMSMProtectMode_Tval;

typedef enum {
	pbMSMConfigSdBerThreshold_ber10_5=1,
	pbMSMConfigSdBerThreshold_ber10_6=2,
	pbMSMConfigSdBerThreshold_ber10_7=3,
	pbMSMConfigSdBerThreshold_ber10_8=4,
	pbMSMConfigSdBerThreshold_ber10_9=5,
	pbMSMConfigSdBerThreshold_global_sd=6
} pbMSMConfigSdBerThreshold_Tval;

typedef enum {
	pbMSMConfigSfBerThreshold_ber10_3=1,
	pbMSMConfigSfBerThreshold_ber10_4=2,
	pbMSMConfigSfBerThreshold_ber10_5=3,
	pbMSMConfigSfBerThreshold_global_sf=4
} pbMSMConfigSfBerThreshold_Tval;

typedef enum {
	pbMSMCardFunction_none=0,
	pbMSMCardFunction_stm_4=4,
	pbMSMCardFunction_stm_16=16,
	pbMSMCardFunction_stm_64=64
} pbMSMCardFunction_Tval;

typedef struct
{
	int		pbMSMSNMRingId;                                                     
	pbMSMProtectMode_Tval pbMSMProtectMode;   
	char		pbMSMSNMRingName[64];   
	int		pbMSMRingIdx;   
	PbSlotNumberType_Tval pbMSMWestSideSlotNum;   
	PbPortType_Tval pbMSMWestSidePortType;   
	int		pbMSMWestSidePortNum;   
	PbSlotNumberType_Tval pbMSMWestProtSlotNum;
	PbPortType_Tval pbMSMWestProtPortType;
	int		pbMSMWestProtPortNum;
	PbSlotNumberType_Tval pbMSMEastSideSlotNum;   
	PbPortType_Tval pbMSMEastSidePortType;   
	int		pbMSMEastSidePortNum;   
	PbSlotNumberType_Tval pbMSMEastProtSlotNum;
	PbPortType_Tval pbMSMEastProtPortType;
	int		pbMSMEastProtPortNum;
	PbOnOff_Tval pbMSMAutoSquelchDiscovery;
	PbOnOff_Tval pbMSMAutoRingDiscovery;
	unsigned int	pbMSMPeerWestAddress;   
	int		pbMSMPeerWestSlotNum;   
	int		pbMSMPeerWestPortType;   
	int		pbMSMPeerWestPortNum;   
	unsigned int	pbMSMPeerEastAddress;   
	int		pbMSMPeerEastSlotNum;   
	int		pbMSMPeerEastPortType;   
	int		pbMSMPeerEastPortNum;   
	pbMSMConfigSdBerThreshold_Tval pbMSMConfigSdBerThreshold;
	pbMSMConfigSfBerThreshold_Tval pbMSMConfigSfBerThreshold;
	int		pbMSMConfigWaitToRestore;   
	PbOnOff_Tval pbMSMConfigExtraTraffic;
	TruthValue_Tval pbMSMConfigEnable;   
	TruthValue_Tval pbMSMConfigDelete;   
	TruthValue_Tval pbMSMSquelchFin;
	pbMSMCardFunction_Tval pbMSMCardFunction;   
	char		pbMSMChannelInfo[255];   
	unsigned int	pbMSMSelfAddress;   
	TruthValue_Tval pbMSMStartAutoSquelch;   
	TruthValue_Tval pbMSMAutoMappingEnable;   
	PbEnableDisable_Tval pbMSMPortMapRefresh;   
}MIBpbMSMConfigTable_t;














































 



 
 
extern MIBDid_t pbMSMStatusTable_Did;

   
typedef enum {
	pbMSMSide_west=0,
	pbMSMSide_east=1
} pbMSMSide_Tval;

typedef enum {
	pbMSMTransBridgeReq_noRequest=0,
	pbMSMTransBridgeReq_reverseRequestRing=1,
	pbMSMTransBridgeReq_reverseRequestSpan=2,
	pbMSMTransBridgeReq_exerciserRing=3,
	pbMSMTransBridgeReq_exerciserSpan=4,
	pbMSMTransBridgeReq_waitToRestore=5,
	pbMSMTransBridgeReq_manualSwitchRing=6,
	pbMSMTransBridgeReq_manualSwitchSpan=7,
	pbMSMTransBridgeReq_signalDegradeRing=8,
	pbMSMTransBridgeReq_signalDegradeSpan=9,
	pbMSMTransBridgeReq_signalDegradeProtection=10,
	pbMSMTransBridgeReq_signalFailRing=11,
	pbMSMTransBridgeReq_signalFailSpan=12,
	pbMSMTransBridgeReq_forcedSwitchRing=13,
	pbMSMTransBridgeReq_forcedSwitchSpan=14,
	pbMSMTransBridgeReq_lockoutProtection=15
} pbMSMTransBridgeReq_Tval;

typedef enum {
	pbMSMTransLongShort_shortPath=0,
	pbMSMTransLongShort_longPath=1
} pbMSMTransLongShort_Tval;

typedef enum {
	pbMSMTransStatus_idle=0,
	pbMSMTransStatus_bridged=1,
	pbMSMTransStatus_bridgedAndSwitched=2,
	pbMSMTransStatus_extraTraffic=3,
	pbMSMTransStatus_msRDI=6,
	pbMSMTransStatus_msAIS=7
} pbMSMTransStatus_Tval;

typedef enum {
	pbMSMReceiveBridgeReq_noRequest=0,
	pbMSMReceiveBridgeReq_reverseRequestRing=1,
	pbMSMReceiveBridgeReq_reverseRequestSpan=2,
	pbMSMReceiveBridgeReq_exerciserRing=3,
	pbMSMReceiveBridgeReq_exerciserSpan=4,
	pbMSMReceiveBridgeReq_waitToRestore=5,
	pbMSMReceiveBridgeReq_manualSwitchRing=6,
	pbMSMReceiveBridgeReq_manualSwitchSpan=7,
	pbMSMReceiveBridgeReq_signalDegradeRing=8,
	pbMSMReceiveBridgeReq_signalDegradeSpan=9,
	pbMSMReceiveBridgeReq_signalDegradeProtection=10,
	pbMSMReceiveBridgeReq_signalFailRing=11,
	pbMSMReceiveBridgeReq_signalFailSpan=12,
	pbMSMReceiveBridgeReq_forcedSwitchRing=13,
	pbMSMReceiveBridgeReq_forcedSwitchSpan=14,
	pbMSMReceiveBridgeReq_lockoutProtection=15
} pbMSMReceiveBridgeReq_Tval;

typedef enum {
	pbMSMReceiveLongShort_shortPath=0,
	pbMSMReceiveLongShort_longPath=1
} pbMSMReceiveLongShort_Tval;

typedef enum {
	pbMSMReceiveStatus_idle=0,
	pbMSMReceiveStatus_bridged=1,
	pbMSMReceiveStatus_bridgedAndSwitched=2,
	pbMSMReceiveStatus_extraTraffic=3,
	pbMSMReceiveStatus_msRDI=6,
	pbMSMReceiveStatus_msAIS=7
} pbMSMReceiveStatus_Tval;

typedef enum {
	pbMSMSideReq_noRequest=0,
	pbMSMSideReq_reverseRequestRing=1,
	pbMSMSideReq_reverseRequestSpan=2,
	pbMSMSideReq_exerciserRing=3,
	pbMSMSideReq_exerciserSpan=4,
	pbMSMSideReq_waitToRestore=5,
	pbMSMSideReq_manualSwitchRing=6,
	pbMSMSideReq_manualSwitchSpan=7,
	pbMSMSideReq_signalDegradeRing=8,
	pbMSMSideReq_signalDegradeSpan=9,
	pbMSMSideReq_signalDegradeProtection=10,
	pbMSMSideReq_signalFailRing=11,
	pbMSMSideReq_signalFailSpan=12,
	pbMSMSideReq_forcedSwitchRing=13,
	pbMSMSideReq_forcedSwitchSpan=14,
	pbMSMSideReq_lockoutProtection=15,
	pbMSMSideReq_reset=20,
	pbMSMSideReq_clear=21,
	pbMSMSideReq_lockoutService=22
} pbMSMSideReq_Tval;

typedef enum {
	pbMSMSideSrc_kByte=0,
	pbMSMSideSrc_line=1,
	pbMSMSideSrc_external=2
} pbMSMSideSrc_Tval;

typedef enum {
	pbMSMSwitchCommand_idle=0,
	pbMSMSwitchCommand_bridge=1,
	pbMSMSwitchCommand_bridgeAndSwitch=2,
	pbMSMSwitchCommand_passthough=3
} pbMSMSwitchCommand_Tval;

typedef enum {
	pbMSMLineReq_signalDegradeRing=8,
	pbMSMLineReq_signalFailRing=11,
	pbMSMLineReq_clear=21
} pbMSMLineReq_Tval;

typedef enum {
	pbMSMCmndReq_manualSwitchRing=6,
	pbMSMCmndReq_forcedSwitchRing=13,
	pbMSMCmndReq_lockoutProtection=15,
	pbMSMCmndReq_reset=21
} pbMSMCmndReq_Tval;

typedef struct
{
	int		pbMSMSNMRingId;                                            
	pbMSMSide_Tval pbMSMSide;                                          
	pbMSMTransBridgeReq_Tval pbMSMTransBridgeReq;   
	int		pbMSMTransDestNodeId;   
	int		pbMSMTransSrcNodeId;   
	pbMSMTransLongShort_Tval pbMSMTransLongShort;   
	pbMSMTransStatus_Tval pbMSMTransStatus;   
	pbMSMReceiveBridgeReq_Tval pbMSMReceiveBridgeReq;   
	int		pbMSMReceiveDestNodeId;   
	int		pbMSMReceiveSrcNodeId;   
	pbMSMReceiveLongShort_Tval pbMSMReceiveLongShort;   
	pbMSMReceiveStatus_Tval pbMSMReceiveStatus;   
	pbMSMSideReq_Tval pbMSMSideReq;   
	pbMSMSideSrc_Tval pbMSMSideSrc;   
	pbMSMSwitchCommand_Tval pbMSMSwitchCommand;   
	pbMSMLineReq_Tval pbMSMLineReq;   
	pbMSMCmndReq_Tval pbMSMCmndReq;   
	TruthValue_Tval pbMSMIsSquelched;   
}MIBpbMSMStatusTable_t;

























 



 
 
extern MIBDid_t pbMSMNodeStatusTable_Did;

   
typedef enum {
	pbMSMNodeKExpressMode_autoLocal=0,
	pbMSMNodeKExpressMode_autoExpress=1,
	pbMSMNodeKExpressMode_forceExpress=2,
	pbMSMNodeKExpressMode_forceLocal=3
} pbMSMNodeKExpressMode_Tval;

typedef struct
{
	int		pbMSMSNMRingId;                                            
	PbMsspAgentState_Tval pbMSMNodeAgentState;   
	PbMsspNodeState_Tval pbMSMNodeState;   
	PbMsspNodeSide_Tval pbMSMNodeCurrentSide;   
	PbMsspRequest_Tval pbMSMNodeCurrentReq;   
	PbMsspRequestSource_Tval pbMSMNodeCurrentSrc;   
	pbMSMNodeKExpressMode_Tval pbMSMNodeKExpressMode;   
	PbMsspRequest_Tval pbMSMEastSwitchCommand;   
	PbMsspLineRequest_Tval pbMSMEastLineReq;   
	PbMsspRequest_Tval pbMSMEastLastSwitchCmd;   
	TruthValue_Tval pbMSMEastIsSquelched;   
	PbMsspRequest_Tval pbMSMWestSwitchCommand;   
	PbMsspLineRequest_Tval pbMSMWestLineReq;   
	PbMsspRequest_Tval pbMSMWestLastSwitchCmd;   
	TruthValue_Tval pbMSMWestIsSquelched;   
	PbMsspSwitchStatus_Tval pbMSMEastSwitchStatus;   
	PbMsspSwitchStatus_Tval pbMSMWestSwitchStatus;   
	int		pbMSMPassNodeId;   
}MIBpbMSMNodeStatusTable_t;

























 



 
 
extern MIBDid_t pbMSMTopoConfigTable_Did;

   
typedef enum {
	pbMSMTopoStatus_unconfigured=0,
	pbMSMTopoStatus_closed=1,
	pbMSMTopoStatus_open=2,
	pbMSMTopoStatus_locallyMisconfigured=3,
	pbMSMTopoStatus_remotelyMisconfigured=4
} pbMSMTopoStatus_Tval;

typedef enum {
	pbMSMTopoConfig_unsuspend=1,
	pbMSMTopoConfig_suspend=2
} pbMSMTopoConfig_Tval;

typedef struct
{
	int		pbMSMSNMRingId;                                                    
	int		pbMSMTopoNumOfNodes;   
	int		pbMSMTopoNodeId0;   
	int		pbMSMTopoNodeId1;   
	int		pbMSMTopoNodeId2;   
	int		pbMSMTopoNodeId3;   
	int		pbMSMTopoNodeId4;   
	int		pbMSMTopoNodeId5;   
	int		pbMSMTopoNodeId6;   
	int		pbMSMTopoNodeId7;   
	int		pbMSMTopoNodeId8;   
	int		pbMSMTopoNodeId9;   
	int		pbMSMTopoNodeId10;   
	int		pbMSMTopoNodeId11;   
	int		pbMSMTopoNodeId12;   
	int		pbMSMTopoNodeId13;   
	int		pbMSMTopoNodeId14;   
	int		pbMSMTopoNodeId15;   
	pbMSMTopoStatus_Tval pbMSMTopoStatus;   
	pbMSMTopoConfig_Tval pbMSMTopoConfig;   
	unsigned int	pbMSMTopoNodeId0IpAddress;   
	unsigned int	pbMSMTopoNodeId1IpAddress;   
	unsigned int	pbMSMTopoNodeId2IpAddress;   
	unsigned int	pbMSMTopoNodeId3IpAddress;   
	unsigned int	pbMSMTopoNodeId4IpAddress;   
	unsigned int	pbMSMTopoNodeId5IpAddress;   
	unsigned int	pbMSMTopoNodeId6IpAddress;   
	unsigned int	pbMSMTopoNodeId7IpAddress;   
	unsigned int	pbMSMTopoNodeId8IpAddress;   
	unsigned int	pbMSMTopoNodeId9IpAddress;   
	unsigned int	pbMSMTopoNodeId10IpAddress;   
	unsigned int	pbMSMTopoNodeId11IpAddress;   
	unsigned int	pbMSMTopoNodeId12IpAddress;   
	unsigned int	pbMSMTopoNodeId13IpAddress;   
	unsigned int	pbMSMTopoNodeId14IpAddress;   
	unsigned int	pbMSMTopoNodeId15IpAddress;   
}MIBpbMSMTopoConfigTable_t;











































 



 
 
extern MIBDid_t pbMSMSquelchTable_Did;

   
typedef enum {
	pbMSMSquelchType_none=0,
	pbMSMSquelchType_add=1,
	pbMSMSquelchType_drop=2,
	pbMSMSquelchType_passthrough=3
} pbMSMSquelchType_Tval;

typedef struct
{
	int		pbMSMSNMRingId;                                            
	PbMsspNodeSide_Tval pbMSMSquelchSide;                              
	PbMsspTrafficDirection_Tval pbMSMSquelchGress;                     
	int		pbMSMSquelchTimeSlot;                                         
	PbMsspTrafficOrder_Tval pbMSMSquelchOrder;   
	pbMSMSquelchType_Tval pbMSMSquelchType;   
	int		pbMSMSquelchSrcNodeId;   
	int		pbMSMSquelchDstNodeId;   
	int		pbMSMSquelchReference;   
	int		pbMSMSquelchOpTimeSlot;   
	RowStatus_Tval pbMSMSquelchRowStatus;   
}MIBpbMSMSquelchTable_t;


















 



 
 
extern MIBDid_t pbMSMCommandTable_Did;

   
typedef enum {
	pbMSMCommandResult_unknown=0,
	pbMSMCommandResult_success=1,
	pbMSMCommandResult_fail=2
} pbMSMCommandResult_Tval;

typedef struct
{
	int		pbMSMSNMRingId;                                              
	PbMsspNodeSide_Tval pbMSMCommandCurrentSide;                         
	PbMsspExternalCommand_Tval pbMSMCommandSwitch;   
	char		pbMSMLastCommandStatus[64];   
	pbMSMCommandResult_Tval pbMSMCommandResult;   
}MIBpbMSMCommandTable_t;












 



 
 
extern MIBDid_t pbAutoMappingTable_Did;

   
typedef struct
{
	int		pbMSMSNMRingId;                                            
	int		pbAutoNodeId;                                                 
	int		pbPortMappingString_len;
	char		pbPortMappingString[768];   
	RowStatus_Tval pbAutoMappingRowStatus;   
}MIBpbAutoMappingTable_t;











 



 
 
extern MIBDid_t pbEpgTable_Did;

   
typedef enum {
	pbEpgType_onevsn=1,
	pbEpgType_oneplusone=2
} pbEpgType_Tval;

typedef enum {
	pbEpgProtMode_none=0,
	pbEpgProtMode_revertive=1,
	pbEpgProtMode_nonrevertive=2
} pbEpgProtMode_Tval;

typedef struct
{
	int		pbEpgId;                                                  
	PbBoardStyle_Tval pbEpgBoardType;   
	pbEpgType_Tval pbEpgType;   
	pbEpgProtMode_Tval pbEpgProtMode;   
	TruthValue_Tval pbEpgEnable;   
	PbEpgSwitchRequest_Tval pbEpgExtCmd;   
	int		pbEpgExtCmdParam;   
	PbEpgCmdResult_Tval pbEpgExtCmdResult;   
	int		pbEpgWtr;   
	PbEpgSwitchRequest_Tval pbEpgPreSwitchReason;   
	PbEpgSwitchRequest_Tval pbEpgCurRequest;   
	PbSlotNumberType_Tval pbEpgProtState;   
	int		pbEpgHoldOffTime;   
	PbSlotNumberType_Tval pbEpgProtSlotNum;   
	PbEpgFunctionCardStatus_Tval pbEpgProtSlotState;   
	PbSlotNumberType_Tval pbEpgWorkSlotNum1;   
	PbEpgFunctionCardStatus_Tval pbEpgWorkSlot1State;   
	PbSlotNumberType_Tval pbEpgWorkSlotNum2;   
	PbEpgFunctionCardStatus_Tval pbEpgWorkSlot2State;   
	PbSlotNumberType_Tval pbEpgWorkSlotNum3;   
	PbEpgFunctionCardStatus_Tval pbEpgWorkSlot3State;   
	PbSlotNumberType_Tval pbEpgWorkSlotNum4;   
	PbEpgFunctionCardStatus_Tval pbEpgWorkSlot4State;   
	PbSlotNumberType_Tval pbEpgWorkSlotNum5;   
	PbEpgFunctionCardStatus_Tval pbEpgWorkSlot5State;   
	PbSlotNumberType_Tval pbEpgWorkSlotNum6;   
	PbEpgFunctionCardStatus_Tval pbEpgWorkSlot6State;   
	PbSlotNumberType_Tval pbEpgWorkSlotNum7;   
	PbEpgFunctionCardStatus_Tval pbEpgWorkSlot7State;   
	PbSlotNumberType_Tval pbEpgWorkSlotNum8;   
	PbEpgFunctionCardStatus_Tval pbEpgWorkSlot8State;   
	RowStatus_Tval pbEpgRowStatus;   
}MIBpbEpgTable_t;







































 



 
 
extern MIBDid_t pbCpgTable_Did;

   
typedef enum {
	pbCpgProtectStatus_worker=0,
	pbCpgProtectStatus_protector=1,
	pbCpgProtectStatus_na=2
} pbCpgProtectStatus_Tval;

typedef enum {
	pbCpgAct_none=0,
	pbCpgAct_force_Worker=1,
	pbCpgAct_force_Protector=2,
	pbCpgAct_manual_Worker=3,
	pbCpgAct_manual_Protector=4,
	pbCpgAct_clear=5,
	pbCpgAct_lockout_protection=6
} pbCpgAct_Tval;

typedef enum {
	pbCpgActStatus_no_Request=0,
	pbCpgActStatus_force_Worker=1,
	pbCpgActStatus_force_Protector=2,
	pbCpgActStatus_manual_Worker=3,
	pbCpgActStatus_manual_Protector=4,
	pbCpgActStatus_ufur_Worker=5,
	pbCpgActStatus_ufur_Protector=6,
	pbCpgActStatus_latchon_Worker=7,
	pbCpgActStatus_latchon_Protector=8,
	pbCpgActStatus_lockout_protection=9
} pbCpgActStatus_Tval;

typedef enum {
	pbCpgCurActStatus_no_Request=0,
	pbCpgCurActStatus_force_Worker=1,
	pbCpgCurActStatus_force_Protector=2,
	pbCpgCurActStatus_manual_Worker=3,
	pbCpgCurActStatus_manual_Protector=4,
	pbCpgCurActStatus_ufur_Worker=5,
	pbCpgCurActStatus_ufur_Protector=6,
	pbCpgCurActStatus_latchon_Worker=7,
	pbCpgCurActStatus_latchon_Protector=8,
	pbCpgCurActStatus_lockout_protection=9
} pbCpgCurActStatus_Tval;

typedef struct
{
	int		pbCpgIndexNo;                                         
	PbSlotNumberType_Tval pbCpgWorkerSlot;   
	PbSlotNumberType_Tval pbCpgProtectorSlot;   
	pbCpgProtectStatus_Tval pbCpgProtectStatus;   
	PbCpgCardStatus_Tval pbWorkerStatus;   
	PbCpgCardStatus_Tval pbProtectorStatus;   
	pbCpgAct_Tval pbCpgAct;   
	pbCpgActStatus_Tval pbCpgActStatus;   
	pbCpgCurActStatus_Tval pbCpgCurActStatus;   
	TruthValue_Tval pbCpgReadyToSwitch;   
}MIBpbCpgTable_t;

















 



 
extern MIBDid_t scEpgGroup_Did;

   
typedef enum {
	scEpgRequest_clear=0,
	scEpgRequest_lockout=1,
	scEpgRequest_manual_switch_to_sc1=2,
	scEpgRequest_manual_switch_to_sc2=3,
	scEpgRequest_no_Request=4
} scEpgRequest_Tval;

typedef enum {
	scEpgLastSwichReason_no_Request=0,
	scEpgLastSwichReason_manual_switch_to_sc1=1,
	scEpgLastSwichReason_manual_switch_to_sc2=2,
	scEpgLastSwichReason_latchon_Switch=3,
	scEpgLastSwichReason_uf2_Switch=4,
	scEpgLastSwichReason_uf1_Switch=5,
	scEpgLastSwichReason_cr_Switch=6,
	scEpgLastSwichReason_lockout_protection=7
} scEpgLastSwichReason_Tval;

typedef enum {
	scEpgStandbySc_failed=0,
	scEpgStandbySc_ok=1
} scEpgStandbySc_Tval;

typedef enum {
	scEpgASCSwithchState_no_switch=0,
	scEpgASCSwithchState_begin_switch=1,
	scEpgASCSwithchState_end_switch=2
} scEpgASCSwithchState_Tval;

typedef struct
{
	PbSlotNumberType_Tval scEpgScWorkerSlot;   
	int		scEpgSc1Status;   
	int		scEpgSc2Status;   
	scEpgRequest_Tval scEpgRequest;   
	scEpgLastSwichReason_Tval scEpgLastSwichReason;   
	scEpgStandbySc_Tval scEpgStandbySc;   
	scEpgASCSwithchState_Tval scEpgASCSwithchState;
}MIBscEpgGroup_t;












 



 
extern MIBDid_t pbSDHProtectionModule_Did;

   



# 42 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_security.h" 1



 




 



 
extern MIBDid_t securityGlobalSettingGroup_Did;

   
typedef enum {
	securityTCBehavior_none=1,
	securityTCBehavior_alarm=2,
	securityTCBehavior_disable_account=3
} securityTCBehavior_Tval;

typedef struct
{
	int		securityPasswordMinLen;   
	int		securityMaxInvalidLogin;   
	int		securityUnauthorizedAccessThreshold;   
	securityTCBehavior_Tval securityTCBehavior;   
}MIBsecurityGlobalSettingGroup_t;









 



 
extern MIBDid_t securityAlarmInhibitGroup_Did;

   
typedef struct
{
	PbEnableDisable_Tval securityULEInhibit;   
	PbEnableDisable_Tval securityUAInhibit;   
	PbEnableDisable_Tval securityAEDInhibit;   
	PbEnableDisable_Tval securityLTWInhibit;   
	PbEnableDisable_Tval securityLFAInhibit;   
}MIBsecurityAlarmInhibitGroup_t;










 



 
extern MIBDid_t snmpSecurityGroup_Did;

   


 



 
extern MIBDid_t securityLogGroup_Did;

   
typedef enum {
	securityLogGetStatus_inactive=0,
	securityLogGetStatus_getStarted=1,
	securityLogGetStatus_getPrepared=2,
	securityLogGetStatus_getCompleted=3,
	securityLogGetStatus_getCompletedNotClear=4
} securityLogGetStatus_Tval;

typedef enum {
	securityLogFullBehavior_wrap_around=1,
	securityLogFullBehavior_stop=2
} securityLogFullBehavior_Tval;

typedef enum {
	securityLogControl_active=1,
	securityLogControl_hold=2,
	securityLogControl_purge=3,
	securityLogControl_trap=4
} securityLogControl_Tval;

typedef struct
{
	int		securityLogMaxSize;
	securityLogGetStatus_Tval securityLogGetStatus;
	int		securityLogFullWarningThreshold;
	securityLogFullBehavior_Tval securityLogFullBehavior;
	securityLogControl_Tval securityLogControl;
}MIBsecurityLogGroup_t;










 



 
 
extern MIBDid_t pbTrapDestinationTable_Did;

   
typedef enum {
	trapDestAccessMode_read=1,
	trapDestAccessMode_write=2
} trapDestAccessMode_Tval;

typedef struct
{
	unsigned int	pbTrapDestIPAddress;                                    
	int		pbTrapDestAlarmPort;                                            
	int		pbTrapDestOperationPort;                                        
	PbUpDown_Tval pbTrapDestAdminStatus;   
	RowStatus_Tval pbTrapDestRowStatus;   
	trapDestAccessMode_Tval trapDestAccessMode;   
	DateTime	trapDestLastWrite;   
	char		trapDestUserName[66];   
	unsigned int	trapDestClientIP;
	unsigned int	trapDestSysUpTime;
}MIBpbTrapDestinationTable_t;

















 



 
extern MIBDid_t writeAccessGroup_Did;

   
typedef struct
{
	int		writeAccessTimeout;   
	int		writeAccessAutoCleanup;
}MIBwriteAccessGroup_t;







 



 
 
extern MIBDid_t portProtectforTCPorUDPTable_Did;

   
typedef enum {
	portProtecttype_tcp=1,
	portProtecttype_udp=2
} portProtecttype_Tval;

typedef struct
{
	int		portProtectIndex;                                        
	portProtecttype_Tval portProtecttype;                         
	char		portProtectDescrption[64];
	RowStatus_Tval portProtectStatus;
}MIBportProtectforTCPorUDPTable_t;











 



 
 
extern MIBDid_t usmUserExtendedTable_Did;

   
typedef struct
{
	int		usmUserEngineID_len;
	char		usmUserEngineID[32];                                                  
	char		usmUserName[32];                                                      
	RowStatus_Tval usmUserExtRowStatus;
	DateTime	usmUserLastSucLogin;
	int		usmUserInvalidLoginAttempts;
	int		usmUserUnauthAccessAttempts;
	int		usmUserPwExpirationDate;
	int		usmUserPwExpirationPeriod;
	int		usmUserAccountExpirationDate;
	int		usmUserID;
}MIBusmUserExtendedTable_t;

















 



 
extern MIBDid_t pbSecurityModule_Did;

   



# 43 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_task.h" 1



 




 



 
extern MIBDid_t tasksGroup_Did;

   
typedef struct
{
	char		applTableVer[30];
}MIBtasksGroup_t;






 



 
 
extern MIBDid_t applTable_Did;

   
typedef enum {
	applStart_none=0,
	applStart_full=1,
	applStart_diag=2,
	applStart_always=3
} applStart_Tval;

typedef struct
{
	int		applIndex;                                       
	PbBoardFunction_Tval applBoardFunction;
	PbBoardProperty_Tval applBoardProperty;
	PbBoardStyle_Tval applBoardStyle;
	PbChassisType_Tval applChassisType;
	char		applName[30];
	int		applNumber;
	char		applFname[15];
	char		applEntryfn[30];
	char		applTaskName[30];
	int		applTaskPri;
	int		applTaskStack;
	int		applLogLevels;
	int		applOptions;
	applStart_Tval applStart;
}MIBapplTable_t;






















 



 
 
extern MIBDid_t aapplTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval aapplSlotNo;              
	int		aapplNumber;                               
	char		aapplName[30];
	int		aapplCpuUsage;
	int		aapplLogLevels;
	int		aapplOptions;
	PbApplState_Tval aapplStatus;
	int		aapplLoadOrder;
}MIBaapplTable_t;















 



 
 
extern MIBDid_t bootpTable_Did;

   
typedef struct
{
	PbBoardStyle_Tval bootpBoardType;                  
	char		bootpFname[20];   
}MIBbootpTable_t;









 



 
 
extern MIBDid_t pbIntVcTable_Did;

   
typedef struct
{
	int		pbIntVcLocalIntf;                                   
	int		pbIntVcLocalVpi;                                    
	int		pbIntVcLocalVci;                                    
	int		pbIntVcRemoteIntf;                                  
	int		pbIntVcRemoteVpi;                                   
	int		pbIntVcRemoteVci;                                   
}MIBpbIntVcTable_t;













 



 
 
extern MIBDid_t pbFileSystemTable_Did;

   
typedef struct
{
	char		pbFileDir[50];                       
	char		pbFileName[50];                      
	int		pbFileSize;   
	int		pbFileSlot;   
}MIBpbFileSystemTable_t;











 



 
 
extern MIBDid_t pbDivaMTable_Did;

   
typedef struct
{
	PbSlotNumberType_Tval pbDivaMslotno;             
	int		pbDivaMportno;                              
	int		pbDivaMmetric;   
}MIBpbDivaMTable_t;










 



 
 
extern MIBDid_t moduleTable_Did;

   
typedef enum {
	moduleStart_none=0,
	moduleStart_full=1,
	moduleStart_diag=2,
	moduleStart_always=3
} moduleStart_Tval;

typedef struct
{
	int		moduleIndex;                                         
	PbBoardFunction_Tval moduleBoardFunction;
	PbBoardProperty_Tval moduleBoardProperty;
	PbBoardStyle_Tval moduleBoardStyle;
	PbChassisType_Tval moduleChassisType;
	char		moduleName[30];
	char		moduleFname[15];
	moduleStart_Tval moduleStart;
	char		moduleInit[30];
}MIBmoduleTable_t;
















 



 
 
extern MIBDid_t pbPortLedsTable_Did;

   
typedef enum {
	pbPortLedColor_off=0,
	pbPortLedColor_green=1,
	pbPortLedColor_yellow=2,
	pbPortLedColor_red=3,
	pbPortLedColor_blinkingGreen=4,
	pbPortLedColor_blinkingYellow=5,
	pbPortLedColor_blinkingRed=6
} pbPortLedColor_Tval;

typedef struct
{
	PbSlotNumberType_Tval pbPortLedProxySlot;                  
	PbSlotNumberType_Tval pbPortLedSlot;                       
	int		pbPortLedIndexNo;                                     
	pbPortLedColor_Tval pbPortLedColor;   
}MIBpbPortLedsTable_t;












# 44 "../../../mib/inc/MIB_cookie.h" 2

# 1 "../../../mib/inc/mib/MIB_pb_private_mgnt.h" 1



 




 



 
extern MIBDid_t pbMgnt_Did;

   
typedef struct
{
	PbEnableDisable_Tval pbTelnetLogin;   
	char		pbPhotonCommunityString[8];   
	char		pbPhotonTrapCommunity[8];   
}MIBpbMgnt_t;








 



 
 
extern MIBDid_t pbUserTable_Did;

   
typedef struct
{
	char		pbUserName[64];                                                  
	char		pbUserPassword[64];   
	int		pbUserInactivityTimeout;   
	PbUserAccessLevel_Tval pbUserAccessLevel;   
	TruthValue_Tval pbUserLoggedIn;   
	int		pbUserTimeLoggedIn;   
	int		pbUserLastAccessed;   
	TruthValue_Tval pbUserAccessBrowser;   
	TruthValue_Tval pbUserAccessCraft;   
	TruthValue_Tval pbUserAccessTL1;   
	TruthValue_Tval pbUserAccessFTP;   
	TruthValue_Tval pbUserAccessCLI;   
	PbEnableDisable_Tval pbCriticalAudible;   
	PbEnableDisable_Tval pbMajorAudible;   
	PbEnableDisable_Tval pbMinorAudible;   
	PbEnableDisable_Tval pbWarningAudible;   
	PbEnableDisable_Tval pbInfoAudible;   
	PbEnableDisable_Tval pbCriticalVisual;   
	PbEnableDisable_Tval pbMajorVisual;   
	PbEnableDisable_Tval pbMinorVisual;   
	PbEnableDisable_Tval pbWarningVisual;   
	PbEnableDisable_Tval pbInfoVisual;   
	PbUserAccessLevel_Tval pbUserCurrentAccessLevel;   
	RowStatus_Tval pbUserRowStatus;   
	int		pbUserLoginCount;   
}MIBpbUserTable_t;
































 



 
 
extern MIBDid_t pbCommunityStrTable_Did;

   
typedef struct
{
	char		pbCommunityStringName[32];                                               
	PbCommunityStringAccessLevel_Tval pbCommunityStringAccessLevel;   
	RowStatus_Tval pbCommunityStringRowStatus;   
	char		pbCommunityStringComment[64];   
}MIBpbCommunityStrTable_t;











 



 
extern MIBDid_t pbPrivateManagement_Did;

   



# 45 "../../../mib/inc/MIB_cookie.h" 2

typedef struct {
	MIBDID did;
	int    last_oid;
} LeafInfo;

extern LeafInfo pbUserTable_cookie;
extern LeafInfo pbTelnetLogin_cookie;
extern LeafInfo pbCommunityStrTable_cookie;
extern LeafInfo pbPhotonCommunityString_cookie;
extern LeafInfo pbPhotonTrapCommunity_cookie;
extern LeafInfo pbCommunityStringName_cookie;
extern LeafInfo pbCommunityStringAccessLevel_cookie;
extern LeafInfo pbCommunityStringRowStatus_cookie;
extern LeafInfo pbCommunityStringComment_cookie;
extern LeafInfo pbUserName_cookie;
extern LeafInfo pbUserPassword_cookie;
extern LeafInfo pbUserInactivityTimeout_cookie;
extern LeafInfo pbUserAccessLevel_cookie;
extern LeafInfo pbUserLoggedIn_cookie;
extern LeafInfo pbUserTimeLoggedIn_cookie;
extern LeafInfo pbUserLastAccessed_cookie;
extern LeafInfo pbUserAccessBrowser_cookie;
extern LeafInfo pbUserAccessCraft_cookie;
extern LeafInfo pbUserAccessTL1_cookie;
extern LeafInfo pbUserAccessFTP_cookie;
extern LeafInfo pbUserAccessCLI_cookie;
extern LeafInfo pbCriticalAudible_cookie;
extern LeafInfo pbMajorAudible_cookie;
extern LeafInfo pbMinorAudible_cookie;
extern LeafInfo pbWarningAudible_cookie;
extern LeafInfo pbInfoAudible_cookie;
extern LeafInfo pbCriticalVisual_cookie;
extern LeafInfo pbMajorVisual_cookie;
extern LeafInfo pbMinorVisual_cookie;
extern LeafInfo pbWarningVisual_cookie;
extern LeafInfo pbInfoVisual_cookie;
extern LeafInfo pbUserCurrentAccessLevel_cookie;
extern LeafInfo pbUserRowStatus_cookie;
extern LeafInfo pbUserLoginCount_cookie;
extern LeafInfo dod_cookie;
extern LeafInfo internet_cookie;
extern LeafInfo mgmt_cookie;
extern LeafInfo private_cookie;
extern LeafInfo snmpV2_cookie;
extern LeafInfo snmpModules_cookie;
extern LeafInfo snmpMIB_cookie;
extern LeafInfo snmpFrameworkMIB_cookie;
extern LeafInfo snmpMPDMIB_cookie;
extern LeafInfo snmpTargetMIB_cookie;
extern LeafInfo snmpNotificationMIB_cookie;
extern LeafInfo snmpUsmMIB_cookie;
extern LeafInfo snmpVacmMIB_cookie;
extern LeafInfo snmpCommunityMIB_cookie;
extern LeafInfo snmpCommunityMIBObjects_cookie;
extern LeafInfo snmpCommunityMIBConformance_cookie;
extern LeafInfo snmpCommunityMIBCompliances_cookie;
extern LeafInfo snmpCommunityMIBGroups_cookie;
extern LeafInfo snmpCommunityTable_cookie;
extern LeafInfo snmpTargetAddrExtTable_cookie;
extern LeafInfo snmpTrapAddress_cookie;
extern LeafInfo snmpTrapCommunity_cookie;
extern LeafInfo snmpTargetAddrName_cookie;
extern LeafInfo snmpTargetAddrTMask_cookie;
extern LeafInfo snmpTargetAddrMMS_cookie;
extern LeafInfo snmpCommunityIndex_cookie;
extern LeafInfo snmpCommunityName_cookie;
extern LeafInfo snmpCommunitySecurityName_cookie;
extern LeafInfo snmpCommunityContextEngineID_cookie;
extern LeafInfo snmpCommunityContextName_cookie;
extern LeafInfo snmpCommunityTransportTag_cookie;
extern LeafInfo snmpCommunityStorageType_cookie;
extern LeafInfo snmpCommunityStatus_cookie;
extern LeafInfo vacmMIBObjects_cookie;
extern LeafInfo vacmMIBConformance_cookie;
extern LeafInfo vacmMIBCompliances_cookie;
extern LeafInfo vacmMIBGroups_cookie;
extern LeafInfo vacmContextTable_cookie;
extern LeafInfo vacmSecurityToGroupTable_cookie;
extern LeafInfo vacmAccessTable_cookie;
extern LeafInfo vacmMIBViews_cookie;
extern LeafInfo vacmViewSpinLock_cookie;
extern LeafInfo vacmViewTreeFamilyTable_cookie;
extern LeafInfo vacmViewTreeFamilyViewName_cookie;
extern LeafInfo vacmViewTreeFamilySubtree_cookie;
extern LeafInfo vacmViewTreeFamilyMask_cookie;
extern LeafInfo vacmViewTreeFamilyType_cookie;
extern LeafInfo vacmViewTreeFamilyStorageType_cookie;
extern LeafInfo vacmViewTreeFamilyStatus_cookie;
extern LeafInfo vacmGroupName_cookie;
extern LeafInfo vacmAccessContextPrefix_cookie;
extern LeafInfo vacmAccessSecurityModel_cookie;
extern LeafInfo vacmAccessSecurityLevel_cookie;
extern LeafInfo vacmAccessContextMatch_cookie;
extern LeafInfo vacmAccessReadViewName_cookie;
extern LeafInfo vacmAccessWriteViewName_cookie;
extern LeafInfo vacmAccessNotifyViewName_cookie;
extern LeafInfo vacmAccessStorageType_cookie;
extern LeafInfo vacmAccessStatus_cookie;
extern LeafInfo vacmSecurityModel_cookie;
extern LeafInfo vacmSecurityName_cookie;
extern LeafInfo vacmGroupName_cookie;
extern LeafInfo vacmSecurityToGroupStorageType_cookie;
extern LeafInfo vacmSecurityToGroupStatus_cookie;
extern LeafInfo vacmContextName_cookie;
extern LeafInfo usmMIBObjects_cookie;
extern LeafInfo usmMIBConformance_cookie;
extern LeafInfo usmMIBCompliances_cookie;
extern LeafInfo usmMIBGroups_cookie;
extern LeafInfo usmStats_cookie;
extern LeafInfo usmUser_cookie;
extern LeafInfo usmUserSpinLock_cookie;
extern LeafInfo usmUserTable_cookie;
extern LeafInfo usmUserEngineID_cookie;
extern LeafInfo usmUserName_cookie;
extern LeafInfo usmUserSecurityName_cookie;
extern LeafInfo usmUserCloneFrom_cookie;
extern LeafInfo usmUserAuthProtocol_cookie;
extern LeafInfo usmUserAuthKeyChange_cookie;
extern LeafInfo usmUserOwnAuthKeyChange_cookie;
extern LeafInfo usmUserPrivProtocol_cookie;
extern LeafInfo usmUserPrivKeyChange_cookie;
extern LeafInfo usmUserOwnPrivKeyChange_cookie;
extern LeafInfo usmUserPublic_cookie;
extern LeafInfo usmUserStorageType_cookie;
extern LeafInfo usmUserStatus_cookie;
extern LeafInfo usmStatsUnsupportedSecLevels_cookie;
extern LeafInfo usmStatsNotInTimeWindows_cookie;
extern LeafInfo usmStatsUnknownUserNames_cookie;
extern LeafInfo usmStatsUnknownEngineIDs_cookie;
extern LeafInfo usmStatsWrongDigests_cookie;
extern LeafInfo usmStatsDecryptionErrors_cookie;
extern LeafInfo snmpNotifyObjects_cookie;
extern LeafInfo snmpNotifyConformance_cookie;
extern LeafInfo snmpNotifyCompliances_cookie;
extern LeafInfo snmpNotifyGroups_cookie;
extern LeafInfo snmpNotifyTable_cookie;
extern LeafInfo snmpNotifyFilterProfileTable_cookie;
extern LeafInfo snmpNotifyFilterTable_cookie;
extern LeafInfo snmpNotifyFilterProfileName_cookie;
extern LeafInfo snmpNotifyFilterSubtree_cookie;
extern LeafInfo snmpNotifyFilterMask_cookie;
extern LeafInfo snmpNotifyFilterType_cookie;
extern LeafInfo snmpNotifyFilterStorageType_cookie;
extern LeafInfo snmpNotifyFilterRowStatus_cookie;
extern LeafInfo snmpTargetParamsName_cookie;
extern LeafInfo snmpNotifyFilterProfileName_cookie;
extern LeafInfo snmpNotifyFilterProfileStorType_cookie;
extern LeafInfo snmpNotifyFilterProfileRowStatus_cookie;
extern LeafInfo snmpNotifyName_cookie;
extern LeafInfo snmpNotifyTag_cookie;
extern LeafInfo snmpNotifyType_cookie;
extern LeafInfo snmpNotifyStorageType_cookie;
extern LeafInfo snmpNotifyRowStatus_cookie;
extern LeafInfo snmpTargetObjects_cookie;
extern LeafInfo snmpTargetConformance_cookie;
extern LeafInfo snmpTargetCompliances_cookie;
extern LeafInfo snmpTargetGroups_cookie;
extern LeafInfo snmpTargetSpinLock_cookie;
extern LeafInfo snmpTargetAddrTable_cookie;
extern LeafInfo snmpTargetParamsTable_cookie;
extern LeafInfo snmpUnavailableContexts_cookie;
extern LeafInfo snmpUnknownContexts_cookie;
extern LeafInfo snmpTargetParamsName_cookie;
extern LeafInfo snmpTargetParamsMPModel_cookie;
extern LeafInfo snmpTargetParamsSecurityModel_cookie;
extern LeafInfo snmpTargetParamsSecurityName_cookie;
extern LeafInfo snmpTargetParamsSecurityLevel_cookie;
extern LeafInfo snmpTargetParamsStorageType_cookie;
extern LeafInfo snmpTargetParamsRowStatus_cookie;
extern LeafInfo snmpTargetAddrName_cookie;
extern LeafInfo snmpTargetAddrTDomain_cookie;
extern LeafInfo snmpTargetAddrTAddress_cookie;
extern LeafInfo snmpTargetAddrTimeout_cookie;
extern LeafInfo snmpTargetAddrRetryCount_cookie;
extern LeafInfo snmpTargetAddrTagList_cookie;
extern LeafInfo snmpTargetAddrParams_cookie;
extern LeafInfo snmpTargetAddrStorageType_cookie;
extern LeafInfo snmpTargetAddrRowStatus_cookie;
extern LeafInfo snmpMPDAdmin_cookie;
extern LeafInfo snmpMPDMIBObjects_cookie;
extern LeafInfo snmpMPDMIBConformance_cookie;
extern LeafInfo snmpMPDMIBCompliances_cookie;
extern LeafInfo snmpMPDMIBGroups_cookie;
extern LeafInfo snmpMPDStats_cookie;
extern LeafInfo snmpUnknownSecurityModels_cookie;
extern LeafInfo snmpInvalidMsgs_cookie;
extern LeafInfo snmpUnknownPDUHandlers_cookie;
extern LeafInfo snmpFrameworkAdmin_cookie;
extern LeafInfo snmpFrameworkMIBObjects_cookie;
extern LeafInfo snmpFrameworkMIBConformance_cookie;
extern LeafInfo snmpFrameworkMIBCompliances_cookie;
extern LeafInfo snmpFrameworkMIBGroups_cookie;
extern LeafInfo snmpEngine_cookie;
extern LeafInfo snmpEngineID_cookie;
extern LeafInfo snmpEngineBoots_cookie;
extern LeafInfo snmpEngineTime_cookie;
extern LeafInfo snmpEngineMaxMessageSize_cookie;
extern LeafInfo snmpAuthProtocols_cookie;
extern LeafInfo snmpPrivProtocols_cookie;
extern LeafInfo usmNoPrivProtocol_cookie;
extern LeafInfo usmDESPrivProtocol_cookie;
extern LeafInfo usmNoAuthProtocol_cookie;
extern LeafInfo usmHMACMD5AuthProtocol_cookie;
extern LeafInfo usmHMACSHAAuthProtocol_cookie;
extern LeafInfo snmpMIBObjects_cookie;
extern LeafInfo snmpMIBConformance_cookie;
extern LeafInfo snmpMIBCompliances_cookie;
extern LeafInfo snmpMIBGroups_cookie;
extern LeafInfo snmpTrap_cookie;
extern LeafInfo snmpTraps_cookie;
extern LeafInfo snmpSet_cookie;
extern LeafInfo snmpSetSerialNo_cookie;
extern LeafInfo snmpTrapOID_cookie;
extern LeafInfo snmpTrapEnterprise_cookie;
extern LeafInfo enterprises_cookie;
extern LeafInfo enterpriseRoot_cookie;
extern LeafInfo enterpriseCommonRoot_cookie;
extern LeafInfo enterpriseReg_cookie;
extern LeafInfo enterpriseGeneric_cookie;
extern LeafInfo enterpriseProducts_cookie;
extern LeafInfo enterpriseCaps_cookie;
extern LeafInfo enterpriseReqs_cookie;
extern LeafInfo enterpriseExpr_cookie;
extern LeafInfo productSeries1Products_cookie;
extern LeafInfo productSeries1Common_cookie;
extern LeafInfo productSeries1CmnConf_cookie;
extern LeafInfo productSeries1CmnObjs_cookie;
extern LeafInfo productSeries1CmnEvents_cookie;
extern LeafInfo productSeries1CmnTCs_cookie;
extern LeafInfo productSeries1TrapGroup_cookie;
extern LeafInfo pbTrapTimeStamp_cookie;
extern LeafInfo pbNode_cookie;
extern LeafInfo pbEvents_cookie;
extern LeafInfo pbCircuitBasedTransport_cookie;
extern LeafInfo pbTMS_cookie;
extern LeafInfo pbOspf_cookie;
extern LeafInfo pbMoln_cookie;
extern LeafInfo pbDcc_cookie;
extern LeafInfo pbEthernet_cookie;
extern LeafInfo pbDiagnostics_cookie;
extern LeafInfo pbOpticsMonitor_cookie;
extern LeafInfo pbPowerCircuitMonitor_cookie;
extern LeafInfo pbOr_cookie;
extern LeafInfo pbTrunk_cookie;
extern LeafInfo pbXcExtend_cookie;
extern LeafInfo pbSdhXcTable_cookie;
extern LeafInfo pbRpr_cookie;
extern LeafInfo pbOAObjects_cookie;
extern LeafInfo pbOsi_cookie;
extern LeafInfo pbIpTunnel_cookie;
extern LeafInfo pbProtection_cookie;
extern LeafInfo pbSecurity_cookie;
extern LeafInfo securityGlobalSettingGroup_cookie;
extern LeafInfo securityAlarmInhibitGroup_cookie;
extern LeafInfo snmpSecurityGroup_cookie;
extern LeafInfo securityLogGroup_cookie;
extern LeafInfo pbTrapDestinationTable_cookie;
extern LeafInfo writeAccessGroup_cookie;
extern LeafInfo portProtectforTCPorUDPTable_cookie;
extern LeafInfo portProtectIndex_cookie;
extern LeafInfo portProtecttype_cookie;
extern LeafInfo portProtectDescrption_cookie;
extern LeafInfo portProtectStatus_cookie;
extern LeafInfo writeAccessTimeout_cookie;
extern LeafInfo writeAccessAutoCleanup_cookie;
extern LeafInfo pbTrapDestIPAddress_cookie;
extern LeafInfo pbTrapDestAlarmPort_cookie;
extern LeafInfo pbTrapDestOperationPort_cookie;
extern LeafInfo pbTrapDestAdminStatus_cookie;
extern LeafInfo pbTrapDestRowStatus_cookie;
extern LeafInfo trapDestAccessMode_cookie;
extern LeafInfo trapDestLastWrite_cookie;
extern LeafInfo trapDestUserName_cookie;
extern LeafInfo trapDestClientIP_cookie;
extern LeafInfo trapDestSysUpTime_cookie;
extern LeafInfo securityLogMaxSize_cookie;
extern LeafInfo securityLogGetStatus_cookie;
extern LeafInfo securityLogFullWarningThreshold_cookie;
extern LeafInfo securityLogFullBehavior_cookie;
extern LeafInfo securityLogControl_cookie;
extern LeafInfo usmUserExtendedTable_cookie;
extern LeafInfo usmUserEngineID_cookie;
extern LeafInfo usmUserName_cookie;
extern LeafInfo usmUserExtRowStatus_cookie;
extern LeafInfo usmUserLastSucLogin_cookie;
extern LeafInfo usmUserInvalidLoginAttempts_cookie;
extern LeafInfo usmUserUnauthAccessAttempts_cookie;
extern LeafInfo usmUserPwExpirationDate_cookie;
extern LeafInfo usmUserPwExpirationPeriod_cookie;
extern LeafInfo usmUserAccountExpirationDate_cookie;
extern LeafInfo usmUserID_cookie;
extern LeafInfo securityULEInhibit_cookie;
extern LeafInfo securityUAInhibit_cookie;
extern LeafInfo securityAEDInhibit_cookie;
extern LeafInfo securityLTWInhibit_cookie;
extern LeafInfo securityLFAInhibit_cookie;
extern LeafInfo securityPasswordMinLen_cookie;
extern LeafInfo securityMaxInvalidLogin_cookie;
extern LeafInfo securityUnauthorizedAccessThreshold_cookie;
extern LeafInfo securityTCBehavior_cookie;
extern LeafInfo epgGroup_cookie;
extern LeafInfo msspringGroup_cookie;
extern LeafInfo mspGroup_cookie;
extern LeafInfo sncpGroup_cookie;
extern LeafInfo mssp4fGroup_cookie;
extern LeafInfo mssp4fConfigTable_cookie;
extern LeafInfo mssp4fTopologyTable_cookie;
extern LeafInfo mssp4fCommandTable_cookie;
extern LeafInfo mssp4fNodeStatusTable_cookie;
extern LeafInfo mssp4fAutoMappingTable_cookie;
extern LeafInfo mssp4fXcTable_cookie;
extern LeafInfo mssp4fRingId_cookie;
extern LeafInfo mssp4fNodeId_cookie;
extern LeafInfo mssp4fXcSide_cookie;
extern LeafInfo mssp4fXcTimeSlot_cookie;
extern LeafInfo mssp4fXcOrder_cookie;
extern LeafInfo mssp4fXcType_cookie;
extern LeafInfo mssp4fRingId_cookie;
extern LeafInfo mssp4fNodeId_cookie;
extern LeafInfo mssp4fWestWorkMappingString_cookie;
extern LeafInfo mssp4fEastWorkMappingString_cookie;
extern LeafInfo mssp4fAutomappingRowStatus_cookie;
extern LeafInfo mssp4fRingId_cookie;
extern LeafInfo mssp4fNodeAgentState_cookie;
extern LeafInfo mssp4fNodeState_cookie;
extern LeafInfo mssp4fNodeCurrentSide_cookie;
extern LeafInfo mssp4fNodeCurrentReq_cookie;
extern LeafInfo mssp4fNodeCurrentSrc_cookie;
extern LeafInfo mssp4fNodeKExpressMode_cookie;
extern LeafInfo mssp4fWestSwitchCommand_cookie;
extern LeafInfo mssp4fWestLineReq_cookie;
extern LeafInfo mssp4fWestLastSwitchCmd_cookie;
extern LeafInfo mssp4fWestSwitchStatus_cookie;
extern LeafInfo mssp4fWestK1byteInput_cookie;
extern LeafInfo mssp4fWestK2byteInput_cookie;
extern LeafInfo mssp4fWestK1byteOutput_cookie;
extern LeafInfo mssp4fWestK2byteOutput_cookie;
extern LeafInfo mssp4fEastSwitchCommand_cookie;
extern LeafInfo mssp4fEastLineReq_cookie;
extern LeafInfo mssp4fEastLastSwitchCmd_cookie;
extern LeafInfo mssp4fEastSwitchStatus_cookie;
extern LeafInfo mssp4fEastK1byteInput_cookie;
extern LeafInfo mssp4fEastK2byteInput_cookie;
extern LeafInfo mssp4fEastK1byteOutput_cookie;
extern LeafInfo mssp4fEastK2byteOutput_cookie;
extern LeafInfo mssp4fPassNodeId_cookie;
extern LeafInfo mssp4fRingId_cookie;
extern LeafInfo mssp4fCommandCurrentSide_cookie;
extern LeafInfo mssp4fCommand_cookie;
extern LeafInfo mssp4fLastCommandStatus_cookie;
extern LeafInfo mssp4fRingId_cookie;
extern LeafInfo mssp4fSumOfNodes_cookie;
extern LeafInfo mssp4fNodeId0_cookie;
extern LeafInfo mssp4fNodeId1_cookie;
extern LeafInfo mssp4fNodeId2_cookie;
extern LeafInfo mssp4fNodeId3_cookie;
extern LeafInfo mssp4fNodeId4_cookie;
extern LeafInfo mssp4fNodeId5_cookie;
extern LeafInfo mssp4fNodeId6_cookie;
extern LeafInfo mssp4fNodeId7_cookie;
extern LeafInfo mssp4fNodeId8_cookie;
extern LeafInfo mssp4fNodeId9_cookie;
extern LeafInfo mssp4fNodeId10_cookie;
extern LeafInfo mssp4fNodeId11_cookie;
extern LeafInfo mssp4fNodeId12_cookie;
extern LeafInfo mssp4fNodeId13_cookie;
extern LeafInfo mssp4fNodeId14_cookie;
extern LeafInfo mssp4fNodeId15_cookie;
extern LeafInfo mssp4fNodeIp0_cookie;
extern LeafInfo mssp4fNodeIp1_cookie;
extern LeafInfo mssp4fNodeIp2_cookie;
extern LeafInfo mssp4fNodeIp3_cookie;
extern LeafInfo mssp4fNodeIp4_cookie;
extern LeafInfo mssp4fNodeIp5_cookie;
extern LeafInfo mssp4fNodeIp6_cookie;
extern LeafInfo mssp4fNodeIp7_cookie;
extern LeafInfo mssp4fNodeIp8_cookie;
extern LeafInfo mssp4fNodeIp9_cookie;
extern LeafInfo mssp4fNodeIp10_cookie;
extern LeafInfo mssp4fNodeIp11_cookie;
extern LeafInfo mssp4fNodeIp12_cookie;
extern LeafInfo mssp4fNodeIp13_cookie;
extern LeafInfo mssp4fNodeIp14_cookie;
extern LeafInfo mssp4fNodeIp15_cookie;
extern LeafInfo mssp4fRingId_cookie;
extern LeafInfo mssp4fProtectMode_cookie;
extern LeafInfo mssp4fRingName_cookie;
extern LeafInfo mssp4fGroupId_cookie;
extern LeafInfo mssp4fWaitToRestore_cookie;
extern LeafInfo mssp4fRingType_cookie;
extern LeafInfo mssp4fWestWorkChannelNutInfo_cookie;
extern LeafInfo mssp4fEastWorkChannelNutInfo_cookie;
extern LeafInfo mssp4fConfigEnabled_cookie;
extern LeafInfo mssp4fConfigDeleted_cookie;
extern LeafInfo mssp4fRingSuspended_cookie;
extern LeafInfo mssp4fAutoSquelchEnabled_cookie;
extern LeafInfo mssp4fAutoMappingEnabled_cookie;
extern LeafInfo mssp4fWestWorkSlotNum_cookie;
extern LeafInfo mssp4fWestWorkPortType_cookie;
extern LeafInfo mssp4fWestWorkPortNum_cookie;
extern LeafInfo mssp4fWestProtSlotNum_cookie;
extern LeafInfo mssp4fWestProtPortType_cookie;
extern LeafInfo mssp4fWestProtPortNum_cookie;
extern LeafInfo mssp4fEastWorkSlotNum_cookie;
extern LeafInfo mssp4fEastWorkPortType_cookie;
extern LeafInfo mssp4fEastWorkPortNum_cookie;
extern LeafInfo mssp4fEastProtSlotNum_cookie;
extern LeafInfo mssp4fEastProtPortType_cookie;
extern LeafInfo mssp4fEastProtPortNum_cookie;
extern LeafInfo mssp4fPeerWestWorkSlotNum_cookie;
extern LeafInfo mssp4fPeerWestWorkPortType_cookie;
extern LeafInfo mssp4fPeerWestWorkPortNum_cookie;
extern LeafInfo mssp4fPeerWestProtSlotNum_cookie;
extern LeafInfo mssp4fPeerWestProtPortType_cookie;
extern LeafInfo mssp4fPeerWestProtPortNum_cookie;
extern LeafInfo mssp4fPeerEastWorkSlotNum_cookie;
extern LeafInfo mssp4fPeerEastWorkPortType_cookie;
extern LeafInfo mssp4fPeerEastWorkPortNum_cookie;
extern LeafInfo mssp4fPeerEastProtSlotNum_cookie;
extern LeafInfo mssp4fPeerEastProtPortType_cookie;
extern LeafInfo mssp4fPeerEastProtPortNum_cookie;
extern LeafInfo pbSncpConfigTable_cookie;
extern LeafInfo pbSncpStateTable_cookie;
extern LeafInfo pbDpnpConfigTable_cookie;
extern LeafInfo pbDpnpStateTable_cookie;
extern LeafInfo pbSncpExtendParaGroup_cookie;
extern LeafInfo pbLoSncpWTR_cookie;
extern LeafInfo pbLoSncpHoldOffTime_cookie;
extern LeafInfo pbDpnpStateLineId_cookie;
extern LeafInfo pbDpnpDetailedStateString1_cookie;
extern LeafInfo pbDpnpDetailedStateString2_cookie;
extern LeafInfo pbDpnpDetailedStateString3_cookie;
extern LeafInfo pbDpnpDetailedStateString4_cookie;
extern LeafInfo pbDpnpTPType_cookie;
extern LeafInfo pbDpnpDstSlotNum_cookie;
extern LeafInfo pbDpnpDstPortType_cookie;
extern LeafInfo pbDpnpDstPortNum_cookie;
extern LeafInfo pbDpnpDstSource_cookie;
extern LeafInfo pbVSncpWorkChSlotNum_cookie;
extern LeafInfo pbVSncpWorkChPortType_cookie;
extern LeafInfo pbVSncpWorkChPortNum_cookie;
extern LeafInfo pbVSncpWorkSource_cookie;
extern LeafInfo pbVSncpProtectChSlotNum_cookie;
extern LeafInfo pbVSncpProtectChPortType_cookie;
extern LeafInfo pbVSncpProtectChPortNum_cookie;
extern LeafInfo pbVSncpProtectSource_cookie;
extern LeafInfo pbVMsspringWorkChSlotNum_cookie;
extern LeafInfo pbVMsspringWorkChPortType_cookie;
extern LeafInfo pbVMsspringWorkChPortNum_cookie;
extern LeafInfo pbVMsspringWorkSource_cookie;
extern LeafInfo pbDpnpSignalDircetion_cookie;
extern LeafInfo pbDpnpGroupId_cookie;
extern LeafInfo pbDpnpWorkChState_cookie;
extern LeafInfo pbDpnpExternalCmd_cookie;
extern LeafInfo pbDpnpExCmdRslt_cookie;
extern LeafInfo pbDpnpCurSwitchReq_cookie;
extern LeafInfo pbDpnpSwitchReason_cookie;
extern LeafInfo pbDpnpHoldOffTime_cookie;
extern LeafInfo pbDpnpRestoreMode_cookie;
extern LeafInfo pbDpnpWTR_cookie;
extern LeafInfo pbDpnpWorkSignalState_cookie;
extern LeafInfo pbDpnpProtectSignalState_cookie;
extern LeafInfo pbDpnpVMsspringSignalState_cookie;
extern LeafInfo pbDpnpRowStatus_cookie;
extern LeafInfo pbDpnpLable_cookie;
extern LeafInfo pbDpnpAddRslt_cookie;
extern LeafInfo pbDpnpDNIId_cookie;
extern LeafInfo pbDpnpIntrusive_cookie;
extern LeafInfo pbDpnpDirection_cookie;
extern LeafInfo pbDpnpFrom_cookie;
extern LeafInfo pbDpnpSwitchEvent_cookie;
extern LeafInfo pbGmplsDpnpDirection_cookie;
extern LeafInfo pbSncpStateLineId_cookie;
extern LeafInfo pbSncpDetailedStateString1_cookie;
extern LeafInfo pbSncpDetailedStateString2_cookie;
extern LeafInfo pbSncpDetailedStateString3_cookie;
extern LeafInfo pbSncpDetailedStateString4_cookie;
extern LeafInfo pbSncpTPType_cookie;
extern LeafInfo pbSncpWorkChSlotNum_cookie;
extern LeafInfo pbSncpWorkChPortType_cookie;
extern LeafInfo pbSncpWorkChPortNum_cookie;
extern LeafInfo pbSncpWorkSource_cookie;
extern LeafInfo pbSncpProtectChSlotNum_cookie;
extern LeafInfo pbSncpProtectChPortType_cookie;
extern LeafInfo pbSncpProtectChPortNum_cookie;
extern LeafInfo pbSncpProtectSource_cookie;
extern LeafInfo pbSncpSrcOrDstSlotNum_cookie;
extern LeafInfo pbSncpSrcOrDstPortType_cookie;
extern LeafInfo pbSncpSrcOrDstPortNum_cookie;
extern LeafInfo pbSncpSrcOrDstSource_cookie;
extern LeafInfo pbSncpSignalDircetion_cookie;
extern LeafInfo pbSncpGroupId_cookie;
extern LeafInfo pbSncpWorkChState_cookie;
extern LeafInfo pbSncpExternalCmd_cookie;
extern LeafInfo pbSncpExCmdRslt_cookie;
extern LeafInfo pbSncpCurSwitchReq_cookie;
extern LeafInfo pbSncpSwitchReason_cookie;
extern LeafInfo pbSncpHoldOffTime_cookie;
extern LeafInfo pbSncpRestoreMode_cookie;
extern LeafInfo pbSncpWTR_cookie;
extern LeafInfo pbSncpWorkSignalState_cookie;
extern LeafInfo pbSncpProtectSignalState_cookie;
extern LeafInfo pbSncpRowStatus_cookie;
extern LeafInfo pbSncpLable_cookie;
extern LeafInfo pbSncpAddRslt_cookie;
extern LeafInfo pbDNIId_cookie;
extern LeafInfo pbSncpIntrusive_cookie;
extern LeafInfo pbSncpWorkGmplsVcNumber_cookie;
extern LeafInfo pbSncpWorkGmplsCallName_cookie;
extern LeafInfo pbSncpProtectGmplsVcNumber_cookie;
extern LeafInfo pbSncpProtectGmplsCallName_cookie;
extern LeafInfo pbSncpWorkorProtToCC_cookie;
extern LeafInfo pbSncpDirection_cookie;
extern LeafInfo pbSncpFrom_cookie;
extern LeafInfo pbGmplsSncpDirection_cookie;
extern LeafInfo pbSncpAction_cookie;
extern LeafInfo pbMspConfigTable_cookie;
extern LeafInfo pbMspGroupActPortType_cookie;
extern LeafInfo pbMspProtectChSlotNum_cookie;
extern LeafInfo pbMspProtectChPortNum_cookie;
extern LeafInfo pbMspProtectChPortType_cookie;
extern LeafInfo pbMspProtectChSignalState_cookie;
extern LeafInfo pbMspGroupId_cookie;
extern LeafInfo pbMspType_cookie;
extern LeafInfo pbMspSignalDircetion_cookie;
extern LeafInfo pbMspRestoreMode_cookie;
extern LeafInfo pbMspExTrafficEnable_cookie;
extern LeafInfo pbMspEnable_cookie;
extern LeafInfo pbMspExternalCmd_cookie;
extern LeafInfo pbMspExtCmdParam_cookie;
extern LeafInfo pbMspExCmdRslt_cookie;
extern LeafInfo pbMspSwitchState_cookie;
extern LeafInfo pbMspCurSwitchReq_cookie;
extern LeafInfo pbMspSwitchReason_cookie;
extern LeafInfo pbMspWTR_cookie;
extern LeafInfo pbMspWorkChNum_cookie;
extern LeafInfo pbMspWorkCh1SlotNum_cookie;
extern LeafInfo pbMspWorkCh1PortNum_cookie;
extern LeafInfo pbMspWorkCh1PortType_cookie;
extern LeafInfo pbMspWorkCh1SignalState_cookie;
extern LeafInfo pbMspWorkCh1PriState_cookie;
extern LeafInfo pbMspWorkCh2SlotNum_cookie;
extern LeafInfo pbMspWorkCh2PortNum_cookie;
extern LeafInfo pbMspWorkCh2PortType_cookie;
extern LeafInfo pbMspWorkCh2SignalState_cookie;
extern LeafInfo pbMspWorkCh2PriState_cookie;
extern LeafInfo pbMspWorkCh3SlotNum_cookie;
extern LeafInfo pbMspWorkCh3PortNum_cookie;
extern LeafInfo pbMspWorkCh3PortType_cookie;
extern LeafInfo pbMspWorkCh3SignalState_cookie;
extern LeafInfo pbMspWorkCh3PriState_cookie;
extern LeafInfo pbMspWorkCh4SlotNum_cookie;
extern LeafInfo pbMspWorkCh4PortNum_cookie;
extern LeafInfo pbMspWorkCh4PortType_cookie;
extern LeafInfo pbMspWorkCh4SignalState_cookie;
extern LeafInfo pbMspWorkCh4PriState_cookie;
extern LeafInfo pbMspWorkCh5SlotNum_cookie;
extern LeafInfo pbMspWorkCh5PortNum_cookie;
extern LeafInfo pbMspWorkCh5PortType_cookie;
extern LeafInfo pbMspWorkCh5SignalState_cookie;
extern LeafInfo pbMspWorkCh5PriState_cookie;
extern LeafInfo pbMspWorkCh6SlotNum_cookie;
extern LeafInfo pbMspWorkCh6PortNum_cookie;
extern LeafInfo pbMspWorkCh6PortType_cookie;
extern LeafInfo pbMspWorkCh6SignalState_cookie;
extern LeafInfo pbMspWorkCh6PriState_cookie;
extern LeafInfo pbMspWorkCh7SlotNum_cookie;
extern LeafInfo pbMspWorkCh7PortNum_cookie;
extern LeafInfo pbMspWorkCh7PortType_cookie;
extern LeafInfo pbMspWorkCh7SignalState_cookie;
extern LeafInfo pbMspWorkCh7PriState_cookie;
extern LeafInfo pbMspWorkCh8SlotNum_cookie;
extern LeafInfo pbMspWorkCh8PortNum_cookie;
extern LeafInfo pbMspWorkCh8PortType_cookie;
extern LeafInfo pbMspWorkCh8SignalState_cookie;
extern LeafInfo pbMspWorkCh8PriState_cookie;
extern LeafInfo pbMspWorkCh9SlotNum_cookie;
extern LeafInfo pbMspWorkCh9PortNum_cookie;
extern LeafInfo pbMspWorkCh9PortType_cookie;
extern LeafInfo pbMspWorkCh9SignalState_cookie;
extern LeafInfo pbMspWorkCh9PriState_cookie;
extern LeafInfo pbMspWorkCh10SlotNum_cookie;
extern LeafInfo pbMspWorkCh10PortNum_cookie;
extern LeafInfo pbMspWorkCh10PortType_cookie;
extern LeafInfo pbMspWorkCh10SignalState_cookie;
extern LeafInfo pbMspWorkCh10PriState_cookie;
extern LeafInfo pbMspWorkCh11SlotNum_cookie;
extern LeafInfo pbMspWorkCh11PortNum_cookie;
extern LeafInfo pbMspWorkCh11PortType_cookie;
extern LeafInfo pbMspWorkCh11SignalState_cookie;
extern LeafInfo pbMspWorkCh11PriState_cookie;
extern LeafInfo pbMspWorkCh12SlotNum_cookie;
extern LeafInfo pbMspWorkCh12PortNum_cookie;
extern LeafInfo pbMspWorkCh12PortType_cookie;
extern LeafInfo pbMspWorkCh12SignalState_cookie;
extern LeafInfo pbMspWorkCh12PriState_cookie;
extern LeafInfo pbMspWorkCh13SlotNum_cookie;
extern LeafInfo pbMspWorkCh13PortNum_cookie;
extern LeafInfo pbMspWorkCh13PortType_cookie;
extern LeafInfo pbMspWorkCh13SignalState_cookie;
extern LeafInfo pbMspWorkCh13PriState_cookie;
extern LeafInfo pbMspWorkCh14SlotNum_cookie;
extern LeafInfo pbMspWorkCh14PortNum_cookie;
extern LeafInfo pbMspWorkCh14PortType_cookie;
extern LeafInfo pbMspWorkCh14SignalState_cookie;
extern LeafInfo pbMspWorkCh14PriState_cookie;
extern LeafInfo pbMspK2Bit5Mismatch_cookie;
extern LeafInfo pbMspK1Bit1to4Mismatch_cookie;
extern LeafInfo pbMspK1Bit5to8Mismatch_cookie;
extern LeafInfo pbMspReceiveK1_cookie;
extern LeafInfo pbMspReceiveK2_cookie;
extern LeafInfo pbMspSendoutK1_cookie;
extern LeafInfo pbMspSendoutK2_cookie;
extern LeafInfo pbMspRowStatus_cookie;
extern LeafInfo pbMspFreeze_cookie;
extern LeafInfo pbMspLable_cookie;
extern LeafInfo pbMSMConfigTable_cookie;
extern LeafInfo pbMSMStatusTable_cookie;
extern LeafInfo pbMSMNodeStatusTable_cookie;
extern LeafInfo pbMSMTopoConfigTable_cookie;
extern LeafInfo pbMSMSquelchTable_cookie;
extern LeafInfo pbMSMCommandTable_cookie;
extern LeafInfo pbAutoMappingTable_cookie;
extern LeafInfo pbMSMSNMRingId_cookie;
extern LeafInfo pbAutoNodeId_cookie;
extern LeafInfo pbPortMappingString_cookie;
extern LeafInfo pbAutoMappingRowStatus_cookie;
extern LeafInfo pbMSMSNMRingId_cookie;
extern LeafInfo pbMSMCommandCurrentSide_cookie;
extern LeafInfo pbMSMCommandSwitch_cookie;
extern LeafInfo pbMSMLastCommandStatus_cookie;
extern LeafInfo pbMSMCommandResult_cookie;
extern LeafInfo pbMSMSNMRingId_cookie;
extern LeafInfo pbMSMSquelchSide_cookie;
extern LeafInfo pbMSMSquelchGress_cookie;
extern LeafInfo pbMSMSquelchTimeSlot_cookie;
extern LeafInfo pbMSMSquelchOrder_cookie;
extern LeafInfo pbMSMSquelchType_cookie;
extern LeafInfo pbMSMSquelchSrcNodeId_cookie;
extern LeafInfo pbMSMSquelchDstNodeId_cookie;
extern LeafInfo pbMSMSquelchReference_cookie;
extern LeafInfo pbMSMSquelchOpTimeSlot_cookie;
extern LeafInfo pbMSMSquelchRowStatus_cookie;
extern LeafInfo pbMSMSNMRingId_cookie;
extern LeafInfo pbMSMTopoNumOfNodes_cookie;
extern LeafInfo pbMSMTopoNodeId0_cookie;
extern LeafInfo pbMSMTopoNodeId1_cookie;
extern LeafInfo pbMSMTopoNodeId2_cookie;
extern LeafInfo pbMSMTopoNodeId3_cookie;
extern LeafInfo pbMSMTopoNodeId4_cookie;
extern LeafInfo pbMSMTopoNodeId5_cookie;
extern LeafInfo pbMSMTopoNodeId6_cookie;
extern LeafInfo pbMSMTopoNodeId7_cookie;
extern LeafInfo pbMSMTopoNodeId8_cookie;
extern LeafInfo pbMSMTopoNodeId9_cookie;
extern LeafInfo pbMSMTopoNodeId10_cookie;
extern LeafInfo pbMSMTopoNodeId11_cookie;
extern LeafInfo pbMSMTopoNodeId12_cookie;
extern LeafInfo pbMSMTopoNodeId13_cookie;
extern LeafInfo pbMSMTopoNodeId14_cookie;
extern LeafInfo pbMSMTopoNodeId15_cookie;
extern LeafInfo pbMSMTopoStatus_cookie;
extern LeafInfo pbMSMTopoConfig_cookie;
extern LeafInfo pbMSMTopoNodeId0IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId1IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId2IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId3IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId4IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId5IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId6IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId7IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId8IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId9IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId10IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId11IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId12IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId13IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId14IpAddress_cookie;
extern LeafInfo pbMSMTopoNodeId15IpAddress_cookie;
extern LeafInfo pbMSMSNMRingId_cookie;
extern LeafInfo pbMSMNodeAgentState_cookie;
extern LeafInfo pbMSMNodeState_cookie;
extern LeafInfo pbMSMNodeCurrentSide_cookie;
extern LeafInfo pbMSMNodeCurrentReq_cookie;
extern LeafInfo pbMSMNodeCurrentSrc_cookie;
extern LeafInfo pbMSMNodeKExpressMode_cookie;
extern LeafInfo pbMSMEastSwitchCommand_cookie;
extern LeafInfo pbMSMEastLineReq_cookie;
extern LeafInfo pbMSMEastLastSwitchCmd_cookie;
extern LeafInfo pbMSMEastIsSquelched_cookie;
extern LeafInfo pbMSMWestSwitchCommand_cookie;
extern LeafInfo pbMSMWestLineReq_cookie;
extern LeafInfo pbMSMWestLastSwitchCmd_cookie;
extern LeafInfo pbMSMWestIsSquelched_cookie;
extern LeafInfo pbMSMEastSwitchStatus_cookie;
extern LeafInfo pbMSMWestSwitchStatus_cookie;
extern LeafInfo pbMSMPassNodeId_cookie;
extern LeafInfo pbMSMSNMRingId_cookie;
extern LeafInfo pbMSMSide_cookie;
extern LeafInfo pbMSMTransBridgeReq_cookie;
extern LeafInfo pbMSMTransDestNodeId_cookie;
extern LeafInfo pbMSMTransSrcNodeId_cookie;
extern LeafInfo pbMSMTransLongShort_cookie;
extern LeafInfo pbMSMTransStatus_cookie;
extern LeafInfo pbMSMReceiveBridgeReq_cookie;
extern LeafInfo pbMSMReceiveDestNodeId_cookie;
extern LeafInfo pbMSMReceiveSrcNodeId_cookie;
extern LeafInfo pbMSMReceiveLongShort_cookie;
extern LeafInfo pbMSMReceiveStatus_cookie;
extern LeafInfo pbMSMSideReq_cookie;
extern LeafInfo pbMSMSideSrc_cookie;
extern LeafInfo pbMSMSwitchCommand_cookie;
extern LeafInfo pbMSMLineReq_cookie;
extern LeafInfo pbMSMCmndReq_cookie;
extern LeafInfo pbMSMIsSquelched_cookie;
extern LeafInfo pbMSMSNMRingId_cookie;
extern LeafInfo pbMSMProtectMode_cookie;
extern LeafInfo pbMSMSNMRingName_cookie;
extern LeafInfo pbMSMRingIdx_cookie;
extern LeafInfo pbMSMWestSideSlotNum_cookie;
extern LeafInfo pbMSMWestSidePortType_cookie;
extern LeafInfo pbMSMWestSidePortNum_cookie;
extern LeafInfo pbMSMWestProtSlotNum_cookie;
extern LeafInfo pbMSMWestProtPortType_cookie;
extern LeafInfo pbMSMWestProtPortNum_cookie;
extern LeafInfo pbMSMEastSideSlotNum_cookie;
extern LeafInfo pbMSMEastSidePortType_cookie;
extern LeafInfo pbMSMEastSidePortNum_cookie;
extern LeafInfo pbMSMEastProtSlotNum_cookie;
extern LeafInfo pbMSMEastProtPortType_cookie;
extern LeafInfo pbMSMEastProtPortNum_cookie;
extern LeafInfo pbMSMAutoSquelchDiscovery_cookie;
extern LeafInfo pbMSMAutoRingDiscovery_cookie;
extern LeafInfo pbMSMPeerWestAddress_cookie;
extern LeafInfo pbMSMPeerWestSlotNum_cookie;
extern LeafInfo pbMSMPeerWestPortType_cookie;
extern LeafInfo pbMSMPeerWestPortNum_cookie;
extern LeafInfo pbMSMPeerEastAddress_cookie;
extern LeafInfo pbMSMPeerEastSlotNum_cookie;
extern LeafInfo pbMSMPeerEastPortType_cookie;
extern LeafInfo pbMSMPeerEastPortNum_cookie;
extern LeafInfo pbMSMConfigSdBerThreshold_cookie;
extern LeafInfo pbMSMConfigSfBerThreshold_cookie;
extern LeafInfo pbMSMConfigWaitToRestore_cookie;
extern LeafInfo pbMSMConfigExtraTraffic_cookie;
extern LeafInfo pbMSMConfigEnable_cookie;
extern LeafInfo pbMSMConfigDelete_cookie;
extern LeafInfo pbMSMSquelchFin_cookie;
extern LeafInfo pbMSMCardFunction_cookie;
extern LeafInfo pbMSMChannelInfo_cookie;
extern LeafInfo pbMSMSelfAddress_cookie;
extern LeafInfo pbMSMStartAutoSquelch_cookie;
extern LeafInfo pbMSMAutoMappingEnable_cookie;
extern LeafInfo pbMSMPortMapRefresh_cookie;
extern LeafInfo pbEpgTable_cookie;
extern LeafInfo pbCpgTable_cookie;
extern LeafInfo scEpgGroup_cookie;
extern LeafInfo scEpgScWorkerSlot_cookie;
extern LeafInfo scEpgSc1Status_cookie;
extern LeafInfo scEpgSc2Status_cookie;
extern LeafInfo scEpgRequest_cookie;
extern LeafInfo scEpgLastSwichReason_cookie;
extern LeafInfo scEpgStandbySc_cookie;
extern LeafInfo scEpgASCSwithchState_cookie;
extern LeafInfo pbCpgIndexNo_cookie;
extern LeafInfo pbCpgWorkerSlot_cookie;
extern LeafInfo pbCpgProtectorSlot_cookie;
extern LeafInfo pbCpgProtectStatus_cookie;
extern LeafInfo pbWorkerStatus_cookie;
extern LeafInfo pbProtectorStatus_cookie;
extern LeafInfo pbCpgAct_cookie;
extern LeafInfo pbCpgActStatus_cookie;
extern LeafInfo pbCpgCurActStatus_cookie;
extern LeafInfo pbCpgReadyToSwitch_cookie;
extern LeafInfo pbEpgId_cookie;
extern LeafInfo pbEpgBoardType_cookie;
extern LeafInfo pbEpgType_cookie;
extern LeafInfo pbEpgProtMode_cookie;
extern LeafInfo pbEpgEnable_cookie;
extern LeafInfo pbEpgExtCmd_cookie;
extern LeafInfo pbEpgExtCmdParam_cookie;
extern LeafInfo pbEpgExtCmdResult_cookie;
extern LeafInfo pbEpgWtr_cookie;
extern LeafInfo pbEpgPreSwitchReason_cookie;
extern LeafInfo pbEpgCurRequest_cookie;
extern LeafInfo pbEpgProtState_cookie;
extern LeafInfo pbEpgHoldOffTime_cookie;
extern LeafInfo pbEpgProtSlotNum_cookie;
extern LeafInfo pbEpgProtSlotState_cookie;
extern LeafInfo pbEpgWorkSlotNum1_cookie;
extern LeafInfo pbEpgWorkSlot1State_cookie;
extern LeafInfo pbEpgWorkSlotNum2_cookie;
extern LeafInfo pbEpgWorkSlot2State_cookie;
extern LeafInfo pbEpgWorkSlotNum3_cookie;
extern LeafInfo pbEpgWorkSlot3State_cookie;
extern LeafInfo pbEpgWorkSlotNum4_cookie;
extern LeafInfo pbEpgWorkSlot4State_cookie;
extern LeafInfo pbEpgWorkSlotNum5_cookie;
extern LeafInfo pbEpgWorkSlot5State_cookie;
extern LeafInfo pbEpgWorkSlotNum6_cookie;
extern LeafInfo pbEpgWorkSlot6State_cookie;
extern LeafInfo pbEpgWorkSlotNum7_cookie;
extern LeafInfo pbEpgWorkSlot7State_cookie;
extern LeafInfo pbEpgWorkSlotNum8_cookie;
extern LeafInfo pbEpgWorkSlot8State_cookie;
extern LeafInfo pbEpgRowStatus_cookie;
extern LeafInfo pbIpTunnelCfgTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbSdhServerType_cookie;
extern LeafInfo pbLapdNetworkSide_cookie;
extern LeafInfo pbLapdT200_cookie;
extern LeafInfo pbLapdT203_cookie;
extern LeafInfo pbLapdN200_cookie;
extern LeafInfo pbLapdAITS_cookie;
extern LeafInfo pbLapdWindow_cookie;
extern LeafInfo pbIpTunnelSrcIpAddress_cookie;
extern LeafInfo pbIpTunnelDesIpAddress_cookie;
extern LeafInfo pbIpTunnelByPass_cookie;
extern LeafInfo pbIpTunnelDstSlot_cookie;
extern LeafInfo pbIpTunnelDstPort_cookie;
extern LeafInfo pbIpTunnelDstMode_cookie;
extern LeafInfo pbIpTunnelStatus_cookie;
extern LeafInfo pbOsiConfigTable_cookie;
extern LeafInfo osiTunnelConfigTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbSdhServerType_cookie;
extern LeafInfo osiTunnelNetworkSide_cookie;
extern LeafInfo osiTunnelT200_cookie;
extern LeafInfo osiTunnelT203_cookie;
extern LeafInfo osiTunnelN200_cookie;
extern LeafInfo osiTunnelAITS_cookie;
extern LeafInfo osiTunnelWindow_cookie;
extern LeafInfo osiTunnelConfigTimer_cookie;
extern LeafInfo osiTunnelTTL_cookie;
extern LeafInfo osiTunnelLocalNasp_cookie;
extern LeafInfo osiTunnelRemoteNasp_cookie;
extern LeafInfo osiTunnelSrcIpAddress_cookie;
extern LeafInfo osiTunnelDesIpAddress_cookie;
extern LeafInfo osiTunnelIpIfMtu_cookie;
extern LeafInfo osiTunnelStatus_cookie;
extern LeafInfo osiTunnelIndex_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbOsiNetworkSide_cookie;
extern LeafInfo pbOsiT200_cookie;
extern LeafInfo pbOsiT203_cookie;
extern LeafInfo pbOsiN200_cookie;
extern LeafInfo pbOsiAITS_cookie;
extern LeafInfo pbOsiWindow_cookie;
extern LeafInfo pbOsiConfigTimer_cookie;
extern LeafInfo pbOsiTTL_cookie;
extern LeafInfo pbOsiLocalNasp_cookie;
extern LeafInfo pbOsiRemoteNasp_cookie;
extern LeafInfo pbOsiSrcIpAddress_cookie;
extern LeafInfo pbOsiDesIpAddress_cookie;
extern LeafInfo pbOsiIpIfMtu_cookie;
extern LeafInfo pbOATable_cookie;
extern LeafInfo pbOASlotNo_cookie;
extern LeafInfo pbOAPortNo_cookie;
extern LeafInfo pbOAModuleConfiguration_cookie;
extern LeafInfo pbOAModuleType_cookie;
extern LeafInfo pbOAHWVersion_cookie;
extern LeafInfo pbOASWVersion_cookie;
extern LeafInfo pbOASerialNumber_cookie;
extern LeafInfo pbOAInputPower_cookie;
extern LeafInfo pbOACurrentOutputPower_cookie;
extern LeafInfo pbOACurrentModuleGain_cookie;
extern LeafInfo pbOAPump1Power_cookie;
extern LeafInfo pbOAPump2Power_cookie;
extern LeafInfo pbOATemperature_cookie;
extern LeafInfo pbOAPump1DriveCurrent_cookie;
extern LeafInfo pbOAPump2DriveCurrent_cookie;
extern LeafInfo pbOAPowerSupplyVoltage_cookie;
extern LeafInfo pbOAOperationMode_cookie;
extern LeafInfo pbOAExpectedOutputPower_cookie;
extern LeafInfo pbOAExpectedModuleGain_cookie;
extern LeafInfo pbOAInputThreshold_cookie;
extern LeafInfo pbOAOutputThreshold_cookie;
extern LeafInfo pbOAShutdownThreshold_cookie;
extern LeafInfo pbOALowTThreshold_cookie;
extern LeafInfo pbOAHighTThreshold_cookie;
extern LeafInfo pbOAAutoShutdown_cookie;
extern LeafInfo pbOAMaxoutputpower_cookie;
extern LeafInfo pbOAMaxPump1Current_cookie;
extern LeafInfo pbOAMaxPump2Current_cookie;
extern LeafInfo pbOARealOperationMode_cookie;
extern LeafInfo pbOARealExpectedOutputPower_cookie;
extern LeafInfo pbOARealExpectedModuleGain_cookie;
extern LeafInfo pbOARealInputThreshold_cookie;
extern LeafInfo pbOARealOutputThreshold_cookie;
extern LeafInfo pbOARealShutdownThreshold_cookie;
extern LeafInfo pbOARealLowTThreshold_cookie;
extern LeafInfo pbOARealHighTThreshold_cookie;
extern LeafInfo pbOARealAutoShutdown_cookie;
extern LeafInfo pbOARealCoolPumpTemp_cookie;
extern LeafInfo rprGeneral_cookie;
extern LeafInfo rprProtocols_cookie;
extern LeafInfo rprService_cookie;
extern LeafInfo rprWan_cookie;
extern LeafInfo rprHwPassThrough_cookie;
extern LeafInfo rprHwpXcTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprHwpXcCapacity_cookie;
extern LeafInfo rprHwpXcSrcSlot_cookie;
extern LeafInfo rprHwpXcSrcPort_cookie;
extern LeafInfo rprHwpXcSource_cookie;
extern LeafInfo rprHwpXcDstSlot_cookie;
extern LeafInfo rprHwpXcDstPort_cookie;
extern LeafInfo rprHwpXcDestination_cookie;
extern LeafInfo rprHwpXcSrcPortType_cookie;
extern LeafInfo rprHwpXcDstPortType_cookie;
extern LeafInfo rprHwpXcConnectId_cookie;
extern LeafInfo rprHwpXcCircuitDirection_cookie;
extern LeafInfo rprHwpXcCcsRpmMsgType_cookie;
extern LeafInfo rprHwpXcCcsRpmMsgState_cookie;
extern LeafInfo rprHwpXcCcsRpmMsgPriority_cookie;
extern LeafInfo rprHwpXcSrcSlotCard_cookie;
extern LeafInfo rprHwpXcDstSlotCard_cookie;
extern LeafInfo rprHwpXcReason_cookie;
extern LeafInfo rprHwpXcRowStatus_cookie;
extern LeafInfo rprHwpXcTableClass_cookie;
extern LeafInfo pbRprWanConfigTable_cookie;
extern LeafInfo pbRprLCASBase_cookie;
extern LeafInfo pbRprSOLCASConfigTable_cookie;
extern LeafInfo pbRprSKLCASConfigTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbTpType_cookie;
extern LeafInfo pbTpIndex_cookie;
extern LeafInfo pbRprSKLCASWanPort_cookie;
extern LeafInfo pbRprSKLCASState_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbTpType_cookie;
extern LeafInfo pbTpIndex_cookie;
extern LeafInfo pbRprSOLCASActive_cookie;
extern LeafInfo pbRprSOLCASWanPort_cookie;
extern LeafInfo pbRprSOLCASState_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbRprWanPortLabel_cookie;
extern LeafInfo pbRprWanPortCapacity_cookie;
extern LeafInfo pbRprWanPortMonitored_cookie;
extern LeafInfo pbRprWanPortXVType_cookie;
extern LeafInfo pbRprWanLCASSupport_cookie;
extern LeafInfo pbRprWanPortCapacityTrigger_cookie;
extern LeafInfo pbRprWanGFPFCS_cookie;
extern LeafInfo pbRprWanGFPFCSAutoDetect_cookie;
extern LeafInfo pbRprWanFlowControl_cookie;
extern LeafInfo rprServiceTable_cookie;
extern LeafInfo rprRateProfileTable_cookie;
extern LeafInfo pbRprMultiCastTable_cookie;
extern LeafInfo pbRprAclTable_cookie;
extern LeafInfo pbRprMultiBroadCastTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprMultiBroadCastId_cookie;
extern LeafInfo rprMultiBroadCastTimeInterval_cookie;
extern LeafInfo rprMultiBroadCastPacketsInterval_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprServiceId_cookie;
extern LeafInfo rprAclMacAddress_cookie;
extern LeafInfo rprAclMacType_cookie;
extern LeafInfo rprAclDoubleVlanId_cookie;
extern LeafInfo rprAclPortsList_cookie;
extern LeafInfo rprAclMode_cookie;
extern LeafInfo rprAclRowStatus_cookie;
extern LeafInfo rprAclTblIdx_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprServiceId_cookie;
extern LeafInfo rprMultiCastMacAddress_cookie;
extern LeafInfo rprMultiCastDoubleVlanId_cookie;
extern LeafInfo rprMultiCastPortsList_cookie;
extern LeafInfo rprMultiCastGMRP_cookie;
extern LeafInfo rprMultiCastRowStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprRateProfNo_cookie;
extern LeafInfo rprTokenRate_cookie;
extern LeafInfo rprBurstSize_cookie;
extern LeafInfo rprExcessBurstSize_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprServiceId_cookie;
extern LeafInfo rprLocalEthPortNo_cookie;
extern LeafInfo rprLocalVID_cookie;
extern LeafInfo rprLocalVPrio_cookie;
extern LeafInfo rprSvcClassifyType_cookie;
extern LeafInfo rprServiceType_cookie;
extern LeafInfo rprSvcMode_cookie;
extern LeafInfo rprRmtStationMac_cookie;
extern LeafInfo rprSvcRingletSel_cookie;
extern LeafInfo rprSvcRealRingletSel_cookie;
extern LeafInfo rprSvcAutoRingletSelEn_cookie;
extern LeafInfo rprSvcCos_cookie;
extern LeafInfo rprSvcTtlMode_cookie;
extern LeafInfo rprSvcTtl0_cookie;
extern LeafInfo rprSvcTtl1_cookie;
extern LeafInfo rprSvcRealTtl0_cookie;
extern LeafInfo rprSvcRealTtl1_cookie;
extern LeafInfo rprSvcWe_cookie;
extern LeafInfo rprSvcSteered_cookie;
extern LeafInfo rprSvcRateProfIndex_cookie;
extern LeafInfo rprSvcTokenRate_cookie;
extern LeafInfo rprSvcBurstSize_cookie;
extern LeafInfo rprSvcMaxDelay_cookie;
extern LeafInfo rprSvcMaxJitter_cookie;
extern LeafInfo rprSvcWeight_cookie;
extern LeafInfo rprGmiiSel_cookie;
extern LeafInfo rprDoubleVlanEn_cookie;
extern LeafInfo rprDoubleVID_cookie;
extern LeafInfo rprDoubleVPrio_cookie;
extern LeafInfo rprSvcLoop_cookie;
extern LeafInfo rprSvcActive_cookie;
extern LeafInfo rprSvcCurStatus_cookie;
extern LeafInfo rprSvcReason_cookie;
extern LeafInfo rprSvcLabel_cookie;
extern LeafInfo rprFlowRuleID_cookie;
extern LeafInfo rprSvcMulBrdCastID_cookie;
extern LeafInfo rprSvcMulBrdCastEn_cookie;
extern LeafInfo rprSvcRowStatus_cookie;
extern LeafInfo rprTopoImageTable_cookie;
extern LeafInfo rprFairnessTable_cookie;
extern LeafInfo rprOamTable_cookie;
extern LeafInfo rprTopoRingInfoTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprRingMultiChoke0_cookie;
extern LeafInfo rprRingMultiChoke1_cookie;
extern LeafInfo rprRingMtuSize_cookie;
extern LeafInfo rprRingTotalHopsTx0_cookie;
extern LeafInfo rprRingTotalHopsTx1_cookie;
extern LeafInfo rprRingLastNeighborMac0_cookie;
extern LeafInfo rprRingLastNeighborMac1_cookie;
extern LeafInfo rprRingCheckSum_cookie;
extern LeafInfo rprRingNeighborCheckSum0_cookie;
extern LeafInfo rprRingNeighborCheckSum1_cookie;
extern LeafInfo rprRingCheckSumValid_cookie;
extern LeafInfo rprRingNeighborCksValid0_cookie;
extern LeafInfo rprRingNeighborCksValid1_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprOamActionType_cookie;
extern LeafInfo rprOamDestAddress_cookie;
extern LeafInfo rprOamRequestRinglet_cookie;
extern LeafInfo rprOamResponseRinglet_cookie;
extern LeafInfo rprOamClassOfService_cookie;
extern LeafInfo rprOamUserData_cookie;
extern LeafInfo rprOamProtected_cookie;
extern LeafInfo rprOamRequestCount_cookie;
extern LeafInfo rprOamTimeout_cookie;
extern LeafInfo rprOamControl_cookie;
extern LeafInfo rprOamResponseCount_cookie;
extern LeafInfo rprOamAvResponseTime_cookie;
extern LeafInfo rprOamResponseStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprFairnessRinglet_cookie;
extern LeafInfo rprFairnessRingletWeight_cookie;
extern LeafInfo rprFairnessReservedRate_cookie;
extern LeafInfo rprFairnessMaxAllowed_cookie;
extern LeafInfo rprFairnessAgeCoef_cookie;
extern LeafInfo rprFairnessLpCoef_cookie;
extern LeafInfo rprFairnessAdvertisementRatio_cookie;
extern LeafInfo rprFairnessMcffReportCoef_cookie;
extern LeafInfo rprFairnessActiveWeightsCoef_cookie;
extern LeafInfo rprFairnessSTQHighThreshold_cookie;
extern LeafInfo rprFairnessSTQMedThreshold_cookie;
extern LeafInfo rprFairnessSTQLowThreshold_cookie;
extern LeafInfo rprFairnessRateHighThreshold_cookie;
extern LeafInfo rprFairnessRateLowThreshold_cookie;
extern LeafInfo rprFairnessResetWaterMarks_cookie;
extern LeafInfo rprFairnessSTQHighWaterMark_cookie;
extern LeafInfo rprFairnessSTQLowWaterMark_cookie;
extern LeafInfo rprFairnessRampUpCoef_cookie;
extern LeafInfo rprFairnessRampDnCoef_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprTopoImageMacAddress_cookie;
extern LeafInfo rprTopoImageSecMacAddress1_cookie;
extern LeafInfo rprTopoImageSecMacAddress2_cookie;
extern LeafInfo rprTopoImageStationIfIndex_cookie;
extern LeafInfo rprTopoImageStationName_cookie;
extern LeafInfo rprTopoImageCapability_cookie;
extern LeafInfo rprTopoImageRinglet0Hops_cookie;
extern LeafInfo rprTopoImageRinglet0ReservedRate_cookie;
extern LeafInfo rprTopoImageRinglet1Hops_cookie;
extern LeafInfo rprTopoImageRinglet1ReservedRate_cookie;
extern LeafInfo rprTopoImageWestProtectionStatus_cookie;
extern LeafInfo rprTopoImageWestWeight_cookie;
extern LeafInfo rprTopoImageEastProtectionStatus_cookie;
extern LeafInfo rprTopoImageEastWeight_cookie;
extern LeafInfo rprTopoImageStatus_cookie;
extern LeafInfo rprTopoImageRemoteNEAddress_cookie;
extern LeafInfo rprTopoImageRemoteSlotNumber_cookie;
extern LeafInfo rprIfTable_cookie;
extern LeafInfo rprSpanTable_cookie;
extern LeafInfo rprSpanProtectionTable_cookie;
extern LeafInfo rprCardProtectionTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprCardProtectionId_cookie;
extern LeafInfo rprCpfWorkSlot_cookie;
extern LeafInfo rprCpfProtSlot_cookie;
extern LeafInfo rprCpfActiveSlot_cookie;
extern LeafInfo rprCpfCoordSlot_cookie;
extern LeafInfo rprCpfCoordProtGroup_cookie;
extern LeafInfo rprCpfEnable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprSpanId_cookie;
extern LeafInfo rprSpanProtectionNeighborValid_cookie;
extern LeafInfo rprSpanProtectionHoldOffTimer_cookie;
extern LeafInfo rprSpanProtectionCommand_cookie;
extern LeafInfo rprSpanProtectionCount_cookie;
extern LeafInfo rprSpanProtectionDuration_cookie;
extern LeafInfo rprSpanProtectionLastActivationTime_cookie;
extern LeafInfo rprSpanProtectionCountReset_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprSpanId_cookie;
extern LeafInfo rprSpanTotalRingletReservedRate_cookie;
extern LeafInfo rprSpanCurrentStatus_cookie;
extern LeafInfo rprSpanWanPortNo_cookie;
extern LeafInfo rprSpanHopsToWrap_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprIfStationsOnRing_cookie;
extern LeafInfo rprIfReversionMode_cookie;
extern LeafInfo rprIfProtectionWTR_cookie;
extern LeafInfo rprIfProtectionFastTimer_cookie;
extern LeafInfo rprIfProtectionSlowTimer_cookie;
extern LeafInfo rprIfAtdTimer_cookie;
extern LeafInfo rprIfKeepaliveTimeout_cookie;
extern LeafInfo rprIfFairnessMode_cookie;
extern LeafInfo rprIfPtqSize_cookie;
extern LeafInfo rprIfStqSize_cookie;
extern LeafInfo rprIfSTQFullThreshold_cookie;
extern LeafInfo rprIfIdleThreshold_cookie;
extern LeafInfo rprIfSesThreshold_cookie;
extern LeafInfo rprIfProtectionMode_cookie;
extern LeafInfo rprIfJumboFramePreferred_cookie;
extern LeafInfo rprIfMacOperModes_cookie;
extern LeafInfo rprIfRingOperModes_cookie;
extern LeafInfo rprIfCurrentStatus_cookie;
extern LeafInfo rprIfLinkRate_cookie;
extern LeafInfo rprIfMplsMode_cookie;
extern LeafInfo rprIfMplsTypeTag_cookie;
extern LeafInfo rprIfVlanPrioEn_cookie;
extern LeafInfo rprIfMyMacAddress_cookie;
extern LeafInfo rprIfPassThroughEn_cookie;
extern LeafInfo rprIfHwPassThrough_cookie;
extern LeafInfo rprIfHwPassThroughStatus_cookie;
extern LeafInfo rprIfResRateR0_cookie;
extern LeafInfo rprIfResRateR1_cookie;
extern LeafInfo rprIfA0RateR0_cookie;
extern LeafInfo rprIfA0RateR1_cookie;
extern LeafInfo rprIfA1RateR0_cookie;
extern LeafInfo rprIfA1RateR1_cookie;
extern LeafInfo rprIfBcirRateR0_cookie;
extern LeafInfo rprIfBcirRateR1_cookie;
extern LeafInfo rprIfRealA0RateR0_cookie;
extern LeafInfo rprIfRealA0RateR1_cookie;
extern LeafInfo rprIfRealA1RateR0_cookie;
extern LeafInfo rprIfRealA1RateR1_cookie;
extern LeafInfo rprIfRealBcirRateR0_cookie;
extern LeafInfo rprIfRealBcirRateR1_cookie;
extern LeafInfo rprIfAlarmMon_cookie;
extern LeafInfo rprIfStationName_cookie;
extern LeafInfo rprIfInterWorking_cookie;
extern LeafInfo rprIfBandMode_cookie;
extern LeafInfo rprIfMulBrdCastEn_cookie;
extern LeafInfo rprIfCardProtection_cookie;
extern LeafInfo rprIfCardProtectionState_cookie;
extern LeafInfo rprIfL2RateLimitMode_cookie;
extern LeafInfo rprIfCrossPortsList_cookie;
extern LeafInfo rprIfLogSave_cookie;
extern LeafInfo rprIfL2CosFlowControl_cookie;
extern LeafInfo pbXConConnectCapacity_cookie;
extern LeafInfo pbXConConnectSrcSlot_cookie;
extern LeafInfo pbXConConnectSrcPort_cookie;
extern LeafInfo pbXConConnectSrcPortType_cookie;
extern LeafInfo pbXConConnectSource_cookie;
extern LeafInfo pbXConConnectDstSlot_cookie;
extern LeafInfo pbXConConnectDstPort_cookie;
extern LeafInfo pbXConConnectDstPortType_cookie;
extern LeafInfo pbXConConnectDestination_cookie;
extern LeafInfo pbXConConnectId_cookie;
extern LeafInfo pbXConConnectCircuitId_cookie;
extern LeafInfo pbXConConnectStatus_cookie;
extern LeafInfo pbXConConnectReason_cookie;
extern LeafInfo pbXConConnectType_cookie;
extern LeafInfo pbXConConnectCircuitDirection_cookie;
extern LeafInfo pbXConConnectAction_cookie;
extern LeafInfo pbXConFrom_cookie;
extern LeafInfo pbXConRowStatus_cookie;
extern LeafInfo pbXConConnectReqNo_cookie;
extern LeafInfo pbXCGmplsVcNumber_cookie;
extern LeafInfo pbXCGmplsCallName_cookie;
extern LeafInfo pbGmplsCcDirection_cookie;
extern LeafInfo pbXCExtendObjects_cookie;
extern LeafInfo pbSdhXcNewContextTable_cookie;
extern LeafInfo pbSdhXcNewLoMapTable_cookie;
extern LeafInfo pbXcMigrationGroup_cookie;
extern LeafInfo pbForwardXcId_cookie;
extern LeafInfo pbReverseXcId_cookie;
extern LeafInfo pbXcMigrationCommand_cookie;
extern LeafInfo pbXcMigrationStatus_cookie;
extern LeafInfo pbXConLoMapLOCCSlot_cookie;
extern LeafInfo pbXConLoMapLOCCPortType_cookie;
extern LeafInfo pbXConLoMapLOCCPort_cookie;
extern LeafInfo pbXConLoMapHOCCIndex_cookie;
extern LeafInfo pbXConLoMapLCSlot_cookie;
extern LeafInfo pbXConLoMapLCPortType_cookie;
extern LeafInfo pbXConLoMapLCPort_cookie;
extern LeafInfo pbXConLoMapLCTP_cookie;
extern LeafInfo gmplsReservationStatus_cookie;
extern LeafInfo gmplsReservationOperate_cookie;
extern LeafInfo pbXConContextLOCCBusNum_cookie;
extern LeafInfo pbXConContextLOCCAuNum_cookie;
extern LeafInfo pbXConContextHOCCAu3Num_cookie;
extern LeafInfo pbXConContextHOCCAu3Step_cookie;
extern LeafInfo pbXConContextHOCCAu3Count_cookie;
extern LeafInfo pbEthGlobalPortTable_cookie;
extern LeafInfo pbEthConfigTable_cookie;
extern LeafInfo pbEthWanConfigTable_cookie;
extern LeafInfo pbEthStaticMACTable_cookie;
extern LeafInfo pbEthStaticMulticastTable_cookie;
extern LeafInfo pbVlanPortCoSTable_cookie;
extern LeafInfo pbEthVlanNameTable_cookie;
extern LeafInfo pbPortQueueTable_cookie;
extern LeafInfo pbEthACLTable_cookie;
extern LeafInfo pbRstpBase_cookie;
extern LeafInfo pbEthStatusTable_cookie;
extern LeafInfo pbVlanPortQinQTable_cookie;
extern LeafInfo pbEthProtPktACLTable_cookie;
extern LeafInfo pbEthReservedTable_cookie;
extern LeafInfo pbIGMPSnooping_cookie;
extern LeafInfo pbMstpBase_cookie;
extern LeafInfo pbLagConfigTable_cookie;
extern LeafInfo pbLacpConfig_cookie;
extern LeafInfo pbEthStaticMplsTable_cookie;
extern LeafInfo eosMapper_cookie;
extern LeafInfo pbGvrpBase_cookie;
extern LeafInfo pbEthPolicingTable_cookie;
extern LeafInfo pbEthColorTable_cookie;
extern LeafInfo pbEthWredTable_cookie;
extern LeafInfo pbEthPortBasedACLTable_cookie;
extern LeafInfo pbGmrpBase_cookie;
extern LeafInfo pbGmrpBridgeTable_cookie;
extern LeafInfo pbGmrpPortTable_cookie;
extern LeafInfo pbVlanGmrpTable_cookie;
extern LeafInfo pbGmrpDMacTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbGmrpVlanId_cookie;
extern LeafInfo pbGmrpMacAddr_cookie;
extern LeafInfo pbGmrpMacMemberPort1_cookie;
extern LeafInfo pbGmrpMacMemberPort2_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbGmrpVlanNumber_cookie;
extern LeafInfo pbVlanGmrpEnable_cookie;
extern LeafInfo pbVlanGmrpRowStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbGmrpPortType_cookie;
extern LeafInfo pbGmrpPortNo_cookie;
extern LeafInfo pbGmrpPortEnable_cookie;
extern LeafInfo pbGmrpPortRegAdminCtrl_cookie;
extern LeafInfo pbGmrpPortApplAdminCtrl_cookie;
extern LeafInfo pbGmrpForwardAllEnable_cookie;
extern LeafInfo pbGmrpBlockPortEnable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbGmrpBridgeAddress_cookie;
extern LeafInfo pbGmrpEnable_cookie;
extern LeafInfo pbGmrpJoinTimer_cookie;
extern LeafInfo pbGmrpLeaveTimer_cookie;
extern LeafInfo pbGmrpLeaveallTimer_cookie;
extern LeafInfo pbGmrpMaxDMacNum_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthPortCls_cookie;
extern LeafInfo pbEthPortBasedACLMacAddr_cookie;
extern LeafInfo pbEthPortBasedACLVid_cookie;
extern LeafInfo pbEthPortBasedACLItemType_cookie;
extern LeafInfo pbEthPortBasedACLRateLimit_cookie;
extern LeafInfo pbEthPortBasedACLCoS_cookie;
extern LeafInfo pbEthPortBasedACLIndex_cookie;
extern LeafInfo pbEthPortBasedACLEntryRowStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbEthWredIndex_cookie;
extern LeafInfo pbEthWredLowThr0_cookie;
extern LeafInfo pbEthWredUpThr0_cookie;
extern LeafInfo pbEthWredLowThr1_cookie;
extern LeafInfo pbEthWredUpThr1_cookie;
extern LeafInfo pbEthWredLowThr2_cookie;
extern LeafInfo pbEthWredUpThr2_cookie;
extern LeafInfo pbEthWredRowStatus_cookie;
extern LeafInfo pbEthWredCount_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbEthColorIndex_cookie;
extern LeafInfo pbEthPrio0Color_cookie;
extern LeafInfo pbEthPrio1Color_cookie;
extern LeafInfo pbEthPrio2Color_cookie;
extern LeafInfo pbEthPrio3Color_cookie;
extern LeafInfo pbEthPrio4Color_cookie;
extern LeafInfo pbEthPrio5Color_cookie;
extern LeafInfo pbEthPrio6Color_cookie;
extern LeafInfo pbEthPrio7Color_cookie;
extern LeafInfo pbEthColorRowStatus_cookie;
extern LeafInfo pbEthColorCount_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbEthPolicingIndex_cookie;
extern LeafInfo pbEthPolicingName_cookie;
extern LeafInfo pbEthPolicingMode_cookie;
extern LeafInfo pbEthPolicingEIRPIR_cookie;
extern LeafInfo pbEthPolicingCIR_cookie;
extern LeafInfo pbEthPolicingEBSPBS_cookie;
extern LeafInfo pbEthPolicingCBS_cookie;
extern LeafInfo pbEthPolicingColorAware_cookie;
extern LeafInfo pbEthPolicingDiscardColor_cookie;
extern LeafInfo pbEthPolicingRowStatus_cookie;
extern LeafInfo pbEthPolicingCount_cookie;
extern LeafInfo pbGvrpBridgeTable_cookie;
extern LeafInfo pbGvrpPortTable_cookie;
extern LeafInfo pbGvrpDVlanTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbGvrpDPortType_cookie;
extern LeafInfo pbGvrpDPortNo_cookie;
extern LeafInfo pbGvrpDVlanList_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbGvrpPortType_cookie;
extern LeafInfo pbGvrpPortNo_cookie;
extern LeafInfo pbGvrpPortEnable_cookie;
extern LeafInfo pbGvrpPortRegAdminCtrl_cookie;
extern LeafInfo pbGvrpPortApplAdminCtrl_cookie;
extern LeafInfo pbGvrpBlockPortEnable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbGvrpBridgeAddress_cookie;
extern LeafInfo pbGvrpEnable_cookie;
extern LeafInfo pbGvrpJoinTimer_cookie;
extern LeafInfo pbGvrpLeaveTimer_cookie;
extern LeafInfo pbGvrpLeaveallTimer_cookie;
extern LeafInfo pbGvrpVlanList_cookie;
extern LeafInfo pbGvrpMaxDVlanNum_cookie;
extern LeafInfo vcgConfigTable_cookie;
extern LeafInfo eosGlobalConfigTable_cookie;
extern LeafInfo pbSOLCASConfigTable_cookie;
extern LeafInfo pbSKLCASConfigTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbTpType_cookie;
extern LeafInfo pbTpIndex_cookie;
extern LeafInfo pbSKLCASVCGPort_cookie;
extern LeafInfo pbSKLCASState_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbTpType_cookie;
extern LeafInfo pbTpIndex_cookie;
extern LeafInfo pbSOLCASActive_cookie;
extern LeafInfo pbSOLCASVCGPort_cookie;
extern LeafInfo pbSOLCASState_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo eosIPGConfig_cookie;
extern LeafInfo eosMTUSize_cookie;
extern LeafInfo eosEslConfig_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo vcgNo_cookie;
extern LeafInfo vcgLabel_cookie;
extern LeafInfo vcgXVType_cookie;
extern LeafInfo vcgChannelId_cookie;
extern LeafInfo vcgLCASSupport_cookie;
extern LeafInfo vcgCapacityTrigger_cookie;
extern LeafInfo vcgPauseTransparent_cookie;
extern LeafInfo vcgLcasHoldOffTime_cookie;
extern LeafInfo vcgGfpMode_cookie;
extern LeafInfo vcgGfpType_cookie;
extern LeafInfo vcgGfpFCS_cookie;
extern LeafInfo vcgGfpFcsAutoDetect_cookie;
extern LeafInfo vcgGfpCsfAutoMode_cookie;
extern LeafInfo vcgPortMonMode_cookie;
extern LeafInfo vcgPortAlarmDisable_cookie;
extern LeafInfo vcgPLCTThreshold_cookie;
extern LeafInfo vcgPLCRThreshold_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbEthLspLabel_cookie;
extern LeafInfo pbEthInPort_cookie;
extern LeafInfo pbEthInPortType_cookie;
extern LeafInfo pbEthPort_cookie;
extern LeafInfo pbEthOutPortType_cookie;
extern LeafInfo pbEthStaticMplsPolicingID_cookie;
extern LeafInfo pbEthStaticMplsColorID_cookie;
extern LeafInfo pbEthStaticMplsStatus_cookie;
extern LeafInfo pbEthLspName_cookie;
extern LeafInfo pbLacpGlobalCfgTable_cookie;
extern LeafInfo pbLacpAgConfigTable_cookie;
extern LeafInfo pbLacpAgInformTable_cookie;
extern LeafInfo pbLacpApConfigTable_cookie;
extern LeafInfo pbLacpApInformTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbLacpPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbLacpPortLinkState_cookie;
extern LeafInfo pbLacpPortBelongAg_cookie;
extern LeafInfo pbLacpPortSync_cookie;
extern LeafInfo pbLacpPortOperKey_cookie;
extern LeafInfo pbLacpPartnerPortNo_cookie;
extern LeafInfo pbLacpPartnerPortMode_cookie;
extern LeafInfo pbLacpPartnerPortPri_cookie;
extern LeafInfo pbLacpPartnerOperKey_cookie;
extern LeafInfo pbLacpPartnerPortState_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbLacpPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbLacpTimeoutMode_cookie;
extern LeafInfo pbLacpPortIndiv_cookie;
extern LeafInfo pbLacpActiveMode_cookie;
extern LeafInfo pbLacpDesigAggor_cookie;
extern LeafInfo pbLacpPortPri_cookie;
extern LeafInfo pbLacpPortBpduMode_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbLacpAgNo_cookie;
extern LeafInfo pbLacpAgOperKey_cookie;
extern LeafInfo pbLacpPortList_cookie;
extern LeafInfo pbLacpPartnerMac_cookie;
extern LeafInfo pbLacpPartnerSysPri_cookie;
extern LeafInfo pbLacpPartnerAgOperKey_cookie;
extern LeafInfo pbLacpPortTypeInAg_cookie;
extern LeafInfo pbLacpPortSpeedInAg_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbLacpAgNo_cookie;
extern LeafInfo pbLacpAgName_cookie;
extern LeafInfo pbLacpAgMac_cookie;
extern LeafInfo pbLacpAgMode_cookie;
extern LeafInfo pbLacpAgRowState_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbLacpEnable_cookie;
extern LeafInfo pbLacpSystemID_cookie;
extern LeafInfo pbLacpSystemPri_cookie;
extern LeafInfo pbLacpHashMode_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbLagId_cookie;
extern LeafInfo pbLagType_cookie;
extern LeafInfo pbLagFlowControl_cookie;
extern LeafInfo pbLagPvid_cookie;
extern LeafInfo pbLagVlan_cookie;
extern LeafInfo pbLagTagMode_cookie;
extern LeafInfo pbLagInputRateLimit_cookie;
extern LeafInfo pbLagBroadCastRateLimit_cookie;
extern LeafInfo pbLagDefaultVlanPriority_cookie;
extern LeafInfo pbLagTxPriAlg_cookie;
extern LeafInfo pbLagTxEnableHol_cookie;
extern LeafInfo pbLagChangeCoS_cookie;
extern LeafInfo pbLagQinQStatus_cookie;
extern LeafInfo pbLagQinQNewVlan_cookie;
extern LeafInfo pbLagQinQNewVlanPriority_cookie;
extern LeafInfo pbLagPortList_cookie;
extern LeafInfo pbLagEnableWred_cookie;
extern LeafInfo pbMstpBridgeBase_cookie;
extern LeafInfo pbMstpPortBase_cookie;
extern LeafInfo pbMstpBridgeStateBase_cookie;
extern LeafInfo pbMstpPortStateBase_cookie;
extern LeafInfo pbCistPortStateTable_cookie;
extern LeafInfo pbMstiPortStateTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbMstPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbMstiNumber_cookie;
extern LeafInfo pbMstiPPVector1_cookie;
extern LeafInfo pbMstiPPVector2_cookie;
extern LeafInfo pbMstiDPVector1_cookie;
extern LeafInfo pbMstiDPVector2_cookie;
extern LeafInfo pbMstiMPVector1_cookie;
extern LeafInfo pbMstiMPVector2_cookie;
extern LeafInfo pbMstiPTimes_cookie;
extern LeafInfo pbMstiDTimes_cookie;
extern LeafInfo pbMstiMTimes_cookie;
extern LeafInfo pbMstiPState_cookie;
extern LeafInfo pbMstiPInfoIs_cookie;
extern LeafInfo pbMstiPRole_cookie;
extern LeafInfo pbMstiPSelectedRole_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbMstPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbCistPortEnable_cookie;
extern LeafInfo pbCistPPVector1_cookie;
extern LeafInfo pbCistPPVector2_cookie;
extern LeafInfo pbCistPPVector3_cookie;
extern LeafInfo pbCistDPVector1_cookie;
extern LeafInfo pbCistDPVector2_cookie;
extern LeafInfo pbCistDPVector3_cookie;
extern LeafInfo pbCistMPVector1_cookie;
extern LeafInfo pbCistMPVector2_cookie;
extern LeafInfo pbCistMPVector3_cookie;
extern LeafInfo pbCistPTimes_cookie;
extern LeafInfo pbCistDTimes_cookie;
extern LeafInfo pbCistMTimes_cookie;
extern LeafInfo pbCistPState_cookie;
extern LeafInfo pbCistPInfoIs_cookie;
extern LeafInfo pbCistPRole_cookie;
extern LeafInfo pbCistPPState_cookie;
extern LeafInfo pbMstRcvdInternal_cookie;
extern LeafInfo pbPortBpduRule_cookie;
extern LeafInfo pbPortOperEdge_cookie;
extern LeafInfo pbCistBridgeStateTable_cookie;
extern LeafInfo pbMstiBridgeStateTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbMstiNumber_cookie;
extern LeafInfo pbMstiRPVector1_cookie;
extern LeafInfo pbMstiRPVector2_cookie;
extern LeafInfo pbMstiRootRemainingHops_cookie;
extern LeafInfo pbMstiRootPortId_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbMstBaseNumPorts_cookie;
extern LeafInfo pbCistRPVector1_cookie;
extern LeafInfo pbCistRPVector2_cookie;
extern LeafInfo pbCistRPVector3_cookie;
extern LeafInfo pbMstRootMaxAge_cookie;
extern LeafInfo pbMstRootMsgAge_cookie;
extern LeafInfo pbMstRootHelloTime_cookie;
extern LeafInfo pbMstRootForwardDelay_cookie;
extern LeafInfo pbCistRootRemainingHops_cookie;
extern LeafInfo pbCistRootPortId_cookie;
extern LeafInfo pbMstCfgDigest_cookie;
extern LeafInfo pbCistPortTable_cookie;
extern LeafInfo pbMstiPortTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbMstPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbMstiNumber_cookie;
extern LeafInfo pbMstiPortPriority_cookie;
extern LeafInfo pbMstiPortPathCost_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbMstPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbMstPort_cookie;
extern LeafInfo pbCistPortPriority_cookie;
extern LeafInfo pbCistExPortPathCost_cookie;
extern LeafInfo pbCistInPortPathCost_cookie;
extern LeafInfo pbMstPortAdminEdgePort_cookie;
extern LeafInfo pbMstPortAdminPointToPoint_cookie;
extern LeafInfo pbMstPortEnable_cookie;
extern LeafInfo pbMstPortCheck_cookie;
extern LeafInfo pbMstPortBpduMode_cookie;
extern LeafInfo pbCistBridgeTable_cookie;
extern LeafInfo pbMstiBridgeTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbMstiNumber_cookie;
extern LeafInfo pbMstiVlanNumber_cookie;
extern LeafInfo pbMstiPriority_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbMstBaseBridgeAddress_cookie;
extern LeafInfo pbCistPriority_cookie;
extern LeafInfo pbMstForceVersion_cookie;
extern LeafInfo pbMstBridgeMaxAge_cookie;
extern LeafInfo pbMstBridgeHelloTime_cookie;
extern LeafInfo pbMstBridgeForwardDelay_cookie;
extern LeafInfo pbMstTxHoldCount_cookie;
extern LeafInfo pbMstMaxHops_cookie;
extern LeafInfo pbMstMstpBEGIN_cookie;
extern LeafInfo pbMstMstpConfigName_cookie;
extern LeafInfo pbMstMstpRevisonLevel_cookie;
extern LeafInfo pbMstEventSwitch_cookie;
extern LeafInfo pbIgmpSnoopingGlobalTable_cookie;
extern LeafInfo pbVlanIgmpSnoopingTable_cookie;
extern LeafInfo pbIgmpSnoopingIpGroupTable_cookie;
extern LeafInfo pbIgmpSnoopingMacGroupTable_cookie;
extern LeafInfo pbIgmpSnoopingMulRouterTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbIgmpVlanNumber_cookie;
extern LeafInfo pbIgmpMulRouterPort_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbIgmpVlanNumber_cookie;
extern LeafInfo pbIgmpMacAddr_cookie;
extern LeafInfo pbIgmpMacMemberPort_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbIgmpVlanNumber_cookie;
extern LeafInfo pbGroupIp_cookie;
extern LeafInfo pbIgmpMacAddr_cookie;
extern LeafInfo pbIgmpIpMemberPort_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbIgmpVlanNumber_cookie;
extern LeafInfo pbVlanIgmpSnoopingEnable_cookie;
extern LeafInfo pbVlanIgmpSnoopingRowStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbIgmpSnoopingEnable_cookie;
extern LeafInfo pbIgmpSnoopingAgingTime_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbEthReserveIndex_cookie;
extern LeafInfo pbEthReservedCfg_cookie;
extern LeafInfo pbEthReservedRowStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbEthACLMacAddr_cookie;
extern LeafInfo pbEthProtPktACLPort_cookie;
extern LeafInfo pbEthProtPktACLIndex_cookie;
extern LeafInfo pbEthProtPktACLRowStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthPortCls_cookie;
extern LeafInfo pbEthVlan_cookie;
extern LeafInfo pbEthQinQNewVlanId_cookie;
extern LeafInfo pbEthQinQNewVlanPriority_cookie;
extern LeafInfo pbVlanPortQinQIndex_cookie;
extern LeafInfo pbEthVlanPortQinQRowStatus_cookie;
extern LeafInfo pbEthQinQNewVlanPriMode_cookie;
extern LeafInfo pbEthQinQVlanSwapEnable_cookie;
extern LeafInfo pbEthQinQVlanSwapVid_cookie;
extern LeafInfo pbEthQinQVlanSwapMode_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthStatusPortStyle_cookie;
extern LeafInfo pbEthStatusPortType_cookie;
extern LeafInfo pbEthStatusPortOperStatus_cookie;
extern LeafInfo pbEthStatusLaserIBias_cookie;
extern LeafInfo pbEthStatusLaserRecvPwr_cookie;
extern LeafInfo pbEthStatusLaserTransPwr_cookie;
extern LeafInfo pbEthStatusLaserTemp_cookie;
extern LeafInfo pbEthStatusCompliance8472_cookie;
extern LeafInfo pbEthStatusNominalMBps_cookie;
extern LeafInfo pbEthStatusWavelength_cookie;
extern LeafInfo pbEthStatusLinkLength9125_cookie;
extern LeafInfo pbEthStatusLinkLength50125_cookie;
extern LeafInfo pbEthStatusLinkLength62125_cookie;
extern LeafInfo pbEthStatusLinkLengthCopper_cookie;
extern LeafInfo pbEthStatusVendorName_cookie;
extern LeafInfo pbEthStatusVendorPN_cookie;
extern LeafInfo pbEthStatusVendorSN_cookie;
extern LeafInfo pbEthStatusManuDate_cookie;
extern LeafInfo pbEthStatusTransCode_cookie;
extern LeafInfo pbEthStatusVendorOUI_cookie;
extern LeafInfo pbEthStatusConnectorType_cookie;
extern LeafInfo pbEthsSAGPartNumber_cookie;
extern LeafInfo pbEthnClLaserTemp_cookie;
extern LeafInfo pbEthnTECCurrent_cookie;
extern LeafInfo pbRstpBridgeTable_cookie;
extern LeafInfo pbRstpPortTable_cookie;
extern LeafInfo pbRstpBridgeStateTable_cookie;
extern LeafInfo pbRstpPortStateTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbRstpPortType_cookie;
extern LeafInfo pbRstpPortNo_cookie;
extern LeafInfo pbRstpPEnable_cookie;
extern LeafInfo pbRstpPPVector1_cookie;
extern LeafInfo pbRstpPPVector2_cookie;
extern LeafInfo pbRstpDsgPVector1_cookie;
extern LeafInfo pbRstpDsgPVector2_cookie;
extern LeafInfo pbRstpMsgPVector1_cookie;
extern LeafInfo pbRstpMsgPVector2_cookie;
extern LeafInfo pbRstpPTimes_cookie;
extern LeafInfo pbRstpDsgTimes_cookie;
extern LeafInfo pbRstpMsgTimes_cookie;
extern LeafInfo pbRstpPForward_cookie;
extern LeafInfo pbRstpPLearn_cookie;
extern LeafInfo pbRstpPInfoIs_cookie;
extern LeafInfo pbRstpPRole_cookie;
extern LeafInfo pbRstpPSelectedRole_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbRstpRPVector1_cookie;
extern LeafInfo pbRstpRPVector2_cookie;
extern LeafInfo pbRstpRootMaxAge_cookie;
extern LeafInfo pbRstpRootHelloTime_cookie;
extern LeafInfo pbRstpRootForwardDelay_cookie;
extern LeafInfo pbRstpRootPortId_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbRstpPortType_cookie;
extern LeafInfo pbRstpPortNo_cookie;
extern LeafInfo pbRstpPortPriority_cookie;
extern LeafInfo pbRstpPortEnable_cookie;
extern LeafInfo pbRstpPortPathCost_cookie;
extern LeafInfo pbRstpPortAdminEdgePort_cookie;
extern LeafInfo pbRstpPortAdminPointToPoint_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbRstpBridgeAddress_cookie;
extern LeafInfo pbRstpBridgePriority_cookie;
extern LeafInfo pbRstpBridgeMaxAge_cookie;
extern LeafInfo pbRstpBridgeHelloTime_cookie;
extern LeafInfo pbRstpBridgeForwardDelay_cookie;
extern LeafInfo pbRstpTxHoldCount_cookie;
extern LeafInfo pbRstpBEGIN_cookie;
extern LeafInfo pbRstpEventSwitch_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbEthACLMacAddr_cookie;
extern LeafInfo pbEthACLVid_cookie;
extern LeafInfo pbEthACLItemType_cookie;
extern LeafInfo pbEthACLRateLimit_cookie;
extern LeafInfo pbEthACLCoS_cookie;
extern LeafInfo pbEthACLIndex_cookie;
extern LeafInfo pbEthACLEntryRowStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthPortStyle_cookie;
extern LeafInfo pbEthTxQueNo_cookie;
extern LeafInfo pbEthPortQueueWeight_cookie;
extern LeafInfo pbEthQueueWredID_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbEthNamedVlan_cookie;
extern LeafInfo pbEthVlanName_cookie;
extern LeafInfo pbEthVlanRowStatus_cookie;
extern LeafInfo pbEthVlanEnableMacLearning_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthPortCls_cookie;
extern LeafInfo pbEthVlan_cookie;
extern LeafInfo pbEthRateLimitValue_cookie;
extern LeafInfo pbEthVlanCoS_cookie;
extern LeafInfo pbEthVlanDisable_cookie;
extern LeafInfo pbEthRedirectionPort_cookie;
extern LeafInfo pbVlanPortCoSIndex_cookie;
extern LeafInfo pbEthVlanPortCoSRowStatus_cookie;
extern LeafInfo pbEthVlanPolicingID_cookie;
extern LeafInfo pbEthVlanColorID_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbStaticMulticastMacAddr_cookie;
extern LeafInfo pbStaticMulticastVlan_cookie;
extern LeafInfo pbStaticMulticastPortList_cookie;
extern LeafInfo pbStaticMulticastIndex_cookie;
extern LeafInfo pbStaticMulticastRowStatus_cookie;
extern LeafInfo pbStaticMulticastForGmrp_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbMACEntryMacAddr_cookie;
extern LeafInfo pbMACEntryVid_cookie;
extern LeafInfo pbMACEntryPort_cookie;
extern LeafInfo pbMACEntryTagged_cookie;
extern LeafInfo pbMACEntryIndex_cookie;
extern LeafInfo pbMACEntryRowStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbWanLabel_cookie;
extern LeafInfo pbWanVlanList_cookie;
extern LeafInfo pbWanTaggedVlanList_cookie;
extern LeafInfo pbWanDefaultVlanPriority_cookie;
extern LeafInfo pbWanChangeCoS_cookie;
extern LeafInfo pbWanPvid_cookie;
extern LeafInfo pbWanTagMode_cookie;
extern LeafInfo pbWanFlowControl_cookie;
extern LeafInfo pbWanTxPriAlg_cookie;
extern LeafInfo pbWanTxEnableHol_cookie;
extern LeafInfo pbWanQinQStatus_cookie;
extern LeafInfo pbWanQinQNewVlan_cookie;
extern LeafInfo pbWanQinQNewVlanPriority_cookie;
extern LeafInfo pbWanTPID_cookie;
extern LeafInfo pbWanMirrorPort_cookie;
extern LeafInfo pbWanLinkStatus_cookie;
extern LeafInfo pbWanPortWorkMode_cookie;
extern LeafInfo pbWanEnableWred_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthPortType_cookie;
extern LeafInfo pbEthPortAdminStatus_cookie;
extern LeafInfo pbPortLinkStatus_cookie;
extern LeafInfo pbEthPortAutoNego_cookie;
extern LeafInfo pbEthPortFlowControl_cookie;
extern LeafInfo pbEthPortSpeed_cookie;
extern LeafInfo pbEthPortDuplex_cookie;
extern LeafInfo pbEthPortVlan_cookie;
extern LeafInfo pbEthPortPvid_cookie;
extern LeafInfo pbUnTagMode_cookie;
extern LeafInfo pbPortInputRateLimit_cookie;
extern LeafInfo pbEthPortMode_cookie;
extern LeafInfo pbEthPortBroadCastRateLimit_cookie;
extern LeafInfo pbPortDefaultVlanPriority_cookie;
extern LeafInfo pbEthPortLabel_cookie;
extern LeafInfo pbLanTxPriAlg_cookie;
extern LeafInfo pbLanTaggedVlan_cookie;
extern LeafInfo pbEthPortLoopback_cookie;
extern LeafInfo pbEthTxEnableHol_cookie;
extern LeafInfo pbEthAutoLaserShutdown_cookie;
extern LeafInfo pbEthAlsMode_cookie;
extern LeafInfo pbEthPortAlarmDisable_cookie;
extern LeafInfo ethPortOnCSF_cookie;
extern LeafInfo ethMDIorMDIX_cookie;
extern LeafInfo pbLanChangeCoS_cookie;
extern LeafInfo pbLanQinQStatus_cookie;
extern LeafInfo pbLanQinQNewVlan_cookie;
extern LeafInfo pbLanQinQNewVlanPriority_cookie;
extern LeafInfo pbLanTPID_cookie;
extern LeafInfo pbLanMirrorPort_cookie;
extern LeafInfo pbEthPortClassifier_cookie;
extern LeafInfo pbEthPortWorkMode_cookie;
extern LeafInfo pbEthEnableWred_cookie;
extern LeafInfo pbSFPWorkMode_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortTagProtocol_cookie;
extern LeafInfo pbAgingTime_cookie;
extern LeafInfo pbIPGConfig_cookie;
extern LeafInfo pbVlanTagProtocolType_cookie;
extern LeafInfo pbMTUSize_cookie;
extern LeafInfo pbEthACLType_cookie;
extern LeafInfo pbEthBroadcastEnableHol_cookie;
extern LeafInfo pbEthTunnelingRxTPID_cookie;
extern LeafInfo pbEthQoSIEEE802Version_cookie;
extern LeafInfo pbEthChangeCoSMode_cookie;
extern LeafInfo pbEthLabelBase_cookie;
extern LeafInfo pbEthWredDropProbability_cookie;
extern LeafInfo pbIpRouteTable_cookie;
extern LeafInfo dccCfgTable_cookie;
extern LeafInfo ipInIpCfgTable_cookie;
extern LeafInfo mpBundleCfgTable_cookie;
extern LeafInfo pbStaticIpRouteTable_cookie;
extern LeafInfo tracertCfgTable_cookie;
extern LeafInfo tracertResultTable_cookie;
extern LeafInfo pingCfgTable_cookie;
extern LeafInfo pingResultTable_cookie;
extern LeafInfo dccReservePortTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo phyPortNum_cookie;
extern LeafInfo logPortNum_cookie;
extern LeafInfo pingIndex_cookie;
extern LeafInfo pingTargetIpAddrDis_cookie;
extern LeafInfo pingDataSizeDis_cookie;
extern LeafInfo pingTripTime_cookie;
extern LeafInfo pingTtlnumber_cookie;
extern LeafInfo pingResult_cookie;
extern LeafInfo pingRstRowStatus_cookie;
extern LeafInfo pingTargetIpAddr_cookie;
extern LeafInfo pingDataSize_cookie;
extern LeafInfo pingCounts_cookie;
extern LeafInfo pingTimeout_cookie;
extern LeafInfo pingRowStatus_cookie;
extern LeafInfo pingStart_cookie;
extern LeafInfo tracertTtlNumber_cookie;
extern LeafInfo tracertTripTime1_cookie;
extern LeafInfo tracertTripTime2_cookie;
extern LeafInfo tracertTripTime3_cookie;
extern LeafInfo tracertPeerIpAddrDis_cookie;
extern LeafInfo tracertRstRowStatus_cookie;
extern LeafInfo tracertPeerIpAddr_cookie;
extern LeafInfo tracertTimeout_cookie;
extern LeafInfo tracertRowStatus_cookie;
extern LeafInfo tracertStart_cookie;
extern LeafInfo pbStaticIpRouteDest_cookie;
extern LeafInfo pbStaticIpRouteMask_cookie;
extern LeafInfo pbStaticIpRouteProto_cookie;
extern LeafInfo pbStaticIpRouteIfIndex_cookie;
extern LeafInfo pbStaticIpRouteNextHop_cookie;
extern LeafInfo pbStaticIpRouteAdminStatus_cookie;
extern LeafInfo pbStaticIpRouteOperStatus_cookie;
extern LeafInfo pbStaticIpRouteMetricType_cookie;
extern LeafInfo pbStaticIpRouteCost_cookie;
extern LeafInfo bundlePeerIpAddr_cookie;
extern LeafInfo bundleIndex_cookie;
extern LeafInfo bundleStatus_cookie;
extern LeafInfo bundleNumber_cookie;
extern LeafInfo bundleRowStatus_cookie;
extern LeafInfo bundleTOS_cookie;
extern LeafInfo ipInIpIfIpAddr_cookie;
extern LeafInfo ipInIpIfMask_cookie;
extern LeafInfo ipInIpDestIpAddr_cookie;
extern LeafInfo ipInIpSourcIpAddr_cookie;
extern LeafInfo ipInIpIfIndex_cookie;
extern LeafInfo ipInIpIfMtu_cookie;
extern LeafInfo ipInIpRowStatus_cookie;
extern LeafInfo ipInIpPeerIfIpAddr_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo mode_cookie;
extern LeafInfo state_cookie;
extern LeafInfo protocol_cookie;
extern LeafInfo dccBundleNumber_cookie;
extern LeafInfo linkStatus_cookie;
extern LeafInfo mspStatus_cookie;
extern LeafInfo destSlot_cookie;
extern LeafInfo destPort_cookie;
extern LeafInfo mtu_cookie;
extern LeafInfo crcMode_cookie;
extern LeafInfo hcocIndex_cookie;
extern LeafInfo multilink_cookie;
extern LeafInfo pbIpRouteDest_cookie;
extern LeafInfo pbIpRouteMask_cookie;
extern LeafInfo pbIpRouteProto_cookie;
extern LeafInfo pbIpRouteIfIndex_cookie;
extern LeafInfo pbIpRouteNextHop_cookie;
extern LeafInfo pbIpRouteType_cookie;
extern LeafInfo pbIpRouteAge_cookie;
extern LeafInfo pbIpRouteMetric1_cookie;
extern LeafInfo pbIpRouteMetric2_cookie;
extern LeafInfo pbIpRouteMetric3_cookie;
extern LeafInfo pbIpRouteMetric4_cookie;
extern LeafInfo pbIpRouteMetric5_cookie;
extern LeafInfo pbTMSRefSourceTable_cookie;
extern LeafInfo pbTMSSysTimingGroup_cookie;
extern LeafInfo pbTMSStClkInTable_cookie;
extern LeafInfo pbTMSStClkOutTable_cookie;
extern LeafInfo pbTMSStClkOutIndex_cookie;
extern LeafInfo pbTMSStClkOutType_cookie;
extern LeafInfo pbTMSStClkOutMonitored_cookie;
extern LeafInfo pbTMSStClkOutSaBit_cookie;
extern LeafInfo pbTMSStClkOutQlThreshold_cookie;
extern LeafInfo pbTMSStClkOutSquelchMode_cookie;
extern LeafInfo tMSStClkOutOhm_cookie;
extern LeafInfo pbTMSStClkOutQlIn_cookie;
extern LeafInfo pbTMSStClkOutQl_cookie;
extern LeafInfo tmsStClkOutputMode_cookie;
extern LeafInfo pbTMSStClkOutState_cookie;
extern LeafInfo pbTMSStClkInIndex_cookie;
extern LeafInfo pbTMSStClkInType_cookie;
extern LeafInfo pbTMSStClkInMonitored_cookie;
extern LeafInfo pbTMSStClkInSaBit_cookie;
extern LeafInfo tMSStClkInOhm_cookie;
extern LeafInfo pbTMSSwitchRequest_cookie;
extern LeafInfo pbTMSActiveTimingSource_cookie;
extern LeafInfo pbTMSTimingState_cookie;
extern LeafInfo pbTMSSysTimingMode_cookie;
extern LeafInfo pbTMSSwitchRequestStatus_cookie;
extern LeafInfo pbTMSHoldoverDTS_cookie;
extern LeafInfo pbTMSStaClkOutSource_cookie;
extern LeafInfo pbTMSQlEnabled_cookie;
extern LeafInfo pbTMSExpectedSMMB_cookie;
extern LeafInfo pbTMSStClkAutoLoopPrevention_cookie;
extern LeafInfo tmsSysSrcFDThreshold_cookie;
extern LeafInfo tmsSysFDValue_cookie;
extern LeafInfo pbTMSRefSourceIndex_cookie;
extern LeafInfo pbTMSRefSourceSlot_cookie;
extern LeafInfo pbTMSRefSourcePort_cookie;
extern LeafInfo pbTMSRefSourceMonitored_cookie;
extern LeafInfo pbTMSSignalStatus_cookie;
extern LeafInfo pbTMSRefSourcePriority_cookie;
extern LeafInfo pbTMSRefSourceLockOut_cookie;
extern LeafInfo pbTMSRefSourceQlIn_cookie;
extern LeafInfo pbTMSRefSourceQl_cookie;
extern LeafInfo pbTMSQlProvionedMode_cookie;
extern LeafInfo pbTMSQlProvioned_cookie;
extern LeafInfo pbTMSRefSourceValid_cookie;
extern LeafInfo pbTMSRefSourceWTR_cookie;
extern LeafInfo tMSIfSdhQLType_cookie;
extern LeafInfo tMSRefSourceQLOut_cookie;
extern LeafInfo tMSForceDnu_cookie;
extern LeafInfo tMSSupportQlOut_cookie;
extern LeafInfo tMSPortType_cookie;
extern LeafInfo tmsRefSourceFDUpband_cookie;
extern LeafInfo tmsRefSourceFDLowband_cookie;
extern LeafInfo tmsRefSourceFDValue_cookie;
extern LeafInfo tmsStoredDrvClkNum_cookie;
extern LeafInfo pbDigitalTransport_cookie;
extern LeafInfo pbSonetSdh_cookie;
extern LeafInfo pbPortConfigTable_cookie;
extern LeafInfo pbPortStatusTable_cookie;
extern LeafInfo pbPortHoTpReserveTable_cookie;
extern LeafInfo pbTPConfigTable_cookie;
extern LeafInfo pbOTUPortConfigTable_cookie;
extern LeafInfo pbSdhPmUpLoadInfo_cookie;
extern LeafInfo pbSdhMostRecentIndex_cookie;
extern LeafInfo pbSdhRecentUapPerfMon_cookie;
extern LeafInfo pbSdhUapPerfMon_cookie;
extern LeafInfo pbSdhRecentPerfMon_cookie;
extern LeafInfo pbSdhCurPerfMon_cookie;
extern LeafInfo pbPRBSPortHisTable_cookie;
extern LeafInfo pbPRBSPortCurTable_cookie;
extern LeafInfo pbOTUPortStatusTable_cookie;
extern LeafInfo pbOTUTPConfigTable_cookie;
extern LeafInfo pbPMFilterConfigGroup_cookie;
extern LeafInfo pbPMFilterTable_cookie;
extern LeafInfo pbPmFilterType_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbTpType_cookie;
extern LeafInfo pbTpIndex_cookie;
extern LeafInfo pbTableType_cookie;
extern LeafInfo pbPmEthId_cookie;
extern LeafInfo pbPmReserved_cookie;
extern LeafInfo pbPmDeleteAllFilter_cookie;
extern LeafInfo pbPmDeleteSlotNumber_cookie;
extern LeafInfo pbPmDeleteSlotFilter_cookie;
extern LeafInfo pbPmDeleteFilterTypeId_cookie;
extern LeafInfo pbPmDeleteFilterType_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbOTUTpChannelId_cookie;
extern LeafInfo pbOTUTpSegment_cookie;
extern LeafInfo pbOTUTpType_cookie;
extern LeafInfo pbOTUMonitored_cookie;
extern LeafInfo pbOTUTimAlmEnable_cookie;
extern LeafInfo pbOTUTimActEnable_cookie;
extern LeafInfo pbOTUTxTti_cookie;
extern LeafInfo pbOTUExpTti_cookie;
extern LeafInfo pbOTURxTti_cookie;
extern LeafInfo pbOTUDegEnable_cookie;
extern LeafInfo pbOTUDegThreshold_cookie;
extern LeafInfo pbOTUDegIntervals_cookie;
extern LeafInfo pbOTUSendMaintenanceType_cookie;
extern LeafInfo pbOTURecvMaintenanceType_cookie;
extern LeafInfo pbOTUPlmAlmEnable_cookie;
extern LeafInfo pbOTUMsimAlmEnable_cookie;
extern LeafInfo pbOTUPtAlmEnable_cookie;
extern LeafInfo pbOTUTxPt_cookie;
extern LeafInfo pbOTUExpPt_cookie;
extern LeafInfo pbOTURxPt_cookie;
extern LeafInfo pmOTUNearEnd15minMonitored_cookie;
extern LeafInfo pmOTUFarEnd15minMonitored_cookie;
extern LeafInfo pmOTUNearEnd24hrMonitored_cookie;
extern LeafInfo pmOTUFarEnd24hrMonitored_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbSMRxTti_cookie;
extern LeafInfo pbRxPt_cookie;
extern LeafInfo pbRxChannelId_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPRBSEnable_cookie;
extern LeafInfo pbPRBSPattern_cookie;
extern LeafInfo pbPRBSErrorBlocks_cookie;
extern LeafInfo pbPRBSErrorSeconds_cookie;
extern LeafInfo pbPRBSStatus_cookie;
extern LeafInfo pbPRBSActualPortType_cookie;
extern LeafInfo prbsMode_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPRBSHisErrorSeconds_cookie;
extern LeafInfo pbPRBSHisMeanError_cookie;
extern LeafInfo pbPRBSTrapTimeStamp_cookie;
extern LeafInfo pbPRBSHisActualPortType_cookie;
extern LeafInfo pbSdhCurPMTable_cookie;
extern LeafInfo pbEthCurPMTable_cookie;
extern LeafInfo pbEthPmMonTimeIntervalSet_cookie;
extern LeafInfo pbxSecEthPmMonTable_cookie;
extern LeafInfo pbRprClientCurPMTable_cookie;
extern LeafInfo pbRprSpanCurPMTable_cookie;
extern LeafInfo pbPdhCurSlipTable_cookie;
extern LeafInfo pbPmMonitoredNumberTable_cookie;
extern LeafInfo pbPdhPmSlipEventEnable_cookie;
extern LeafInfo pbxSecEthPMTable_cookie;
extern LeafInfo pbPdhCurPMTable_cookie;
extern LeafInfo pbEthVlanPMTable_cookie;
extern LeafInfo pbxSecEthVlanPMTable_cookie;
extern LeafInfo pbEthTcaPMTable_cookie;
extern LeafInfo pbFecCurPMTable_cookie;
extern LeafInfo pbOtuCurPMTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbOtuTableType_cookie;
extern LeafInfo pbChannelID_cookie;
extern LeafInfo pbSegmentNum_cookie;
extern LeafInfo pbOtuPmMonitor_cookie;
extern LeafInfo pbOtuEsPmCounter_cookie;
extern LeafInfo pbOtuBbePmCounter_cookie;
extern LeafInfo pbOtuSesPmCounter_cookie;
extern LeafInfo pbOtuUasPmCounter_cookie;
extern LeafInfo pbOtuCsesPmCounter_cookie;
extern LeafInfo pbOtuFcPmCounter_cookie;
extern LeafInfo pbOtuOfsPmCounter_cookie;
extern LeafInfo pbOtuIaesPmCounter_cookie;
extern LeafInfo pbOtuBiaesPmCounter_cookie;
extern LeafInfo pbOtuPmTableReset_cookie;
extern LeafInfo pbOtuPmTimeStart_cookie;
extern LeafInfo pbOtuPmTimeElapse_cookie;
extern LeafInfo pbOtuPmSuspect_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbFecTableType_cookie;
extern LeafInfo pbFecPmMonitor_cookie;
extern LeafInfo pbFecTecPmCounterHigh_cookie;
extern LeafInfo pbFecTecPmCounterLow_cookie;
extern LeafInfo pbFecZecPmCounterHigh_cookie;
extern LeafInfo pbFecZecPmCounterLow_cookie;
extern LeafInfo pbFecOecPmCounterHigh_cookie;
extern LeafInfo pbFecOecPmCounterLow_cookie;
extern LeafInfo pbFecFcPmCounter_cookie;
extern LeafInfo pbFecPmTableReset_cookie;
extern LeafInfo pbFecPmTimeStart_cookie;
extern LeafInfo pbFecPmTimeElapse_cookie;
extern LeafInfo pbFecPmSuspect_cookie;
extern LeafInfo pbFecUncorrectedBlkHigh_cookie;
extern LeafInfo pbFecUncorrectedBlkLow_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthPmPortType_cookie;
extern LeafInfo pbEthPmBbrEnable_cookie;
extern LeafInfo pbEthPmBbrThresholdValue_cookie;
extern LeafInfo pbEthPmBprEnable_cookie;
extern LeafInfo pbEthPmBprThresholdValue_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthPmPortType_cookie;
extern LeafInfo pbEthVlanNo_cookie;
extern LeafInfo pbxSecEthVlanPmEnableStatus_cookie;
extern LeafInfo pbxSecEthVlanPmTimeStart_cookie;
extern LeafInfo pbxSecEthVlanPmStartFlag_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthVlanTableType_cookie;
extern LeafInfo pbEthPmPortType_cookie;
extern LeafInfo pbEthVlanNo_cookie;
extern LeafInfo pbEthVlanPmMonitor_cookie;
extern LeafInfo pbEthVlanPmTimeStart_cookie;
extern LeafInfo pbEthVlanPmTimeElapse_cookie;
extern LeafInfo pbEthVlanPmTimeSuspect_cookie;
extern LeafInfo pbEthVlanPmTableReset_cookie;
extern LeafInfo pbEthVlanInPktsHigh_cookie;
extern LeafInfo pbEthVlanInPktsLow_cookie;
extern LeafInfo pbEthVlanInBytesHigh_cookie;
extern LeafInfo pbEthVlanInBytesLow_cookie;
extern LeafInfo pbEthVlanDropPktsHigh_cookie;
extern LeafInfo pbEthVlanDropPktsLow_cookie;
extern LeafInfo pbEthVlanRcvUnicastFramesHigh_cookie;
extern LeafInfo pbEthVlanRcvUnicastFramesLow_cookie;
extern LeafInfo pbEthVlanRcvTotalFramesHigh_cookie;
extern LeafInfo pbEthVlanRcvTotalFramesLow_cookie;
extern LeafInfo pbEthVlanRcvUnicastBytesHigh_cookie;
extern LeafInfo pbEthVlanRcvUnicastBytesLow_cookie;
extern LeafInfo pbEthVlanRcvTotalBytesHigh_cookie;
extern LeafInfo pbEthVlanRcvTotalBytesLow_cookie;
extern LeafInfo pbEthVlanPmEnableStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbPdhTableType_cookie;
extern LeafInfo pbPdhPmMonitor_cookie;
extern LeafInfo pbPdhEsPmCounter_cookie;
extern LeafInfo pbPdhEsPmThresholdEnable_cookie;
extern LeafInfo pbPdhEsPmThresholdLow_cookie;
extern LeafInfo pbPdhEsPmThresholdHigh_cookie;
extern LeafInfo pbPdhBbePmCounter_cookie;
extern LeafInfo pbPdhBbePmThresholdEnable_cookie;
extern LeafInfo pbPdhBbePmThresholdLow_cookie;
extern LeafInfo pbPdhBbePmThresholdHigh_cookie;
extern LeafInfo pbPdhSesPmCounter_cookie;
extern LeafInfo pbPdhSesPmThresholdEnable_cookie;
extern LeafInfo pbPdhSesPmThresholdLow_cookie;
extern LeafInfo pbPdhSesPmThresholdHigh_cookie;
extern LeafInfo pbPdhUasPmCounter_cookie;
extern LeafInfo pbPdhUasPmThresholdEnable_cookie;
extern LeafInfo pbPdhUasPmThresholdLow_cookie;
extern LeafInfo pbPdhUasPmThresholdHigh_cookie;
extern LeafInfo pbPdhCsesPmCounter_cookie;
extern LeafInfo pbPdhCsesPmThresholdEnable_cookie;
extern LeafInfo pbPdhCsesPmThresholdLow_cookie;
extern LeafInfo pbPdhCsesPmThresholdHigh_cookie;
extern LeafInfo pbPdhFcPmCounter_cookie;
extern LeafInfo pbPdhFcPmThresholdEnable_cookie;
extern LeafInfo pbPdhFcPmThresholdLow_cookie;
extern LeafInfo pbPdhFcPmThresholdHigh_cookie;
extern LeafInfo pbPdhPmTableReset_cookie;
extern LeafInfo pbPdhPmTimeStart_cookie;
extern LeafInfo pbPdhPmTimeElapse_cookie;
extern LeafInfo pbPdhPmSuspect_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbxSecEthCurPmPortType_cookie;
extern LeafInfo pbxSecEthPmEnableStatus_cookie;
extern LeafInfo pbxSecEthPmTimeStart_cookie;
extern LeafInfo pbxSecEthPmStartFlag_cookie;
extern LeafInfo pbSdhFtpCmndHostIP_cookie;
extern LeafInfo pbPmMonitoredAll_cookie;
extern LeafInfo pbPmMonitoredCtp_cookie;
extern LeafInfo pbPmMonitoredTtp_cookie;
extern LeafInfo pbPmMonitoredEth_cookie;
extern LeafInfo pbPmMonitoredRprClient_cookie;
extern LeafInfo pbPmMonitoredRprSpan_cookie;
extern LeafInfo pbPmMonitoredAtm_cookie;
extern LeafInfo pbPmMonitoredPdhSlip_cookie;
extern LeafInfo pbPmMonitoredPdhCrc_cookie;
extern LeafInfo pbPmMonitoredFec_cookie;
extern LeafInfo pbPmMonitoredOtu_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbSlipTableType_cookie;
extern LeafInfo pbSlipMonitor_cookie;
extern LeafInfo pbPSlipCounter_cookie;
extern LeafInfo pbPSlipThresholdEnable_cookie;
extern LeafInfo pbPSlipThreshold_cookie;
extern LeafInfo pbNSlipCounter_cookie;
extern LeafInfo pbNSlipThresholdEnable_cookie;
extern LeafInfo pbNSlipThreshold_cookie;
extern LeafInfo pbSlipTableReset_cookie;
extern LeafInfo pbSlipTimeStart_cookie;
extern LeafInfo pbSlipTimeElapse_cookie;
extern LeafInfo pbSlipSuspect_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprSpanId_cookie;
extern LeafInfo pbRprSpanTableType_cookie;
extern LeafInfo pbRprSpanPmMonitor_cookie;
extern LeafInfo pbRprSpanPmTimeStart_cookie;
extern LeafInfo pbRprSpanPmTimeElapse_cookie;
extern LeafInfo pbRprSpanPmTimeSuspect_cookie;
extern LeafInfo pbRprSpanPmTableReset_cookie;
extern LeafInfo pbRprSpanInAllFramesHigh_cookie;
extern LeafInfo pbRprSpanInAllFramesLow_cookie;
extern LeafInfo pbRprSpanInWrapFramesHigh_cookie;
extern LeafInfo pbRprSpanInWrapFramesLow_cookie;
extern LeafInfo pbRprSpanInDataFramesHigh_cookie;
extern LeafInfo pbRprSpanInDataFramesLow_cookie;
extern LeafInfo pbRprSpanInCtrlFramesHigh_cookie;
extern LeafInfo pbRprSpanInCtrlFramesLow_cookie;
extern LeafInfo pbRprSpanInFaFramesHigh_cookie;
extern LeafInfo pbRprSpanInFaFramesLow_cookie;
extern LeafInfo pbRprSpanOutAllFramesHigh_cookie;
extern LeafInfo pbRprSpanOutAllFramesLow_cookie;
extern LeafInfo pbRprSpanOutAddFramesHigh_cookie;
extern LeafInfo pbRprSpanOutAddFramesLow_cookie;
extern LeafInfo pbRprSpanOutTransitFramesHigh_cookie;
extern LeafInfo pbRprSpanOutTransitFramesLow_cookie;
extern LeafInfo pbRprSpanOutWrapFramesHigh_cookie;
extern LeafInfo pbRprSpanOutWrapFramesLow_cookie;
extern LeafInfo pbRprSpanOutTpFramesHigh_cookie;
extern LeafInfo pbRprSpanOutTpFramesLow_cookie;
extern LeafInfo pbRprSpanOutFaFramesHigh_cookie;
extern LeafInfo pbRprSpanOutFaFramesLow_cookie;
extern LeafInfo pbRprSpanOutOamFramesHigh_cookie;
extern LeafInfo pbRprSpanOutOamFramesLow_cookie;
extern LeafInfo pbRprSpanErrHecFramesHigh_cookie;
extern LeafInfo pbRprSpanErrHecFramesLow_cookie;
extern LeafInfo pbRprSpanErrParityFramesHigh_cookie;
extern LeafInfo pbRprSpanErrParityFramesLow_cookie;
extern LeafInfo pbRprSpanErrFcsFramesHigh_cookie;
extern LeafInfo pbRprSpanErrFcsFramesLow_cookie;
extern LeafInfo pbRprSpanErrEdgeFramesHigh_cookie;
extern LeafInfo pbRprSpanErrEdgeFramesLow_cookie;
extern LeafInfo pbRprSpanErrSelfSourceFramesHigh_cookie;
extern LeafInfo pbRprSpanErrSelfSourceFramesLow_cookie;
extern LeafInfo pbRprSpanInUcastClassAFramesHigh_cookie;
extern LeafInfo pbRprSpanInUcastClassAFramesLow_cookie;
extern LeafInfo pbRprSpanInUcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprSpanInUcastClassAOctetsLow_cookie;
extern LeafInfo pbRprSpanInUcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprSpanInUcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprSpanInUcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprSpanInUcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprSpanInUcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprSpanInUcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprSpanInUcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprSpanInUcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprSpanInUcastClassCFramesHigh_cookie;
extern LeafInfo pbRprSpanInUcastClassCFramesLow_cookie;
extern LeafInfo pbRprSpanInUcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprSpanInUcastClassCOctetsLow_cookie;
extern LeafInfo pbRprSpanInMcastClassAFramesHigh_cookie;
extern LeafInfo pbRprSpanInMcastClassAFramesLow_cookie;
extern LeafInfo pbRprSpanInMcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprSpanInMcastClassAOctetsLow_cookie;
extern LeafInfo pbRprSpanInMcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprSpanInMcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprSpanInMcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprSpanInMcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprSpanInMcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprSpanInMcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprSpanInMcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprSpanInMcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprSpanInMcastClassCFramesHigh_cookie;
extern LeafInfo pbRprSpanInMcastClassCFramesLow_cookie;
extern LeafInfo pbRprSpanInMcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprSpanInMcastClassCOctetsLow_cookie;
extern LeafInfo pbRprSpanOutUcastClassAFramesHigh_cookie;
extern LeafInfo pbRprSpanOutUcastClassAFramesLow_cookie;
extern LeafInfo pbRprSpanOutUcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprSpanOutUcastClassAOctetsLow_cookie;
extern LeafInfo pbRprSpanOutUcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprSpanOutUcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprSpanOutUcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprSpanOutUcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprSpanOutUcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprSpanOutUcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprSpanOutUcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprSpanOutUcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprSpanOutUcastClassCFramesHigh_cookie;
extern LeafInfo pbRprSpanOutUcastClassCFramesLow_cookie;
extern LeafInfo pbRprSpanOutUcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprSpanOutUcastClassCOctetsLow_cookie;
extern LeafInfo pbRprSpanOutMcastClassAFramesHigh_cookie;
extern LeafInfo pbRprSpanOutMcastClassAFramesLow_cookie;
extern LeafInfo pbRprSpanOutMcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprSpanOutMcastClassAOctetsLow_cookie;
extern LeafInfo pbRprSpanOutMcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprSpanOutMcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprSpanOutMcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprSpanOutMcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprSpanOutMcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprSpanOutMcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprSpanOutMcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprSpanOutMcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprSpanOutMcastClassCFramesHigh_cookie;
extern LeafInfo pbRprSpanOutMcastClassCFramesLow_cookie;
extern LeafInfo pbRprSpanOutMcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprSpanOutMcastClassCOctetsLow_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo pbRprClientTableType_cookie;
extern LeafInfo pbRprClientPmMonitor_cookie;
extern LeafInfo pbRprClientPmTimeStart_cookie;
extern LeafInfo pbRprClientPmTimeElapse_cookie;
extern LeafInfo pbRprClientPmTimeSuspect_cookie;
extern LeafInfo pbRprClientPmTableReset_cookie;
extern LeafInfo pbRprClientInAllFramesHigh_cookie;
extern LeafInfo pbRprClientInAllFramesLow_cookie;
extern LeafInfo pbRprClientOutAllFramesHigh_cookie;
extern LeafInfo pbRprClientOutAllFramesLow_cookie;
extern LeafInfo pbRprClientOutPassFramesHigh_cookie;
extern LeafInfo pbRprClientOutPassFramesLow_cookie;
extern LeafInfo pbRprClientOutDropFramesHigh_cookie;
extern LeafInfo pbRprClientOutDropFramesLow_cookie;
extern LeafInfo pbRprClientOutErrLenFramesHigh_cookie;
extern LeafInfo pbRprClientOutErrLenFramesLow_cookie;
extern LeafInfo pbRprClientOutErrCrcFramesHigh_cookie;
extern LeafInfo pbRprClientOutErrCrcFramesLow_cookie;
extern LeafInfo pbRprClientInUcastClassAFramesHigh_cookie;
extern LeafInfo pbRprClientInUcastClassAFramesLow_cookie;
extern LeafInfo pbRprClientInUcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprClientInUcastClassAOctetsLow_cookie;
extern LeafInfo pbRprClientInUcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprClientInUcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprClientInUcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprClientInUcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprClientInUcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprClientInUcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprClientInUcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprClientInUcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprClientInUcastClassCFramesHigh_cookie;
extern LeafInfo pbRprClientInUcastClassCFramesLow_cookie;
extern LeafInfo pbRprClientInUcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprClientInUcastClassCOctetsLow_cookie;
extern LeafInfo pbRprClientInMcastClassAFramesHigh_cookie;
extern LeafInfo pbRprClientInMcastClassAFramesLow_cookie;
extern LeafInfo pbRprClientInMcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprClientInMcastClassAOctetsLow_cookie;
extern LeafInfo pbRprClientInMcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprClientInMcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprClientInMcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprClientInMcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprClientInMcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprClientInMcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprClientInMcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprClientInMcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprClientInMcastClassCFramesHigh_cookie;
extern LeafInfo pbRprClientInMcastClassCFramesLow_cookie;
extern LeafInfo pbRprClientInMcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprClientInMcastClassCOctetsLow_cookie;
extern LeafInfo pbRprClientOutUcastClassAFramesHigh_cookie;
extern LeafInfo pbRprClientOutUcastClassAFramesLow_cookie;
extern LeafInfo pbRprClientOutUcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprClientOutUcastClassAOctetsLow_cookie;
extern LeafInfo pbRprClientOutUcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprClientOutUcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprClientOutUcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprClientOutUcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprClientOutUcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprClientOutUcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprClientOutUcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprClientOutUcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprClientOutUcastClassCFramesHigh_cookie;
extern LeafInfo pbRprClientOutUcastClassCFramesLow_cookie;
extern LeafInfo pbRprClientOutUcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprClientOutUcastClassCOctetsLow_cookie;
extern LeafInfo pbRprClientOutMcastClassAFramesHigh_cookie;
extern LeafInfo pbRprClientOutMcastClassAFramesLow_cookie;
extern LeafInfo pbRprClientOutMcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprClientOutMcastClassAOctetsLow_cookie;
extern LeafInfo pbRprClientOutMcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprClientOutMcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprClientOutMcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprClientOutMcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprClientOutMcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprClientOutMcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprClientOutMcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprClientOutMcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprClientOutMcastClassCFramesHigh_cookie;
extern LeafInfo pbRprClientOutMcastClassCFramesLow_cookie;
extern LeafInfo pbRprClientOutMcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprClientOutMcastClassCOctetsLow_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbxSecEthPmPortType_cookie;
extern LeafInfo pbxSecEthPmReportStatus_cookie;
extern LeafInfo pbxSecEthPmMonitorStatus_cookie;
extern LeafInfo pbxSecEthPmFtpHostIP_cookie;
extern LeafInfo pbxSecEthPmFtpPath_cookie;
extern LeafInfo pbxSecEthPmFtpFileName_cookie;
extern LeafInfo pbxSecEthPmFtpUsername_cookie;
extern LeafInfo pbxSecEthPmFtpPassword_cookie;
extern LeafInfo pbxSecEthPmRequestId_cookie;
extern LeafInfo pbxSecEthPmRespMsg_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthTableType_cookie;
extern LeafInfo pbEthPmPortType_cookie;
extern LeafInfo pbEthPmMonitor_cookie;
extern LeafInfo pbEthPmTimeStart_cookie;
extern LeafInfo pbEthPmTimeElapse_cookie;
extern LeafInfo pbEthPmTimeSuspect_cookie;
extern LeafInfo pbEthPmTableReset_cookie;
extern LeafInfo pbEthInUnicastPktsHigh_cookie;
extern LeafInfo pbEthInUnicastPktsLow_cookie;
extern LeafInfo pbEthInMulcastPktsHigh_cookie;
extern LeafInfo pbEthInMulcastPktsLow_cookie;
extern LeafInfo pbEthInBrdcastPktsHigh_cookie;
extern LeafInfo pbEthInBrdcastPktsLow_cookie;
extern LeafInfo pbEthInAllBadPktsHigh_cookie;
extern LeafInfo pbEthInAllBadPktsLow_cookie;
extern LeafInfo pbEthInAllBadBytesHigh_cookie;
extern LeafInfo pbEthInAllBadBytesLow_cookie;
extern LeafInfo pbEthInAllGoodPktsHigh_cookie;
extern LeafInfo pbEthInAllGoodPktsLow_cookie;
extern LeafInfo pbEthInAllGoodBytesHigh_cookie;
extern LeafInfo pbEthInAllGoodBytesLow_cookie;
extern LeafInfo pbEthInOverSizePktsHigh_cookie;
extern LeafInfo pbEthInOverSizePktsLow_cookie;
extern LeafInfo pbEthInUnderSizePktsHigh_cookie;
extern LeafInfo pbEthInUnderSizePktsLow_cookie;
extern LeafInfo pbEthInCRCErrPktsHigh_cookie;
extern LeafInfo pbEthInCRCErrPktsLow_cookie;
extern LeafInfo pbEthOutUnicastPktsHigh_cookie;
extern LeafInfo pbEthOutUnicastPktsLow_cookie;
extern LeafInfo pbEthOutMulcastPktsHigh_cookie;
extern LeafInfo pbEthOutMulcastPktsLow_cookie;
extern LeafInfo pbEthOutBrdcastPktsHigh_cookie;
extern LeafInfo pbEthOutBrdcastPktsLow_cookie;
extern LeafInfo pbEthOutAllGoodPktsHigh_cookie;
extern LeafInfo pbEthOutAllGoodPktsLow_cookie;
extern LeafInfo pbEthOutAllGoodBytesHigh_cookie;
extern LeafInfo pbEthOutAllGoodBytesLow_cookie;
extern LeafInfo pbEthBbrEnable_cookie;
extern LeafInfo pbEthBbrThresholdValue_cookie;
extern LeafInfo pbEthBprEnable_cookie;
extern LeafInfo pbEthBprThresholdValue_cookie;
extern LeafInfo pbEthCurInAllBytesHigh_cookie;
extern LeafInfo pbEthCurInAllBytesLow_cookie;
extern LeafInfo pbEthCurInAllPktsHigh_cookie;
extern LeafInfo pbEthCurInAllPktsLow_cookie;
extern LeafInfo pbEthCurInAllGoodPktsHigh_cookie;
extern LeafInfo pbEthCurInAllGoodPktsLow_cookie;
extern LeafInfo pbEthCurInAllBadPktsHigh_cookie;
extern LeafInfo pbEthCurInAllBadPktsLow_cookie;
extern LeafInfo pbEthCurInUnicastPktsHigh_cookie;
extern LeafInfo pbEthCurInUnicastPktsLow_cookie;
extern LeafInfo pbEthCurInMulcastPktsHigh_cookie;
extern LeafInfo pbEthCurInMulcastPktsLow_cookie;
extern LeafInfo pbEthCurInBrdcastPktsHigh_cookie;
extern LeafInfo pbEthCurInBrdcastPktsLow_cookie;
extern LeafInfo pbEthCurInDropPktsHigh_cookie;
extern LeafInfo pbEthCurInDropPktsLow_cookie;
extern LeafInfo pbEthCurInUndersizeGoodPktsHigh_cookie;
extern LeafInfo pbEthCurInUndersizeGoodPktsLow_cookie;
extern LeafInfo pbEthCurInOversizeGoodPktsHigh_cookie;
extern LeafInfo pbEthCurInOversizeGoodPktsLow_cookie;
extern LeafInfo pbEthCurIn64PktsHigh_cookie;
extern LeafInfo pbEthCurIn64PktsLow_cookie;
extern LeafInfo pbEthCurIn65To127PktsHigh_cookie;
extern LeafInfo pbEthCurIn65To127PktsLow_cookie;
extern LeafInfo pbEthCurIn128To255PktsHigh_cookie;
extern LeafInfo pbEthCurIn128To255PktsLow_cookie;
extern LeafInfo pbEthCurIn256To511PktsHigh_cookie;
extern LeafInfo pbEthCurIn256To511PktsLow_cookie;
extern LeafInfo pbEthCurIn512To1023PktsHigh_cookie;
extern LeafInfo pbEthCurIn512To1023PktsLow_cookie;
extern LeafInfo pbEthCurIn1024To1518PktsHigh_cookie;
extern LeafInfo pbEthCurIn1024To1518PktsLow_cookie;
extern LeafInfo pbEthCurInPausePktsHigh_cookie;
extern LeafInfo pbEthCurInPausePktsLow_cookie;
extern LeafInfo pbEthCurInFragmentPktsHigh_cookie;
extern LeafInfo pbEthCurInFragmentPktsLow_cookie;
extern LeafInfo pbEthCurInJabberPktsHigh_cookie;
extern LeafInfo pbEthCurInJabberPktsLow_cookie;
extern LeafInfo pbEthCurInAlignmentErrorPktsHigh_cookie;
extern LeafInfo pbEthCurInAlignmentErrorPktsLow_cookie;
extern LeafInfo pbEthCurOutAllBytesHigh_cookie;
extern LeafInfo pbEthCurOutAllBytesLow_cookie;
extern LeafInfo pbEthCurOutAllPktsHigh_cookie;
extern LeafInfo pbEthCurOutAllPktsLow_cookie;
extern LeafInfo pbEthCurOutUnicastPktsHigh_cookie;
extern LeafInfo pbEthCurOutUnicastPktsLow_cookie;
extern LeafInfo pbEthCurOutMulcastPktsHigh_cookie;
extern LeafInfo pbEthCurOutMulcastPktsLow_cookie;
extern LeafInfo pbEthCurOutBrdcastPktsHigh_cookie;
extern LeafInfo pbEthCurOutBrdcastPktsLow_cookie;
extern LeafInfo pbEthCurOutGoodPausePktsHigh_cookie;
extern LeafInfo pbEthCurOutGoodPausePktsLow_cookie;
extern LeafInfo pbEthCurOutCollisionPktsHigh_cookie;
extern LeafInfo pbEthCurOutCollisionPktsLow_cookie;
extern LeafInfo pbCurOutInvalidSymbolsHigh_cookie;
extern LeafInfo pbCurOutInvalidSymbolsLow_cookie;
extern LeafInfo pbCurOutSuperblockCrcErrorHigh_cookie;
extern LeafInfo pbCurOutSuperblockCrcErrorLow_cookie;
extern LeafInfo pbCurOutSuperblockCorrectableCrcErrorHigh_cookie;
extern LeafInfo pbCurOutSuperblockCorrectableCrcErrorLow_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbTpType_cookie;
extern LeafInfo pbTpIndex_cookie;
extern LeafInfo pbTableType_cookie;
extern LeafInfo pbPmActualPortType_cookie;
extern LeafInfo pbSdhPmMonitor_cookie;
extern LeafInfo pbEsPmCounter_cookie;
extern LeafInfo pbEsPmThresholdEnable_cookie;
extern LeafInfo pbEsPmThresholdLow_cookie;
extern LeafInfo pbEsPmThresholdHigh_cookie;
extern LeafInfo pbBbePmCounter_cookie;
extern LeafInfo pbBbePmThresholdEnable_cookie;
extern LeafInfo pbBbePmThresholdLow_cookie;
extern LeafInfo pbBbePmThresholdHigh_cookie;
extern LeafInfo pbSesPmCounter_cookie;
extern LeafInfo pbSesPmThresholdEnable_cookie;
extern LeafInfo pbSesPmThresholdLow_cookie;
extern LeafInfo pbSesPmThresholdHigh_cookie;
extern LeafInfo pbUasPmCounter_cookie;
extern LeafInfo pbUasPmThresholdEnable_cookie;
extern LeafInfo pbUasPmThresholdLow_cookie;
extern LeafInfo pbUasPmThresholdHigh_cookie;
extern LeafInfo pbCsesPmCounter_cookie;
extern LeafInfo pbCsesPmThresholdEnable_cookie;
extern LeafInfo pbCsesPmThresholdLow_cookie;
extern LeafInfo pbCsesPmThresholdHigh_cookie;
extern LeafInfo pbFcPmCounter_cookie;
extern LeafInfo pbFcPmThresholdEnable_cookie;
extern LeafInfo pbFcPmThresholdLow_cookie;
extern LeafInfo pbFcPmThresholdHigh_cookie;
extern LeafInfo pbOfsPmCounter_cookie;
extern LeafInfo pbOfsPmThresholdEnable_cookie;
extern LeafInfo pbOfsPmThresholdLow_cookie;
extern LeafInfo pbOfsPmThresholdHigh_cookie;
extern LeafInfo pbPpjcPmCounter_cookie;
extern LeafInfo pbPpjcPmThresholdEnable_cookie;
extern LeafInfo pbPpjcPmThresholdLow_cookie;
extern LeafInfo pbPpjcPmThresholdHigh_cookie;
extern LeafInfo pbNpjcPmCounter_cookie;
extern LeafInfo pbNpjcPmThresholdEnable_cookie;
extern LeafInfo pbNpjcPmThresholdLow_cookie;
extern LeafInfo pbNpjcPmThresholdHigh_cookie;
extern LeafInfo pbPmTableReset_cookie;
extern LeafInfo pbPmTimeStart_cookie;
extern LeafInfo pbPmTimeElapse_cookie;
extern LeafInfo pbPmProtStatus_cookie;
extern LeafInfo pbPmTpStatus_cookie;
extern LeafInfo pbPmSuspect_cookie;
extern LeafInfo pbSdhRecentPMTable_cookie;
extern LeafInfo pbEthRecentPMTable_cookie;
extern LeafInfo pbRprClientRecentPMTable_cookie;
extern LeafInfo pbRprSpanRecentPMTable_cookie;
extern LeafInfo pbPdhRecentSlipTable_cookie;
extern LeafInfo pbxSecEthRecentPMTable_cookie;
extern LeafInfo pbPdhRecentPMTable_cookie;
extern LeafInfo pbEthVlanRecentPMTable_cookie;
extern LeafInfo pbxSecEthVlanRecentPMTable_cookie;
extern LeafInfo pbFecRecentPMTable_cookie;
extern LeafInfo pbOtuRecentPMTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbOtuRecentChannelID_cookie;
extern LeafInfo pbOtuRecentSegmentNum_cookie;
extern LeafInfo pbOtuRecentTableType_cookie;
extern LeafInfo pbOtuRecentTimesIndex_cookie;
extern LeafInfo pbOtuRecentEsPmCounter_cookie;
extern LeafInfo pbOtuRecentBbePmCounter_cookie;
extern LeafInfo pbOtuRecentSesPmCounter_cookie;
extern LeafInfo pbOtuRecentUasPmCounter_cookie;
extern LeafInfo pbOtuRecentCsesPmCounter_cookie;
extern LeafInfo pbOtuRecentFcPmCounter_cookie;
extern LeafInfo pbOtuRecentOfsPmCounter_cookie;
extern LeafInfo pbOtuRecentIaesPmCounter_cookie;
extern LeafInfo pbOtuRecentBiaesPmCounter_cookie;
extern LeafInfo pbOtuRecentPmTimeEnd_cookie;
extern LeafInfo pbOtuSuspect_cookie;
extern LeafInfo pbOtuComplete_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbFecRecentTableType_cookie;
extern LeafInfo pbFecRecentTimesIndex_cookie;
extern LeafInfo pbFecRecentTecPmCounterHigh_cookie;
extern LeafInfo pbFecRecentTecPmCounterLow_cookie;
extern LeafInfo pbFecRecentZecPmCounterHigh_cookie;
extern LeafInfo pbFecRecentZecPmCounterLow_cookie;
extern LeafInfo pbFecRecentOecPmCounterHigh_cookie;
extern LeafInfo pbFecRecentOecPmCounterLow_cookie;
extern LeafInfo pbFecRecentFcPmCounter_cookie;
extern LeafInfo pbFecRecentPmTimeEnd_cookie;
extern LeafInfo pbFecSuspect_cookie;
extern LeafInfo pbFecComplete_cookie;
extern LeafInfo pbFecRecUncorrectedBlkHigh_cookie;
extern LeafInfo pbFecRecUncorrectedBlkLow_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthPmPortType_cookie;
extern LeafInfo pbEthVlanNo_cookie;
extern LeafInfo pbxSecEthVlanTimesIndex_cookie;
extern LeafInfo pbxSecEthVlanRecentPmStartFlag_cookie;
extern LeafInfo pbxSecEthVlanRecentInPkts_cookie;
extern LeafInfo pbxSecEthVlanRecentInBytes_cookie;
extern LeafInfo pbxSecEthVlanRecentDropPkts_cookie;
extern LeafInfo pbxSecEthVlanRecentRcvUnicastFrames_cookie;
extern LeafInfo pbxSecEthVlanRecentRcvTotalFrames_cookie;
extern LeafInfo pbxSecEthVlanRecentRcvUnicastBytes_cookie;
extern LeafInfo pbxSecEthVlanRecentRcvTotalBytes_cookie;
extern LeafInfo pbxSecEthVlanFrmStartIndex_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthVlanRecentTableType_cookie;
extern LeafInfo pbEthPmPortType_cookie;
extern LeafInfo pbEthVlanNo_cookie;
extern LeafInfo pbEthVlanRecentTimesIndex_cookie;
extern LeafInfo pbEthVlanRecentPmTimeEnd_cookie;
extern LeafInfo pbEthVlanRecentPmTimeSuspect_cookie;
extern LeafInfo pbEthVlanRecentInPktsHigh_cookie;
extern LeafInfo pbEthVlanRecentInPktsLow_cookie;
extern LeafInfo pbEthVlanRecentInBytesHigh_cookie;
extern LeafInfo pbEthVlanRecentInBytesLow_cookie;
extern LeafInfo pbEthVlanRecentDropPktsHigh_cookie;
extern LeafInfo pbEthVlanRecentDropPktsLow_cookie;
extern LeafInfo pbEthVlanRecentRcvUnicastFramesHigh_cookie;
extern LeafInfo pbEthVlanRecentRcvUnicastFramesLow_cookie;
extern LeafInfo pbEthVlanRecentRcvTotalFramesHigh_cookie;
extern LeafInfo pbEthVlanRecentRcvTotalFramesLow_cookie;
extern LeafInfo pbEthVlanRecentRcvUnicastBytesHigh_cookie;
extern LeafInfo pbEthVlanRecentRcvUnicastBytesLow_cookie;
extern LeafInfo pbEthVlanRecentRcvTotalBytesHigh_cookie;
extern LeafInfo pbEthVlanRecentRcvTotalBytesLow_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbPdhRecentTableType_cookie;
extern LeafInfo pbPdhRecentTimesIndex_cookie;
extern LeafInfo pbPdhRecentEsPmCounter_cookie;
extern LeafInfo pbPdhRecentBbePmCounter_cookie;
extern LeafInfo pbPdhRecentSesPmCounter_cookie;
extern LeafInfo pbPdhRecentUasPmCounter_cookie;
extern LeafInfo pbPdhRecentCsesPmCounter_cookie;
extern LeafInfo pbPdhRecentFcPmCounter_cookie;
extern LeafInfo pbPdhRecentPmTimeEnd_cookie;
extern LeafInfo pbPdhSuspect_cookie;
extern LeafInfo pbPdhComplete_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbxSecEthRecentPmPortType_cookie;
extern LeafInfo pbxSecEthTimesIndex_cookie;
extern LeafInfo pbxSecEthRecentPmStartFlag_cookie;
extern LeafInfo pbxSecEthRecentInUnicastPkts_cookie;
extern LeafInfo pbxSecEthRecentInMulcastPkts_cookie;
extern LeafInfo pbxSecEthRecentInBrdcastPkts_cookie;
extern LeafInfo pbxSecEthRecentInAllBadPkts_cookie;
extern LeafInfo pbxSecEthRecentInAllBadBytes_cookie;
extern LeafInfo pbxSecEthRecentInAllGoodPkts_cookie;
extern LeafInfo pbxSecEthRecentInAllGoodBytes_cookie;
extern LeafInfo pbxSecEthRecentInOverSizePkts_cookie;
extern LeafInfo pbxSecEthRecentInUnderSizePkts_cookie;
extern LeafInfo pbxSecEthRecentInCRCErrPkts_cookie;
extern LeafInfo pbxSecEthRecentOutUnicastPkts_cookie;
extern LeafInfo pbxSecEthRecentOutMulcastPkts_cookie;
extern LeafInfo pbxSecEthRecentOutBrdcastPkts_cookie;
extern LeafInfo pbxSecEthRecentOutAllGoodPkts_cookie;
extern LeafInfo pbxSecEthRecentOutAllGoodBytes_cookie;
extern LeafInfo pbxSecEthFrmStartIndex_cookie;
extern LeafInfo pbxSecEthRcntInAllBytes_cookie;
extern LeafInfo pbxSecEthRcntInAllPkts_cookie;
extern LeafInfo pbxSecEthRcntInAllGoodPkts_cookie;
extern LeafInfo pbxSecEthRcntInAllBadPkts_cookie;
extern LeafInfo pbxSecEthRcntInUnicastPkts_cookie;
extern LeafInfo pbxSecEthRcntInMulcastPkts_cookie;
extern LeafInfo pbxSecEthRcntInBrdcastPkts_cookie;
extern LeafInfo pbxSecEthRcntInDropPkts_cookie;
extern LeafInfo pbxSecEthRcntInUndersizeGoodPkts_cookie;
extern LeafInfo pbxSecEthRcntInOversizeGoodPkts_cookie;
extern LeafInfo pbxSecEthRcntIn64Pkts_cookie;
extern LeafInfo pbxSecEthRcntIn65To127Pkts_cookie;
extern LeafInfo pbxSecEthRcntIn128To255Pkts_cookie;
extern LeafInfo pbxSecEthRcntIn256To511Pkts_cookie;
extern LeafInfo pbxSecEthRcntIn512To1023Pkts_cookie;
extern LeafInfo pbxSecEthRcntIn1024To1518Pkts_cookie;
extern LeafInfo pbxSecEthRcntInPausePkts_cookie;
extern LeafInfo pbxSecEthRcntInFragmentPkts_cookie;
extern LeafInfo pbxSecEthRcntInJabberPkts_cookie;
extern LeafInfo pbxSecEthRcntInAlignmentErrorPkts_cookie;
extern LeafInfo pbxSecEthRcntOutAllBytes_cookie;
extern LeafInfo pbxSecEthRcntOutAllPkts_cookie;
extern LeafInfo pbxSecEthRcntOutUnicastPkts_cookie;
extern LeafInfo pbxSecEthRcntOutMulcastPkts_cookie;
extern LeafInfo pbxSecEthRcntOutBrdcastPkts_cookie;
extern LeafInfo pbxSecEthRcntOutGoodPausePkts_cookie;
extern LeafInfo pbxSecEthRcntOutCollisionPkts_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbRecentSlipTableType_cookie;
extern LeafInfo pbRecentSlipTimesIndex_cookie;
extern LeafInfo pbRecentPSlipCounter_cookie;
extern LeafInfo pbRecentNSlipCounter_cookie;
extern LeafInfo pbSlipTimeEnd_cookie;
extern LeafInfo pbRecentSlipSuspect_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprSpanId_cookie;
extern LeafInfo pbRprSpanRecentTableType_cookie;
extern LeafInfo pbRprSpanTableRecentTimesIndex_cookie;
extern LeafInfo pbRprSpanRecentPmTimeEnd_cookie;
extern LeafInfo pbRprSpanRecentPmTimeSuspect_cookie;
extern LeafInfo pbRprSpanRecentInAllFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInAllFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInWrapFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInWrapFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInDataFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInDataFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInCtrlFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInCtrlFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInFaFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInFaFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutAllFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutAllFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutAddFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutAddFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutTransitFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutTransitFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutWrapFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutWrapFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutTpFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutTpFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutFaFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutFaFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutOamFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutOamFramesLow_cookie;
extern LeafInfo pbRprSpanRecentErrHecFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentErrHecFramesLow_cookie;
extern LeafInfo pbRprSpanRecentErrParityFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentErrParityFramesLow_cookie;
extern LeafInfo pbRprSpanRecentErrFcsFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentErrFcsFramesLow_cookie;
extern LeafInfo pbRprSpanRecentErrEdgeFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentErrEdgeFramesLow_cookie;
extern LeafInfo pbRprSpanRecentErrSelfSourceFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentErrSelfSourceFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassAFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassAFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassAOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassCFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassCFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentInUcastClassCOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassAFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassAFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassAOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassCFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassCFramesLow_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentInMcastClassCOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassAFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassAFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassAOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassCFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassCFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentOutUcastClassCOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassAFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassAFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassAOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassCFramesHigh_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassCFramesLow_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprSpanRecentOutMcastClassCOctetsLow_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo pbRprClientRecentTableType_cookie;
extern LeafInfo pbRprClientTableRecentTimesIndex_cookie;
extern LeafInfo pbRprClientRecentPmTimeEnd_cookie;
extern LeafInfo pbRprClientRecentPmTimeSuspect_cookie;
extern LeafInfo pbRprClientRecentInAllFramesHigh_cookie;
extern LeafInfo pbRprClientRecentInAllFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutAllFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutAllFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutPassFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutPassFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutDropFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutDropFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutErrLenFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutErrLenFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutErrCrcFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutErrCrcFramesLow_cookie;
extern LeafInfo pbRprClientRecentInUcastClassAFramesHigh_cookie;
extern LeafInfo pbRprClientRecentInUcastClassAFramesLow_cookie;
extern LeafInfo pbRprClientRecentInUcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentInUcastClassAOctetsLow_cookie;
extern LeafInfo pbRprClientRecentInUcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprClientRecentInUcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprClientRecentInUcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentInUcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprClientRecentInUcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprClientRecentInUcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprClientRecentInUcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentInUcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprClientRecentInUcastClassCFramesHigh_cookie;
extern LeafInfo pbRprClientRecentInUcastClassCFramesLow_cookie;
extern LeafInfo pbRprClientRecentInUcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentInUcastClassCOctetsLow_cookie;
extern LeafInfo pbRprClientRecentInMcastClassAFramesHigh_cookie;
extern LeafInfo pbRprClientRecentInMcastClassAFramesLow_cookie;
extern LeafInfo pbRprClientRecentInMcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentInMcastClassAOctetsLow_cookie;
extern LeafInfo pbRprClientRecentInMcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprClientRecentInMcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprClientRecentInMcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentInMcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprClientRecentInMcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprClientRecentInMcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprClientRecentInMcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentInMcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprClientRecentInMcastClassCFramesHigh_cookie;
extern LeafInfo pbRprClientRecentInMcastClassCFramesLow_cookie;
extern LeafInfo pbRprClientRecentInMcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentInMcastClassCOctetsLow_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassAFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassAFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassAOctetsLow_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassCFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassCFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentOutUcastClassCOctetsLow_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassAFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassAFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassAOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassAOctetsLow_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassBCirFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassBCirFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassBCirOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassBCirOctetsLow_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassBEirFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassBEirFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassBEirOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassBEirOctetsLow_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassCFramesHigh_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassCFramesLow_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassCOctetsHigh_cookie;
extern LeafInfo pbRprClientRecentOutMcastClassCOctetsLow_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthRecentTableType_cookie;
extern LeafInfo pbEthRecentPmPortType_cookie;
extern LeafInfo pbEthRecentTimesIndex_cookie;
extern LeafInfo pbEthRecentPmTimeEnd_cookie;
extern LeafInfo pbEthRecentPmTimeSuspect_cookie;
extern LeafInfo pbEthRecentInUnicastPktsHigh_cookie;
extern LeafInfo pbEthRecentInUnicastPktsLow_cookie;
extern LeafInfo pbEthRecentInMulcastPktsHigh_cookie;
extern LeafInfo pbEthRecentInMulcastPktsLow_cookie;
extern LeafInfo pbEthRecentInBrdcastPktsHigh_cookie;
extern LeafInfo pbEthRecentInBrdcastPktsLow_cookie;
extern LeafInfo pbEthRecentInAllBadPktsHigh_cookie;
extern LeafInfo pbEthRecentInAllBadPktsLow_cookie;
extern LeafInfo pbEthRecentInAllBadBytesHigh_cookie;
extern LeafInfo pbEthRecentInAllBadBytesLow_cookie;
extern LeafInfo pbEthRecentInAllGoodPktsHigh_cookie;
extern LeafInfo pbEthRecentInAllGoodPktsLow_cookie;
extern LeafInfo pbEthRecentInAllGoodBytesHigh_cookie;
extern LeafInfo pbEthRecentInAllGoodBytesLow_cookie;
extern LeafInfo pbEthRecentInOverSizePktsHigh_cookie;
extern LeafInfo pbEthRecentInOverSizePktsLow_cookie;
extern LeafInfo pbEthRecentInUnderSizePktsHigh_cookie;
extern LeafInfo pbEthRecentInUnderSizePktsLow_cookie;
extern LeafInfo pbEthRecentInCRCErrPktsHigh_cookie;
extern LeafInfo pbEthRecentInCRCErrPktsLow_cookie;
extern LeafInfo pbEthRecentOutUnicastPktsHigh_cookie;
extern LeafInfo pbEthRecentOutUnicastPktsLow_cookie;
extern LeafInfo pbEthRecentOutMulcastPktsHigh_cookie;
extern LeafInfo pbEthRecentOutMulcastPktsLow_cookie;
extern LeafInfo pbEthRecentOutBrdcastPktsHigh_cookie;
extern LeafInfo pbEthRecentOutBrdcastPktsLow_cookie;
extern LeafInfo pbEthRecentOutAllGoodPktsHigh_cookie;
extern LeafInfo pbEthRecentOutAllGoodPktsLow_cookie;
extern LeafInfo pbEthRecentOutAllGoodBytesHigh_cookie;
extern LeafInfo pbEthRecentOutAllGoodBytesLow_cookie;
extern LeafInfo pbEthRcntInAllBytesHigh_cookie;
extern LeafInfo pbEthRcntInAllBytesLow_cookie;
extern LeafInfo pbEthRcntInAllPktsHigh_cookie;
extern LeafInfo pbEthRcntInAllPktsLow_cookie;
extern LeafInfo pbEthRcntInAllGoodPktsHigh_cookie;
extern LeafInfo pbEthRcntInAllGoodPktsLow_cookie;
extern LeafInfo pbEthRcntInAllBadPktsHigh_cookie;
extern LeafInfo pbEthRcntInAllBadPktsLow_cookie;
extern LeafInfo pbEthRcntInUnicastPktsHigh_cookie;
extern LeafInfo pbEthRcntInUnicastPktsLow_cookie;
extern LeafInfo pbEthRcntInMulcastPktsHigh_cookie;
extern LeafInfo pbEthRcntInMulcastPktsLow_cookie;
extern LeafInfo pbEthRcntInBrdcastPktsHigh_cookie;
extern LeafInfo pbEthRcntInBrdcastPktsLow_cookie;
extern LeafInfo pbEthRcntInDropPktsHigh_cookie;
extern LeafInfo pbEthRcntInDropPktsLow_cookie;
extern LeafInfo pbEthRcntInUndersizeGoodPktsHigh_cookie;
extern LeafInfo pbEthRcntInUndersizeGoodPktsLow_cookie;
extern LeafInfo pbEthRcntInOversizeGoodPktsHigh_cookie;
extern LeafInfo pbEthRcntInOversizeGoodPktsLow_cookie;
extern LeafInfo pbEthRcntIn64PktsHigh_cookie;
extern LeafInfo pbEthRcntIn64PktsLow_cookie;
extern LeafInfo pbEthRcntIn65To127PktsHigh_cookie;
extern LeafInfo pbEthRcntIn65To127PktsLow_cookie;
extern LeafInfo pbEthRcntIn128To255PktsHigh_cookie;
extern LeafInfo pbEthRcntIn128To255PktsLow_cookie;
extern LeafInfo pbEthRcntIn256To511PktsHigh_cookie;
extern LeafInfo pbEthRcntIn256To511PktsLow_cookie;
extern LeafInfo pbEthRcntIn512To1023PktsHigh_cookie;
extern LeafInfo pbEthRcntIn512To1023PktsLow_cookie;
extern LeafInfo pbEthRcntIn1024To1518PktsHigh_cookie;
extern LeafInfo pbEthRcntIn1024To1518PktsLow_cookie;
extern LeafInfo pbEthRcntInPausePktsHigh_cookie;
extern LeafInfo pbEthRcntInPausePktsLow_cookie;
extern LeafInfo pbEthRcntInFragmentPktsHigh_cookie;
extern LeafInfo pbEthRcntInFragmentPktsLow_cookie;
extern LeafInfo pbEthRcntInJabberPktsHigh_cookie;
extern LeafInfo pbEthRcntInJabberPktsLow_cookie;
extern LeafInfo pbEthRcntInAlignmentErrorPktsHigh_cookie;
extern LeafInfo pbEthRcntInAlignmentErrorPktsLow_cookie;
extern LeafInfo pbEthRcntOutAllBytesHigh_cookie;
extern LeafInfo pbEthRcntOutAllBytesLow_cookie;
extern LeafInfo pbEthRcntOutAllPktsHigh_cookie;
extern LeafInfo pbEthRcntOutAllPktsLow_cookie;
extern LeafInfo pbEthRcntOutUnicastPktsHigh_cookie;
extern LeafInfo pbEthRcntOutUnicastPktsLow_cookie;
extern LeafInfo pbEthRcntOutMulcastPktsHigh_cookie;
extern LeafInfo pbEthRcntOutMulcastPktsLow_cookie;
extern LeafInfo pbEthRcntOutBrdcastPktsHigh_cookie;
extern LeafInfo pbEthRcntOutBrdcastPktsLow_cookie;
extern LeafInfo pbEthRcntOutGoodPausePktsHigh_cookie;
extern LeafInfo pbEthRcntOutGoodPausePktsLow_cookie;
extern LeafInfo pbEthRcntOutCollisionPktsHigh_cookie;
extern LeafInfo pbEthRcntOutCollisionPktsLow_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbTpType_cookie;
extern LeafInfo pbTpIndex_cookie;
extern LeafInfo pbRecentTableType_cookie;
extern LeafInfo pbRecentTimesIndex_cookie;
extern LeafInfo pbRecentEsPmCounter_cookie;
extern LeafInfo pbRecentBbePmCounter_cookie;
extern LeafInfo pbRecentSesPmCounter_cookie;
extern LeafInfo pbRecentUasPmCounter_cookie;
extern LeafInfo pbRecentCsesPmCounter_cookie;
extern LeafInfo pbRecentFcPmCounter_cookie;
extern LeafInfo pbRecentOfsPmCounter_cookie;
extern LeafInfo pbRecentPpjcPmCounter_cookie;
extern LeafInfo pbRecentNpjcPmCounter_cookie;
extern LeafInfo pbRecentPmTimeEnd_cookie;
extern LeafInfo pbSuspect_cookie;
extern LeafInfo pbComplete_cookie;
extern LeafInfo pbSdhUapPMTable_cookie;
extern LeafInfo pbPdhUapPMTable_cookie;
extern LeafInfo pbOtuUapPMTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbUapChannelID_cookie;
extern LeafInfo pbUapSegmentNum_cookie;
extern LeafInfo pbOtuUapType_cookie;
extern LeafInfo pbOtuIsAtUap_cookie;
extern LeafInfo pbOtuUAPCounter_cookie;
extern LeafInfo pbOtuUapTimeStart_cookie;
extern LeafInfo pbOtuUapTimeElapse_cookie;
extern LeafInfo pbOtuUAPReset_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbPdhUapType_cookie;
extern LeafInfo pbPdhIsAtUap_cookie;
extern LeafInfo pbPdhUAPCounter_cookie;
extern LeafInfo pbPdhUapTimeStart_cookie;
extern LeafInfo pbPdhUapTimeElapse_cookie;
extern LeafInfo pbPdhUAPReset_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbTpType_cookie;
extern LeafInfo pbTpIndex_cookie;
extern LeafInfo pbPmUapType_cookie;
extern LeafInfo pbIsAtUap_cookie;
extern LeafInfo pbUAPCounter_cookie;
extern LeafInfo pbUapTimeStart_cookie;
extern LeafInfo pbUapTimeElapse_cookie;
extern LeafInfo pbUAPReset_cookie;
extern LeafInfo pbSdhRecentUapPMTable_cookie;
extern LeafInfo pbPdhRecentUapPMTable_cookie;
extern LeafInfo pbOtuRecentUapPMTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbOtuRecUapChannelID_cookie;
extern LeafInfo pbOtuRecUapSegmentNum_cookie;
extern LeafInfo pbOtuRecUapType_cookie;
extern LeafInfo pbOtuUapTimesIndex_cookie;
extern LeafInfo pbOtuRecentUapTimeStart_cookie;
extern LeafInfo pbOtuRecentUapTimeEnd_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbPdhRecUapType_cookie;
extern LeafInfo pbPdhUapTimesIndex_cookie;
extern LeafInfo pbPdhRecentUapTimeStart_cookie;
extern LeafInfo pbPdhRecentUapTimeEnd_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbTpType_cookie;
extern LeafInfo pbTpIndex_cookie;
extern LeafInfo pbPmRecUapType_cookie;
extern LeafInfo pbUapTimesIndex_cookie;
extern LeafInfo pbRecentUapTimeStart_cookie;
extern LeafInfo pbRecentUapTimeEnd_cookie;
extern LeafInfo pbSdhMostRecentIndexTable_cookie;
extern LeafInfo pbEthMostRecentIndexTable_cookie;
extern LeafInfo pbRprClientMostRecentIndexTable_cookie;
extern LeafInfo pbRprSpanMostRecentIndexTable_cookie;
extern LeafInfo pbSlipMostRecentIndexTable_cookie;
extern LeafInfo pbxSecEthIndexTable_cookie;
extern LeafInfo pbPdhMostRecentIndexTable_cookie;
extern LeafInfo pbEthVlanMostRecentIndexTable_cookie;
extern LeafInfo pbxSecEthVlanIndexTable_cookie;
extern LeafInfo pbFecMostRecentIndexTable_cookie;
extern LeafInfo pbOtuMostRecentIndexTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbOtuRecChannelID_cookie;
extern LeafInfo pbOtuRecSegmentNum_cookie;
extern LeafInfo pbOtuRecIntervalTblType_cookie;
extern LeafInfo pbOtuRecentIntervalTblMRI_cookie;
extern LeafInfo pbOtuRecentIntervalUpMaxIndex_cookie;
extern LeafInfo pbOtuRecentUapTblMRI_cookie;
extern LeafInfo pbOtuRecentUapUpMaxIndex_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbFecRecIntervalTblType_cookie;
extern LeafInfo pbFecRecentIntervalTblMRI_cookie;
extern LeafInfo pbFecRecentIntervalUpMaxIndex_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthPmPortType_cookie;
extern LeafInfo pbEthVlanNo_cookie;
extern LeafInfo pbxSecEthVlanRecentIntervalTblMRI_cookie;
extern LeafInfo pbxSecEthVlanRecentIntervalUpMaxIndex_cookie;
extern LeafInfo pbxSecEthVlanRecentIndex_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthVlanIndexTableType_cookie;
extern LeafInfo pbEthPmPortType_cookie;
extern LeafInfo pbEthVlanNo_cookie;
extern LeafInfo pbEthVlanRecentIntervalTblMRI_cookie;
extern LeafInfo pbEthVlanRecentIntervalUpMaxIndex_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbPdhRecIntervalTblType_cookie;
extern LeafInfo pbPdhRecentIntervalTblMRI_cookie;
extern LeafInfo pbPdhRecentIntervalUpMaxIndex_cookie;
extern LeafInfo pbPdhRecentUapTblMRI_cookie;
extern LeafInfo pbPdhRecentUapUpMaxIndex_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbxSecEthIndexPortType_cookie;
extern LeafInfo pbxSecEthRecentIntervalTblMRI_cookie;
extern LeafInfo pbxSecEthRecentIntervalUpMaxIndex_cookie;
extern LeafInfo pbxSecEthRecentIndex_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbSlipRecentTblMRI_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo rprSpanId_cookie;
extern LeafInfo pbRprSpanIndexTableType_cookie;
extern LeafInfo pbRprSpanRecentIntervalTblMRI_cookie;
extern LeafInfo pbRprSpanRecentIntervalUpMaxIndex_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo rprIfIndex_cookie;
extern LeafInfo pbRprClientIndexTableType_cookie;
extern LeafInfo pbRprClientRecentIntervalTblMRI_cookie;
extern LeafInfo pbRprClientRecentIntervalUpMaxIndex_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbEthIndexTableType_cookie;
extern LeafInfo pbEthIndexPmPortType_cookie;
extern LeafInfo pbEthRecentIntervalTblMRI_cookie;
extern LeafInfo pbEthRecentIntervalUpMaxIndex_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbTpType_cookie;
extern LeafInfo pbTpIndex_cookie;
extern LeafInfo pbPmRecIntervalTblType_cookie;
extern LeafInfo pbRecentIntervalTblMRI_cookie;
extern LeafInfo pbRecentIntervalUpMaxIndex_cookie;
extern LeafInfo pbRecentUapTblMRI_cookie;
extern LeafInfo pbRecentUapUpMaxIndex_cookie;
extern LeafInfo pbSdhPMUpLoadTable_cookie;
extern LeafInfo pbxSecEthUploadTable_cookie;
extern LeafInfo pbxSecEthFtpHostIP_cookie;
extern LeafInfo pbxSecEthFtpFileName_cookie;
extern LeafInfo pbxSecEthFtpUsername_cookie;
extern LeafInfo pbxSecEthFtpPassword_cookie;
extern LeafInfo pbSdhFtpCmndHostIP_cookie;
extern LeafInfo pbSdhFtpCmndPath_cookie;
extern LeafInfo pbSdhFtpFileName_cookie;
extern LeafInfo pbSdhFtpCmndUsername_cookie;
extern LeafInfo pbSdhFtpCmndPassword_cookie;
extern LeafInfo pbSdhFtpCmndLastState_cookie;
extern LeafInfo pbAdjustTimeDircetion_cookie;
extern LeafInfo pbAdjustTimeDays_cookie;
extern LeafInfo pbAdjustTimeHours_cookie;
extern LeafInfo pbAdjustTimeMins_cookie;
extern LeafInfo pbAdjustTimeSecs_cookie;
extern LeafInfo pbUpLoadStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbOTUPortLabel_cookie;
extern LeafInfo pbOTUPortMode_cookie;
extern LeafInfo pbOTUPortAlarmDisable_cookie;
extern LeafInfo pbOTUPortLoopback_cookie;
extern LeafInfo pbFecEnable_cookie;
extern LeafInfo pbInsFecErrEnable_cookie;
extern LeafInfo pbInsFecErrRate_cookie;
extern LeafInfo pbWaveLengthId_cookie;
extern LeafInfo pbSMTimAlmEnable_cookie;
extern LeafInfo pbSMTimActEnable_cookie;
extern LeafInfo pbSMTxTti_cookie;
extern LeafInfo pbSMExpTti_cookie;
extern LeafInfo pbPTAlmEnable_cookie;
extern LeafInfo pbTxPt_cookie;
extern LeafInfo pbExpPt_cookie;
extern LeafInfo pbCIDAlmEnable_cookie;
extern LeafInfo pbTxChannelId_cookie;
extern LeafInfo pbAutoAdjustManulStopEnable_cookie;
extern LeafInfo pbPortPathMapping_cookie;
extern LeafInfo pbOTUFecMode_cookie;
extern LeafInfo pbOTUPortAdminStatus_cookie;
extern LeafInfo pbOTUPortUsageStatus_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbTpType_cookie;
extern LeafInfo pbTpIndex_cookie;
extern LeafInfo pbMonitored_cookie;
extern LeafInfo pbOutSignalType_cookie;
extern LeafInfo pbInSignalType_cookie;
extern LeafInfo pbTransPsl_cookie;
extern LeafInfo pbExpPsl_cookie;
extern LeafInfo pbSLEnable_cookie;
extern LeafInfo pbSLAccept_cookie;
extern LeafInfo pbTtiEnabled_cookie;
extern LeafInfo pbTimAisDis_cookie;
extern LeafInfo pbTtiTransmitMode_cookie;
extern LeafInfo pbTtiReceiveMode_cookie;
extern LeafInfo pbTtiOctetTransmitted_cookie;
extern LeafInfo pbTtiOctetExpected_cookie;
extern LeafInfo pbTtiOctetAccepted_cookie;
extern LeafInfo pbSdMode_cookie;
extern LeafInfo pbBurstyEnable_cookie;
extern LeafInfo pbBurstyPeriod_cookie;
extern LeafInfo pbBurstyPercent_cookie;
extern LeafInfo pbDegEnable_cookie;
extern LeafInfo pbDegThreshold_cookie;
extern LeafInfo pbExcEnable_cookie;
extern LeafInfo pbExcThreshold_cookie;
extern LeafInfo pbAISEnable_cookie;
extern LeafInfo pbRDIEnable_cookie;
extern LeafInfo pmNearEnd15minMonitored_cookie;
extern LeafInfo pmFarEnd15minMonitored_cookie;
extern LeafInfo pmNearEnd24hrMonitored_cookie;
extern LeafInfo pmFarEnd24hrMonitored_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo vc4ReservationStatus_cookie;
extern LeafInfo vc4ReservationOperate_cookie;
extern LeafInfo tpOperationLock_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbPortStatus_cookie;
extern LeafInfo pbPortOperStatus_cookie;
extern LeafInfo pbLaserIBias_cookie;
extern LeafInfo pbLaserRecvPwr_cookie;
extern LeafInfo pbLaserTransPwr_cookie;
extern LeafInfo pbLaserTemp_cookie;
extern LeafInfo pbCompliance8472_cookie;
extern LeafInfo pbnNominalMBps_cookie;
extern LeafInfo pbnWavelength_cookie;
extern LeafInfo pbnLinkLength9125_cookie;
extern LeafInfo pbnLinkLength50125_cookie;
extern LeafInfo pbnLinkLength62125_cookie;
extern LeafInfo pbnLinkLengthCopper_cookie;
extern LeafInfo pbsVendorName_cookie;
extern LeafInfo pbsVendorPN_cookie;
extern LeafInfo pbsVendorSN_cookie;
extern LeafInfo pbsManuDate_cookie;
extern LeafInfo pbnTransCode_cookie;
extern LeafInfo pbsVendorOUI_cookie;
extern LeafInfo pbnConnectorType_cookie;
extern LeafInfo pbnClLaserTemp_cookie;
extern LeafInfo pbnTECCurrent_cookie;
extern LeafInfo pbsSAGPartNumber_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbPortType_cookie;
extern LeafInfo pbPortNo_cookie;
extern LeafInfo pbPortLabel_cookie;
extern LeafInfo pbPortMode_cookie;
extern LeafInfo pbPdhPortMap_cookie;
extern LeafInfo pbPortAdminStatus_cookie;
extern LeafInfo pbPortMapRefresh_cookie;
extern LeafInfo pbPortAutoLaserShutdown_cookie;
extern LeafInfo pbPortLoopback_cookie;
extern LeafInfo pbFecMode_cookie;
extern LeafInfo pbActualPortType_cookie;
extern LeafInfo pbImpedance_cookie;
extern LeafInfo pbCodeType_cookie;
extern LeafInfo pbHaulType_cookie;
extern LeafInfo pbRetimingStatus_cookie;
extern LeafInfo pbPortAlsMode_cookie;
extern LeafInfo pbPortAlarmDisable_cookie;
extern LeafInfo expectedFrameStructure_cookie;
extern LeafInfo crcMultiframeDetector_cookie;
extern LeafInfo portClassification_cookie;
extern LeafInfo portUsageStatus_cookie;
extern LeafInfo portMappingString_cookie;
extern LeafInfo portOperationLock_cookie;
extern LeafInfo pbActiveAlarmTable_cookie;
extern LeafInfo pbAlarmTypeCfgTable_cookie;
extern LeafInfo pbCurrentEventsTable_cookie;
extern LeafInfo pbEventTypeCfgTable_cookie;
extern LeafInfo pbLogSizeGroup_cookie;
extern LeafInfo alarmPersistentTime_cookie;
extern LeafInfo logPolicyGroup_cookie;
extern LeafInfo pbAlarmStatGroup_cookie;
extern LeafInfo pbLogClearGroup_cookie;
extern LeafInfo pbClearAlarmLog_cookie;
extern LeafInfo pbClearEventsLog_cookie;
extern LeafInfo pbNumberOfCriticalAlarms_cookie;
extern LeafInfo pbNumberOfMajorAlarms_cookie;
extern LeafInfo pbNumberOfMinorAlarms_cookie;
extern LeafInfo pbNumberOfWarningAlarms_cookie;
extern LeafInfo pbNumberOfIndeterminateAlarms_cookie;
extern LeafInfo pbNumberOfEvents_cookie;
extern LeafInfo eventLogPolicy_cookie;
extern LeafInfo alarmDetectionPersistentTime_cookie;
extern LeafInfo alarmClearingPersistentTime_cookie;
extern LeafInfo pbAlarmLogSize_cookie;
extern LeafInfo pbEventLogSize_cookie;
extern LeafInfo pbEventTypeID_cookie;
extern LeafInfo pbEventTypeCategory_cookie;
extern LeafInfo pbEventTypeName_cookie;
extern LeafInfo pbEventTypeSendTrap_cookie;
extern LeafInfo pbCurrentEventIndex_cookie;
extern LeafInfo pbCurrentEventID_cookie;
extern LeafInfo pbCurrentEventDTS_cookie;
extern LeafInfo pbCurrentEventName_cookie;
extern LeafInfo pbCurrentEventCategory_cookie;
extern LeafInfo pbCurrentEventDetails_cookie;
extern LeafInfo pbAlarmTypeID_cookie;
extern LeafInfo pbAlarmTypeCode_cookie;
extern LeafInfo pbAlarmTypeDesc_cookie;
extern LeafInfo pbAlarmTypeCategory_cookie;
extern LeafInfo pbAlarmTypeSeverity_cookie;
extern LeafInfo pbAlarmTypeInhibit_cookie;
extern LeafInfo pbAlarmTypeMDO1_cookie;
extern LeafInfo pbAlarmTypeMDO2_cookie;
extern LeafInfo pbAlarmTypeTrapID_cookie;
extern LeafInfo pbAlarmTypeMDO3_cookie;
extern LeafInfo pbAlarmTypeMDO4_cookie;
extern LeafInfo pbAlmClassID_cookie;
extern LeafInfo pbAlarmTypeMDO5_cookie;
extern LeafInfo pbAlarmTypeMDO6_cookie;
extern LeafInfo pbAlarmTypeMDO7_cookie;
extern LeafInfo pbAlarmTypeMDO8_cookie;
extern LeafInfo pbActiveAlarmIndex_cookie;
extern LeafInfo pbActiveAlarmCode_cookie;
extern LeafInfo pbActiveAlarmSeverity_cookie;
extern LeafInfo pbActiveAlarmSource_cookie;
extern LeafInfo pbActiveAlarmDTS_cookie;
extern LeafInfo pbActiveAlarmClearDTS_cookie;
extern LeafInfo pbActiveAlarmSlotNum_cookie;
extern LeafInfo pbActiveAlarmPortNum_cookie;
extern LeafInfo pbActiveAlarmPortType_cookie;
extern LeafInfo pbActiveAlarmDesc_cookie;
extern LeafInfo pbActiveAlarmTypeID_cookie;
extern LeafInfo pbActiveAlarmCategory_cookie;
extern LeafInfo pbActiveAlarmPathType_cookie;
extern LeafInfo pbActiveAlarmSeqNum_cookie;
extern LeafInfo pbActiveAlarmAcknowledge_cookie;
extern LeafInfo pbActiveAlarmState_cookie;
extern LeafInfo pbActiveAlarmBoardStyle_cookie;
extern LeafInfo pbActiveAlarmActualPortType_cookie;
extern LeafInfo addressGroup_cookie;
extern LeafInfo systemGroup_cookie;
extern LeafInfo ledGroup_cookie;
extern LeafInfo pbSlotTable_cookie;
extern LeafInfo pbMibSaveRestoreGroup_cookie;
extern LeafInfo pbInterfaceGroup_cookie;
extern LeafInfo pbUpgradeGroup_cookie;
extern LeafInfo pbConfigCmndGroup_cookie;
extern LeafInfo pbStatsGroup_cookie;
extern LeafInfo alarmGroup_cookie;
extern LeafInfo pbNodeGroup_cookie;
extern LeafInfo nodeControlGroup_cookie;
extern LeafInfo ntpGroup_cookie;
extern LeafInfo pbDateTimeStampGroup_cookie;
extern LeafInfo pbCFCardGroup_cookie;
extern LeafInfo flashSpaceGroup_cookie;
extern LeafInfo pbExtensionShelfTable_cookie;
extern LeafInfo nodePropertyGroup_cookie;
extern LeafInfo eowFunctionGroup_cookie;
extern LeafInfo gmplsLicenseGroup_cookie;
extern LeafInfo e2romConfigGroup_cookie;
extern LeafInfo systemMonitorGroup_cookie;
extern LeafInfo cpuUtilEventGroup_cookie;
extern LeafInfo memUtilEventGroup_cookie;
extern LeafInfo cfCardUtilEventGroup_cookie;
extern LeafInfo flashUtilEventGroup_cookie;
extern LeafInfo flashUtilThreshold_cookie;
extern LeafInfo flashUtilEventEnable_cookie;
extern LeafInfo cfCardUtilThreshold_cookie;
extern LeafInfo cfCardUtilEventEnable_cookie;
extern LeafInfo memUtilThreshold_cookie;
extern LeafInfo memUtilEventEnable_cookie;
extern LeafInfo cpuUtilThreshold_cookie;
extern LeafInfo cpuUtilEventEnable_cookie;
extern LeafInfo pbE2RomConfigTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbE2RomPageNo_cookie;
extern LeafInfo pbE2RomTlvData_cookie;
extern LeafInfo gmplsLicenseStatus_cookie;
extern LeafInfo gmplsLicenseSyncFlag_cookie;
extern LeafInfo eowOhccTable_cookie;
extern LeafInfo siGlobalTable_cookie;
extern LeafInfo siChannelManageTable_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo ifIdx_cookie;
extern LeafInfo siChannelID_cookie;
extern LeafInfo siChannelGroup_cookie;
extern LeafInfo siChannelName_cookie;
extern LeafInfo siChannelEnable_cookie;
extern LeafInfo siChannelNoiseDetect_cookie;
extern LeafInfo siChannelNoiseState_cookie;
extern LeafInfo siChannelRingManage_cookie;
extern LeafInfo siChannelRingManageState_cookie;
extern LeafInfo siChannelRingManageDelta_cookie;
extern LeafInfo siChannelRingManageSwitch_cookie;
extern LeafInfo siChannelGroupSetting_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo ifIdx_cookie;
extern LeafInfo siName_cookie;
extern LeafInfo siPhoneNum_cookie;
extern LeafInfo siClipper_cookie;
extern LeafInfo siEnsThreshold_cookie;
extern LeafInfo siErmThreshold_cookie;
extern LeafInfo siErmAtt_cookie;
extern LeafInfo siErmStep_cookie;
extern LeafInfo siWorkingMode_cookie;
extern LeafInfo siUpdateDsp_cookie;
extern LeafInfo siUpdateDspState_cookie;
extern LeafInfo eowOhccDestination_cookie;
extern LeafInfo eowOhccSource_cookie;
extern LeafInfo eowOhccRingManaged_cookie;
extern LeafInfo nodeHoccCapability_cookie;
extern LeafInfo nodeLoccCapability_cookie;
extern LeafInfo pbExtensionShelfID_cookie;
extern LeafInfo pbMainShelfIP_cookie;
extern LeafInfo pbMainShelfReadString_cookie;
extern LeafInfo pbMainShelfWriteString_cookie;
extern LeafInfo pbMainShelfSlot_cookie;
extern LeafInfo pbMainShelfPort_cookie;
extern LeafInfo pbExtesionShelfIP_cookie;
extern LeafInfo pbExtesionShelfReadString_cookie;
extern LeafInfo pbExtesionShelfWriteString_cookie;
extern LeafInfo pbExtesionShelfSlot_cookie;
extern LeafInfo pbExtesionShelfPort_cookie;
extern LeafInfo pbExtesionShelfLabel_cookie;
extern LeafInfo pbExtesionShelfDesc_cookie;
extern LeafInfo pbExtesionShelfRowStatus_cookie;
extern LeafInfo boardFlashFreeSpace_cookie;
extern LeafInfo cfCardFreeSpace_cookie;
extern LeafInfo pbCFCardStatus_cookie;
extern LeafInfo pbCFCardMIBValidity_cookie;
extern LeafInfo pbStandbyCFCardStatus_cookie;
extern LeafInfo pbDateTimeStampSet_cookie;
extern LeafInfo pbDateTimeStampGet_cookie;
extern LeafInfo pbTimeZone_cookie;
extern LeafInfo pbNTPEnabled_cookie;
extern LeafInfo pbNTPIpAddress_cookie;
extern LeafInfo pbNTPRequestPeriods_cookie;
extern LeafInfo pbWarmRebootNode_cookie;
extern LeafInfo pbColdRebootNode_cookie;
extern LeafInfo pbResetNode_cookie;
extern LeafInfo pbNodeShutDown_cookie;
extern LeafInfo pbNodePowerOff_cookie;
extern LeafInfo pbNodeType_cookie;
extern LeafInfo pbNodeSoftwareVersion_cookie;
extern LeafInfo pbNodeIntSoftwareVersion_cookie;
extern LeafInfo pbDatabaseVersion_cookie;
extern LeafInfo pbNodeAlarmEnabling_cookie;
extern LeafInfo pbAISEnabled_cookie;
extern LeafInfo pbRDIEnabled_cookie;
extern LeafInfo pbAlarmSuppress_cookie;
extern LeafInfo pbAlarmCutOff_cookie;
extern LeafInfo pbACOGroup_cookie;
extern LeafInfo pbIoaTable_cookie;
extern LeafInfo pbDataExcDegTpNum_cookie;
extern LeafInfo alarmRuleSwitchGroup_cookie;
extern LeafInfo pbSEFEnabled_cookie;
extern LeafInfo lofMonitoringSwitch_cookie;
extern LeafInfo lomMonitoringSwitch_cookie;
extern LeafInfo lopMonitoringSwitch_cookie;
extern LeafInfo pbIoaIndexNo_cookie;
extern LeafInfo pbIoaLabel_cookie;
extern LeafInfo pbIoaEnableStatus_cookie;
extern LeafInfo pbIoaStatus_cookie;
extern LeafInfo pbAudibleACO_cookie;
extern LeafInfo pbVisualACO_cookie;
extern LeafInfo pbExtAudible_cookie;
extern LeafInfo pbExtVisual_cookie;
extern LeafInfo pbNumberOfReboots_cookie;
extern LeafInfo pbNodeDownDTS_cookie;
extern LeafInfo nodeMibCmnd_cookie;
extern LeafInfo nodeMibCmndState_cookie;
extern LeafInfo nodeMibCmndCancel_cookie;
extern LeafInfo nodeMibNmsOperateArea_cookie;
extern LeafInfo nodeMibUpDownLoadStatus_cookie;
extern LeafInfo nodeMibLastCommand_cookie;
extern LeafInfo nodeMibFailedIndex_cookie;
extern LeafInfo pbBackupMIBState_cookie;
extern LeafInfo pbMIBOperationDateTimeStamp_cookie;
extern LeafInfo nodeMIBFilesNoCheck_cookie;
extern LeafInfo pbUpgradeCurrRelease_cookie;
extern LeafInfo pbUpgradeBackupRelease_cookie;
extern LeafInfo pbUpgradeHostIP_cookie;
extern LeafInfo pbUpgradeHostUsername_cookie;
extern LeafInfo pbUpgradeHostPassword_cookie;
extern LeafInfo pbUpgradeHostFileName_cookie;
extern LeafInfo pbUpgradeCommand_cookie;
extern LeafInfo pbUpgradeCmndState_cookie;
extern LeafInfo pbUpgradeStateDescr_cookie;
extern LeafInfo pbUpgradeLoadCancel_cookie;
extern LeafInfo nodeUpgradeLastCommand_cookie;
extern LeafInfo nodeUpgradeFailedIndex_cookie;
extern LeafInfo nodeUpgradeSwitchPhySlot_cookie;
extern LeafInfo upgradeSoftwareInfoTable_cookie;
extern LeafInfo upgradeSoftwareSlotNumber_cookie;
extern LeafInfo upgradeSoftwareIndication_cookie;
extern LeafInfo upgradeSoftwareRole_cookie;
extern LeafInfo upgradeSoftwareName_cookie;
extern LeafInfo upgradeSoftwareDatetime_cookie;
extern LeafInfo pbPhysicalAdjacencyTable_cookie;
extern LeafInfo pbAutoLinkStarted_cookie;
extern LeafInfo pbAutoLinkTable_cookie;
extern LeafInfo pbLocalSlot_cookie;
extern LeafInfo pbLocalPort_cookie;
extern LeafInfo pbRemoteIPAddress_cookie;
extern LeafInfo pbRemoteSlot_cookie;
extern LeafInfo pbRemotePort_cookie;
extern LeafInfo pbLocalSlotNumber_cookie;
extern LeafInfo pbLocalPortNumber_cookie;
extern LeafInfo pbRemoteNodeMolnAdjacencyStatus_cookie;
extern LeafInfo pbRemoteNodeIPAddress_cookie;
extern LeafInfo pbRemoteSlotNumber_cookie;
extern LeafInfo pbRemotePortNumber_cookie;
extern LeafInfo pbLinkOperationalStatus_cookie;
extern LeafInfo pbInterfaceDescr_cookie;
extern LeafInfo pbLinkGeneralType_cookie;
extern LeafInfo pbLinkRowStatus_cookie;
extern LeafInfo pbAutoSaver_cookie;
extern LeafInfo pbAutoSaverFirstTimer_cookie;
extern LeafInfo pbAutoSaverLastTimer_cookie;
extern LeafInfo pbConfigType_cookie;
extern LeafInfo pbSaveMib_cookie;
extern LeafInfo pbClearMibSet_cookie;
extern LeafInfo pbSlotNumber_cookie;
extern LeafInfo pbSlotType_cookie;
extern LeafInfo pbBoardType_cookie;
extern LeafInfo pbBoardFunction_cookie;
extern LeafInfo pbCardState_cookie;
extern LeafInfo pbCardDesiredState_cookie;
extern LeafInfo pbSlotNumberDisplay_cookie;
extern LeafInfo pbSlotReset_cookie;
extern LeafInfo pbSlotNoFailuresTotal_cookie;
extern LeafInfo pbSlotResetNoFailures_cookie;
extern LeafInfo pbSlotResetDuration_cookie;
extern LeafInfo pbSlotName_cookie;
extern LeafInfo pbSlotDescr_cookie;
extern LeafInfo pbSlotIfCardDumb_cookie;
extern LeafInfo pbSlotCrashDumpAvail_cookie;
extern LeafInfo pbSlotLastBoardType_cookie;
extern LeafInfo pbSlotBoardTypeDifference_cookie;
extern LeafInfo pbSlotExplicitDeletion_cookie;
extern LeafInfo pbProvisionedCardType_cookie;
extern LeafInfo pbSlotCardSpecificInfo_cookie;
extern LeafInfo pbSlotTemperatureThreshold_cookie;
extern LeafInfo pbSlotTemperatureHysteresis_cookie;
extern LeafInfo pbSlotTemperatureCleanThreshold_cookie;
extern LeafInfo pbSlotLastBoardFunction_cookie;
extern LeafInfo pbDiagMode_cookie;
extern LeafInfo pbDiagActive_cookie;
extern LeafInfo pbLogicalSlotNumber_cookie;
extern LeafInfo pbSlotDisable_cookie;
extern LeafInfo pbForceResetSlot_cookie;
extern LeafInfo pbSlotFailureLedColor_cookie;
extern LeafInfo pbSlotActiveLedColor_cookie;
extern LeafInfo pbBoardSerialNumber_cookie;
extern LeafInfo pbBoardOption_cookie;
extern LeafInfo pbCode_cookie;
extern LeafInfo pbLatchState_cookie;
extern LeafInfo pbSlotAlarmDisable_cookie;
extern LeafInfo provisionedCardFunction_cookie;
extern LeafInfo slotConfigOption_cookie;
extern LeafInfo cardSoftwareVersion_cookie;
extern LeafInfo cardBootVersion_cookie;
extern LeafInfo cardBootDatetime_cookie;
extern LeafInfo cardHardwareVersion_cookie;
extern LeafInfo cardFusionVersion_cookie;
extern LeafInfo cardFpgaVersion_cookie;
extern LeafInfo macAddress_cookie;
extern LeafInfo slotMaxDCCNumber_cookie;
extern LeafInfo slotMapValue_cookie;
extern LeafInfo pbLedTest_cookie;
extern LeafInfo pbLedTable_cookie;
extern LeafInfo pbLedSlotNo_cookie;
extern LeafInfo pbLedIndexNo_cookie;
extern LeafInfo pbLedLabel_cookie;
extern LeafInfo pbLedColor_cookie;
extern LeafInfo pbMode_cookie;
extern LeafInfo pbLabel_cookie;
extern LeafInfo pbLocation_cookie;
extern LeafInfo pbDescription_cookie;
extern LeafInfo pbContact_cookie;
extern LeafInfo pbSerialNumber_cookie;
extern LeafInfo pbRelease_cookie;
extern LeafInfo pbBootImageVersion_cookie;
extern LeafInfo pbBackplaneHwVersion_cookie;
extern LeafInfo pbBackplaneCode_cookie;
extern LeafInfo pbIpAddress_cookie;
extern LeafInfo pbIpMask_cookie;
extern LeafInfo pbGatewayAddress_cookie;
extern LeafInfo pbEthernetAddress_cookie;
extern LeafInfo pbEthernetMask_cookie;
extern LeafInfo pbSystemMacAddress_cookie;
extern LeafInfo pbSlaveEthernetAddress_cookie;
extern LeafInfo pbEthernetAddressSecondary_cookie;
extern LeafInfo pbEthernetMaskSecondary_cookie;
extern LeafInfo pbEthernetEnable_cookie;
extern LeafInfo pbGMPLSIPAddress_cookie;
extern LeafInfo pbGMPLSIPMask_cookie;
extern LeafInfo pbMgmtPort1Status_cookie;
extern LeafInfo pbMgmtPort2Status_cookie;
extern LeafInfo enterpriseModules_cookie;
extern LeafInfo enterpriseNetworkingProducts_cookie;
extern LeafInfo product1Reg_cookie;
extern LeafInfo enterpriseGlobalRegModule_cookie;
extern LeafInfo productSeries1MibModule_cookie;
extern LeafInfo pbSecurityModule_cookie;
extern LeafInfo pbEventsModule_cookie;
extern LeafInfo pbSonetModule_cookie;
extern LeafInfo enterpriseGlobalTCModule_cookie;
extern LeafInfo enterpriseOAModule_cookie;
extern LeafInfo pbSDHProtectionModule_cookie;
extern LeafInfo enterpriseRstpMIB_cookie;
extern LeafInfo pbEthernetModule_cookie;
extern LeafInfo pbTMSModule_cookie;
extern LeafInfo pbXCContextModule_cookie;
extern LeafInfo pbXCModule_cookie;
extern LeafInfo pbSonetSDHPMModule_cookie;
extern LeafInfo pbNodeModule_cookie;
extern LeafInfo pbRprModule_cookie;
extern LeafInfo pbDccModule_cookie;
extern LeafInfo pbGvrpModule_cookie;
extern LeafInfo pbOsiModule_cookie;
extern LeafInfo pbIpTunnelModule_cookie;
extern LeafInfo enterpriseMstpMIB_cookie;
extern LeafInfo enterpriseEOSModule_cookie;
extern LeafInfo pbGmrpModule_cookie;
extern LeafInfo pbPrivateManagement_cookie;
extern LeafInfo mib_2_cookie;
extern LeafInfo system_cookie;
extern LeafInfo at_cookie;
extern LeafInfo interfaces_cookie;
extern LeafInfo ip_cookie;
extern LeafInfo icmp_cookie;
extern LeafInfo tcp_cookie;
extern LeafInfo udp_cookie;
extern LeafInfo egp_cookie;
extern LeafInfo transmission_cookie;
extern LeafInfo snmp_cookie;
extern LeafInfo ianaifType_cookie;
extern LeafInfo ifMIB_cookie;
extern LeafInfo agentxMIB_cookie;
extern LeafInfo agentxObjects_cookie;
extern LeafInfo agentxConformance_cookie;
extern LeafInfo agentxMIBGroups_cookie;
extern LeafInfo agentxMIBCompliances_cookie;
extern LeafInfo agentxGeneral_cookie;
extern LeafInfo agentxConnection_cookie;
extern LeafInfo agentxSession_cookie;
extern LeafInfo agentxRegistration_cookie;
extern LeafInfo agentxRegistrationTableLastChange_cookie;
extern LeafInfo agentxRegistrationTable_cookie;
extern LeafInfo agentxConnIndex_cookie;
extern LeafInfo agentxSessionIndex_cookie;
extern LeafInfo agentxRegIndex_cookie;
extern LeafInfo agentxRegContext_cookie;
extern LeafInfo agentxRegStart_cookie;
extern LeafInfo agentxRegRangeSubId_cookie;
extern LeafInfo agentxRegUpperBound_cookie;
extern LeafInfo agentxRegPriority_cookie;
extern LeafInfo agentxRegTimeout_cookie;
extern LeafInfo agentxRegInstance_cookie;
extern LeafInfo agentxSessionTableLastChange_cookie;
extern LeafInfo agentxSessionTable_cookie;
extern LeafInfo agentxConnIndex_cookie;
extern LeafInfo agentxSessionIndex_cookie;
extern LeafInfo agentxSessionObjectID_cookie;
extern LeafInfo agentxSessionDescr_cookie;
extern LeafInfo agentxSessionAdminStatus_cookie;
extern LeafInfo agentxSessionOpenTime_cookie;
extern LeafInfo agentxSessionAgentXVer_cookie;
extern LeafInfo agentxSessionTimeout_cookie;
extern LeafInfo agentxConnTableLastChange_cookie;
extern LeafInfo agentxConnectionTable_cookie;
extern LeafInfo agentxConnIndex_cookie;
extern LeafInfo agentxConnOpenTime_cookie;
extern LeafInfo agentxConnTransportDomain_cookie;
extern LeafInfo agentxConnTransportAddress_cookie;
extern LeafInfo agentxDefaultTimeout_cookie;
extern LeafInfo agentxMasterAgentXVer_cookie;
extern LeafInfo ifMIBObjects_cookie;
extern LeafInfo ifConformance_cookie;
extern LeafInfo ifGroups_cookie;
extern LeafInfo ifCompliances_cookie;
extern LeafInfo ifXTable_cookie;
extern LeafInfo ifTableLastChange_cookie;
extern LeafInfo ifTestTable_cookie;
extern LeafInfo ifRcvAddressTable_cookie;
extern LeafInfo ifStackTable_cookie;
extern LeafInfo ifStackLastChange_cookie;
extern LeafInfo ifStackHigherLayer_cookie;
extern LeafInfo ifStackLowerLayer_cookie;
extern LeafInfo ifStackStatus_cookie;
extern LeafInfo ifIndex_cookie;
extern LeafInfo ifRcvAddressAddress_cookie;
extern LeafInfo ifRcvAddressStatus_cookie;
extern LeafInfo ifRcvAddressType_cookie;
extern LeafInfo ifIndex_cookie;
extern LeafInfo ifTestId_cookie;
extern LeafInfo ifTestStatus_cookie;
extern LeafInfo ifTestType_cookie;
extern LeafInfo ifTestResult_cookie;
extern LeafInfo ifTestCode_cookie;
extern LeafInfo ifTestOwner_cookie;
extern LeafInfo ifIndex_cookie;
extern LeafInfo ifName_cookie;
extern LeafInfo ifInMulticastPkts_cookie;
extern LeafInfo ifInBroadcastPkts_cookie;
extern LeafInfo ifOutMulticastPkts_cookie;
extern LeafInfo ifOutBroadcastPkts_cookie;
extern LeafInfo ifHCInOctets_cookie;
extern LeafInfo ifHCInUcastPkts_cookie;
extern LeafInfo ifHCInMulticastPkts_cookie;
extern LeafInfo ifHCInBroadcastPkts_cookie;
extern LeafInfo ifHCOutOctets_cookie;
extern LeafInfo ifHCOutUcastPkts_cookie;
extern LeafInfo ifHCOutMulticastPkts_cookie;
extern LeafInfo ifHCOutBroadcastPkts_cookie;
extern LeafInfo ifLinkUpDownTrapEnable_cookie;
extern LeafInfo ifHighSpeed_cookie;
extern LeafInfo ifPromiscuousMode_cookie;
extern LeafInfo ifConnectorPresent_cookie;
extern LeafInfo ifAlias_cookie;
extern LeafInfo ifCounterDiscontinuityTime_cookie;
extern LeafInfo snmpInPkts_cookie;
extern LeafInfo snmpInBadVersions_cookie;
extern LeafInfo snmpOutPkts_cookie;
extern LeafInfo snmpInBadCommunityNames_cookie;
extern LeafInfo snmpInBadCommunityUses_cookie;
extern LeafInfo snmpInASNParseErrs_cookie;
extern LeafInfo snmpEnableAuthenTraps_cookie;
extern LeafInfo snmpOutTraps_cookie;
extern LeafInfo snmpOutGetResponses_cookie;
extern LeafInfo snmpOutSetRequests_cookie;
extern LeafInfo snmpOutGetNexts_cookie;
extern LeafInfo snmpOutGetRequests_cookie;
extern LeafInfo snmpOutGenErrs_cookie;
extern LeafInfo snmpOutBadValues_cookie;
extern LeafInfo snmpOutNoSuchNames_cookie;
extern LeafInfo snmpOutTooBigs_cookie;
extern LeafInfo snmpInTraps_cookie;
extern LeafInfo snmpInGetResponses_cookie;
extern LeafInfo snmpInSetRequests_cookie;
extern LeafInfo snmpInGetNexts_cookie;
extern LeafInfo snmpInGetRequests_cookie;
extern LeafInfo snmpInTotalSetVars_cookie;
extern LeafInfo snmpInTotalReqVars_cookie;
extern LeafInfo snmpInGenErrs_cookie;
extern LeafInfo snmpInReadOnlys_cookie;
extern LeafInfo snmpInBadValues_cookie;
extern LeafInfo snmpInNoSuchNames_cookie;
extern LeafInfo snmpInTooBigs_cookie;
extern LeafInfo snmpSilentDrops_cookie;
extern LeafInfo snmpProxyDrops_cookie;
extern LeafInfo egpInMsgs_cookie;
extern LeafInfo egpInErrors_cookie;
extern LeafInfo egpOutMsgs_cookie;
extern LeafInfo egpOutErrors_cookie;
extern LeafInfo egpNeighTable_cookie;
extern LeafInfo egpAs_cookie;
extern LeafInfo egpNeighAddr_cookie;
extern LeafInfo egpNeighState_cookie;
extern LeafInfo egpNeighAs_cookie;
extern LeafInfo egpNeighInMsgs_cookie;
extern LeafInfo egpNeighInErrs_cookie;
extern LeafInfo egpNeighOutMsgs_cookie;
extern LeafInfo egpNeighOutErrs_cookie;
extern LeafInfo egpNeighInErrMsgs_cookie;
extern LeafInfo egpNeighOutErrMsgs_cookie;
extern LeafInfo egpNeighStateUps_cookie;
extern LeafInfo egpNeighStateDowns_cookie;
extern LeafInfo egpNeighIntervalHello_cookie;
extern LeafInfo egpNeighIntervalPoll_cookie;
extern LeafInfo egpNeighMode_cookie;
extern LeafInfo egpNeighEventTrigger_cookie;
extern LeafInfo udpInDatagrams_cookie;
extern LeafInfo udpNoPorts_cookie;
extern LeafInfo udpInErrors_cookie;
extern LeafInfo udpOutDatagrams_cookie;
extern LeafInfo udpTable_cookie;
extern LeafInfo udpLocalAddress_cookie;
extern LeafInfo udpLocalPort_cookie;
extern LeafInfo tcpRtoAlgorithm_cookie;
extern LeafInfo tcpRtoMin_cookie;
extern LeafInfo tcpRtoMax_cookie;
extern LeafInfo tcpMaxConn_cookie;
extern LeafInfo tcpActiveOpens_cookie;
extern LeafInfo tcpPassiveOpens_cookie;
extern LeafInfo tcpAttemptFails_cookie;
extern LeafInfo tcpEstabResets_cookie;
extern LeafInfo tcpCurrEstab_cookie;
extern LeafInfo tcpInSegs_cookie;
extern LeafInfo tcpOutSegs_cookie;
extern LeafInfo tcpRetransSegs_cookie;
extern LeafInfo tcpConnTable_cookie;
extern LeafInfo tcpInErrs_cookie;
extern LeafInfo tcpOutRsts_cookie;
extern LeafInfo tcpConnLocalAddress_cookie;
extern LeafInfo tcpConnLocalPort_cookie;
extern LeafInfo tcpConnRemAddress_cookie;
extern LeafInfo tcpConnRemPort_cookie;
extern LeafInfo tcpConnState_cookie;
extern LeafInfo icmpInMsgs_cookie;
extern LeafInfo icmpInErrors_cookie;
extern LeafInfo icmpInDestUnreachs_cookie;
extern LeafInfo icmpInTimeExcds_cookie;
extern LeafInfo icmpInParmProbs_cookie;
extern LeafInfo icmpInSrcQuenchs_cookie;
extern LeafInfo icmpInRedirects_cookie;
extern LeafInfo icmpInEchos_cookie;
extern LeafInfo icmpInEchoReps_cookie;
extern LeafInfo icmpInTimestamps_cookie;
extern LeafInfo icmpInTimestampReps_cookie;
extern LeafInfo icmpInAddrMasks_cookie;
extern LeafInfo icmpInAddrMaskReps_cookie;
extern LeafInfo icmpOutMsgs_cookie;
extern LeafInfo icmpOutErrors_cookie;
extern LeafInfo icmpOutDestUnreachs_cookie;
extern LeafInfo icmpOutTimeExcds_cookie;
extern LeafInfo icmpOutParmProbs_cookie;
extern LeafInfo icmpOutSrcQuenchs_cookie;
extern LeafInfo icmpOutRedirects_cookie;
extern LeafInfo icmpOutEchos_cookie;
extern LeafInfo icmpOutEchoReps_cookie;
extern LeafInfo icmpOutTimestamps_cookie;
extern LeafInfo icmpOutTimestampReps_cookie;
extern LeafInfo icmpOutAddrMasks_cookie;
extern LeafInfo icmpOutAddrMaskReps_cookie;
extern LeafInfo ipForwarding_cookie;
extern LeafInfo ipDefaultTTL_cookie;
extern LeafInfo ipInReceives_cookie;
extern LeafInfo ipInHdrErrors_cookie;
extern LeafInfo ipInAddrErrors_cookie;
extern LeafInfo ipForwDatagrams_cookie;
extern LeafInfo ipInUnknownProtos_cookie;
extern LeafInfo ipInDiscards_cookie;
extern LeafInfo ipInDelivers_cookie;
extern LeafInfo ipOutRequests_cookie;
extern LeafInfo ipOutDiscards_cookie;
extern LeafInfo ipOutNoRoutes_cookie;
extern LeafInfo ipReasmTimeout_cookie;
extern LeafInfo ipReasmReqds_cookie;
extern LeafInfo ipReasmOKs_cookie;
extern LeafInfo ipReasmFails_cookie;
extern LeafInfo ipFragOKs_cookie;
extern LeafInfo ipFragFails_cookie;
extern LeafInfo ipFragCreates_cookie;
extern LeafInfo ipAddrTable_cookie;
extern LeafInfo ipRouteTable_cookie;
extern LeafInfo ipNetToMediaTable_cookie;
extern LeafInfo ipRoutingDiscards_cookie;
extern LeafInfo ipNetToMediaIfIndex_cookie;
extern LeafInfo ipNetToMediaNetAddress_cookie;
extern LeafInfo ipNetToMediaPhysAddress_cookie;
extern LeafInfo ipNetToMediaType_cookie;
extern LeafInfo ipRouteDest_cookie;
extern LeafInfo ipRouteIfIndex_cookie;
extern LeafInfo ipRouteMetric1_cookie;
extern LeafInfo ipRouteMetric2_cookie;
extern LeafInfo ipRouteMetric3_cookie;
extern LeafInfo ipRouteMetric4_cookie;
extern LeafInfo ipRouteNextHop_cookie;
extern LeafInfo ipRouteType_cookie;
extern LeafInfo ipRouteProto_cookie;
extern LeafInfo ipRouteAge_cookie;
extern LeafInfo ipRouteMask_cookie;
extern LeafInfo ipRouteMetric5_cookie;
extern LeafInfo ipRouteInfo_cookie;
extern LeafInfo ipAdEntAddr_cookie;
extern LeafInfo ipAdEntIfIndex_cookie;
extern LeafInfo ipAdEntNetMask_cookie;
extern LeafInfo ipAdEntBcastAddr_cookie;
extern LeafInfo ipAdEntReasmMaxSize_cookie;
extern LeafInfo ifNumber_cookie;
extern LeafInfo ifTable_cookie;
extern LeafInfo ifIndex_cookie;
extern LeafInfo ifDescr_cookie;
extern LeafInfo ifType_cookie;
extern LeafInfo ifMtu_cookie;
extern LeafInfo ifSpeed_cookie;
extern LeafInfo ifPhysAddress_cookie;
extern LeafInfo ifAdminStatus_cookie;
extern LeafInfo ifOperStatus_cookie;
extern LeafInfo ifLastChange_cookie;
extern LeafInfo ifInOctets_cookie;
extern LeafInfo ifInUcastPkts_cookie;
extern LeafInfo ifInNUcastPkts_cookie;
extern LeafInfo ifInDiscards_cookie;
extern LeafInfo ifInErrors_cookie;
extern LeafInfo ifInUnknownProtos_cookie;
extern LeafInfo ifOutOctets_cookie;
extern LeafInfo ifOutUcastPkts_cookie;
extern LeafInfo ifOutNUcastPkts_cookie;
extern LeafInfo ifOutDiscards_cookie;
extern LeafInfo ifOutErrors_cookie;
extern LeafInfo ifOutQLen_cookie;
extern LeafInfo ifSpecific_cookie;
extern LeafInfo atTable_cookie;
extern LeafInfo atIfIndex_cookie;
extern LeafInfo atNetAddress_cookie;
extern LeafInfo atPhysAddress_cookie;
extern LeafInfo sysDescr_cookie;
extern LeafInfo sysObjectID_cookie;
extern LeafInfo sysUpTime_cookie;
extern LeafInfo sysContact_cookie;
extern LeafInfo sysName_cookie;
extern LeafInfo sysLocation_cookie;
extern LeafInfo sysServices_cookie;
extern LeafInfo sysORLastChange_cookie;
extern LeafInfo sysORTable_cookie;
extern LeafInfo sysORIndex_cookie;
extern LeafInfo sysORID_cookie;
extern LeafInfo sysORDescr_cookie;
extern LeafInfo sysORUpTime_cookie;
extern LeafInfo applTable_cookie;
extern LeafInfo aapplTable_cookie;
extern LeafInfo bootpTable_cookie;
extern LeafInfo pbIntVcTable_cookie;
extern LeafInfo pbFileSystemTable_cookie;
extern LeafInfo pbDivaMTable_cookie;
extern LeafInfo moduleTable_cookie;
extern LeafInfo applTableVer_cookie;
extern LeafInfo pbPortLedsTable_cookie;
extern LeafInfo pbPortLedProxySlot_cookie;
extern LeafInfo pbPortLedSlot_cookie;
extern LeafInfo pbPortLedIndexNo_cookie;
extern LeafInfo pbPortLedColor_cookie;
extern LeafInfo moduleIndex_cookie;
extern LeafInfo moduleBoardFunction_cookie;
extern LeafInfo moduleBoardProperty_cookie;
extern LeafInfo moduleBoardStyle_cookie;
extern LeafInfo moduleChassisType_cookie;
extern LeafInfo moduleName_cookie;
extern LeafInfo moduleFname_cookie;
extern LeafInfo moduleStart_cookie;
extern LeafInfo moduleInit_cookie;
extern LeafInfo pbDivaMslotno_cookie;
extern LeafInfo pbDivaMportno_cookie;
extern LeafInfo pbDivaMmetric_cookie;
extern LeafInfo pbFileDir_cookie;
extern LeafInfo pbFileName_cookie;
extern LeafInfo pbFileSize_cookie;
extern LeafInfo pbFileSlot_cookie;
extern LeafInfo pbIntVcLocalIntf_cookie;
extern LeafInfo pbIntVcLocalVpi_cookie;
extern LeafInfo pbIntVcLocalVci_cookie;
extern LeafInfo pbIntVcRemoteIntf_cookie;
extern LeafInfo pbIntVcRemoteVpi_cookie;
extern LeafInfo pbIntVcRemoteVci_cookie;
extern LeafInfo bootpBoardType_cookie;
extern LeafInfo bootpFname_cookie;
extern LeafInfo aapplSlotNo_cookie;
extern LeafInfo aapplNumber_cookie;
extern LeafInfo aapplName_cookie;
extern LeafInfo aapplCpuUsage_cookie;
extern LeafInfo aapplLogLevels_cookie;
extern LeafInfo aapplOptions_cookie;
extern LeafInfo aapplStatus_cookie;
extern LeafInfo aapplLoadOrder_cookie;
extern LeafInfo applIndex_cookie;
extern LeafInfo applBoardFunction_cookie;
extern LeafInfo applBoardProperty_cookie;
extern LeafInfo applBoardStyle_cookie;
extern LeafInfo applChassisType_cookie;
extern LeafInfo applName_cookie;
extern LeafInfo applNumber_cookie;
extern LeafInfo applFname_cookie;
extern LeafInfo applEntryfn_cookie;
extern LeafInfo applTaskName_cookie;
extern LeafInfo applTaskPri_cookie;
extern LeafInfo applTaskStack_cookie;
extern LeafInfo applLogLevels_cookie;
extern LeafInfo applOptions_cookie;
extern LeafInfo applStart_cookie;



# 22 "../../../appl/CTD/inc/ctd_tm.h" 2

# 1 "../../../appl/CTD/inc/ctd_fm.h" 1
 




















































































































































































































































































































































                                                         



 























































































































































































































































































































 





















































































































































































































































































































typedef enum
{
    FMA_ALM_CLEAR = 0,
    FMA_ALM_SET = 1
}                       FMA_ALM_STATE;

typedef struct
{
    unsigned int    entity;
    unsigned int    instance;
    HW_PORT_TYPE    portType;
    SDH_TP_TYPE     tpType;
    UINT32          e;
    UINT32          d;
    UINT32          c;
    UINT32          b;
    UINT32          a;
    UINT32          k;
    UINT32          l;
    UINT32          m;    
} FM_TP_PATH_T;

typedef struct {
    BOOL phyPathValid;                      
    BOOL realPathValid;                     
    BOOL phyPathChanged;                    
    BOOL realPathChanged;                   
    unsigned int oldRealEntity;             
    FM_TP_PATH_T basePath;                  
    FM_TP_PATH_T phyPath;                   
    FM_TP_PATH_T realPath;                  
} FM_PCO_TP_T; 

 


 
typedef unsigned int    FM_AISMODE_T;



typedef unsigned int    FM_RDIMODE_T;



 










typedef PB_ERROR_CODE_T (*FMA_AlmModDefectFunc)(UINT32 SlotId, INSTANCE_T Instance, UINT32 PortId, HW_PORT_TYPE PortType, SDH_TP_TYPE TpType, COMPACT_TP_T TpIndex, UINT32 DefectId, FMA_ALM_STATE state);

typedef PB_ERROR_CODE_T (*FMC_AlmModDefectFunc)(UINT32 SlotId, INSTANCE_T Instance, UINT32 PortId, HW_PORT_TYPE PortType, SDH_TP_TYPE TpType, COMPACT_TP_T TpIndex, UINT32 DefectId, FMA_ALM_STATE state);

typedef PB_ERROR_CODE_T (*FMA_AlmModDefectByBusFunc)(UINT16 busNo, UINT16 vc4No, TU_TP_T tpIndex, SDH_TP_TYPE tpType, UINT32 defectId, FMA_ALM_STATE state);


 






typedef PB_ERROR_CODE_T  (*FMA_GetBusBySlot)(UINT32 slotId, UINT32 portNo, UINT32 portType, COMPACT_TP_T tpIndex, UINT16 * pBusNo, UINT16 * pVc4No);

typedef PB_ERROR_CODE_T  (*FMA_GetSlotByBus)(UINT16 busNo, UINT16 vc4No, UINT32 * pSlotId, UINT32 * pPortNo,UINT32 * pEDCB, UINT32 * pPortType);

extern PB_ERROR_CODE_T FMA_RegisterBusBySlot(FMA_GetBusBySlot pFmaBusBySlot);
extern PB_ERROR_CODE_T FMA_RegisterSlotByBus(FMA_GetSlotByBus pFmaSlotByBus);





 
































































































# 23 "../../../appl/CTD/inc/ctd_tm.h" 2


 
typedef enum
{
    TMS_SSF_CLEAR = 0,
    TMS_SSF_SET = 1
}TMS_SSF_ACTION;


 
typedef void (*TMA_S1ChangeCallbackFunc)(UINT32 instance, UINT8 port, UINT8 ssmb);
extern void TMA_S1ChangeCallback(UINT32 instance, UINT8 port, UINT8 ssmb);

 
typedef PB_ERROR_CODE_T (*TMH_InitCCFPGAFunc)(UINT32 instance, BOOL ccstate, BOOL cardstate, BOOL warm);
extern PB_ERROR_CODE_T TMH_InitCCFPGA(UINT32 instance, BOOL ccstate, BOOL cardstate, BOOL warm);

 
typedef void (*TMA_ModDefectFunc)(UINT32 instance, UINT8 SlotId, UINT8 PortId, UINT32 DefectId, TMS_SSF_ACTION state);
extern void TMA_SetDefect(UINT32 instance, UINT8 SlotId, UINT8 PortId, UINT32 DefectId,TMS_SSF_ACTION state);

 
typedef PB_ERROR_CODE_T (*TMH_SwitchHOCCFunc)(BOOL isWorkHOCC, BOOL bCallTmaStep);
extern PB_ERROR_CODE_T TMH_SwitchHOCC(BOOL isWorkHOCC, BOOL bCallTmaStep);

extern PB_ERROR_CODE_T TMT_DetectCardState(INSTANCE_T inst, BOOL *pbActive);
extern PB_ERROR_CODE_T TMT_DeActivateSICard (INSTANCE_T inst);
extern PB_ERROR_CODE_T TMT_ActivateSICard (INSTANCE_T inst, BOOL bWarm);
extern PB_ERROR_CODE_T TMT_InitSICard (INSTANCE_T inst, UINT32 ulStyle, UINT32 ulOption);
extern void TMI_ReportAlarm(FMC_AlmModDefectFunc pFunc);

typedef PB_ERROR_CODE_T (*TM_RptPLLUnlock)(SLOT_NUMBER_T slot, BOOL bUnitFail);
void TMT_RegRptPLLUnlock(FMA_AlmModDefectFunc pFunc);
void TMH_RegRptPLLUnlock(TM_RptPLLUnlock pFunc);
void TMH_RegReportAlarmFunction(FMA_AlmModDefectFunc pFunc);











# 19 "../../../appl/CTD/inc/ctd_drv.h" 2


# 1 "../../../appl/CTD/inc/ctd_pm.h" 1
  









 
















typedef struct{
    UINT8 pbSlotNumber;
    UINT8 pbPortNo;
    UINT8 portType; 
    TP_INDEX_T tpIndex;
}__attribute__ ((packed))  PM_TP_NO_T;  

typedef struct {
    BOOL bRsBipMon;
    BOOL bRsOfsMon;
    BOOL bMsBipMon;
    BOOL bMsReiMon;
}__attribute__ ((packed))  PMA_SOH_MON_T;

typedef struct {
    BOOL bHpBipMon;
    BOOL bHpReiMon;
    BOOL bHpNpjcMon;
    BOOL bHpPpjcMon;
    BOOL bHpFeNpjcMon;
    BOOL bHpFePpjcMon;
}__attribute__ ((packed))  PMA_HP_MON_T;

typedef struct {
    BOOL bLpBipMon;
    BOOL bLpReiMon;
}__attribute__ ((packed))  PMA_LP_MON_T;

# 94 "../../../appl/CTD/inc/ctd_pm.h"

typedef struct
{
    UINT8    pbSlotNumber;                                 
    INSTANCE_T            instance;
    UINT8        pbPortNo;                                                 
    UINT32 pbEthTableType;                                  
    UINT8   pbEthPmPortType;                                  
    UINT32 pbEthPmMonitor;
    char        pbEthPmTimeStart[16];
    unsigned int    pbEthPmTimeElapse;
    BOOL            pbEthPmTimeSuspect;
    BOOL            pbEthPmTableReset;
    unsigned int    pbEthInUnicastPktsHigh;
    unsigned int    pbEthInUnicastPktsLow;
    unsigned int    pbEthInMulcastPktsHigh;
    unsigned int    pbEthInMulcastPktsLow;
    unsigned int    pbEthInBrdcastPktsHigh;
    unsigned int    pbEthInBrdcastPktsLow;
    unsigned int    pbEthInAllBadPktsHigh;
    unsigned int    pbEthInAllBadPktsLow;
    unsigned int    pbEthInAllBadBytesHigh;
    unsigned int    pbEthInAllBadBytesLow;
    unsigned int    pbEthInAllGoodPktsHigh;
    unsigned int    pbEthInAllGoodPktsLow;
    unsigned int    pbEthInAllGoodBytesHigh;
    unsigned int    pbEthInAllGoodBytesLow;
    unsigned int    pbEthInOverSizePktsHigh;
    unsigned int    pbEthInOverSizePktsLow;
    unsigned int    pbEthInUnderSizePktsHigh;
    unsigned int    pbEthInUnderSizePktsLow;
    unsigned int    pbEthInCRCErrPktsHigh;
    unsigned int    pbEthInCRCErrPktsLow;
    unsigned int    pbEthOutUnicastPktsHigh;
    unsigned int    pbEthOutUnicastPktsLow;
    unsigned int    pbEthOutMulcastPktsHigh;
    unsigned int    pbEthOutMulcastPktsLow;
    unsigned int    pbEthOutBrdcastPktsHigh;
    unsigned int    pbEthOutBrdcastPktsLow;
    unsigned int    pbEthOutAllGoodPktsHigh;
    unsigned int    pbEthOutAllGoodPktsLow;
    unsigned int    pbEthOutAllGoodBytesHigh;
    unsigned int    pbEthOutAllGoodBytesLow;
    UINT32          pbEthBbrEnable;
    int        pbEthBbrThresholdValue;
    UINT32         pbEthBprEnable;
    int        pbEthBprThresholdValue;
}__attribute__ ((packed))  ETH_PM_DTAT_T;  

typedef struct 
{  
    UINT8                      slot;
    UINT8                      portType;
    UINT8                      actPortType;
    UINT8                      portNo;
    TP_INDEX_T             tpIndex;
}__attribute__ ((packed))  PM_TP_INDEX_T;


typedef struct
{
	UINT8			   instance;
       UINT8                   slot;
       UINT8                   portType;                         
       UINT8                   portNo;                                  
}__attribute__ ((packed))  PM_ETH_INDEX_T;

 
typedef struct
{
    UINT8  slot;
    int     rprIfIndex;
}__attribute__ ((packed))  PM_RPR_CLIENT_INDEX_T;

typedef struct
{
    UINT8  slot;
    int     rprIfIndex;
    RprSpan_Tval rprSpanId;
}__attribute__ ((packed))  PM_RPR_SPAN_INDEX_T;

typedef struct
{
    UINT8         slot;
    UINT8         portType;
    UINT8         portNo;
}__attribute__ ((packed))  PM_PDH_SLIP_INDEX_T;

typedef struct
{
    UINT8                 slot;
    UINT8                 portType;
    UINT8                 portNo;
}__attribute__ ((packed))  PM_PDH_INDEX_T;

typedef struct
{
    UINT8                 slot;
    UINT8                 portType;
    UINT8                 portNo;
}__attribute__ ((packed))  PM_FEC_INDEX_T;


typedef struct 
{
	 
	
# 216 "../../../appl/CTD/inc/ctd_pm.h"

    
     
    UINT32  ethInAllBytes; 
    UINT32  ethInAllPkts; 
    UINT32  ethInAllGoodPkts; 
    UINT32  ethInAllBadPkts;  
    UINT32  ethInUnicastPkts; 
    UINT32  ethInMulcastPkts; 
    UINT32  ethInBrdcastPkts; 
    UINT32  ethInDropPkts;
    UINT32  ethInUndersizeGoodPkts; 
    UINT32  ethInOversizeGoodPkts;
    UINT32  ethIn64Pkts;
    UINT32  ethIn65To127Pkts;
    UINT32  ethIn128To255Pkts; 
    UINT32  ethIn256To511Pkts; 
    UINT32  ethIn512To1023Pkts;
    UINT32  ethIn1024To1518Pkts; 
    UINT32  ethInPausePkts;
    UINT32  ethInFragmentPkts; 
    UINT32  ethInJabberPkts; 
    UINT32  ethInAlignmentErrorPkts;
    UINT32  ethOutAllBytes; 
    UINT32  ethOutAllPkts;
    UINT32  ethOutUnicastPkts; 
    UINT32  ethOutMulcastPkts; 
    UINT32  ethOutBrdcastPkts; 
    UINT32  ethOutGoodPausePkts;
    UINT32  ethOutCollisionPkts;
}__attribute__ ((packed))  PM_ETH_DATA_T;

typedef struct
{
	UINT8 				instance;
         UINT8                slot;
         UINT8                portNo; 
         UINT8                portType;
         UINT32              vlanId;
}__attribute__ ((packed))  PM_ETH_VLAN_INDEX_T;      

typedef struct 
{
       UINT32    ethVlanInBytes;
       UINT32    ethVlanInPkts;  
       UINT32    ethVlanDropPkts;    
       UINT32    RcvUnicastFrames;
       UINT32    RcvTotalFrames;
       UINT32    RcvUnicastBytes;
       UINT32    RcvTotalBytes;
}__attribute__ ((packed))  PM_ETH_VLAN_DATA_T;    

typedef void (*PMP_GetRsOofFunc)(UINT8 slot,UINT16 port,HW_PORT_TYPE portType);













 































































# 21 "../../../appl/CTD/inc/ctd_drv.h" 2

# 1 "../../../appl/CTD/inc/ctd_mp.h" 1
 


















# 1 "../../../include/tasks.h" 1











 



 
 
 
 
 
 
















































 

 



















 















 






  
























































 


























 



 



 
typedef enum
{
    TASK_CORE_SECTION       = 0,  
    TASK_URGENT_SECTION     = 1,  
    TASK_MANAGEMENT_SECTION = 2,  
    TASK_NORMAL_SECTION     = 3,  
    TASK_IDLE_SECTION       = 4  
} TASK_PRIORITY_SECTION_T;












 
 
 
 
 

 








 
 
 




















































 






 



































 





 














 







 




















 

 







































 















 


































 























 










































 


 



 




 







 













































































































































































# 20 "../../../appl/CTD/inc/ctd_mp.h" 2



 







typedef enum msp_switch_type
{
    MSP_REQ_LP        =     127,  
    MSP_REQ_SF_P      =     126,  
    MSP_REQ_FS_W      =     125,  
    MSP_REQ_FS_P      =     123,  
    MSP_REQ_FS_2      =     122,  
    MSP_REQ_FS_3      =     121,  
    MSP_REQ_FS_4      =     120,  
    MSP_REQ_FS_5      =     119,  
    MSP_REQ_FS_6      =     118,  
    MSP_REQ_FS_7      =     117,  
    MSP_REQ_FS_8      =     116,  
    MSP_REQ_FS_9      =     115,  
    MSP_REQ_FS_10     =     114,  
    MSP_REQ_FS_11     =     113,  
    MSP_REQ_FS_12     =     112,  
    MSP_REQ_FS_13     =     111,  
    MSP_REQ_FS_14     =     110,  
    MSP_REQ_FS_15     =     109,  

    MSP_REQ_SF_W      =     108,  
    MSP_REQ_SF_H_2    =     107,  
    MSP_REQ_SF_H_3    =     106,  
    MSP_REQ_SF_H_4    =     105,  
    MSP_REQ_SF_H_5    =     104,  
    MSP_REQ_SF_H_6    =     103,  
    MSP_REQ_SF_H_7    =     102,  
    MSP_REQ_SF_H_8    =     101,  
    MSP_REQ_SF_H_9    =     100,  
    MSP_REQ_SF_H_10   =     99,  
    MSP_REQ_SF_H_11   =     98,  
    MSP_REQ_SF_H_12   =     97,  
    MSP_REQ_SF_H_13   =     96,  
    MSP_REQ_SF_H_14   =     95,  

    MSP_REQ_SF_L_1    =     94,  
    MSP_REQ_SF_L_2    =     93,  
    MSP_REQ_SF_L_3    =     92,  
    MSP_REQ_SF_L_4    =     91,  
    MSP_REQ_SF_L_5    =     90,  
    MSP_REQ_SF_L_6    =     89,  
    MSP_REQ_SF_L_7    =     88,  
    MSP_REQ_SF_L_8    =     87,  
    MSP_REQ_SF_L_9    =     86,  
    MSP_REQ_SF_L_10   =     85,  
    MSP_REQ_SF_L_11   =     84,  
    MSP_REQ_SF_L_12   =     83,  
    MSP_REQ_SF_L_13   =     82,  
    MSP_REQ_SF_L_14   =     81,  

    MSP_REQ_SD_P      =     78,  
    MSP_REQ_SD_W      =     76,  
    MSP_REQ_SD_H_2    =     75,  
    MSP_REQ_SD_H_3    =     74,  
    MSP_REQ_SD_H_4    =     73,  
    MSP_REQ_SD_H_5    =     72,  
    MSP_REQ_SD_H_6    =     71,  
    MSP_REQ_SD_H_7    =     70,  
    MSP_REQ_SD_H_8    =     69,  
    MSP_REQ_SD_H_9    =     68,  
    MSP_REQ_SD_H_10   =     67,  
    MSP_REQ_SD_H_11   =     66,  
    MSP_REQ_SD_H_12   =     65,  
    MSP_REQ_SD_H_13   =     64,  
    MSP_REQ_SD_H_14   =     63,  

    MSP_REQ_SD_L_1    =     62,  
    MSP_REQ_SD_L_2    =     61,  
    MSP_REQ_SD_L_3    =     60,  
    MSP_REQ_SD_L_4    =     59,  
    MSP_REQ_SD_L_5    =     58,  
    MSP_REQ_SD_L_6    =     57,  
    MSP_REQ_SD_L_7    =     56,  
    MSP_REQ_SD_L_8    =     55,  
    MSP_REQ_SD_L_9    =     54,  
    MSP_REQ_SD_L_10   =     53,  
    MSP_REQ_SD_L_11   =     52,  
    MSP_REQ_SD_L_12   =     51,  
    MSP_REQ_SD_L_13   =     50,  
    MSP_REQ_SD_L_14   =     49,  

    MSP_REQ_MS_W      =     46,  
    MSP_REQ_MS_P      =     45,  
    MSP_REQ_MS_2      =     44,  
    MSP_REQ_MS_3      =     43,  
    MSP_REQ_MS_4      =     42,  
    MSP_REQ_MS_5      =     41,  
    MSP_REQ_MS_6      =     40,  
    MSP_REQ_MS_7      =     39,  
    MSP_REQ_MS_8      =     38,  
    MSP_REQ_MS_9      =     37,  
    MSP_REQ_MS_10     =     36,  
    MSP_REQ_MS_11     =     35,  
    MSP_REQ_MS_12     =     34,  
    MSP_REQ_MS_13     =     33,  
    MSP_REQ_MS_14     =     32,  
    MSP_REQ_MS_15     =     31,  
    MSP_REQ_WTR       =     30,  
    MSP_REQ_EXER_0    =     29,  
    MSP_REQ_EXER_1    =     28,  
    MSP_REQ_EXER_2    =     27,  
    MSP_REQ_EXER_3    =     26,  
    MSP_REQ_EXER_4    =     25,  
    MSP_REQ_EXER_5    =     24,  
    MSP_REQ_EXER_6    =     23,  
    MSP_REQ_EXER_7    =     22,  
    MSP_REQ_EXER_8    =     21,  
    MSP_REQ_EXER_9    =     20,  
    MSP_REQ_EXER_10   =     19,  
    MSP_REQ_EXER_11   =     18,  
    MSP_REQ_EXER_12   =     17,  
    MSP_REQ_EXER_13   =     16,  
    MSP_REQ_EXER_14   =     15,  
    MSP_REQ_EXER_15   =     14,  
    MSP_REQ_RR        =     13,  
    MSP_REQ_DNR       =     12,  
    MSP_REQ_NRM       =     0,  
    MSP_REQ_NR        =     0,  
} MSP_SWITCH_REQ_T;

typedef enum msp_switch_type_t
{
    MSP_NO_REQUEST = 0,      
    MSP_DNOTREV_REQ,         
    MSP_REVERT_REQ,          
    MSP_UNUSED3_REQ,         
    MSP_EXERCISE_REQ,        
    MSP_UNUSED5_REQ,         
    MSP_WTR_REQ,             
    MSP_UNUSED7_REQ,         
    MSP_MAN_REQ,             
    MSP_UNUSED9_REQ,         
    MSP_SD_LPRI_REQ,         
    MSP_SD_HPRI_REQ,         
    MSP_SF_LPRI_REQ,         
    MSP_SF_HPRI_REQ,         
    MSP_FORCED_REQ,          
    MSP_LOCK_REQ,            
    MSP_UNKNOWN_REQ = 0x10
} MSP_SWITCH_REQ_TYPE_T;

 



typedef enum
{
    CTD_MSP_SIG_STATE_UNKNOWN   = 0x0,
    CTD_MSP_SIG_STATE_NORMAL    = 0x1,
    CTD_MSP_SIG_STATE_DEGRADE   = 0x2,
    CTD_MSP_SIG_STATE_FAIL      = 0x3,
}CTD_MSP_SIG_STATE_T;

typedef enum
{
    CTD_MSP_UNIDIRECTION    = 0x0,
    CTD_MSP_BIDIRECTION     = 0x1,
}CTD_MSP_DIR_MODE_T;

typedef enum
{
    CTD_MSP_NONREVERTIVE = 0x0,
    CTD_MSP_REVERTIVE    = 0x1,
}CTD_MSP_RVT_MODE_T;

typedef enum
{
    CTD_MSP_ACTIVE       = 0x0,
    CTD_MSP_STANDBY      = 0x1,
}CTD_MSP_CH_STATE_T;

typedef enum
{
    CTD_MSP_PRIORI_HIGH       = 0x0,
    CTD_MSP_PRIORI_LOW        = 0x1,
}CTD_MSP_CH_PRIORITY_T;

typedef enum
{
    CTD_MSP_CMD_CLEAR               = 0x7F,
    CTD_MSP_LOCK_OUT_PROT           = 0x7E,
    CTD_MSP_FORCE_SWTICH_WORK       = 0x7D,
    CTD_MSP_FORCE_SWITCH_PROT_1     = 0x7B,
    CTD_MSP_FORCE_SWITCH_PROT_2     = 0x7A,
    CTD_MSP_FORCE_SWITCH_PROT_3     = 0x79,    
    CTD_MSP_FORCE_SWITCH_PROT_4     = 0x78,    
    CTD_MSP_FORCE_SWITCH_PROT_5     = 0x77,    
    CTD_MSP_FORCE_SWITCH_PROT_6     = 0x76,    
    CTD_MSP_FORCE_SWITCH_PROT_7     = 0x75,    
    CTD_MSP_FORCE_SWITCH_PROT_8     = 0x74,    
    CTD_MSP_FORCE_SWITCH_PROT_9     = 0x73,    
    CTD_MSP_FORCE_SWITCH_PROT_10    = 0x72,        
    CTD_MSP_FORCE_SWITCH_PROT_11    = 0x71,        
    CTD_MSP_FORCE_SWITCH_PROT_12    = 0x70,        
    CTD_MSP_FORCE_SWITCH_PROT_13    = 0x6F,        
    CTD_MSP_FORCE_SWITCH_PROT_14    = 0x6E,
    CTD_MSP_FORCE_SWITCH_PROT_15    = 0x6D,            
    CTD_MSP_MANUAL_SWITCH_WORK      = 0x2E,
    CTD_MSP_MANUAL_SWITCH_PROT_1    = 0x2D,
    CTD_MSP_MANUAL_SWITCH_PROT_2    = 0x2C,
    CTD_MSP_MANUAL_SWITCH_PROT_3    = 0x2B,
    CTD_MSP_MANUAL_SWITCH_PROT_4    = 0x2A,
    CTD_MSP_MANUAL_SWITCH_PROT_5    = 0x29,
    CTD_MSP_MANUAL_SWITCH_PROT_6    = 0x28,
    CTD_MSP_MANUAL_SWITCH_PROT_7    = 0x27,
    CTD_MSP_MANUAL_SWITCH_PROT_8    = 0x26,
    CTD_MSP_MANUAL_SWITCH_PROT_9    = 0x25,
    CTD_MSP_MANUAL_SWITCH_PROT_10   = 0x24,
    CTD_MSP_MANUAL_SWITCH_PROT_11   = 0x23,
    CTD_MSP_MANUAL_SWITCH_PROT_12   = 0x22,
    CTD_MSP_MANUAL_SWITCH_PROT_13   = 0x21,
    CTD_MSP_MANUAL_SWITCH_PROT_14   = 0x20,
    CTD_MSP_MANUAL_SWITCH_PROT_15   = 0x1F,
    CTD_MSP_EXER_0                  = 0x1D,
    CTD_MSP_EXER_1                  = 0x1C,
    CTD_MSP_EXER_2                  = 0x1B,
    CTD_MSP_EXER_3                  = 0x1A,
    CTD_MSP_EXER_4                  = 0x19,
    CTD_MSP_EXER_5                  = 0x18,
    CTD_MSP_EXER_6                  = 0x17,
    CTD_MSP_EXER_7                  = 0x16,
    CTD_MSP_EXER_8                  = 0x15,
    CTD_MSP_EXER_9                  = 0x14,
    CTD_MSP_EXER_10                 = 0x13,
    CTD_MSP_EXER_11                 = 0x12,
    CTD_MSP_EXER_12                 = 0x11,
    CTD_MSP_EXER_13                 = 0x10,    
    CTD_MSP_EXER_14                 = 0x0F,    
    CTD_MSP_EXER_15                 = 0x0E,
    CTD_MSP_NO_COMMAND              = 0x00,
}CTD_MSP_CMD_TYPE_T;

typedef enum 
{
    CTD_MSP_CMD_RSLT_SUCC        = 0x1,
    CTD_MSP_CMD_RSLT_LOWPRIORITY = 0x2,
    CTD_MSP_CMD_SWITCH_TO_ACTIVE = 0x3,
    CTD_MSP_CMD_SWITCH_FAIL      = 0x4,   
}CTD_MSP_CMD_RSLT_T;

typedef enum
{
    CTD_MSP_RSN_CLEAR_CMD         = 0x1,
    CTD_MSP_RSN_LP_CMD            = 0x2,
    CTD_MSP_RSN_FORCED_SWITCH     = 0x3,
    CTD_MSP_RSN_MANUAL_SWITCH     = 0x4,
    CTD_MSP_RSN_SIGNAL_FAIL       = 0x5,
    CTD_MSP_RSN_SIGNAL_FAIL_CLEAR = 0x6,
    CTD_MSP_RSN_SIGNAL_DEG        = 0x7,
    CTD_MSP_RSN_SIGNAL_DEG_CLEAR  = 0x8,
    CTD_MSP_RSN_NO_SWITCH         = 0x9,
} CTD_MSP_SWITCH_RSN_T;

typedef enum
{
    CTD_MSP_PRIO_LOW              = 0x0,
    CTD_MSP_PRIO_HIGH             = 0x1,
}CTD_MSP_PRIO_TYPE_T;

typedef struct
{
     
    CTD_MSP_SIG_STATE_T  eWorkSigState[14];
    CTD_MSP_CH_STATE_T   eWorkChState[14]; 
     
    CTD_MSP_SIG_STATE_T  eProtSigState;
    CTD_MSP_CH_STATE_T   eProtChState; 
     
    CTD_MSP_CMD_TYPE_T   eMspExtCmd;                          
    CTD_MSP_CMD_RSLT_T   eMspExtCmdRslt;  
     
    MSP_SWITCH_REQ_T     eMspCurSwitchReq;                         
    CTD_MSP_SWITCH_RSN_T eMspSwitchReason;
     
    BOOL                 bFopAlarm;
     
    UINT8                ucRcvdK1Value;
    UINT8                ucRcvdK2Value;
     
    UINT8                ucTransK1Value;
    UINT8                ucTransK2Value;
     
    BOOL                 bIsExtra;
}CTD_MSP_STATE_T;

typedef struct
{
    UINT16              usMspGrpNum;
    UINT8               ucMspWorkChNum;
    UINT8               ucMspType;   
    CTD_MSP_STATE_T     *sMspState;
}CTD_HW_MSP_STATE_T;
# 347 "../../../appl/CTD/inc/ctd_mp.h"


 
typedef struct
{
     
    UINT16 usGrpId;
    UINT16 usProtSlot;                  
    UINT16 usProtPort;
    UINT8  ucProtPortType;
    UINT16 usWorkNum;                   
    UINT16 usWorkSlot[14];
    UINT16 usWorkPort[14];
    UINT8  ucWorkPortType[14];
    UINT8  ucWorkPrio[14];
    UINT32              ulWtrVal;	    
    CTD_MSP_CMD_TYPE_T  eCmdType;       
    BOOL   bIsExtra;                    

    BOOL   bEnable;                     
}CTD_MSP_1VSN_CFG_T;

typedef struct
{
    UINT16 usWorkNum;
    CTD_MSP_PRIO_TYPE_T sPrio[14];
}CTD_MSP_PRIO_STAT_T;

 
typedef struct
{
    UINT16 usBusRx;
    UINT16 usBusTx;
    UINT16 usVc4;
}CTD_TP_PARAM_CFG_T;
typedef struct
{
    UINT16 usTpCount;
    UINT16 usGrpId;
    CTD_TP_PARAM_CFG_T *tp;
}CTD_1ToNMSP_TP_CFG_T;
typedef struct
{
     
     
    CTD_1ToNMSP_TP_CFG_T sProtTp;                  
    UINT16 usWorkNum;                   
    CTD_1ToNMSP_TP_CFG_T sWorkTP[14];    
    UINT8 ucWorkPrio[14];    
    UINT32              ulWtrVal;	    
    CTD_MSP_CMD_TYPE_T  eCmdType;       
    BOOL   bIsExtra;                    

    BOOL   bEnable;                     
}CTD_MSP_1VSN_DRV_CFG_T;


# 419 "../../../appl/CTD/inc/ctd_mp.h"


typedef struct
{
    UINT16              usGrpId;          
    UINT16              usWorkSlot;	    
    UINT16              usWorkPort;	    
    UINT16              usProtSlot;     
    UINT16              usProtPort;	    
    HW_PORT_TYPE        ePortType;      
    CTD_MSP_DIR_MODE_T  eDirMode;       
    CTD_MSP_RVT_MODE_T  eRvtMode;       
    UINT32              ulWtrVal;	    
    CTD_MSP_CMD_TYPE_T  eCmdType;       
    BOOL                bEnable;        
}CTD_MSP_1PLUS1_CFG_T;

typedef void (*MPA_Handle_RcvKbyte)(UINT16 usSlot, UINT16 ucPort, UINT8 ucPorttype, UINT8 ucK1, UINT8 ucK2);
typedef void (*MPA_RcvPhyDefect)(UINT16 usSlot, UINT16 ucPort, UINT8 ucPorttype, MSP_SWITCH_REQ_T eType, UINT32 ulFlag);
typedef void (*MPA_RprtMspStatus)(UINT16 wMspGrpId, CTD_MSP_STATE_T *psMspState);



# 22 "../../../appl/CTD/inc/ctd_drv.h" 2

# 1 "../../../appl/CTD/inc/ctd_ms.h" 1
 















# 1 "../../../include/log.h" 1
 
 
 
 
 
 
 
 	
 
 













































	extern int stack(unsigned int *pStack[], int levels);
	extern void logBackN(const char *title, int n);
	extern void logBackM(const char *title, int n);
	extern void logExit(char *file, int line);
	extern void logDump(char *file, int line);
	extern void sys_panic(char *file, int line);

















 




























	typedef struct {
		int head;
		int tail;
		command_tt *queue[500 ];
	} LogQ_t;

	extern unsigned int utility_log;
	extern unsigned int MsgRecvLogOpen;
	extern unsigned int COL_Log;
	
	
	extern LogQ_t logq;
	void debug_log_init(void);
	void log_enqueue(command_tt * log);
	void log_clear(void);
	void consoleLog(const char *fmt, ...);
 
	void ulogit(int line, const char *file_name, int global, const char *fmt, ...);





# 17 "../../../appl/CTD/inc/ctd_ms.h" 2


# 1 "../../../include/globals.h" 1
#ident  "%full_filespec: globals.h#17:incl:1 %"

# 148 "../../../include/globals.h"

# 19 "../../../appl/CTD/inc/ctd_ms.h" 2




 


typedef enum 
{
    LC_UNKNOWN = 0,      
    LC_CLEAR   = 1,      
    LC_SD      = 2,      
    LC_SF      = 3,      
    LC_MAX     = 4 
}LINE_COND_T;

typedef enum 
{
    KEXP_AUTO_MODE      = 0,     
    KEXP_LOCAL_HW_MODE  = 1,     
    KEXP_LOCAL_SW_MODE  = 2,     
    KEXP_FORCE_MODE     = 3,     
}CTD_KEXP_MODE_T;

typedef enum 
{
    KEXP_UNKNOWN_STATE     = 0,     
    KEXP_AUTO_LOCAL_STATE  = 1,     
    KEXP_AUTO_EXP_STATE    = 2,     
    KEXP_FORCE_EXP_STATE   = 3,     
    KEXP_FORCE_SW_STATE    = 4,     
    KEXP_FORCE_HW_STATE    = 5,     
}CTD_KEXP_STATE_T;

typedef struct
{
    UINT32          ulSlot;
    HW_PORT_TYPE    ePortType;
    UINT32          ulPort;
    UINT32          ulPeerSlot;
    HW_PORT_TYPE    ePeerPortType;
    UINT32          ulPeerPort;
    UINT8           ucNodeId;
    UINT8           ucNodeNum;    
    CTD_KEXP_MODE_T eMode;
    BOOL            bEnable;
}CTD_MSSP_ENABLE_T;

typedef struct
{
    UINT32              ulSlot;
    UINT32              ulPortType;
    UINT32              ulPort;
    union
    {
        struct
        {
            UINT8       ucK1;
            UINT8       ucK2;
            UINT16      usBackup;
        } sKBytes;
        LINE_COND_T     eLineCond;
    } uRptChange;
} CTD_MSSP_CHANGERPT_T;

typedef void (*MSA_ChangeKbyteBatch)(CTD_MSSP_CHANGERPT_T *pKb, UINT32  batNo);
typedef void (*MSA_ChangeLineCondBatch)(CTD_MSSP_CHANGERPT_T *pLc, UINT32  batNo);

typedef void (*MST_ChangeOfKb)(UINT32 ulSlot, UINT8 ucPortType, UINT8 ucPort, UINT8 ucK1, UINT8 ucK2);
typedef void (*MST_ChangeOfLineCond)(UINT32 ulSlot, UINT8 ucPortType, UINT8 ucPort, LINE_COND_T eLcCond);




 




# 23 "../../../appl/CTD/inc/ctd_drv.h" 2

# 1 "../../../appl/CTD/inc/ctd_sp.h" 1
 
 
 
 
 
 
 
 
 
 
 
 




























 
typedef enum 
{
    SNCP_BACK_TP       = 0,
    SNCP_WORK_TP1      = 1,
    SNCP_WORK_TP2      = 2,
    SNCP_WORK_TP3      = 3,
    SNCP_WORK_TP4      = 4,
    SNCP_WORK_TP5      = 5,
    SNCP_WORK_TP6      = 6,
    SNCP_WORK_TP7      = 7,
    SNCP_WORK_TP8      = 8,
    SNCP_WORK_TP9      = 9,
    SNCP_WORK_TP10     = 10,
    SNCP_WORK_TP11     = 11,
    SNCP_WORK_TP12     = 12,
    SNCP_WORK_TP13     = 13,
    SNCP_WORK_TP14     = 14,
    SNCP_WORK_TP15     = 15
}SNCP_TP_CHANNEL_T;

typedef enum 
{
    SNCP_LOGIC_TP      = 0,
    SNCP_PHYSICAL_TP   = 1,
    SNCP_PHYSICAL_TP1  = 2
}SNCP_LOGIC_TP_DEFINE_T;


typedef enum 
{
    SNCP_I             = 0,
    SNCP_N             = 1,
}SNCP_INTRUSIVE_T;

typedef struct 
{
    UINT16               usSlotNum;
    UINT16               usPortType;
    UINT16               usPortNum;
    TP_INDEX_T           TpIndex;
    BOOL                  bInUse;       
}__attribute__ ((packed))  SNCP_CHANNEL_T;


typedef enum 
{
    SPC_TP_TYPE_PROTECT             = 0,
    SPC_TP_TYPE_PROTECT_MS_PROTECT,    
    SPC_TP_TYPE_WORK,
    SPC_TP_TYPE_WORK_MS_PROTECT,
    SPC_TP_TYPE_WORK2,
    SPC_TP_TYPE_WORK2_MS_PROTECT,
    SPC_TP_TYPE_WORK3,
    SPC_TP_TYPE_WORK3_MS_PROTECT,
    SPC_TP_TYPE_WORK4,
    SPC_TP_TYPE_WORK4_MS_PROTECT,
    SPC_TP_TYPE_WORK5,
    SPC_TP_TYPE_WORK5_MS_PROTECT,
    SPC_TP_TYPE_WORK6,
    SPC_TP_TYPE_WORK6_MS_PROTECT,
    SPC_TP_TYPE_WORK7,
    SPC_TP_TYPE_WORK7_MS_PROTECT,
    SPC_TP_TYPE_WORK8,
    SPC_TP_TYPE_WORK8_MS_PROTECT,
    SPC_TP_TYPE_WORK9,
    SPC_TP_TYPE_WORK9_MS_PROTECT,
    SPC_TP_TYPE_WORK10,
    SPC_TP_TYPE_WORK10_MS_PROTECT,
    SPC_TP_TYPE_WORK11,
    SPC_TP_TYPE_WORK11_MS_PROTECT,
    SPC_TP_TYPE_WORK12,
    SPC_TP_TYPE_WORK12_MS_PROTECT,
    SPC_TP_TYPE_WORK13,
    SPC_TP_TYPE_WORK13_MS_PROTECT,
    SPC_TP_TYPE_WORK14,
    SPC_TP_TYPE_WORK14_MS_PROTECT,
    SPC_TP_TYPE_WORK15,
    SPC_TP_TYPE_WORK15_MS_PROTECT
}SNCP_TP_TYPE_T;


typedef enum
{
    SNCP_SIGNAL_UNKNOW     = 0,
    SNCP_SIGNAL_NORMAL     = 1,
    SNCP_SIGNAL_FAIL       = 2,
    SNCP_SIGNAL_DEG        = 3,
}SNCP_SIGNAL_STATE_T;

 
typedef enum
{
    SNCP_1J1_TYPE      = 0,
    SNCP_1BN_TYPE      = 1
}SNCP_GROUP_TYPE_T;

 
typedef struct 
{    
    BOOL                  bEnableSncp;
    UINT32                ulSncpId;										
    UINT16                usWorkBus;
    UINT8                 ucWorkVc4;
    UINT16                usProtBus;
    UINT8                 ucProtVc4;
    UINT16                usDestBus;
    UINT8                 ucDestVc4;
     
    BOOL                 bXCEnable;
     
    UINT16                usDestFirstBus; 
    UINT8                 ucDestFirstVc4;

    UINT8                 ucSncpIntrusive;                
    UINT8                 ucSncpRestoreMode;                
    UINT16                usHoldOffTime;                    
    UINT16                usSncpWTR;   
}__attribute__ ((packed))  HW_HO3P_SNCP_CONFIG_T;

typedef struct 
{
    UINT8                ucBus;
    UINT8                ucVc4;
    UINT16               KLM;   
}__attribute__ ((packed))  LO_MATRIX_ITEM_T;

 
typedef struct 
{    
    BOOL                    bEnableSncp;
    UINT32                  ulSncpId;										
    LO_MATRIX_ITEM_T        WorkItem;
    LO_MATRIX_ITEM_T        ProtItem;
    LO_MATRIX_ITEM_T        DestItem;
    UINT8                	ucSncpIntrusive;                
    UINT8                   ucSncpRestoreMode;  
    UINT8                   ucSncpExtCmd;              
    UINT16                  usHoldOffTime;                    
    UINT16                  usSncpWTR;   
}__attribute__ ((packed))  HW_LO3P_SNCP_CONFIG_T;

typedef union
{
    HW_HO3P_SNCP_CONFIG_T   HoSncpGroup;
    HW_LO3P_SNCP_CONFIG_T   LoScnpGroup;
}__attribute__ ((packed))  HW_3P_SNCP_CONFIG_T;



 
typedef struct 
{    
    UINT32                ulSncpId;													 
    SNCP_GROUP_TYPE_T     GroupType;                          
    UINT8                 ucWorkTpNum;                        
    UINT32                ulTpType;                           
    SNCP_CHANNEL_T  	  Tp[1  + 1][3 ];   
    SNCP_CHANNEL_T        destOrSrcTp[1 +1];   
    UINT8                 ucSncpIntrusive;                    
    UINT8                 ucSncpRestoreMode;                  
    UINT16                usHoldOffTime;                      
    UINT16                usSncpWTR;   
	
 
    UINT8                 ucSncpSignalDircetion;              
}__attribute__ ((packed))  HW_SNCP_CONFIG_T;
typedef struct 
{    
    UINT32                      ulSncpId;                                                    
    SNCP_GROUP_TYPE_T     GroupType;                          
    UINT8                       ucWorkTpNum;                        
    UINT32                      ulTpType;                           
    SNCP_CHANNEL_T          Tp[1  + 1][3 ];   
    SNCP_CHANNEL_T          destOrSrcTp[1 +1];   
    UINT8                       ucSncpIntrusive;                    
    UINT8                       ucSncpRestoreMode;                  
    UINT16                      usHoldOffTime;                      
    UINT16                      usSncpWTR;   
    
 
    UINT8                       ucSncpSignalDircetion;              

    UINT8                   ucSncpFrom;                           
    BOOL                    SncpNeedReportGMPL;           
}__attribute__ ((packed))  HW_SNCP_GMPL_CONFIG_T;  
 
typedef struct 
{
    UINT32     ulSncpId;
    UINT8      ucTpSignalState[1  + 1][3 ];         
    UINT8      ucTpTransfer[1  + 1];               
    UINT8      ucChannelState;                    
    
    UINT16     usTimerStart;                       
    UINT32     ulTimerNum;
        
    UINT8      ucSncpExternalCmd;                            
    UINT8      ucSncpExtCmdResult;                           
    UINT8      ucSncpCurSwitchReq;                           
    UINT8      ucSncpSwitchReason;                           
}__attribute__ ((packed))  HW_SNCP_STATE_T;

 
typedef struct
{
    UINT32                      ulSncpGroupId;
    SNCP_CHANNEL_T              Tp;    
    SNCP_TP_TYPE_T              TpId;
    BOOL                        bEnable;
    SNCP_INTRUSIVE_T            Intrusive;
}__attribute__ ((packed))  SW_SNCP_TP_ENABLE_STR_T;

 
typedef struct
{
    UINT16               slot;
    UINT16               portType;
    UINT16               port;
    TP_INDEX_T           Tp;
    UINT32               SncpIndex;
    SNCP_TP_TYPE_T       WorkTpId;
    SNCP_SIGNAL_STATE_T  bSet;
}__attribute__ ((packed))  SW_SNCP_STATE_REPORT_T;

 

 
typedef void (*SPA_DrvIntHappenFunc)(UINT32 type);

 
typedef PB_ERROR_CODE_T (*SPA_SncpDprFunc)(INSTANCE_T inst, UINT32* pulReportNum, SW_SNCP_STATE_REPORT_T** ppsReportData);
typedef PB_ERROR_CODE_T (*SPA_SncpPollFunc)(INSTANCE_T inst, UINT32* pulReportNum, SW_SNCP_STATE_REPORT_T** ppsReportData);

 
typedef void (*SPA_BoardStateChangeFunc)(BOARD_EVENT_T ulEvent, SLOT_NUMBER_T usSlotNum, BOARD_STYLE_T ulCardType);

 
 
typedef PB_ERROR_CODE_T (*SPA_SetProtModeFunc)(INSTANCE_T inst,  const SW_SNCP_TP_ENABLE_STR_T *pEnableStr);

 

 
 
typedef UINT32 (*SPA_AddHWSncpToDrvFunc)(INSTANCE_T inst,const HW_3P_SNCP_CONFIG_T *pSncpGrp);

 
typedef UINT32 (*SPA_DelHWSncpToDrvFunc)(INSTANCE_T inst, UINT32 ulSncpGrpId, UINT16 usDestBus, UINT8 ucDestVc4, UINT32 eDelMethod);

 
 
typedef UINT32 (*SPA_ExtSwitchCmdToDrvFunc)(INSTANCE_T inst, UINT32 ulSncpGrpId, UINT8 eCmdType, UINT16 usDestBus, UINT8 ucDestVc4);

 
typedef UINT32 (*SPA_SetSncpHoldOffTimeToDrvFunc)(INSTANCE_T inst, UINT32 ulSncpGrpId, UINT16 usTimeCnt, UINT16 usDestBus, UINT8 ucDestVc4);

 
typedef UINT32 (*SPA_SetSncpWtrToDrvFunc)(INSTANCE_T inst, UINT32 ulSncpGrpId, UINT16 usSncpWtr, UINT16 usDestBus, UINT8 ucDestVc4);

 
typedef UINT32 (*SPA_SetSncpIntrusiveToDrvFunc)(INSTANCE_T inst, UINT32 ulSncpGrpId, SNCP_INTRUSIVE_T eNIMode, UINT16 usDestBus, UINT8 ucDestVc4);

 
typedef UINT32 (*SPA_SetSncpRvtModeFunc)(INSTANCE_T inst, UINT32 ulSncpGrpId, UINT8 eRvtMode, UINT16 usDestBus, UINT8 ucDestVc4);

 
typedef UINT32 (*SPA_ConfigSncpEndFunc)(void);

 
 
typedef UINT32 (*SPA_ReportSncpStateFunc)(const HW_SNCP_STATE_T *pSncpGrpState);  


 
typedef PB_ERROR_CODE_T (*SPA_SetHapsModeFunc)(INSTANCE_T inst, UINT32 ulPortType,UINT32 ulPort, TP_INDEX_T tpIndex, SNCP_INTRUSIVE_T  eNIMode, BOOL bEnable);
typedef UINT32 (*SPA_GetProtStatFromDrvFunc)(void);
 
 

 
extern void SPA_RegSetProtMode(SPA_SetProtModeFunc pFunc); 

 
extern void SPA_RegAddHWSncpToDrv(SPA_AddHWSncpToDrvFunc pFunc);
extern void SPA_RegDelHWSncpToDrv(SPA_DelHWSncpToDrvFunc pFunc);

 
extern void SPA_RegSetExCmdToDrv(SPA_ExtSwitchCmdToDrvFunc pFunc);
extern void SPA_RegSetHoldoffTimeToDrv(SPA_SetSncpHoldOffTimeToDrvFunc pFunc);
extern void SPA_RegSetWtrToDrv(SPA_SetSncpWtrToDrvFunc pFunc);
extern void SPA_RegSetIntrusiveToDrv(SPA_SetSncpIntrusiveToDrvFunc pFunc);
extern void SPA_RegSetRvtToDrv(SPA_SetSncpRvtModeFunc pFunc);

 
extern void SPA_RegConfigHWEnd(SPA_ConfigSncpEndFunc pFunc);

 
extern void SPA_RegSncpDpr(SPA_SncpDprFunc pFunc);

 
extern void SPA_RegSncpPoll(SPA_SncpPollFunc pFunc);

 
 
extern void SPA_RegSetHapsMode(SPA_SetHapsModeFunc pFunc); 
extern void SPA_RegGetProtStatFromDrv(SPA_GetProtStatFromDrvFunc pFunc);

typedef void (*SPC_BoardStateChangeFunc)(BOARD_EVENT_T ulEvent, SLOT_NUMBER_T usSlotNum, BOARD_STYLE_T ulCardType);

 
typedef PB_ERROR_CODE_T (*SPA_SetForcRrptAlmFunc)(INSTANCE_T inst);
extern void SPA_RegSetForceRrptAlm(SPA_SetForcRrptAlmFunc pFunc);

 
typedef int (*SPA_RegGetPhyDestSncpFunc)(void *pTP, UINT32 ulTpNum);
extern void SPA_RegGetPhyDestSncp(SPA_RegGetPhyDestSncpFunc pFunc);

 
 
typedef PB_ERROR_CODE_T (*SPA_SetLoSncpHoldOffTimeCommonValueToDrvFunc)(INSTANCE_T inst, UINT32 ulTimer1, UINT32 ulTimer2, UINT32 ulTimer3);
 
 
typedef PB_ERROR_CODE_T (*SPA_SetLoSncpWtrCommoneValueToDrvFunc)(INSTANCE_T inst, UINT32 ulTimer1, UINT32 ulTimer2, UINT32 ulTimer3);
  
 
extern void SPA_RegSetLoSncpHoldOffTimeCommonValueToDrv (SPA_SetLoSncpHoldOffTimeCommonValueToDrvFunc pFunc);
extern void SPA_RegSetLoSncpWtrCommoneValueToDrv (SPA_SetLoSncpWtrCommoneValueToDrvFunc pFunc);
 
 
 
typedef int (*funcCCSetSncpGroupAdd)(INSTANCE_T inst, HW_3P_SNCP_CONFIG_T *psSncpGrp);

typedef int (*funcCCSetSncpExtCmd)(INSTANCE_T inst, UINT32 ulSncpGrpId, UINT32 eCmdType, UINT16 usDestBus, UINT8 ucDestVc4);

typedef int (*funcCCSetSncpHoldOffTime)(INSTANCE_T inst, UINT32 ulSncpGrpId, UINT16 usTimeCnt, UINT16 usDestBus, UINT8 ucDestVc4) ;

typedef int (*funcCCSetSncpWtr)(INSTANCE_T inst, UINT32 ulSncpGrpId, UINT16 usSncpWtr, UINT16 usDestBus, UINT8 ucDestVc4) ;
typedef int (*funcCCSetSncpNIMode)(INSTANCE_T inst, UINT32 ulSncpGrpId, SNCP_INTRUSIVE_T eNIMode, UINT16 usDestBus, UINT8 ucDestVc4) ;

typedef int (*funcCCSetSncpRvtMode)(INSTANCE_T inst, UINT32 ulSncpGrpId, UINT32 eRvtMode, UINT16 usDestBus, UINT8 ucDestVc4) ;
typedef int (*funcCCSetSncpDelete)(INSTANCE_T inst, UINT32 ulSncpGrpId, UINT16 usDestBus, UINT8 ucDestVc4, UINT32 eDelMethod) ;
typedef int (*funcCCSetHoSwitch)(INSTANCE_T inst, UINT16 fromBus, UINT8 fromVc4, UINT16 toBus, UINT8 toVc4, BOOL bSwitch);
 
typedef int (*funcCCReplaceLoSncpW)(INSTANCE_T inst,UINT16 SncpGroupId, LO_MATRIX_ITEM_T srcXcPoint,BOOL bWork);
typedef int  (*funcCCSetLoSncpGlobalWtr)(INSTANCE_T inst, UINT32 ulWtrTimer1, UINT32 ulWtrTimer2, UINT32 ulWtrTimer3);
typedef int (*funcCCSetLoSncpGloablHoldoff)(INSTANCE_T inst, UINT32 ulTimer1, UINT32 ulTimer2, UINT32 ulTimer3);












# 24 "../../../appl/CTD/inc/ctd_drv.h" 2

# 1 "../../../appl/PRA/inc/pra_ei.h" 1
 




























typedef struct 
{
    command_tt head;
    int state;
} PRAApplInitState_t;

typedef struct
{
    PbSlotNumberType_Tval SlotNo;          
    UINT32 PortNo;              
    PbPortType_Tval            PortType;            


    TruthValue_Tval             PRBSEnable;        
    pbPRBSPattern_Tval       PRBSPattern;
    UINT32                          PRBSErrorBlocks;
    UINT32                          PRBSErrorSeconds;
    pbPRBSStatus_Tval         PRBSStatus;
    PbActualPortType_Tval        PRBSActualPortType;

    prbsMode_Tval   PRBSMode;
}PRA_PORTINFO_T;


 
PB_ERROR_CODE_T PRA_SetPortStatus (UINT8 slotNo,
    UINT8 port,
    UINT8 portType,
    pbPRBSStatus_Tval syncStatus);

typedef PB_ERROR_CODE_T (*PRA_SetPortStatus_Func)
    (UINT8 slotNo,
    UINT8 port,
    UINT8 portType,
    pbPRBSStatus_Tval syncStatus);

 
typedef PB_ERROR_CODE_T (*PRA_GetPortInfo_Func) 
    (INSTANCE_T inst, 
    UINT32 ulSlot, 
    HW_PORT_TYPE ePortType, 
    UINT32 ulPort, 
    UINT32 *pulPrbsCnt);

 
typedef PB_ERROR_CODE_T (*PRA_SetPortInfo_Func) 
    (INSTANCE_T inst, 
    UINT32 ulSlot, 
    HW_PORT_TYPE ePortType, 
    UINT32 ulPort, 
    pbPRBSPattern_Tval ePrbsMode, 
    BOOL bEnable);

void PRA_PollData(UINT32 counter);
void PRA_RegGetPortInfo(PRA_GetPortInfo_Func pFunc);
void PRA_RegSetPortInfo(PRA_SetPortInfo_Func pFunc);




 



























# 25 "../../../appl/CTD/inc/ctd_drv.h" 2

# 1 "../../../appl/CTD/inc/ctd_gfa.h" 1















 
typedef PB_ERROR_CODE_T (*GFA_CfgAlm2DrvFunc)(INSTANCE_T inst, BOOL bEnable);
extern void GFA_RegSetAlmCfg(GFA_CfgAlm2DrvFunc pFunc);

 

 
 
typedef PB_ERROR_CODE_T (*GFA_RprtAlarmDetect2GmplsFunc)(        UINT32          ulSlot,         HW_PORT_TYPE    tPortType,        HW_PORT_TYPE    tCurPortType,        UINT32          ulPort,        SDH_TP_TYPE     tpType,        COMPACT_TP_T    tpIndex,        UINT32          ulDefectId,        FMA_ALM_STATE   eState    );
# 34 "../../../appl/CTD/inc/ctd_gfa.h"

 
extern PB_ERROR_CODE_T GFA_AlarmDetect(
        UINT32          ulSlot, 
        HW_PORT_TYPE    ePortType,
        HW_PORT_TYPE    eCurPortType,
        UINT32          ulPort,
        SDH_TP_TYPE     tpType,
        COMPACT_TP_T    tpIndex,
        UINT32          ulDefectId,
        FMA_ALM_STATE   eState);

 
extern void GFM_BoardStateChange(BOARD_EVENT_T ulEvent, SLOT_NUMBER_T usSlotNum, BOARD_STYLE_T ulCardType);












 






























# 26 "../../../appl/CTD/inc/ctd_drv.h" 2


 
typedef unsigned char       BYTE;

typedef unsigned short      WORD;
typedef unsigned int        DWORD;


 



















 




 






 







 










 


typedef enum 
{
     
    CTD_MODE_STM1       = 0x0,
    CTD_MODE_STM1E      = 0x1,
    CTD_MODE_STM4       = 0x2,
    CTD_MODE_STM16      = 0x3,
    CTD_MODE_STM64      = 0x4,
    CTD_MODE_STM256     = 0x5,

     
    CTD_MODE_E1         = 0x6,
    CTD_MODE_DS1        = 0x7,
    CTD_MODE_E3         = 0x8,
    CTD_MODE_DS3        = 0x9,

     
    CTD_MODE_OTU2       = 0xa,
}CTD_MODE_TYPE_T;

 
typedef enum 
{
    CTD_FEC_MODE_RS     = 0x0,
    CTD_FEC_MODE_UFEC   = 0x1,
}CTD_FEC_MODE_TYPE_T;

 
typedef struct
{
    BOOL los;
    BOOL lof;
    BOOL oof;
    BOOL tim;
}CTD_RS_ALARM_T;

 
typedef struct
{
    BOOL ais;
    BOOL exc;
    BOOL deg;
    BOOL rdi;
}CTD_MS_ALARM_T;

 
typedef struct
{
    BOOL ais;
    BOOL lop;
}CTD_AU_ALARM_T;

 
typedef struct
{
    BOOL tim;
    BOOL plm;
    BOOL uneq;
    BOOL exc;
    BOOL deg;
    BOOL rdi;
    BOOL lom;
    BOOL ais;
}CTD_HP_ALARM_T;

 
typedef struct
{
    BOOL ais;
    BOOL lop;
}CTD_TU_ALARM_T;

 
typedef struct
{
    BOOL tim;
    BOOL plm;
    BOOL uneq;
    BOOL exc;
    BOOL deg;
    BOOL rdi;
    BOOL rfi;
}CTD_LP_ALARM_T;

 
typedef struct
{
    BOOL los;
    BOOL ais;
    BOOL lof;
    BOOL lom;
    BOOL rdi;
}CTD_PPI_ALARM_T;

 
typedef struct
{
    BOOL mcd;
    BOOL fcd;
    BOOL fma;
    BOOL osr;
    BOOL tcu;
}CTD_EQ_ALARM_T;

 
typedef struct
{
    BOOL los;
    BOOL ldf;
    BOOL oof;
    BOOL wca;
    BOOL cim;
}CTD_TR_ALARM_T;

 
typedef struct
{
    BOOL lfa;
    BOOL lma;
    BOOL tim;
    BOOL iae;
    BOOL bdi;
    BOOL ptm;
}CTD_OTU_ALARM_T;

 
typedef struct
{
    BOOL los;
    BOOL mfo;
    BOOL dfo;
}CTD_E1_ALARM_T;


 
typedef struct
{
    BOOL lof;
    BOOL tim;
    BOOL ais;
    BOOL exc;
    BOOL deg;
    BOOL rdi;
}CTD_MSP_ALARM_INT_T;

 
typedef struct
{
    BOOL lof;
    BOOL tim;
    BOOL ais;
    BOOL exc;
    BOOL deg;
    BOOL rdi;
}CTD_MSP_ALARM_DELTA_T;

 
typedef struct
{
    BOOL lof;
    BOOL tim;
    BOOL ais;
    BOOL exc;
    BOOL deg;
    BOOL rdi;
}CTD_MSP_ALARM_STATE_T;

 
typedef struct
{
    BOOL ais;
    BOOL lop;
    BOOL uneq;
    BOOL tim;
    BOOL vcais;
    BOOL exc;
    BOOL deg;
}CTD_SNCP_ALARM_INT_T;

 
typedef struct
{
    BOOL ais;
    BOOL lop;
    BOOL uneq;
    BOOL tim;
    BOOL vcais;
    BOOL exc;
    BOOL deg;
}CTD_SNCP_ALARM_DELTA_T;

 
typedef struct
{
    BOOL ais;
    BOOL lop;
    BOOL uneq;
    BOOL tim;
    BOOL vcais;
    BOOL exc;
    BOOL deg;
}CTD_SNCP_ALARM_STATE_T;

 
typedef enum
{
    CTD_INSERT_ALM_TYPE_MSAIS       =   0x0,
    CTD_INSERT_ALM_TYPE_AUAIS       =   0x1,
    CTD_INSERT_ALM_TYPE_TUAIS       =   0x2,
    CTD_INSERT_ALM_TYPE_AIS         =   0x3,
    CTD_INSERT_ALM_TYPE_UNEQ        =   0x4, 
    CTD_INSERT_ALM_TYPE_RDI         =   0x5,
}CTD_INSERT_ALM_TYPE_T;

 
typedef enum
{
    CTD_INSERT_ALM_IN_LASER         =   0x0,
    CTD_INSERT_ALM_OUT_LASER        =   0x1,
}CTD_INSERT_ALM_DIR_T;

 
typedef enum
{
    CTD_LOOPBACK_TYPE_NONE              =   0x0,
    CTD_LOOPBACK_TYPE_OUTWARD_NEAR      =   0x1,
    CTD_LOOPBACK_TYPE_INWARD_FAR        =   0x2,
    CTD_LOOPBACK_TYPE_INWARD_NEAR       =   0x3,
    CTD_LOOPBACK_TYPE_OUTWARD_FAR       =   0x4, 
    CTD_LOOPBACK_TYPE_OUTWARD_TSI       =   0x5,
}CTD_LOOPBACK_TYPE_T;

 
typedef struct 
{
     
    WORD                busNum;

    BYTE                portNum;
    WORD                augNum;
    CARDTYPE_T          type;    
}CTD_CARD_CONFIG_T;

 
typedef struct 
{
    BYTE  byPort;
    BOOL  bEnable;
}CTD_TIMING_CONFIG_T;

 
typedef PB_ERROR_CODE_T (*CPS_GetLatchState)(SLOT_NUMBER_T slot, LATCH_STATE_T eLatchState);
 
typedef PB_ERROR_CODE_T (*CAD_RprtUnitFail)(SLOT_NUMBER_T slot, BOOL bUnitFail);
    
 
typedef struct
{
     
    TMA_S1ChangeCallbackFunc        cRprtSsmToTMA;
    TMA_ModDefectFunc               cRprtDefectToTMA;
     
    FMA_AlmModDefectFunc            cRprtDefectToFMA;
    FMA_AlmModDefectByBusFunc       cRptDefectToFmaByBus;
     
    PMP_GetRsOofFunc                cRprtRsOofToPMA; 
     
    CPS_GetLatchState               cRprtLatchStateToCPA;
     
    CPS_GetLatchState               cRprtLatchStateToCAI;
     
    PRA_SetPortStatus_Func          cRprtPrbsStateToPRA;  
     
    MPA_Handle_RcvKbyte             cRprtMspKByte;
    MPA_RcvPhyDefect                cRprtMspLineCond;
    MPA_RprtMspStatus               cRprtMspStatus;
     
    MST_ChangeOfKb                  cRprtMsspKByte;
    MST_ChangeOfLineCond            cRprtMsspLineCond;
     
    MSA_ChangeKbyteBatch            cRprtMsspKByteBatch;
    MSA_ChangeLineCondBatch         cRprtMsspLineCondBatch;
     
    SPA_DrvIntHappenFunc            cRprtSpaDrvIntHappen; 
    SPA_ReportSncpStateFunc         cRprtSpaSncpState;
     
    TMH_InitCCFPGAFunc              cRprtCcInitToTmh;
    TMH_SwitchHOCCFunc              cRptCcStateToTmh;
     
    GFA_RprtAlarmDetect2GmplsFunc   cRprtDefectToGmpls;
     
    CAD_RprtUnitFail                cRprtUnitFailToCAD;
}CTD_CALLBACKS_T;

 
typedef enum
{
    CTD_MSG_TYPE_INT        = 0x0,
    CTD_MSG_TYPE_POLLING    = 0x1,
    CTD_MSG_TYPE_1HZINT     = 0x2,
    CTD_MSG_TYPE_LATCH      = 0x3,
}CTD_MSG_TYPE_T;

 
typedef struct
{
    ENTITY_T            etEntity;
    DWORD               dwPos;
    CTD_MSG_TYPE_T      eMsgType;
}CTD_MSG_DATA_T;

 
typedef enum
{
    CTD_SNCP_SIG_DIR_UNKNOWN    = 0x0,
    CTD_SNCP_SIG_DIR_SOURCE     = 0x1,
    CTD_SNCP_SIG_DIR_SINK       = 0x2,
    CTD_SNCP_SIG_DIR_BIDRECTION = 0x3,
}CTD_SNCP_SIG_DIR_T;

 
typedef enum
{
    CTD_SNCP_CMD_UNKNOWN        = 0x0,
    CTD_SNCP_CMD_CLEAR          = 0x1,
    CTD_SNCP_LOCK_OUT           = 0x2,
    CTD_SNCP_FORCE_SWITCH_TOP   = 0x3,
    CTD_SNCP_FORCE_SWITCH_TOW   = 0x4,
    CTD_SNCP_MANUAL_SWITCH_TOP  = 0x5,
    CTD_SNCP_MANUAL_SWITCH_TOW  = 0x6,
    CTD_SNCP_NO_COMMAND         = 0x7,
}CTD_SNCP_CMD_TYPE_T; 

 
typedef enum 
{
    CTD_SNCP_MODE_UNKNOWN       = 0x0,
    CTD_SNCP_MODE_REVERTIVE     = 0x1,
    CTD_SNCP_MODE_NONREVERTIVE  = 0x2,
} CTD_SNCP_MODE_T;

 
typedef enum 
{
    CTD_SNCP_REQ_CLEAR              = 0x1,
    CTD_SNCP_REQ_LOCKOUT            = 0x2,
    CTD_SNCP_REQ_FORCE_SWITCH_TOP   = 0x3,
    CTD_SNCP_REQ_FORCE_SWITCH_TOW   = 0x4,
    CTD_SNCP_REQ_SIGNAL_FAIL        = 0x5,
    CTD_SNCP_REQ_MANUAL_SWITCH_TOP  = 0x6,
    CTD_SNCP_REQ_MANUAL_SWITCH_TOW  = 0x7,
    CTD_SNCP_REQ_WAIT_TO_RESTORE    = 0x8,
    CTD_SNCP_REQ_NO_REQUEST         = 0x9,
    CTD_SNCP_REQ_SIGNAL_DEG         = 0xa,
} CTD_SNCP_REQ_TYPE_T;

 
typedef enum 
{
    CTD_SNCP_CMD_RSLT_SUCC              = 0x1,
    CTD_SNCP_CMD_RSLT_LOWPRIORITY       = 0x2,
    CTD_SNCP_CMD_RSLT_SWITCH_TO_ACTIVE  = 0x3,
    CTD_SNCP_CMD_SWITCH_FAIL            = 0x4,
    CTD_SNCP_CMD_SWITCH_TIMEOUT         = 0x5,
    CTD_SNCP_CMD_WAIT_CC_RSP            = 0x6,
    CTD_SNCP_BAD_COMMAND                = 0x7,
} CTD_SNCP_CMD_RSLT_T;

 
typedef enum 
{
    CTD_SNCP_STATE_ACTIVE  = 0x1,
    CTD_SNCP_STATE_STANDBY = 0x2,
} CTD_SNCP_CHL_STATE_T;

 
typedef enum 
{
    CTD_SNCP_RSN_CLEAR_CMD         = 0x1,
    CTD_SNCP_RSN_LP_CMD            = 0x2,
    CTD_SNCP_RSN_FORCED_SWITCH     = 0x3,
    CTD_SNCP_RSN_MANUAL_SWITCH     = 0x4,
    CTD_SNCP_RSN_SIGNAL_FAIL       = 0x5,
    CTD_SNCP_RSN_SIGNAL_FAIL_CLEAR = 0x6,
    CTD_SNCP_RSN_NO_SWITCH         = 0x7,
    CTD_SNCP_RSN_SIGNAL_DEG        = 0x8,
    CTD_SNCP_RSN_SIGNAL_DEG_CLEAR  = 0x9,
} CTD_SNCP_SWITCH_RSN_T;

 
typedef struct
{
     
    BOOL                bResult;
     
    SNCP_SIGNAL_STATE_T eWorkState;
    SNCP_SIGNAL_STATE_T eProtState;
     
    CTD_SNCP_REQ_TYPE_T eCmdReq;
}CTD_SNCP_STATE_T;


# 22 "../../../appl/CAH/inc/cah_defs.h" 2


 




 

 


 

 

 

 

 

 


 


 

 


typedef enum
{
    CAH_DEBUG_LEVEL_ERROR       =   0,
    CAH_DEBUG_LEVEL_CFGSET      =   1,
    CAH_DEBUG_LEVEL_CFGGET      =   2,
    CAH_DEBUG_LEVEL_INIT        =   3,
    CAH_DEBUG_LEVEL_INT         =   4,
    CAH_DEBUG_LEVEL_OP          =   5,
    CAH_DEBUG_LEVEL_PMON        =   6,
    CAH_DEBUG_LEVEL_CLOCK       =   7,
    CAH_DEBUG_LEVEL_SNCP        =   8,
    CAH_DEBUG_LEVEL_MSP         =   9,
    CAH_DEBUG_LEVEL_MSSP        =   10,
    CAH_DEBUG_LEVEL_ALARM       =   11
}CAH_DBG_LEVEL_T;





 





 





 
typedef enum
{
    CAH_REG_ATTRBT_RW       =   0x0,
    CAH_REG_ATTRBT_RO       =   0x1,
    CAH_REG_ATTRBT_WO       =   0x2,
    CAH_REG_ATTRBT_RC       =   0x3,
    CAH_REG_ATTRBT_WC       =   0x4,
}CAH_REG_ATTRBT_T;

 
typedef struct
{
    DWORD             dwBaseOffset;
    BYTE              bySimRegNum;
    DWORD             dwSimRegInterval;
    WORD              wValue;
    WORD              wMask;
    CAH_REG_ATTRBT_T  eRegAttrbt;
    WORD              wDelayTime;
}CAH_INIT_TABLE_T;

 
typedef struct
{
    DWORD     dwBaseOffset;
    BYTE      bySimRegNum;
    DWORD     dwSimRegInterval;
    WORD      wValue;
    WORD      wMask;
}CAH_STATE_CHECK_TABLE_T;

 
typedef struct
{
    DWORD  dwPeriodMsSet;
    DWORD  dwPeriodMsClear;
    WORD   wThreshSet;
    WORD   wThreshClear;
}CAH_B2_THRESHOLD_T;

 
typedef struct
{
    ENTITY_T             dwEntityNo;
    DWORD                dwBaseAddr;
    CTD_MODE_TYPE_T      ePortType[0x7e ];
}CAH_CFG_INFO_T;

 
extern  WORD cah_WriteRegister(DWORD dwBaseAddr, DWORD dwOffset, WORD wValue, WORD wMask, CAH_REG_ATTRBT_T eRegAttrbt);
extern  WORD cah_ReadRegister(DWORD dwBaseAddr, DWORD dwOffset, WORD* pwValue, WORD wMask, CAH_REG_ATTRBT_T eRegAttrbt);
extern  WORD cah_PollBusyBit(DWORD dwBaseAddr, DWORD dwOffset);
extern  WORD cah_IndWrite(DWORD dwBaseAddr, DWORD dwAddrIndAddr, DWORD dwAddrIndData, WORD wAddr, WORD wValue, WORD wMaskval, WORD wMaskaddr);
extern  WORD cah_IndRead(DWORD dwBaseAddr, DWORD dwAddrIndAddr, DWORD dwAddrIndData, WORD wAddr, WORD *pwValue, WORD wMaskval, WORD wMaskaddr);
extern  WORD cah_IndWriteQuick(DWORD dwBaseAddr, DWORD dwAddrIndAddr, DWORD dwAddrIndData, WORD wAddr, WORD wValue, WORD wMaskval, WORD wMaskaddr);
extern  WORD cah_IndReadQuick(DWORD dwBaseAddr, DWORD dwAddrIndAddr, DWORD dwAddrIndData, WORD wAddr, WORD *pwValue, WORD wMaskval, WORD wMaskaddr);
extern  WORD cah_Write8Reg(DWORD dwBaseAddr, DWORD dwOffset, BYTE byValue, BYTE byMask, CAH_REG_ATTRBT_T eRegAttrbt);
extern  WORD cah_Read8Reg(DWORD dwBaseAddr, DWORD dwOffset, BYTE *pbyValue, BYTE byMask, CAH_REG_ATTRBT_T eRegAttrbt);
extern  WORD cah_Write32Reg(DWORD dwBaseAddr, DWORD dwOffset, DWORD dwValue, DWORD dwMask, CAH_REG_ATTRBT_T eRegAttrbt);
extern  WORD cah_Read32Reg(DWORD dwBaseAddr, DWORD dwOffset, DWORD *pdwValue, DWORD dwMask, CAH_REG_ATTRBT_T eRegAttrbt);
extern  WORD cah_Stm4TpConvert(CTD_MODE_TYPE_T ePortType, COMPACT_TP_T tpId, BYTE *byAug1No, BYTE *byAu3No);
extern  WORD cah_Stm16TpConvert(CTD_MODE_TYPE_T ePortType, COMPACT_TP_T tpId, BYTE *byAug4No, BYTE *byAug1No, BYTE *byAu3No);
extern  WORD cah_Stm64TpConvert(CTD_MODE_TYPE_T ePortType, COMPACT_TP_T tpId, BYTE *byAug16No, BYTE *byAug4No, BYTE *byAug1No, BYTE *byAu3No);
extern  WORD cah_Stm256TpConvert(CTD_MODE_TYPE_T ePortType, COMPACT_TP_T tpId, BYTE *byAug64No, BYTE *byAug16No, BYTE *byAug4No, BYTE *byAug1No, BYTE *byAu3No);
extern  DWORD CAH_GetCardBaseAddr(CARDTYPE_T eCardType);

 
extern  DBG_MODULE_T       gCahDbgModuleNo;
extern  CAH_B2_THRESHOLD_T gCahStm1B2Threshold[0x9  - 0x3  + 1];
extern  CAH_B2_THRESHOLD_T gCahStm4B2Threshold[0x9  - 0x3  + 1];
extern  CAH_B2_THRESHOLD_T gCahStm16B2Threshold[0x9  - 0x3  + 1];
extern  CAH_B2_THRESHOLD_T gCahStm64B2Threshold[0x9  - 0x3  + 1];
extern  CAH_B2_THRESHOLD_T gCahStm256B2Threshold[0x9  - 0x3  + 1];

 
extern  int sysClkRateGet(void);
extern  STATUS taskDelay(int ticks);
extern  UINT32 sysTimestampFreq(void);




 




















# 16 "cah_126e1.c" 2


# 1 "../../../appl/SDD/inc/sdd_ei.h" 1
 





















 
 

# 1 "../../../infra/SYS/target/h/time.h" 1
 

 

 


























# 1 "../../../infra/SYS/target/h/sigevent.h" 1
 

 

 
















union sigval
    {
    int			sival_int;
    void		*sival_ptr;
    };

struct sigevent
    {
    int			sigev_signo;
    union sigval	sigev_value;
    int			sigev_notify;
    };






# 32 "../../../infra/SYS/target/h/time.h" 2




# 46 "../../../infra/SYS/target/h/time.h"



typedef struct __timer *timer_t ;




typedef unsigned int clock_t ;








typedef int clockid_t;





struct tm
	{
	int tm_sec;	 
	int tm_min;	 
	int tm_hour;	 
	int tm_mday;	 
	int tm_mon;	 
	int tm_year;	 
	int tm_wday;	 
	int tm_yday;	 
	int tm_isdst;	 
	};

struct timespec
    {
    					 
    time_t tv_sec;			 
    long tv_nsec;			 
    };

struct itimerspec
    {
    struct timespec it_interval;	 
    struct timespec it_value;		 
    };






 



extern uint_t      _clocks_per_sec(void);
extern char *	   asctime (const struct tm *_tptr);
extern clock_t	   clock (void);
extern char *	   ctime (const time_t *_cal);
extern double	   difftime (time_t _t1, time_t _t0);
extern struct tm * gmtime (const time_t *_tod);
extern struct tm * localtime (const time_t *_tod);
extern time_t	   mktime (struct tm *_tptr);
extern size_t	   strftime (char *_s, size_t _n, const char *_format,
		   	      const struct tm *_tptr);
extern time_t	   time (time_t *_tod);

extern int 	clock_gettime (clockid_t clock_id, struct timespec *tp);
extern int 	clock_settime (clockid_t clock_id, const struct timespec *tp);
extern int 	clock_getres (clockid_t clock_id, struct timespec *res);

extern int 	timer_create (clockid_t clock_id, struct sigevent *evp,
			      timer_t *ptimer);
extern int 	timer_delete (timer_t timerid);
extern int 	timer_gettime (timer_t timerid, struct itimerspec *value);
extern int 	timer_settime (timer_t timerid, int flags,
		               const struct itimerspec *value,
			       struct itimerspec *ovalue);
extern int 	timer_getoverrun (timer_t timerid);

extern int      timer_connect (timer_t timerid, VOIDFUNCPTR routine, int arg);
extern int      timer_cancel (timer_t timerid);
extern int      timer_show (timer_t timerid);

extern int 	nanosleep (const struct timespec *rqtp, struct timespec *rmtp);




extern int	   asctime_r(const struct tm *_tm, char *_buffer,
			     size_t *_buflen);
extern char *	   ctime_r (const time_t *_cal, char *_buffer, size_t *_buflen);
extern int	   gmtime_r (const time_t *_tod, struct tm *_result);
extern int	   localtime_r (const time_t *_tod, struct tm *_result);



# 184 "../../../infra/SYS/target/h/time.h"







# 26 "../../../appl/SDD/inc/sdd_ei.h" 2

# 1 "../../../infra/inc/I2cdrv.h" 1





# 1 "../../../infra/inc/bspType.h" 1

# 23 "../../../infra/inc/bspType.h"




# 6 "../../../infra/inc/I2cdrv.h" 2








 














 





 




































 






 









 







 









 



 









                                

























 














































 















































 

















































































                                    






































































































































 


extern int i2c_debug;





	
 







typedef struct {
    int addr;
	int maxChannelNum;
} I2C_MUX_ADDR_TBL_T;



typedef enum {
	I2C_ACCESS_OK    = 0,
	I2C_BUS_ERROR    = 0x1000,      
	I2C_BUS_TIMEOUT  = 0x2000,    
	I2C_ERROR_UNKNOWN= 0x4000
} I2C_ERROR_TYPE;     

typedef struct {
    int          valid;        
	int          taskId;       
	const char * pTaskName;    
    const char * pCaller;      
    const char * pCallerFile; 
    int          callerLine;  
    int          muxAddr;      
    int          channel;      
} I2C_BUS_CHANNEL_STATUS;

typedef struct {
    int               valid;              
    int               taskId;             
    const char *      pTaskName;          
    int               devAddr;            
    const char *      pCaller;            
    const char *      pCallerFile;        
    int               callerLine;         
    struct tm        currTime;           
    I2C_ERROR_TYPE    reason;             
	                                       
	                                       
										  
    int               times;              
} I2C_BUS_ERR_RECORD;  

typedef struct {
    int               isSemInUse;          
    SEM_ID            selfID;              
    int               taskId;              
    const char *      pTaskName;           
    const char *      pSemOwner;           
    const char *      pSemOwnerFile;      
    int               semOwnerLine;       
    int               semForMuxAddr;      
    int               semTimeOut;          
    int               semCounter;          
} I2C_SEM_RECORD;        

# 542 "../../../infra/inc/I2cdrv.h"




typedef struct {
    int          valid;            
	int          taskId;           
	const char * pTaskName;       
} I2C_SEM_OVERTIME_RECORD_ITEM;   

typedef struct {
	int          nextItem;           
	I2C_SEM_OVERTIME_RECORD_ITEM item[10 ];    
} I2C_SEM_OVERTIME_RECORD;   



typedef enum {
	I2C_RAW,
	I2C_RAW_BASE,
	I2C_BASE,
} I2C_OPTION;        

typedef struct {
    void *tlvDataPtr;
    char deviceName[32];
} TLV_data_t;


















	



 









        
 
 







       
 






















 
int i2cDrvSemTake(const char*pCaller, const char* pCallerFile, 
        const int callerLine, int muxAddr, int wait);
int i2cDrvSemGive(void);

int tlvDrvSemTake(const char*pCaller, const char* pCallerFile, 
        const int callerLine, int wait);
int tlvDrvSemGive(void);

int i2cGeneralRead(const char* pCaller, const char* pCallerFile,
        int callerLine, int fd, int offset, int address, char *buffer, int numbytes, int *status, I2C_OPTION option);

int i2cGeneralWrite(const char* pCaller, const char* pCallerFile,
        int callerLine, int fd, int offset, int address, char *buffer, int numbytes, int *status, I2C_OPTION option);

void i2cBusChannelStatusClear(void);
void i2cBusChannelStatusSet(const char* pCaller, const char* pCallerFile, int callerLine, 
	int muxAddr, int channel);

void i2cTrace(int level, char* mname, int line, char*fname, char* fmt, ...);

void i2cCloseDrv(int value);  

int i2cCreate(char *name);
int i2cDrv(void);
void i2cFDClose(int fd);





# 27 "../../../appl/SDD/inc/sdd_ei.h" 2

# 1 "../../../infra/inc/bspMem.h" 1
 















 




































# 1 "../../../infra/inc/bspType.h" 1

# 23 "../../../infra/inc/bspType.h"




# 54 "../../../infra/inc/bspMem.h" 2






 


 



# 192 "../../../infra/inc/bspMem.h"


 


 



 


 














 
 


 




# 243 "../../../infra/inc/bspMem.h"


 







































# 28 "../../../appl/SDD/inc/sdd_ei.h" 2


# 1 "../../../appl/FMA/inc/fma_ei.h" 1

























 


























extern UINT32 CRC32(UINT32 LastBufCRC,const UINT8 * pBuf,UINT32 BufLength);


 


 

typedef enum
{
    FM_DEFECT_BIT_TYPE_UNKNOW       = 0,
        
    FM_DEFECT_BIT_TYPE_STM1_LC      = 1,
    FM_DEFECT_BIT_TYPE_STM1_PM      = 2,
    FM_DEFECT_BIT_TYPE_STM1_LOCC    = 3,
    FM_DEFECT_BIT_TYPE_STM4_LC      = 4,
    FM_DEFECT_BIT_TYPE_STM4_PM      = 5,
    FM_DEFECT_BIT_TYPE_STM4_LOCC    = 6,
    FM_DEFECT_BIT_TYPE_STM16_LC     = 7,
    FM_DEFECT_BIT_TYPE_STM16_PM     = 8,
    FM_DEFECT_BIT_TYPE_STM16_LOCC   = 9,
    FM_DEFECT_BIT_TYPE_STM64_LC     = 10,
    FM_DEFECT_BIT_TYPE_STM64_PM     = 11,
    FM_DEFECT_BIT_TYPE_STM64_LOCC   = 12,
    FM_DEFECT_BIT_TYPE_STM256_LC    = 13,
    FM_DEFECT_BIT_TYPE_STM256_PM    = 14,
    FM_DEFECT_BIT_TYPE_STM256_LOCC  = 15,
    
    FM_DEFECT_BIT_TYPE_E1           = 16,
    FM_DEFECT_BIT_TYPE_E1_PM        = 17,
    FM_DEFECT_BIT_TYPE_E3DS3        = 18,
    FM_DEFECT_BIT_TYPE_E3DS3_PM     = 19,
    
    FM_DEFECT_BIT_TYPE_LAN          = 20,
    FM_DEFECT_BIT_TYPE_VWAN         = 21,
    FM_DEFECT_BIT_TYPE_RPR          = 22,
    FM_DEFECT_BIT_TYPE_VSTM1        = 23,
    FM_DEFECT_BIT_TYPE_VSTM1_PM     = 24,
    FM_DEFECT_BIT_TYPE_VSTM4        = 25,
    FM_DEFECT_BIT_TYPE_VSTM4_PM     = 26,
    FM_DEFECT_BIT_TYPE_VSTM16       = 27,
    FM_DEFECT_BIT_TYPE_VSTM16_PM    = 28,
    FM_DEFECT_BIT_TYPE_VSTM64       = 29,
    FM_DEFECT_BIT_TYPE_VSTM64_PM    = 30,
    
    FM_DEFECT_BIT_TYPE_OA           = 31,
    
    FM_DEFECT_BIT_TYPE_SLOT         = 32,
    FM_DEFECT_BIT_TYPE_NE           = 33,

    FM_DEFECT_BIT_TYPE_OTU2         = 34
}FM_DEFECT_BIT_TYPE_T;


typedef struct 
{
    BOOL empty;
    FM_DEFECT_BIT_TYPE_T  defectType;
    unsigned int  entity;
    unsigned int  instance;
    unsigned int  bitPos;
    unsigned int  index;
    unsigned char  state;            
} __attribute__ ((packed))  FMA_DEFECT_BIT_UNIT_T;


typedef struct
{
    unsigned int index;
    FMA_DEFECT_BIT_UNIT_T *pBitUnits;
    int number;
    void *pNext;
} __attribute__ ((packed))  FMA_SENT_BIT_MSG;

typedef struct
{
   int length;
   void *pNext;
}__attribute__ ((packed))  FMA_SENT_BIT_MSG_HEAD;

typedef struct 
{
    command_tt head;
    unsigned int index;
    unsigned int defectBitNumber;

    unsigned int crc32;

    FMA_DEFECT_BIT_UNIT_T *pDefectTable;    
} FMA_FMC_DEFECT_BIT_REPORT_CMD_T;










 

  


# 170 "../../../appl/FMA/inc/fma_ei.h"


typedef enum
{
    FM_GENERAL_TYPE_UNKOWN  = 0,
    FM_GENERAL_TYPE_SDH_LC  = 1,
    FM_GENERAL_TYPE_PDH     = 2,
    FM_GENERAL_TYPE_DATA    = 3,
    FM_GENERAL_TYPE_CC      = 4,
    FM_GENERAL_TYPE_SC      = 5,
    FM_GENERAL_TYPE_OA      = 6,
} FM_GENERAL_TYPE_T;


 






# 201 "../../../appl/FMA/inc/fma_ei.h"


typedef enum
{
    FMA_LOCATION_UNKNOW    = 0,
    FMA_LOCATION_LC        = 1,
    FMA_LOCATION_CC        = 2,
    FMA_LOCATION_SC        = 3,
    FMA_LOCATION_SI        = 4,   
} FMA_CPU_LOCATION_T;


typedef enum
{
    FMA_START_UP_UNKNOW            = 0,
    FMA_START_UP_AGENT_BOOT_UP     = 1,
    FMA_START_UP_SC_BOOT_UP        = 2,
} FMA_START_UP_INFO_T;

typedef struct
{
    command_tt head;
    UINT32 slot;                   
    UINT32 cardStyle;
    UINT32 cardOption;
    FMA_START_UP_INFO_T agentInform;
    UINT8 cpuLocation;         
    UINT8 type;                     
} FMA_FMC_AGENT_START_UP_CMD_T;

 
typedef struct
{
    command_tt head;
    UINT32 agentSlotId;
    UINT32 option;
} FMA_FMC_QUERY_ALARM_PERSISENT_TIME_CMD_T;

 
typedef struct
{
    command_tt head;
    UINT32 slotNo;
    UINT32 portNo;
    UINT32 portType;
} FMA_FMC_LOS_STATE_CHANGE_CMD_T;

 
typedef struct
{
    command_tt head;
    UINT32 queryId;    
} FMA_FMC_QUERY_STATE_CMD_T;

 
typedef FMA_FMC_QUERY_STATE_CMD_T FMC_FMA_QUERY_STATE_ACK_CMD_T;


 
typedef struct
{
    command_tt head;
     
} FMA_FMM_CUTOFF_STATE_FROM_BUTTON_CMD_T;

 
typedef struct
{
    command_tt head;
    BOOL state;
} FMA_FMM_SUPPRESS_STATE_FROM_BUTTON_CMD_T;

 
typedef struct
{
    command_tt head;
    UINT32 slotId;
    UINT32 portNo;
    UINT32 defectId;
    FMA_ALM_STATE state;
} PMA_FMA_LAN_DEFECT_REPORT_CMD_T;

 
typedef struct
{
	command_tt head;
	UINT32 slotID;
}FMA_FMC_LOCC_CHANGE_CMD_T;


 
typedef struct
{
    command_tt head;
    UINT32 slotId;
    UINT32 portNo;
    UINT32 defectId;
    FMA_ALM_STATE state;
} FMA_FMA_FAN_DEFECT_REPORT_CMD_T;





 





























typedef struct
{
    UINT32 slotNo;                
    
    INSTANCE_T instance;          
                               
    UINT32 portNo;                
                                  
    HW_PORT_TYPE portType;        
  
    SDH_TP_TYPE tpType;           

    COMPACT_TP_T tpIndex;         


    UINT32 defectId;              

    FMA_ALM_STATE state;          

    UINT8 deep;                   
    
} FM_SET_DEFECT_INFRMATION_T;

 







typedef enum
{
    FM_DEFECT_TYPE_NONE    = 0, 
    FM_DEFECT_TYPE_LC      = 1,
    FM_DEFECT_TYPE_LOCC    = 2,
    FM_DEFECT_TYPE_DATA    = 3,
    FM_DEFECT_TYPE_PM      = 4,         
} FM_DEFECT_GROUP_TYPE_T;

extern  PB_ERROR_CODE_T FMA_SetDefectByBus_Callback(UINT16 busNo, UINT16 vc4No, TU_TP_T tpIndex, SDH_TP_TYPE tpType, UINT32 defectId, FMA_ALM_STATE state);
extern  PB_ERROR_CODE_T FMA_AddBoardEntity(UINT32 slot, UINT32 exist, UINT32 style, UINT32 option);
extern  PB_ERROR_CODE_T FMA_SetDefect_Callback(UINT32 SlotId, INSTANCE_T Instance, UINT32 PortId, HW_PORT_TYPE PortType, SDH_TP_TYPE TpType, COMPACT_TP_T TpIndex, UINT32 DefectId, FMA_ALM_STATE state);
extern void FMA_HandleButtonEvent(SDD_BUTTON_EVENT_TYPE buttonType, BUTTON_STATE_T oldState, BUTTON_STATE_T newState);
extern void FMA_TCL_CreateCommand(Tcl_Interp *interp);










# 30 "../../../appl/SDD/inc/sdd_ei.h" 2

# 1 "../../../appl/CTD/inc/ctd_cc.h" 1
 



















# 1 "../../../appl/gco/gcb/inc/Gcb_ei.h" 1

 
 
 
 
 
 
 
 
 
 





















 
 
 
 
 






 



 
 
 
 
 
 
 
 












 
 
 
 
 
typedef enum 
{   
    non_GMPLS   = 0x0,
    I_NNI       = 0x1,
    E_NNI       = 0x2,
    UNI10       = 0x3,
    UNI20       = 0x4,
}PB_GMPLS_PORT_CLASSIFIER;


typedef enum 
{   
    unreserved  = 0x0,
    migrating   = 0x1,
    reserved    = 0x2,
} PB_TP_RESERVATION;


typedef struct 
{
    UINT16      slotNo;
    UINT16      portType;        
    							                            
    UINT16      portNo;
    TP_INDEX_T  tpIndex;         
}PB_GMPLS_TP_T;


typedef struct 
{
    UINT16       vcgXVType;      
                                 
                                 
                                 
    UINT16       vcgLCASSupport; 
                                 
    UINT16       vcgGfpType;     
                                                                 
    char         vcgChannelId[50];   
 } PB_GMPLS_VCG_CONFIG_T;


typedef struct 
{
    PB_GMPLS_TP_T       fromTP;
    PB_GMPLS_TP_T       toTP;
	int		GmplsVcNumber;   
	char	GmplsCallName[255];    
    PbGmplsCcDirection_Tval GmplsCcDirection; 
} PB_GMPLS_XC_T;

typedef enum 
{
	PB_GMPLS_XC_CREATION = 0x1,
	PB_GMPLS_XC_DELETION = 0x2,
}PB_GMPLS_XC_OPERATION;

typedef enum 
{
	PB_GMPLS_XC_NORMAL_CONFIG = 0x1,
	PB_GMPLS_XC_FAST_CONFIG = 0x2,
}PB_GMPLS_XC_CONFIG_MODE;

 typedef struct 
{
    PB_GMPLS_TP_T       workingTP;
    PB_GMPLS_TP_T       protectingTP;
    PB_GMPLS_TP_T       srcOrDestTP;
    UINT16              direction;    
                                      
} PB_GMPLS_SNCP_T;

typedef enum 
{
	PB_GMPLS_SNCP_CREATION_OR_CC_TO_SNCP_CONVERTION = 0x1,
	PB_GMPLS_SNCP_DELETION = 0x2,
	PB_GMPLS_SNCP_TO_CC = 0x3,
	PB_GMPLS_SNCP_EXTERNAL_SWITCHING = 0x4,
}PB_GMPLS_SNCP_OPERATION;	

typedef enum 
{
	PB_GMPLS_SNCP_WORKING_TO_CC = 0x1,
	PB_GMPLS_SNCP_PROTECTION_TO_CC = 0x2,
}PB_GMPLS_SNCP_TO_CC_CONVERTION;	

typedef enum 
{
    PB_GMPLS_SNCP_CLEAR = 0x1,
    PB_GMPLS_SNCP_MANUAL_TO_PROTECTING =0x2,
    PB_GMPLS_SNCP_MANUAL_TO_WORKING = 0x3,
    PB_GMPLS_SNCP_FORCED_TO_PROTECTING = 0x4,
    PB_GMPLS_SNCP_LOCKOUT = 0x5,
} PB_GMPLS_SNCP_SWITCHING_COMMAND;
	
typedef enum 
{
    PB_GMPLS_SNCP_ACTIVE = 0x1,
    PB_GMPLS_SNCP_STANDBY = 0x2,
} PB_GMPLS_SNCP_STATUS;

 typedef struct 
{
    UINT16      slotNo;
    UINT16      portNo;
} PB_GMPLS_MSP_PORT_T;


typedef enum 
{
    PB_GMPLS_LOF         = 0x1,
    PB_GMPLS_TIM         = 0x2,
    PB_GMPLS_RSEXE      = 0x3,
    PB_GMPLS_RSDEG      = 0x4,
    PB_GMPLS_MSAIS      = 0x5,
    PB_GMPLS_MSEXE      = 0x6,
    PB_GMPLS_MSDEG      = 0x7,
    PB_GMPLS_MSRDI      = 0x8,
} PB_GMPLS_ALARM;

 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
PB_ERROR_CODE_T CBM_SDH_PORT_CLASSIFIER_CHANGE_CHECK(UINT16       slotNo,
                                           UINT16       portType,
                                           UINT16       portNo,
                                           PB_GMPLS_PORT_CLASSIFIER newPortClassifier);


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
PB_ERROR_CODE_T CBM_TP_RESERVATION_CHANGE_CHECK( UINT16   slotNo,
                                       UINT16   portType,
                                       UINT16   portNo,
                                       UINT16   action,
                                       UINT16   tpNum,
                                       UINT16   *tp);


 
 
 
 
 
 
gmplsLicenseStatus_Tval CBM_CheckGmplsStatus();


 
 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt    head;
    UINT16        slotNo;
    UINT16        portType;
    UINT16        realPortType;
    UINT16        portNo;
    UINT16        mapString_len;
    char          portMappingString[384];
}CMND_IFM_CBM_PORT_MAPPING_CHANGE_IND_T;


 
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
PB_ERROR_CODE_T CBM_ETH_PORT_CLASSIFIER_CHANGE_CHECK( UINT16      slotNo,
                                            UINT16      portType,
                                            UINT16      portNo,
                                            PB_GMPLS_PORT_CLASSIFIER    newPortClassifier);


 
 
 
 
 
 
                                                                             
 
 
 
 
 
 
 

PB_ERROR_CODE_T CBM_VCG_CONFIGURATION_CHANGE_CHECK( UINT16        slotNo,
                                          UINT16        vcgNo,
                                          PB_GMPLS_VCG_CONFIG_T         vcgConfiguration);

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

PB_ERROR_CODE_T CBM_VCG_CAPACITY_CHANGE_CHECK( UINT16             slotNo,
                                     PB_GMPLS_TP_T      tp,
                                     UINT16             vcgNo,
                                     UINT16             direction,
                                     UINT16             action);



 
 
 
 
 


 
 
 
 
 
 
 
 
 
 
 
 
 
 

PB_ERROR_CODE_T CBM_XC_CHANGE_CHECK( PB_GMPLS_XC_OPERATION        action,
                           PB_GMPLS_XC_T                xc);


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt    head;
    UINT32        correlator;
    UINT16        configMode;   
    UINT16        deletedXcNum;
    UINT16        createdXcNum;
    PB_GMPLS_XC_T modifiedXc[1];
} CMND_CBM_CCS_MODIFY_XC_REQ_T;

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt    head;
    UINT32        correlator;
    UINT16        deletedXcNum;
    UINT16        createdXcNum;
    UINT16        result[1];   
} CMND_CCS_CBM_MODIFY_XC_ACK_T;

 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt      head;
    UINT16          action;  
    UINT16          result;  
    PB_GMPLS_XC_T   xc;
}CMND_CCS_CBM_XC_CHANGE_IND_T;


 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

PB_ERROR_CODE_T CBM_SNCP_CHANGE_CHECK(PB_GMPLS_SNCP_OPERATION        action,
                            PB_GMPLS_SNCP_TO_CC_CONVERTION workOrProtToCc,
                            PB_GMPLS_SNCP_T                sncp);

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt                       head;
    UINT32                           correlator;
    UINT16                           action;          
    UINT16                           workOrProtToCc;  
    UINT16                           sncpNum;
    PB_GMPLS_SNCP_T                  modifiedSncp[1];
} CMND_CBM_SPM_MODIFY_SNCP_REQ_T;

 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt    head;
    UINT32        correlator;
    UINT16        sncpNum;
    UINT16        result[1];  
} CMND_SPM_CBM_MODIFY_SNCP_ACK_T;

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt                       head;
    UINT32                           correlator;
    UINT16                           action;     
    UINT16                           sncpNum;
    PB_GMPLS_SNCP_T                  switchedSncp[1];
} CMND_CBM_SPM_SWITCH_SNCP_REQ_T;


 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt    head;
    UINT32        correlator;
    UINT16        sncpNum; 
    UINT16        result[1];  
} CMND_SPM_CBM_SWITCH_SNCP_ACK_T;

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
        command_tt              head;
        UINT16                  action;  
        UINT16                  workOrProtToCc;  
        PB_GMPLS_SNCP_T         sncp;
        UINT16                  result;  
}CMND_SPM_CBM_SNCP_CHANGE_IND_T;

 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt            head;
    UINT16                           action;    
     
    UINT16                result; 
     
    PB_GMPLS_SNCP_T       sncp;
} CMND_SPM_CBM_SNCP_SWITCH_IND_T;

 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt            head;
    PB_GMPLS_SNCP_T       sncp;
    UINT16                currentStatus;  
} CMND_SPA_CBM_SNCP_SWITCHING_IND_T;


 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

PB_ERROR_CODE_T CBM_MSP_CREATION_CHECK(UINT16                     mspScheme,
                             UINT16                     portType,
                             PB_GMPLS_MSP_PORT_T        port);

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt          head; 
    UINT16              action;
    UINT16              mspScheme;
    UINT16              portType;
    UINT16              workingPortNum;
    PB_GMPLS_MSP_PORT_T protectingPort;
    PB_GMPLS_MSP_PORT_T workingPort[14];
} CMND_MPM_CBM_MSP_CHANGE_IND_T;


 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
PB_ERROR_CODE_T CBM_MSSPRING_CREATION_CHECK(UINT16                protectionScheme,
                                  UINT16                portType,
                                  PB_GMPLS_MSP_PORT_T   port);

 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt          head;
    UINT16              action; 
    UINT16              protectionScheme;
    UINT16              portType;
    PB_GMPLS_MSP_PORT_T eastPort;
    PB_GMPLS_MSP_PORT_T eastProtectionPort;
    PB_GMPLS_MSP_PORT_T westPort;
    PB_GMPLS_MSP_PORT_T westProtectingPort;
}CMND_MSM_CBM_MSSPRING_CHANGE_IND_T;


 
 
 
 
 


 
 
 
 
 

 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt    head;
    UINT16        action;
} CMND_MFM_CBM_MIB_SAVING_ADMISSION_IND_T;
 
 
 
 
 

 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt    head;
    UINT16        gmplsStatus;
} CMND_CBM_SNMPAGENT_GMPLS_ACTIVE_IND_T;


 
 
 
 
 

 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt   head;
    UINT16       slotNo;
    UINT16       trigger;
} CMND_CBM_GFA_ALARM_IND_TRIGGER_T;


 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
void CBM_SEND_ALARM_IND_TO_GMPLS(UINT16              slotNo,
                                 UINT16              portType,
                                 UINT16              portNo,
                                 UINT16              cardType,
                                 PB_GMPLS_ALARM      alarmType,
                                 UINT16              alarmStatus);
                                 
                                 

 
 
 
 
 

 
 
 
 
 
 
typedef struct 
{
   command_tt    head;
} CMND_PSM_CBM_MANUAL_SC_SWITCHING_IND_T;



 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
typedef struct 
{
    command_tt    head;
    UINT16        slotNo;
    UINT16        reserved;
}CMND_UDM_MIB_RESTORE_READY_NOTIFY_T;

 
 
 
 
 
 
 
 
 
PB_ERROR_CODE_T 
	     CBM_IFM_OTU_PORT_PATH_MAPPING_CHANGE_CHECK(
	                                               UINT16       slotNo,                                        
                                                   UINT16       portNo,
                                                   UINT8        newPathMapping);


 
 
 
typedef unsigned int (*CHK_TP_RESERVE_CALLBACK)(const PB_GMPLS_TP_T* , UINT32 );
extern void grm_register_ccs_check_tp_reserve(CHK_TP_RESERVE_CALLBACK );
extern void grm_register_spm_check_tp_reserve(CHK_TP_RESERVE_CALLBACK );








# 21 "../../../appl/CTD/inc/ctd_cc.h" 2
















 













typedef enum{
    SDD_EC_P2W,
    SDD_EC_W2P,
}SDD_EC_SWITCH_T;

 
typedef enum{
    NO_TP_MAP =0,        
    PDH_TP_MAP,              
    SDH_TP_MAP,                 
    GMPLS_TP_MAP             
}MAPPER_TYPE_T;

typedef enum{
    LO_MAPPING_TU12 = 0,
    LO_MAPPING_TU3,
    LO_MAPPING_TU11,
    LO_MAPPING_TU2
}LO_MAPPING_TYPE_T;

typedef struct LoTPMapperItem_s
{
    MAPPER_TYPE_T TpType;     
    UINT8 bGmplsReserve;
    UINT8 LoBus;     
    UINT8 LoVC4;     
    UINT8  HoSlot;     
    UINT8  HoPortType;  
    UINT8  HoPort;     
    UINT8  HoE;          
    UINT8  HoD;         
    UINT8  HoC;         
    UINT8  HoB;         
} __attribute__ ((packed))  LO_TP_MAPPER_ITEM_T;

 
typedef struct CrossConnectionItem_s
{    
    UINT8 fromBus;       
    UINT8 fromVC4;       
    UINT8 type : 5;     
    UINT8 k : 3;         
    UINT8 l : 4;         
    UINT8 m : 3;         
    UINT8 bChanged : 1;  
} __attribute__ ((packed))  CROSS_CONNECT_ITEM_T;

 
typedef struct CrossConnectionHCCMatrix_s
{
    CROSS_CONNECT_ITEM_T    VC4List[136 ][16 ];     



} __attribute__ ((packed))  CROSS_CONNECT_HCC_MATRIX_T;
 
typedef struct CrossConnectionLCCMatrix_s
{
    CROSS_CONNECT_ITEM_T    VC3List[9 ][16 ][3 ];     
    CROSS_CONNECT_ITEM_T    VC12List[9 ][16 ][3 ][7 ][3 ];     
} __attribute__ ((packed))  CROSS_CONNECT_LCC_MATRIX_T;

 
typedef struct CrossConnectionHLCMatrix_s
{
    CROSS_CONNECT_ITEM_T    VC4List[18 ][16 ];     
    CROSS_CONNECT_ITEM_T    VC3List[18 ][16 ][3 ];     
    CROSS_CONNECT_ITEM_T    VC12List[18 ][16 ][3 ][7 ][3 ];     
} __attribute__ ((packed))  CrossConnectionHLCMatrix;

 
 
typedef int (*funcPBCCHCCAllCXC)(INSTANCE_T instance, CROSS_CONNECT_HCC_MATRIX_T *pMatrix);
typedef int (*funcPBCCLCCAllCXC)(INSTANCE_T instance, CROSS_CONNECT_LCC_MATRIX_T *pMatrix);
typedef int (*funcPBCCHLCAllCXC)(INSTANCE_T instance, CrossConnectionHLCMatrix *pMatrix);

 
 
typedef int (*funcPBCCSetVC4XC) (INSTANCE_T instance, UINT16 FromBus, UINT16 FromVC4, UINT16 ToBus, UINT16 ToVC4);
typedef int (*funcPBCCSetVC3XC) (INSTANCE_T instance,UINT16 FromBus, UINT16 FromVC4, UINT16 FromVC3, UINT16 ToBus, UINT16 ToVC4, UINT16 ToVC3);
typedef int (*funcPBCCSetVC12XC) (INSTANCE_T instance,UINT16 FromBus, UINT16 FromVC4, UINT16 FromVC3, UINT16 FromVC2, UINT16 FromVC1, UINT16 ToBus, UINT16 ToVC4, UINT16 ToVC3, UINT16 ToVC2, UINT16 ToVC1);

 
 
typedef int (*funcPBCCTpMapper)(INSTANCE_T instance, LO_TP_MAPPER_ITEM_T *pMapper);
 
typedef int (*funcPBCCLoMapping)(INSTANCE_T instance, UINT16 LoBus, UINT16 LoVC4, UINT8 LoK, LO_MAPPING_TYPE_T type);

 
typedef int (*funcPBCCSetHOCCCardState) (INSTANCE_T instance,BOOL bState);
typedef int (*funcPBCCFreezeAllSNCP) (INSTANCE_T instance,BOOL bEnable);
typedef int (*funcPBCCFreezeAllHardWareMSP) (INSTANCE_T instance,BOOL bEnable);

 
typedef int (*funcPBCCSetLOCCMode) (INSTANCE_T instance,BOOL bState);

 
 
typedef int (*funcPBCCSetEPGSwitch) (INSTANCE_T instance,UINT32 WorkSlot,UINT32 ProtSlot,BOOL bState);
typedef int (*funcPBCCSetMSPSwitch) (INSTANCE_T instance,UINT32 WorkSlot,HW_PORT_TYPE eWorkPortType,UINT32 WorkPort,
											UINT32 ProtSlot,HW_PORT_TYPE eProtPortType,UINT32 ProtPort,BOOL bState);
typedef int (*funcPBCCSetDRISwitch) (INSTANCE_T instance,UINT32 WorkSlot,HW_PORT_TYPE eWorkPortType,UINT32 WorkPort,TP_INDEX_T WorkTp,
											UINT32 ProtSlot,HW_PORT_TYPE eProtPortType,UINT32 ProtPort,TP_INDEX_T ProtTp,BOOL bState);

 
 
typedef int (*funcPBCCSetAis)(INSTANCE_T inst,HW_PORT_TYPE ePortType, UINT32 ulPort, TP_INDEX_T eTp, ALARM_DIRECTION_T eDir, BOOL bEnable);
typedef int (*funcPBCCSetCardAis)(INSTANCE_T inst,UINT32 Slot,BOOL bEnable);

typedef int (*funcGetLoccSlotFromLCTP)(UINT8 ucTpType, UINT8 ucSlot, UINT8 ucPortType
    , UINT8 ucPort, UINT32 ulTp);    

typedef PB_ERROR_CODE_T (* funcPBCCSetLOCCSwitch)(ENTITY_T etEntity, CTD_LOCC_MODE_T eLccMode, BOOL bWorkState);

typedef int (*funcPBCCSetCardType) (INSTANCE_T inst, UINT32 ulSlot, UINT32 ulStyle, UINT32 ulOption, BOOL bWarm);
typedef int (*funcCCSetPortType)(INSTANCE_T instance, UINT32 ulSlotNo, UINT32 ulPortType, UINT16  usPortId, UINT32 ulActualPortType);
typedef unsigned int (*funcCCGetNofreeCapacityslot)(UINT32 slot);

typedef  int (*funcCCGetGcbCheck)(PB_GMPLS_XC_OPERATION action, PB_GMPLS_XC_T xc);














# 31 "../../../appl/SDD/inc/sdd_ei.h" 2



# 1 "../../../appl/FMM/inc/fmm_ei.h" 1


 


















 



 






















 


 

typedef struct 
{
    command_tt          head;
    BOOL                enable;             
} FMM_HANDLE_SUPPRESS_CMND_T;

 
typedef struct
{
    command_tt          head;
    MIBpbActiveAlarmTable_t  data;
} FMM_AEL_NEW_ALARM_RAISED_CMND_T;

 
typedef enum
{
    LOG_COMMAND_NONE = 0,
    LOG_COMMAND_CLEAR_ALARMLOG  = 1,
    LOG_COMMAND_CLEAR_EVENTLOG  = 2,    
} LOG_COMMAND_T;

typedef struct
{
    command_tt         head;
    LOG_COMMAND_T      logCmd;    
} FMM_AEL_LOG_STATUS_CMND_T;


 





typedef struct
{
    command_tt         head;
    int                DetectTime;
    int                ClearTime;   
} FMM_FMC_ALM_PERSISTENT_TIME_CFG_CMND_T;


 
typedef struct
{
    command_tt         head;
    BOOL               enable;
} FMM_FMC_ENABLE_CORRELATION_CMND_T;


 
typedef struct
{
    command_tt          head;    
    BOOL                enable;
} FMM_FMC_NE_LEVEL_ALARM_INHIBIT_CMND_T;

 
typedef struct
{
    command_tt          head;    
    BOOL                aisEnable;
    BOOL                rdiEnable;
} FMM_FMC_NE_LEVEL_AIS_RDI_INHIBIT_CMND_T;

 
typedef struct
{
	command_tt          head;
	BOOL                alarmEnable;
    BOOL                aisEnable;
    BOOL                rdiEnable;
	BOOL                sefEnable;
	unsigned int        changeMsgBit;
}FMM_FMC_NE_LEVEL_CFG_CMND_T;

 
typedef struct
{
	PbEnableDisable_Tval pbNodeAlarmEnabling;
	TruthValue_Tval pbAISEnabled;
	TruthValue_Tval pbRDIEnabled;
	TruthValue_Tval pbSEFEnabled;
	unsigned int    changeMsgBit; 
}FMM_NE_LEVEL_CFG_CHANGE_T;

 

typedef enum
{
	FMM_ALARM_ENABLE = 0,
	FMM_AIS_ENABLE   = 1,
	FMM_RDI_ENABLE   = 2,
	FMM_SEF_ENABLE   = 3
}FMM_NE_CFG_NUM;

 
typedef struct
{
    command_tt          head;
    UINT32              ledId;
    BOOL                ledOn;
} FMM_FMA_ALARM_OUT_STATE_CMND_T;

 
typedef struct
{
    command_tt          head;
    UINT32              ledId;
    BOOL                ledOn;
} FMM_FMA_MDO_STATE_CMND_T;


 
 

































typedef struct
{
    command_tt          head;
    UINT32              ioaIndex;
    BOOL                enable;
} FMM_FMC_MDI_ENABLE_CFG_CMND_T;


 
typedef struct
{
    command_tt          head;
    UINT32              alarmId;
    BOOL                inhibit;
} FMM_FMC_ALARM_TYPE_INHIBIT_CFG_CMND_T;

 
typedef enum
{
    FM_ALARM_RULE_NO_CHANGE = 0,           
    FM_ALARM_RULE_ENABLE = 1,               
    FM_ALARM_RULE_DISABLE = 2,              
} FM_ALARM_RULE_T;

typedef struct
{
    command_tt          head;
    FM_ALARM_RULE_T     lof;
    FM_ALARM_RULE_T     lom;
    FM_ALARM_RULE_T     lop;
} FMM_FMC_ALARM_RULE_CFG_CMND_T;

extern int FMM_TclCmdShowProc(ClientData clientData, Tcl_Interp *interp, int argc, char *argv[]);








 































































# 34 "../../../appl/SDD/inc/sdd_ei.h" 2

# 1 "../../../appl/CAI/inc/cai_ei.h" 1
 






















 







typedef PB_ERROR_CODE_T (*CAI_LoadCard)(UINT32 instance,
                                        UINT32 style,
                                        UINT32 option);

 





PB_ERROR_CODE_T CAI_RegisterLoad(CAI_LoadCard pFunc);

 









 





 
 


 






 

 





 

 





 

 





 

 





 

 





 

 







typedef PB_ERROR_CODE_T (*CAI_BoardStateChangeFunc)(BOARD_EVENT_T event, SLOT_NUMBER_T slot, BOARD_STYLE_T style);
PB_ERROR_CODE_T CAI_BoardStateChange(BOARD_EVENT_T event, SLOT_NUMBER_T slot, BOARD_STYLE_T style);


 






 


 





 

 






typedef PB_ERROR_CODE_T (*CAI_ReportCardFailureFunc)(INT16 entityNum, BOOL failure);
PB_ERROR_CODE_T CAI_ReportCardFailure(INT16 entityNum, BOOL failure);



 






typedef struct _CM_CARDPRESENCE_T
{
    BOARD_PRESENCE_T  IsPresent[16  + 1];   
                                          
} CM_CARDPRESENCE_T;

typedef void (*CAI_ReportCardPresenceFunc)(INT16 entityNum,
                                       ENTITY_INFO_T *pInfo);
void CAI_ReportCardPresence(INT16 entityNum,
                                       ENTITY_INFO_T *pInfo);

 





 



 

 
 







# 35 "../../../appl/SDD/inc/sdd_ei.h" 2




# 1 "../../../appl/CAD/inc/cad_ei.h" 1
 




















void Cad_PrintTask(void);
 







typedef PB_ERROR_CODE_T (*CAD_InitCard)(UINT32 instance,
                                        UINT32 style,
                                        UINT32 option);

 





PB_ERROR_CODE_T CAD_RegisterInit(CAD_InitCard pFunc);


 






typedef PB_ERROR_CODE_T (*CAD_ActivateCard)(UINT32 instance,BOOL bWarmFlag);

 





PB_ERROR_CODE_T CAD_RegisterActive(CAD_ActivateCard pFunc);

 





typedef PB_ERROR_CODE_T (*CAD_DeinitCard)(UINT32 instance);

 





PB_ERROR_CODE_T CAD_RegisterDeinit(CAD_DeinitCard pFunc);

 





typedef PB_ERROR_CODE_T (*CAD_DeactiveCard)(UINT32 instance);

 





PB_ERROR_CODE_T CAD_RegisterDeactive(CAD_DeactiveCard pFunc);

 







 
 


 






typedef PB_ERROR_CODE_T (*CAD_Detecter)(INSTANCE_T instance, BOOL *pState);


 





PB_ERROR_CODE_T CAD_RegisterDetecter(CAD_Detecter pFunc);

 








 
 
 
 



 





 









 
 

 
                                       

 






typedef PB_ERROR_CODE_T (*CAD_ColdRebootFunc)(void);
extern PB_ERROR_CODE_T CAD_RegisterColdReboot(CAD_ColdRebootFunc pFunc);

 






typedef PB_ERROR_CODE_T (*CAD_ReportCardFailureFunc)(UINT32 slot, BOOL failure);
extern PB_ERROR_CODE_T CAD_ReportCardFailure(UINT32 slot, BOOL failure);

 






typedef PB_ERROR_CODE_T (*CAD_ReportFanFailureFunc)(UINT32 slot, BOOL failure);
extern PB_ERROR_CODE_T CAD_ReportFanFailure(UINT32 slot, BOOL failure);

 






typedef PB_ERROR_CODE_T (*CAD_FpgaVersion)(INSTANCE_T    inst, char *pFpgaVer);

PB_ERROR_CODE_T CAD_RegisterFpgaVersion(CAD_FpgaVersion pFunc);

 





typedef PB_ERROR_CODE_T (*CAD_RebootL2Card)(UINT32 instance);
PB_ERROR_CODE_T CAD_RegisterRebootL2Card(CAD_RebootL2Card pFunc);

typedef PB_ERROR_CODE_T (*CAD_SetLOCCCardState)(UINT32 instance, BOOL bState);
PB_ERROR_CODE_T CAD_RegSetLOCCCardState(CAD_SetLOCCCardState pFunc);

typedef PB_ERROR_CODE_T (*CAD_SetLcAIS)(UINT32 instance,UINT32 slot, BOOL bSet);
PB_ERROR_CODE_T CAD_RegSetLcAIS(CAD_SetLcAIS pFunc);

 

extern PB_ERROR_CODE_T CMC_RegisterECCActive(CAD_ActivateCard pFunc);
extern PB_ERROR_CODE_T CMC_RegisterECCDeactive(CAD_DeactiveCard pFunc);







# 39 "../../../appl/SDD/inc/sdd_ei.h" 2










 

typedef PB_ERROR_CODE_T (*SDD_TEMP_EVENT_CBACK)(ENTITY_T entity,
                                     SDD_BOARD_TYPE_T boardtype,
                                     BOOL bOverheat);
 
typedef PB_ERROR_CODE_T (*SDD_TEMP_LEM_EVENT_CBACK)(ENTITY_T entity,
                                     SDD_BOARD_TYPE_T boardtype,
                                     BOOL bOverheat);
 
typedef void (*HII_PTInitGESFPFunc)(int instance, int portId, int SFPType);

 
typedef PB_ERROR_CODE_T(*SDD_NotifyIfmSfpChange)(UINT32 ulSlotNo, UINT32 ulPortType, UINT32 ulPortNo, UINT32 ulConnectorType); 

 
SDD_RESET_REASON SDD_GetResetReason();
void SDD_LoadModule();
void SDD_SetModuleState(BOOL enable);
BOOL SDD_GetModuleState();
BOOL SDD_JudgeIfMaincard();
PbBoardStyle_Tval SDD_GetLocalBoardStyle();


 
PB_ERROR_CODE_T SDD_InitCard(ENTITY_T entity, CARDTYPE_T cardType, UINT32 ulOption);
PB_ERROR_CODE_T SDD_ActivateCard(ENTITY_T entity, BOOL bWarm);
PB_ERROR_CODE_T SDD_DeactivateCard(ENTITY_T entity);
PB_ERROR_CODE_T SDD_DeinitCard(ENTITY_T entity);

BOARD_PRESENCE_T SDD_GetBoardPresence(ENTITY_T entity);
PbBoardFunction_Tval SDD_GetBoardFunction(ENTITY_T entity);
PbBoardStyle_Tval SDD_GetBoardStyle(ENTITY_T entity);
UINT SDD_GetSlotBoardRev(ENTITY_T entity);
PB_ERROR_CODE_T SDD_ResetBoard(ENTITY_T entity);
PB_ERROR_CODE_T SDD_SetLedState(ENTITY_T entity, LED_TYPE_T ledType,UINT16 usLedId, LED_STATE_T ledState);
PB_ERROR_CODE_T SDD_GetLedState(ENTITY_T entity, LED_TYPE_T ledType,UINT16 usLedId, LED_STATE_T *pLedState);
PB_ERROR_CODE_T SDD_GetMdiPort(UINT16 portId, BOOL *pbState);
PB_ERROR_CODE_T SDD_SetMdoPort(UINT16 portId, BOOL bEnable);
PB_ERROR_CODE_T SDD_SetAlarmOut(UINT16 portId, BOOL bEnable);
PB_ERROR_CODE_T SDD_SetGlobalOutSignalStatus(BOOL bEnable);
PB_ERROR_CODE_T SDD_SetCardOutSignalStatus(ENTITY_T entity, BOOL bEnable);
PB_ERROR_CODE_T SDD_SetCardManageSignalStatus(ENTITY_T entity, UINT16 portId, BOOL bEnable);
PB_ERROR_CODE_T SDD_SetPortAdminStatus(ENTITY_T entity, HW_PORT_TYPE portType, UINT16 portId, BOOL bEnable);
PB_ERROR_CODE_T SDD_EnablePortAls(ENTITY_T entity, HW_PORT_TYPE portType, UINT16 portId, BOOL bEnable);
PB_ERROR_CODE_T SDD_GetInSignalPowerStatus(ENTITY_T entity, HW_PORT_TYPE portType, UINT16 portId, BOOL *pNip);
PB_ERROR_CODE_T SDD_NotifyLos(ENTITY_T entity, HW_PORT_TYPE portType, UINT16 portId, BOOL bLos);
PB_ERROR_CODE_T SDD_GetOutSignalStatus(ENTITY_T entity, HW_PORT_TYPE portType, UINT16 portId, BOOL *pStatus);
PB_ERROR_CODE_T SDD_SetSTMMode(ENTITY_T entity, HW_PORT_TYPE ePortType, UINT32 ulPort, HW_ACT_PORT_TYPE Mode);
PB_ERROR_CODE_T SDD_GetSfpPresence(ENTITY_T entity, HW_PORT_TYPE portType, UINT16 portId, BOOL *pState);
PB_ERROR_CODE_T SDD_GetLaserParameters(ENTITY_T entity, HW_PORT_TYPE portType, UINT16 portId, LASER_PARAMETER_T *pParam);
PB_ERROR_CODE_T SDD_GetRtcTime(struct tm *pMyTime);
PB_ERROR_CODE_T SDD_SetRtcTime(struct tm *myTime);
PB_ERROR_CODE_T SDD_GetTemperature(ENTITY_T entity, UINT16 id, INT16 *pTemp);
PB_ERROR_CODE_T SDD_SetTempThreshold(ENTITY_T entity, TEMPERATURE_T high, TEMPERATURE_T low);
 
void SDD_Enable1HzInterrupt(BOOL bEnable);
void SDD_RegisterReportDefect(FMA_AlmModDefectFunc pFunc);         
void SDD_RegisterBoardsPresence(CAI_ReportCardPresenceFunc pFunc); 
void SDD_RegisterReportCardFail(CAI_ReportCardFailureFunc pFunc);  
void SDD_RegisterNotifyIfmSfpChange(SDD_NotifyIfmSfpChange pFunc);  

PB_ERROR_CODE_T SDD_RegisterLatchEvent(SDD_LATCH_EVENT_CBACK pFunc);   
PB_ERROR_CODE_T SDD_NotifyLatchEvent(ENTITY_T entity,CARDTYPE_T cardType,BOARD_EVENT_T latchEvent);
PB_ERROR_CODE_T SDD_RegisterBoardEvent(SDD_BOARD_EVENT_CBACK pFunc);   
PB_ERROR_CODE_T SDD_RegisterEntityUpdate(SDD_ENTITY_UPDATE_CBACK pFunc);   
PB_ERROR_CODE_T SDD_RegisterOneSecTimer(SDD_ONE_SECOND_TIMER_CBACK pFunc);
PB_ERROR_CODE_T SDD_RegisterButtonEvent(SDD_BUTTON_EVENT_CBACK pFunc);
PB_ERROR_CODE_T SDD_RegisterTempEvent(SDD_TEMP_EVENT_CBACK pFunc);         
PB_ERROR_CODE_T SDD_RegisterTempLemEvent(SDD_TEMP_LEM_EVENT_CBACK pFunc);         
PB_ERROR_CODE_T SDD_SetNotifyTempEvent(ENTITY_T entity);

 


void SDD_RegisterReportFanFail(CAD_ReportFanFailureFunc pFunc);


PB_ERROR_CODE_T SDD_ECCardSwitch(ENTITY_T entity, SDD_EC_SWITCH_T switchStatus);
PB_ERROR_CODE_T SDD_TimingRefSel(ENTITY_T entity, TIMING_REF_T refSet);
PB_ERROR_CODE_T SDD_DetectFanFailure(ENTITY_T entity);
PB_ERROR_CODE_T SDD_DetectPwrFailure(ENTITY_T entity);
void SDD_EnableLedTest(BOOL bEnable);
PB_ERROR_CODE_T SDD_GetFanState(ENTITY_T entity, FAN_STATUS_T *pState);
PB_ERROR_CODE_T SDD_GetPowerState(ENTITY_T entity, UINT16 id, POWER_STATUS_T *pState);
PB_ERROR_CODE_T SDD_GetLocalSlot(SLOT_NUMBER_T *pSlot);

 
PB_ERROR_CODE_T SDD_SET_OAReset (BOOL bActive);
PB_ERROR_CODE_T SDD_SET_OADisable (BOOL bActive);
PB_ERROR_CODE_T SDD_SET_OAOutputMute (BOOL bActive);
PB_ERROR_CODE_T SDD_SET_OALinkLED (BOOL bActive);
PB_ERROR_CODE_T SDD_GET_OAPin (UINT16 *wStatus);
PB_ERROR_CODE_T SDD_ResetAsic(ENTITY_T entity);
PB_ERROR_CODE_T SDD_InitSi5319Sedres(ENTITY_T entity);

 
PB_ERROR_CODE_T SDD_SetGeElLaserParameters(ENTITY_T entity,HW_PORT_TYPE portType,
                                UINT16 port, UINT8 offset,UINT16 val);
PB_ERROR_CODE_T SDD_GetGeElLaserParameters(ENTITY_T entity,HW_PORT_TYPE portType,
                                UINT16 port, UINT8 offset,UINT16 * val);
void SDD_RegHIIInitGeSfp(HII_PTInitGESFPFunc fun);
 
PB_ERROR_CODE_T SDD_SetAlsMode(ENTITY_T entity, HW_PORT_TYPE portType, UINT16  usPortId, ALS_MODE_STATUS_T aMode);
PB_ERROR_CODE_T SDDSetPCA9555(UINT8 addr, UINT16 states);
PB_ERROR_CODE_T SDD_SetImpedanceMode(ENTITY_T entity, HW_PORT_TYPE ePortType, UINT32 ulPort, PDH_OHM_TYPE_T eOhmType);
void SDD_NotifyNERebootFinished();
void SDD_NotifyNERebootStarted();

 
PB_ERROR_CODE_T SDD_SetPortAdminStatus(ENTITY_T entity, HW_PORT_TYPE portType, UINT16 portId, BOOL bEnable);
PB_ERROR_CODE_T SDD_SetWavelength(ENTITY_T etEntity, HW_PORT_TYPE portType, UINT16 portId, UINT16 wWaveLengthId);
PB_ERROR_CODE_T SDD_SetDecisionThreshold(ENTITY_T etEntity, HW_PORT_TYPE portType, UINT16 portId, UINT16 wDecesionThreshold);
PB_ERROR_CODE_T SDD_GetOsrAbnormalStatus(ENTITY_T etEntity, HW_PORT_TYPE portType, UINT16 portId, BOOL *pbState);
PB_ERROR_CODE_T SDD_GetDecisionThreshold(ENTITY_T etEntity, HW_PORT_TYPE portType, UINT16 portId, UINT16 *wDecesionThreshold);

PB_ERROR_CODE_T SDD_ISELECSFP(ENTITY_T entity, HW_PORT_TYPE portType, UINT16 portId, BOOL *wIsElecSfp);

extern int sdd_get_ioN_port (int io, int * pFirst, int * pLast);
extern int SDD_126xE1IOPortConvert (int port);











# 18 "cah_126e1.c" 2

# 1 "../../../infra/inc/bspFpga.h" 1
 

























# 1 "../../../infra/SYS/target/h/intLib.h" 1
 

 

 























































 





 

extern  int intCnt;		 


 



 














 



extern STATUS 	intConnect (VOIDFUNCPTR *vector, VOIDFUNCPTR routine,
	       	    	    int parameter);
extern FUNCPTR 	intHandlerCreate (FUNCPTR routine, int parameter);
extern void 	intLockLevelSet (int newLevel);
extern int 	intLockLevelGet (void);
extern BOOL 	intContext (void);
extern int 	intCount (void);
extern void 	intVecBaseSet (FUNCPTR *baseAddr);
extern FUNCPTR *intVecBaseGet (void);
extern void 	intVecSet (FUNCPTR *vector, FUNCPTR function);
extern FUNCPTR 	intVecGet (FUNCPTR *vector);
extern int 	intLevelSet (int level);
extern int 	intLock (void);
extern int 	intUnlock (int oldSR);
extern int 	intRegsLock (REG_SET *pRegs);
extern void 	intRegsUnlock (REG_SET *pRegs, int lockKey);
extern STATUS   intVecTableWriteProtect (void);

























extern int	intDisable (int);
extern int	intEnable (int);














# 205 "../../../infra/SYS/target/h/intLib.h"







# 27 "../../../infra/inc/bspFpga.h" 2



# 1 "../../../infra/inc/bspType.h" 1

# 23 "../../../infra/inc/bspType.h"




# 30 "../../../infra/inc/bspFpga.h" 2

# 1 "../../../infra/inc/target.h" 1
 

 
 

 





 










# 1 "../../../infra/inc/bspType.h" 1

# 23 "../../../infra/inc/bspType.h"




# 23 "../../../infra/inc/target.h" 2
























# 31 "../../../infra/inc/bspFpga.h" 2











     






















    typedef struct _FPGA_REGADDR_T
    {
        UINT32 mask1;
        UINT32 mask2;
        UINT32 mask3;
        UINT32 status1;
        UINT32 status2;
        UINT32 status3;
    }
    FPGA_REGADDR_T;


# 120 "../../../infra/inc/bspFpga.h"


     

 
     





     































    typedef struct _FPGA_SIG_PIN_T
    {
        UINT32 port;                   
        UINT32 pin;                    
    }
    FPGA_SIG_PIN_T;






























    typedef struct _FPGA_PIN_XILINX_T
    {
        FPGA_SIG_PIN_T BitInit;             
        FPGA_SIG_PIN_T BitProgram;          
        FPGA_SIG_PIN_T BitDone;             
 
        FPGA_SIG_PIN_T BitFpgaDln;          
    }
    FPGA_PIN_XILINX_T;

    typedef struct _FPGA_PIN_ALTERA_T
    {
        FPGA_SIG_PIN_T BitDClk;             
        FPGA_SIG_PIN_T BitData0;            
        FPGA_SIG_PIN_T BitnConfig;          
        FPGA_SIG_PIN_T BitnStatus;          
        FPGA_SIG_PIN_T BitCfgDone;          
 
        FPGA_SIG_PIN_T BitFpgaDln;          
    }
    FPGA_PIN_ALTERA_T;

    typedef union _FPGA_PINCFG_T
    {
        FPGA_PIN_ALTERA_T PinAltera;        
        FPGA_PIN_XILINX_T PinXilinx;        
    } FPGA_PINCFG_T;

     









     





























     


    typedef enum
    {
        FPGA_STATUS_INACTIVE,
        FPGA_STATUS_ACTIVE,
    } FPGA_STATUS_T;

    typedef int FPGA_HANDLE;







    typedef unsigned int FPGA_HOOK_ARG;
    typedef void ( * FPGA_HOOK_FUNC ) ( FPGA_HANDLE handle, FPGA_HOOK_ARG usrHookArg );

    typedef struct _FPGA_INTHOOK_T
    {
        FPGA_HOOK_FUNC UsrHookFunc;         
        FPGA_HOOK_ARG UsrHookArg;          
        UINT32 Interrupts;          
        UINT32 Missed;              
    }
    FPGA_INTHOOK_T;

    typedef struct _FPGA_ERR_STATS_T
    {
        UINT32 Interrupts;                  
        UINT32 WriteViolate;                
        UINT32 AddressViolate;              
        UINT32 SizeViolate;                 
        UINT32 PortViolate;                 
        UINT32 BoundaryViolate;             
    }
    FPGA_ERRSTATS_T;


     


    typedef struct _FPGA_CHIP_DATA_T
    {
        UINT8 Chip;                
        UCHAR CS;                  
        UINT16 IntNum;              
        UINT32 MemBase;             
        FPGA_PINCFG_T PinCfg;              
        UCHAR * FileName;            
        void ( *RegInit ) ( struct _FPGA_CHIP_DATA_T * );
         
        UINT32 SubFunction;         
        UCHAR * SubSlot;             

        FPGA_INTHOOK_T IntHook[ 64  ];
         
        FPGA_ERRSTATS_T ErrStats;            
        UINT32 Version;             
        UINT32 Status;
    }
    FPGA_CHIP_DATA_T;





    typedef struct
    {
        UCHAR fileName[ 13  ];
        UINT32 fileLength;
    }
    FPGA_MEM_DATA_T;

    extern FPGA_CHIP_DATA_T *fpgaChips;

# 408 "../../../infra/inc/bspFpga.h"


     



    extern int pbFpgaInit( void );

    extern FPGA_STATUS_T pbFpgaGetStatus( void );

     



    extern FPGA_HANDLE pbFpgaIsrHookAdd
        (
            int chip,
            int asic,
            int bank,
            FPGA_HOOK_FUNC usrHookFunc,
            FPGA_HOOK_ARG usrHookArg
        );
    extern FPGA_HANDLE pbFpgaIsrHookSlotAdd
        (
            PbSlotNumberType_Tval slot,
            FPGA_HOOK_FUNC usrHookFunc,
            FPGA_HOOK_ARG usrHookArg
        );

     



    extern int pbFpgaIsrHookDel( FPGA_HANDLE handle );

     



    extern int pbFpgaIntrEnable( FPGA_HANDLE handle );
    extern int pbFpgaIntrSlotEnable( PbSlotNumberType_Tval slot );

     



    extern int pbFpgaIntrDisable( FPGA_HANDLE handle );
    extern int pbFpgaIntrSlotDisable( PbSlotNumberType_Tval slot );

    extern int pbFpgaIsrEnable( int chip );
    extern int pbFpgaIsrDisable( int chip );
    extern int pbFpgaDownloadCS( UINT cs, char * fileName, UCHAR * data, UINT length );










    typedef void ( * FPGA_WRITE_FUNC ) ( UINT32 addr, UINT32 var );
    typedef UINT32 ( * FPGA_READ_FUNC ) ( UINT32 addr );

    STATUS pbFpgaDownloadSlot( PbSlotNumberType_Tval slot, char * fileName, UINT32 memBase );
    STATUS pbFpgaInitSlot( PbSlotNumberType_Tval slot, char * fileName );







# 19 "cah_126e1.c" 2

# 1 "../../../appl/CAH/inc/cah_126e1.h" 1
 



















# 57 "../../../appl/CAH/inc/cah_126e1.h"
































































 







































 









 



 






 



 


 










 


 


 


 

 

 

 


 

 

 

 


 


 


 





 
 
 

 



 
 
 

 

 
 
 

 




 
 
 

 


 


 


 


 


 


 


 

 

 


 
 
 

 


 
 
 

 


 


 


 


 




 



 


 


 
 
 

 



 
 
 

 



 

















 








 


















 
























 

























 

















 









     




 
 





 
 











    
 
 



 
 


 
 


 
 

















 
 




 
 









 
 













 
 













 




 


















 






























 




























# 670 "../../../appl/CAH/inc/cah_126e1.h"

 
















































# 728 "../../../appl/CAH/inc/cah_126e1.h"
































typedef enum
{
        eEm4EntEmp = 0,
        eEm4EntInit,
        eEm4EntAct  
}
EM4_ENTITY_STATE_T;


typedef void (*CPA_LATCHSTATE_CHANGE_FUNC)(BOARD_EVENT_T ulEvent, SLOT_NUMBER_T usSlotNum, BOARD_STYLE_T ulCardType);
typedef struct
{
    FMA_AlmModDefectFunc pFuncReportAlm;  
    CPA_LATCHSTATE_CHANGE_FUNC pFuncReportLatch;  
    PRA_SetPortStatus_Func pFuncReportPrbs;  
    CAI_BoardStateChangeFunc pFuncReportFault;
} CAH_EM4_CALLBACKS_T;

typedef struct {
    UINT32 address;
    UINT8 value;
}EM4_REG_CONF;

typedef struct 
{
    UINT32 baseAddr;

     


    UINT32 portMask[(63 )  + 1];    
}CAH_EM4_CHIP_CONFIG_T;



typedef struct
{
        PbTIModeType_Tval J2Mode;
        UINT8 J2Data[64  + 1];
}
EM4_J2_BYTES_T;
typedef struct
{
        PRBS_MODE_T prbsMode;
        PRBS_STATUS_T prbsStatus;
         


        UINT32 prbsErrCnt;
        BOOL bForceRpt;
}
EM4_PRBS_CONFIG_T;

typedef enum
{
    EM4_NONE_FORCE  = 0,
    EM4_DOWN_FORCE  = 1<<1,
    EM4_UP_FORCE    = 1<<2
}EM4_FORCE_MODE_T;



 


typedef struct
{
        UINT16 almID[14  + 2];

 

























        UINT16 state[14  + 2];
        UINT16 e1state[14  + 2];

 
















        UINT16 ssf;
        UINT16 ssfFc;
         


        UINT16 rdi;
        UINT16 rfi;
        UINT16 rdiFc;
        UINT16 rfiFc;

         


        UINT16 temp;     
        UINT16 e1ssf;
        UINT16 e1ssfFc;
        UINT16 e1rdi;
        UINT16 e1rdiFc;
}
EM4_POH_ALM_T;




 


typedef struct
{
         


        CAH_EM4_CHIP_CONFIG_T cfg;

         


        UINT32 su_id;

         


        BOOL bPlmEnable[(63 )  + 1];
        BOOL bTimEnable[(63 )  + 1];
        PORT_STATE_T adminState[(63 )  + 1];
        UINT8 ExpPSL[(63 )  + 1];
        UINT8 RcvPSL[(63 )  + 1];
        UINT8 TranPSL[(63 )  + 1];

        EM4_J2_BYTES_T strExpJ2[(63 )  + 1];
        EM4_J2_BYTES_T strTranJ2[(63 )  + 1];
        EM4_J2_BYTES_T strRcvJ2[(63 )  + 1];

         



        PDH_PORT_MODE_T portType[(63 )  + 1];

         


        EM4_PRBS_CONFIG_T prbsCfg[(63 )  + 1];

         


         unsigned int faisMode[(63 )  + 1];
        EM4_FORCE_MODE_T frdiMode[(63 )  + 1];

         


        PORT_LOOPBACK_T loopMode[(63 )  + 1];

         


        EM4_POH_ALM_T pohAlm[(63 )  + 1];
        int farcrc[(63 )  + 1];
        int crc[(63 )  + 1];
        int nslip[(63 )  + 1];
        int pslip[(63 )  + 1];

         



        BOOL forceRptAlm[(63 )  + 1];

         


        UINT32 nPrbsDelayCount[(63 )  + 1];

         


        BOOL bRestrainRDI[(63 )  + 1];
        BOOL bBlockAis[(63 )  + 1];
        int isFrcais [(63 )  + 1];
        int isFrcrdi [(63 )  + 1];

         


        int isRptLOM [(63 )  + 1];
        PDH_FRAME_MODE_T isFrame [(63 )  + 1];

        
}
EM4_CHIP_VAR_T;


 
 

typedef enum
{ 
    EM4_ENTITY_UNKNOWN = 0,
    EM4_ENTITY_LATCH,
    EM4_ENTITY_DELATCH
}CAH_EM4_ENTITY_LATCH_T;




typedef enum{
    eEM4_DISABLE = 0,
    eEM4_ENABLE  = 1
}eEM4SWITCH;





typedef struct
{
         






        ENTITY_T entity;

         






        DWORD dwPos;

         




        UINT32 chipNum;

         


        EM4_ENTITY_STATE_T state;

         


        EM4_CHIP_VAR_T chip_var[(5) ];
}
CAH_EM4_ENTITY_DATA_T;







 




typedef union _ARG32
{
    int arg;
    char tti[32 + 3];
}ARG32;
typedef void (*PMyFun)( );


typedef struct __E1_CALL
{
     
    PMyFun p;
    int arg1;
    int arg2;
    int arg3;
 







    ARG32 arg4;
    int arg5;
    int arg6;
    int sn;
    int id;
    struct __E1_CALL *pNext;
    struct __E1_CALL *pHashNext[11];
    char fname[64];
}
E1_CALL;








 














 
 
typedef union _sexar_framer_cfgctrl
{
	struct _cfgCtrlBit
	{
   		unsigned    loopback:1;     
		unsigned	IsDS3:1;	    
		unsigned	losEnable:1;    
		unsigned	reset:1;        
		unsigned	Unused:1;	    
        unsigned	format:1;	    
        unsigned	TimeRefSel:2;   
	} cfgCtrlBit;
	UINT1 cfgCtrl;
}EXAR_FRAMER_CTRL_T;


typedef union _sexar_rxj1_cfgctrl_x
{
	struct _rxj1CtrlBit_x
	{
   		unsigned    reserved :3;   
		unsigned	bufMode:1;	   
		unsigned	threshold:1;   
		unsigned	alignment:1;   
		unsigned	length:2;	   
	} rxj1CtrlBit;
	UINT1 rxj1Ctrl;
}EXAR_RXJ2_CTRL_T;





typedef enum
{
        eEm4J2Tx = 0,
        eEm4J2Rx,
        eEm4J2Exp
}
EM4_J2_DIRECTION_T;

typedef enum
{
        eEm4J2Rd = 0,
        eEm4J2Wr,
}
EM4_J2_OPERATION_T;


typedef enum 
{
    EEM_LB_GL_TOH_IN = 5 ,         
    EEM_LB_GL_POH_IN,         
    EEM_LB_LC_FRM_IN,         
    EEM_LB_LC_FRM_OUT,      

}EEM_LOOPBACK_T;

typedef enum 
{
    EEM_LB_CLEAR , 
    EEM_LB_SET,

}EEM_LB_ACC_T;



 


typedef union __sexar_alarm_dect
{
    struct __alarmCtrlBit
    {

         


        int rfi:1;
        int ppi_tlct:1;  
        int ppi_crc:1;
        int ppi_oof:1;   
        int ppi_lof:1;
        int ppi_lom:1;
        int ppi_rdi:1;
        int other1:1;          

         


        int tim:1;             
        int unstablePSL:1;     
        int uneq:1;            
        int plm:1;             

        int rdi:1;             
        int unstableRDI:1;     
        int lop:1;             
        int ais:1;             
    }
    alarmCtrlBit;
    UINT16 alarmCtrl;
}
ALARM_REGISTER_T;



typedef struct
{
        SEM_ID semInst[((1) )  + 1];
        SEM_ID semData;
        SEM_ID semLink;
        SEM_ID semFind;

         


        SEM_ID semLatchIsr;
        SEM_ID semisr;

        SEM_ID semLatchIsrClose;
        SEM_ID semisrclose;

        int latchent;
        int fpgaHanle[((1) )  + 1];
        BOOL bEM4Running;
        MSG_Q_ID EM4MsgQId;
        CAH_EM4_ENTITY_LATCH_T EM4LatchStatus[((1) )  + 1];
        BOOL EM4LatchForceRpt[((1) )  + 1];
        UINT16 IntCount[((1) )  + 1];
}
CAH_EM4_ENTITY_TRIB_T;



typedef union sexar_prbs_cfgctrl
{
	struct __prbsCtrlBit
	{
   		UINT1   TxOHSrc:1;    
		UINT1	Unused:2;	  
		UINT1	lock:1;       
		UINT1	RxPRBS:1;     
		UINT1	TxPRBS:1;	  
        UINT1	reserved:2;	  
	} prbsCtrlBit;
	UINT1 prbsCtrl;
}__EXAR_PRBS_CTRL_T;

typedef union sexar_ais_cfgctrl
{
	struct __aisCtrlBit
	{
   		UINT1   reserved:1;       
		UINT1   unstableC2:1;	  
		UINT1	uneq:1;           
		UINT1	plm:1;            
		UINT1	unstableJ1:1;	  
		UINT1	tim:1;	          
                                  
		UINT1	lop:1;	          
        UINT1	enable:1;	      
	} aisCtrlBit;
	UINT1 aisCtrl;
}__EXAR_AIS_CTRL_T;


typedef union sexar_insert_cfgctrl
{
	struct __insertCtrlBit
	{
   		UINT1   F2:1;              
		UINT1   rei:2;	           
		UINT1	rdi:2;             
		UINT1	c2:1;              
		UINT1	pdi:1;             
		UINT1	ais:1;             
	} insertCtrlBit;
	UINT1 insertCtrl;
}__EXAR_INSERT_CTRL_T;


            
typedef union sexar_alarm_dect_2
{
	struct _alarmCtrlBit_2
	{
   		UINT1   tim:1;            
		UINT1   unstableC2:1;	  
		UINT1	uneq:1;           
		UINT1	plm:1;            
		UINT1	rdi:1;	          
		UINT1	unstableRDI:1;	  
		UINT1	lop:1;	          
        UINT1	ais:1;	          
	} alarmCtrlBit;
	UINT1 alarmCtrl;
}__EXAR_ALARM_DECT_T;


 



 
WORD CAH_EM4_SetPortLoopback(DWORD dwPos, BYTE byPortNo, CTD_LOOPBACK_TYPE_T eLpbckMd);

 
WORD CAH_EM4_SetInsertAlm(DWORD dwPos, BYTE byPortNo, CTD_INSERT_ALM_TYPE_T eAlmType, CTD_INSERT_ALM_DIR_T eInsAlmDir, BOOL bEnable);
 
 
WORD CAH_EM4_GetPrbsState(DWORD dwPos, BYTE byPortNo, BOOL *bPrbsSucc);
WORD CAH_EM4_EnablePrbs (DWORD dwPos, UINT32 byPortNo, BOOL bEnable) ;
WORD CAH_EM4_GetPrbsErrCount (DWORD dwPos, UINT32 byPortNo, DWORD *dwPrbsCnt);

 
UINT32 sysTimestamp(void);
UINT32 sysTimestampFreq(void);
 
WORD CAH_EM4_GetTimingRef(DWORD dwPos, BYTE   byClockNum, BYTE    *byPort,  BOOL  *bEnable);
WORD CAH_EM4_SetTimingRef(DWORD dwPos, BYTE    byPort, BYTE   byClockNum,  BOOL  bEnable);

 
WORD CAH_EM4_GetFpgaVersion(DWORD dwPos, char *pucFpgaVer);
WORD CAH_EM4_GetPpiAlmState(DWORD dwPos, UINT8 byPortNo, CTD_PPI_ALARM_T *psPpiAlarm);
WORD CAH_EM4_GetLpAlmState(DWORD dwPos, UINT8 byPortNo, CTD_LP_ALARM_T *psLpAlarm);
WORD CAH_EM4_GetTuAlmState(DWORD dwPos, UINT8 byPortNo, CTD_TU_ALARM_T *psTuAlarm);

 
WORD CAH_EM4_getLatchStatus (DWORD dwPos);

 
WORD CAH_EM4_EnableReTiming (DWORD dwPos, BYTE byPortNo,BOOL bEnable);

 
WORD CAH_EM4_GetLpBipCount (DWORD dwPos, BYTE byPortNo, DWORD * pLpBip, DWORD * pLpRei);
WORD CAH_EM4_GetPpiBipCount(DWORD dwPos, BYTE byPortNo, DWORD *pPpiBip, DWORD *pPpiFebe);
WORD CAH_EM4_GetSlip (DWORD dwPos, BYTE byPortNo ,DWORD *pdwPSlip, DWORD *pdwNSlip);

 
WORD CAH_EM4_SetRxLpTtiMode(DWORD dwPos, UINT32 byPortNo, TTI_LEN_T eTtiMode);
WORD CAH_EM4_SetTxLpTtiMode (DWORD dwPos, UINT32 byPortNo, TTI_LEN_T eTtiMode);
WORD CAH_EM4_SetRxLpTtiAisEnable(DWORD dwPos, BYTE byPortNo, BOOL bEnable);
WORD CAH_EM4_SetRxLpTtiAlmEnable(DWORD dwPos, BYTE byPortNo, BOOL bEnable);
WORD CAH_EM4_SetRxExpLpTti (DWORD dwPos, UINT32 byPortNo, TTI_LEN_T eTtiMode, const BYTE *pbyTtiValue);
WORD CAH_EM4_SetTxLpTti (DWORD dwPos, UINT32 byPortNo, TTI_LEN_T eTtiMode, const BYTE *pbyTtiValue);
WORD CAH_EM4_GetTxLpTti(DWORD dwPos, BYTE byPortNo, TTI_LEN_T eTtiMode, BYTE *pbyTtiValue);
WORD CAH_EM4_GetRxRcvLpTti(DWORD dwPos, BYTE byPortNo, TTI_LEN_T eTtiMode, BYTE *pbyTtiValue);
WORD CAH_EM4_GetRxExpLpTti(DWORD dwPos, BYTE byPortNo, TTI_LEN_T eTtiMode, BYTE *pbyTtiValue);

 
WORD CAH_EM4_SetRxLpPslEnable(DWORD dwPos, BYTE byPortNo, BOOL bEnable);
WORD CAH_EM4_SetRxTxLpPsl(DWORD dwPos, BYTE byPortNo,  BYTE byC2Value);
WORD CAH_EM4_SetRxExpLpPsl(DWORD dwPos, BYTE byPortNo,  BYTE byC2Value);
WORD CAH_EM4_SetTxLpPsl(DWORD dwPos, BYTE byPortNo,  BYTE byC2Value);
WORD CAH_EM4_GetRxRcvLpPsl (DWORD dwPos, UINT32 byPortNo, BYTE *pbyC2Value);
WORD CAH_EM4_GetRxExpLpPsl(DWORD dwPos, BYTE byPortNo,  BYTE *byC2Value);

 
WORD CAH_EM4_DetectCardState (DWORD dwPos, BOOL * pbActive);
WORD CAH_EM4_GetInnerB1Err(DWORD dwPos, BYTE byPortNo, DWORD *pdwB1Err);
WORD CAH_EM4_GetTdmLinkErr(DWORD dwPos, DWORD *pdwTdmErr);
WORD CAH_EM4_Get8KClkState(DWORD dwPos, BOOL *pbActive);
WORD CAH_EM4_Get19MClkState(DWORD dwPos, BOOL *pbActive);
WORD CAH_EM4_GetPllState(DWORD dwPos, BOOL *pbPllLock);

  
PB_ERROR_CODE_T CAH_EM4_loadModule ();
PB_ERROR_CODE_T CAH_EM4_activateEntity(ENTITY_T entity, BOOL b_warm) ;
PB_ERROR_CODE_T CAH_EM4_deActEntity(ENTITY_T entity);
WORD CAH_EM4_InitEntity (ENTITY_T etEntity, DWORD dwPos, CTD_MODE_TYPE_T eporttype);
WORD CAH_EM4_DeInitEntity (DWORD dwPos);
WORD CAH_EM4_Initialize(DWORD dwPos);




# 20 "cah_126e1.c" 2






















 


 


 



 


typedef int (*CahEm4pTransFunc) (ENTITY_T);
 














typedef struct
{
    INT32 id;
    INT8 *pName;
    INT32 pri;
    INT32 opt;
    INT32 stckSz;
    FUNCPTR pEntryPtr;
}
CAH_EM4_TSK_CTL_BLK_T;


int buffer_reset (void);



extern int sysClkRateGet (void);
extern unsigned long GetSystemTime (void);

unsigned short em4_bpc_rd (int page, int off);
unsigned short em4Map2Wr (int page, int off, unsigned short val);
unsigned short em4Map1Wr (int page, int off, unsigned short val);
unsigned short em4MapRd (int page, int off, unsigned short *pU1, unsigned short *pU2);
unsigned short em4Map2Rd (int page, int off);
unsigned short em4Map1Rd (int page, int off);
 


int cah_em4_hash (int sn);
void cah_em4_isrFunc (FPGA_HANDLE handle, FPGA_HOOK_ARG entity);
void cah_em4_EnableLatchInt (UINT32 entity, BOOL Enable);
void cah_em4_latch ();
void cah_em4_Dpr ();
void cah_em4_readAlarm (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort);
E1_CALL *cah_em4_findCall2 (ENTITY_T entity, void *pfun, int arg1   ,
                                   int arg2   , int arg3   , int sn);
CAH_EM4_ENTITY_DATA_T *cah_em4_fd2entptr (int fd);
int cah_em4_trace (int level, char *msg, ...);
int cah_em4_ent2fd (ENTITY_T ent);
int cah_em4_pohWrite (void *base, int port, unsigned short reg, unsigned short val);
CAH_EM4_ENTITY_DATA_T *cah_em4_getEntityPtr (ENTITY_T entity);
int cah_em4_registerCall (PMyFun pFun, int arg3, int arg4, int arg5, int sn, char *fname);
unsigned short cah_em4_pohRead (void *base, int port, unsigned short reg);
void *cah_em4_getEntityBase (ENTITY_T ent, int port);
void cah_em4_forceAis (void *baseaddr, int port, eEM4SWITCH ais);
CAH_EM4_ENTITY_DATA_T *cah_em4_getEntity (ENTITY_T ent);
void em4_PollBit (DWORD dwPos, DWORD addrOffset, int byPortNo);

void cah_em4_getprbsname (int, char *);

DBG_MODULE_T gCahEm4DbgModNum;   
DBG_INFO_T gCahEm4DbgInfo = {    
    "EM4",                       
    {
      
      
      
     {"ERROR", 1, "error occurs."},
     {"INIT", 0, "Init info."},
     {"CFGGET", 0, "Get config information."},
     {"CFGSET", 0, "Set config information."},
     {"ALM", 0, "Alarm report information."},
     {"PMON", 0, "Performance information. "},
     {"INTR", 0, "Interrupt information. "},
     {"EGP", 0, "Egp information. "},
     {"OP", 0, "Operation information. "},
     }
};

 


CAH_EM4_ENTITY_TRIB_T gCahEm4TribVar;




CAH_EM4_ENTITY_DATA_T *gppCahEm4CfgVar[((1) )  + 1];
CAH_CFG_INFO_T gCahEm4CfgData[((1) )  + 1];

 



unsigned int g_cah_em4entity = 0;

 


unsigned int g_isrcount = 0;

 


CahEm4pTransFunc g_pTransFunc;

 


E1_CALL *gpCahEm4HashHead[((1) )  + 1][11 + 1] = { {0} };
E1_CALL gCahEm4Head[((1) )  + 1] = { {((void *)0) , 0, 0, 0, {0}, 0, 0, 0} };
E1_CALL *pCahEm4Head[((1) )  + 1];
int gCahEm4LinkFlag = 0;

 
static char g_aname[32];

 
int g_fflag = 1;

 


int gCahEm4SelectEnt = 5;
int gCahEm4SelectPort = 0;

  int g_UseInt = 1;

 
int g_waste = 0, g_wasetCnt = 10;


 





int g_crccnt = 8;
 


int gggg_cnt = 0;
UINT32 g_StampStart = 0, g_StampStop = 0;

 


int g_cnt = 14;
int g_flaggg = 0;

int nnn = 1;
int g_delay = 0;
int g_port = 0;

 



int g_testcrc = 0;
int cah_em4_blockupais = 0;
BOOL gbCahEm4RunPollingTask[3 + 1] = { 1 , 1 , 1 , 1  };
FMA_AlmModDefectFunc cah_em4_gpFunRptAlm = ((void *)0) ;
CPA_LATCHSTATE_CHANGE_FUNC cah_em4_gpFunRptLatch = ((void *)0) ;
PRA_SetPortStatus_Func cah_em4_gpFunRptPRBS = ((void *)0) ;

int gCahEm4Flag[((1) )  + 2] = { 0 };

 
char g_CahEm4Des[] = "  gCahEm4DebugLevel ::= {        1:      ,         2: how cah_em4_AlmPollingTask () is running now ?         3: how cah_em4_set_global_register () is running now ?                 how cah_em4_StopPolling ... ?                 what's the EM4_GetFarCRCErrCount () result ?                 what's the EM4_GetCRCErrCount () result ?         4: what's the cah_em4_ReportAlarm () result ?         5: none,         6: none,         7: none,         8: none,         9: 10: 11: how cah_em4_AlmPollingTask () is polling in detail ?         12: none,         13: how cah_em4_ClearAlarm ()/cah_em4_RiseAlarm () is running now ?         14: what's the indirect register's content in cah_em4_read_hardware_alarm () ?         15: watch EM4_EnablePlm ()         16: watch how is cah_em4_ReadPrbsStatus () running ?         17: watch EM4_SetTransTti ()         18: HOW EM4_GetLpPmPara () IS RUNNING NOW?         19: how are EM4_PrbsEnable ()/EM4_SetPrbsMode ()/cah_em4_getprbsstatus ()              running now ?         20: how is cah_em4_Dpr () running now ?         21: how is EM4_GetSlip () running now ?         22: how is cah_em4_latch () running now ?         23: how is EM4_GetPDHPM () running now ?         24: is there any PDH-NOTHING/SDH-NOTHING ?         33: ReadAlarm and ReportAlamr debug info.                                    // operated by polling g_testcrc(0) // control for test crc-error-counter g_UseInt=0; // if(g_UseInt == 1) the use polling to collect data,                // or use 1hz-interrupt only. int g_waste=0, g_wasetCnt=10; int g_pollcnt=0, g_hdcnt=0, }        " ;

const char *cah_em4_fmsg[] = {
    "__________________________________________________________________",
    "unsigned short\tem4Map1Wr\t(int page, int off, unsigned short val)",
    "unsigned short\tem4MapRd\t(int page, int off)",
    "unsigned short\tem4_bpc_rd\t(int page, int off)", 
    "unsigned short\tem4_bpc_wr\t(int page, int off)", 
    " ",    
    "void\tem4help\t\t(void)",
    "int\tem4BpcShow\t(void)",
    "int\tem4MapShow\t(void)",
    "int\tem4ShowCons\t(int port)",
    "int\tem4ShowEnable\t(int port)",
    "int\tem4AllInLoop\t(int flag) //backplane inloop",
    " ",
    "int\tem4ShowIntMask\t(int port)",
    "int\tem4ShowPageAddr\t(void)",
    "int\tem4ShowAlarms\t(int port)",
    "int\tem4ShowPortMap\t(void)",
    "int\tem4TtiRxTest\t(int port)",
    "int\tem4TtiTxTest\t(int port)",
    "int\tem4SetExpPslTest\t(int port, int c2)", 
    "int\tem4GetExpPslTest\t(int port)",
    "int\tem4GetExpTtiTest\t(int port)",
    "int\tem4SetExpTtiTest\t(int port)", 
    " ",
    "__________________________________________________________________",
    "",
    ((void *)0) 
};

 
static int g_phyportmap [130];
int em4ShowPortMap (void)
{
    int i = 0;

    for (i = 0; i < 130; i ++) {
        if (g_phyportmap[i] != i) {
            printf ("\r\ng_phyportmap[%3d]\t=\t[%3d]", i, g_phyportmap[i]);
        }
    }

    printf ("\r\n");
    return 0;
}


int
em4showsample (void)
{
    unsigned short u = 0;

    cah_ReadRegister (0x50000000, ((0x00 << 10) | 0x00) , &u, 0xffff, CAH_REG_ATTRBT_RO);
    printf ("\r\n%s(): devid(%x)", __FUNCTION__, u);

    cah_ReadRegister (0x50000000, (((0x00 << 10) | 0x01)) , &u, 0xffff, CAH_REG_ATTRBT_RO);
    printf ("\r\n%s(): PAGE0(%x)", __FUNCTION__, u);

    cah_ReadRegister (0x50000000, ((0x00 << 10) | 0x05) , &u, 0xffff, CAH_REG_ATTRBT_RO);
    printf ("\r\n%s(): EM4_IRQ_IN(%x)", __FUNCTION__, u);

    cah_ReadRegister (0x50000000, ((0x00 << 10) | 0x0a) , &u, 0xffff, CAH_REG_ATTRBT_RO);
    printf ("\r\n%s(): EM4_MON_INTER(%x)", __FUNCTION__, u);

    cah_ReadRegister (0x50000000, ((0x00 << 10) | 0x0b) , &u, 0xffff, CAH_REG_ATTRBT_RO);
    printf ("\r\n%s(): EM4_TIMING_SEL(%x)", __FUNCTION__, u);

    cah_ReadRegister (0x50000000, ((0x00 << 10) | 0x10) , &u, 0xffff, CAH_REG_ATTRBT_RO);
    printf ("\r\n%s(): EM4_TEST(%x)", __FUNCTION__, u);

    printf ("\r\n");
    return 0;
}



int g_cahem4version = 30;
int g_CahEm4Delay = 2;


CAH_EM4_TSK_CTL_BLK_T cah_em4_PollingTask = {
    0x00,                        
    "cahem4Poll",                
    231 ,   
    0x00,                        
    20480,                       
    (FUNCPTR) ((void *)0) 
};

CAH_EM4_TSK_CTL_BLK_T cah_em4_EpgTask = {
    0x00,                        
      "cahem4Epg",
     
    230 ,   
    0x00,                        
    20480,                       
    (FUNCPTR) cah_em4_latch
};


CAH_EM4_TSK_CTL_BLK_T cah_em4_DprTsk = {
    0x00,                        
      "cahem4Dpr",
     
    230 ,   
    0x00,                        
    20480,                       
    (FUNCPTR) cah_em4_Dpr
};


 



int gCahEm4PrbsPort = 0;

 
int g_wait = 0;

 

int gHardSlot = 0;
int gHardPort = 0;


int gCahEm4DebugLevel = 0;


 






WORD
cah_em4_deInitEntity (DWORD dwPos)
{
    WORD wRtn = ((WORD)0x0) ;

     
    memset (&gCahEm4CfgData[0], 0, sizeof (gCahEm4CfgData));
    CAH_EM4_DeInitEntity (0);

     
    return wRtn;
}



 




int
cah_em4_mapPort (int port)
{
    return ((port > 62) ? ((port + 1) - 63) : (port + 1));
}


int g_logbackn = 0;
 




int
cah_em4_port2chip (int port)
{
    if (0 > port || 125 < port)
    {
        printf ("\r\n%s: port(%d) overflow!!!", __FUNCTION__, port);
        if (g_logbackn == 1)
        {
            logBackN ("cah_em4_port2chip ", 5);
        }
    }

    return (0 <= port && 62 >= port) ? 1 : 2;
}


int
cah_em4_dwPos2id (DWORD dwPos)
{
    int id = 0;

    return (id);
}

 


int
cah_em4_dwPos2ent (DWORD dwPos)
{
    return (cah_em4_dwPos2id (dwPos));
}

 




PB_ERROR_CODE_T
cah_em4_initModule ()
{
    static int flag = 1;
    UINT8 slot;

    if (0 == flag)
    {
        return ((WORD)0x0) ;
    }
    flag = 0;

    gCahEm4DbgModNum = DBG_RegisterModule (&gCahEm4DbgInfo);

    for (slot = 0; slot < ((1) ) ; slot++)
    {
        gCahEm4TribVar.EM4LatchForceRpt[slot] = 1 ;
		 


        gppCahEm4CfgVar[slot] = ((void *)0) ;
        printf ("\r\n%s(): set gppCahEm4CfgVar[%d] to 0", __FUNCTION__, slot);
    }

    return ((WORD)0x0) ;
}




 






PB_ERROR_CODE_T
cah_em4_loadModule ()
{
    PB_ERROR_CODE_T pbRtn = ((WORD)0x0) ;

    memset (&gCahEm4CfgData, 0, sizeof (gCahEm4CfgData));
    cah_em4_initModule ();

    return pbRtn;
}

int
cah_em4_getChipBase (int slot, int chipno)
{
    int base = 0;

    base = 0;

    return (base);
}

 






void
cah_em4_Info ()
{
    DWORD dwPos;

    for (dwPos = 0; dwPos < ((1) ) ; dwPos++)
    {
        if (CTD_MODE_E1 == gCahEm4CfgData[dwPos].ePortType[0]
            || CTD_MODE_DS1 == gCahEm4CfgData[dwPos].ePortType[0])
        {
            printf ("EntityNo[%d],portType[%d],dwBaseAddr[0x%x].\r\n",
                    gCahEm4CfgData[dwPos].dwEntityNo, gCahEm4CfgData[dwPos].ePortType[0],
                    gCahEm4CfgData[dwPos].dwBaseAddr);
        }
    }
}

 


unsigned short
cah_em4_rd (void *base, int offset)
{
    unsigned short ud = 0xcc;

    ud = *((volatile unsigned short *) (base) + offset);

    return (ud);
}


 


int
cah_em4_wr (void *base, int offset, unsigned short data)
{
    *((volatile unsigned short *) (base) + offset) = data;

    return 0;
}

 



int
cah_em4_enableLatchIntr (void *baseaddr, int flag)
{
    unsigned short u = 0;

    if (flag)
    {
        u = 0x0001;              
    }
    else
    {
        u = 0x0000;
    }

    cah_WriteRegister ((DWORD) baseaddr, ((0x00 << 10) | 0X09) , u, 0x00ff, CAH_REG_ATTRBT_RW);



    return (((WORD)0x0) );
}


 


void
cah_em4_pollBitDmapper (unsigned short *baseaddr, int offset)
{
    int i = 0;
    unsigned short u = 0x0000;

    for (;;)
    {
        u = cah_em4_rd (baseaddr, offset);
        if ((u & 0x8000))
        {
            i = i + 1;
            if (i >= 100)
            {
                printf ("\r\n%s(): FATAL ERROR ON INDRECT REG ACCESS", __FUNCTION__);
                break;
            }
        }
        else
        {
            break;
        }
    }
}


 


void
cah_em4_pollBit (unsigned short *baseaddr, int offset)
{
    int i = 0;
    unsigned short u = 0x0000;

    for (;;)
    {
        u = cah_em4_rd (baseaddr, offset);
        if ((u & 0x8000))
        {
            i = i + 1;
            if (i >= 100)
			{
				printf ("\r\n%s():\t register unstable forever!!!", __FUNCTION__);
                break;
			}
        } else {
            break;
        }
    }
}


 


void
cah_em4_enablePlm (void *baseaddr, UINT32 ulChip, UINT32 ulPort, int mode)
{
    unsigned short u = 0x0000;

     


    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 ,          0x8000 + (ulPort << 8) + 0x00 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );

    u &= ~0x0004;
    u |= (mode ? 1 : 0) << 2;

    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);

    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (ulPort << 8) + 0x00 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );

     


    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 ,   0x8000 + (ulPort << 8) + 0x01 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );

    if (1 == mode)
    {
         
        u &= ~0x0040;
        u |= 0x0010;
    }
    else
    {
        u &= ~0x0040;
        u &= ~0x0010;
    }
    u |= (0x0020 | 0x0080);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (ulPort << 8) + 0x01 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
}

 


int
cah_em4_setPrbs (void *baseaddr, eEM4SWITCH flag)
{
    unsigned short u = 0;

    if (eEM4_ENABLE == flag)
    {
        u |= 0x0001;
    }
    else
    {
        u &= ~0x0001;
    }
    cah_em4_wr (baseaddr, (0x04 << 10) | 0x47 , u);
    return (((WORD)0x0) );
}

 



void
cah_em4_enableTim (void *baseaddr, UINT32 ulChip, UINT32 ulPort, int mode)
{
    unsigned short u = 0x0000;

     


    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 ,         
                0x8000 + (ulPort << 8) + 0x00 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );

    u &= ~(0x0001 + 0x0080);
    u |= (mode ? (1 + 0x0080) : 0);

    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (ulPort << 8) + 0x00 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );

     


    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 ,   0x8000 + (ulPort << 8) + 0x01 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );

    if (1 == mode)
    {
        u |= 0x0008;
        u |= 0x0004;
    }
    else
    {
        u &= ~0x0008;
        u &= ~0x0004;
    }

    u |= (0x0020 | 0x0080);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (ulPort << 8) + 0x01 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
}
 



void
cah_em4_enableTimRdi (void *baseaddr, UINT32 ulChip, UINT32 ulPort, int mode)
{
    unsigned short u = 0x0000;

     


    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 ,         
                0x8000 + (ulPort << 8) + 0x00 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );

    u &= ~(0x0001 + 0x0080);
    u |= (mode ? (1 + 0x0080) : 0);

    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (ulPort << 8) + 0x00 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );

     


    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 ,   0x8000 + (ulPort << 8) + 0x01 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );

    if (1 == mode)
    {
        u |= 0x0008;
         
    }
    else
    {
        u &= ~0x0008;
         
    }

    u |= (0x0020 | 0x0080);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (ulPort << 8) + 0x01 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
}

 



void
cah_em4_enableTimAis (void *baseaddr, UINT32 ulChip, UINT32 ulPort, int mode)
{
    unsigned short u = 0x0000;

     


    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 ,         
                0x8000 + (ulPort << 8) + 0x00 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );

    u &= ~(0x0001 + 0x0080);
    u |= (mode ? (1 + 0x0080) : 0);

    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (ulPort << 8) + 0x00 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );

     


    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 ,   0x8000 + (ulPort << 8) + 0x01 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );

    if (1 == mode)
    {
         
        u |= 0x0004;
    }
    else
    {
         
        u &= ~0x0004;
    }

    u |= (0x0020 | 0x0080);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (ulPort << 8) + 0x01 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
}


 


void
cah_em4_setTtiMode (void *baseaddr, UINT32 ulChip, UINT32 ulPort, int mode)
{
    unsigned short u = 0x0000;

    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 ,   0x8000 + (ulPort << 8) + 0x01 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );

    u &= ~0x0001;
    u |= mode;

    u |= (0x0020 | 0x0080);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (ulPort << 8) + 0x01 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
}


 



void
cah_em4_setPrbs2 (void *baseaddr, int port, eEM4SWITCH flag)
{
     


    cah_em4_wr (baseaddr, (0x04 << 10) | 0x30 , port);
    cah_em4_setPrbs (baseaddr, flag);
}


 


void
cah_em4_initERdi (void *baseaddr, eEM4SWITCH flag)
{
     


    unsigned short u = 0;


    cah_ReadRegister ((DWORD) baseaddr, (0x04 << 10) | 0x42 , &u, 0xffff, CAH_REG_ATTRBT_RO);



    if (eEM4_ENABLE == flag)
    {
        u |= 0x0001;
    }
    else
    {
        u &= ~0x0001;
    }


    cah_WriteRegister ((DWORD) baseaddr, (0x04 << 10) | 0x42 , u, 0xffff, CAH_REG_ATTRBT_RW);



}


 




void
cah_em4_forceAis (void *baseaddr, int port, eEM4SWITCH ais)
{
    unsigned short u1 = 0;

    cah_em4_wr (baseaddr, (0x04 << 10) | 0x30 , port);
    u1 = cah_em4_rd (baseaddr, (   (0x04 << 10) | 0x40 ));
    if (gCahEm4SelectPort == port)
        cah_em4_trace (40 , "%s: base(%x), offset(%x), port(%d), u1(%x)",
                       __FUNCTION__, baseaddr, (0x04 << 10) | 0x40 , port, u1);

    if (eEM4_ENABLE == ais)
    {
        if (u1 & 0x8000)
        {
            if (gCahEm4SelectPort == port)
                cah_em4_trace (40 , "already set, skip");
            goto endfrcais;
        }
        else
        {
            if (gCahEm4SelectPort == port)
                cah_em4_trace (40 , "set tu-asi insertion");
            u1 |= 0x8000;
        }
    }
    else
    {
        if (u1 & 0x8000)
        {
            if (gCahEm4SelectPort == port)
                cah_em4_trace (40 , "clear tu-asi insertion");
            u1 &= ~0x8000;
        }
        else
        {
            if (gCahEm4SelectPort == port)
                cah_em4_trace (40 , "already cleared, skip");
            goto endfrcais;
        }
    }
    cah_em4_wr (baseaddr, (   (0x04 << 10) | 0x40 ), u1);
  endfrcais:

  return;
}

void
cah_em4_forceRdi (void *baseaddr, int port, eEM4SWITCH rdi)
{
    unsigned short u = 0;

    cah_em4_wr (baseaddr, (0x04 << 10) | 0x30 , port);
    u = cah_em4_rd (baseaddr, (0x04 << 10) | 0x40 );

    if (eEM4_ENABLE == rdi)
    {
        if ((u & 0x0001) && !(u & 0x0002))
        {
            goto endFrcrdi;
        }
        else
        {
            u |= 0x0001;
            u &= ~0x0002;
        }
    }
    else
    {
        if ((u & 0x0002) && !(u & 0x0001))
        {
            goto endFrcrdi;
        }
        else
        {
            u |= 0x0002;
            u &= (~0x0001);
        }
    }
    cah_em4_wr (baseaddr, (0x04 << 10) | 0x40 , u);
  endFrcrdi:

  return;
}

int
cah_em4_initReiRdiRfi (void *baseaddr, eEM4SWITCH rei, eEM4SWITCH rdi, eEM4SWITCH rfi)
{
    unsigned short u = 0;

    u = cah_em4_rd (baseaddr, (0x04 << 10) | 0x40 );

    if (eEM4_ENABLE == rei)
    {
        u |= 0x0080;
    }
    else
    {
        u &= (~0x0080);
    }


    if (eEM4_ENABLE == rdi)
    {
        u |= 0x0002;
    }
    else
    {
        u &= (~0x0002);
    }

    if (eEM4_ENABLE == rfi)
    {
        u |= 0x0020;
    }
    else
    {
        u &= (~0x0020);
    }

    cah_em4_wr (baseaddr, (0x04 << 10) | 0x40 , u);
    return (((WORD)0x0) );

}


int
cah_em4_initPsl (void *baseaddr, unsigned short psl)
{
    unsigned short u = 0;

    u = cah_em4_rd (baseaddr, (0x04 << 10) | 0x40 );
    u &= 0xffe3;
    u &= ~0x0020;                
    u |= ((psl & 0x0007) << 2);
    cah_em4_wr (baseaddr, (0x04 << 10) | 0x40 , u);
    return (((WORD)0x0) );
}

void
em4SetPsl2 (void *baseaddr, unsigned short port, unsigned short psl)
{
    unsigned short u = 0;

    cah_em4_wr (baseaddr, (0x04 << 10) | 0x30 , port);
    u = cah_em4_rd (baseaddr, (0x04 << 10) | 0x40 );
    u &= 0xffe3;                 
    u &= ~0x0020;
    u |= ((psl & 0x0007) << 2);
    cah_em4_wr (baseaddr, (0x04 << 10) | 0x40 , u);
}


void
em4SetPsl (int ent, unsigned short port, unsigned short psl)
{
    void *baseaddr = ((void *)0) ;

    if (((void *)0)  == cah_em4_getEntity (ent))
    {
        printf ("\r\nent(%d) NOT active", ent);
        return;
    }

    baseaddr = cah_em4_getEntityBase (ent, port);

    em4SetPsl2 (baseaddr, port, psl);
}



int
cah_em4_setTxj2 (void *baseaddr, const unsigned char *j2)
{
    int r = 0;

    if (((void *)0)  == j2)
    {
        printf ("\r\nem4_set_j2: j2 NULL");
        return (r = -1);
    }

    cah_em4_wr (baseaddr, (((0x04 << 10) | 0x50 ) + 0), (unsigned short) j2[0] | (j2[1] << 8));
    cah_em4_wr (baseaddr, (((0x04 << 10) | 0x50 ) + 1), (unsigned short) j2[2] | (j2[3] << 8));
    cah_em4_wr (baseaddr, (((0x04 << 10) | 0x50 ) + 2), (unsigned short) j2[4] | (j2[5] << 8));
    cah_em4_wr (baseaddr, (((0x04 << 10) | 0x50 ) + 3), (unsigned short) j2[6] | (j2[7] << 8));
    cah_em4_wr (baseaddr, (((0x04 << 10) | 0x50 ) + 4), (unsigned short) j2[8] | (j2[9] << 8));
    cah_em4_wr (baseaddr, (((0x04 << 10) | 0x50 ) + 5), (unsigned short) j2[10] | (j2[11] << 8));
    cah_em4_wr (baseaddr, (((0x04 << 10) | 0x50 ) + 6), (unsigned short) j2[12] | (j2[13] << 8));
    cah_em4_wr (baseaddr, (((0x04 << 10) | 0x50 ) + 7), (unsigned short) j2[14] | (j2[15] << 8));

    return (((WORD)0x0) );
}

void
cahem4setTxj2_2 (void *baseaddr, unsigned short port, const unsigned char *j2)
{
    cah_em4_wr (baseaddr, (0x04 << 10) | 0x30 , port);
    cah_em4_setTxj2 (baseaddr, j2);
}


int
cah_em4_setConsequentAction (void *baseaddr, int port, eEM4SWITCH uneqrdi, eEM4SWITCH plmrdi,
                             eEM4SWITCH uneqais, eEM4SWITCH plmais, eEM4SWITCH timrdi,
                             eEM4SWITCH timais, int ttimode)
{
     



    unsigned short u = 0;

    if (eEM4_ENABLE == uneqrdi)
    {
        u |= 0x0080;
    }
    else
    {
        u &= ~0x0080;
    }

    if (eEM4_ENABLE == plmrdi)
    {
        u |= 0x0040;
    }
    else
    {
        u &= ~0x0040;
    }

    if (eEM4_ENABLE == uneqais)
    {
        u &= ~0x0020;
    }
    else
    {
        u |= 0x0020;
    }

    if (eEM4_ENABLE == plmais)
    {
        u &= ~0x0010;
    }
    else
    {
        u |= 0x0010;
    }


    if (eEM4_ENABLE == timrdi)
    {
        u |= 0x0008;
    }
    else
    {
        u &= ~0x0008;
    }

    if (eEM4_ENABLE == timais)
    {
        u &= ~0x0004;
    }
    else
    {
        u |= 0x0004;
    }

    if (eEM4_ENABLE == ttimode)
    {
        u &= ~0x0001;
    }
    else
    {
        u |= 0x0001;
    }

    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (port << 8) + 0x01 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );

    return (((WORD)0x0) );
}


int
cah_em4_setDegThreshold (void *baseaddr, int port, unsigned short threshold)
{
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , threshold);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (port << 8) + ((0x02 << 10) | 0x20 ));
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    return (((WORD)0x0) );
}



void
cah_em4_getTslNow (void *baseaddr, int port, unsigned short *tslexp)
{
    unsigned short u = 0;

    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 ,   0x8000 + (port << 8) + 0x0d );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );
    *tslexp = ((u & 0x7000) >> 12);
}

void
em4GetTslAcc (int ent, int port)
{
    unsigned short tsl;
    void *baseaddr = ((void *)0) ;

    if (((void *)0)  == cah_em4_getEntity (ent))
    {
        printf ("\r\nent(%d) NOT active", ent);
        return;
    }

    baseaddr = cah_em4_getEntityBase (ent, port);

    cah_em4_getTslNow (baseaddr, port, &tsl);
    printf ("\r\nent(%d), port(%d), accepted psl(0x%x)", ent, port, tsl & 0x0007);
    printf ("\r\n");
}

void
cah_em4_get_j2exp (void *baseaddr, int port, unsigned char *j2exp)
{
    unsigned short u = 0;
    int i = 0;

	printf ("\r\n%s(): base(%p), port(%d)", __FUNCTION__, baseaddr, port);
    for (i = 0; i < 8; i++)
    {
        cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0x8000 + (port << 8) + (0X20  + i));
        cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
        u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );
		j2exp[i * 2] = u & 0x00ff;
		j2exp[i * 2 + 1] = (u >> 8) & 0x00ff;
    }
}



void
cah_em4_set_j2exp (void *baseaddr, int port, const unsigned char *j2exp)
{
    unsigned short u = 0;
    int i = 0;

    for (i = 0; i < 8; i++)
    {
        u = j2exp[i * 2] + (j2exp[i * 2 + 1] << 8);
        cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
        cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (port << 8) + (0X20  + i));
        cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    }
}

 


int
em4ShowJ2Exp (int entity, int port)
{
    unsigned short u = 0;
    int i = 0;
    void *baseaddr = (void *) 0;

    baseaddr = cah_em4_getEntityBase (entity, port);

    if ((void *) 0 == baseaddr)
    {
        printf ("\r\nentity(%d) not active", entity);
        return (0);
    }
    for (i = 0; i < 8; i++)
    {

        cah_WriteRegister ((DWORD) baseaddr, (0x02 << 10) | 0x01 ,      
                           0x8000 + (port << 8) + (0X20  + i), 0x7f3f,
                           CAH_REG_ATTRBT_RW);




        cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );

        cah_ReadRegister ((DWORD) baseaddr, (0x02 << 10) | 0x03 , &u, 0xffff, CAH_REG_ATTRBT_RO);



        printf ("\r\nj2exp[%2d]=[%x]", i * 2, u & 0x00ff);
        printf ("\r\nj2exp[%2d]=[%x]", i * 2 + 1, (u & 0xff00) >> 8);
    }
    printf ("\r\n");

    return (0);
}


void
cah_em4_getJ2Acc (void *baseaddr, int port, unsigned char *j2exp)
{
    unsigned short u = 0;
    int i = 0;

    for (i = 0; i < 16; i++)
    {
        cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 ,     
                    0x8000 + (port << 8) + (0x10  + i));
        cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
        u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );
        *(j2exp + i) = u & (0x00ff);
         
    }
}


int
cah_em4_setIntMask (void *baseaddr, int port, eEM4SWITCH tsl, eEM4SWITCH rdi, eEM4SWITCH uneq,
                    eEM4SWITCH plm, eEM4SWITCH tim, eEM4SWITCH erdi, unsigned short tslexp)
{
    unsigned short u = 0;


    if (eEM4_ENABLE == tsl)
    {
        u |= 0x0100;
    }
    else
    {
        u &= ~0x0100;
    }

    if (eEM4_ENABLE == rdi)
    {
        u |= 0x0010;
    }
    else
    {
        u &= ~0x0010;
    }
    if (eEM4_ENABLE == uneq)
    {
        u |= 0x0008;
    }
    else
    {
        u &= ~0x0008;
    }
    if (eEM4_ENABLE == plm)
    {
        u |= 0x0004;
    }
    else
    {
        u &= ~0x0004;
    }
    if (eEM4_ENABLE == tim)
    {
        u |= 0x0001;
    }
    else
    {
        u &= ~0x0001;
    }
    if (eEM4_ENABLE == erdi)
    {
        u |= 0x0200;
    }
    else
    {
        u &= ~0x0200;
    }

    u &= ~0x3800;
    u |= ((tslexp & 0x0007) << 11);

    u |= 0xc000;                 

    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (port << 8) + 0x00 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    return (((WORD)0x0) );
}

void
em4SetPslexp (void *baseaddr, unsigned short port, unsigned short pslexp)
{
    unsigned short u = 0x0000;

    u = cah_em4_pohRead (baseaddr, port, 0x00 );
    u &= (~0x3800);
    u |= ((pslexp & 0x0007) << 11);

    cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , u);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (port << 8) + 0x00 );
    cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
}



int
cah_em4_set_inloopretiming (void *baseaddr, int port, eEM4SWITCH inloop, eEM4SWITCH retiming)
{
    unsigned short u = 0;

    u = cah_em4_rd (baseaddr, ((0x05 << 10) | 0x100 ) + port);
# 1433 "cah_126e1.c"


    if (eEM4_ENABLE == retiming)
    {
        u |= 0x0001;
    }
    else
    {
        u &= ~0x0001;
    }

    cah_em4_wr (baseaddr, ((0x05 << 10) | 0x100 ) + port, u);

    return (((WORD)0x0) );
}




UINT16 cah_em4_PollBusyBit(UINT32 dwBaseAddr, UINT32 dwOffset)
{
    UINT16    wcnt     = 0x0;
    UINT16    wdelay   = 0x0;
    UINT16    wdummy   = 0x0;
    
    while (((UINT16)(*(volatile UINT16*)( (UINT32)(dwBaseAddr + (dwOffset * 2)) )))  & 0x8000 )
    {
        if (wcnt++ > 50 )
        {
            return ((UINT16)0x4) ;
        }
        
        for (wdelay = 0; wdelay < 100 ; wdelay++)
        {
            wdummy = wdummy + wdelay;
        }
    }
    
    return ((WORD)0x0) ;
}

 
typedef enum
{
    EM4_REG_ATTRBT_RW   = 0x0,
    EM4_REG_ATTRBT_RO   = 0x1,
    EM4_REG_ATTRBT_WO   = 0x2,
    EM4_REG_ATTRBT_RC   = 0x3,
    EM4_REG_ATTRBT_WC   = 0x4,
}EM4_REG_ATTRBT_T;

UINT16 cah_em4_WriteRegister(UINT32 dwBaseAddr,         UINT32 dwOffset,         UINT16 wValue,         UINT16 wMask,         EM4_REG_ATTRBT_T eRegAttrbt)




{
    UINT32   dwAbsAddr          = 0x0;
    UINT16   wCurRegValue       = 0x0;
    UINT16   wShouldWrtValue    = 0x0;
    UINT16   wRtn               = ((WORD)0x0) ;

     
    dwAbsAddr = (UINT32)(dwBaseAddr + (dwOffset * 2));
     
    wCurRegValue = ((UINT16)(*(volatile UINT16*)( dwAbsAddr ))) ;
     
    wShouldWrtValue = (UINT16)((wValue & wMask) | (wCurRegValue & (~wMask)));
     
    ((UINT16)(*(volatile UINT16*)( dwAbsAddr )))  = wShouldWrtValue;

     
    
# 1517 "cah_126e1.c"


     
    return wRtn;
}



UINT16 cah_em4_ReadRegister(UINT32 dwBaseAddr, UINT32 dwOffset, UINT16 *pwValue, UINT16 wMask, EM4_REG_ATTRBT_T eRegAttrbt)
{
    UINT32   dwAbsAddr       = 0x0;
    UINT16   wCurRegValue    = 0x0;
    UINT16   wRtn            = ((WORD)0x0) ;

    if (((void *)0)  != pwValue)
    {
         
        dwAbsAddr = (UINT32)(dwBaseAddr + (dwOffset * 2));
         
        wCurRegValue = ((UINT16)(*(volatile UINT16*)( dwAbsAddr ))) ;
         
        *pwValue = (UINT16)(wCurRegValue & wMask);

         
        if (EM4_REG_ATTRBT_WC == eRegAttrbt)
        {
             
            ((UINT16)(*(volatile UINT16*)( dwAbsAddr )))  = wMask;
        }
        
         
        if (EM4_REG_ATTRBT_RW == eRegAttrbt)
        {          
            if (0xcccc!= ((UINT16)(*(volatile UINT16*)( ((0x00 << 10) | 0x10)  ))) )
            {
                wRtn = ((UINT16)0x2) ;
            }
        }
    }
    else
    {
        wRtn = ((UINT16)0x1) ;
    }
    
     
    return wRtn;
}



UINT16 cah_em4_IndRead(UINT32 dwbaseAddr ,     UINT32 dwaddrIndAddr ,     UINT32 addrIndData ,     UINT16 waddr ,     UINT16 *pwvalue,     UINT16 wMaskval,     UINT16 wMaskaddr)






{
    UINT16 wRtn    = ((WORD)0x0) ;

     
    wRtn |= cah_em4_PollBusyBit(dwbaseAddr, dwaddrIndAddr);
    if (((WORD)0x0)  != wRtn)
    {
        return ((UINT16)0x8) ;
    }

     
    wRtn |= cah_em4_WriteRegister(dwbaseAddr, dwaddrIndAddr, ((waddr & (~0x4000 )) | 0x8000 ), wMaskaddr, EM4_REG_ATTRBT_WO);
	
     
    wRtn |= cah_em4_PollBusyBit(dwbaseAddr, dwaddrIndAddr);
    if (((WORD)0x0)  != wRtn)
    {
        return ((UINT16)0x8) ;
    }

     
    wRtn |= cah_em4_ReadRegister(dwbaseAddr, addrIndData , pwvalue, wMaskval, EM4_REG_ATTRBT_RO);

    return ((WORD)0x0) ;
}



 



PB_ERROR_CODE_T
cah_em4_setPdhPortMode (ENTITY_T entity, UINT32 ulChip, UINT32 ulPort, PDH_PORT_MODE_T type)
{
    void *base = 0x0000;
    int port = 0;
    unsigned short u = 0x0000;
    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;

    pEntity = cah_em4_getEntityPtr (entity);
    cah_em4_registerCall ((PMyFun) cah_em4_setPdhPortMode, (int) ulPort, (int) type, 0, 11, __FUNCTION__);

    if (pEntity->state != eEm4EntAct)
    {
        return PB_ERR_STATE;
    }
    base = (void *) cah_em4_getEntityBase (entity, ulPort);
    u = cah_em4_pohRead (base, port = ulPort, 0x00 );
    u &= ~0x000c;                
    cah_em4_pohWrite (base, port, 0x00 , u);

    return ((WORD)0x0) ;
}

int cah_em4_gete1outloop (void * base, int port)
{
	unsigned short loop = 0;
	int byte = 0;
	int bit = 0;

	byte = (port / 16);
	bit = (port % 16);
	
	loop = cah_em4_rd (base, (((0x04 << 10) | 0x10 ) + byte));
# 1648 "cah_126e1.c"

    return (loop & (0x0001 << bit)) ? 1:0;
}


int cah_em4_e1outloop (void * base, int port, int enable)
{
	unsigned short loop = 0;
	int byte = 0;
	int bit = 0;

	byte = (port / 16);
	bit = (port % 16);
	
	loop = cah_em4_rd (base, (((0x04 << 10) | 0x10 ) + byte));
	if (enable)
	{
		loop |= (0x0001 << bit);
	} else {
		loop &= ~(0x0001 << bit);
	}

	cah_em4_wr (base, (((0x04 << 10) | 0x10 ) + byte), loop);

	return ((WORD)0x0) ;
}

int cah_em4_e1inloop (void * base, int port, int enable)
{
	unsigned short loop = 0;
	
	loop = cah_em4_rd (base, (((0x05 << 10) | 0x100 ) + port));
	if (enable)
	{
		loop |= (0x0002);
	} else {
		loop &= ~(0x0002);
	}

	cah_em4_wr (base, (((0x05 << 10) | 0x100 ) + port), loop);

	return ((WORD)0x0) ;
}
 


WORD
CAH_EM4_SetPortLoopback (DWORD dwPos, BYTE byPortNo, CTD_LOOPBACK_TYPE_T eLpbckMd)
{
    WORD wRtn = ((WORD)0x0) ;
	int port = 0;
	void * base = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    cah_em4_registerCall ((PMyFun) CAH_EM4_SetPortLoopback, (int) SDD_126xE1IOPortConvert(byPortNo), (int) eLpbckMd, 0, 16, __FUNCTION__);
	port = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
	base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));

    switch (eLpbckMd)
    {
    case CTD_LOOPBACK_TYPE_NONE:
		cah_em4_e1inloop (base, port, CTD_LOOPBACK_TYPE_NONE);
		cah_em4_e1outloop (base, port, CTD_LOOPBACK_TYPE_NONE);
        break;

    case CTD_LOOPBACK_TYPE_INWARD_NEAR:
        break;

    case CTD_LOOPBACK_TYPE_OUTWARD_FAR:
        break;

    case CTD_LOOPBACK_TYPE_OUTWARD_NEAR:
         
         

		cah_em4_e1inloop (base, port, eLpbckMd);
        break;

    case CTD_LOOPBACK_TYPE_INWARD_FAR:
         
         

		cah_em4_e1outloop (base, port, eLpbckMd);
		return ((WORD)0x0) ;
        break;

    default:
        wRtn = ((WORD)0x1) ;
		return wRtn;
    }

    return wRtn;
}

 


PB_ERROR_CODE_T
cah_em4_prbsEnable (ENTITY_T entity, UINT32 ulChip, UINT32 ulPort, BOOL bEnable)
{
    CAH_EM4_ENTITY_DATA_T *pEntity;
    PB_ERROR_CODE_T ret = ((WORD)0x0) ;
    void *base = 0x0000;
     
     


    if ((((  entity   >= ((1) ) )) || ((   ulChip  )  >=  (1) ) || ((   ulPort  ) >= (63 ) ) ) )
    {
        printf ("\r\n%s!  Entity= %d; File=%s; Line=%d  \r\n", __FUNCTION__, entity, "cah_126e1.c",
                1758);
        return PB_ERR_PARAM;
    }

    pEntity = cah_em4_getEntityPtr (entity);
    cah_em4_registerCall ((PMyFun) cah_em4_prbsEnable, (int) ulPort, (int) bEnable, 0, 16, __FUNCTION__);

    if (pEntity->state != eEm4EntAct)
    {
        return PB_ERR_STATE;
    }

    if (bEnable)
    {
         


        base = (void *) cah_em4_getEntityBase (entity, ulPort);
        cah_em4_setPrbs2 (base, ulPort, eEM4_ENABLE);

        ret = ((WORD)0x0) ;
        if (((WORD)0x0)  == ret)
        {
            pEntity->chip_var[ulChip].prbsCfg[ulPort].prbsMode = PRBS_MODE_POWER15;
            pEntity->chip_var[ulChip].prbsCfg[ulPort].prbsStatus = PRBS_STATUS_SYNCING;
            pEntity->chip_var[ulChip].prbsCfg[ulPort].prbsErrCnt = 0;
        }
    }
    else
    {
        base = (void *) cah_em4_getEntityBase (entity, ulPort);
        cah_em4_setPrbs2 (base, ulPort, eEM4_DISABLE);
        ret = ((WORD)0x0) ;
        if (((WORD)0x0)  == ret)
        {
            pEntity->chip_var[ulChip].prbsCfg[ulPort].prbsMode = PRBS_MODE_NA;
            pEntity->chip_var[ulChip].prbsCfg[ulPort].prbsStatus = PRBS_STATUS_NA;
            pEntity->chip_var[ulChip].prbsCfg[ulPort].prbsErrCnt = 0;
        }
    }

    if (((WORD)0x0)  != ret)
    {
        printf ("\r\n%s!  Entity= %d; File=%s; Line=%d  \r\n", __FUNCTION__, entity, "cah_126e1.c",
                1802);
        return ((WORD)0x1) ;
    }

    pEntity->chip_var[ulChip].prbsCfg[ulPort].bForceRpt = 1 ;
    cah_em4_trace (19, "%s: entity(%d), port(%d), flag(%d)", __FUNCTION__, entity, ulPort, bEnable);

    return ((WORD)0x0) ;
}

double cah_em4_wasteOfTime (void);


int g_ulport = 200;


UINT16
cah_em4_readPdhAlarm(void *base, int port)
{
    unsigned short val = 0, val1 = 0;
# 1833 "cah_126e1.c"

    int i = 0   ;

    g_waste = 1 + cah_em4_wr (base, (0x04 << 10) | 0x17 , port | 0x8000);   
    for (;;)
    {
if (0) taskDelay (1);            
else cah_em4_wasteOfTime ();
        val = cah_em4_rd (base, (0x04 << 10) | 0x17 );
        if (0 == (val & 0x8000))
        {
            break;
        }
        else
        {
            i = i + 1;
            if (i >= 100)
            {
                printf ("\r\n%s():\tbase(%p), port(%d), FATAL ERROR INDIRECT REG ACCESS ERROR !!!", __FUNCTION__, base, port);
                { { if (DBG_GetLevel(  gCahDbgModuleNo ,     CAH_DEBUG_LEVEL_ERROR  )) {printf("[%s] %d: ", "cah_126e1.c", 1852); printf   ("\r\ncah_em4_readPdhAlarm():\tbase(%p), port(%d), FATAL ERROR INDIRECT REG ACCESS ERROR !!!\r\n", base, port)  ;} if (DBG_NvGetLevel(  gCahDbgModuleNo ,     CAH_DEBUG_LEVEL_ERROR  ))    ; } ; } ;
                break;
            }
        }
    }

    val 	= cah_em4_rd (base, (0x04 << 10) | 0x1b ) & 0x0007;
    val1 	= cah_em4_rd (base, (0x04 << 10) | 0x1c ) & 0x0001;
    if (port == g_ulport)
    {
        printf ("\r\ncah_em4_readPdhAlarm:port(%d), val(%x), val1(%x)", port, val, val1);
    }

    val     = val & 0x0007;
    val1    = val1 & 0x0001;
	val 	= val | ((val1 & 0x0001) ? 0x0008 : 0x0000);

    return (val);
}


static double gCahEm4Time = 0;

double cah_em4_wasteOfTime ()
{
    static double ary [16];
    double r = 0.0;
    int i = 0;

    for (i = 0; i < 5; i ++)
    {
        r = ary[i] = ary[i+1] * ary[i+2] / ary[i+3];
    }

    gCahEm4Time = r;

    return r;
}

int g_inderr = 1;


UINT32
cah_em4_readCrc4RemoteReg (void *base, int port, UINT16 *pNear, UINT16 *pRemote)
{
    UINT16 val;
    
    int i = 0   ;
    g_waste = 1 + cah_em4_wr (base, (0x04 << 10) | 0x18 , port | 0x8000);   
    for (;;)
    {
        val = cah_em4_rd (base, (0x04 << 10) | 0x18 );
        if (0 == (val & 0x8000))
        {
            break;
        }
        else
        {
            i = i + 1;
if (0) taskDelay (1);            
else cah_em4_wasteOfTime ();
            if (i >= 100)
            {
                if (g_inderr)
                printf ("\r\n%s():\tbase(%p), port(%d), FATAL ERROR ON INDIRECT REG ACCESS", __FUNCTION__, base, port);
                break;
            }
        }
    }
    val = cah_em4_rd (base, (0x04 << 10) | 0x1a );
    *pNear = val;

    val = cah_em4_rd (base, (0x04 << 10) | 0x19 );
    val &= ~0xfc00;                
    *pRemote = val;

    return val;
}



UINT32
cah_em4_readCrc4Reg (void *base, int port)
{
    UINT16 val;
    








    int i = 0   ;
    g_waste = 1 + cah_em4_wr (base, (0x04 << 10) | 0x18 , port | 0x8000);   
    for (;;)
    {
        val = cah_em4_rd (base, (0x04 << 10) | 0x18 );
        if (0 == (val & 0x8000))
        {
            break;
        }
        else
        {
            i = i + 1;
if (0) taskDelay (1);            
else cah_em4_wasteOfTime ();
            if (i >= 100)
            {
                if (g_inderr)
                printf ("\r\n%s():\tbase(%p), port(%d), FATAL ERROR ON INDIRECT REG ACCESS", __FUNCTION__, base, port);
                break;
            }
        }
    }
    val = cah_em4_rd (base, (0x04 << 10) | 0x1a );

    return val;
}

# 2054 "cah_126e1.c"


# 2093 "cah_126e1.c"



unsigned short
cah_em4_getFarCrcErrCount (void *base, int port)
{
    unsigned short u = 0;








    int i = 0;
    cah_em4_wr (base, (0x04 << 10) | 0x18 , port | 0x8000);         
    for (;;)
    {
        u = cah_em4_rd (base, (0x04 << 10) | 0x18 );
        if (0 == (u & 0x8000))
        {
            break;
        }
        else
        {
            i = i + 1;
if (0) taskDelay (1);            
else cah_em4_wasteOfTime ();
            if (i >= 100)
            {
if (g_inderr)
                printf ("\r\n%s():\tbase(%p), port(%d), FATAL ERROR ON INDRECT REG ACCESS", __FUNCTION__, base, port);
                break;
            }
        }
    }
    u = cah_em4_rd (base, (0x04 << 10) | 0x19 );
    u &= ~0xfc00;                



    return u;
}

# 2161 "cah_126e1.c"


# 2216 "cah_126e1.c"



inline int
cah_em4_ent2chipno (ENTITY_T entity)
{
    return cah_em4_ent2fd (entity);
}



 


PB_ERROR_CODE_T
cah_em4_insertUpAis (ENTITY_T entity, UINT32 ulChip, UINT32 ulPort, BOOL bEnable)
{
    CAH_EM4_ENTITY_DATA_T *pEntity;
     


    if (cah_em4_blockupais && bEnable)
    {
        printf ("\r\n%s, has been blocked! entity%d ulChip%d ulPort%d \r\n", __FUNCTION__, entity,
                ulChip, ulChip);
        bEnable = 0 ;
    }

    if ((((  entity   >= ((1) ) )) || ((   ulChip  )  >=  (1) ) || ((   ulPort  ) >= (63 ) ) ) )
    {
        printf ("\r\n%s!  Entity= %d; File=%s; Line=%d  \r\n", __FUNCTION__, entity, "cah_126e1.c",
                2247);
        return PB_ERR_PARAM;
    }

    pEntity = cah_em4_getEntityPtr (entity);

    if (pEntity->state != eEm4EntAct)
    {
        printf ("\r\n%s!  Entity= %d; File=%s; Line=%d  \r\n", __FUNCTION__, entity, "cah_126e1.c",
                2256);
        return PB_ERR_STATE;
    }

    return ((WORD)0x0) ;
}

extern STATUS vxMemProbe(char * adrs, int mode, int length, char * pVal);

 



PB_ERROR_CODE_T
cah_em4_detectCardStateEx (CAH_EM4_ENTITY_DATA_T * pEntity, BOOL * pbActive)
{
    void *base = (void *) 0;
    unsigned short val = 0;
    static int flag = 1;
    volatile unsigned short *p = ((void *)0) ;

    *pbActive = 0 ;
     


	base = cah_em4_getEntityBase(0, 3);
    p = (unsigned short *)0x50000020;
    if (-1 == vxMemProbe((char *)p, 0 , 2, (char *)&val))
    {
 
        *pbActive = 0 ;
        return ((WORD)0x0) ;
    } else {
   
    }

    p = (unsigned short *)0x60000020;
    if (-1 == vxMemProbe((char *)p, 0 , 2, (char *)&val))
    {
    
        *pbActive = 0 ;
        return ((WORD)0x0) ;
    } else {
     
    }

     
    val = cah_em4_rd (base, (((0x00 << 10) | 0x01)) );
    if (0x28 == (val&0xff))
    {
        if (flag)
            printf ("\r\ndetect(TRUE), base(%x), val(%x)", (int) base, val);
        *pbActive = 1 ;
    }
    else
    {
        printf ("\r\ndetect(FALSE), base(%x), val(%x)", (int) base, val);
        *pbActive = 0 ;
    }

	base = cah_em4_getEntityBase(0, 77);
     
    val = cah_em4_rd (base, (((0x00 << 10) | 0x01)) );
    if (0x28 == (val&0xff))
    {
        if (flag)
            printf ("\r\ndetect(TRUE), base(%x), val(%x)", (int) base, val);
        *pbActive = 1 ;
    }
    else
    {
        printf ("\r\ndetect(FALSE), base(%x), val(%x)", (int) base, val);
        *pbActive = 0 ;
    }
 
    flag = 0;

    return ((WORD)0x0) ;
}


# 2481 "cah_126e1.c"



# 2504 "cah_126e1.c"


 





















 




void
cah_em4_showbuild (void)
{
    printf ("\r\n>>>>> EM4: this load is build at (%s):(%s)(%s) <<<<<\r\n", "Apr 13 2009", "15:08:05",
            __FUNCTION__);
}

 



int
cah_em4_show (char *fmt, ...)
{
    va_list argptr;
    int cnt;

    printf ("\r\n");
    (__builtin_next_arg (  fmt ), __builtin_memcpy ((  argptr  ), __builtin_saveregs (), sizeof(__gnuc_va_list)) ) ;
    cnt = vprintf (fmt, argptr);
    ((void)0) ;

    return (cnt);
}

 






int
em4help (void)
{
    int i = 0;

    printf ("\r\nEM4 version(%d)", g_cahem4version);

    while (cah_em4_fmsg[i] != ((void *)0) )
    {
        cah_em4_show ("%s", cah_em4_fmsg[i]);
        i++;
    }

    return 0;

}



 


int
cah_em4_portCfg2 (void *baseaddr)
{
    int port = 0;
    char *j2 = "FPGA_BENCH_T";
    int j = 0, i = 0;
    int r = 0;
    
    cah_em4_wr(baseaddr, (0x02 << 10) | 0x00 , 0x8000);
    taskDelay (10);
    cah_em4_wr(baseaddr, (0x02 << 10) | 0x00 , 0x0001);
    
    for (port = 0; port < (63 ) ; port++)
    {
         



		cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , 0xd3ff);
         

		cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000+(port << 8) + 0);
         

        taskDelay (2);
		cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , 0x01c8);
         

		cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000+(port << 8) + 1);
         
        taskDelay (2);

        if ((port << 10))
        {
            for (i = 0; i < 16; i++)
            {
                if (((WORD)0x0)  !=
                    (r = cah_em4_setDegThreshold (baseaddr, port, (j2[1] << 8) + 0x80 + j2[0])))
                {
                    printf ("\r\ncah_em4_setDegThreshold () failed");
                    return (r);
                }
                for (j = 1; j < 8; j++)
                {
                    if (((WORD)0x0)  !=
                        (r =
                         cah_em4_setDegThreshold (baseaddr, port,
                                                  (j2[j * 2 + 1] << 8) + j2[j * 2])))
                    {
                        printf ("\r\ncah_em4_setDegThreshold() failed");
                        return (r);
                    }
                }
            }
        }
        else
        {
            for (i = 0; i < 16; i++)
            {
                if (((WORD)0x0)  !=
                    (r = cah_em4_setDegThreshold (baseaddr, port, (j2[1] << 8) + 0x80 + j2[0])))
                {
                    printf ("\r\ncah_em4_setDegThreshold () failed");
                    return (r);
                }

                for (j = 1; j < 8; j++)
                {
                    if (((WORD)0x0)  !=
                        (r =
                         cah_em4_setDegThreshold (baseaddr, port,
                                                  (j2[j * 2 + 1] << 8) + j2[j * 2])))
                    {
                        printf ("\r\ncah_em4_setDegThreshold () failed");
                        return (r);
                    }
                }
            }
        }
    }

    return 0;
}

int
cah_em4_portCfg1 (void *baseaddr)
{
    char buffer[256];
    int port = 0;
    char *j2 = "FPGA_BENCH_T";
    int r = 0;
    unsigned short u = 0;


     


    for (port = 0; port < (63 ) ; port++)
    {
         



        cah_em4_wr (baseaddr, (0x04 << 10) | 0x30 , port);       
        cah_em4_wr (baseaddr, (0x04 << 10) | 0x40 , 0x8a);
        cah_em4_wr (baseaddr, (0x04 << 10) | 0x47 , 0);
        cah_em4_wr (baseaddr, (0x04 << 10) | 0x41 , 0);

         
        u = 0;





  
# 2727 "cah_126e1.c"

 
         sprintf (buffer, "%s%d", j2, port);
        if (((WORD)0x0)  != (r = cah_em4_setTxj2 (baseaddr, (const unsigned char *) buffer)))
        {
            printf ("\r\n%s(): cah_em4_setTxj2() failed", __FUNCTION__);
            return (r);
        }
    }
    return (((WORD)0x0) );
}



void
e1_init (void)
{
    printf ("\r\nInitialize 63-E1 Mapper (%x) : version (%x)(%x)", (0x50000000) , em4Map1Rd (0, 0),
            em4Map2Rd (0, 0));
    em4Map1Wr (0, 1, 0x29);
    taskDelay (sysClkRateGet () / 4);
    em4Map1Wr (0, 1, 0x28);

    taskDelay (sysClkRateGet () / 2);

    printf ("\r\nInitialize DCM monitor interval to 15ms ");
    em4Map1Wr (0, 0x0a, 0x3200);
}

int gCahEm4Print = 0;
int
cah_em4_printf (char *msg, ...)
{
    va_list argptr;
    int cnt = 0;

    if (0 == gCahEm4Print) 
    {
        taskDelay (3);
        return 0;
    }

    if (1 == gCahEm4Print)
    {
        printf ("\r\n");
        (__builtin_next_arg (  msg ), __builtin_memcpy ((  argptr  ), __builtin_saveregs (), sizeof(__gnuc_va_list)) ) ;
        cnt = vprintf (msg, argptr);
        ((void)0) ;
    }

    return (cnt);
}



void
e1_dcm_state (void *baseaddr)
{
    unsigned short v = 0;

    v = cah_em4_rd (baseaddr, ((0x00 << 10) | 0x0a) );
    if (v & 0x80)
    {
        printf ("\r\nDCM: locked");
    }
    else
    {
        printf ("\r\nDCM: UNlocked!");
    }
    printf ("\r\n monitored at interval 15 ms, resetted ");
}

 




















































extern UINT8 GetSlotNumber(void);



 





































































































































void cah_em4_e1desync_default(void *baseaddr)
{
    int i=0;   
    unsigned short u = 0;
    cah_em4_printf("\r\nDesync: set to 32-byte fifo with center 12");
    cah_em4_wr (baseaddr, (0x05 << 10) | 0x00 , 0x30c);
    cah_em4_wr (baseaddr, ((0x05 << 10) | 0x00 ) | 0x0c, 0);
    cah_em4_wr (baseaddr, ((0x05 << 10) | 0x00 ) | 0x0d, 0);
    cah_em4_wr (baseaddr, ((0x05 << 10) | 0x00 ) | 0x0e, 0);
    cah_em4_wr (baseaddr, ((0x05 << 10) | 0x00 ) | 0x0f, 0);
    
    cah_em4_printf ("\r\nInitial desynchronizer (macro): ");
    for (i = 0; i < (63 ) ; i++)
    {      
        u = cah_em4_rd (baseaddr, (0x05 << 10) |( 0x100+ i));
        u &= ~0x0007;
        cah_em4_wr (baseaddr, (0x05 << 10) |( 0x100+ i), u );        
    }

}
void cah_em4_e1sync_default(void *baseaddr)
{
      cah_em4_printf("\r\nsync: set LIU fall edge");
       
     cah_em4_wr (baseaddr, (0x04 << 10) | 0x00 , 0x0010);
     cah_em4_printf("\r\nall ports are set E1");
     cah_em4_wr (baseaddr, (0x04 << 10) | 0x0c, 0);
     cah_em4_wr (baseaddr, (0x04 << 10) | 0x0d, 0);
     cah_em4_wr (baseaddr, (0x04 << 10) | 0x0e, 0);
     cah_em4_wr (baseaddr, (0x04 << 10) | 0x0f, 0);
}
void  cah_em4_e1_tb_outloop_disable(void *baseaddr)
{    
    unsigned short u = 0;
    u=cah_em4_rd(baseaddr,(((0x00 << 10) | 0x01)) );
    u=u& 0xfffd;
    cah_em4_wr (baseaddr, (((0x00 << 10) | 0x01)) ,u);
     
    u=cah_em4_rd(baseaddr,(0x04 << 10) | 0x00 );
    u=u|0x0010;
    cah_em4_wr (baseaddr, (0x04 << 10) | 0x00 ,u);
    
}
void cah_em4_e1_poh_rx_mask_int(void *baseaddr)
{
    int i=0;
    cah_em4_printf ("\r\ndisable plmais and timais");
    for (i = 0; i < (63 ) ; i++)
    {                     
        cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , 0x1000);
        cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (i << 8) + 0x00 );
        cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    }
}


void cah_em4_e1_poh_set_degrade (void *baseaddr,int level,int thr,int period)
{

    cah_em4_printf ("\r\nSet poh rx degrade ...");
    cah_em4_wr(baseaddr, (0x02 << 10) | (0x020+2*(level&0x07)), (thr&0x7f)+((period&0xff)<<8)  );
    cah_em4_wr(baseaddr, (0x02 << 10) | (0x021+2*(level&0x07)), ((period&0xffff00)>>8)  );
}

void cah_em4_e1_poh_clr_degrade (void *baseaddr,int level,int thr,int period)
{

    cah_em4_printf ("\r\nSet poh rx degrade ...");
    cah_em4_wr(baseaddr, (0x02 << 10) | (0x030+2*(level&0x07)), (thr&0x7f)+((period&0xff)<<8)  );
    cah_em4_wr(baseaddr, (0x02 << 10) | (0x031+2*(level&0x07)), ((period&0xffff00)>>8)  );
}

int
cah_em4_setGlobalRegister (void *baseaddr)
{
    int r = 0;
    int j1_locked = 0;
    int i = 0;
     
     

    cah_em4_printf ("\r\n%s(): base(%p)", __FUNCTION__, baseaddr);
    
    
 
    cah_em4_printf ("\r\nInitialize 63-E1 Mapper (%p) : version (%x)", baseaddr, em4Map1Rd (0, 0));
    cah_em4_wr (baseaddr, (((0x00 << 10) | 0x01)) , 0x29);
     
    taskDelay (10);
    cah_em4_wr (baseaddr, (((0x00 << 10) | 0x01)) , 0x28);
     
    taskDelay (40);
    
     












    cah_em4_printf ("\r\nInitialize DCM monitor interval to 15ms ");
    cah_em4_wr(baseaddr, ((0x00 << 10) | 0x0a) , 0x3200);
    
    if ((((WORD)0x0)  != (r = cah_em4_portCfg1 (baseaddr))))
    {
        cah_em4_printf ("\r\n%s(): cah_em4_portCfg1() failed", __FUNCTION__);
        return (r);
    }



      
    
    cah_em4_trace (3, "e1j2 set");

    cah_em4_wr (baseaddr, (0x02 << 10) | 0x00 , 0x8000);
     
   taskDelay (sysClkRateGet () / 2);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x00 , 0x0001);
    taskDelay (sysClkRateGet () / 2);
    

    

    
    cah_em4_trace (3, "e1ram reset");
    if (((WORD)0x0)  != (r = cah_em4_portCfg2 (baseaddr)))
    {
        cah_em4_printf ("\r\ncah_em4_portCfg2 () failed");
        return (r);
    }



    cah_em4_trace (3, "e1rx&tx set");
    cah_em4_printf ("\r\nResetting mapper ...");
    cah_em4_wr (baseaddr, (0x04 << 10) | 0x00 , 0x0119);
     
    taskDelay (40);
    cah_em4_wr (baseaddr, (0x04 << 10) | 0x00 , 0x0118);
    
    cah_em4_printf ("\r\nwait for telecom bus stable ...");
    
     
    j1_locked = 0;
    while (j1_locked == 0)
    {
        j1_locked = cah_em4_rd (baseaddr, (0x05 << 10) | 0x00 ) & 0x80;
    }
     

    
    cah_em4_printf ("\r\nResetting demapper ...");
    cah_em4_wr (baseaddr, (0x05 << 10) | 0x00 , 0x8004);
     
     
    taskDelay (50);
    cah_em4_wr (baseaddr, (0x05 << 10) | 0x00 , 0x0004);
    cah_em4_printf ("\r\nResetting poh ...");
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x00 , 0x8000);
      
    taskDelay (50);
    cah_em4_wr (baseaddr, (0x02 << 10) | 0x00 , 0x0000);
    e1_dcm_state (baseaddr);
    cah_em4_printf ("\r\nWaiting DCM stablized ...");
     
    taskDelay (50);
    e1_dcm_state (baseaddr);
     
    
     
    cah_em4_e1desync_default(baseaddr);
    cah_em4_e1sync_default(baseaddr);
    
    cah_em4_printf ("\r\ndisable plmais and timais");
    for (i = 0; i < (63 ) ; i++)
    {                     
        cah_em4_wr (baseaddr, (0x02 << 10) | 0x02 , 0x01c8);
        cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0xc000 + (i << 8) + 0x01 );
        cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
    }


     cah_em4_e1_tb_outloop_disable(baseaddr);
     cah_em4_e1_poh_rx_mask_int(baseaddr);

     cah_em4_e1_poh_set_degrade (baseaddr,0,40,16000);
     cah_em4_e1_poh_clr_degrade(baseaddr,0,6,16000);
     	
     cah_em4_e1_poh_set_degrade (baseaddr,1,80,16000);     
     cah_em4_e1_poh_clr_degrade(baseaddr,1,8,16000);
     
     
    cah_em4_wr (baseaddr, (0x05 << 10) | 0x200 , (0x3f)  | ((0x3f)  << 8));
    cah_em4_wr (baseaddr, (0x05 << 10) | 0x201 , (0x3f)  | ((0x3f)  << 8));


    
    cah_Write8Reg (0x70000000 ,  
                    0x1b  + 0, 
                    0x07,  
                    0x07,  
                    CAH_REG_ATTRBT_RW);
    cah_Write8Reg (0x70000000 ,  
                    0x1b  + 1,  
                    0x07,  
                    0x07,  
                    CAH_REG_ATTRBT_RW);
    cah_Write8Reg (0x70000000 ,  
                    0x1b  + 2,  
                    0x07,  
                    0x07,  
                    CAH_REG_ATTRBT_RW);
    cah_Write8Reg (0x70000000 ,  
                    0x1b  + 3, 
                    0x07,  
                    0x07,  
                    CAH_REG_ATTRBT_RW);
    return ((WORD)0x0) ;
}




 


PB_ERROR_CODE_T
cah_em4_initChip (ENTITY_T entity)
{

    cah_em4_printf ("\r\n%s: ent(%d), base address(0x%x)", __FUNCTION__, entity, (int) (0x50000000) );
    cah_em4_printf ("\r\n%s: setGlobalRegister-1", __FUNCTION__);
    if (((WORD)0x0)  != cah_em4_setGlobalRegister ((void *) (0x50000000) ))
    {
        cah_em4_printf ("\r\n%s(): set map1 failed", __FUNCTION__);
        return ((WORD)0x1) ;
    }

    cah_em4_printf ("\r\n%s: ent(%d), base address(0x%x)", __FUNCTION__, entity, (int) (0x60000000) );
    cah_em4_printf ("\r\n%s: setGlobalRegister-2", __FUNCTION__);
    if (((WORD)0x0)  != cah_em4_setGlobalRegister ((void *) (0x60000000) ))
    {
        cah_em4_printf ("\r\n%s(): set map2 failed", __FUNCTION__);
        return ((WORD)0x1) ;
    }
    
    return ((WORD)0x0) ;
}





 



void
cah_em4_InitAlmID (CAH_EM4_ENTITY_DATA_T * pEntity, int ulChip, int ulPort)
{
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[0] = 34 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[1] = 32 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[2] = 36 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[3] = 39 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[4] = 37 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[5] = 38 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[6] = 91 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[7] = 92 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[8] = 42 ;
     


    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[9] = 232 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[10] = 233 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[11] = 235 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[12] = 230 ;
    pEntity->chip_var[ulChip].pohAlm[ulPort].almID[13] = 231 ;




}

void
cah_em4_initVars (CAH_EM4_ENTITY_DATA_T * pEntity, int chipNum)
{
    UINT32 ulChip, ulPort;

    for (ulChip = 0; (int)ulChip < chipNum; ulChip++)
    {
        for (ulPort = 0; ulPort < (63 ) ; ulPort++)
        {
             


            pEntity->chip_var[ulChip].bPlmEnable[ulPort] = 0 ;
            pEntity->chip_var[ulChip].bTimEnable[ulPort] = 0 ;
            pEntity->chip_var[ulChip].bRestrainRDI[ulPort] = 0 ;
            pEntity->chip_var[ulChip].bBlockAis[ulPort] = 0 ;
            pEntity->chip_var[ulChip].adminState[ulPort] = PORT_STATE_UP;
            pEntity->chip_var[ulChip].faisMode[ulPort] =   0;
            pEntity->chip_var[ulChip].frdiMode[ulPort] = EM4_NONE_FORCE;
            pEntity->chip_var[ulChip].loopMode[ulPort] = PORT_LOOP_NONE;
            pEntity->chip_var[ulChip].portType[ulPort] = PDH_MODE_E1;
            memset((INT1 *)( &pEntity->chip_var[ulChip].prbsCfg[ulPort] ), (  0 ), (  sizeof (EM4_PRBS_CONFIG_T) )) ;
            pEntity->chip_var[ulChip].prbsCfg[ulPort].prbsMode = PRBS_MODE_NA;

             


            pEntity->chip_var[ulChip].strExpJ2[ulPort].J2Mode = PbTIModeType_modeFixedOneByte;
            pEntity->chip_var[ulChip].strTranJ2[ulPort].J2Mode = PbTIModeType_modeFixedOneByte;
            pEntity->chip_var[ulChip].strRcvJ2[ulPort].J2Mode = PbTIModeType_modeFixedOneByte;
            memset((INT1 *)( pEntity->chip_var[ulChip].strExpJ2[ulPort].J2Data ), (  0 ), (  64  + 1 )) ;
            memset((INT1 *)( pEntity->chip_var[ulChip].strTranJ2[ulPort].J2Data ), (  0 ), (  64  + 1 )) ;
            memset((INT1 *)( pEntity->chip_var[ulChip].strRcvJ2[ulPort].J2Data ), (  0 ), (  64  + 1 )) ;

             


            pEntity->chip_var[ulChip].ExpPSL[ulPort] = 0;
            pEntity->chip_var[ulChip].RcvPSL[ulPort] = 0;
            pEntity->chip_var[ulChip].TranPSL[ulPort] = 0;

             


            pEntity->chip_var[ulChip].forceRptAlm[ulPort] = 1 ;
            pEntity->chip_var[ulChip].pohAlm[ulPort].ssf = 0;
            pEntity->chip_var[ulChip].pohAlm[ulPort].ssfFc = 0;
            pEntity->chip_var[ulChip].pohAlm[ulPort].rdi = 0;
            pEntity->chip_var[ulChip].pohAlm[ulPort].rfi = 0;
            pEntity->chip_var[ulChip].pohAlm[ulPort].rdiFc = 0;
            pEntity->chip_var[ulChip].pohAlm[ulPort].rfiFc = 0;
            pEntity->chip_var[ulChip].pohAlm[ulPort].e1rdiFc = 0;

            cah_em4_InitAlmID (pEntity, ulChip, ulPort);
        }
    }
}

int
cah_em4_chkdwPos (DWORD dwPos)
{
    return 0;
}

int
cah_em4_chkId (ENTITY_T entity, int port)
{
    unsigned short u = 0x0000;

    u = cah_em4_rd (cah_em4_getEntityBase (entity, port), ((0x00 << 10) | 0x00) );
    if (0x8915  != u)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

 






















int
cah_em4_revise (int isCall)
{
    E1_CALL *ppp = ((void *)0) , *ppp1 = ((void *)0) ;
	ENTITY_T entity = 0;
    int fd = 0;
    int j = 0;

    fd = cah_em4_ent2fd (entity);
    printf ("\r\n%s: entity(%d), fd(%d), isCall(%s)", __FUNCTION__, entity, fd,
            isCall ? "CALL" : "CLEAR");
    if (((void *)0)  == pCahEm4Head[fd])
    {
        printf ("\r\n%s, pointer NULL entity(%d)", __FUNCTION__, entity);
        return (0);
    }

    ppp1 = pCahEm4Head[fd];

    for (;;)
    {
        ppp = ppp1;
        if (1 == isCall)
        {
            if (((void *)0)  == ppp->p)
            {
                printf ("\r\npCahEm4Head[%d]->p NULL", fd);
                break;
            }
            else
            {
                 
                if (5 == ppp->sn || 6 == ppp->sn)
                {
                    (*(ppp->p)) (ppp->arg1, ppp->arg3, ppp->arg4.tti, ppp->arg5, ppp->arg6, ppp->arg2);
                }
                else
                {
                    (*(ppp->p)) (ppp->arg1, ppp->arg3, ppp->arg4.arg, ppp->arg5, ppp->arg6, ppp->arg2);
                }
            }
        }

        ppp1 = ppp1->pNext;
        if (0 == isCall)
        {
            if (((void *)0)  == pCahEm4Head[fd]->pNext)
            {
                printf ("\r\nEM4 link empty");
                break;
            }
            pCahEm4Head[fd] = pCahEm4Head[fd]->pNext;
            ppp->pNext = ((void *)0) ;
            free (ppp);
            ppp = ((void *)0) ;
        }
        if (ppp1)
        {
            if (((void *)0)  == ppp1->p)
            {
                printf ("\r\nem4 link end!!!");
                if (0 == isCall)
                    for (j = 0; j < 11; j++)
                    {
                        gpCahEm4HashHead[fd][j] = &gCahEm4Head[fd];
                    }
                break;
            }
            if (((void *)0)  == ppp1->pNext)
            {
                printf ("\r\nEM4 link empty");
                if (0 == isCall)
                    for (j = 0; j < 11; j++)
                    {
                        gpCahEm4HashHead[fd][j] = &gCahEm4Head[fd];
                    }
                break;
            }
        }
        else
        {
            printf ("\r\nlink end for NULL");
            if (0 == isCall)
                for (j = 0; j < 11; j++)
                {
                    gpCahEm4HashHead[fd][j] = &gCahEm4Head[fd];
                }
            break;
        }
    }

    return (0);
}



 


void
cah_em4_initLink (void)
{
    int i = 0, j = 0;

    for (i = 0; i < ((1) ) ; i++)
    {
        pCahEm4Head[i] = &gCahEm4Head[i];
        for (j = 0; j < 11; j++)
        {
            gpCahEm4HashHead[i][j] = &gCahEm4Head[i];
        }
        memset (&gCahEm4Head[i], 0, sizeof (E1_CALL));
    }
}



 


int
cah_em4_registerCall (PMyFun pFun, int port, int arg4, int arg5, int sn, char *fname)
{
    E1_CALL *p = ((void *)0) , *pold = ((void *)0) ;
    int fd = 0;
    int hash = 0;

    if (0 == gCahEm4LinkFlag)
    {
        cah_em4_initLink ();
        gCahEm4LinkFlag = 1;
    }
    fd = cah_em4_ent2fd (0);

if (1) return 0;

    pold = ((void *)0) ;

    pold = cah_em4_findCall2 (0, pFun, 0, 0, port, sn);
    if (pold)
    {
        pold->p = pFun;
        pold->arg1 = 0;
        pold->arg2 = 0;
        pold->arg3 = port;

         


        if (5 == sn || 6 == sn)
        {
            memcpy (pold->arg4.tti, (unsigned char *) arg4, 16);
        }
        else
        {
            pold->arg4.arg = arg4;
        }
        pold->arg5 = arg5;
        pold->arg6 = arg5;
        pold->sn = sn;

        return (0);
    }

    if (((void *)0)  == (p = (E1_CALL *) malloc (sizeof (E1_CALL))))
    {
        printf ("\r\n%s: malloc error", __FUNCTION__);
        return (0);
    }
    p->id = pCahEm4Head[fd]->id + 1;
    p->p = pFun;
    p->arg1 = 0;
    p->arg2 = 0;
    p->arg3 = port;
    memcpy (p->fname, fname, strlen (fname));
    if (5 == sn || 6 == sn)
    {
        memcpy (p->arg4.tti, (unsigned char *) arg4, 16);
    }
    else
    {
        p->arg4.arg = arg4;
    }
    p->arg5 = arg5;
    p->arg6 = arg5;
    p->sn = sn;

    p->pNext = pCahEm4Head[fd];
     
    hash = cah_em4_hash (sn);
    p->pHashNext[hash] = gpCahEm4HashHead[fd][hash];     
    pCahEm4Head[fd] = p;
    gpCahEm4HashHead[fd][hash] = p;      

    return (0);
}




 


int
cahem4tree (void)
{
    E1_CALL *ppp = ((void *)0) , *ppp1 = ((void *)0) ;
    int fd = 0;
	ENTITY_T entity = 0;

    if (((void *)0)  == cah_em4_getEntity (entity))
    {
        printf ("\r\nEnt(%d) NOT active", entity);
        return (0);
    }

    fd = cah_em4_ent2fd (entity);
    if (((void *)0)  == pCahEm4Head[fd])
    {
        printf ("\r\n%s, pointer NULL entity(%d)", __FUNCTION__, entity);
        return (0);
    }

    ppp1 = pCahEm4Head[fd];

    for (;;)
    {
        ppp = ppp1;
        if (1)
        {
            if (((void *)0)  == ppp->p)
            {
                printf ("\r\npCahEm4Head[%d]->p NULL", fd);
                break;
            }
            else
            {
                 
                if (5 == ppp->sn || 6 == ppp->sn)
                {
                    printf ("\r\nid(%2d), arg3(%d), arg4(%d), sn(%3d), n(%s)",
                            ppp->id, ppp->arg3, ppp->arg4.arg, ppp->sn,
                            ppp->fname);
                }
                else
                {
                    printf ("\r\nid(%2d), arg3(%d), arg4(%d), sn(%3d), n(%s)",
                            ppp->id, ppp->arg3, ppp->arg4.arg, ppp->sn,
                            ppp->fname);
                }
            }
        }

        ppp1 = ppp1->pNext;
        if (0)
        {
            pCahEm4Head[fd] = pCahEm4Head[fd]->pNext;
            ppp->pNext = ((void *)0) ;
            free (ppp);
            ppp = ((void *)0) ;
        }
        if (((void *)0)  == ppp1->p)
        {
            printf ("\r\nem4 link end!!!");
            break;
        }
    }

    return (0);
}




  










E1_CALL *
cah_em4_findCall2 (ENTITY_T entity, void *pfun, int arg1   ,
                   int arg2   ,
                   int arg3   ,
                   int sn)
{
    E1_CALL *ppp = ((void *)0) , *ppp1 = ((void *)0) ;
    E1_CALL *pr = ((void *)0) ;
    int fd = 0;
    int hash = 0;
     

    if (((void *)0)  == cah_em4_getEntityPtr (entity))
    {
        printf ("\r\nEnt(%d) NOT active", entity);
        return (((void *)0) );
    }

    hash = cah_em4_hash (sn);

    fd = cah_em4_ent2fd (entity);
    if (((void *)0)  == gpCahEm4HashHead[fd][hash])
    {
        printf ("\r\n%s, pointer NULL entity(%d)", __FUNCTION__, entity);
        return (((void *)0) );
    }

    ppp1 = gpCahEm4HashHead[fd][hash];

    for (;;)
    {
        ppp = ppp1;
        if (((void *)0)  == ppp->p)
        {
            printf ("\r\n%s(): pEm4HashHead[%d]->p NULL, func NOT found, first time!", __FUNCTION__, fd);
            break;
        }
        else
        {
             


            if (ppp1->p == pfun && ppp1->sn == sn && ppp1->arg1 == arg1 && ppp1->arg2 == arg2
                && ppp1->arg3 == arg3)
            {
                 
                pr = ppp1;
                cah_em4_trace (4, "\r\n%s: GOT ent(%d:%d), su(%d), port(%d)", __FUNCTION__, entity,
                               arg1, arg2, arg3);
                return (pr);
            }

        }

        ppp1 = ppp1->pHashNext[hash];

        if (((void *)0)  == ppp1->p)
        {
            break;
        }
    }

    return (((void *)0) );
}




  




int
cah_em4_hash (int sn)
{
    int r = 0;

    switch (sn)
    {
    case 4:
        r = 0;
        break;
    case 2:
        r = 1;
        break;
    case 8:
        r = 2;
        break;
    case 6:
        r = 3;
        break;
    case 99:
        r = 4;
        break;
    case 1:
        r = 5;
        break;
    case 5:
        r = 6;
        break;
    case 9:
        r = 7;
        break;
    case 111:
        r = 8;
        break;

    case 25:
    case 26:
        r = 9;
        break;

    default:
        r = 9;
        break;
    }

    return (r);
}


 


int
cahem4tree2 (int hash)
{
    E1_CALL *ppp = ((void *)0) , *ppp1 = ((void *)0) ;
    int fd = 0;
	ENTITY_T entity = 0;

    if (0 > hash || 10 < hash)
    {
        printf ("\r\nhash index error");
        return (0);
    }

    if (((void *)0)  == cah_em4_getEntity (entity))
    {
        printf ("\r\nEnt(%d) NOT active", entity);
        return (0);
    }

    fd = cah_em4_ent2fd (entity);
    if (((void *)0)  == gpCahEm4HashHead[fd][hash])
    {
        printf ("\r\n%s, pointer NULL entity(%d)", __FUNCTION__, entity);
        return (0);
    }

    ppp1 = gpCahEm4HashHead[fd][hash];

    for (;;)
    {
        ppp = ppp1;
        if (1)
        {
            if (((void *)0)  == ppp->p)
            {
                printf ("\r\ngpCahEm4HashHead[%d][%d]->p NULL", fd, hash);
                break;
            }
            else
            {
                if (5 == ppp->sn || 6 == ppp->sn)
                {
                    printf ("\r\nid(%4d), arg3(%d), arg4(%d), sn(%3d),n(%s)",
                            ppp->id, ppp->arg3, ppp->arg4.arg, ppp->sn,
                            ppp->fname);
                }
                else
                {
                    printf ("\r\nid(%4d), arg3(%d), arg4(%d), sn(%3d),n(%s)",
                            ppp->id, ppp->arg3, ppp->arg4.arg, ppp->sn,
                            ppp->fname);
                }
            }
        }

        ppp1 = ppp1->pHashNext[hash];
        if (0)
        {
            gpCahEm4HashHead[fd][hash] = gpCahEm4HashHead[fd][hash]->pHashNext[hash];
            ppp->pNext = ((void *)0) ;
            free (ppp);
            ppp = ((void *)0) ;
        }
        if (((void *)0)  == ppp1->p)
        {
            printf ("\r\nem4 link end!!!");
            break;
        }
    }

    return (0);
}

int em4GetPrbsStateTest (int port)
{
    BOOL s = 0 ;
    DWORD cnt = 0;

    {if (125 <  port ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  port ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;

    CAH_EM4_GetPrbsState (0, port, &s);
    CAH_EM4_GetPrbsErrCount (0, port, &cnt);
    printf ("\r\n%s(): status (%d), cnt(%d)", __FUNCTION__, s, cnt);
    printf ("\r\n");

    return 0;
}


WORD
CAH_EM4_GetPrbsState (DWORD dwPos, BYTE byPortNo, BOOL * bPrbsSucc)
{
    void *base = (void *) 0;
    short u = 0;
    int port = SDD_126xE1IOPortConvert(byPortNo);

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	port = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
    base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));
    cah_em4_wr (base, (0x05 << 10) | 0x20 , port & 0x003f);
    taskDelay (sysClkRateGet () / 10);
    u = cah_em4_rd (base, (0x05 << 10) | 0x20 );
	*bPrbsSucc = ((u & 0x8000) ? 1 : 0);

    return ((WORD)0x0) ;
}

void
cah_em4_getprbsname (int tempStatus, char *almType)
{
    switch (tempStatus)
    {
    case PRBS_STATUS_NA:
        (((char*) almType )[((int)(  4 + 1 ) <= 0) ? 0 : (  4 + 1 )-1] = '\0', strncpy( almType ,   "NONE" , ((int)(  4 + 1 ) <= 0) ? 0 : (  4 + 1 )-1)) ;
        break;
    case PRBS_STATUS_SYNCING:
        (((char*) almType )[((int)(  7 + 1 ) <= 0) ? 0 : (  7 + 1 )-1] = '\0', strncpy( almType ,   "SYNCING" , ((int)(  7 + 1 ) <= 0) ? 0 : (  7 + 1 )-1)) ;
        break;
    case PRBS_STATUS_SYNC_SUCC:
        (((char*) almType )[((int)(  4 + 1 ) <= 0) ? 0 : (  4 + 1 )-1] = '\0', strncpy( almType ,   "SUCC" , ((int)(  4 + 1 ) <= 0) ? 0 : (  4 + 1 )-1)) ;
        break;
    case PRBS_STATUS_SYNC_FAIL:
        (((char*) almType )[((int)(  4 + 1 ) <= 0) ? 0 : (  4 + 1 )-1] = '\0', strncpy( almType ,   "FAIL" , ((int)(  4 + 1 ) <= 0) ? 0 : (  4 + 1 )-1)) ;
        break;
    case PRBS_STATUS_SYNC_ERROR:
        (((char*) almType )[((int)(  7 + 1 ) <= 0) ? 0 : (  7 + 1 )-1] = '\0', strncpy( almType ,   "SYNCERR" , ((int)(  7 + 1 ) <= 0) ? 0 : (  7 + 1 )-1)) ;
        break;
    default:
        (((char*) almType )[((int)(  7 + 1 ) <= 0) ? 0 : (  7 + 1 )-1] = '\0', strncpy( almType ,   "UNKNOWN" , ((int)(  7 + 1 ) <= 0) ? 0 : (  7 + 1 )-1)) ;
        break;
    }

}



 








 








int
cah_em4_pohWrite (void *base, int port, unsigned short reg, unsigned short val)
{
    unsigned short u = 0x0000;

    u |= (port << 8);
    u |= reg;
    u |= 0x8000;                 
    u |= 0x4000;                 

    cah_em4_wr (base, (0x02 << 10) | 0x02 , val);
    cah_em4_wr (base, (0x02 << 10) | 0x01 , u);
    cah_em4_pollBit ((unsigned short *) base, (0x02 << 10) | 0x01 );

    return ((WORD)0x0) ;
}

 





unsigned short
cah_em4_pohRead (void *base, int port, unsigned short reg)
{
    unsigned short u = 0x0000;
    unsigned short result = 0x00000;

    u |= (port << 8);
    u |= reg;
    u |= 0x8000;                 
    u |= 0x0000;                 

    cah_em4_wr (base, (0x02 << 10) | 0x01 , u);
    cah_em4_pollBit ((unsigned short *) base, (0x02 << 10) | 0x01 );
    result = cah_em4_rd (base, (0x02 << 10) | 0x03 );

    return (result);
}

 



unsigned short
cah_em4_readDemapper (void *base, int port, unsigned short reg)
{
    unsigned short u = 0x0000;
    unsigned short result = 0x00000;

    u |= (port);
    u |= 0x8000;                 

    cah_em4_wr (base, (0x05 << 10) | 0x01 , u);
    cah_em4_pollBitDmapper ((unsigned short *) base, (0x05 << 10) | 0x01 );
    result = cah_em4_rd (base, reg);

    return (result);
}

 




CAH_EM4_ENTITY_DATA_T *
cah_em4_getEntity (ENTITY_T ent)
{
    int i = 0;
    CAH_EM4_ENTITY_DATA_T *p = (CAH_EM4_ENTITY_DATA_T *) 0;

    i = 0;
    p = gppCahEm4CfgVar[i];

    return p;
}

 





void *
cah_em4_getEntityBase (ENTITY_T ent, int port)
{
    CAH_EM4_ENTITY_DATA_T *p = (CAH_EM4_ENTITY_DATA_T *) 0;
    void *base = 0x0000;

    p = cah_em4_getEntity (ent);
    base = (void *) p->chip_var[cah_em4_port2chip (port) - 1].cfg.baseAddr;

    return base;
}

 


CAH_EM4_ENTITY_DATA_T *
cah_em4_OpenEx (void)
{
    int memSize = 0;
    CAH_EM4_ENTITY_DATA_T *p = ((void *)0) ;

     





	if (((void *)0)  != gppCahEm4CfgVar[0])
	{
		printf ("\r\n%s(): gppCahEm4CfgVar[0](%p) NOT NULL !!!", __FUNCTION__, gppCahEm4CfgVar[0]);
		return ((void *)0) ;
	}

    memSize = (sizeof (CAH_EM4_ENTITY_DATA_T) + sizeof (EM4_CHIP_VAR_T) * 1) * 2;

    p = (CAH_EM4_ENTITY_DATA_T *) malloc( memSize ); ;
    if (!p)
    {
        printf ("\r\n%s():\tFD malloc failed !!!!!!", __FUNCTION__);
        return ((void *)0) ;
    }








    gppCahEm4CfgVar[0] = p;      
    memset((INT1 *)( p ), (  0x00 ), (  memSize )) ;

    return p;
}


int
cah_em4_ent2fd (ENTITY_T ent)
{
    int i = 0;

    return (i);
}


CAH_EM4_ENTITY_DATA_T *
cah_em4_fd2entptr (int fd)
{
    return gppCahEm4CfgVar[fd];
}

int
cah_em4_fd2ent (int fd)
{
    return (gppCahEm4CfgVar[fd]->entity);
}

CAH_EM4_ENTITY_DATA_T *
cah_em4_getEntityPtr (ENTITY_T entity)
{
    return cah_em4_getEntity (entity);
}




 





int
cah_em4_ent2Id (ENTITY_T entity)
{
    int id = 0;

    if (((void *)0)  == g_pTransFunc)
    {
        printf ("\r\n%s:KEY translation function not registered!!!", __FUNCTION__);
        return -1;
    }

    id = (*g_pTransFunc) (entity);

    return id;
}




void
em4Mi (void)
{
    int fd = 0;

    for (fd = 0; fd < (((1) ) ); fd++)
    {
        printf ("\r\nfd(%2d) m-base(%p), status(%6s), liu-base(%p)",
                fd, cah_em4_getEntityBase (cah_em4_fd2ent (fd), 1),
                (eEm4EntAct == gppCahEm4CfgVar[fd]->state) ? "ACTIVE" : (eEm4EntInit ==
                                                                         gppCahEm4CfgVar[fd]->
                                                                         state ? "INIT" : "IDLE"),
                (void *) 0xffff   );
    }
    printf ("\r\n");
}




void
cah_em4Info (void)
{
    int i = 0;
    UINT32 entity;
    CAH_EM4_ENTITY_DATA_T *pEntity;

    for (i = 0; i < ((1) ) ; i++)
    {
        if (gppCahEm4CfgVar[i] == ((void *)0) )
        {
            continue;
        }

         
        if (gppCahEm4CfgVar[i]->state != eEm4EntAct)
        {
            continue;
        }

        entity = gppCahEm4CfgVar[i]->entity;

        pEntity = cah_em4_getEntityPtr (entity);

        printf ("\r\nentity(%d), base(0x%x)", entity, pEntity->chip_var[0].cfg.baseAddr);
    }

    printf ("\r\n");
}


void
em4AllInLoop (int flag)
{
    unsigned short u = 0x0000;
	int ent = 0;

    if (((void *)0)  == cah_em4_getEntity (ent))
    {
        printf ("\r\nent(%d) NOT active", ent);
        return;
    }


    u = cah_em4_rd (cah_em4_getEntityBase (ent, 1), (((0x00 << 10) | 0x01)) );
    if (flag)
    {
        u |= 0x0002;
    }
    else
    {
        u &= ~0x0002;
    }

    cah_em4_wr (cah_em4_getEntityBase (ent, 1), (((0x00 << 10) | 0x01)) , u);
    u = cah_em4_rd (cah_em4_getEntityBase (ent, 100), (((0x00 << 10) | 0x01)) );
    if (flag)
    {
        u |= 0x0002;
    }
    else
    {
        u &= ~0x0002;
    }

    cah_em4_wr (cah_em4_getEntityBase (ent, 100), (((0x00 << 10) | 0x01)) , u);
}


int
em4ShowIntMask (int ulPort)
{
    unsigned short u = 0x0000, uu = 0;
	int port = 0;

	{if (125 <  ulPort ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  ulPort ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	port = cah_em4_mapPort (ulPort) - 1;
    u = 	cah_em4_pohRead ((void *)(0x50000000) , port, 0x00 );
    uu = 	cah_em4_pohRead ((void *)(0x60000000) , port, 0x00 );
    printf ("\r\nbase\t(%p)(%p)", (void *)(0x50000000) , (void *)(0x60000000) );
    printf ("\r\nssf\t(%s)(%s), ", 	u & 0x4000 ? "ENABLE" : "DISABLE", 
									uu & 0x4000 ? "ENABLE" : "DISABLE");
    printf ("\r\nerdi\t(%s)(%s), ", u & 0x0200 ? "ENABLE" : "DISABLE", 
									uu & 0x0200 ? "ENABLE" : "DISABLE");
    printf ("\r\ntsl\t(%s)(%s), ", 	u & 0x0100 ? "ENABLE" : "DISABLE", 
									uu & 0x0100 ? "ENABLE" : "DISABLE");
    printf ("\r\ntti\t(%s)(%s), ", 	u & 0x0080 ? "ENABLE" : "DISABLE", 
									uu & 0x0080 ? "ENABLE" : "DISABLE");
    printf ("\r\nsf\t(%s)(%s), ", 	u & 0x0040 ? "ENABLE" : "DISABLE", 
									uu & 0x0040 ? "ENABLE" : "DISABLE");
    printf ("\r\nsd\t(%s)(%s), ", 	u & 0x0020 ? "ENABLE" : "DISABLE", 
									uu & 0x0020 ? "ENABLE" : "DISABLE");
    printf ("\r\nrdi\t(%s)(%s), ", 	u & 0x0010 ? "ENABLE" : "DISABLE", 
									uu & 0x0010 ? "ENABLE" : "DISABLE");
    printf ("\r\nuneq\t(%s)(%s), ", u & 0x0008 ? "ENABLE" : "DISABLE", 
									uu & 0x0008 ? "ENABLE" : "DISABLE");
    printf ("\r\nplm\t(%s)(%s), ", 	u & 0x0004 ? "ENABLE" : "DISABLE", 
									uu & 0x0004 ? "ENABLE" : "DISABLE");
    printf ("\r\ntiu\t(%s)(%s), ", 	u & 0x0002 ? "ENABLE" : "DISABLE", 
									uu & 0x0002 ? "ENABLE" : "DISABLE");
    printf ("\r\ntim\t(%s)(%s)", 	u & 0x0001 ? "ENABLE" : "DISABLE", 
									uu & 0x0001 ? "ENABLE" : "DISABLE");
    printf ("\r\n");
	return 0;
}



int
em4ShowCons (int port)
{
    unsigned short u = 0x0000, uu = 0;
	int ulPort = 0;

	{if (125 <  port ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  port ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	ulPort = cah_em4_mapPort (port) - 1;
    u = cah_em4_pohRead ((void *) (0x50000000) , ulPort, 0x01 );
    uu = cah_em4_pohRead ((void *) (0x60000000) , ulPort, 0x01 );
    printf ("\r\nbase       (%x)(%x)", (0x50000000) , (0x60000000) );
    printf ("\r\nuneqrdi    (%s)(%s), ", u & 0x0080 ? "ON" : "OFF", uu & 0x0080 ? "ON" : "OFF");
    printf ("\r\nplmrdi     (%s)(%s), ", u & 0x0040 ? "ON" : "OFF", uu & 0x0040 ? "ON" : "OFF");
    printf ("\r\nuneqais    (%s)(%s), ", u & 0x0020 ? "ON" : "OFF", uu & 0x0020 ? "ON" : "OFF");
    printf ("\r\nplmais     (%s)(%s), ", u & 0x0010 ? "ON" : "OFF", uu & 0x0010 ? "ON" : "OFF");
    printf ("\r\ntimrdi     (%s)(%s), ", u & 0x0008 ? "ON" : "OFF", uu & 0x0008 ? "ON" : "OFF");
    printf ("\r\ntimais     (%s)(%s), ", u & 0x0004 ? "ON" : "OFF", uu & 0x0004 ? "ON" : "OFF");
    printf ("\r\nttimode    (%s)(%s), ", u & 0x0001 ? "1-BYTEMODE" : "16-BYTEMODE", uu & 0x0001 ? "1-BYTEMODE" : "16-BYTEMODE");
    printf ("\r\n");
	return 0;
}



int
cah_em4_trace (int level, char *msg, ...)
{
    va_list argptr;
    int cnt = 0;

    if (level == gCahEm4DebugLevel)
    {
        printf ("\r\n");
        (__builtin_next_arg (  msg ), __builtin_memcpy ((  argptr  ), __builtin_saveregs (), sizeof(__gnuc_va_list)) ) ;
        cnt = vprintf (msg, argptr);
        ((void)0) ;
    }

    return (cnt);
}


int
em4ShowEnable (int ulPort)
{
    int ent = 0;
    unsigned short u = 0x0000, uu, u1 = 0, u0 = 0;
	int port = cah_em4_mapPort (ulPort) - 1;

	{if (125 <  ulPort ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  ulPort ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    u1 = em4MapRd (5, 0x100 + port, &u0, &u1);

    cah_em4_wr ((void *) (0x50000000) , (0x04 << 10) | 0x30 , port);
    u = em4Map1Rd (4, 0x40);

    cah_em4_wr ((void *) (0x60000000) , (0x04 << 10) | 0x30 , port);
    uu = em4Map2Rd (4, 0x40);
    printf ("\r\nentity(%d), port(%d), u(%x), uu(%x)", ent, port, u, uu);
    printf ("\r\nrdi_force     (%d)(%d)", u & 0x0001 ? 1 : 0, uu & 0x0001 ? 1 : 0);
    printf ("\r\nrdi_en        (%d)(%d)", u & 0x0002 ? 1 : 0, uu & 0x0002 ? 1 : 0);
    printf ("\r\nrei_force     (%d)(%d)", u & 0x0040 ? 1 : 0, uu & 0x0040 ? 1 : 0);
    printf ("\r\nrei_en        (%d)(%d)", u & 0x0080 ? 1 : 0, uu & 0x0080 ? 1 : 0);
    printf ("\r\nbip2_disable  (%d)(%d)", u & 0x0100 ? 1 : 0, uu & 0x0100 ? 1 : 0);
    printf ("\r\nerdi_en       (%d)(%d)", u & 0x0200 ? 1 : 0, uu & 0x0200 ? 1 : 0);
    printf ("\r\nerdi_force    (%d)(%d)", u & 0x0400 ? 1 : 0, uu & 0x0400 ? 1 : 0);
    printf ("\r\ntu12_aisforce (%d)(%d)", u & 0x8000 ? 1 : 0, uu & 0x8000 ? 1 : 0);
    printf ("\r\nrfi[force]    (%d)(%d)", u & 0x0020 ? 1 : 0, uu & 0x0020 ? 1 : 0);
    printf ("\r\nretiming      (%d)(%d)", u0 & 0x0001 ? 1 : 0, u1 & 0x0001 ? 1 : 0);
    printf ("\r\n");

	return 0;
}


void
em4ShowJ2Acc (int ent, int port)
{
    int i = 0;
    void *baseaddr = (void *) 0;
    unsigned char j2exp[66];

    if (((void *)0)  == cah_em4_getEntity (ent))
    {
        printf ("\r\nEnt(%d) NOT active", ent);
        return;
    }

    baseaddr = cah_em4_getEntityBase (ent, port);

    cah_em4_getJ2Acc (baseaddr, port, j2exp);

    printf ("\r\nj2: entity(%d), port(%d)", ent, port);
    for (i = 0; i <= 15; i++)
    {
        printf ("\r\nj2[%2d]= (0x%2x), char(%c)",
                i, j2exp[i], (j2exp[i] >= 0x20 && j2exp[i] <= 0x7e) ? j2exp[i] : 'Y');
    }
}


void
em4ShowPageAddr (void)
{
    printf ("\r\npage0x01<<10 = (0x400      * 2 = 0x0800)");
    printf ("\r\npage0x02<<10 = (0x800      * 2 = 0x1000)");
    printf ("\r\npage0x04<<10 = (0x1000     * 2 = 0x2000)");
    printf ("\r\npage0x05<<10 = (0x1400     * 2 = 0x2800)");
}



int 
em4ShowAlarms (int ulPort)
{
    int ent = 0;
    unsigned short u = 0, u1 = 0, u2 = 0, u3 = 0, u4 = 0;
    unsigned short mu = 0, mu1 = 0, mu2 = 0, mu3 = 0, mu4 = 0;
    void *baseaddr = (void *) 0;
    unsigned short uu = 0;
    unsigned short muu = 0;
    int regcnt = 0;
    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;
	int port = 0;

	{if (125 <  ulPort ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  ulPort ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	port = cah_em4_mapPort (ulPort) - 1;

    baseaddr = cah_em4_getEntityBase (ent, port + 1);
    printf ("\r\n%s(): base(%p)", __FUNCTION__, baseaddr);

 



    u = cah_em4_pohRead (baseaddr, port, 0x03 );
    mu = cah_em4_pohRead ((void *)(0x60000000) , port, 0x03 );

    u1 = cah_em4_pohRead (baseaddr, port, 0x04 );
    mu1 = cah_em4_pohRead ((void *)(0x60000000) , port, 0x04 );


 


    cah_em4_wr (baseaddr, ((0x01 << 10) | 0x03) , 0x8000 | port);
    regcnt = 0;
    while (1)
    {
        uu = cah_em4_rd (baseaddr, ((0x01 << 10) | 0x03) );
        if (!(0x8000 & uu))
        {
            break;
        }
        else
        {
            regcnt++;
            if (regcnt > 100)
            {
                printf ("\r\nsomething wrong when read_hardware entity(%d)", ent);
                break;
            }
        }
    }
    u2 = cah_em4_rd (baseaddr, ((0x01 << 10) | 0x10) );  

    regcnt = 0;
    cah_em4_wr ((void *)(0x60000000) , ((0x01 << 10) | 0x03) , 0x8000 | port);
    while (1)
    {
        muu = cah_em4_rd ((void *)(0x60000000) , ((0x01 << 10) | 0x03) );
        if (!(0x8000 & uu))
        {
            break;
        }
        else
        {
            regcnt++;
            if (regcnt > 100)
            {
                printf ("\r\nsomething wrong when read_hardware entity(%d)", ent);
                break;
            }
        }
    }
    mu2 = cah_em4_rd ((void *)(0x60000000) , ((0x01 << 10) | 0x10) );  
 


    pEntity = cah_em4_getEntity (ent);
    u3 = cah_em4_readPdhAlarm (baseaddr, port);
    mu3 = cah_em4_readPdhAlarm ((void *)(0x60000000) , port);

    printf ("\r\nent(%d), port(%d)", ent, port);
    printf ("\r\nSTATUS\t\t\t\tINTR");
    printf ("\r\n______________________________________________________+++");

    printf ("\r\nlop\t(%s)(%s)\t", (u2 & 0x0003) == 0x0001 ? "AIS-RISE" : ((u2 & 0x0003) == 0x0002 ? "LOP-RISE" : "NORMAL"),
				(mu2 & 0x0003) == 0x0001 ? "AIS-RISE" : ((mu2 & 0x0003) == 0x0002 ? "LOP-RISE" : "NORMAL"));

    printf ("\r\npi_ssf\t(%s)(%s)\t", (u & 0x4000) ? " RISE" : "CLEAR", (mu & 0x4000) ? " RISE" : "CLEAR");
    printf ("\tssf\t(%s)(%s)", (u1 & 0x4000) ? " RISE" : "CLEAR",
		(mu1 & 0x4000) ? " RISE" : "CLEAR");


    printf ("\r\ntsl_ais\t(%s)(%s)\t", (u & 0x0800) ? " RISE" : "CLEAR",
			(mu & 0x0800) ? " RISE" : "CLEAR");
    printf ("\ttslais\t(%s)(%s)", (u1 & 0x0800) ? " RISE" : "CLEAR",
			(mu1 & 0x0800) ? " RISE" : "CLEAR");

    printf ("\r\nsf\t(%s)(%s)\t", (u & 0x0040) ? " RISE" : "CLEAR", 
			(mu & 0x0040) ? " RISE" : "CLEAR");
    printf ("\tsf\t(%s)(%s)", (u1 & 0x0040) ? " RISE" : "CLEAR", 
			(mu1 & 0x0040) ? " RISE" : "CLEAR");

    printf ("\r\nsd\t(%s)(%s)\t", (u & 0x0020) ? " RISE" : "CLEAR", 
			(mu & 0x0020) ? " RISE" : "CLEAR");
    printf ("\tsd\t(%s)(%s)", (u1 & 0x0020) ? " RISE" : "CLEAR", 
			(mu1 & 0x0020) ? " RISE" : "CLEAR");

    printf ("\r\nrdi\t(%s)(%s)\t", (u & 0x0010) ? " RISE" : "CLEAR", 
			(mu & 0x0010) ? " RISE" : "CLEAR");
    printf ("\trdi\t(%s)(%s)", (u1 & 0x0010) ? " RISE" : "CLEAR", 
			(mu1 & 0x0010) ? " RISE" : "CLEAR");

    printf ("\r\nuneq\t(%s)(%s)\t", (u & 0x0008) ? " RISE" : "CLEAR", 
			(mu & 0x0008) ? " RISE" : "CLEAR");
    printf ("\tuneq\t(%s)(%s)", (u1 & 0x0008) ? " RISE" : "CLEAR", 
			(mu1 & 0x0008) ? " RISE" : "CLEAR");

    printf ("\r\nplm\t(%s)(%s)\t", (u & 0x0004) ? " RISE" : "CLEAR", 
			(mu & 0x0004) ? " RISE" : "CLEAR");
    printf ("\tplm\t(%s)(%s)", (u1 & 0x0004) ? " RISE" : "CLEAR", 
			(mu1 & 0x0004) ? " RISE" : "CLEAR");

    printf ("\r\ntiu\t(%s)(%s)\t", (u & 0x0002) ? " RISE" : "CLEAR", 
			(mu & 0x0002) ? " RISE" : "CLEAR");
    printf ("\ttiu\t(%s)(%s)", (u1 & 0x0002) ? " RISE" : "CLEAR", 
			(mu1 & 0x0002) ? " RISE" : "CLEAR");

    printf ("\r\ntim\t(%s)(%s)\t", (u & 0x0001) ? " RISE" : "CLEAR", 
			(mu & 0x0001) ? " RISE" : "CLEAR");
    printf ("\ttim\t(%s)(%s)", (u1 & 0x0001) ? " RISE" : "CLEAR", 
			(mu1 & 0x0001) ? " RISE" : "CLEAR");

 


    printf ("\r\ne1lof\t(%s)(%s)\t", (u3 & 0x0004) ? "RISE" : "CLEAR", 
			(mu3 & 0x0004) ? "RISE" : "CLEAR");
    printf ("\r\ne1lom\t(%s)(%s)\t", (u3 & 0x0002) ? "RISE" : "CLEAR", 
			(mu3 & 0x0002) ? "RISE" : "CLEAR");
    printf ("\r\ne1rdi\t(%s)(%s)\t", (u3 & 0x0001) ? "RISE" : "CLEAR", 
			(mu3 & 0x0001) ? "RISE" : "CLEAR");
    printf ("\r\ne1ais\t(%s)(%s)\t", (u3 & 0x0008) ? "RISE" : "CLEAR", 
			(mu3 & 0x0008) ? "RISE" : "CLEAR");


 


	u4 = em4Map1Rd (0, 0x12);
	mu4 = em4Map2Rd (0, 0x12);
    printf ("\r\ne1los\t(%x)(%x)\t", (u4), (mu4));

    printf ("\r\n______________________________________________________+++");
    printf ("\r\n");

	return 0;
}

char *
cah_em4_fid2name (int id, char *pname)
{
    char *name = pname;

    switch (id)
    {
    case 111:
        sprintf (pname, "%s", "EM4_EnableRetiming");
        break;
    case 1:
        sprintf (pname, "%s", "EM4_SetTransPsl");
        break;
    case 2:
        sprintf (pname, "%s", "EM4_SetExpPsl");
        break;
    case 3:
        sprintf (pname, "%s", "EM4_GetRcvPsl");
        break;
    case 4:
        sprintf (pname, "%s", "EM4_EnablePlm");
        break;
    case 5:
        sprintf (pname, "%s", "EM4_SetTransTti");
        break;
    case 6:
        sprintf (pname, "%s", "EM4_SetExpTti");
        break;
    case 7:
        sprintf (pname, "%s", "EM4_GetAccTti");
        break;
    case 8:
        sprintf (pname, "%s", "EM4_EnableTim");
        break;
    case 9:
        sprintf (pname, "%s", "EM4_SetTransTtiMode");
        break;
    case 10:
        sprintf (pname, "%s", "EM4_SetTransTti");
        break;
    case 11:
        sprintf (pname, "%s", "EM4_SetPdhPortMode");
        break;
    case 12:
        sprintf (pname, "%s", "EM4_GetPortType");
        break;
    case 13:
        sprintf (pname, "%s", "EM4_SetPortLoopback");
        break;


    case 14:
        sprintf (pname, "%s", "EM4_EnablePort");
        break;
    case 15:
        sprintf (pname, "%s", "EM4_GetPortState");
        break;

    case 16:
        sprintf (pname, "%s", "EM4_PrbsEnable");
        break;
    case 17:
        sprintf (pname, "%s", "EM4_SetPrbsMode");
        break;
    case 18:
        sprintf (pname, "%s", "EM4_GetPrbsMode");
        break;
    case 19:
        sprintf (pname, "%s", "EM4_GetPrbsErrCount");
        break;
    case 20:
        sprintf (pname, "%s", "EM4_ForceInsertAis");
        break;
    case 21:
        sprintf (pname, "%s", "EM4_ForceInsertRdi");
        break;

    case 24:
        sprintf (pname, "%s", "EM4_BlockAIS");
        break;


    case 25:
        sprintf (pname, "%s", "EM4_SelectFrame");
        break;
    case 26:
        sprintf (pname, "%s", "EM4_EnableLOMLOF");
        break;

    case 99:
        sprintf (pname, "%s", "EM4_SetRcvTtiMode");
        break;
    default:
        printf (pname, "%d", id);
        break;
    }

    return (name);
}


 


















char *
cah_em4_id2name (int id)
{
    if ((32  != id)
        && (34  != id)
        && (36  != id)
        && (37  != id)
        && (38  != id)
        && (42  != id)
        && (39  != id)
        && (69  != id)
        && (91  != id)
        && (92  != id)
        && (230  != id)
        && (231  != id)
        && (232  != id)
        && (233  != id) && (235  != id))
    {
        printf ("\r\ncah_em4_id2name: unknown id(%d)", id);
        if (g_fflag)
        {
            logBackN ("unkonwn id", 5);
            g_fflag = 0;
        }
        sprintf (g_aname, "%s", "unknown id");
        return g_aname;
    }

 








    switch (id)
    {
    case 91 :
        sprintf (g_aname, "%s", "ppi-nslip");
        break;
    case 92 :
        sprintf (g_aname, "%s", "ppi-pslip");
        break;
    case 32 :
        sprintf (g_aname, "%s", "tu-lop");
        break;
    case 34 :
        sprintf (g_aname, "%s", "tu-ais");
        break;
    case 36 :
        sprintf (g_aname, "%s", "lp-tim");
        break;
    case 37 :
        sprintf (g_aname, "%s", "lp-uneq");
        break;
    case 38 :
        sprintf (g_aname, "%s", "lp-rdi");
        break;
    case 42 :
        sprintf (g_aname, "%s", "lp-rfi");
        break;
    case 39 :
        sprintf (g_aname, "%s", "lp-plm");
        break;
    case 69 :
        sprintf (g_aname, "%s", "ppi-los");
        break;
         


    case 230 :
        sprintf (g_aname, "%s", "ppi-lof");
        break;
    case 231 :
        sprintf (g_aname, "%s", "ppi-lom");
        break;
    case 232 :
        sprintf (g_aname, "%s", "ppi-rdi");
        break;
    case 235 :
        sprintf (g_aname, "%s", "lpvc_tlct");
        break;
    case 233 :
        sprintf (g_aname, "%s", "ppi-crc");
        break;
    default:
        break;
    }
    return g_aname;
}


 


int
cah_em4_isPDHRiseOrClear (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort, int almNum)
{
    return (pEntity->chip_var[ulChip].pohAlm[ulPort].e1state[almNum] & 0x0001 );
}


 


int
cah_em4_isRiseOrClear (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort, int almNum)
{
    return (pEntity->chip_var[ulChip].pohAlm[ulPort].state[almNum] & 0x0001 );
}


int
cah_em4_isPDHAlmRPTNow (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort, int almNum)
{
    return (pEntity->chip_var[ulChip].pohAlm[ulPort].e1state[almNum] & 0x8000 );
}


int
cah_em4_isAlmRPTNow (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort, int almNum)
{
    return (pEntity->chip_var[ulChip].pohAlm[ulPort].state[almNum] & 0x8000 );
}

int
cah_em4_isAlmForced (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort)
{
    return (pEntity->chip_var[ulChip].forceRptAlm[ulPort] == 1 );
}


int
cah_em4_isPDHAlmForced (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort)
{
    return (pEntity->chip_var[ulChip].forceRptAlm[ulPort] == 1 );
}


void
cah_em4_clearAlmForced (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort)
{
    pEntity->chip_var[ulChip].forceRptAlm[ulPort] = 0 ;
}






BOOL
cah_em4_isPDHAlmSet (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort, int almNum)
{
    return (pEntity->chip_var[ulChip].pohAlm[ulPort].e1state[almNum] & 0x0001 );
}


BOOL
cah_em4_isAlmSet (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort, int almNum)
{
    return (pEntity->chip_var[ulChip].pohAlm[ulPort].state[almNum] & 0x0001 );
}

int
cah_em4_isEntLive (int fd)
{
    return (eEm4EntAct == gppCahEm4CfgVar[fd]->state) ? 1 : 0;
}




 


int
cah_em4_isCardOn (int instance)
{
    if (SDD_BOARD_PRESENT == SDD_GetBoardPresence ((CTD_SlotToInstance( instance )) ))
    {
        return 1;
    }
    else
    {
        if (33 == gCahEm4DebugLevel)
            printf ("\r\niscardon: inst(%d) FALL-OFF", instance);
        return 0;
    }
    return 1;
}




 

































unsigned short
cah_em4_getPlm (ALARM_REGISTER_T * alarmCtrlReg)
{
    return (alarmCtrlReg->alarmCtrlBit.plm);
}


unsigned short
cah_em4_getUneq (ALARM_REGISTER_T * alarmCtrlReg)
{
    return (alarmCtrlReg->alarmCtrlBit.uneq);
}

unsigned short
cah_em4_getTim (ALARM_REGISTER_T * alarmCtrlReg)
{
    return (alarmCtrlReg->alarmCtrlBit.tim);
}


unsigned short
cah_em4_getRdi (ALARM_REGISTER_T * alarmCtrlReg)
{
    return (alarmCtrlReg->alarmCtrlBit.rdi);
}

unsigned short
cah_em4_getRfi (ALARM_REGISTER_T * alarmCtrlReg)
{
    return (alarmCtrlReg->alarmCtrlBit.rfi);
}


PB_ERROR_CODE_T
cah_em4_GetSlip (int dwPos, UINT32 byPortNo, UINT32 * pPSlip, UINT32 * pNSlip)
{
    unsigned short slip = 0;
    void *base = 0;
    int ulPort = 0;

	{if (125 <  byPortNo ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  byPortNo ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	ulPort = cah_em4_mapPort (byPortNo) - 1;
       base = cah_em4_getEntityBase (0, byPortNo);

       slip = cah_em4_readDemapper (base, ulPort, (0x05 << 10) | 0X02 );
    *pPSlip = ((slip & 0xff00) >> 8);       
    *pNSlip = (slip & 0x00ff);      

    cah_em4_trace (21, "%s: entity(%d), port(%d), base(%x), reg(%x), val(%x)",
                   __FUNCTION__, 0, byPortNo, base, (0x05 << 10) | 0X02 , slip);

    return ((WORD)0x0) ;
}


int
cah_em4_simslip (ENTITY_T entity, UINT32 ulPort, int n, int flag)
{
    int i = 0;
    unsigned int pslip, nslip;

    if (0 == flag)
    {
        return 0;
    }

    for (i = 0; i < n; i++)
    {
        taskDelay (100);
        cah_em4_GetSlip (entity, ulPort, &pslip, &nslip);
        printf ("\r\nnslip(%d), pslip(%d)", nslip, pslip);
    }
    return 0;
}



BOOL
cah_em4_isTTI16Byte (CAH_EM4_ENTITY_DATA_T * pEntity, int ulChip, int ulPort)
{
    return (PbTIModeType_mode16Byte == pEntity->chip_var[ulChip].strRcvJ2[ulPort].J2Mode);

}


BOOL
cah_em4_isTimEnable (CAH_EM4_ENTITY_DATA_T * pEntity, int ulChip, int ulPort)
{
    return (pEntity->chip_var[ulChip].bTimEnable[ulPort]);
}




BOOL
cah_em4_isPLMEnable (CAH_EM4_ENTITY_DATA_T * pEntity, int ulChip, int ulPort)
{
    return (pEntity->chip_var[ulChip].bPlmEnable[ulPort]);
}

 


UINT16
cah_em4_getAlarmId (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort, int almNum)
{
    UINT16 almID;

    almID = pEntity->chip_var[ulChip].pohAlm[ulPort].almID[almNum];

    return almID;
}






BOOL
cah_em4_isPDHAlarmRised (UINT16 wTemp, CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip,
                         UINT32 ulPort, int almNum)
{
    return (wTemp && (!cah_em4_isPDHAlmSet (pEntity, ulChip, ulPort, almNum)));
}


 






BOOL
cah_em4_isAlarmRised (UINT16 wTemp, CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort,
                      int almNum)
{
    return (wTemp && (!cah_em4_isAlmSet (pEntity, ulChip, ulPort, almNum)));
}


BOOL
cah_em4_isAlarmCleared (UINT16 wTemp, CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort,
                        int almNum)
{
    return ((!wTemp) && (cah_em4_isAlmSet (pEntity, ulChip, ulPort, almNum)));
}

BOOL
cah_em4_isPDHAlarmCleared (UINT16 wTemp, CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip,
                           UINT32 ulPort, int almNum)
{
    return ((!wTemp) && (cah_em4_isPDHAlmSet (pEntity, ulChip, ulPort, almNum)));
}

int
cah_em4_getAlarmType (CAH_EM4_ENTITY_DATA_T * pEntity,
                      UINT32 ulChip, UINT32 ulPort, UINT16 almID, UINT8 * almType)
{

    memset((INT1 *)( almType ), (  0 ), (  sizeof (almType) )) ;
    switch (almID)
    {
    case 91 :
        (((char*) almType )[((int)(  9 + 1 ) <= 0) ? 0 : (  9 + 1 )-1] = '\0', strncpy( almType ,   "PPI-NSLIP" , ((int)(  9 + 1 ) <= 0) ? 0 : (  9 + 1 )-1)) ;
        break;
    case 92 :
        (((char*) almType )[((int)(  9 + 1 ) <= 0) ? 0 : (  9 + 1 )-1] = '\0', strncpy( almType ,   "PPI-PSLIP" , ((int)(  9 + 1 ) <= 0) ? 0 : (  9 + 1 )-1)) ;
        break;
    case 38 :
        (((char*) almType )[((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1] = '\0', strncpy( almType ,   "LP-RDI" , ((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1)) ;
        if (0)
            if (pEntity->chip_var[ulChip].bRestrainRDI[ulPort])
            {
                return -1;
            }
        (((char*) almType )[((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1] = '\0', strncpy( almType ,   "LP-RDI" , ((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1)) ;
        break;
    case 42 :
        (((char*) almType )[((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1] = '\0', strncpy( almType ,   "LP-RFI" , ((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1)) ;
        if (0)
            if (pEntity->chip_var[ulChip].bRestrainRDI[ulPort])
            {
                return -1;
            }
        (((char*) almType )[((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1] = '\0', strncpy( almType ,   "LP-RFI" , ((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1)) ;
        break;
    case 34 :
        (((char*) almType )[((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1] = '\0', strncpy( almType ,   "TU-AIS" , ((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1)) ;
        break;
    case 32 :
        (((char*) almType )[((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1] = '\0', strncpy( almType ,   "TU-LOP" , ((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1)) ;
        break;
    case 36 :
         





        (((char*) almType )[((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1] = '\0', strncpy( almType ,   "LP-TIM" , ((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1)) ;
        break;
    case 39 :
         





        (((char*) almType )[((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1] = '\0', strncpy( almType ,   "LP-PLM" , ((int)(  6 + 1 ) <= 0) ? 0 : (  6 + 1 )-1)) ;
        break;
    case 37 :
        (((char*) almType )[((int)(  8 + 1 ) <= 0) ? 0 : (  8 + 1 )-1] = '\0', strncpy( almType ,   "LP-UNREQ" , ((int)(  8 + 1 ) <= 0) ? 0 : (  8 + 1 )-1)) ;
        break;
    default:
        (((char*) almType )[((int)(  7 + 1 ) <= 0) ? 0 : (  7 + 1 )-1] = '\0', strncpy( almType ,   "UNKNOWN" , ((int)(  7 + 1 ) <= 0) ? 0 : (  7 + 1 )-1)) ;
        break;
    }

    return 0;
}


FMA_ALM_STATE
cah_em4_getPlmStatus (CAH_EM4_ENTITY_DATA_T * pEntity,
                      UINT32 ulChip, UINT32 ulPort, FMA_ALM_STATE state)
{
    if (0  == pEntity->chip_var[ulChip].bPlmEnable[ulPort])
    {
        return FMA_ALM_CLEAR;
    }

     


    return state;
}


FMA_ALM_STATE
cah_em4_getTimStatus (CAH_EM4_ENTITY_DATA_T * pEntity,
                      UINT32 ulChip, UINT32 ulPort, FMA_ALM_STATE state)
{
    if (0  == pEntity->chip_var[ulChip].bTimEnable[ulPort])
    {
        return FMA_ALM_CLEAR;
    }

    if (PbTIModeType_mode16Byte != pEntity->chip_var[ulChip].strRcvJ2[ulPort].J2Mode)
    {
        return FMA_ALM_CLEAR;
    }

     


    return state;
}


void
cah_em4_addSsfFc (ENTITY_T entity, int port)
{
    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;


    pEntity = cah_em4_getEntity (entity);
    pEntity->chip_var[0].pohAlm[port].e1ssfFc++;

}


void
cah_em4_Dpr ()
{
    unsigned int entity = 0;
    CAH_EM4_ENTITY_DATA_T *pEntity;
    int instance = 0;
    int fd = 0;

    while (1)
    {
         


        if (!gbCahEm4RunPollingTask[1 ])
        {
            taskDelay (100);
            printf ("\r\ngbCahEm4RunPollingTask[EM4_DPR_TASK_ID]");
            continue;
        }
        entity = gCahEm4TribVar.latchent;

        if ((( entity  >= ((1) ) )) )
        {
            printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 5185);
            continue;
        }

        if (((void *)0)  == (pEntity = cah_em4_getEntityPtr (entity)))
        {
            printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 5191);
            continue;
        }

        fd = cah_em4_ent2fd (entity);
        if (eEm4EntAct != pEntity->state)
        {
            printf ("\r\ncah_em4_Dpr!  entity(%d) NOT active", entity);
            continue;
        }

        instance = (CTD_SlotToInstance((  entity  ) )) ;

        if (EM4_ENTITY_DELATCH == gCahEm4TribVar.EM4LatchStatus[fd])
        {
            printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 5206);
            continue;
        }
        else
        {
            printf ("\r\nset DELATCH");
            gCahEm4TribVar.EM4LatchStatus[fd] = EM4_ENTITY_DELATCH;
        }

        cah_em4_trace (20, "%s detach entity:%d status:%d !!! \r\n", __FUNCTION__, entity,
                       gCahEm4TribVar.EM4LatchStatus[fd   ]);
    }
}

int
pbFpgaIntrEnable (int handle)
{
    return 0 ;
}

int
pbFpgaIsrHookAdd (int fpga, int bank, int device, FPGA_HOOK_FUNC usrHookFunc,
                  FPGA_HOOK_ARG usrHookArg)
{
    return 0 ;
}

PB_ERROR_CODE_T
cah_em4_intConnect (ENTITY_T entity)
{
    CAH_EM4_ENTITY_DATA_T *pEntity;
     
    int fpga, bank, device;
    int fd = 0;


    if (((void *)0)  == (pEntity = cah_em4_getEntityPtr (entity)))
    {
         
        return PB_ERR_PARAM;
    }
     
    fd = cah_em4_ent2fd (entity);

    fpga = 1;
    bank = 0;
    device = ( entity )  - 1;
    gCahEm4TribVar.fpgaHanle[fd] =
        pbFpgaIsrHookAdd (fpga, bank, device, (FPGA_HOOK_FUNC) cah_em4_isrFunc, entity);

    if (((FPGA_HANDLE)(-1))  == gCahEm4TribVar.fpgaHanle[fd])
    {
        printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 5258);
        return ((WORD)0x1) ;
    }
    printf ("\r\n126xe1rt entity(%d) interrupt handler installed ok", entity);
     
    if (0  != pbFpgaIntrEnable (gCahEm4TribVar.fpgaHanle[fd]))
    {
        printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 5265);
        return ((WORD)0x1) ;
    }

    pbFpgaIsrEnable (fpga);

    return ((WORD)0x0) ;
}


void
cah_em4_init_nothing ()
{
    static int flag = 1;
    int i = 0;

    if (flag)
    {
        memset((INT1 *)( &gCahEm4TribVar ), (  0 ), (  sizeof (CAH_EM4_ENTITY_TRIB_T) )) ;
        for (i = 0; i < ((1) ) ; i++)
        {
            gppCahEm4CfgVar[i] = 0;
            printf ("\r\n%s(): set gppCahEm4CfgVar[%d] to 0", __FUNCTION__, i);
        }
        flag = 0;
    }
}

BOOL
cah_em4_IsLatchIntEnable (UINT32 entity)
{
    BOOL bEnable = 0 ;
    void *baseaddr = cah_em4_getEntityBase (entity, 100);
    unsigned short u = 0;

    cah_ReadRegister ((DWORD) baseaddr, (((0x00 << 10) | 0X09) ), &u, 0xffff, CAH_REG_ATTRBT_RO);



    if (0x0001 & u)
    {
        bEnable = 1 ;
    }
    else
    {
        bEnable = 0 ;
    }

    return bEnable;
}



void
cah_em4_enableLatchInt (UINT32 entity, BOOL Enable)
{
    void *baseAddr = cah_em4_getEntityBase (entity, 1);

    if (Enable)
    {
        cah_em4_enableLatchIntr (baseAddr, 1);
    }
    else
    {
        cah_em4_enableLatchIntr (baseAddr, 0);
    }
}


void
cah_em4_isrFunc (FPGA_HANDLE handle, FPGA_HOOK_ARG entity)
{
    int fd = 0;

    if ((( entity  >= ((1) ) )) )
    {
        return;
    }

    cah_em4_enableLatchInt (entity, 0 );

     



    fd = cah_em4_ent2fd (entity);
    gCahEm4TribVar.IntCount[fd   ]++;
    g_isrcount++;
    gCahEm4TribVar.latchent = entity;
    g_cah_em4entity = entity;
}


PB_ERROR_CODE_T
cah_em4_intUnConnect (ENTITY_T entity)
{
    CAH_EM4_ENTITY_DATA_T *pEntity;
     
    int ReturnValue;
    int fd = 0;


    if (((void *)0)  == (pEntity = cah_em4_getEntityPtr (entity)))
    {
         
        return PB_ERR_PARAM;
    }

     
    fd = cah_em4_ent2fd (entity);

    if (((FPGA_HANDLE)(-1))  == gCahEm4TribVar.fpgaHanle[fd])
    {
         
        return ((WORD)0x1) ;
    }

    ReturnValue = pbFpgaIntrDisable (gCahEm4TribVar.fpgaHanle[fd]);
    if (ReturnValue != 0 )
    {
         
        return ((WORD)0x1) ;
    }

    ReturnValue = pbFpgaIsrHookDel (gCahEm4TribVar.fpgaHanle[fd]);
    if (ReturnValue != 0 )
    {
         
        return ((WORD)0x1) ;
    }

    return ((WORD)0x0) ;
}



void
cah_em4_latch ()
{
    UINT8 fd   ;
    CAH_EM4_ENTITY_LATCH_T Status;
    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;
    UINT16 intCount;
    INT32 intLockRet;
    void *baseaddr = ((void *)0) ;
    int instance = 0;
    int fdd = 0;

    while (1)
    {
        taskDelay (100);

        if (!gbCahEm4RunPollingTask[2 ])
        {
            printf ("\r\ncah_em4_latch (): gbCahEm4RunPollingTask[EM4_EPG_TASK_ID] NULL");
            continue;
        }

        for (fd = 0; fd < ((1) ) ; fd++)
        {
            if (!cah_em4_isEntLive (fd))
            {
                cah_em4_trace (2, "FD(%2d), state NOT active, continue", fd);
                continue;
            }

            if (!cah_em4_isCardOn (( cah_em4_fd2ent (fd) ) ))
            {
                printf ("\r\nFD(%2d), card NOT on", fd);
                continue;
            }

             
            pEntity = cah_em4_fd2entptr (fd);
            cah_em4_trace (22, "%s, entity(%d)", __FUNCTION__, pEntity->entity);

            Status = CAH_EM4_getLatchStatus (pEntity->entity);
             
            fdd = cah_em4_ent2fd (pEntity->entity);
            baseaddr = cah_em4_getEntityBase (pEntity->entity, 100);
            instance = (CTD_SlotToInstance((  pEntity->entity  ) )) ;

            if (EM4_ENTITY_LATCH == Status)
            {
                if ((EM4_ENTITY_LATCH != gCahEm4TribVar.EM4LatchStatus[fdd   ])
                    || (gCahEm4TribVar.EM4LatchForceRpt[   fdd]))
                {
                    gCahEm4TribVar.EM4LatchStatus[   fdd] = EM4_ENTITY_LATCH;

                    printf ("\r\nEM4_EpgPoll fd %d Latch Attached! \r\n", fd);
                }

                if (gCahEm4TribVar.EM4LatchForceRpt[fdd   ])
                {
                    gCahEm4TribVar.EM4LatchForceRpt[fdd   ] = 0 ;
                }

                if (!cah_em4_IsLatchIntEnable (pEntity->entity))
                {
                     
                    cah_em4_enableLatchIntr (baseaddr, 1);
                }
            }
            else if (EM4_ENTITY_DELATCH == Status)
            {
                if (cah_em4_IsLatchIntEnable (pEntity->entity))
                {
                     
                    cah_em4_enableLatchIntr (baseaddr, 0);

                }

                if ((EM4_ENTITY_DELATCH != gCahEm4TribVar.EM4LatchStatus[fdd   ])
                    || (gCahEm4TribVar.EM4LatchForceRpt[   fdd]))
                {
                    gCahEm4TribVar.EM4LatchStatus[fdd   ] = EM4_ENTITY_DELATCH;
                    gCahEm4TribVar.EM4LatchForceRpt[fdd   ] = 0 ;

                    printf ("\r\nEM4_EpgPoll fd %d Latch Detached! \r\n", fd);
                }

                if (gCahEm4TribVar.EM4LatchForceRpt[fdd   ])
                {
                    gCahEm4TribVar.EM4LatchForceRpt[fdd   ] = 0 ;
                }
            }

            intLockRet = intLock ();
            intCount = gCahEm4TribVar.IntCount[fdd   ];

            if (intCount < 80 )
            {
                intLockRet = intLock ();
                gCahEm4TribVar.IntCount[fdd   ] = 0;
            }
            else
            {
                printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", fd, "cah_126e1.c", 5502);
            }
        }
    }
}



 


int
CAH_EM4_enableLOMLOF (ENTITY_T entity, int chip, int port, int enable)
{
    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;
     


    cah_em4_trace (36, "%s: entity(%d), port(%d), enalbe(%d)", __FUNCTION__, entity, port, enable);

    if (0 > port || 62 < port)
    {
        printf ("\r\n%s: port(%d) overflow", __FUNCTION__, port);
        return ((WORD)0x1) ;
    }

    pEntity = cah_em4_getEntityPtr (entity);
    cah_em4_registerCall ((PMyFun) CAH_EM4_enableLOMLOF, port, enable, 0, 26, __FUNCTION__);

    if (pEntity->state != eEm4EntAct)
    {
        return PB_ERR_STATE;
    }


    cah_em4_trace (35, "%s: entity(%d), port(%d), enalbe(%d)", __FUNCTION__, entity, port, enable);
    pEntity->chip_var[chip].isRptLOM[port] = enable;
    pEntity->chip_var[chip].forceRptAlm[port] = 1 ;

    return ((WORD)0x0) ;
}




PB_ERROR_CODE_T
CAH_EM4_initModule ()
{
    return cah_em4_initModule ();
}

int cah_em4_initPortMap (void);
PB_ERROR_CODE_T
CAH_EM4_loadModule ()
{
    static int flag = 0;

    if (flag) return 0;
    flag = 1;
    cah_em4_init_nothing ();
    cah_em4_initPortMap ();
    return cah_em4_loadModule ();
}


PB_ERROR_CODE_T
CAH_EM4_ForceAIS (DWORD dwPos, int nport, eEM4SWITCH ais)
{



    void *base = ((void *)0) ;
    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;
    ENTITY_T entity = 0;
    int port = 0;

    port = cah_em4_mapPort (nport) - 1;;

    if (0 > port || 62 < port)
    {
        printf ("\r\nEM4_ForceAIS : port(%d) overflow", port);
        return (((WORD)0x1) );
    }

    base = cah_em4_getEntityBase (entity, port);
    if (gCahEm4SelectPort == port)
    {
        cah_em4_trace (40 , "\r\n%s(): entity(%d), port(%d), base(%p)", __FUNCTION__,
                       entity, port, base);
    }
    cah_em4_forceAis (base, port, ais);
    pEntity = cah_em4_getEntity (entity);

    pEntity->chip_var[0].isFrcais[port] = ais;

    return (((WORD)0x0) );


    return 0;

}

 





PB_ERROR_CODE_T
CAH_EM4_forceRDI (DWORD dwPos, int nport, eEM4SWITCH rdi)
{




    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;
    void *base = ((void *)0) ;
    ENTITY_T entity = 0;
    int port = 0;

    port = cah_em4_mapPort (nport) - 1;;

    base = cah_em4_getEntityBase (entity, port);
    cah_em4_trace (34, "\r\nentity(%d), port(%d), base(%p), rdi(%d)", entity, port, base, rdi);
    cah_em4_forceRdi (base, port, rdi);

    pEntity = cah_em4_getEntity (entity);
    pEntity->chip_var[0].isFrcrdi[port] = rdi;

    return (((WORD)0x0) );


    return 0;
}


WORD
CAH_EM4_EnableReTiming (DWORD dwPos, BYTE byPortNo, BOOL bEnable)
{




    void *base = ((void *)0) ;
    int ulPort = 0;

    {if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
    base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));
    cah_em4_registerCall ((PMyFun) CAH_EM4_EnableReTiming, (int) SDD_126xE1IOPortConvert(byPortNo), (int) bEnable, 0, 111, __FUNCTION__);
    cah_em4_set_inloopretiming (base, ulPort, eEM4_DISABLE   ,
                                bEnable ? eEM4_ENABLE : eEM4_DISABLE);

    return ((WORD)0x0) ;

}

WORD cah_em4_getTimingRef (BYTE mid_port, BYTE *byPort, BOOL bEnable)
{
    return 0;
}

int em4_queryPortFromSlot(int slot);
int testgettiming (byclocknum)
{
    BYTE byPortNo = 0;
    BOOL bEnable = 0;

    CAH_EM4_GetTimingRef (0, byclocknum, &byPortNo, &bEnable);
    printf ("\r\n%s(): byclock(%d), byPort(%d), enable(%d)", __FUNCTION__, byclocknum, byPortNo, bEnable);
    return 0;
}

int g_testtiming = 0;
int g_testtiming1 = 0;

 
WORD
CAH_EM4_GetTimingRef (DWORD dwPos, BYTE byClockNum, BYTE * byPortNo, BOOL * bEnable)
{
    WORD   wRtn       = ((WORD)0x0) ;
    BYTE	wRegValue = 0x0;
    BYTE port = 0;  

    wRtn = cah_Read8Reg(0x70000000 , 0x1b  + byClockNum, &wRegValue, 0x0F, CAH_REG_ATTRBT_RW);
    port = (wRegValue & 0x7) + 1;
if (g_testtiming1)
printf ("\r\n%s(): port(%3d), wRegValue(%4x)", __FUNCTION__, port - 1, wRegValue);
    if(!(wRegValue & 0x08))
    {
		*bEnable = 1 ;	
    }
    else
    {
		*bEnable = 0 ;
		port = 0;
    }
 
    if (0 == port) 
    {
        *byPortNo = 0;
        return ((WORD)0x0) ;
    } else {
        *byPortNo = em4_queryPortFromSlot(port);
if (g_testtiming1)
        printf ("\r\n%s(): virtualport(%3d), realport(%3d)", __FUNCTION__, port, *byPortNo);
    }

    { { if (DBG_GetLevel(  gCahDbgModuleNo ,    CAH_DEBUG_LEVEL_CLOCK  )) {printf("[%s] %d: ", "cah_126e1.c", 5710); printf   ("\r\n CAH_S14_GetTimingRef: Port = [%d], clock = [%d], wRegValue = [0x%x], bEnable = [%d]", *byPortNo , byClockNum, wRegValue, *bEnable)  ;} if (DBG_NvGetLevel(  gCahDbgModuleNo ,    CAH_DEBUG_LEVEL_CLOCK  ))    ; } ; } ;    
 
    return ((WORD)0x0) ;
}

 


int em4_queryPortFromSlot(int slot)
{
    int port = 0;
    void * base = 0;
    unsigned short u = 0;

    if (4 >= slot)
    {
        base = cah_em4_getEntityBase (0, 1);     
        u = cah_em4_rd (base, ((0x05 << 10) | 0x200 ) + (slot-1) / 2);
        if ((slot) % 2)
        {
            if ((0x3f)  == (u & 0xff))
            {
                return 0;
            } else {
                return (u & 0xff);
            }
        } else {
            if ((0x3f)  == ((u & 0xff00)>>8))
            {
                return 0;
            } else {
                return ((u & 0xff00)>>8);
            }
        }
    } else {
        base = cah_em4_getEntityBase (0, 100);     
        u = cah_em4_rd (base, ((0x05 << 10) | 0x200 ) + (slot-1-4) / 2);
if (g_testtiming1)
printf ("\r\nbase(%p), u(%x), slot(%d),(slot-1-4)/2(%d)", base, u, slot, (slot-1-4)/2);
        if ((slot) % 2)
        {
            if ((0x3f)  == (u & 0xff))
            {
                return 0;
            } else {
                return (u & 0xff) + 63;
            }
        } else {
            if ((0x3f)  == ((u & 0xff00)>>8))
            {
                return 0;
            } else {
                return ((u & 0xff00)>>8) + 63;
            }
        }
    }

    return (port);
}

int g_timingVirtualSlot [8];

int em4_getSlotFrom126port (int port, int enable)
{
    int slot = 99;
    int n = 0;
    void * base = 0;
    unsigned short u = 0, u1 = 0;


    n = cah_em4_mapPort (port) - 1;
    base = cah_em4_getEntityBase (0, port);     
    u = cah_em4_rd (base, (0x05 << 10) | 0x200 );
    u1 = cah_em4_rd (base, ((0x05 << 10) | 0x200 ) + 1);
    if (g_testtiming) printf ("\r\n%s(): EM4_RCK_0_1_SEL(%x),EM4_RCK_0_1_SEL+1(%x)", __FUNCTION__, u, u1);

    
    if (0 <= port && 62 >= port)
    {
        if ((u & 0xff) == n) 
        {            
            if (g_testtiming) printf ("\r\n%s(): found: 0", __FUNCTION__);
            slot = 0;
            if (0 == enable)
            {
                g_timingVirtualSlot[0] = (0x3f) ;
                u &= 0xff00;
                u |= 0x00ff;
                cah_em4_wr (base, (0x05 << 10) | 0x200 , u);
            }
        }
        if (((u & 0xff00) >> 8) == n) 
        {
            if (g_testtiming) printf ("\r\n%s(): found: 1", __FUNCTION__);
            slot = 1;
            if (0 == enable)
            {
                g_timingVirtualSlot[1] = (0x3f) ;
                u &= 0x00ff;
                u |= 0xff00;
                cah_em4_wr (base, (0x05 << 10) | 0x200 , u);
            }
        }

        if ((u1 & 0xff) == n)         
        {
            if (g_testtiming) printf ("\r\n%s(): found: 2", __FUNCTION__);
            slot = 2;
            if (0 == enable)
            {
                g_timingVirtualSlot[2] = (0x3f) ;
                u1 &= 0xff00;
                u1 |= 0x00ff;
                cah_em4_wr (base, (((0x05 << 10) | 0x200 )+1), u1);
            }
        }
        if (((u1 & 0xff00) >> 8) == n)  
        { 
            if (g_testtiming) printf ("\r\n%s(): found: 3", __FUNCTION__);
            slot = 3;
            if (0 == enable)
            {
                g_timingVirtualSlot[3] = (0x3f) ;
                u1 &= 0x00ff;
                u1 |= 0xff00;
                cah_em4_wr (base, (((0x05 << 10) | 0x200 )+1), u1);
            }
        }
    } else {
        if ((u & 0xff) == n)
        {
            if (g_testtiming) printf ("\r\n%s(): found: 4", __FUNCTION__);
            slot = 4;
            if (0 == enable)
            {
                g_timingVirtualSlot[4] = (0x3f) ;
                u &= 0xff00;
                u |= 0x00ff;
                cah_em4_wr (base, ((0x05 << 10) | 0x200 ), u);
            }
        }
        if (((u & 0xff00) >> 8) == n)    
        {
            if (g_testtiming) printf ("\r\n%s(): found: 5", __FUNCTION__);
            slot = 5;
            if (0 == enable)
            {
                g_timingVirtualSlot[5] = (0x3f) ;
                u &= 0x00ff;
                u |= 0xff00;
                cah_em4_wr (base, ((0x05 << 10) | 0x200 ), u);
            }
        }
        if ((u1 & 0xff) == n)            
        {
            if (g_testtiming) printf ("\r\n%s(): found: 6", __FUNCTION__);
            slot = 6;
            if (0 == enable)
            {
                g_timingVirtualSlot[6] = (0x3f) ;
                u1 &= 0xff00;
                u1 |= 0x00ff;
                cah_em4_wr (base, (((0x05 << 10) | 0x200 ) + 1), u1);
            }
        }
        if (((u1 & 0xff00) >> 8) == n)   
        {
            if (g_testtiming) printf ("\r\n%s(): found: 7", __FUNCTION__);
            slot = 7;
            if (0 == enable)
            {
                g_timingVirtualSlot[7] = (0x3f) ;
                u1 &= 0x00ff;
                u1 |= 0xff00;
                cah_em4_wr (base, (((0x05 << 10) | 0x200 ) + 1), u1);
            }
        }
    }









     
    if (99 == slot)
    {
        if (0 <= port && 62 >= port)
        {
            if ((u & 0xff) == (0x3f) )
            {
                if (g_testtiming) printf ("\r\n%s(): alloc: 0", __FUNCTION__);
                u &= 0xff00;
                u |= n;
                slot = 0;
                cah_em4_wr (base, (0x05 << 10) | 0x200 , u);
                goto allocend;
            }
            if (((u & 0xff00)>>8) == (0x3f) )
            {
                if (g_testtiming) printf ("\r\n%s(): alloc: 1", __FUNCTION__);
                u &= 0xff;
                u |= (n << 8);
                slot = 1;
                cah_em4_wr (base, (0x05 << 10) | 0x200 , u);
                goto allocend;
            }
            if ((u1 & 0xff) == (0x3f) )
            {
                if (g_testtiming) printf ("\r\n%s(): alloc: 2", __FUNCTION__);
                u1 &= 0xff00;
                u1 |= n;
                slot = 2;
                cah_em4_wr (base, ((0x05 << 10) | 0x200 ) + 1, u1);
                goto allocend;
            }
            if (((u1 & 0xff00)>>8) == (0x3f) )
            {
                if (g_testtiming) printf ("\r\n%s(): alloc: 3", __FUNCTION__);
                u1 &= 0xff;
                u1 |= (n << 8);
                slot = 3;
                cah_em4_wr (base, ((0x05 << 10) | 0x200 ) + 1, u1);
                goto allocend;
            }
        } else {
            if ((u & 0xff) == (0x3f) )
            {
                if (g_testtiming) printf ("\r\n%s(): alloc: 4", __FUNCTION__);
                u &= 0xff00;
                u |= n;
                slot = 4;
                cah_em4_wr (base, (0x05 << 10) | 0x200 , u);
                goto allocend;
            }
            if (((u & 0xff00)>>8) == (0x3f) )
            {
                if (g_testtiming) printf ("\r\n%s(): alloc: 5", __FUNCTION__);
                u &= 0xff;
                u |= (n << 8);
                slot = 5;
                cah_em4_wr (base, (0x05 << 10) | 0x200 , u);
                goto allocend;
            }
            if ((u1 & 0xff) == (0x3f) )
            {
                if (g_testtiming) printf ("\r\n%s(): alloc: 6", __FUNCTION__);
                u1 &= 0xff00;
                u1 |= n;
                slot = 6;
                cah_em4_wr (base, ((0x05 << 10) | 0x200 ) + 1, u1);
                goto allocend;
            }
            if (((u1 & 0xff00)>>8) == (0x3f) )
            {
                if (g_testtiming) printf ("\r\n%s(): alloc: 7", __FUNCTION__);
                u1 &= 0xff;
                u1 |= (n << 8);
                slot = 7;
                cah_em4_wr (base, ((0x05 << 10) | 0x200 ) + 1, u1);
                goto allocend;
            }
        }
    }

allocend:
    return (slot + 1);
}

int g_timing = 0;

int cah_em4_setTimingRef84 (port_mid, byClockNum, bEnable)
{
    WORD   wRtn       = ((WORD)0x0) ;
    int byPortNo = port_mid;

if (g_timing) printf ("\r\n%s(): port_mid(%d)", __FUNCTION__, port_mid);

    if (bEnable)
    {
    	 
    	wRtn = cah_Write8Reg (0x70000000 ,                      0x1b  + byClockNum,                      byPortNo,                      0x07,                      CAH_REG_ATTRBT_RW);




	     
    	wRtn = cah_Write8Reg (0x70000000 ,                      0x1b  + byClockNum,                      0x0,                      0x08,                      CAH_REG_ATTRBT_RW);




	
    } else {
    	
    	wRtn = cah_Write8Reg (0x70000000 ,                      0x1b  + byClockNum,                      0x07,                      0x07,                      CAH_REG_ATTRBT_RW);




	 
    	wRtn = cah_Write8Reg (0x70000000 ,                      0x1b  + byClockNum,                       0x0,                       0x08 ,                       CAH_REG_ATTRBT_RW);




	
    }
     
    return wRtn;
}

void cah_em4_InitTiming (void)
{
    static int flag = 0;

    if (flag == 1)
    {   
        return ;
    }
   
    flag = 1;
 
    cah_em4_wr ((void *)(0x50000000) , (0x05 << 10) | 0x200 , (0x3f)  | ((0x3f)  << 8));
    cah_em4_wr ((void *)(0x50000000) , (0x05 << 10) | 0x201 , (0x3f)  | ((0x3f)  << 8));
    cah_em4_wr ((void *)(0x60000000) , (0x05 << 10) | 0x200 , (0x3f)  | ((0x3f)  << 8));
    cah_em4_wr ((void *)(0x60000000) , (0x05 << 10) | 0x201 , (0x3f)  | ((0x3f)  << 8));
}

int cah_em4_initPortMap (void)
{
    int i = 0;

    memset (g_phyportmap, 0, 4 * 130);
    for (i = 0; i < 130; i ++)
    {
        g_phyportmap[i] = i;
    }

    g_phyportmap[9] = 9;
    g_phyportmap[10] = 10;
    g_phyportmap[11] = 11;
    g_phyportmap[12] = 12;
    g_phyportmap[34] = 34;
    g_phyportmap[35] = 35;
    g_phyportmap[39] = 39;
    g_phyportmap[40] = 40;
    g_phyportmap[41] = 41;
    g_phyportmap[42] = 42;
    g_phyportmap[43] = 43;
    g_phyportmap[44] = 44;
    g_phyportmap[63] = 63;
    g_phyportmap[64] = 64;
# 6081 "cah_126e1.c"


    return 0;
}



unsigned short em4_bpc_wr (int page, int off, unsigned short val);
WORD
CAH_EM4_SetTimingRef (DWORD dwPos, BYTE byPort, BYTE byClockNum, BOOL bEnable)
{
    int port_mid = 0 , n = 0;
    void * base = ((void *)0) ;
    BYTE byPortCopy = SDD_126xE1IOPortConvert(byPort), realport = 0;
    BOOL status;
    unsigned short c1 = 0, c2 = 0, ccc = 0;
    unsigned char pt = 0;

     
    {if (125 <  SDD_126xE1IOPortConvert(byPort) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPort) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    cah_em4_registerCall ((PMyFun) CAH_EM4_SetTimingRef, (int) SDD_126xE1IOPortConvert(byPort), (int) byClockNum, bEnable, 1, __FUNCTION__);
    
    if (0 == bEnable)
    {
        CAH_EM4_GetTimingRef (0, byClockNum, &realport, &status);
        byPortCopy = realport;
        if (g_testtiming) printf ("\r\nGetRef() GOT: byClockNum(%d), realport(%03d)",byClockNum, realport); 
    }

    n = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPort)) - 1;
    base = cah_em4_getEntityBase (0, byPortCopy);     

    port_mid = em4_getSlotFrom126port (byPortCopy, bEnable);
    if (g_testtiming) printf ("\r\n%s(): byPortCopy(%03d)(%03d), byClockNum(%d), en(%d), mid(%d)", __FUNCTION__, byPortCopy, byPort, byClockNum, bEnable, port_mid);
    printf ("\r\n%s(): byPortCopy(%03d)(%03d), byClockNum(%d), en(%d), mid(%d)", __FUNCTION__, byPortCopy, byPort, byClockNum, bEnable, port_mid);








 
    c1 = em4Map1Rd (5, 0x200);
    pt = (c1 & 0xff) + 1; if (pt > 129) return (((WORD)0x2) );
    ccc = (g_phyportmap[pt] - 1);
    c1 &= 0xff00;
    c1 |= ccc;

    pt = ((c1 & 0xff00)>>8) + 1; if (pt > 129) return (((WORD)0x2) );
    ccc = (g_phyportmap[pt] - 1);
    c1 &= 0xff;
    c1 |= ccc << 8;

 
    c2 = em4Map1Rd (5, 0x201);
    pt = (c2 & 0xff) + 1; if (pt > 129) return (((WORD)0x2) );
    ccc = (g_phyportmap[pt] - 1);
    c2 &= 0xff00;
    c2 |= ccc;

    pt = ((c2 & 0xff00)>>8) + 1; if (pt > 129) return (((WORD)0x2) );
    ccc = (g_phyportmap[pt] - 1);
    c2 &= 0xff;
    c2 |= ccc << 8;


    em4_bpc_wr (0x03, 0x07, c1);
    em4_bpc_wr (0x03, 0x08, c2);


 

 
    c1 = em4Map2Rd (5, 0x200);
    pt = (c1 & 0xff) + 1; if (pt > 129) return (((WORD)0x2) );
    ccc = (g_phyportmap[pt] - 1);
    c1 &= 0xff00;
    c1 |= ccc;

    pt = ((c1 & 0xff00)>>8) + 1; if (pt > 129) return (((WORD)0x2) );
    ccc = (g_phyportmap[pt] - 1);
    c1 &= 0xff;
    c1 |= ccc << 8;

 
    c2 = em4Map2Rd (5, 0x201);
    pt = (c2 & 0xff) + 1; if (pt > 129) return (((WORD)0x2) );
    ccc = (g_phyportmap[pt] - 1);
    c2 &= 0xff00;
    c2 |= ccc;

    pt = ((c2 & 0xff00)>>8) + 1; if (pt > 129) return (((WORD)0x2) );
    ccc = (g_phyportmap[pt] - 1);
    c2 &= 0xff;
    c2 |= ccc << 8;

    em4_bpc_wr (0x03, 0x09, c1);
    em4_bpc_wr (0x03, 0x0a, c2);




    cah_em4_setTimingRef84 (port_mid - 1, byClockNum, bEnable);

    return ((WORD)0x0) ;
}

PB_ERROR_CODE_T
CAH_EM4_setTransPsl (DWORD dwPos, UINT32 ulChip, UINT32 nulPort, UINT8 PSL)
{




    void *base = 0x0000;
    int ulPort = 0;

	{if (125 <  nulPort ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  nulPort ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
     ulPort = cah_em4_mapPort (nulPort) - 1;
    cah_em4_registerCall ((PMyFun) CAH_EM4_setTransPsl, (int) nulPort, (int) PSL, 0, 1, __FUNCTION__);
    base = (void *) cah_em4_getEntityBase (0, nulPort);
   em4SetPsl2 (base, ulPort, PSL);
   

    return ((WORD)0x0) ;

}

 
WORD
CAH_EM4_SetRxExpLpPsl (DWORD dwPos, BYTE byPortNo, BYTE byC2Value)
{




    void *base = 0x00000;
    int ulPort = SDD_126xE1IOPortConvert(byPortNo);

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
   ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
    base = (void *) cah_em4_getEntityBase(0, SDD_126xE1IOPortConvert(byPortNo));
    cah_em4_registerCall ((PMyFun) CAH_EM4_SetRxExpLpPsl, (int) SDD_126xE1IOPortConvert(byPortNo), (int) byC2Value, 0, 2, __FUNCTION__);
    em4SetPslexp (base, ulPort, byC2Value);

    return ((WORD)0x0) ;

}

void
cah_em4_get_j2acc (void *baseaddr, int port, unsigned char *j2exp)
{
    unsigned short u = 0;
    int i = 0;

    for (i = 0; i < 16; i++)
    {
        cah_em4_wr (baseaddr, (0x02 << 10) | 0x01 , 0x8000 + (port << 8) + (0x10  + i));
        cah_em4_pollBit ((unsigned short *) baseaddr, (0x02 << 10) | 0x01 );
        u = cah_em4_rd (baseaddr, (0x02 << 10) | 0x03 );
        *(j2exp + i) = u & (0x00ff);
    }
}

int em4TtiTxTest (int port)
{
	{if (125 <  port ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  port ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	 
	CAH_EM4_SetTxLpTti (0, port, TTI_LEN_16BYTE, "f1234567890ABCDE");
	return 0;
}


int em4TtiRxTest (int n)
{
	unsigned char tti[16] ;

	{if (125 <  n ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  n ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	memset (tti, 0, 16);
	CAH_EM4_GetRxRcvLpTti (0, n, TTI_LEN_16BYTE, tti);
	tti[15] = '\0';
	printf ("%s(): tti(%s)", __FUNCTION__, tti);
	return 0;
}

 
WORD
CAH_EM4_GetRxRcvLpTti (DWORD dwPos, BYTE byPortNo, TTI_LEN_T eTtiMode, BYTE * pbyTtiValue)
{
	int entity = 0;
	int i = 0;
	int ulPort = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    if (!pbyTtiValue)
    {
        printf ("\r\nEM4_GetAccTti: entity(%d), file(%s) line(%d), tti is NULL!  \r\n", entity, "cah_126e1.c", 6279);
        return PB_ERR_PARAM;
    }
	
	ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
	cah_em4_get_j2acc (cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo)), ulPort, pbyTtiValue);
	if (TTI_LEN_16BYTE == eTtiMode)
	{
	} else if (TTI_LEN_FIXED1B == eTtiMode) {
		for (i = 1; i < 16; i ++)
		{
			pbyTtiValue[i] = pbyTtiValue[0];
		}
	}

    return ((WORD)0x0) ;
}

int em4SetExpPslTest (int port, int c2)
{
	{if (125 <  port ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  port ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	CAH_EM4_SetRxExpLpPsl (0, port, c2);
	printf ("\r\n%s(): write c2(%d)", __FUNCTION__, c2);
	return 0;
}

int em4GetExpPslTest (int port)
{
	unsigned char u = 0;

	{if (125 <  port ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  port ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	CAH_EM4_GetRxExpLpPsl (0, port, &u);
	printf ("\r\npsl(%x)", u);
	printf ("\r\n");
	return 0;
}

 
WORD
CAH_EM4_GetRxExpLpPsl (DWORD dwPos, BYTE byPortNo, BYTE * byC2Value)
{
    void *base = 0x00000;
    int ulPort = SDD_126xE1IOPortConvert(byPortNo);
	unsigned short u = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
    base = (void *) cah_em4_getEntityBase(0, SDD_126xE1IOPortConvert(byPortNo));
    cah_em4_registerCall ((PMyFun) CAH_EM4_SetRxExpLpPsl, (int) SDD_126xE1IOPortConvert(byPortNo), (int) byC2Value, 0, 2, __FUNCTION__);
    u = cah_em4_pohRead (base,  SDD_126xE1IOPortConvert(byPortNo), 0x00 );
	u = u & (0x3800);
	u = (u >> 11);
	*byC2Value = u;

    return ((WORD)0x0) ;
}

 
WORD
CAH_EM4_SetTxLpPsl (DWORD dwPos, BYTE byPortNo, BYTE byC2Value)
{
	return CAH_EM4_setTransPsl  (dwPos, 0, SDD_126xE1IOPortConvert(byPortNo), byC2Value);
}

 
WORD
CAH_EM4_SetRxTxLpPsl (DWORD dwPos, BYTE byPortNo, BYTE byC2Value)
{
	return CAH_EM4_setTransPsl  (dwPos, 0, SDD_126xE1IOPortConvert(byPortNo), byC2Value);
}

int em4GetRxRcvPslTest (int port)
{
	unsigned char c2 = 0;

	{if (125 <  port ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  port ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	CAH_EM4_GetRxRcvLpPsl (0, port, &c2);
	printf ("\r\nc2(%x)", c2);
	printf ("\r\n\r\n");
	return 0;
}

 
WORD
CAH_EM4_GetRxRcvLpPsl (DWORD dwPos, UINT32 byPortNo, BYTE * pbyC2Value)
{




    void *base = 0x0000;
    unsigned short u = 0x0000;
    int ulPort = SDD_126xE1IOPortConvert(byPortNo);
    int nulPort = SDD_126xE1IOPortConvert(byPortNo);

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    ulPort = cah_em4_mapPort (nulPort) - 1;

    if (!pbyC2Value)
    {
        printf ("\r\n%s(): pPSL is NULL!", __FUNCTION__);
        return ((WORD)0x1) ;
    }

    base = (void *) cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));
    cah_em4_getTslNow (base,SDD_126xE1IOPortConvert(byPortNo), &u);
    *pbyC2Value = u;
    return ((WORD)0x0) ;

}


 


void
em4_PollBit (DWORD dwPos, DWORD addrOffset, int byPortNo)
{
    WORD i = 0;
    WORD data = 0x0000;
    int chipno = cah_em4_port2chip (byPortNo);
    DWORD base = cah_em4_getChipBase (0, chipno);

    for (;;)
    {

         
        cah_ReadRegister (base, (addrOffset), &data, 0xFFFF, CAH_REG_ATTRBT_RW);
        if (data & 0x8000)
        {
            i = (WORD) (i + 1);
            if (i >= 100 )
            {
                 

                 

                break;
            }
        }
        else
        {
            break;
        }
    }
}

 
PB_ERROR_CODE_T
CAH_EM4_enablePlm (DWORD dwPos, UINT32 ulChip, UINT32 nulPort, BOOL bEnable)
{




    CAH_EM4_ENTITY_DATA_T *pEntity;
    void *baseaddr = ((void *)0) ;
    ENTITY_T entity = 0;
    int ulPort = 0;

	{if (125 <  nulPort ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  nulPort ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    pEntity = cah_em4_getEntityPtr (entity);
    ulPort = cah_em4_mapPort (nulPort) - 1;
    cah_em4_registerCall ((PMyFun) CAH_EM4_enablePlm, (int) nulPort, (int) bEnable, 0, 4, __FUNCTION__);

    baseaddr = cah_em4_getEntityBase (0, nulPort);
    cah_em4_enablePlm (baseaddr, 0, ulPort, bEnable);
    


    return ((WORD)0x0) ;

}


WORD
CAH_EM4_SetRxLpPslEnable (DWORD dwPos, BYTE byPortNo, BOOL bEnable)
{
    WORD data;

	data = 0;
	CAH_EM4_enablePlm (dwPos, 0, SDD_126xE1IOPortConvert(byPortNo), bEnable);

    return ((WORD)0x0) ;
}


 
WORD
CAH_EM4_SetTxLpTti (DWORD dwPos, UINT32 byPortNo, TTI_LEN_T eTtiMode, const BYTE * pbyTtiValue)
{





    void *base = 0x0000;
	int i = 0;
	unsigned char mytti[16];
    ENTITY_T entity = 0;
	int ulPort = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
       ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
	memset (mytti, 0, 16);

    cah_em4_registerCall ((PMyFun) CAH_EM4_SetTxLpTti, (int) SDD_126xE1IOPortConvert(byPortNo), (int) pbyTtiValue, 0, 5, __FUNCTION__);

    base = (void *) cah_em4_getEntityBase (entity, SDD_126xE1IOPortConvert(byPortNo));
	memcpy (mytti, pbyTtiValue, 16);
	if (TTI_LEN_FIXED1B == eTtiMode)
	{
		for (i = 0; i < 16; i ++) mytti[i] = pbyTtiValue[0];
	}
    cahem4setTxj2_2 (base,  ulPort, mytti);
   

    return ((WORD)0x0) ;

}


 
WORD
CAH_EM4_SetRxExpLpTti (DWORD dwPos, UINT32 byPortNo, TTI_LEN_T eTtiMode, const BYTE * pbyTtiValue)
{





    void *base = 0x0000;
    int ulPort = 0;
	char mytti[16];
	int i = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
      ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;

	memset (mytti, 0, 16); memcpy (mytti, pbyTtiValue, 16);
    cah_em4_registerCall ((PMyFun) CAH_EM4_SetRxExpLpTti, (int) SDD_126xE1IOPortConvert(byPortNo), (int) pbyTtiValue, 0, 6, __FUNCTION__);
	
    base = (void *) cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));
	if (TTI_LEN_FIXED1B == eTtiMode)
	{
		 
		for (i = 0; i < 16; i ++) mytti[i] = pbyTtiValue[1];
	}
   cah_em4_set_j2exp (base, ulPort, mytti);
  

    return ((WORD)0x0) ;

}

int em4GetExpTtiTest (int port)
{
	unsigned char tti[16];
	int i = 0;

	memset (tti, 0, 16);
	CAH_EM4_GetRxExpLpTti (0, port, TTI_LEN_16BYTE, tti);
	tti[15] = '\0';
	printf ("\r\ntti(%s)", tti);
	printf ("\r\n\t");
	for (i = 0; i < 16; i ++)
	{
		printf ("(%x)", tti[i]);
	}
	printf ("\r\n");

	return 0;
}

int em4SetExpTtiTest (int port)
{
	{if (125 <  port ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  port ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	CAH_EM4_SetRxExpLpTti (0, port, TTI_LEN_16BYTE, "f0987654321abcde");
	return 0;
}

 
WORD
CAH_EM4_GetRxExpLpTti (DWORD dwPos, BYTE byPortNo, TTI_LEN_T eTtiMode, BYTE * pbyTtiValue)
{
	void * base = 0;
	int ulPort = 0;
	unsigned char mytti[16];

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
	base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));
       cah_em4_get_j2exp (base, ulPort, mytti);
      
	memcpy (pbyTtiValue, mytti, 16);
    return 0;
}

int cah_em4_ManualInsertPayloadAIS (int byPortNo, int flag )
{
     void * base = 0;
     int byte = 0;
      int ulPort = 0;
      int bit = 0;
      unsigned short val = 0;

       ulPort = cah_em4_mapPort (byPortNo) - 1;
	base = cah_em4_getEntityBase (0, byPortNo);
	byte = ulPort / 16;
	bit = ulPort % 16;
	

	val = cah_em4_rd (base, ((((0x00 << 10) | 0x20) ) + byte));
	if (flag == 1 )
	{
        val = val | (1 << bit);
	} else {
        val = val & (~(1 << bit));
	}

	cah_em4_wr (base, ((((0x00 << 10) | 0x20) ) + byte), val);

    return 0;
}
 










WORD
CAH_EM4_SetInsertAlm (DWORD dwPos, BYTE byPortNo, CTD_INSERT_ALM_TYPE_T eAlmType,
                      CTD_INSERT_ALM_DIR_T eInsAlmDir, BOOL bEnable)
{
	void * base = 0;
	int port = 0;
	unsigned short alm = 0;
	UINT8 byLoop = 0x0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    cah_em4_registerCall ((PMyFun) CAH_EM4_SetInsertAlm, (int) SDD_126xE1IOPortConvert(byPortNo), (int) eAlmType, bEnable, 8, __FUNCTION__);

	base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));
	port = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
       cah_em4_wr (base, (0x04 << 10) | 0x30 , port);       
       
	alm = cah_em4_rd (base, (0x04 << 10) | 0x40 );

	if (CTD_INSERT_ALM_TYPE_TUAIS == eAlmType 
		&& CTD_INSERT_ALM_OUT_LASER == eInsAlmDir)
	{
		if (1  == bEnable)
		{
			alm |= 0x8000;
		} else {
			alm &= ~0x8000;
		}
              cah_em4_wr (base, (0x04 << 10) | 0x30 , port);       
		alm = cah_em4_wr (base, (0x04 << 10) | 0x40 , alm);
	}

	if (CTD_INSERT_ALM_TYPE_AIS == eAlmType 
		&& CTD_INSERT_ALM_OUT_LASER == eInsAlmDir)
	{
		if (1  == bEnable)
		{
		}
		else 
		{
            if (SDD_126xE1IOPortConvert(byPortNo) == 0)
            {
                for (byLoop=0;byLoop<126;byLoop++)
	                cah_em4_ManualInsertPayloadAIS(byLoop, 0 );
			}
		}
	}
	if (CTD_INSERT_ALM_TYPE_RDI == eAlmType
		&& CTD_INSERT_ALM_OUT_LASER == eInsAlmDir)
	{
		if (1  == bEnable)
		{
			alm |= 0x0001;
            alm &= ~0x0002;
		} else {
			alm &= ~0x0001;
            alm |= 0x0002;
		}
              cah_em4_wr (base, (0x04 << 10) | 0x30 , port);       
	      alm = cah_em4_wr (base, (0x04 << 10) | 0x40 , alm);
	}
	
    return ((WORD)0x0) ;
}



 
WORD
CAH_EM4_SetRxLpTtiAlmEnable (DWORD dwPos, BYTE byPortNo, BOOL bEnable)
{




    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;
    void *baseaddr = ((void *)0) ;
    int ulPort = 0;

    cah_em4_trace (15, "\r\n%s:, ulPort(%d) bEnable(%d)", __FUNCTION__, ulPort, bEnable);
    

    ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
    pEntity = cah_em4_getEntityPtr (0);
    cah_em4_registerCall ((PMyFun) CAH_EM4_SetRxLpTtiAlmEnable, (int) SDD_126xE1IOPortConvert(byPortNo), (int) bEnable, 0, 8, __FUNCTION__);
    baseaddr = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));
    cah_em4_enableTimRdi (baseaddr, 0, ulPort, bEnable);
    

    return ((WORD)0x0) ;

}

 
WORD
CAH_EM4_SetRxLpTtiAisEnable (DWORD dwPos, BYTE byPortNo, BOOL bEnable)
{




    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;
    void *baseaddr = ((void *)0) ;
    int ulPort = 0;

    cah_em4_trace (15, "\r\n%s:, ulPort(%d) bEnable(%d)", __FUNCTION__, ulPort, bEnable);
    
    ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
    pEntity = cah_em4_getEntityPtr (0);
    cah_em4_registerCall ((PMyFun) CAH_EM4_SetRxLpTtiAlmEnable, (int) SDD_126xE1IOPortConvert(byPortNo), (int) bEnable, 0, 8, __FUNCTION__);
    baseaddr = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));
    cah_em4_enableTimAis (baseaddr, 0, ulPort, bEnable);
    

    return ((WORD)0x0) ;

}

 
WORD
CAH_EM4_SetTxLpTtiMode (DWORD dwPos, UINT32 byPortNo, TTI_LEN_T eTtiMode)
{



    return ((WORD)0x0) ;

    return ((WORD)0x0) ;
}

UINT8
cah_em4_readChReg (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort, UINT32 addrOffset)
{
    UINT8 data = 0;
    UINT32 offset = (0x2000  + ulPort * 0x1000  + addrOffset);

    data = (*((volatile UINT8 *)( (pEntity->chip_var[ulChip].cfg.baseAddr) )+(  offset ))) ;

    return data;
}


void
cah_em4_writeChReg (CAH_EM4_ENTITY_DATA_T * pEntity, UINT32 ulChip, UINT32 ulPort,
                    UINT32 addrOffset, UINT8 data)
{
    UINT32 offset = (0x2000  + ulPort * 0x1000  + addrOffset);

    (*((volatile UINT8 *)( (pEntity->chip_var[ulChip].cfg.baseAddr) )+(  offset )))=(UINT8)(  data ) ;
}


 
WORD
CAH_EM4_SetRxLpTtiMode (DWORD dwPos, UINT32 byPortNo, TTI_LEN_T eTtiMode)
{




    return ((WORD)0x0) ;
}

PB_ERROR_CODE_T
CAH_EM4_setPdhPortMode (DWORD dwPos, UINT32 ulChip, UINT32 nulPort, PDH_PORT_MODE_T type)
{




    void *base = 0x0000;
    int port = 0;
    unsigned short u = 0x0000;
    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;
    ENTITY_T entity = 0;
    int ulPort = 0;

    ulPort = cah_em4_mapPort (nulPort);


    pEntity = cah_em4_getEntityPtr (entity);
    cah_em4_registerCall ((PMyFun) CAH_EM4_setPdhPortMode, (int) nulPort, (int) type, 0, 11, __FUNCTION__);

    if (pEntity->state != eEm4EntAct)
    {
         
        return PB_ERR_STATE;
    }
    base = (void *) cah_em4_getEntityBase (entity, ulPort);
    
   u = cah_em4_pohRead (base, port = ulPort, 0x00 );
   
    u &= ~0x000c;                
    cah_em4_pohWrite (base, port, 0x00 , u);
    

    return ((WORD)0x0) ;




    return 0;
}



 


PB_ERROR_CODE_T
CAH_EM4_getPortType (DWORD dwPos, UINT32 ulChip, UINT32 nulPort, PDH_PORT_MODE_T * pType)
{





    CAH_EM4_ENTITY_DATA_T *pEntity;
    ENTITY_T entity = 0;
    int ulPort = 0;

    ulPort = cah_em4_mapPort (nulPort);


    if ((((  entity   >= ((1) ) )) || ((   ulChip  )  >=  (1) ) || ((   ulPort  ) >= (63 ) ) ) )
    {
        printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 6839);
        return PB_ERR_PARAM;
    }

    if (!pType)
    {
        printf ("\r\nEntity%d: %s %d EM4_GetPortType pType is NULL!  \r\n", entity, "cah_126e1.c",
                6846);
        return PB_ERR_PARAM;
    }

    pEntity = cah_em4_getEntityPtr (entity);
    if (pEntity->state == eEm4EntInit)
    {
        return ((WORD)0x0) ;
    }

    if (pEntity->state != eEm4EntAct)
    {
         
        return PB_ERR_STATE;
    }

   *pType = pEntity->chip_var[ulChip].portType[ulPort];
  


    return ((WORD)0x0) ;




    return 0;
}


 



PB_ERROR_CODE_T
CAH_EM4_enablePort (DWORD dwPos, UINT32 ulChip, UINT32 ulPort, PORT_STATE_T state)
{





# 6931 "cah_126e1.c"

    return ((WORD)0x0) ;





    return 0;
}



 



PB_ERROR_CODE_T
CAH_EM4_getPortState (DWORD dwPos, UINT32 ulChip, UINT32 nulPort, PORT_STATE_T * pState)
{




    CAH_EM4_ENTITY_DATA_T *pEntity;
    ENTITY_T entity = 0;
    int ulPort = 0;

    ulPort = cah_em4_mapPort (nulPort) - 1;


     if ((((  entity   >= ((1) ) )) || ((   ulChip  )  >=  (1) ) || ((   ulPort  ) >= (63 ) ) ) )     
    {
        printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 6963);
        return PB_ERR_PARAM;
    }

    if (!pState)
    {
        printf ("\r\nEntity%d: %s %d EM4_GetPortState pState is NULL!  \r\n", entity, "cah_126e1.c",
                6970);
        return PB_ERR_PARAM;
    }

    pEntity = cah_em4_getEntityPtr (entity);
    if (pEntity->state == eEm4EntInit)
    {
        return ((WORD)0x0) ;
    }

    if (pEntity->state != eEm4EntAct)
    {
         
        return PB_ERR_STATE;
    }

   *pState = pEntity->chip_var[ulChip].adminState[ulPort];
  

    return ((WORD)0x0) ;




    return 0;
}


WORD
CAH_EM4_EnablePrbs (DWORD dwPos, UINT32 byPortNo, BOOL bEnable)
{




      int port = 0;
	void * base = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
       port = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
	base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));

    cah_em4_registerCall ((PMyFun) CAH_EM4_EnablePrbs, (int) SDD_126xE1IOPortConvert(byPortNo), (int) bEnable, 0, 17, __FUNCTION__);

    if (bEnable)
    {
        cah_em4_setPrbs2 (base, port, eEM4_ENABLE);
        
    }
    else
    {
        cah_em4_setPrbs2 (base, port, eEM4_DISABLE);
        
    }
    return ((WORD)0x0) ;

}

PB_ERROR_CODE_T
CAH_EM4_getPrbsMode (DWORD dwPos, UINT32 ulChip, UINT32 nulPort, PRBS_MODE_T * pMode)
{




    CAH_EM4_ENTITY_DATA_T *pEntity;
    ENTITY_T entity = 0;
    int ulPort = 0;

    ulPort = cah_em4_mapPort (nulPort);


    if ((((  entity   >= ((1) ) )) || ((   ulChip  )  >=  (1) ) || ((   ulPort  ) >= (63 ) ) ) )     
    {
        printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 7044);
        return PB_ERR_PARAM;
    }

    if (!pMode)
    {
        printf ("\r\nEntity%d: %s %d EM4_GetPrbsMode pMode is NULL!  \r\n", entity, "cah_126e1.c",
                7051);
        return PB_ERR_PARAM;
    }

    pEntity = cah_em4_getEntityPtr (entity);
    if (pEntity->state == eEm4EntInit)
    {
        return ((WORD)0x0) ;
    }

    if (pEntity->state != eEm4EntAct)
    {
         
        return PB_ERR_STATE;
    }

    *pMode = pEntity->chip_var[ulChip].prbsCfg[ulPort].prbsMode;    
    return ((WORD)0x0) ;





    return 0;
}

# 7136 "cah_126e1.c"


# 7147 "cah_126e1.c"



int em4GetPrbsErrTest (int port)
{
    DWORD cnt = 0;

    {if (125 <  port ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  port ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;

    CAH_EM4_GetPrbsErrCount (0, port, &cnt);
    printf ("\r\n%s(): cnt(%d)", __FUNCTION__, cnt);
    printf ("\r\n");
    return 0;
}
 


WORD
CAH_EM4_GetPrbsErrCount (DWORD dwPos, UINT32 byPortNo, DWORD * dwPrbsCnt)
{




    int ulPort = 0;
    int nulPort = SDD_126xE1IOPortConvert(byPortNo);
	void * base = 0;
	unsigned short c1 = 0, c2 = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
       ulPort = cah_em4_mapPort (nulPort) - 1;
	base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));

     












	
    cah_em4_wr (base, (0x05 << 10) | 0x20 , ulPort & 0x003f);    
    c2 = cah_em4_rd (base, (0x05 << 10) | 0x22 );
    c1 = cah_em4_rd (base, (0x05 << 10) | 0x21 );
	*dwPrbsCnt = (c2 + (c1 << 16));
    return ((WORD)0x0) ;

}

PB_ERROR_CODE_T
CAH_EM4_forceRptDefect (DWORD dwPos, UINT32 ulChip, UINT32 nulPort)
{





    CAH_EM4_ENTITY_DATA_T *pEntity;
    ENTITY_T entity = 0;
    int ulPort = 0;

    ulPort = cah_em4_mapPort (nulPort);


    if ((((  entity   >= ((1) ) )) || ((   ulChip  )  >=  (1) ) || ((   ulPort  ) >= (63 ) ) ) )  
    {
        printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 7219);
        return PB_ERR_PARAM;
    }

    pEntity = cah_em4_getEntityPtr (entity);
    if (pEntity->state != eEm4EntAct)
    {
        printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 7226);
        return PB_ERR_STATE;
    }

    pEntity->chip_var[ulChip].forceRptAlm[ulPort] = 1 ;    

    return ((WORD)0x0) ;





    return 0;
}

PB_ERROR_CODE_T
CAH_EM4_forceReportEpg (DWORD dwPos)
{




    UINT8 chipNo;
    ENTITY_T entity = 0;

    chipNo = cah_em4_ent2chipno (entity);

    gCahEm4TribVar.EM4LatchForceRpt[chipNo] = 1 ;

    return ((WORD)0x0) ;

    return 0;
}



 


PB_ERROR_CODE_T
CAH_EM4_forceInsertAis (DWORD dwPos, UINT32 ulChip, UINT32 nulPort, ALARM_DIRECTION_T dir,
                        BOOL bEnable)
{




    int ulPort = 0;

    ulPort = cah_em4_mapPort (nulPort);
    return ((WORD)0x0) ;


    return 0;
}

 


PB_ERROR_CODE_T
CAH_EM4_forceInsertRdi (DWORD dwPos, UINT32 ulChip, UINT32 nulPort, BOOL bEnable)
{




    CAH_EM4_ENTITY_DATA_T *pEntity;
    void *base = 0x0000;
    int port = 0;
    unsigned short u = 0x0000;
    ENTITY_T entity = 0;
    int ulPort = 0;
    __EXAR_INSERT_CTRL_T insertCtrlReg;
    UINT8 G1 = 0;

    ulPort = cah_em4_mapPort (nulPort);

     



    if (1)
        return ((WORD)0x0) ;

    if ((((  entity   >= ((1) ) )) || ((   ulChip  )  >=  (1) ) || ((   ulPort  ) >= (63 ) ) ) )    
    {
        printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 7312);
        return PB_ERR_PARAM;
    }

    pEntity = cah_em4_getEntityPtr (entity);

    cah_em4_registerCall ((PMyFun) CAH_EM4_forceInsertRdi, (int) nulPort, (int) bEnable, 0, 21, __FUNCTION__);

     



    memset((INT1 *)( &insertCtrlReg ), (  0 ), (  sizeof (insertCtrlReg) )) ;
# 7344 "cah_126e1.c"

    base = (void *) cah_em4_getEntityBase (entity, port);
    cah_em4_wr (base, (0x04 << 10) | 0x30 , port);       
    taskDelay (5);
    u = cah_em4_rd (base, (0x04 << 10) | 0x40 );
    insertCtrlReg.insertCtrlBit.rdi = ((u & 0x00ff) & 0x0001) ? 1 : 0;

    if (bEnable)
    {
        insertCtrlReg.insertCtrlBit.rdi = 0x01;
        G1 = 0x08;
         
         pEntity->chip_var[ulChip].frdiMode[nulPort] = EM4_UP_FORCE;
    }
    else
    {
        insertCtrlReg.insertCtrlBit.rdi = 0x00;
        G1 = 0;
         
         pEntity->chip_var[ulChip].frdiMode[nulPort] = EM4_NONE_FORCE;
    }

 


     


    if (insertCtrlReg.insertCtrlBit.rdi)
    {
        u |= 0x0003;
    }
    else
    {
        u &= ~0x0001;
    }
    u |= 0x0002;
    u &= ~0x0020;
    cah_em4_wr (base, (0x04 << 10) | 0x40 , u);
    taskDelay (5);

    return ((WORD)0x0) ;


    return 0;
}

int em4GetPpiBipTest (int port)
{
    DWORD bip = 0, rei = 0;

    {if (125 <  port ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  port ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    CAH_EM4_GetPpiBipCount (0, port, &bip, &rei);
    printf ("%s(): bip(%d), rei(%d)", __FUNCTION__, bip, rei);
    printf ("\r\n");

    return 0;
}



WORD
CAH_EM4_GetPpiBipCount (DWORD dwPos, BYTE byPortNo, DWORD * pPpiBip, DWORD * pPpiFebe)
{
	int port = 0;
	void * base = 0;
	unsigned short remote = 0;
	unsigned short near = 0, err = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	port = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
	base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));





	err = cah_em4_readCrc4RemoteReg (base, port, &near, &remote);	

	
	*pPpiBip = near;
	*pPpiFebe = remote;

    return ((WORD)0x0) ;
}

int em4GetBipTest (int port)
{
    DWORD bip = 0, rei = 0;

    {if (125 <  port ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  port ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    CAH_EM4_GetLpBipCount (0, port, &bip, &rei);
    printf ("%s(): bip(%d), rei(%d)", __FUNCTION__, bip, rei);
    printf ("\r\n");

    return 0;
}


WORD
CAH_EM4_GetLpBipCount (DWORD dwPos, BYTE byPortNo, DWORD * pLpBip, DWORD * pLpRei)
{
	void * base = 0;
	int port = 0;
	unsigned short bip2 = 0;
	unsigned short rei = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;

	port = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
	base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));

	bip2 = cah_em4_pohRead (base, port, 0x05 );
	rei  = cah_em4_pohRead (base, port, 0x06 );	
	*pLpBip = bip2;
	*pLpRei = rei;

    return ((WORD)0x0) ;
}

PB_ERROR_CODE_T
CAH_EM4_getLpPmPara (DWORD dwPos, UINT32 ulChip, UINT32 nulPort, MSHPLP_PMPARA_T * pPmPara)
{





    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;
    UINT32 data = 0;
    UINT32 temp = 0;
    unsigned int d1 = 0, d2 = 0;
    MSHPLP_PMPARA_T pmParm;
    void *base = 0x0000;
    int port = 0;
    unsigned short u = 0x0000;
    ENTITY_T entity = 0;
    int ulPort = 0;

    ulPort = cah_em4_mapPort (nulPort);


    if ((((  entity   >= ((1) ) )) || ((   ulChip  )  >=  (1) ) || ((   ulPort  ) >= (63 ) ) ) )  
    {
        printf ("\r\n**Error!  Entity= %d; File=%s; Line=%d  \r\n", entity, "cah_126e1.c", 7488);
        return PB_ERR_PARAM;
    }

    if (!pPmPara)
    {
        printf ("\r\nEntity%d: %s %d EM4_GetLpPmPara pPmPara is NULL!  \r\n", entity, "cah_126e1.c",
                7495);
        return PB_ERR_PARAM;
    }

    if (((void *)0)  == (pEntity = cah_em4_getEntityPtr (entity)))
    {
        printf ("\r\nlppm: entity(%d) NULL", entity);
        return PB_ERR_PARAM;
    }

    if (pEntity->state == eEm4EntInit)
    {
        return ((WORD)0x0) ;
    }

    if (eEm4EntAct != pEntity->state)
    {
        printf ("\r\ngetpm: entity(%d), not active", entity);
        return PB_ERR_STATE;
    }

    memset((INT1 *)( &pmParm ), (  0 ), (  sizeof (MSHPLP_PMPARA_T) )) ;
     
     

# 7530 "cah_126e1.c"

    base = (void *) cah_em4_getEntityBase (entity, ulPort);
   
    u = port = ulPort;
    temp = cah_em4_pohRead (base, port, 0x05 );    
    d1 = data = temp;

    pmParm.bip = data;

    data = 0;

# 7551 "cah_126e1.c"

    base = (void *) cah_em4_getEntityBase (entity, ulPort);    
    u = port = ulPort;
    temp = cah_em4_pohRead (base, port, 0x06 );    
    d2 = data = (temp);

    pmParm.rei = data;


    if (pEntity->chip_var[ulChip].pohAlm[ulPort].ssf   )
    {
        pmParm.bip = 0;
        pmParm.rei = 0;
        pmParm.fefc = 0;
         
        pmParm.fc = 1 + pEntity->chip_var[ulChip].pohAlm[nulPort].ssfFc;
    }
    else
    {
        pmParm.fc = 0;
        if (pEntity->chip_var[ulChip].pohAlm[ulPort].rdi
              )
        {
            pmParm.fefc = 1
                + pEntity->chip_var[ulChip].pohAlm[ulPort].rdiFc
                + pEntity->chip_var[ulChip].pohAlm[ulPort].rfiFc;             
            pmParm.rei = 0;
        }                        



        else
        {
            pmParm.fefc = 0;
        }
    }

    if (pEntity->chip_var[ulChip].pohAlm[ulPort].ssf == 0x0100 )
    {
        pEntity->chip_var[ulChip].pohAlm[ulPort].ssf = 0;
    }
    pEntity->chip_var[ulChip].pohAlm[ulPort].ssfFc = 0;

    if (pEntity->chip_var[ulChip].pohAlm[ulPort].rdi == 0x0100 )
    {
        pEntity->chip_var[ulChip].pohAlm[ulPort].rdi = 0;
    }
    pEntity->chip_var[ulChip].pohAlm[ulPort].rdiFc = 0;
    if (pEntity->chip_var[ulChip].pohAlm[ulPort].rfi == 0x0100 )
    {
        pEntity->chip_var[ulChip].pohAlm[ulPort].rfi = 0;
    }
    pEntity->chip_var[ulChip].pohAlm[ulPort].rfiFc = 0;
    
    

    memcpy((INT1 *)( pPmPara ), (INT1 *)(  &pmParm ), (  sizeof (pmParm) )) ;

    cah_em4_trace (18, "ent(%d), port(%d), bip(%d:%d), rei(%d:%d), fc(%d), fefc(%d), ssf(%x)",
                   entity, ulPort, pPmPara->bip, d1, pPmPara->rei, d2, pPmPara->fc, pPmPara->fefc,
                   pEntity->chip_var[ulChip].pohAlm[ulPort].ssf);
    
    
    return ((WORD)0x0) ;








    return 0;
}


WORD
CAH_EM4_DetectCardState (DWORD dwPos, BOOL * pbActive)
{




    CAH_EM4_ENTITY_DATA_T *p= ((void *)0) ;
    PB_ERROR_CODE_T ret;

    if (!pbActive)
    {
        printf ("\r\n%s(): pbActive is NULL!  \r\n", __FUNCTION__);
        return PB_ERR_PARAM;
    }

    ret = cah_em4_detectCardStateEx (p, pbActive);

    return ret;

}

PB_ERROR_CODE_T
CAH_EM4_blockAIS (DWORD dwPos, BOOL bEnable)
{





    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;
    UINT32 ulChip = 0;
    UINT32 ulPort = 0;
    ENTITY_T entity = 0;

    pEntity = cah_em4_getEntityPtr (entity);

    cah_em4_registerCall ((PMyFun) CAH_EM4_blockAIS, 0, 0, bEnable, 24, __FUNCTION__);

    if (pEntity->state != eEm4EntAct)
    {
         
        return PB_ERR_STATE;
    }


    for (ulChip = 0; ulChip < pEntity->chipNum; ulChip++)
    {
        for (ulPort = 0; ulPort < (63 ) ; ulPort++)
        {
            if (bEnable)
            {
                pEntity->chip_var[ulChip].bBlockAis[ulPort] = 1 ;
                cah_em4_insertUpAis (entity, ulChip, ulPort, 0 );
            }
            else
            {
                pEntity->chip_var[ulChip].bBlockAis[ulPort] = 0 ;
            }
        }
    }

    return ((WORD)0x0) ;




    return 0;
}










WORD
CAH_EM4_Initialize (DWORD dwPos)
{
    printf ("\r\n%s():\t called dwPos(%d)", __FUNCTION__, dwPos);

	CAH_EM4_activateEntity (0, 0 );
    return ((WORD)0x0) ;
}



 





WORD
CAH_EM4_GetInnerB1Err (DWORD dwPos, BYTE byPortNo, DWORD *pdwB1Err)
{
    return ((WORD)0x0) ;
}



 





WORD
CAH_EM4_GetTdmLinkErr (DWORD dwPos, DWORD *pdwTdmErr)
{
    return ((WORD)0x0) ;
}


 




WORD
CAH_EM4_Get8KClkState (DWORD dwPos, BOOL *pbActive)
{
    return ((WORD)0x0) ;
}

 





WORD
CAH_EM4_Get19MClkState (DWORD dwPos, BOOL *pbActive)
{
    return ((WORD)0x0) ;
}

 




WORD
CAH_EM4_GetPllState (DWORD dwPos, BOOL * pbPllLock)
{
    unsigned char * p = (unsigned char *)0x70000110 ;

    *pbPllLock = ((*p) & 0x02) ? 1  : 0 ;

    return ((WORD)0x0) ;
}



 

































WORD
CAH_EM4_InitEntity (ENTITY_T etEntity, DWORD dwPos, CTD_MODE_TYPE_T eporttype)
{




    CAH_EM4_ENTITY_DATA_T *pEntity;
	int ulChip = 0, ulPort = 0, i = 0;

    cah_em4_showbuild ();
    CAH_EM4_loadModule ();

    printf ("\r\n%s():\tentity(%d),\tdwPos(%d),\tporttype(%d)", __FUNCTION__, etEntity, dwPos, eporttype);
    cah_em4_trace (1 , "%s: File(%s) Line(%d)", __FUNCTION__, "cah_126e1.c", 7832);

    if (((void *)0)  == (pEntity = cah_em4_OpenEx ()))
    {
        printf ("\r\n%s(): cah_em4_OpenEx() CANNOT GET FD!!!", __FUNCTION__);
        return ((WORD)0x1) ;
    }

    cah_em4_printf ("\r\n126xe1rt: pEntity(%p)", pEntity);

     


    pEntity->entity = 0;
    pEntity->dwPos = 0;
    pEntity->chipNum = 2;
    pEntity->state = eEm4EntInit;
    pEntity->chip_var[0].cfg.baseAddr = (0x50000000) ;
    pEntity->chip_var[1].cfg.baseAddr = (0x60000000) ;
    pEntity->chip_var[0].su_id = 0;
    pEntity->chip_var[1].su_id = 0;

    cah_em4_initVars (pEntity, 2);
    for (i = 0; i < 8; i ++) g_timingVirtualSlot[i] = 0;

    for (ulChip = 0; ulChip < (int)pEntity->chipNum; ulChip++)
    {
        for (ulPort = 0; ulPort < (63 ) ; ulPort++)
        {
             


            pEntity->chip_var[ulChip].forceRptAlm[ulPort] = 1 ;
            pEntity->chip_var[ulChip].prbsCfg[ulPort].bForceRpt = 1 ;
            pEntity->chip_var[ulChip].nPrbsDelayCount[ulPort] = 0;
        }
    }
    return ((WORD)0x0) ;

    return 0;
}


CAH_EM4_CHIP_CONFIG_T g_cahEm4ChipCfg[2];
CAH_EM4_CALLBACKS_T g_callback;

typedef struct
{
    UINT32 address;
    unsigned short value;
}
CAH_EM4_REG_CONF;

# 1 "../../../infra/inc/fpga_glue.h" 1
 























# 1 "../../../infra/inc/bspPublic.h" 1
 























extern struct _GlobalParams *pGlobalParams;

extern void sysHardDelayUs(UINT32 us);
extern void InitGlobalParams(void);

UINT GetBootProperties(void);
int GetBoardStyle(void);
int GetCardFunction(void);
UINT GetBoardProperties(void);
UINT GetBoardRev(void);
UINT GetChassisRev(void);

int GetFlashSize (void);
int GetRamSize (void);
int GetFlashType(void);
int GetSysFlashAddr (void);

BOOL GetBoardFlashBoot(void);
BOOL GetBoardFlashStorage(void);

UCHAR *GetBoardMacAddr(void);

UINT8 GetSlotNumber(void);

UCHAR *GetBootVersion(void);
UCHAR *GetBootDate(void);

u_long GetMainIP(void);

UINT32 GetBootHostAddress(void);
 
UINT32 ppcTBLfreq(void);
UINT32 ppcTBLrd(void);
UINT32 ppcTBUrd(void);

void ppcTBLwr(UINT32);
void ppcTBUwr(UINT32);

UINT8 sysModck13Get(void);
int sysClkRateGet(void);

void enablePrintProcess(void);
void disablePrintProcess(void);

int GetBoardSStatus(void);
void ClearBoardSStatus(int value);
void SetBoardSStatus(int value);
int GetBoardUStatus(void);
void SetBoardUStatus(int value);

UINT32 GetBpSerialNumber(char * buffer, UINT32 bufLen);
UINT32 GetBpManDate(char * buffer, UINT32 bufLen);


UINT32 GetSlotSerialNumber(PbSlotNumberType_Tval slot, char * buffer, UINT32 bufLen);
UINT32 GetSlotManDate(PbSlotNumberType_Tval slot, char * buffer, UINT32 bufLen);
UINT32 GetSlotPbCode(PbSlotNumberType_Tval slot, char * buffer, UINT32 bufLen);
UINT32 GetSlotSiProductNumber(PbSlotNumberType_Tval slot,char * buffer, UINT32 bufLen);
UINT32 GetSlotOEMProductNumber(PbSlotNumberType_Tval slot, char * buffer, UINT32 bufLen);
int GetSlotCardFunction(PbSlotNumberType_Tval slot);
int GetSlotBoardStyle(PbSlotNumberType_Tval slot);
UINT GetSlotBoardRev(PbSlotNumberType_Tval slot);
UINT GetSlotConfigOption(PbSlotNumberType_Tval slot);
void setFusionMisc(void);
UINT8 GetFusionMisc(void);
UINT8 scFpgaByteRead(int addr, BOOL readClear, const char * func, int line );
INT32 scFpgaByteWrite(int addr, UINT8 value, BOOL writeClear, const char * func, int line );
UINT8 scFpgaBitRead(int addr, UINT8 mask, BOOL readClear, const char * func, int line );
INT32 scFpgaBitWrite(int addr, UINT8 value, UINT8 mask, BOOL writeClear, const char * func, int line );


int slowJobAdd(FUNCPTR routine, int param1, int param2, int param3, int param4);

void RomWriteEnable(void);
void RomWriteDisable(void);
void setBootFlashVpp(int );
    














 




 




# 161 "../../../infra/inc/bspPublic.h"



























	
















# 257 "../../../infra/inc/bspPublic.h"







# 25 "../../../infra/inc/fpga_glue.h" 2


     

     






    typedef enum {
        FPGA_CTRL_OK = 0,
        FPGA_CTRL_ERROR = -1,
        FPGA_UNDEFINE_BOARDSTYLE = 101,
        FPGA_SETDABDL_FAIL,
        FPGA_SETDABACCESS_FAIL,
        FPGA_SETOFFLINE_FAIL,
        FPGA_SETCLOCK_FAIL,
        FPGA_RESETCARD_FAIL,
        FPGA_DLFPGA_FAIL,
        FPGA_INVALID_PARAM,
        FPGA_BOARD_REV_ERROR,
        FPGA_GLUE_TEST_ERROR
    }FPGA_DLFPGA_T;
     


     
    int fpga_InitCard ( int slot, BOOL bDLFPGA );






 






























# 7885 "cah_126e1.c" 2


void em4SetTbLoop (void)
{
	unsigned short u = 0;

	u = em4Map1Rd (0, 1);
	u |= 0x0002;
	em4Map1Wr (0, 1, u);

	u = em4Map2Rd (0, 1);
	u |= 0x0002;
	em4Map2Wr (0, 1, u);
	cah_em4_printf ("\r\n%s(): bpc loop set !!!", __FUNCTION__);
cah_em4_printf ("\r\n");
cah_em4_printf ("\r\n");
cah_em4_printf ("\r\n");
cah_em4_printf ("\r\n");
cah_em4_printf ("\r\n");
cah_em4_printf ("\r\n");
}



int wydact (void);
int
wydinit ()
{
    printf ("\r\n%s(): downloading fpga for slot 25...", __FUNCTION__);
    fpga_InitCard (GetSlotNumber (), 1);
printf ("\r\n");
printf ("\r\n");
printf ("\r\n");
printf ("\r\n");
printf ("\r\n");
printf ("\r\n");
    printf ("\r\n%s(): wait for 3 seconds between donwloading fpga and initialize procedure ...", __FUNCTION__);
    taskDelay (sysClkRateGet () * 3);
    memset (&g_cahEm4ChipCfg, 0, sizeof (CAH_EM4_CHIP_CONFIG_T));
    memset (&g_callback, 0, sizeof (CAH_EM4_CALLBACKS_T));
    if (((WORD)0x1)  == CAH_EM4_InitEntity (0, 0, 2))
	{
		printf ("\r\nCAH_EM4_InitEntity () error, abort !!!");
		return 0;
	}

    wydact ();
	taskDelay (sysClkRateGet () * 3);
	em4SetTbLoop ();
    return 0;
}


unsigned short
em4Map2Wr (int page, int off, unsigned short val)
{
    unsigned short u = 0;


    cah_em4_wr ((void *) (0x60000000) , ((page << 10) + off), val);
    return u;
}


unsigned short
em4Map1Wr (int page, int off, unsigned short val)
{
    unsigned short u = 0;


    cah_em4_wr ((void *) (0x50000000) , ((page << 10) + off), val);
    return u;
}

unsigned short
em4MapRd (int page, int off, unsigned short *pU1, unsigned short *pU2)
{
    unsigned short u2 = 0;
    unsigned short u1 = 0;
    *pU1 = u1 = cah_em4_rd ((void *) (0x50000000) , ((page << 10) + off));
    *pU2 = u2 = cah_em4_rd ((void *) (0x60000000) , ((page << 10) + off));

    return u1;
}

unsigned short
em4Map2Rd (int page, int off)
{
    unsigned short u1 = 0;
    u1 = cah_em4_rd ((void *) (0x60000000) , ((page << 10) + off));

    return u1;
}


unsigned short
em4Map1Rd (int page, int off)
{
    unsigned short u1 = 0;
    u1 = cah_em4_rd ((void *) (0x50000000) , ((page << 10) + off));

    return u1;
}

unsigned short
em4_bpc_wr (int page, int off, unsigned short val)
{
    unsigned short u = 0;


    cah_em4_wr ((void *) (0x80000000) , ((page << 10) + off), val);
    return u;
}

unsigned short
em4_fus_rd (void)
{
    unsigned short u = 0;
    u = cah_em4_rd ((void *) 0x70000010, 0);

    return u;
}

unsigned short
em4_bpc_rd (int page, int off)
{
    unsigned short u = 0;
    u = cah_em4_rd ((void *) (0x80000000) , ((page << 10) + off));

    return u;
}

static void
bpc_mgt_wps (void)
{
    unsigned short u = 0;

    u = em4_bpc_rd (1, 0x001);
    if (u & 0x8000)
    {
        printf ("\r\npage(1)off(0x001):\twork channel select");
    }
    else
    {
        printf ("\r\npage(1)off(0x001):\tprotect channel select");
    }
}

void
bpc_dcm_rst_al (void)
{
    unsigned short u = 0;

    printf ("\r\npage(4)off(0x000):\treset all dcm");
    em4_bpc_wr (4, 0x000, 0x007);
     
    u = 1;
    return;
}

void
bpc_dcm_st (void)
{
    unsigned short u1 = em4_bpc_rd (4, 0x010);
    unsigned short u2 = em4_bpc_rd (4, 0x011);

    printf ("\r\npage(4)off(0x010):\tclk77m locked: (%x)", u1);
    printf ("\r\npage(4)off(0x011):\tclk93m locked: (%x)", u2);

    return;
}

void
bpc_rst (void)
{
    em4_bpc_wr (0, 0x003, 0x0001);
     
    taskDelay (10);
    em4_bpc_wr (0, 0x003, 0x0000);
}

void
mgt_pwr (int flag)
{
    if (flag)
    {
        em4_bpc_wr (1, 0x20, 0);
        em4_bpc_wr (1, 0x30, 0);
    }
    else
    {
        em4_bpc_wr (1, 0x20, 1);
        em4_bpc_wr (1, 0x30, 1);
    }
}

void
bpc_mgt_descram (int ch, int en)
{
    unsigned short u = 0;

    if (ch)
    {
        cah_em4_printf ("\r\npage(1)off(0x34):\tprotect channel");
        u = em4_bpc_rd (1, 0x034);
    }
    else
    {
        cah_em4_printf ("\r\npage(1)off0x024):\twork channel");
        u = em4_bpc_rd (1, 0x024);
    }

    if (en)
    {
        cah_em4_printf ("(enable)");
        u = u | 0x0020;
    }
    else
    {
        cah_em4_printf ("(disable)");
        u = u & 0xffdf;
    }

    if (ch)
    {
        em4_bpc_wr (1, 0x034, u);
    }
    else
    {
        em4_bpc_wr (1, 0x024, u);
    }
}


void
bpc_mgt_up_scram (int en)
{
    unsigned short u = 0;

    u = em4_bpc_rd (1, 0x003);
    if (en)
    {
        cah_em4_printf ("\r\npage(1)off(0x003):\tmgt up scram enable");
        em4_bpc_wr (1, 0x003, u | 0x0001);
    }
    else
    {
        cah_em4_printf ("\r\npage(1)off(0x003):\tmgt up scram disable");
        em4_bpc_wr (1, 0x003, u & 0xfffe);
    }
}

 
















 
void
bpc_rst_mgt_fifo (void)
{
    unsigned short u = 0;

    cah_em4_printf ("\r\npage(1)off(0x020/0x030):\treset mgt tx/rx fifo ...");
    
     
    u = em4_bpc_rd (1, 0x003);
    em4_bpc_wr (1,  0x003, u | 0x0010);
     
    
    u = em4_bpc_rd (1, 0x020);
    em4_bpc_wr (1, 0x020, u | 0x0060);
    u = em4_bpc_rd (1, 0x030);
    em4_bpc_wr (1, 0x030, u | 0x0060);
    
    taskDelay (10);
    
      
    u = em4_bpc_rd (1, 0x003);
    em4_bpc_wr (1,  0x003, u & 0xFFEF);
     
    
    u = em4_bpc_rd (1, 0x020);
    em4_bpc_wr (1, 0x020, u & 0xff9f);
    u = em4_bpc_rd (1, 0x030);
    em4_bpc_wr (1, 0x030, u & 0xff9f);
    cah_em4_printf ("           (done)");
}

void
bpc_mgt_8k_mon_w (void)
{
    unsigned short dly8k_now = 0;
    unsigned short row_now = 0;
    unsigned short col_now = 0;
    unsigned short cyc_now = 0;

    unsigned short dly8k = 0;
    unsigned short cyc = 0;
    unsigned short col = 0;
    unsigned short row = 0;

    dly8k_now = em4_bpc_rd (1, 0x002);
    row_now = (dly8k_now & 0xf000) >> 12;
    col_now = (dly8k_now & 0x0ff8) >> 3;
    cyc_now = (dly8k_now & 0x0003);
    cah_em4_printf ("\r\npage(1)off(0x002):\tmgt c1 delay set now : row %d ; col %d ; cyc %d", row_now,
            col_now, cyc_now);

    cah_em4_printf ("\r\npage(1)off(0x026):\tRead out work channel phase ofset is : ");
    dly8k = em4_bpc_rd (1, 0x026);
    cyc = dly8k - ((dly8k / 4) * 4);
    col = dly8k / 4 - dly8k / (270 * 4) * 270;
    row = dly8k / (4 * 270);
    cah_em4_printf ("row : %d ; col : %d ; cyc : %d", row, col, cyc);

}


void
bpc_mgt_8k_mon_p (void)
{
    unsigned short dly8k_now = 0;
    unsigned short row_now = 0;
    unsigned short col_now = 0;
    unsigned short cyc_now = 0;

    unsigned short dly8k = 0;
    unsigned short cyc = 0;
    unsigned short col = 0;
    unsigned short row = 0;


    dly8k_now = em4_bpc_rd (1, 0x002);
    row_now = (dly8k_now & 0xf000) >> 12;
    col_now = (dly8k_now & 0x0ff8) >> 3;
    cyc_now = (dly8k_now & 0x003);
    cah_em4_printf ("\r\npage(1)off(0x002):\tmgt c1 delay set now : row %d ; col %d ; cyc %d", row_now,
            col_now, cyc_now);

    cah_em4_printf ("\r\npage(1)off(0x036):\tRead out work channel phase ofset is : ");
    dly8k = em4_bpc_rd (1, 0x036);
    cyc = dly8k - ((dly8k / 4) * 4);
    col = dly8k / 4 - dly8k / (270 * 4) * 270;
    row = dly8k / (4 * 270);
    cah_em4_printf ("row : %d ; col : %d ; cyc : %d", row, col, cyc);

}

void
set_c1j1_a (unsigned short row, unsigned short col, unsigned short cyc)
{
    em4_bpc_wr (2, 3, (row << 12) + (col << 3) + cyc);
}

void
set_c1j1_b (unsigned short row, unsigned short col, unsigned short cyc)
{
    em4_bpc_wr (2, 4, (row << 12) + (col << 3) + cyc);
}

int
em4MapShow ()
{
    unsigned short u1 = 0, u2 = 0;


    printf ("\r\n--- 126xE1 Mapper Fpga Chip ---");
    printf ("\r\n[page 0]");
    em4MapRd (0, 0, &u1, &u2);
    printf ("\r\noffset(0x00),\tval(0x%x)(0x%x): \tdevice version", u1, u2);


    em4MapRd (0, 0x01, &u1, &u2);
    printf ("\r\noffset(0x01),\tval(0x%x)(0x%x): \ttb_out_edge, tb_out_loop", u1, u2);

    em4MapRd (0, 0x02, &u1, &u2);
    printf ("\r\noffset(0x02),\tval(0x%x)(0x%x): \tdevice id", u1, u2);

    em4MapRd (0, 0x03, &u1, &u2);
    printf ("\r\noffset(0x03),\tval(0x%x)(0x%x): \tcontrol c1 add/drop", u1, u2);

    em4MapRd (0, 0x05, &u1, &u2);
    printf ("\r\noffset(0x05),\tval(0x%x)(0x%x): \tirq in", u1, u2);


    em4MapRd (0, 0x09, &u1, &u2);
    printf ("\r\noffset(0x09),\tval(0x%x)(0x%x): \tmask", u1, u2);

    em4MapRd (0, 0x0a, &u1, &u2);
    printf ("\r\noffset(0x0a),\tval(0x%x)(0x%x): \tdcm_locked/reset_cnt", u1, u2);

    em4MapRd (0, 0x0b, &u1, &u2);
    printf ("\r\noffset(0x0b),\tval(0x%x)(0x%x): \ttimeing_sel1", u1, u2);
    em4MapRd (0, 0x10, &u1, &u2);
    printf ("\r\noffset(0x10),\tval(0x%x)(0x%x): \ttest_reg", u1, u2);

    em4MapRd (0, 0x12, &u1, &u2);
    printf ("\r\noffset(0x12),\tval(0x%x)(0x%x): \te1_los", u1, u2);

    printf ("\r\n[page 0]");

    em4MapRd (1, 0x00, &u1, &u2);
    printf ("\r\noffset(0x00),\tval(0x%x)(0x%x): \th4_lom", u1, u2);

    em4MapRd (1, 0x01, &u1, &u2);
    printf ("\r\noffset(0x01),\tval(0x%x)(0x%x): \th4_lom_d", u1, u2);

    em4MapRd (1, 0x02, &u1, &u2);
    printf ("\r\noffset(0x02),\tval(0x%x)(0x%x): \th4_lom_m", u1, u2);

    em4MapRd (1, 0x03, &u1, &u2);
    printf ("\r\noffset(0x03),\tval(0x%x)(0x%x): \tlopi_vc12", u1, u2);

    em4MapRd (1, 0x04, &u1, &u2);
    printf ("\r\noffset(0x04),\tval(0x%x)(0x%x): \tlop_v1/v2", u1, u2);

    em4MapRd (1, 0x10, &u1, &u2);
    printf ("\r\noffset(0x10),\tval(0x%x)(0x%x): \tlopi_state", u1, u2);


    printf ("\r\n[page 2]: POH Monitor");

    printf ("\r\n");

    printf ("\r\n[page 4]: POH Monitor");

    em4MapRd (4, 0x000, &u1, &u2);
    printf ("\r\noffset(0x000),\tval(0x%x)(0x%x): \tc1_unstable/c1_delay/edge/rst", u1, u2);


    em4MapRd (4, 0x001, &u1, &u2);
    printf ("\r\noffset(0x001),\tval(0x%x)(0x%x): \tthresh_high/low", u1, u2);

    em4MapRd (4, 0x00c, &u1, &u2);
    printf ("\r\noffset(0x00c),\tval(0x%x)(0x%x): \tcfg_vc12_pdh", u1, u2);
    em4MapRd (4, 0x00d, &u1, &u2);
    printf ("\r\noffset(0x00d),\tval(0x%x)(0x%x): \tcfg_vc12_pdh", u1, u2);
    em4MapRd (4, 0x00e, &u1, &u2);
    printf ("\r\noffset(0x00e),\tval(0x%x)(0x%x): \tcfg_vc12_pdh", u1, u2);
    em4MapRd (4, 0x00f, &u1, &u2);
    printf ("\r\noffset(0x00f),\tval(0x%x)(0x%x): \tcfg_vc12_pdh", u1, u2);

    em4MapRd (4, 0x010, &u1, &u2);
    printf ("\r\noffset(0x010),\tval(0x%x)(0x%x): \te1_outloop", u1, u2);
    em4MapRd (4, 0x011, &u1, &u2);
    printf ("\r\noffset(0x011),\tval(0x%x)(0x%x): \te1_outloop", u1, u2);
    em4MapRd (4, 0x012, &u1, &u2);
    printf ("\r\noffset(0x012),\tval(0x%x)(0x%x): \te1_outloop", u1, u2);
    em4MapRd (4, 0x013, &u1, &u2);
    printf ("\r\noffset(0x013),\tval(0x%x)(0x%x): \te1_outloop", u1, u2);

    printf ("\r\n");
    return 0;
}

int
em4BpcShow ()
{
    printf ("\r\n--- Back Plane Converter ---");
    printf ("\r\n[page 0]");
    printf ("\r\noffset(0x00),\tval(0x%x): \tmajor &minor version", em4_bpc_rd (0, 0));
    printf ("\r\noffset(0x01),\tval(0x%x): \ttest_reg", em4_bpc_rd (0, 1));
    printf ("\r\noffset(0x03),\tval(0x%x): \tsw_rst", em4_bpc_rd (0, 3));
    printf ("\r\noffset(0x04),\tval(0x%x): \tref8k", em4_bpc_rd (0, 4));
    printf ("\r\n[page 1]");
    printf ("\r\noffset(0x00),\tval(0x%x): \ttest_reg", em4_bpc_rd (1, 0));
    printf ("\r\noffset(0x01),\tval(0x%x): \twpsel", em4_bpc_rd (1, 1));
    printf ("\r\noffset(0x02),\tval(0x%x): \tc1delay", em4_bpc_rd (1, 2));
    printf ("\r\noffset(0x03),\tval(0x%x): \tc1delay", em4_bpc_rd (1, 3));
    printf ("\r\noffset(0x04),\tval(0x%x): \tfrm_up_haps", em4_bpc_rd (1, 4));
    printf ("\r\noffset(0x05),\tval(0x%x): \tfrm_up_haps", em4_bpc_rd (1, 5));
    printf ("\r\noffset(0x06),\tval(0x%x): \tfrm_up_haps", em4_bpc_rd (1, 6));



    printf ("\r\noffset(0x20),\tval(0x%x): \tmgt", em4_bpc_rd (1, 0x20));
    printf ("\r\noffset(0x21),\tval(0x%x): \tmgt", em4_bpc_rd (1, 0x21));
    printf ("\r\noffset(0x22),\tval(0x%x): \tauto_mgt_tx_rst_cnt", em4_bpc_rd (1, 0x22));
    printf ("\r\noffset(0x23),\tval(0x%x): \tfrm_ds_b1_cnt", em4_bpc_rd (1, 0x23));
    printf ("\r\noffset(0x24),\tval(0x%x): \tfrm_ds", em4_bpc_rd (1, 0x24));
    printf ("\r\noffset(0x25),\tval(0x%x): \tdbg_edge/data_sel", em4_bpc_rd (1, 0x25));
    printf ("\r\noffset(0x26),\tval(0x%x): \tfp8k_mon", em4_bpc_rd (1, 0x26));
    printf ("\r\noffset(0x27),\tval(0x%x): \tmgt_tx/rx_center", em4_bpc_rd (1, 0x27));
    printf ("\r\noffset(0x28),\tval(0x%x): \tline c1 realtime mon", em4_bpc_rd (1, 0x28));
    printf ("\r\noffset(0x29),\tval(0x%x): \tlof_cnt", em4_bpc_rd (1, 0x29));
    printf ("\r\n\"Port 1(protection channel) configuration/status ");
    printf ("\r\n0x30, 0x31, 0x32, 0x33,0x34,0x35,0x36,0x37\"");
    printf ("\r\n[page 2]");

    printf ("\r\noffset(0x00),\tval(0x%x): \ttest_reg", em4_bpc_rd (2, 0x0));
    printf ("\r\noffset(0x01),\tval(0x%x): \ttbi_19m_out/inloop", em4_bpc_rd (2, 0x1));
    printf ("\r\noffset(0x02),\tval(0x%x): \tadd_19m_c1delay_row/col", em4_bpc_rd (2, 0x2));
    printf ("\r\noffset(0x03),\tval(0x%x): \ttbi_add_a_c1delay_row/col", em4_bpc_rd (2, 0x3));
    printf ("\r\noffset(0x04),\tval(0x%x): \ttbi_add_b_c1delay_row/col", em4_bpc_rd (2, 0x4));


    printf ("\r\n[page 3]");
    printf ("\r\noffset(0x000),\tval(0x%x): \ttest_reg", em4_bpc_rd (3, 0x000));
    printf ("\r\noffset(0x004),\tval(0x%x): \tepg_alarm_a/b_fall_edge_en", em4_bpc_rd (3, 0x004));
    printf ("\r\noffset(0x005),\tval(0x%x): \tepg_alarm_a/b_even_err_msk", em4_bpc_rd (3, 0x005));
    printf ("\r\noffset(0x007),\tval(0x%x): \tlos_num_a0/1", em4_bpc_rd (3, 0x007));
    printf ("\r\noffset(0x008),\tval(0x%x): \tlos_num_a2/3", em4_bpc_rd (3, 0x008));
    printf ("\r\noffset(0x009),\tval(0x%x): \tlos_num_b0/1", em4_bpc_rd (3, 0x009));
    printf ("\r\noffset(0x00a),\tval(0x%x): \tlos_num_b2/3", em4_bpc_rd (3, 0x00a));
    printf ("\r\n[page 4]");

    printf ("\r\noffset(0x000),\tval(0x%x): \tintr,cpu_dcm_all_rst", em4_bpc_rd (4, 0x000));
    printf ("\r\noffset(0x005),\tval(0x%x): \ttest_reg", em4_bpc_rd (4, 0x005));
    printf ("\r\noffset(0x010),\tval(0x%x): \tint_dcm_rst_cnt_77m", em4_bpc_rd (4, 0x010));
    printf ("\r\noffset(0x011),\tval(0x%x): \tint_dcm_rst_cnt_93m", em4_bpc_rd (4, 0x011));
    printf ("\r\noffset(0x012),\tval(0x%x): \tint_dcm_rst_cnt_19m", em4_bpc_rd (4, 0x012));



    printf ("\r\n");

    return 0;
}

int
cah_em4_initFusion (void)
{




# 8434 "cah_126e1.c"

  
    return 0;
}



int buffer_reset (void)
{
    unsigned short u = 0;
    
    u = em4_bpc_rd (1, 0x003);
    u |= 0x0011;
    em4_bpc_wr (1, 0x003, u);    

    printf ("\r\nwaiting for clear ...");
    taskDelay (1);
    u &= ~0x0010;
    em4_bpc_wr (1, 0x003, u);    
    
    return 0;
}
void  cah_em4_initCpu  (void)
{
# 8468 "cah_126e1.c"

    ((UINT32)(*(volatile UINT32*)( 0xf0000120 )))  = 0x70000401;    
    ((UINT32)(*(volatile UINT32*)( 0xf0000124 )))  = 0xffff0e3e;
    
    ((UINT32)(*(volatile UINT32*)( 0xf0000110 )))  = 0x50000801;  
    ((UINT32)(*(volatile UINT32*)( 0xf0000114  )))  =0xff00014a;  
       
    ((UINT32)(*(volatile UINT32*)( 0xf0000118 )))  = 0x60000801;  
    ((UINT32)(*(volatile UINT32*)( 0xf000011c )))  = 0xff00014a ;      
}

void cah_em4_init_fus (void)
{
  UINT8 u=0;
# 8516 "cah_126e1.c"

    ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x0102 )))  =0x02;
   taskDelay(10);
   ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x0102 )))  =0x06;
   taskDelay(10);
    ((UINT8)(*(volatile UINT8*)( (0x70000000)  + 0x04 )))  = 0x00;

   u= ((UINT8)(*(volatile UINT8*)( (0x70000000)  + 0x02 ))) ;
   u=u&0x02;
   if (u == 0x02)
   {
   	  printf ("\r\ncah_em4_init_fus:Select work CC!");
   }
   else
   {
        printf ("\r\ncah_em4_init_fus:Select protect CC!!");
   }
    
   ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x03 )))  = 0x00;
   ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x04 )))  = 0x00;
   ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x05 )))  = 0x00;
   ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x06  )))  = 0x00;

   
  ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x000b )))  = 0x00;
  ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x000d )))  = 0x00;
  ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x000f )))  = 0x00;
  ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x0011 )))  = 0x00;
  ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x0013 )))  = 0x16;
  ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x0015 )))  =0x16;
 
  
 ((UINT8)(*(volatile UINT8*)( (0x70000000)  + 0x001b )))  =  0x00;
  ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x001c )))  =  0x00;
  ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x001d )))  =  0x03;
  ((UINT8)(*(volatile UINT8*)( (0x70000000)  +0x001e )))  =  0x03; 
 printf ("\r\ncah_em4_init_fus:fusion initializtion is ok!");
    
} 
int cah_em4_initBpc (void)
{
    printf ("\r\n<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
     





    cah_em4_printf ("\r\n%s(): init BackPlane Converter at base(%x)", __FUNCTION__, (0x80000000) );
    cah_em4_printf ("\r\n%s(): The version register(%x)", __FUNCTION__, (em4_bpc_rd (0, 0)));
    cah_em4_printf ("\r\n%s(): The major version is (%x)", __FUNCTION__, (em4_bpc_rd (0, 0) >> 8));
    cah_em4_printf ("\r\n%s(): The minor version is (%x)", __FUNCTION__, (em4_bpc_rd (0, 0) & 0xff));
    cah_em4_printf ("\r\n%s(): The micro version is (%x)", __FUNCTION__, em4_bpc_rd (0, 1));
    bpc_mgt_wps ();

    bpc_dcm_rst_al ();
     
    taskDelay(10);
    
    bpc_dcm_st ();
    printf ("\r\npage(0)off(0x003):\tsoftware reset");
    bpc_rst ();


    buffer_reset ();     
    taskDelay (10);


    cah_em4_printf ("\r\npage(1)off(0x20/0x30):\tinitialize mgt");
    mgt_pwr (1);
     
    taskDelay (10);

    cah_em4_printf ("\r\nscram enable");
    bpc_mgt_descram (0, 1);
    bpc_mgt_descram (1, 1);
    bpc_mgt_up_scram (1);

    cah_em4_printf ("\r\npage(1)off(0x002):\tset bpc rx 8k delay");
    em4_bpc_wr (1, 0x002, 0x87b0);
    
    cah_em4_printf ("\r\nFIFO reset");
     
    taskDelay (50);
    bpc_rst_mgt_fifo ();

    taskDelay (10);
    bpc_mgt_8k_mon_w ();
    bpc_mgt_8k_mon_p ();

    printf ("\r\npage(2)off(1):\tinitialize the TBI");

    em4_bpc_wr (2, 1, 0x0000);
    
    cah_em4_printf ("\r\npage(2)off(3/4):\t set c1j1 a/b");
    set_c1j1_a (0, 2, 0);
    set_c1j1_b (0, 2, 0);
    
    cah_em4_printf ("\r\npage(3)off(0x4):\t  set  epg alarm page ");
    em4_bpc_wr (3, 0x04, 0x0000);
    
    printf ("\r\n126xE1 bpc initialization complete");
    printf ("\r\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");

    return 0;
}




 


PB_ERROR_CODE_T
CAH_EM4_activateEntity (DWORD dwPos, BOOL b_warm)
{




    CAH_EM4_ENTITY_DATA_T *pEntity = 0;
    ENTITY_T entity = 0;

    cah_em4_trace (1 , "\r\n(%s): Enter EM4_activateEntity File=%s; Line=%d  ", __FUNCTION__, "cah_126e1.c", 8639);

    if (!b_warm)
    {
         


         
        printf ("\r\n%s(): cah_em4_initCpu ()", __FUNCTION__);
         
         
         
         
        printf ("\r\n%s(): cah_em4_initChip", __FUNCTION__);
        cah_em4_initBpc();
         
         
         
        printf ("\r\n%s(): BPC initialized @0x80000000", __FUNCTION__);
        
        printf ("\r\n[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[[");
        if (((WORD)0x0)  != cah_em4_initChip (entity))
        {
            printf ("\r\EM4_activateEntity: **Error!  Entity= %d; File=%s; Line=%d  ", entity, "cah_126e1.c", 8662);
    		printf ("\r\n]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]");
            return ((WORD)0x1) ;
        }





    }
    printf ("\r\n]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]]");

    gCahEm4TribVar.bEM4Running = 1 ;
    pEntity->state = eEm4EntAct;
      
     
     
      

    return ((WORD)0x0) ;
}


int
wydact (void)
{
    CAH_EM4_activateEntity (0, 0);
    return 0;
}



PB_ERROR_CODE_T
CAH_EM4_deActEntity (DWORD dwPos)
{




    CAH_EM4_ENTITY_DATA_T *pEntity = ((void *)0) ;
    ENTITY_T entity = 0;

    printf ("\r\nEM4_DEACTENTITY: ENTITY(%d) DEACTIVE", entity);

    if (((void *)0)  == (pEntity = cah_em4_getEntityPtr (entity)))
    {
        printf ("\r\nEM4_DeactEntity:  entity(%d) ptr NULL, File(%s), line(%d)", entity, "cah_126e1.c", 8708);
        return PB_ERR_PARAM;
    }

    if (eEm4EntAct != pEntity->state)
    {
        printf ("\r\nEM4_DeactEntity: entity(%d) status error, file(%s), line(%d)", entity, "cah_126e1.c", 8714);
        return PB_ERR_STATE;
    }

    pEntity->state = eEm4EntInit;
    printf ("\r\nEM4_DeactEntity : entity(%d) intUnconnected", entity);

    return ((WORD)0x0) ;

}



WORD
CAH_EM4_DeInitEntity (DWORD dwPos)
{




	CAH_EM4_ENTITY_DATA_T *p = ((void *)0) ;
    printf ("\r\n%s(): enter function ", __FUNCTION__);

	p = cah_em4_getEntityPtr (0);
    cah_em4_revise (0);

	if (!p)
	{
		printf ("\r\n%s(): pointer gppCahEm4CfgVar[0] NULL ", __FUNCTION__);
		return ((WORD)0x1) ;
	}

	if (p->state)
	{
		printf ("\r\n%s(): card state(%d) error", __FUNCTION__, gppCahEm4CfgVar[0]->state);
		return ((WORD)0x1) ;
	}

   	{ if( p ) { free((void*) p );  p  = ((void *)0) ; }} ;
    gppCahEm4CfgVar[0] = ((void *)0) ;

    return ((WORD)0x0) ;

}





void
CAH_EM4_setTransFunc (CahEm4pTransFunc pfunc)
{



    g_pTransFunc = pfunc;


    return;
}



WORD
CAH_EM4_GetSlip (DWORD dwPos, BYTE byPortNo, DWORD * pdwPSlip, DWORD * pdwNSlip)
{
    cah_em4_GetSlip (dwPos, SDD_126xE1IOPortConvert(byPortNo), pdwPSlip, pdwNSlip);
    return 0;
}

WORD
CAH_EM4_getLatchStatus (DWORD dwPos)
{




    CAH_EM4_ENTITY_LATCH_T ret = EM4_ENTITY_UNKNOWN;
    void *baseaddr = ((void *)0) ;
    unsigned short status = 0;
    CAH_EM4_ENTITY_DATA_T *pEntity;
    ENTITY_T entity = 0;

    baseaddr = cah_em4_getEntityBase (entity, 100);
    pEntity = cah_em4_getEntityPtr (entity);
    status = cah_em4_rd (baseaddr, ((0x00 << 10) | 0x05) );
    if (status & 0x01)
    {
        ret = EM4_ENTITY_DELATCH;
    } else {
        ret = EM4_ENTITY_LATCH;
    }
    return ret;

}


WORD
CAH_EM4_GetTuAlmState (DWORD dwPos, UINT8 byPortNo, CTD_TU_ALARM_T * psTuAlarm)
{

     int ulPort = 0;
	unsigned short uu = 0, u = 0;
	void * base = 0;
	int regcnt = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));
      ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;

	 
     cah_em4_wr (base, ((0x01 << 10) | 0x03) , ulPort | 0x8000);
     

    regcnt = 0;
    while (1)
    {
        uu = cah_em4_rd (base, ((0x01 << 10) | 0x03) );
        if (!(0x8000 & uu))
        {
            break;
        }
        else
        {
            regcnt++;
            if (regcnt > 100)
            {
				printf ("\r\n%s(): fpga read error ", __FUNCTION__);
                break;
            }
        }
    }

    u = cah_em4_rd (base, ((0x01 << 10) | 0x10) ) & 0x0003;
    psTuAlarm->ais = (0x0001 == u) ? 1 : 0;
    psTuAlarm->lop = (0x0002 == u) ? 1 : 0;

    return ((WORD)0x0) ;
}



WORD
CAH_EM4_GetLpAlmState (DWORD dwPos, UINT8 byPortNo, CTD_LP_ALARM_T * psLpAlarm)
{
     int ulPort = 0; 
	void * base = 0;
	unsigned short u = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
	base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));
       ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
	 u = cah_em4_pohRead (base, ulPort, 0x03 );
	

    psLpAlarm->tim 	= ((u & 0x0001) ? 1 : 0);
    psLpAlarm->plm 	= ((u & 0x0004) ? 1 : 0);
    psLpAlarm->uneq = ((u & 0x0008) ? 1 : 0);
    psLpAlarm->rdi 	= ((u & 0x0010) ? 1 : 0);
    psLpAlarm->rfi 	= ((u & 0x0080) ? 1 : 0);
    
	return ((WORD)0x0) ;
}

int g_mon = 0;
int g_monport = 0;

int cah_em4_readPortSchemeLos (int groupport , int group)
{
    int phyport = 0;
    UINT16 reg = 0;
    int los = 0;

    if (0 > groupport || 71 < groupport)
    {
        printf ("(%s): port number(%d) overflow", "cah_126e1.c", groupport);
        return -1;
    }

    phyport = g_phyportmap [groupport + 1] - 1;
    reg = em4_bpc_rd (3, (phyport/16)+((!group) ? ((0x021) ):0x26));
    if (g_mon && g_monport == groupport)
    {
            printf ("group(%d)phyport(%2d)reg(%4x)off(%4x)\r\n", group, phyport, reg,   (phyport/16)+((!group) ? ((0x021) ):0x26) );
    }
    los = (reg & (0x0001 << ((phyport) % 16))) ? 1 : 0;

    return los;
}


WORD
cah_em4_getPhyLos (DWORD dwPos, UINT8 byPortNo, CTD_PPI_ALARM_T * psPpiAlarm)
{
    ENTITY_T entity = 0;
    int ulPort = 0;
    CAH_EM4_ENTITY_DATA_T *pEntity;
	void * base = 0;
    int schemePort = 0;

	{if (125 <  byPortNo ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  byPortNo ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;

    pEntity = cah_em4_getEntityPtr (entity);
    ulPort = cah_em4_mapPort (byPortNo) - 1;
    base = cah_em4_getEntityBase (0, byPortNo);

    schemePort = g_phyportmap [ulPort + 1] - 1;
    
     psPpiAlarm->los = cah_em4_readPortSchemeLos (ulPort, byPortNo / 63);
     

# 8963 "cah_126e1.c"


    return psPpiAlarm->los;
}

 


static int
isIOAbsent(int port, unsigned char *pIO)
{
    int absent = 0;
    unsigned short val = 0;
    int io = 0;

    io = 0;

    if (port >= 0 && port <=20){
        val = em4Map1Rd (0, 0x12);
        absent = (val & 0x02) ? 1 : 0;
        *pIO = 5;    
    } else if (21 <= port && port <= 41){
        val = em4Map1Rd (0, 0x12);
        absent = (val & 0x04) ? 1 : 0;
        *pIO = 3; 
    } else if (42 <= port && port <= 62){
        val = em4Map1Rd (0, 0x12);
        absent = (val & 0x08) ? 1 : 0;
        *pIO = 1; 
    } else if ((0 + 63) <= port && port <= (20 + 63)){
        val = em4Map2Rd (0, 0x12);
        absent = (val & 0x02) ? 1 : 0;
        *pIO = 6; 
    } else if ((21 + 63) <= port && port <= (41 + 63)) {
        val = em4Map2Rd (0, 0x12);
        absent = (val & 0x04) ? 1 : 0;
        *pIO = 4; 
    } else if ((42 + 63) <= port && port <= (62 + 63)) {
        val = em4Map2Rd (0, 0x12);
        absent = (val & 0x08) ? 1 : 0;
        *pIO = 2; 
    }
   
    return (absent);
}




 


int clr_payload_ais (int port)
{
	void * base = 0;
    unsigned short val = 0;
    int byte = 0, bit = 0;

    byte = port / 16;
    bit = port % 16;
    
    val = cah_em4_rd (base, ((((0x00 << 10) | 0x20) ) + byte));
    
    if (val & (0x01 << bit)) 
    {
         


        val &= ~(0x01 << bit);
        cah_em4_wr (base, ((((0x00 << 10) | 0x20) ) + byte), val);
    } else {
    }

    return 0;

}


 


int set_payload_ais (int port)
{
	void * base = 0;
    unsigned short val = 0;
    int byte = 0, bit = 0;

    byte = port / 16;
    bit = port % 16;
    
    val = cah_em4_rd (base, ((((0x00 << 10) | 0x20) ) + byte));
    
    if (val & (0x01 << bit)) 
    {
    } else {
        val |= (0x01 << bit);
        cah_em4_wr (base, ((((0x00 << 10) | 0x20) ) + byte), val);
    }

    return 0;
}


int set_ioN_ais (int io)
{
    int firstport = 0, lastport = 0;
    int i = 0;

    sdd_get_ioN_port (io, &firstport, &lastport);

    for (i = firstport; i <= lastport; i ++);
    {
        set_payload_ais (i);
    }
    
    return 0;
}





int clear_IO_payload_si (io)
{
    int firstport = 0, lastport = 0;
    int i = 0;

    sdd_get_ioN_port (io, &firstport, &lastport);

    for (i = firstport; i <= lastport; i ++);
    {
        clr_payload_ais (i);
    }
    
    return 0;
}




int g_ioabsent = 0;





WORD
CAH_EM4_GetPpiAlmState (DWORD dwPos, UINT8 byPortNo, CTD_PPI_ALARM_T * psPpiAlarm)
{
    ENTITY_T entity = 0;
    int ulPort = 0;
    CAH_EM4_ENTITY_DATA_T *pEntity;
	unsigned short u = 0;
	void * base = 0;
    int isloop = 0;
    CTD_PPI_ALARM_T alm;
    unsigned short val = 0;
    unsigned char io = 0;

    val = 0;

	{if (125 <  SDD_126xE1IOPortConvert(byPortNo) ) { printf ("\r\n%s(): port(%d) overflow", __FUNCTION__,  SDD_126xE1IOPortConvert(byPortNo) ); printf ("\r\n\r\n\r\n"); return ((WORD)0x1) ; } } ;
    memset (&alm, 0, sizeof (CTD_PPI_ALARM_T));

    pEntity = cah_em4_getEntityPtr (entity);
    ulPort = cah_em4_mapPort (SDD_126xE1IOPortConvert(byPortNo)) - 1;
	base = cah_em4_getEntityBase (0, SDD_126xE1IOPortConvert(byPortNo));
    if (byPortNo == g_ulport)
    {
        printf ("\r\nCAH_EM4_GetPpiAlmState: ulport(%d)", ulPort); 
    }
    u = cah_em4_readPdhAlarm (base, ulPort);
    
    isloop = cah_em4_gete1outloop (base, ulPort);

    psPpiAlarm->lom = (u & 0x0002) ? 1 : 0;
    psPpiAlarm->ais = (u & 0x0008) ? 1 : 0;  
    psPpiAlarm->rdi = (u & 0x0001) ? 1 : 0;
    psPpiAlarm->lof = (u & 0x0004) ? 1 : 0;

     


    psPpiAlarm->los = cah_em4_getPhyLos (dwPos, SDD_126xE1IOPortConvert(byPortNo), &alm);

     


    if (isloop) psPpiAlarm->los = 0;



     




    if (isIOAbsent(SDD_126xE1IOPortConvert(byPortNo), &io))
    {
        if ((SDD_126xE1IOPortConvert(byPortNo) == 13 || SDD_126xE1IOPortConvert(byPortNo) == 120) && g_ioabsent)
        {
            printf ("\r\nport(%d)'s IO is absent , skip it", SDD_126xE1IOPortConvert(byPortNo));
        }
        psPpiAlarm->los = 1;
         



        clear_IO_payload_si (io);
# 9184 "cah_126e1.c"

    } else {
        if (psPpiAlarm->los)
        {
            cah_em4_ManualInsertPayloadAIS (SDD_126xE1IOPortConvert(byPortNo), 1 );
        }
        else
        {
            cah_em4_ManualInsertPayloadAIS (SDD_126xE1IOPortConvert(byPortNo), 0 );
        }
    }

     
    return ((WORD)0x0) ;
}

WORD
CAH_EM4_GetFpgaVersion (DWORD dwPos, char *pucFpgaVer)
{
	unsigned short version = 0, v1 = 0;

	version = em4Map1Rd (0, 0);
    v1 = em4_bpc_rd (0, 0);
	 
    sprintf (pucFpgaVer, "V%d.%d, V%d.%d", 
        (version & 0xff00) >> 8, (version & 0xff),
        (v1 && 0xff00) >> 8, (v1 && 0x00ff));
    return (((WORD)0x0) );
}


WORD
CAH_EM4_GetTxLpTti (DWORD dwPos, BYTE byPortNo, TTI_LEN_T eTtiMode, BYTE * pbyTtiValue)
{
    return ((WORD)0x0) ;
}




int
pbFpgaIsrHookDel (int handle)
{
    return 0 ;
}

int
pbFpgaIsrEnable (int chip)
{
    return 0 ;
}

int
pbFpgaIntrDisable (int handle)
{
    return 0 ;
}

 





































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































 








