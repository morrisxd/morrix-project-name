
ifeq "$(OS)" "Windows_NT"
	WINDPLCC=windpl.exe
else
	WINDPLCC=windpl
endif

all:  example.obj example_rx.obj example_tx.obj example.eef 

	
OBJ = \
example_tx.obj \
example_rx.obj \
example.obj

example.eef: $(OBJ)
	 @echo ''
	 @echo 'Creating executable ...'
	 @echo ''
	winlnk  -o example.eef -g   $(OBJ)
	 @echo 'Build completed'
	 
	
example.obj : ./example.dpl example.h
	 $(WINDPLCC)  -rB -dWP -g  -I. -I./tests  -o example.obj example.dpl

example_rx.obj : ./example_rx.dpl example.h
	 $(WINDPLCC)  -rB -dWP -g  -I. -I./tests  -o example_rx.obj example_rx.dpl

example_tx.obj : ./example_tx.dpl example.h
	 $(WINDPLCC)  -rB -dWP -g  -I. -I./tests  -o example_tx.obj example_tx.dpl
	 
clean:
		rm -f *.obj *.eef
