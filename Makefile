all:
	scons
	./build/main

include Makefile_tests

policy:
	scons policy_test
	./policy_test
	dot build/actorsHierarchyClosure.dot -Tpng -o build/actorsHierarchyClosure.png
	dot build/objectsHierarchyClosure.dot -Tpng -o build/objectsHierarchyClosure.png

clean:
	rm -rf build

generate_tests:
	python generate_tests.py

dot:
	dot tc-out.dot -Tpng -o image.png

test: all_tests

.PHONY: all
