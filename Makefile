CXX = g++
SRCS = $(wildcard src/*.cpp)
OBJS = $(wildcard *.o)
TARGET = main
INC = -I ./include
OPT = -lncurses -lpthread

all : $(TARGET)
	$(CXX) -std=c++17 -o $(TARGET) $(OBJS) $(INC) $(OPT)

$(TARGET) :
	$(CXX) -std=c++17 -c $(SRCS) $(INC)

clean:
	rm -f $(OBJS)