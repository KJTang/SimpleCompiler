# output name
TARGET = a.out
# compiler
CC = g++
# files
SRC =   main.cpp \
		VirtualMachine.cpp \
		Lexer.cpp \
		AST.cpp \
		Parser.cpp

# object files
OBJS = $(SRC:.cpp=.o)
# compile setting
COMPILE_FLAGS = -std=c++11
# include files
INCLUDE_PATH = 
# library 
LINKER_FLAGS = 

all : $(TARGET)
# target file generate rule
$(TARGET) : $(OBJS)
	$(CC) $(OBJS) $(LINKER_FLAGS) -o $(TARGET) $(COMPILE_FLAGS)

# define suffixes rule
.SUFFIXES : .cpp
# object file generate rule
.cpp.o :
	$(CC) $(INCLUDE_PATH) $(LINKER_FLAGS) -c $< -std=c++11
# clean
clean:
	rm $(TARGET) $(OBJS)