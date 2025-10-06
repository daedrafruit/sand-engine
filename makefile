SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp,%.o,$(SRCS))

all: build run

%.o: src/%.cpp
	g++ -g -c $< -o $@ -Iinclude -I/usr/include/SDL3

build: $(OBJS)
	g++ $(OBJS) -g -o sandgame -L/usr/lib -lSDL3

run: sandgame
	./sandgame

clean:
	rm -f *.o sandgame

