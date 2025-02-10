main: main.o
	g++ -o main main.o -L/usr/lib/x86_64-linux-gnu/sfml -lsfml-graphics -lsfml-window -lsfml-system
 
main.o: main.cpp 
	g++ -c main.cpp

