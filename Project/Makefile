PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

FLAGS = -Wall -Wextra -pedantic -std=c++11

release: FLAGS += -O3
release: all

debug: FLAGS += -g
debug: all

all:  base.cpp base_helper.o
	g++ $(FLAGS) base.cpp base_helper.o -o base

base_helper.o:   base_helper.cpp base_helper.h
			g++ $(FLAGS) base_helper.cpp -c

clean:
	rm -f *.o base