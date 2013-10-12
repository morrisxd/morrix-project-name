/****************************************************/
/* File: symtab.c                                   */
/* Symbol table implementation for the TINY compiler */
/* (allows only one symbol table)                   */
/* Symbol table is implemented as a chained         */
/* hash table                                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

/* SIZE is the size of the hash table */
#define SIZE 211

/*
 * SHIFT is the power of two used as multiplier in hash function
 */
#define SHIFT 4

/* the hash function */
static int 
hash(char *key)
{
	int             temp = 0;
	int             i = 0;
	while (key[i] != '\0') {
		temp = ((temp << SHIFT) + key[i]) % SIZE;
		++i;
	}
	return temp;
}

#if 0
/*
 * the list of line numbers of the source code in which a variable is
 * referenced
 */
typedef struct LineListRec {
	int             lineno;
	struct LineListRec *next;
}              *LineList;

/*
 * The record in the bucket lists for each variable, including name, assigned
 * memory location, and the list of line numbers in which it appears in the
 * source code
 */
typedef struct BucketListRec {
	char            name[126];
	LineList        lines;
	int             memloc;	/* memory location for variable */
	struct BucketListRec *next;
	char 		func[126];	/* domain */
	int 		dirty;
}              *BucketList;
#endif

char saved_func_name [1024];

/* the hash table */
static BucketList hashTable[SIZE];
static BucketList typedefNameTable[SIZE];

/*
 * Procedure st_insert inserts line numbers and memory locations into the
 * symbol table loc = memory location is inserted only the first time,
 * otherwise ignored
 */
