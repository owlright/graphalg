# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++17 -Wall -Wno-write-strings  -Wno-sign-compare -Wno-stringop-truncation
LDFLAGS = -lgvc -lcgraph -lcdt
DBGCXXFLAGS = $(CXXFLAGS) -g3 -O0 -DDEBUG
# Makefile settings - Can be customized.
LIBNAME = graph
DBGLIBNAME = $(LIBNAME)_dbg
EXT = .cc
LIBSRCDIR = src
LIBOBJDIR = obj

all:
	@echo "Must name a target!"
############# execute ##############
SRCS := $(wildcard *.cc)
EXES := $(patsubst %.cc, %, $(SRCS))

%: %$(EXT) $(LIBNAME)
	$(CC) $(CXXFLAGS) -o $@ -I$(LIBSRCDIR) $< -L. -l$(LIBNAME)

############# library ##############
LIBSRC := $(shell find $(LIBSRCDIR) -type f -name '*$(EXT)')
LIBINC := $(shell find $(LIBSRCDIR) -type f -name '*.h')
LIBOBJ := $(LIBSRC:$(LIBSRCDIR)/%$(EXT)=$(LIBOBJDIR)/%.o)
LIBDEP := $(LIBSRC:$(LIBSRCDIR)/%$(EXT)=$(LIBOBJDIR)/%.d)
DBGLIBOBJ := $(LIBSRC:$(LIBSRCDIR)/%$(EXT)=$(LIBOBJDIR)/%_dbg.o)
DEP := $(patsubst $(LIBSRCDIR)/%.cc,$(LIBOBJDIR)/%.d,$(LIBSRC))

$(LIBNAME): $(LIBOBJ)
	$(CC) $(CXXFLAGS) -O3 -shared -o lib$@.so $^ $(LDFLAGS)

$(LIBOBJDIR)/%.d: $(LIBSRCDIR)/%$(EXT)
	@mkdir -p $(@D)
	@$(CC) $(CXXFLAGS) $< -MM -MT $(@:%.d=$(LIBOBJDIR)/%.o) >$@

########### objects #######################
-include $(DEP)

$(LIBOBJDIR)/%.o: $(LIBSRCDIR)/%$(EXT) $(LIBINC)
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -O3 -fPIC -o $@ -c $<

# UNIX-based OS variables & settings
RM = rm -f
DELOBJ = $(LIBOBJ)

################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DBGOBJ) $(DEP) lib$(LIBNAME).so lib$(DBGLIBNAME).so $(EXES)

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)
