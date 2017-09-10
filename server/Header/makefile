all: prog

prog: test.o DataBase.o
	g++ -std=c++11 test.o DataBase.o -g -o prog
test.o: test.cpp
	g++ -std=c++11 test.cpp -c -g -o test.o
DataBase.o: DataBase.cpp DataBase.hpp
	g++ -std=c++11 DataBase.cpp -c -g -o DataBase.o
clean:
	rm *.o
	rm prog
