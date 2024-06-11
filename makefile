SRCDIR := src
INCDIR := include
BUILDDIR := build

SRCS := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

CXXFLAGS := -I$(INCDIR) -IC:\ProgramData\mingw64\SDL2\include\SDL2
LDFLAGS := -LC:\ProgramData\mingw64\SDL2\lib -lmingw32 -lSDL2main -lSDL2 

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	g++ -c $< -o $@ $(CXXFLAGS)

run: $(OBJS)
	g++ $(OBJS) -o $@ $(LDFLAGS)

clean:
	del /Q build\*.o run.exe

