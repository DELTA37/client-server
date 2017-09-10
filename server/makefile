all: prog 

prog: main.o Server.o Header/DataBase.o
	g++ -std=c++11 main.o Server.o Header/DataBase.o -g -o prog
main.o: main.cpp
	g++ -std=c++11 main.cpp -c -g -o main.o
Server.o: Server.cpp Server.hpp
	g++ -std=c++11 Server.cpp -c -g -o Server.o
Header/DataBase.o: Header/DataBase.cpp Header/DataBase.hpp
	g++ -std=c++11 Header/DataBase.cpp -c -g -o Header/DataBase.o
clean:
	rm *.o
	rm prog
