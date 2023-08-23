TARGET=my_mastermind
OBJ=my_mastermind.c
GCC_FLAGS= -Wall -Wextra -Werror
GCC=gcc
FLAG_O=-o

all: ${OBj}
	${GCC} ${GCC_FLAGS} ${OBJ} ${FLAG_O} ${TARGET}