#ifndef __DEFS_H__
#define __DEFS_H__



#define PBCVSDENY	"pb.deny"
#define PBCVSALLOW	"pb.allow"
#define PBCVSCHECK	"pb.check"
#define CVSROOTADM_ALLOW PBCVSALLOW
#define CVSROOTADM_CHECK PBCVSCHECK     /* in use now */

#define TEMPMSGKEY 	9990
#define TEMPSEMKEY	8880
#define TEMPSHMKEY	7770

#define MAXMSGLEN	100

#if 1
typedef struct CVSMSG
{
        int len;
        long type;
        char text[512];
}
CVSMSG;
#endif

#define RAWID	1L
#define SERVERIP	"192.168.11.74"
#define SERVERPORT 	80
#define READTIMEOUT	0
#define WRITETIMEOUT	0

#define __CVSMR_DEBUG__	0
#define TMPBUFLEN	2048

/* extern int PB_MR_CHECK (void *); */
extern int pb_mrchk (void *);

#if 0
#define CHECK_CALLER(caller) \
	{ \
		FILE *fp=NULL; \
		char pbfname[TMPBUFLEN]; \
		int namelen = 0; \
		size_t linebuf_len = 0; \
		char *linebuf, username[TMPBUFLEN]; \
		strcpy (username, caller); \
		sprintf (pbfname, "%s/%s/%s", current_parsed_root->original,\
			CVSROOTADM, CVSROOTADM_CHECK); \
	    	fp = CVS_FOPEN (pbfname, "r"); \
	    	if (fp == NULL) \
    		{ \
			if (!existence_error (errno)){ \
	    			error (1, 0, "cannot open %s", pbfname); \
				fp = NULL; \
			} \
    		}else{  \
    /* Look for a relevant line -- one with this user's name. */ \
    namelen = strlen (username); \
    while (getline (&linebuf, &linebuf_len, fp) >= 0) \
    { \
	if ((strncmp (linebuf, username, namelen) == 0) \
	    && (linebuf[namelen] == ':')) \
        { \
	    found_it = 1; \
	    break; \
        } \
    } \
			ACTION ("OPEN CONFIGURATION FILE", "OK");  \
			fclose(fp); \
		} \
	}


#define PB_MR_CHECK(debug)	\
    /*	\
     * added by ikoo for testing -m{-i} optarg 	\
     * fix me: something to do with cvstmp handling is not perfect.	\
     * \
     */\
    {	\
	int err=0;\
	int found_it = 0; \
        unsigned long loglen = 0; \
        char cvstmp[TMPBUFLEN]; \
	CHECK_CALLER(getcaller()) \
	if(0 == found_it) goto skip_check; \
	sprintf(cvstmp, "[PB]Error MRID or User Name. Please check" \
	    " your MR or User Name in CVS"); \
    	if (saved_message[0] != '-') \
    	{ \
	    error (1, 0, "[PB]Need mrid. Usage:{-i\"{mrid}\"......]}."); \
	    return -2; \
    	} \
    	if (saved_message[1]!= 'i') \
    	{ \
	    error (1, 0, "[PB]Need mrid. Usage:{-i\"{mrid}\"......]}."); \
	    return -2; \
    	} \
    	loglen = strlen (saved_message); \
    	if (2 > checkquote (saved_message)) \
    	{ \
	    /* error (1, 0, cvstmp); */ \
	    error (1, 0, "[PB]Need mrid. Usage:{-i\"{mrid}\"......]}."); \
	    return -3; \
    	} \
        getmrid (saved_message, mrid); \
        sprintf(cvstmp, "[PB]Error MRID[%s] or User Name. Please check" \
	    " your MR or User Name in CVS", mrid); \
	err = querymrid (getcaller(), mrid); \
    	if(1 == err) \
    	{ \
	    /* sprintf(cvstmp, "MRID[%s] NOT valid!.", mrid); */ \
	    sprintf(cvstmp, "Error MR[%s] Status. Check if the input" \
		" MR is in assigned or deferred status.", mrid); \
	    error (1, 0, cvstmp); \
    	}else if (0 == err){ \
	    sprintf(cvstmp, "MRID[%s] valid.", mrid); \
	    error (0, 0, cvstmp); \
    	}else{ /* err < 0 means net error */ \
		error (1, 0, "!!!Communication error with MR webserver!!!"); \
	} \
skip_check: \
    }                           /* end of added by ikoo */


#endif
#endif