void 
st_insert_id (char *name, int lineno, int loc)
{
	int             h = hash(name);
	BucketList      l = hashTable[h];
	while (
		(l != NULL) 
		&& (
			(strcmp(name, l->name) != 0) || 
			(strcmp(saved_func_name, l->func) != 0)
		   )
	      )
	{
		l = l->next;
	}
	if (l == NULL) {	/* variable not yet in table */
		l = (BucketList) malloc(sizeof(struct BucketListRec));
		if (!l) {
			printf ("st_insert: malloc failed");
		}
#if (0) 
		l->name = name;
#else
		sprintf (l->name, "%s", name);
#endif
		l->lines = (LineList) malloc(sizeof(struct LineListRec));
		if (!(l->lines)) {
			printf ("st_insert: malloc failed, l->lines");
		}
		l->lines->lineno = lineno;
		l->memloc = loc;
		l->lines->next = NULL;
		l->dirty = 1;
		sprintf (l->func, "%s", saved_func_name);
		l->next = hashTable[h];
		hashTable[h] = l;
	} else {		/* found in table, so just add line number */
		LineList        t = l->lines;
		while (t->next != NULL)
			t = t->next;
		t->next = (LineList) malloc(sizeof(struct LineListRec));
		if (!(t->next)) {
			printf ("st_insert: malloc failed, t->next");
		}
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
}				/* st_insert */


void 
st_insert_typedef (char *name, int lineno, int loc)
{
	int             h = hash(name);
	BucketList      l = typedefNameTable[h];
	while (
		(l != NULL) 
		&& (
			(strcmp(name, l->name) != 0) || 
			(strcmp(saved_func_name, l->func) != 0)
		   )
	      )
	{
		l = l->next;
	}
	if (l == NULL) {	/* variable not yet in table */
		l = (BucketList) malloc(sizeof(struct BucketListRec));
		if (!(l)) {
			printf ("st_insert_typedef: malloc failed, l");
		}
#if (0) 
		l->name = name;
#else
		sprintf (l->name, "%s", name);
#endif
		l->lines = (LineList) malloc(sizeof(struct LineListRec));
		if (!(l->lines)) {
			printf ("st_insert_typedef: malloc failed, l->lines");
		}
		l->lines->lineno = lineno;
		l->memloc = loc;
		l->lines->next = NULL;
		l->dirty = 1;
		sprintf (l->func, "%s", saved_func_name);
		l->next = typedefNameTable[h];
		typedefNameTable[h] = l;
	} else {		/* found in table, so just add line number */
		LineList        t = l->lines;
		while (t->next != NULL)
			t = t->next;
		t->next = (LineList) malloc(sizeof(struct LineListRec));
		if (!(l->next)) {
			printf ("st_insert_typedef: malloc failed, l->next");
		}
		t->next->lineno = lineno;
		t->next->next = NULL;
	}
}				/* st_insert */



/*
 * Function st_lookup returns the memory location of a variable or -1 if not
 * found
 */
#if 0
int 
#else
BucketList
#endif
st_lookup(char *name)
{
	int             h = hash(name);
	BucketList      l = hashTable[h];
	
	while ((l != NULL) 
		&& ((strcmp(name, l->name) != 0) || 
			((strcmp(saved_func_name, l->func) != 0))))
	{
		l = l->next;
	}
	if (l == NULL)
#if 0
		return -1;
#else
		return NULL;
#endif
	else
#if 0
		return l->memloc;
#else
		return l;
#endif
}


BucketList
st_lookup_typedef(char *name)
{
	int             h = hash(name);
	BucketList      l = typedefNameTable[h];
	
	while ((l != NULL) 
		&& ((strcmp(name, l->name) != 0))) 
	{
		l = l->next;
	}
	if (l == NULL)
		return NULL;
	else
		return l;
}




/*
 * Procedure printSymTab prints a formatted listing of the symbol table
 * contents to the listing file
 */
void 
printSymTab(FILE * listing)
{
	int             i;
	fprintf(listing, "Variable Name  Location  Domain         Line Numbers\n");
	fprintf(listing, "-------------- --------  -------------- ------------\n");
	for (i = 0; i < SIZE; ++i) {
		if (hashTable[i] != NULL) {
			BucketList      l = hashTable[i];
			while (l != NULL) {
				LineList        t = l->lines;
				fprintf(listing, "%+14s ", l->name);
				fprintf(listing, "%+8d  ", l->memloc);
				fprintf(listing, "%+14s ", l->func);
				while (t != NULL) {
					fprintf(listing, "%+12d ", t->lineno);
					t = t->next;
				}
				fprintf(listing, "\n");
				l = l->next;
			}
		}
	}
}				/* printSymTab */



void 
printTypedefTab(FILE * listing)
{
	int             i;
	fprintf(listing, "Typedef  Name  Location  Domain         Line Numbers\n");
	fprintf(listing, "-------------- --------  -------------- ------------\n");
	for (i = 0; i < SIZE; ++i) {
		if (typedefNameTable[i] != NULL) {
			BucketList      l = typedefNameTable[i];
			while (l != NULL) {
				LineList        t = l->lines;
				fprintf(listing, "%+14s ", l->name);
				fprintf(listing, "%+8d  ", l->memloc);
				fprintf(listing, "%+14s ", l->func);
				while (t != NULL) {
					fprintf(listing, "%+12d ", t->lineno);
					t = t->next;
				}
				fprintf(listing, "\n");
				l = l->next;
			}
		}
	}
}				/* printSymTab */

void 
release_table (void)
{
	int i = 0;
	BucketList b1 = NULL, b2 = NULL;
	LineList l1 = NULL, l2 = NULL;

	for (i = 0; i < SIZE; i ++) {
		b1 = hashTable[i];
		while (b1 != NULL) {
			if (NULL != b1->next)
			{
				b2 = b1->next;
			} else {
				b2 = NULL;
			}
#if 1
			/* 
			 * free lines first 
			 */
			l1 = b1->lines;
			while (1)
			{
				if (NULL == l1) {
					goto freebucket;
				}
				if (NULL != l1->next)
				{
					l2 = l1->next;
				} else {
					l2 = NULL;
				}
				free (l1);
				l1 = l2;
			}
#endif
freebucket:
			free (b1);
			b1 = b2;
		}
	}
	return ;
}


void 
release_typedef_table (void)
{
	int i = 0;
	BucketList b1 = NULL, b2 = NULL;
	LineList l1 = NULL, l2 = NULL;

	for (i = 0; i < SIZE; i ++) {
		b1 = typedefNameTable[i];
		while (b1 != NULL) {
			if (NULL != b1->next)
			{
				b2 = b1->next;
			} else {
				b2 = NULL;
			}
#if 1
			/* 
			 * free lines first 
			 */
			l1 = b1->lines;
			while (1)
			{
				if (NULL == l1) {
					goto freebucket;
				}
				if (NULL != l1->next)
				{
					l2 = l1->next;
				} else {
					l2 = NULL;
				}
				free (l1);
				l1 = l2;
			}
#endif
freebucket:
			free (b1);
			b1 = b2;
		}
	}
	return ;
}




void print_st (void)
{
	FILE * fp;
	fp = fopen ("st.sym", "w+");
	printSymTab (fp);
	fprintf (fp, "\n");
	printTypedefTab (fp);
	fclose (fp);
}
