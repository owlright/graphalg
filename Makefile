########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++17 -Wall -Wno-write-strings  -Wno-sign-compare
LDFLAGS = -lgvc -lcgraph -lcdt
DBGCXXFLAGS = $(CXXFLAGS) -g3 -O0 -DDEBUG
# Makefile settings - Can be customized.
APPNAME = app
LIBNAME = libgraph.so
DBGLIBNAME = libgraph_dbg.so
EXT = .cc
SRCDIR = src
OBJDIR = obj

############## Do not change anything from here downwards! #############
SRC := $(shell find $(SRCDIR) -type f -name '*$(EXT)')
OBJ := $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP := $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.d)
DBGOBJ := $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%_dbg.o)
# OBJ := $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.o,$(SRC))
# DEP := $(patsubst $(SRCDIR)/%.cc,$(OBJDIR)/%.d,$(SRC))
# UNIX-based OS variables & settings
RM = rm -f
DELOBJ = $(OBJ)
# Windows OS variables & settings
DEL = del
EXE = .exe
WDELOBJ = $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)\\%.o)

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME) $(LIBNAME)

debug: $(APPNAME)_dbg $(DBGLIBNAME)

$(APPNAME)_dbg: $(DBGOBJ) main.cc
	$(CC) $(DBGCXXFLAGS) -o $@ -I$(SRCDIR) $^ $(LDFLAGS)

$(DBGLIBNAME): $(DBGOBJ)
	$(CC) $(DBGCXXFLAGS) -shared -o $@ -I$(SRCDIR) $^ $(LDFLAGS)

# Builds the app
$(APPNAME): $(OBJ) main.cc
	$(CC) $(CXXFLAGS) -o $@ -I$(SRCDIR) $^ $(LDFLAGS)
# $(info CREATED $(APPNAME))

$(LIBNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -shared -o $@ -I$(SRCDIR) $^ $(LDFLAGS)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -fPIC -o $@ -I$(SRCDIR) -c $<

$(OBJDIR)/%_dbg.o: $(SRCDIR)/%$(EXT)
	@mkdir -p $(@D)
	$(CC) $(DBGCXXFLAGS) -fPIC -o $@ -I$(SRCDIR) -c $<

# Creates the dependecy rules
$(OBJDIR)/%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

# Includes all .h files
# -include $(DEP)



################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DBGOBJ) $(DEP) $(APPNAME) $(APPNAME)_dbg $(LIBNAME) $(DBGLIBNAME)

# Cleans only all files with the extension .d
.PHONY: cleandep
cleandep:
	$(RM) $(DEP)

#################### Cleaning rules for Windows OS #####################
# Cleans complete project
.PHONY: cleanw
cleanw:
	$(DEL) $(WDELOBJ) $(DEP) $(APPNAME)$(EXE)

# Cleans only all files with the extension .d
.PHONY: cleandepw
cleandepw:
	$(DEL) $(DEP)