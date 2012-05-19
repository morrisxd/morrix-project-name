#include "cvs.h"
#include "stdio.h"
#include "stdlib.h"

/*
 * utils used by commit.c etc...
 */
void ACTION (char *hint, char * msg)
{
	char buffer[2048];

	memset(buffer, 0, 2048);
	if (NULL == msg)
	{
		sprintf(buffer, "%s=[NULL]", hint);
	}
	else
	{
		sprintf(buffer, "%s=[%s]", hint, msg);
	}

	error (0, 0, buffer);
}

/*****
 * print content of a linktable of List
 **/
int showlist(List * pl)
{
	int num = 0;
	int i = 0;
	Node * ph=NULL;
	Node * pe=NULL;

	if (pl)
	{
		ph = pe = pl->list;
	}
	else 
	{
		ACTION ("List null", "");
		return 0;
	}

	ACTION ("inside showlist", "");

	while (pe)
	{	
		num += 1;
		// ACTION ("1", "");
		break;
/*
		if (pe->key != NULL)
		{
			// ACTION ("key", pe->key);
		}
		else
		{
			// ACTION ("key", "NULL");
		}
		ACTION ("2", "");
		pe = pe->next;
		ACTION ("3", "");
		if (pe == ph) break;
		ACTION ("4", "");
*/
	}

	ACTION ("showlist ok", "");

	return num;
}

