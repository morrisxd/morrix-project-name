#
# xxx.l and xxx.o MUST have the idential 'xxx' filename, and the same, 
# xxx.y and xxx.o MUST have the idential 'xxx' filename.


# YACC   = bison
YFLAGS = -d
OBJS = cl.o c.o symtab.o
# OBJS   = cl.o 
cl:	$(OBJS)
	cc $(OBJS) -ll -o cl
clean:
	rm -f *.o y.tab.h cl *.core *.BAK

