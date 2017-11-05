NAME := MyServer
FLAGS := -ggdb -g -O -std=c++11 -pthread -DLDAP_DEPRECATED
LIBFLAGS := /usr/lib/x86_64-linux-gnu/libboost_filesystem.so /usr/lib/x86_64-linux-gnu/libldap.so /usr/lib/x86_64-linux-gnu/liblber.so
CC := g++

all: ${NAME}

${NAME}: *cpp
	${CC} ${FLAGS} -o ${NAME} *.cpp ${LIBFLAGS}

clean:
	rm -f *o ${NAME}
