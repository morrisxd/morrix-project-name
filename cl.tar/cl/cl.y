/*
ANSI C Yacc grammar
In 1985, Jeff Lee published his Yacc grammar (which is accompanied by a matching Lex specification) for the April 30, 1985 draft version of the ANSI C standard.  Tom Stockfisch reposted it to net.sources in 1987; that original, as mentioned in the answer to question 17.25 of the comp.lang.c FAQ, can be ftp'ed from ftp.uu.net, file usenet/net.sources/ansi.c.grammar.Z. 
Jutta Degener, 1995 
*/

/*
   - Now the problem is:
      "
      typedef struct this_is_a_struct this_is_a_struct;
      struct this_is_a_struct {int i; float f;};
      "
      in the above case, 'this_is_a_struct' is both a type and a name, 
      so it is confused.
   - we can focus on 'endoftype' now. this is the correct 
      location & value. Success !
      may/30/2013
   - something wrong if typedef (func)(para list)
 */

%{
#include "globals.h"
#include "symtab.h"

extern int column;
#define YYDEBUG 1
/*
 * control for print debug info on console
 */
#ifdef PRINT_RELATION
#undef PRINT_RELATION
#endif

#ifdef PRINT_IDENTIFIER
#undef PRINT_IDENTIFIER
#endif

#define PRINT_INIT_DECLARATOR
#ifdef PRINT_INIT_DECLARATOR
#undef PRINTF_INIT_DECLARATOR
#endif


/*
 * These macro has no use indeed, the code was written firstly not so good,
 * and then was improved later. So I use the macro(s) to disable the old
 * code and use new code.
 */
#define SET_CLEAN_IN_ID_RECOGNIZE
#define CHECK_INDEX_IN_ID_RECOGNIZE

int tmp = 0;

int functypedef = 0;
int pre_s_u = 0;
int after_type_specifiers = 0;
int in_namelist = 0;
int in_struct_or_union = 0;
int in_para_list = 0;
int block_level = 0;
int in_typedef = 0;
int external_func_decl = 0;

// #define YYSTYPE treenode *

/*
 * save the identifier temperoraily for instance usage, 
 * and will be discarded soon. 
 */
char g_cur_sym [MAXSYMLEN];
char identifier [MAXSYMLEN];
char saved_identifier [MAXSYMLEN];

/* 
 * used for depth record for function block '{ ... }'
 */
char g_namespace [MAXSYMLEN];

/*
 * function name which has been saved when SYMBOL was recognized
 */
char saved_func_name [MAXSYMLEN]; 

/*
 * check if the array index range has been checked for overflow safe
 */
int check_dirty (int index);
int show_pos (const char * tag);
int type_specifiers (int n);

/*
 * Used when analyzing array []
 */
char saved_array_name [MAXSYMLEN];
char saved_array_index_name [MAXSYMLEN];

/*
 * saved symbols when recognized in 'if (i < j)' like expression
 */
char saved_left_sym [MAXSYMLEN];
char saved_right_sym [MAXSYMLEN];

/*
 * Used when handling 'if(...)', 'int i, ...', and 'foo (int n)', etc ...
 */
int in_cond_chk = 0;
int in_postfix_expression = 0;
int enter_func = 0;

/*
 * Used when found 'if (...)' structure 
 */
int set_clean (char * saved_id);

int save_array (char * arrayname);

/*
 * When checking 'name[i] = ...' struct for 'array index out of range' error.
 */
int check_array_index (char * indexname);

/*
 * for 'if (i < 3)' look-like structure
 */
int save_left_sym (char * left);
int save_right_sym (char * right);

/*
 * for compound-statement '{}' look-like  structure .
 */
int enter_block (void);
int leave_block (void);

/*
 * for data type analyze
 */
variable in_var;

%}



%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token INLINE INLINE2

%start translation_unit

%nonassoc WITHOUT_STRUCT_CONTENT
%nonassoc '{' 
%%

