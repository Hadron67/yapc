# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cfy/programs/C/yapc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cfy/programs/C/yapc

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/cfy/programs/C/yapc/CMakeFiles /home/cfy/programs/C/yapc/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/cfy/programs/C/yapc/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named yapc

# Build rule for target.
yapc: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 yapc
.PHONY : yapc

# fast build rule for target.
yapc/fast:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/build
.PHONY : yapc/fast

code_gen.o: code_gen.c.o

.PHONY : code_gen.o

# target to build an object file
code_gen.c.o:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/code_gen.c.o
.PHONY : code_gen.c.o

code_gen.i: code_gen.c.i

.PHONY : code_gen.i

# target to preprocess a source file
code_gen.c.i:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/code_gen.c.i
.PHONY : code_gen.c.i

code_gen.s: code_gen.c.s

.PHONY : code_gen.s

# target to generate assembly for a file
code_gen.c.s:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/code_gen.c.s
.PHONY : code_gen.c.s

gen.o: gen.c.o

.PHONY : gen.o

# target to build an object file
gen.c.o:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/gen.c.o
.PHONY : gen.c.o

gen.i: gen.c.i

.PHONY : gen.i

# target to preprocess a source file
gen.c.i:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/gen.c.i
.PHONY : gen.c.i

gen.s: gen.c.s

.PHONY : gen.s

# target to generate assembly for a file
gen.c.s:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/gen.c.s
.PHONY : gen.c.s

grammar.o: grammar.c.o

.PHONY : grammar.o

# target to build an object file
grammar.c.o:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/grammar.c.o
.PHONY : grammar.c.o

grammar.i: grammar.c.i

.PHONY : grammar.i

# target to preprocess a source file
grammar.c.i:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/grammar.c.i
.PHONY : grammar.c.i

grammar.s: grammar.c.s

.PHONY : grammar.s

# target to generate assembly for a file
grammar.c.s:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/grammar.c.s
.PHONY : grammar.c.s

grammar_builder.o: grammar_builder.c.o

.PHONY : grammar_builder.o

# target to build an object file
grammar_builder.c.o:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/grammar_builder.c.o
.PHONY : grammar_builder.c.o

grammar_builder.i: grammar_builder.c.i

.PHONY : grammar_builder.i

# target to preprocess a source file
grammar_builder.c.i:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/grammar_builder.c.i
.PHONY : grammar_builder.c.i

grammar_builder.s: grammar_builder.c.s

.PHONY : grammar_builder.s

# target to generate assembly for a file
grammar_builder.c.s:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/grammar_builder.c.s
.PHONY : grammar_builder.c.s

main.o: main.c.o

.PHONY : main.o

# target to build an object file
main.c.o:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/main.c.o
.PHONY : main.c.o

main.i: main.c.i

.PHONY : main.i

# target to preprocess a source file
main.c.i:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/main.c.i
.PHONY : main.c.i

main.s: main.c.s

.PHONY : main.s

# target to generate assembly for a file
main.c.s:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/main.c.s
.PHONY : main.c.s

parser.o: parser.c.o

.PHONY : parser.o

# target to build an object file
parser.c.o:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/parser.c.o
.PHONY : parser.c.o

parser.i: parser.c.i

.PHONY : parser.i

# target to preprocess a source file
parser.c.i:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/parser.c.i
.PHONY : parser.c.i

parser.s: parser.c.s

.PHONY : parser.s

# target to generate assembly for a file
parser.c.s:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/parser.c.s
.PHONY : parser.c.s

spool.o: spool.c.o

.PHONY : spool.o

# target to build an object file
spool.c.o:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/spool.c.o
.PHONY : spool.c.o

spool.i: spool.c.i

.PHONY : spool.i

# target to preprocess a source file
spool.c.i:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/spool.c.i
.PHONY : spool.c.i

spool.s: spool.c.s

.PHONY : spool.s

# target to generate assembly for a file
spool.c.s:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/spool.c.s
.PHONY : spool.c.s

yapc.o: yapc.c.o

.PHONY : yapc.o

# target to build an object file
yapc.c.o:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/yapc.c.o
.PHONY : yapc.c.o

yapc.i: yapc.c.i

.PHONY : yapc.i

# target to preprocess a source file
yapc.c.i:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/yapc.c.i
.PHONY : yapc.c.i

yapc.s: yapc.c.s

.PHONY : yapc.s

# target to generate assembly for a file
yapc.c.s:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/yapc.c.s
.PHONY : yapc.c.s

ytest.o: ytest.c.o

.PHONY : ytest.o

# target to build an object file
ytest.c.o:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/ytest.c.o
.PHONY : ytest.c.o

ytest.i: ytest.c.i

.PHONY : ytest.i

# target to preprocess a source file
ytest.c.i:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/ytest.c.i
.PHONY : ytest.c.i

ytest.s: ytest.c.s

.PHONY : ytest.s

# target to generate assembly for a file
ytest.c.s:
	$(MAKE) -f CMakeFiles/yapc.dir/build.make CMakeFiles/yapc.dir/ytest.c.s
.PHONY : ytest.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... yapc"
	@echo "... code_gen.o"
	@echo "... code_gen.i"
	@echo "... code_gen.s"
	@echo "... gen.o"
	@echo "... gen.i"
	@echo "... gen.s"
	@echo "... grammar.o"
	@echo "... grammar.i"
	@echo "... grammar.s"
	@echo "... grammar_builder.o"
	@echo "... grammar_builder.i"
	@echo "... grammar_builder.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... parser.o"
	@echo "... parser.i"
	@echo "... parser.s"
	@echo "... spool.o"
	@echo "... spool.i"
	@echo "... spool.s"
	@echo "... yapc.o"
	@echo "... yapc.i"
	@echo "... yapc.s"
	@echo "... ytest.o"
	@echo "... ytest.i"
	@echo "... ytest.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
