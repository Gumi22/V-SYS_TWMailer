NAME := MyServer
FLAGS := -ggdb -g -O -std=c++11 -pthread -DLDAP_DEPRECATED
CC := g++

all: ${NAME}

${NAME}: *cpp
	${CC} ${FLAGS} -o ${NAME} *.cpp /usr/lib/x86_64-linux-gnu/libldap.so /usr/lib/x86_64-linux-gnu/liblber.so -luuid

clean:
	rm -f *o ${NAME}