primary_expression
	: IDENTIFIER 
		{ 
			sprintf (identifier, "%s", g_cur_sym); 
			if (1 == in_cond_chk) {
#ifdef PRINT_IDENTIFIER
				printf ("#id(%s)#", identifier);
#endif
#ifdef SET_CLEAN_IN_ID_RECOGNIZE
				set_clean (identifier);
#endif
			}

			if (1 == in_postfix_expression) {
#ifdef PRINT_IDENTIFIER
				printf ("#postid(%s)", identifier);
#endif

#ifdef CHECK_INDEX_IN_ID_RECOGNIZE
				check_array_index (g_cur_sym); 
#endif
			}
		}
	| CONSTANT
	| STRING_LITERAL
	| '(' expression ')'
	;

postfix_expression
	: primary_expression
	| postfix_expression { save_array (g_cur_sym); } 
		'[' {in_postfix_expression = 1;} expression 
			{ 
#ifndef CHECK_INDEX_IN_ID_RECOGNIZE
				check_array_index (g_cur_sym); 
#endif
			}
		']' {in_postfix_expression = 0;}
	| postfix_expression '(' ')'
	| postfix_expression '(' argument_expression_list ')'
	| postfix_expression '.' IDENTIFIER
	| postfix_expression PTR_OP IDENTIFIER
	| postfix_expression INC_OP
	| postfix_expression DEC_OP
	;

argument_expression_list
	: assignment_expression
	| argument_expression_list ',' assignment_expression
	;

unary_expression
	: postfix_expression
	| INC_OP unary_expression
	| DEC_OP unary_expression
	| unary_operator cast_expression
	| SIZEOF unary_expression
	| SIZEOF '(' type_name ')'
	;

unary_operator
	: '&'
	| '*'
	| '+'
	| '-'
	| '~'
	| '!'
	;

cast_expression
	: unary_expression
	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression
	| multiplicative_expression '*' cast_expression
	| multiplicative_expression '/' cast_expression
	| multiplicative_expression '%' cast_expression
	;

additive_expression
	: multiplicative_expression
	| additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
	;

shift_expression
	: additive_expression
	| shift_expression LEFT_OP additive_expression
	| shift_expression RIGHT_OP additive_expression
	;

relational_expression
	: shift_expression
	| relational_expression 
		{ 
			save_left_sym (g_cur_sym);
#ifdef PRINT_RELATION
			printf ("relation(%s)", g_cur_sym);
#endif
		} 
	'<' shift_expression 
		{ 
			save_right_sym (g_cur_sym); 
#ifdef PRINT_RELATION
			printf ("relation(%s)", g_cur_sym);
#endif
		}
	| relational_expression 
		{ 
			save_left_sym (g_cur_sym);
#ifdef PRINT_RELATION
			printf ("relation(%s)", g_cur_sym);
#endif
		} 
	'>' shift_expression
		{ 
			save_right_sym (g_cur_sym); 
#ifdef PRINT_RELATION
			printf ("relation(%s)", g_cur_sym);
#endif
		}
	| relational_expression LE_OP shift_expression
	| relational_expression GE_OP shift_expression
	;

equality_expression
	: relational_expression
	| equality_expression EQ_OP relational_expression
	| equality_expression NE_OP relational_expression
	;

and_expression
	: equality_expression
	| and_expression '&' equality_expression
	;

exclusive_or_expression
	: and_expression
	| exclusive_or_expression '^' and_expression
	;

inclusive_or_expression
	: exclusive_or_expression
	| inclusive_or_expression '|' exclusive_or_expression
	;

logical_and_expression
	: inclusive_or_expression
	| logical_and_expression AND_OP inclusive_or_expression
	;

logical_or_expression
	: logical_and_expression
	| logical_or_expression OR_OP logical_and_expression
	;

conditional_expression
	: logical_or_expression
	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression
	| unary_expression assignment_operator assignment_expression
	;

assignment_operator
	: '='
	| MUL_ASSIGN
	| DIV_ASSIGN
	| MOD_ASSIGN
	| ADD_ASSIGN
	| SUB_ASSIGN
	| LEFT_ASSIGN
	| RIGHT_ASSIGN
	| AND_ASSIGN
	| XOR_ASSIGN
	| OR_ASSIGN
	;

expression
	: assignment_expression
	| expression ',' assignment_expression
	;

constant_expression
	: conditional_expression
	;


