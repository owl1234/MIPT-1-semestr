CUR_PROG = language
ARGS = 


CPP = g++
LD = g++

WARNINGS = -Wall -Wextra
STANDARD = 
OPTIMIZATION = -O1

CFLAGS = $(WARNINGS) $(STANDARD) $(OPTIMIZATION) -g

.PHONY : all run debug clean

compile: parsing.o compiler.o warnings.o stack.o work_with_tree.o work_with_file.o
	$(CPP) $(CFLAGS) -c ./scr/main.cpp -o main.o
	$(LD) main.o parsing.o compiler.o warnings.o stack.o work_with_tree.o work_with_file.o -o $(CUR_PROG)
	./language

assembly: assembler.o warnings.o stack.o work_with_tree.o work_with_file.o common.o
	$(CPP) $(CFLAGS) -c ./scr/assembler.cpp -o assembler.o
	$(LD) assembler.o warnings.o stack.o work_with_tree.o work_with_file.o common.o -o $(CUR_PROG) 
	./language -a asm_result.txt code.xax

processing: processor.o warnings.o stack.o work_with_tree.o work_with_file.o common.o
	$(CPP) $(CFLAGS) -c ./scr/processor.cpp -o processor.o
	$(LD) processor.o warnings.o stack.o work_with_tree.o work_with_file.o common.o -o $(CUR_PROG) 
	./language code.xax proc_log.txt

assembler.o: ./scr/assembler.cpp ./scr/assembler.h
	$(CPP) $(CFLAGS) -c ./scr/assembler.cpp -o assembler.o

parsing.o: ./scr/parsing.cpp ./scr/parsing.h
	$(CPP) $(CFLAGS) -c ./scr/parsing.cpp -o parsing.o
	
compiler.o: ./scr/compiler.cpp ./scr/compiler.h
	$(CPP) $(CFLAGS) -c ./scr/compiler.cpp -o compiler.o
	
warnings.o: ./scr/warnings.cpp ./scr/warnings.h
	$(CPP) $(CFLAGS) -c ./scr/warnings.cpp -o warnings.o
	
stack.o: ./scr/stack.cpp ./scr/stack.h
	$(CPP) $(CFLAGS) -c ./scr/stack.cpp -o stack.o
	
work_with_tree.o: ./scr/work_with_tree.cpp ./scr/work_with_tree.h
	$(CPP) $(CFLAGS) -c ./scr/work_with_tree.cpp -o work_with_tree.o

work_with_file.o: ./scr/work_with_file.cpp ./scr/work_with_file.h
	$(CPP) $(CFLAGS) -c ./scr/work_with_file.cpp -o work_with_file.o

common.o: ./scr/common.cpp ./scr/common.h
	$(CPP) $(CFLAGS) -c ./scr/common.cpp -o common.o

run: all
	./$(CUR_PROG) $(ARGS)

debug: all
	valgrind --dump-instr=yes --tool=callgrind  ./$(CUR_PROG) $(ARGS)

clean:
	rm *.o
