SRC_DIRS = src src/cmdline src/disasm src/interp src/output
TEST_DIRS = tests

# Name of the executable here. If your system expects an extension
# (e.g., .exe on Windows), put it on the EXEEXT line.
EXEEXT =
PROGRAM = pep8$(EXEEXT)

# Source code listing and executable naming. To add another file
# for the project, remove the "# " at the beginning of the line and
# change the name from another.c to the name of your source file.
# All names should be relative to the src/ directory, so if your
# file is src/foo/bar.c, then you should refer to it as foo/bar.c.
# Note that you would also need to add src/foo to the SRC_DIRS list.
project_SRC  = main.c
project_SRC += cmdline/parse.c
project_SRC += output/print.c
project_SRC += disasm/disasm.c

#####################################################################
# Make no edits below this line unless you know what you are doing! #
#####################################################################

# These lines set up the names of objects based on the source file
# names, and specify the directories to create in
SOURCES = $(addprefix src/,$(project_SRC))
OBJECTS = $(sort $(addprefix build/,$(patsubst %.c,%.o,$(SOURCES))))
BUILD_DIRS = $(sort $(addprefix build/,$(SRC_DIRS) $(TEST_DIRS)))

all: $(BUILD_DIRS) all-prog

%.test: tests/Makefile
	cd tests && $(MAKE) $@

$(BUILD_DIRS):
	mkdir -p $@

clean:
	-test -z "$(PROGRAM)" || rm -f $(PROGRAM)
	rm -rf build
	rm -f tests/*.output

PWDDIR = $(shell pwd)

# Compiler option specifications. Do NOT modify this part unless
# you know what you are doing.
SHELL = /bin/sh
VPATH = $(SRCDIR)

# Specify the compiler and linker
CC = gcc
CCLD = $(CC)
LINK = $(CCLD)

ifeq ($(strip $(shell command -v $(CC) 2> /dev/null)),)
  $(warning *** Compiler ($(CC)) not found. Please contact instructor ***)
endif

# Compiler flags and warnings specification here.
WARNINGS = -Wall -Werror
CFLAGS = -g -O2
CPPFLAGS =
LDFLAGS =

# Tell make how to compile .c files into object files
build/%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $(WARNINGS) -c $< -o $@

# Specify how to build the final executable. This depends on first
# building all the objects. Once they are all compiled, we link
# them together into a single executable.
$(PROGRAM): $(OBJECTS)
	@rm -f $(PROGRAM)
	$(CCLD) -o $@ $(OBJECTS)

all-prog: $(PROGRAM)
