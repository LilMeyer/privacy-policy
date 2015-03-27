all:
	scons
	./build/main

include Makefile_tests

policy:
	scons policy_test
	./policy_test
	dot actorsHierarchyClosure.dot -Tpng -o image.png
	dot objectsHierarchyClosure.dot -Tpng -o image.png

clean:
	rm -rf build

generate_tests:
	python generate_tests.py

dot:
	dot tc-out.dot -Tpng -o image.png

test: all_tests

.PHONY: all
