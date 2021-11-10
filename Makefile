main: main.o cell.o table.o txttable.o excel.o
	g++ -o main main.o cell.o table.o txttable.o excel.o -lncurses

main.o: main.cpp
	g++ -c -o main.o main.cpp

cell.o: cell.h cell.cpp
	g++ -c -o cell.o cell.cpp

table.o: table.h table.cpp
	g++ -c -o table.o table.cpp

txttable.o: table.h txttable.h txttable.cpp
	g++ -c -o txttable.o txttable.cpp

excel.o: excel.h excel.cpp
	g++ -c -o excel.o excel.cpp

clean:
	rm *.o
