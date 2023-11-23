########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++17 -Wall -Wno-write-strings  -Wno-sign-compare
LDFLAGS = -lgvc -lcgraph -lcdt

# Makefile settings - Can be customized.
APPNAME = app
LIBNAME = libgraph.so
EXT = .cc
SRCDIR = src
OBJDIR = obj

############## Do not change anything from here downwards! #############
SRC := $(shell find $(SRCDIR) -type f -name '*$(EXT)')
OBJ := $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.o)
DEP := $(SRC:$(SRCDIR)/%$(EXT)=$(OBJDIR)/%.d)
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
# @echo $(SRC)
# @echo $(OBJ)

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ -I$(SRCDIR) main.cc $^ $(LDFLAGS)
# $(info CREATED $(APPNAME))

$(LIBNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -shared -o $@ -I$(SRCDIR) $^ $(LDFLAGS)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%$(EXT)
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -fPIC -o $@ -I$(SRCDIR) -c $<

# Creates the dependecy rules
$(OBJDIR)/%.d: $(SRCDIR)/%$(EXT)
	@$(CPP) $(CFLAGS) $< -MM -MT $(@:%.d=$(OBJDIR)/%.o) >$@

# Includes all .h files
# -include $(DEP)



################### Cleaning rules for Unix-based OS ###################
# Cleans complete project
.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(DEP) $(APPNAME) $(LIBNAME)

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