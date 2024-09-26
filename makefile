SRCDIR := src
INCDIR := include
BUILDDIR := build

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

build/%.o: src/%.cpp
	g++ -g -c $< -o $@ -I$(INCDIR) -I/usr/include/SDL2

run: $(OBJS)
	g++ $(OBJS) -g -o run -L/usr/lib -lSDL2

clean:
	rm -f $(BUILDDIR)/*.o run
