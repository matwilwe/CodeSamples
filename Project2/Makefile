PATH := /usr/um/gcc-4.7.0/bin:$(PATH)
LD_LIBRARY_PATH := /usr/um/gcc-4.7.0/lib64
LD_RUN_PATH := /usr/um/gcc-4.7.0/lib64

FLAGS = -Wall -Wextra -pedantic -std=c++11

release: FLAGS += -O3
release: all

debug: FLAGS += -g
debug: all

all: market.cpp order.o median.o market_helper.o transfer.o
	g++ $(FLAGS) market.cpp order.o median.o market_helper.o transfer.o -o market

order.o: order.cpp
	g++ $(FLAGS) -c order.cpp

market_helper.o: market_helper.cpp
	g++ $(FLAGS) -c market_helper.cpp

transfer.o: transfer.cpp
	g++ $(FLAGS) -c transfer.cpp

median.o: median.cpp
	g++ $(FLAGS) -c median.cpp

clean:
	rm -f *.o market

