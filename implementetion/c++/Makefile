# compilator use
CC =g++
# flags for compile
CFLAGS =-c -Wall
# standart use
CXXFLAGS =-std=c++14

PATH_HPP =./src/headers/
PATH_BUILD =./build/

TARGET =essent_matrix
SRC =main.cpp
OBJ =$(SRC:.cpp = .o)

# сделать отдельную папку для объектников или их тоже в build, тогкда как?

all : $(TARGET)

$(TARGET) : main.o
	mkdir $(PATH_BUILD)
	$(CC) -o $(PATH_BUILD)$(TARGET) main.o

main.o : main.cpp $(PATH_HPP)EssentMtrx.hpp $(PATH_HPP)Polynom.hpp
	$(CC) $(CFLAGS) $(SRC) $(CXXFLAGS)

transform :
	$(CC) Transformation_Matrix.cpp $(CXXFLAGS) -o transform_coord

clean :
	rm -rf  $(PATH_BUILD) *.o
