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
     if only i see (func_name), i take 'func_name' as function type definitioin. because i think it must be "typedef (*fname)(int i, float f); but it was a mistake. i should fix it.
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

#define PRINTF myprintf

void myprintf()
{
}

#define TYPEDEF_FUNC {td_func=1;PRINTF("//FUNCTYPEDEF(%s)//", g_cur_sym); column = column + strlen(g_cur_sym) + 17; st_insert_typedef(g_cur_sym, lineno, column);}

/*
 * These macro has no use indeed, the code was written firstly not so good,
 * and then was improved later. So I use the macro(s) to disable the old
 * code and use new code.
 */
#define SET_CLEAN_IN_ID_RECOGNIZE
#define CHECK_INDEX_IN_ID_RECOGNIZE

typedef enum{
   in_para_list,
   in_func_typedef,
   in_struct_or_union,
   in_none
}pos;

pos tmp = in_none;

int td_func = 0;
int in_typedef = 0;

// #define YYSTYPE treenode *

/*
 * save the identifier temperoraily for instance usage, 
 * and will be discarded soon. 
 */
char g_cur_sym [MAXSYMLEN];
char identifier [MAXSYMLEN];
char saved_identifier [MAXSYMLEN];
%}



%token IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token XOR_ASSIGN OR_ASSIGN TYPE_NAME

%token TYPEDEF EXTERN STATIC AUTO REGISTER
%token CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token STRUCT UNION ENUM ELLIPSIS

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%nonassoc LOWER_THAN_DECLARATOR
%nonassoc DECLARATOR
%nonassoc H_DECLARATOR

%token CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token INLINE INLINE2

%start translation_unit

%nonassoc LOWER_THAN_BRACE
%nonassoc '{' 



%%

primary_expression
	: IDENTIFIER 
	| CONSTANT
	| STRING_LITERAL
	| '(' expression ')'
	;

postfix_expression
	: primary_expression
	| postfix_expression '[' expression ']'
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
	| relational_expression '<' shift_expression 
	| relational_expression '>' shift_expression
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
              if (td_func)
	      {
	         PRINTF("//td_func(%d)//",td_func);
                 column = column + 1 + 13;
              }
              if (in_typedef)
              {
                 if (td_func) 
                 {
                    // function type
                    td_func = 0;
                    PRINTF("//clearFunctypedef//");
                    column = column + 20;
                 } else {
                    // normal type
                    sprintf (saved_identifier, "%s", g_cur_sym); 
                    PRINTF ("//EOT(%s)//",g_cur_sym);
                    column = column + strlen(g_cur_sym) + 9;
                    st_insert_typedef (saved_identifier,lineno, column);
                    in_typedef = 0; 
//		    puts("//outoftypedef//");
                 }
                 in_typedef = 0;
//		 puts("//outoftypedef#2//");
              } else {
                 if (td_func)
                 {
                    // function pointer definition
                    td_func = 0;
                    PRINTF ("//clear:td_func//");
                    column = column + 17;
                 } else {
                 }
              }
           }
	;

declaration_specifiers
	: storage_class_specifier 
	| storage_class_specifier declaration_specifiers
	| type_specifier 
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
	: VOID
	| CHAR
	| SHORT
	| INT
	| LONG
	| FLOAT
	| DOUBLE
	| SIGNED
	| UNSIGNED
	| struct_or_union_specifier
	| enum_specifier
	| TYPE_NAME
	;

struct_or_union_specifier
	: struct_or_union IDENTIFIER '{' struct_declaration_list '}'
	| struct_or_union '{' struct_declaration_list '}'
	| struct_or_union IDENTIFIER %prec LOWER_THAN_BRACE
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
	| type_specifier
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

direct_declarator
	: IDENTIFIER 
            {
               switch (tmp) {
               case in_para_list:
#if 1
                  PRINTF("//PARA(%s)//", g_cur_sym);
                  column = column + strlen(g_cur_sym) + 10;
#endif
                  break;
               case in_struct_or_union:
                  PRINTF ("//VAR(%s)//", g_cur_sym);
                  column = column + strlen(g_cur_sym) + 9;
                  break;
               default:
                  break;
               }
            }
	| '(' declarator ')'
	| direct_declarator '[' constant_expression ']'
	| direct_declarator '[' ']'
	| direct_declarator '(' parameter_type_list ')' 
	| direct_declarator '(' identifier_list ')'
	| direct_declarator '(' ')'
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
	: parameter_list
	| parameter_list ',' ELLIPSIS
	;

parameter_list
	: parameter_declaration
	| parameter_list ',' parameter_declaration
	;

parameter_declaration
	: declaration_specifiers declarator %prec DECLARATOR
	| declaration_specifiers abstract_declarator
	| declaration_specifiers	%prec LOWER_THAN_DECLARATOR 
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
	: '(' abstract_declarator ')'
	| '[' ']'
	| '[' constant_expression ']'
	| direct_abstract_declarator '[' ']'
	| direct_abstract_declarator '[' constant_expression ']'
	| '(' ')'
	| '(' parameter_type_list ')'
	| direct_abstract_declarator '(' ')'
	| direct_abstract_declarator '(' parameter_type_list ')'
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

selection_statement
	: IF '(' expression ')' statement	%prec LOWER_THAN_ELSE
	| IF '(' expression ')' statement ELSE statement
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
	printf ("\n%*s\n%*s(line(%d:%d))SYM(%s)\n", column, "^", column, s, lineno, column, g_cur_sym);
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
