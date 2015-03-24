include Makefile_tests

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


.PHONY: all main money_test bimap_test graph_test
