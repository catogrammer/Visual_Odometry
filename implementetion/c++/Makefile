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
	if [ ! -d $(PATH_BUILD) ]; then mkdir $(PATH_BUILD); fi
	$(CC) -o $(PATH_BUILD)$(TARGET) main.o

main.o : main.cpp $(PATH_HPP)EssentMtrx.hpp
	$(CC) $(CFLAGS) $(SRC) $(CXXFLAGS)

transform : transform_fetures.cpp
	$(CC) transform_fetures.cpp $(CXXFLAGS) -o $(PATH_BUILD)transform_coord

check_equat_xEx : check_equation_xEx.cpp
	$(CC) check_equation_xEx.cpp $(CXXFLAGS) -o $(PATH_BUILD)chec_xEx

homogenues_coords : glob_to_homogen_coords.cpp
	$(CC) glob_to_homogen_coords.cpp $(CXXFLAGS) -o $(PATH_BUILD)homo_coord

clean :
	rm -rf  $(PATH_BUILD) *.o
