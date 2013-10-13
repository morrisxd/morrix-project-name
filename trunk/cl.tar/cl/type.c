typedef unsigned int uint;

typedef (*fptr)(int i, float f);

typedef struct { int i; } mystruct;

uint myint = 3;

int myfunc (int n, char * pName)
{
	int i = 4;

	i = 3;

	return i;
}

void myfunc2 (int i, void (*p)(int age, char *sex));
void (myfunc4)(int i, float s);
typedef int (*myfunc6)(int i, float fnum);
typedef int (myfunc5)(int i, float fnum2);

int p = 9;

typedef struct mystruct2 {
   int i;
   int (myfunct3)(int flag, float salary);
} mystruct2;

mystruct2 mys;

typedef int (myfunc7)(int i, float fnum2);
