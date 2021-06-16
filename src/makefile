#include directories
INCLUDES+=. core core/nodes scenes tools vendor/imgui vendor/imgui/backends vendor/imgui-sdl

#libraries
#the order of the $(LIBS) is important, at least for MinGW
LIBS+=
ifeq ($(OS),Windows_NT)
	LIBS+=-lmingw32 -lSDL2main
endif
ifeq ($(shell uname), Linux)
	LIBS+=-static-libstdc++ -lpthread
endif
LIBS+=-lSDL2 -lSDL2_image -lSDL2_mixer

#flags
CXXFLAGS+=-std=c++17 $(addprefix -I,$(INCLUDES)) -Wno-psabi

#source
CXXSRC=$(wildcard *.cpp) \
	$(wildcard core/*.cpp) \
	$(wildcard core/nodes/*.cpp) \
	$(wildcard scenes/*.cpp) \
	$(wildcard tools/*.cpp) \
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
