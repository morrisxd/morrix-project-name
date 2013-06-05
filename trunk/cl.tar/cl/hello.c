#define HI

int * page = (int *)0;

typedef unsigned short int USI;
typedef unsigned short int ushort_t;

USI myusi;

int f1 (int USI, char * name, char ch, float f)
{
	return 0;
}



typedef int pencil;
pencil * ppppppp;


typedef int *pencil2;

ushort_t *pm_us;
ushort_t us1;

typedef int (*FUNCPTR) (int);	    
typedef int (*FUNCPTR2) (int age);	    
ushort_t us2;

typedef unsigned char UINT8;
UINT8 u8 = 0;
UINT8 *u9;

struct mystruct {
	int i;
	float f;
};

typedef struct _p {
	int i;
	float f;
}myp;

myp (*findProc) (struct mystruct * tablePtr, const char * key);


typedef myp* mypp;
mypp ppp;

enum eName{
	e_TOM,
	e_JACK,
	e_SUSAN
};

int g_array[100];

int foo (int id)
{
	int local[100];
	int arra2[3][44];
	int i = 0;
	int j = 0;
	int k = 3;
	int l,m,n;

	if (i < 3 && j > 6) 
	{
		{
			if (i < j + i * j)
			{
			}
		}

		{
			printf ("sayhi");
		}

		{
			{
				i = i<3?i-4:j;
			}
		}

		return 0;
	}

	printf ("helloworld");
	localreference[9] = 7;
	i = local[66];
	i = local[j];
	j = 77;
	local[j] = 66;
	local[i] = 66;
	local[i - 1 - j] = 66;
	arra2[i][j] = 66;
	local[id] = 0x99;
	local[id - 1 - l] = 0x99;
	i = local[id];

	return 3;
}

int g_sigh = 0;


char * bar (float f)
{
	int i = 0;
	static char * p = "hellowrold";

	foo (3);
	return p;
}

int block_test (int n)
{
	{
		int i = 0;

		i = 3;
	}
	return 0;
}

extern int funcAAA (int n);

int funA (int age, char * name)
{
	int n = 0;

	if (age > 6)
	{
		printf ("ok");
	} else {
		printf ("old");
	}

	if (age > 6) printf ("6");
	else if (age > 60) printf ("66");
	else printf ("sigh");

	g_array [age] = 0;
	g_array [n] = 0;

	return 0;
}

