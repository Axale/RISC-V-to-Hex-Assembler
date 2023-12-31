# Makefile for RISC-V Assembler
top_level = assembler
test_level = test
libraries = $(source_path)filein.cpp $(source_path)translator.cpp
main_target = assembler.o
test_target = test.o
include_path = include/
source_path = source/

top_level_file = $(top_level).cpp
test_file = $(test_level).cpp
top_level_path = $(source_path)$(top_level_file)
test_path = $(source_path)$(test_file)

.PHONY: clean

assembler: $(top_level_path) $(libraries)
	g++ $(top_level_path) $(libraries) -o $(main_target)

assembler-debug: $(top_level_path) $(libraries)
	g++ $(top_level_path) $(libraries) -o $(main_target) -g

clean:
	rm -rf $(main_target) $(test_target) meminit.hex

test: $(test_path) $(libraries)
	g++ $(test_path) $(libraries) -o $(test_target)

test-run: $(test_path) $(libraries)
	g++ $(test_path) $(libraries) -o $(test_target) -g
	./$(test_target)