declaration
	: declaration_specifiers ';'
	| declaration_specifiers init_declarator_list ';' 
           {
              printf("//functypedef(%d)//", functypedef);
              if (in_typedef && 0 == functypedef) 
              {
                 sprintf (saved_identifier, "%s", g_cur_sym); 
#if 1
                 printf ("//EOT(%s)functypedef(%d)//",g_cur_sym, functypedef); // very important
#endif
                 st_insert_typedef (saved_identifier, lineno, column);
                 in_typedef = 0;
              } /* clear flag for 'typedef' */ 
           }
	;

declaration_specifiers
	: storage_class_specifier 
		{
			/* 
			 * 'auto','extern','static','register','typedef'
			 */
			in_var.storage_class_specifier = $1; 
		}
	| storage_class_specifier declaration_specifiers
	| type_specifier 
		{
		/*
		 * This is what we need when defining a variable
		 * Here decides the 
		 *	'int' , 'char', 'long', typedef_name, etc ...
		 */
			in_var.type_specifier = $1;
		#if 0
			printf ("#####(%s)", g_cur_sym);
		#endif
		} 
	| type_specifier declaration_specifiers
	| type_qualifier 
	| type_qualifier declaration_specifiers
	| function_specifier 
	| function_specifier declaration_specifiers 
	;

function_specifier
	: INLINE
	| INLINE2
	;


init_declarator_list
	: init_declarator 
	| init_declarator_list ',' init_declarator 
	;

init_declarator
	: declarator
	| declarator '=' initializer
	;

storage_class_specifier
	: TYPEDEF { in_typedef = 1; }
	| EXTERN
	| STATIC
	| AUTO
	| REGISTER
	;

type_specifier
	: VOID { type_specifiers (3); }
	| CHAR { type_specifiers (3); }
	| SHORT { type_specifiers (4); }
	| INT { type_specifiers (4); }
	| LONG { type_specifiers (4); }
	| FLOAT { type_specifiers (4); }
	| DOUBLE { type_specifiers (4); }
	| SIGNED { type_specifiers (4); }
	| UNSIGNED { type_specifiers (4); }
	| struct_or_union_specifier { type_specifiers (4); }
	| enum_specifier { type_specifiers (4); }
	| TYPE_NAME { type_specifiers (4); }
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' { in_struct_or_union = 1;} struct_declaration_list {in_struct_or_union=0;} '}'
	| struct_or_union '{' {in_struct_or_union=1;} struct_declaration_list {in_struct_or_union=0;} '}'
	| struct_or_union IDENTIFIER { /* printf ("//PRETYPE//");*/ in_struct_or_union=0;} %prec WITHOUT_STRUCT_CONTENT
	;

struct_or_union
	: STRUCT
	| UNION
	;

struct_declaration_list
	: struct_declaration
	| struct_declaration_list struct_declaration
	;

struct_declaration
	: specifier_qualifier_list struct_declarator_list ';'
	;

specifier_qualifier_list
	: type_specifier specifier_qualifier_list
	| type_specifier {in_var.type_specifier = $1;}
	| type_qualifier specifier_qualifier_list
	| type_qualifier
	;

struct_declarator_list
	: struct_declarator
	| struct_declarator_list ',' struct_declarator
	;

struct_declarator
	: declarator
	| ':' constant_expression
	| declarator ':' constant_expression
	;

enum_specifier
	: ENUM '{' enumerator_list enumerator_list_end '}'
	| ENUM IDENTIFIER '{' enumerator_list enumerator_list_end '}'
	| ENUM IDENTIFIER
	;

enumerator_list_end
	: 
	| ','
	;


enumerator_list
	: enumerator
	| enumerator_list ',' enumerator
	;


enumerator
	: IDENTIFIER
	| IDENTIFIER '=' constant_expression
	;


type_qualifier
	: CONST
	| VOLATILE
	;

declarator
	: pointer direct_declarator
	| direct_declarator
	;

pre_direct_declarator
	: { in_namelist = 1; }
	;


