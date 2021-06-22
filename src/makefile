#include directories
INCLUDES+=. core core/nodes scenes scenes/pong tools vendor/colliders vendor/imgui vendor/imgui/backends vendor/imgui-sdl

#libraries
LIBS+=-static-libstdc++ -lpthread
ifeq (commandline,${whichshell}) #I think I've screwed with my windows shell too much
	LIBS+=-lmingw32
else
	LIBS+=-lcygwin
endif
LIBS+=-lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lole32 -loleaut32 -limm32 -lwinmm

#flags
CXXFLAGS+=-std=c++17 $(addprefix -I,$(INCLUDES)) -Wno-psabi `sdl2-config --cflags --libs` -mwindows

#source
CXXSRC=$(wildcard *.cpp) \
	$(wildcard core/*.cpp) \
	$(wildcard core/nodes/*.cpp) \
	$(wildcard scenes/*.cpp) \
	$(wildcard scenes/pong/*.cpp) \
	$(wildcard tools/*.cpp) \
	$(wildcard vendor/colliders/*.cpp) \
	$(wildcard vendor/imgui/*.cpp) \
	$(wildcard vendor/imgui/backends/imgui_impl_sdl.cpp) \
	$(wildcard vendor/imgui-sdl/imgui_sdl.cpp)

#objects
OBJDIR=obj
OBJ+=$(CXXSRC:.cpp=.o)

#output
OUTDIR=../out
OUT=$(addprefix $(OUTDIR)/,out)

#targets
all: $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(OBJ) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
