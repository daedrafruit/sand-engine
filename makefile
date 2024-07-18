SRCDIR := src
INCDIR := include
BUILDDIR := build

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

build/%.o: src/%.cpp
	g++ -c $< -o $@ -Iinclude -IC:\ProgramData\mingw64\SDL2\include\SDL2

run: $(OBJS)
	g++ $(OBJS) -o $@ -LC:\ProgramData\mingw64\SDL2\lib -lmingw32 -lSDL2main -lSDL2 

clean:
	del /Q build\*.o run.exe
