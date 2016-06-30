# output name
TARGET = a.out
# compiler
CC = g++
# files
SRC =   main.cpp \
		Lexer.cpp \
		AST.cpp \
		Parser.cpp \
		Assembler.cpp 

# object files
OBJS = $(SRC:.cpp=.o)
# compile setting
COMPILE_FLAGS = -std=c++11 -ggdb
# include files
INCLUDE_PATH = 
# library 
LINKER_FLAGS = -std=c++11

all : $(TARGET)
# target file generate rule
$(TARGET) : $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LINKER_FLAGS)

# define suffixes rule
.SUFFIXES : .cpp
# object file generate rule
.cpp.o :
	$(CC) $(INCLUDE_PATH) -c $< $(COMPILE_FLAGS)
	
# denpendency
%.d:%.cpp
	@set -e; \
	rm -f $@; $(CC) -MM $< $(INCLUDEFLAGS) > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(OBJS:.o=.d)

.PHONY : clean
# clean
-clean:
	rm $(TARGET) $(OBJS) *.d *.d.*