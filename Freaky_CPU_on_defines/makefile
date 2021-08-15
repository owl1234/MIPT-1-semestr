all:
	
	
assembly:
	g++ -Wextra -Wall ./src/assembler.cpp ./src/common.cpp
	./a.out -a example.txt 290421res.xax
	
processing:
	g++ -Wextra -Wall ./src/processor.cpp ./src/common.cpp ./src/stack.cpp
	./a.out 290421res.xax proc_log.txt

run:	
	
debug:	
