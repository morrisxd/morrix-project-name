#ifndef _STDLIB_
#define _STDLIB_

#include <winmon.h>

#ifndef _SIZE_T_
#define _SIZE_T_        __typeof(sizeof(int))
typedef _SIZE_T_ size_t;
#endif

#ifndef NULL
#define NULL    0
#endif

typedef struct {
        int     quot;
        int     rem;
} div_t;

typedef struct {
        long    quot;
        long    rem;
} ldiv_t;

long strtol(const char *str, char **endptr, int base);
unsigned long strtoul(const char *str, char **endptr, int base);

long atol(const char *str);
int atoi(const char *str);

char *lltostr(long longvalue, char *endptr);
char *ulltostr(unsigned long longvalue, char *endptr);

void qsort( char *base, int  n, int  size, int  (*compar)(void *,void *) );
void winmon_exit(int);

void *bsearch(const void *key, const void *base0, size_t nmemb,
     size_t size, int (*compar)(const void *,const void *));

int rand(void);
void srand(unsigned int);

extern int abs(int);
extern div_t div(int, int);
extern long int labs(long);
extern ldiv_t ldiv(long, long);

#define exit(rc)	WinMon_exit(rc)
#define wait(t)		WinMon_wait(t)
#define clock()		WinMon_clock()
#define getenv(name)    WinMon_getenv(name)
#define system(c)       WinMon_system(c)

#endif /* _STDLIB_ */


