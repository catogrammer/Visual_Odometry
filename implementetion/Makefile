# compilator use
CC = g++
# flags for compile
CFLAGS = -c -Wall
# standart use
CXXFLAGS = -std=c++14

PATH_HPP = ./src/headers/
PATH_BUILD = ./build/

TARGET = essent_matrix
SRC = main.cpp
OBJ = $(SRC:.cpp = .o)

# сделать отделбную папку для объектников или их тоже в build, тогкда как?

all : $(TARGET)

$(TARGET) : main.o
	$(CC) -o $(PATH_BUILD)$(TARGET) main.o

main.o : main.cpp $(PATH_HPP)EssentMtrx.hpp $(PATH_HPP)Polynom.hpp
	$(CC) $(CFLAGS) $(SRC) $(CXXFLAGS)

%.hpp : %.hpp
	$(CC) $< $@ $(CXXFLAGS)

clean :
	rm -f  $(PATH_BUILD)$(TARGET) *.o *.gch
