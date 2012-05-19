/*
 * utils used by commit.c etc...
 */
extern void ACTION (char *hint, char * msg);

#if 0
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
#endif


/*****
 * print content of a linktable of List
 **/
extern int showlist(List * pl);

#if 0
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
#endif

