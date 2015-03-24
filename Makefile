# include Makefile_tests

all:
	scons
	./build/main

# compile_main:
# 	scons main

clean:
	rm -rf build

generate_tests:
	python generate_tests.py

main:
	scons
	./build/main

money_test:
	scons build/money_test
	./build/money_test

bimap_test:
	scons build/bimap_test
	./build/bimap_test

graph_test:
	scons build/graph_test
	./build/graph_test

graph_test2:
	scons build/graph_test2
	./build/graph_test2

graph_test3:
	scons build/graph_test3
	./build/graph_test3

.PHONY: all main money_test bimap_test graph_test
