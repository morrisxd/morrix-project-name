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

int p = 9;

typedef struct mystruct2 {
   int i;
   int (myfunct3)(int flag, float salary);
} mystruct2;

mystruct2 mys;

