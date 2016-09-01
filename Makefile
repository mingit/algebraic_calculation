DIR_SRC = ./src
DIR_OBJ = ./obj
DIR_BIN = ./bin

CXX			=	g++
CFLAGS		=	-std=c++11 -g -Wall 
LDFLAGS		=	

TARGET		=   cal_algebraic

${DIR_BIN}/${TARGET}: ${DIR_OBJ}/cal_algebraic.o
	$(CXX) $< ${LDFLAGS} -o $@ 

${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	$(CXX) $(CFLAGS) -c  $< -o $@

.PHONY:clean
clean:
	-rm -fr ${DIR_OBJ}/*.o
	-rm -fr ${DIR_BIN}/*
