NAME := MyClient
FLAGS := -ggdb -g -Wno-unused-result -Wall -O -std=c++11
CC := g++

all: ${NAME}

${NAME}: *cpp
	${CC} ${FLAGS} -o ${NAME} *.cpp

clean:
	rm -f *o ${NAME}
