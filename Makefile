include Makefile_tests

make_all:
	scons

hierarchy:
	scons hierarchy_test
	./hierarchy_test
	dot src/examples/actors.dot -Tpng -o src/examples/actors.png
	dot src/examples/actors1.dot -Tpng -o src/examples/actors1.png
	dot src/examples/actors1_f2.dot -Tpng -o src/examples/actors1_f2.png


policy:
	scons policy_case1_test
	./policy_case1_test
	dot src/tests/case1/actors.dot -Tpng -o src/tests/case1/actors.png
	dot src/tests/case1/objects.dot -Tpng -o src/tests/case1/objects.png

travis:
	scons mode=debug && make generate_tests && make run_all

clean:
	rm -rf build
	rm -f *_test
	rm -f src/core/*.o

generate_tests:
	python generate_tests.py

test: generate_tests all_tests

.PHONY: all policy
