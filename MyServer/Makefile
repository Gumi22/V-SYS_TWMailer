NAME := MyServer
FLAGS := -ggdb -g -Wall -Werror -O -std=c++11 -pthread
CC := g++

all: ${NAME}

${NAME}: *cpp
	${CC} ${FLAGS} -o ${NAME} *.cpp

clean:
	rm -f *o ${NAME}
