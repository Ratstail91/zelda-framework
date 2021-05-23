#include directories
INCLUDES+=. core core/nodes scenes tools

#libraries
#the order of the $(LIBS) is important, at least for MinGW
LIBS+=
ifeq ($(OS),Windows_NT)
	LIBS+=-lmingw32 -lSDL2main
endif
ifeq ($(shell uname), Linux)
	LIBS+=-static-libstdc++
endif
LIBS+=-lSDL2 -lSDL2_image

#flags
CXXFLAGS+=-std=c++17 $(addprefix -I,$(INCLUDES))

#source
CXXSRC=$(wildcard *.cpp) \
	$(wildcard core/*.cpp) \
	$(wildcard core/nodes/*.cpp) \
	$(wildcard scenes/*.cpp) \
	$(wildcard tools/*.cpp)

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
