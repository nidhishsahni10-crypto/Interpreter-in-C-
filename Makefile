FNAME = pic

CPP = g++
BISON = bison
FLEX = flex
TGT = runme

SCAN = $(FNAME).l
PARSE = $(FNAME).y
HEADER = $(FNAME).hh
FILE = $(FNAME).cc
EXTRA_FILES = env.cpp objects.cpp

OBJ = scan.o parse.tab.o
CFLAGS = -g

$(TGT): $(OBJ)
	$(CPP) --std=c++0x  $(OBJ) -o $(TGT) -ly 

scan.o: scan.c $(HEADER) parse.tab.h
	$(CPP) --std=c++0x  $(CFLAGS) -c $<

parse.tab.o: parse.tab.c parse.tab.h $(FILE) $(HEADER)
	$(CPP) --std=c++0x  $(CFLAGS) -c  $<

scan.c : $(SCAN) $(HEADER) parse.tab.h
	$(FLEX) -l --yylineno -o scan.c $(SCAN)

parse.tab.c parse.tab.h : $(PARSE) $(HEADER) $(FILE) $(EXTRA_FILES)
	$(BISON) -b parse -dv  $(PARSE)

clean :
	rm -f *.o *.output
	rm -f $(TGT) 
	rm -rf parse.tab.c parse.tab.h scan.c 

test : $(TGT)
	./$(TGT) < input_monkey.txt