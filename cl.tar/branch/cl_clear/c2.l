/*
ANSI C grammar, Lex specification
In 1985, Jeff Lee published this Lex specification together with a Yacc grammar for the April 30, 1985 ANSI C draft.  Tom Stockfisch reposted both to net.sources in 1987; that original, as mentioned in the answer to question 17.25 of the comp.lang.c FAQ, can be ftp'ed from ftp.uu.net, file usenet/net.sources/ansi.c.grammar.Z. 
I intend to keep this version as close to the current C Standard grammar as possible; please let me know if you discover discrepancies. 

Jutta Degener, 1995 

*/



precompile		#
newline			\n
D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*
P			("[" | "]" | "(" | ")" | "{" | "}" | "*" | "," | "." | "=" | ";" | "..." | "#")

%{
#include <stdio.h>
#include "globals.h"

#include "y.tab.h"

int lineno = 1;

void count();
char chsharp = 0;
%}

%%
{L}({L}|{D})*		{ return IDENTIFIER; }
{P}			{ return PUNCTUATOR; }


[ \t\v\n\f]		{ count(); }
.			{ /* ignore bad characters */ }

%%

yywrap()
{
	return(1);
}


comment()
{
	char c, c1;

loop:
	while ((c = input()) != '*' && c != 0)
		putchar(c);

	if ((c1 = input()) != '/' && c != 0)
	{
		unput(c1);
		goto loop;
	}

	if (c != 0)
		putchar(c1);
}


int column = 0;

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n')
			column = 0;
		else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;

	ECHO;
}


int check_type()
{
/*
* pseudo code --- this is what it should check
*
*	if (yytext == type_name)
*		return(TYPE_NAME);
*
*	return(IDENTIFIER);
*/

/*
*	it actually will only return IDENTIFIER
*/

	return(IDENTIFIER);
}