direct_declarator
	: IDENTIFIER 
        {
#if 0
           sprintf (saved_identifier, "%s", g_cur_sym); 
           show_id_or_type ();
           if (1==in_typedef && 0==in_para_list &&0==in_struct_or_union)
           {
           } else {
              if (0 == in_para_list && 0 == in_struct_or_union)
              {
                 printf ("//ID(%s)in(%d)inp(%d)//", 
                    saved_identifier, in_struct_or_union, in_para_list);
                 st_insert_id (saved_identifier, lineno, column);
              } else {
                 printf ("//inp(%d)ins(%d)//", in_para_list, in_struct_or_union);
              }
           }
#endif
	}
	| '(' declarator {functypedef=1;printf("//FUNCTYPEDEF(%s)//", g_cur_sym);st_insert_typedef(g_cur_sym, lineno, column);} ')'
	| direct_declarator '[' direct_declarator_needinsert constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')' 
            { functypedef=0; }
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'
	;

direct_declarator_needinsert :
		{
			// printf("$direct_declarator\"%s\"$", g_cur_sym);
			st_insert_id (g_cur_sym, lineno, column);
		}
	;



pointer
	: '*'
	| '*' type_qualifier_list
	| '*' pointer
	| '*' type_qualifier_list pointer
	;

type_qualifier_list
	: type_qualifier
	| type_qualifier_list type_qualifier
	;


parameter_type_list
	: parameter_list { printf ("(P1)"); after_type_specifiers = 0; }
	| parameter_list ',' ELLIPSIS
	;

parameter_list
	: parameter_declaration {printf ("(P2)"); after_type_specifiers = 0; }
	| parameter_list ',' parameter_declaration {printf ("(P3)"); after_type_specifiers = 0;}
	;

parameter_declaration
	: declaration_specifiers_declarator declarator end_declarator
		{
#ifdef PRINT_INIT_DECLARATOR
			/* printf ("@@@(%s)", g_cur_sym); */
#endif
			st_insert_id(g_cur_sym, lineno, column);
		}
	| declaration_specifiers_declarator abstract_declarator end_declarator
	| declaration_specifiers
	|   
	;

end_declarator
	: { in_namelist = 0; }
	;

declaration_specifiers_declarator
	: declaration_specifiers after_decl_spec 
	;

after_decl_spec
	: { in_namelist = 1; /* printf("(AFTER_DECL(%s))", g_cur_sym);*/ }
	;

identifier_list
	: IDENTIFIER
	| identifier_list ',' IDENTIFIER
	;

type_name
	: specifier_qualifier_list
	| specifier_qualifier_list abstract_declarator
	;

abstract_declarator
	: pointer
	| direct_abstract_declarator
	| pointer direct_abstract_declarator
	;

direct_abstract_declarator
	: '(' abstract_declarator 
		{
#ifdef PRINT_INIT_DECLARATOR
			printf ("(%s)", g_cur_sym); 
#endif
		} 
	')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator 
		{
			printf("$direct_abstract_declarator$");
		}
		'[' ']'
	| direct_abstract_declarator 
		{printf("$direct_abstract_declarator2$");}
		'[' constant_expression ']'
	| '(' ')'
	| '(' {before_para (3); in_para_list=1;} parameter_type_list {after_para(3); in_para_list=0;} ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' {before_para (3); in_para_list=1;}parameter_type_list {after_para (3); in_para_list=0;} ')'
	;

initializer
	: assignment_expression
	| '{' initializer_list '}'
	| '{' initializer_list ',' '}'
	;

initializer_list
	: initializer
	| initializer_list ',' initializer
	;

statement
	: labeled_statement
	| compound_statement
	| expression_statement
	| selection_statement
	| iteration_statement
	| jump_statement
	;

labeled_statement
	: IDENTIFIER ':' statement
	| CASE constant_expression ':' statement
	| DEFAULT ':' statement
	;

compound_statement
	: '{' '}'
	| '{' statement_list '}'
	| '{' declaration_list '}' 
	| '{' declaration_list statement_list '}'
	;

declaration_list
	: declaration
	| declaration_list declaration
	;

statement_list
	: statement
	| statement_list statement
	;

expression_statement
	: ';'
	| expression ';'
	;

selection_statement_first_half
	: IF '(' {in_cond_chk = 1;} expression 
		{
#ifndef SET_CLEAN_IN_ID_RECOGNIZE
			set_clean (g_cur_sym); 
#endif
		}
		')' {in_cond_chk = 0;} statement;

