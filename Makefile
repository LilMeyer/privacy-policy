all:
	scons
	./build/main

include Makefile_tests

policy:
	scons policy_test
	./policy_test
	dot build/actorsHierarchyClosure.dot -Tpng -o build/actorsHierarchyClosure.png
	dot build/objectsHierarchyClosure.dot -Tpng -o build/objectsHierarchyClosure.png

travis:
	scons mode=debug && make generate_tests && make run_tests

clean:
	rm -f *_test
	rm -f main
	rm -rf build

generate_tests:
	python generate_tests.py

test: generate_tests all_tests

.PHONY: all
