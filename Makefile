main: main.o initialpage.o cell.o table.o txttable.o excel.o excellist.o
	g++ -o main main.o initialpage.o cell.o table.o txttable.o excel.o excellist.o -lncurses

main.o: main.cpp
	g++ -c -o main.o main.cpp

initialpage.o: initialpage.h initialpage.cpp
	g++ -c -o initialpage.o initialpage.cpp

cell.o: cell.h cell.cpp
	g++ -c -o cell.o cell.cpp

table.o: table.h table.cpp
	g++ -c -o table.o table.cpp

txttable.o: table.h txttable.h txttable.cpp
	g++ -c -o txttable.o txttable.cpp

excel.o: excel.h excel.cpp
	g++ -c -o excel.o excel.cpp

excellist.o: excellist.h excellist.cpp
	g++ -c -o excellist.o excellist.cpp

clean:
	rm *.o
