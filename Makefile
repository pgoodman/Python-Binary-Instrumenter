
CXX_FLAGS = -std=c++11 -stdlib=libc++ -Wall -Werror -Wextra
CXX_FLAGS += -pedantic -I./

GC_OBJS = bin/gc/main.o bin/gc/pthread.o

SRC_OBJS = bin/src/main.o

bin/gc/%.o: gc/%.cc
	clang++ 

bin/gc/%.o: gc/%.cc
	clang++ $(CXX_FLAGS) -c $< -o $@

bin/src/%.o: src/%.cc
	python cc.py $(CXX_FLAGS) -c $< -o $@

install:
	-mkdir bin
	-mkdir bin/gc
	-mkdir bin/src

clean:
	-rm bin/gc/*.o
	-rm bin/src/*.o
	-rm bin/src/*.S
	-rm program.out

all: $(GC_OBJS) $(SRC_OBJS)
	clang++ $(CXX_FLAGS) -pthread $(GC_OBJS) $(SRC_OBJS) -o program.out
