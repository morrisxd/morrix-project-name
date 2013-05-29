#
# xxx.l and xxx.o MUST have the idential 'xxx' filename, and the same, 
# xxx.y and xxx.o MUST have the idential 'xxx' filename.

# We use sample file cah_126e1.i

################################################################
# We cannot use 'bison' because bison would not generate y.tab.c file. 
# Standard YACC will do. And make program will execute mv -f y.tab.c 
# filename.c
# YACC   = bison
YFLAGS = -d -v
OBJS = cl.o c.o symtab.o
cl:	$(OBJS)
	cc $(OBJS) -ll -o cl
clean:
	rm -f *.o y.tab.h cl *.core *.BAK

