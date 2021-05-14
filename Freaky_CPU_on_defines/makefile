all:
	g++ -c -O1 main.cpp
	g++ main.o -o sfml_app image_processing.cpp -lsfml-graphics -lsfml-window -lsfml-system
	
assembly:
	g++ -Wextra -Wall assembler.cpp common.cpp
	./a.out -a example.txt 290421res.xax
	
processing:
	g++ -Wextra -Wall processor.cpp common.cpp stack.cpp
	./a.out 290421res.xax proc_log.txt

run:	all
	./sfml_app
	
debug:	all
	valgrind ./sfml_app
