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

