#Windows 7:
#RM=del /y

#Windows 8.1, Windows 10:
#RM=del /S

#Output directory
OUTDIR=out

#Windows-specific .dll files
BINDIR=bin

all: $(OUTDIR)
	$(MAKE) -C src

debug: export CXXFLAGS+=-g
debug: all

release: export CXXFLAGS+=-O2
release: clean all

rebuild: clean all

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
