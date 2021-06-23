#Windows 7:
#RM=del /y

#Windows 8.1, Windows 10:
#RM=del /S

#Output directory
OUTDIR=out

all: $(OUTDIR) binary
	$(MAKE) -C src

debug: export CXXFLAGS+=-g
debug: all

release: export CXXFLAGS+=-O2
release: clean all

rebuild: clean all

#For use on my machine ONLY (I have a bin directory with all of the .dlls next to this repo)
binary: $(OUTDIR)
ifeq ($(OS),Windows_NT)
	xcopy /Y "..\bin\*.dll" $(OUTDIR)
endif

$(OUTDIR):
	mkdir $(OUTDIR)

clean:
ifeq (commandline,${whichshell}) #I think I've screwed with my windows shell too much
	# not working
else
	find . -type f -name '*.o' -exec rm -f -r -v {} \;
	find . -type f -name '*.a' -exec rm -f -r -v {} \;
#	rm $(OUTDIR)/* -f -r -v
#	find . -empty -type d -delete
endif
