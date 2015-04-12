include Makefile_tests

make_all:
	scons

policy:
	scons build/release/src/bin/policy_test
	./build/release/src/bin/policy_test
	# dot build/actorsHierarchyClosure.dot -Tpng -o build/actorsHierarchyClosure.png
	# dot build/objectsHierarchyClosure.dot -Tpng -o build/objectsHierarchyClosure.png
	# dot build/actorsHierarchyClosureReverse.dot -Tpng -o build/actorsHierarchyClosureReverse.png
	# dot build/actorsGraphviz.dot -Tpng -o build/actorsGraphviz.png
	# dot build/objectsGraphviz.dot -Tpng -o build/objectsGraphviz.png


travis:
	scons mode=debug && make generate_tests && make run_all

clean:
	rm -rf build

generate_tests:
	python generate_tests.py

test: generate_tests all_tests

.PHONY: all policy
