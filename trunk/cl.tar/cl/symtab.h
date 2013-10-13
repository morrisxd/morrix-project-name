/****************************************************/
/* File: symtab.h                                   */
/* Symbol table interface for the TINY compiler     */
/* (allows only one symbol table)                   */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "stdio.h"

/* Procedure st_insert inserts line numbers and
 * memory locations into the symbol table
 * loc = memory location is inserted only the
 * first time, otherwise ignored
 */
extern void st_insert_id( char * name, int lineno, int loc );
extern void st_insert_typedef(char *name, int lineno, int loc);

/* Function st_lookup returns the memory 
 * location of a variable or -1 if not found
 */

/* Procedure printSymTab prints a formatted 
 * listing of the symbol table contents 
 * to the listing file
 */
extern void printSymTab(FILE * listing);
extern void printTypedefTab(FILE * listing);
extern void release_table (void);
extern void release_typedef_table (void);
extern void print_st (void);

typedef struct LineListRec {
	int             lineno;
	struct LineListRec *next;
}              *LineList;
typedef struct BucketListRec {
	char            name[126];
	LineList        lines;
	int             memloc;	/* memory location for variable */
	struct BucketListRec *next;
	char 		func[126];	/* domain */
	int 		dirty;
}              *BucketList;

extern BucketList st_lookup ( char * name );
extern BucketList st_lookup_typedef(char *name);

extern int in_enum;

#endif
