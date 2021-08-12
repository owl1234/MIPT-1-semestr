CUR_PROG = language
ARGS = 


CPP = g++
LD = g++

WARNINGS = -Wall -Wextra
STANDARD = 
OPTIMIZATION = -O1

CFLAGS = $(WARNINGS) $(STANDARD) $(OPTIMIZATION) -g

.PHONY : all run debug clean

all: parsing.o compiler.o warnings.o stack.o work_with_tree.o work_with_file.o
	$(CPP) $(CFLAGS) -c main.cpp -o main.o
	$(LD) main.o parsing.o compiler.o warnings.o stack.o work_with_tree.o work_with_file.o -o $(CUR_PROG)

parsing.o: parsing.cpp parsing.h
	$(CPP) $(CFLAGS) -c parsing.cpp -o parsing.o
	
compiler.o: compiler.cpp compiler.h
	$(CPP) $(CFLAGS) -c compiler.cpp -o compiler.o
	
warnings.o: warnings.cpp warnings.h
	$(CPP) $(CFLAGS) -c warnings.cpp -o warnings.o
	
stack.o: stack.cpp stack.h
	$(CPP) $(CFLAGS) -c stack.cpp -o stack.o
	
work_with_tree.o: work_with_tree.cpp work_with_tree.h
	$(CPP) $(CFLAGS) -c work_with_tree.cpp -o work_with_tree.o

work_with_file.o: work_with_file.cpp work_with_file.h
	$(CPP) $(CFLAGS) -c work_with_file.cpp -o work_with_file.o


run: all
	./$(CUR_PROG) $(ARGS)

debug: all
	valgrind --dump-instr=yes --tool=callgrind  ./$(CUR_PROG) $(ARGS)

clean:
	rm *.o