selection_statement
	: selection_statement_first_half
	| selection_statement_first_half ELSE statement
	| SWITCH '(' expression ')' statement
	;

iteration_statement
	: WHILE '(' expression ')' statement
	| DO statement WHILE '(' expression ')' ';'
	| FOR '(' expression_statement expression_statement ')' statement
	| FOR '(' expression_statement expression_statement expression ')' statement
	;

jump_statement
	: GOTO IDENTIFIER ';'
	| CONTINUE ';'
	| BREAK ';'
	| RETURN ';'
	| RETURN expression ';'
	;

translation_unit
	: external_declaration
	| translation_unit external_declaration
	;

external_declaration
	: function_definition
	| declaration
	;

function_definition
	: declaration_specifiers declarator declaration_list compound_statement
	| declaration_specifiers declarator compound_statement 
	| declarator declaration_list compound_statement
	| declarator compound_statement
	; 

%%



#include <stdio.h>

extern char yytext[];

yyerror(s)
char *s;
{
	fflush(stdout);
	printf("\n%*s\n%*s(line(%d:%d))SYM(%s)\n", column, "^", column, s, lineno, column, g_cur_sym);
}

int 
check_dirty (int index)
{
	return 0;
}

#include "string.h"

int 
main (int argc, char **argv)
{
	int err = 0;

yydebug = 0;
	err = yyparse ();
	printf ("\nyyparse return (%d)\n\n", err);
	print_st ();
	release_table ();
	release_typedef_table ();

	return 0;
}

int 
set_clean (char * saved_id)
{
	BucketList p = NULL;

	// printf ("LEFT(%s)RIGHT(%S)", saved_left_sym, saved_right_sym);
	p = st_lookup (saved_left_sym);
	if (NULL == p) {
		// nothing 
	} else {
		p->dirty = 0;
	}
	p = st_lookup (saved_right_sym);
	if (NULL == p) {
		// nothing 
	} else {
		p->dirty = 0;
	}

	return 0;
}

int 
save_array (char * arrayname)
{
	// printf ("postfix_expression(%s)",arrayname); 
	check_dirty(0);
	sprintf (saved_array_name, "%s", arrayname);

	return 0;
}

int 
check_array_index (char * indexname)
{
	BucketList p = NULL;

	if (0 != strcmp (saved_array_name, indexname)) {
		printf("SYM(%s)",indexname);

		p = st_lookup (indexname);
		if (NULL == p) {
			/* increadiable */;
			printf ("INCREADIABLE");
		} else {
			if (p->dirty) {
				/*
				 * dirty, NOT cleared
				 */
				printf ("===>NOT cleared(line:%d)", lineno);
			} else {
			}
		}
	}

	return 0;
}

int 
save_left_sym (char * left)
{
	sprintf (saved_left_sym, "%s", left);
}

int 
save_right_sym (char * right)
{
	sprintf (saved_right_sym, "%s", right);
}

int 
enter_block (void)
{
	// printf ("enter block [%s]", saved_func_name);
	return 0;
}

int 
leave_block (void)
{
	// printf ("leave block [%s]", saved_func_name);
	memset (saved_func_name, 0, MAXSYMLEN);

	return 0;
}

int show_id_or_type (void)
{
#if 0
	printf("(FOUND ID_IN_DECL(%s)P(%d))INTYPEDEF(%d)SU(%d)(%s)",
		saved_identifier, 
		in_para_list, 
		in_typedef, 
		in_struct_or_union,
		(1==in_typedef&&0==in_para_list&&0==in_struct_or_union)? 
				"st_inserttype":"st_insert_id"); 
#endif

	return 0;
}

int show_pos (const char * tag)
{
	printf (tag);
	return 0;
}

int 
type_specifiers (int n)
{
	if (in_para_list)
	{
		if (0)
		{
			printf ("(TYPESPECIFIERS)");
		} else {
			printf ("(T)");
		}
	}
	after_type_specifiers = 1;

	return 0;
}


/* 
 * before parameter declaration list in function head.
 */
int
before_para (int n)
{
	// printf ("(INININ)");
	return 0;
}

/* 
 * after parameter declaration list in function head.
 */
int
after_para (int n)
{
	// printf ("(OUOUOU)");
	return 0;
}
