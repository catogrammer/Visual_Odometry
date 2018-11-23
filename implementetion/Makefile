# compilator use
CC = g++
# flags for compile
CFLAGS = -c -Wall
# standart use
CXXFLAGS = -std=c++14
TARGET = essent_matrix
SRC = main.cpp
OBJ = $(SRC:.cpp = .o) 


all : $(TARGET)

$(TARGET) : main.o
	$(CC) -o $(TARGET) main.o

main.o : main.cpp EssentMtrx.hpp Polynom.hpp
	$(CC) $(CFLAGS) $(SRC) $(CXXFLAGS)

%.hpp : %.hpp
	$(CC) $< $@ $(CXXFLAGS)

clean : 
	rm -f  $(TARGET) *.o *.gch
