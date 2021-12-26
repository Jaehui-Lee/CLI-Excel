main: main.o initialpage.o filemanager.o manual.o cell.o table.o excel.o excellist.o
	g++ -o main main.o initialpage.o filemanager.o manual.o cell.o table.o excel.o excellist.o -lncurses -lpthread

main.o: main.cpp
	g++ -c -o main.o main.cpp

initialpage.o: initialpage.h initialpage.cpp
	g++ -c -o initialpage.o initialpage.cpp

filemanager.o: filemanager.h filemanager.cpp
	g++ -c -o filemanager.o filemanager.cpp

manual.o: manual.h manual.cpp
	g++ -c -o manual.o manual.cpp

cell.o: cell.h cell.cpp
	g++ -c -o cell.o cell.cpp

table.o: table.cpp
	g++ -c -o table.o table.cpp

excel.o: excel.h excel.cpp
	g++ -c -o excel.o excel.cpp

excellist.o: excellist.h excellist.cpp
	g++ -c -o excellist.o excellist.cpp

clean:
	rm *.o
