all:
	scons
	./build/main

include Makefile_tests

clean:
	rm -rf build

generate_tests:
	python generate_tests.py

dot:
	dot tc-out.dot -Tpng -o image.png

.PHONY: all
