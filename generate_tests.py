import glob

makefileTests = file('Makefile_tests', 'w')
files = glob.glob('src/core/*_test.cpp')
str = ''
all = ''
run_all = '\t'

bin_path = 'build/release/src/bin/'

for file in files:
  f = file[9:-4]
  all = all + ' ' + f
  str = str + f + ':\n\t'
  str = str + 'scons ' + bin_path + f + '\n\t'
  str = str + './' + bin_path + f + '\n\n'
  run_all = run_all + './' + bin_path + f + '\n\t'
str = str + 'all_tests:' + all + '\n\n'
str = str + 'run_all: make_all\n'
str = str + run_all + '\n'

str = str + '.PHONY:' + all + '\n'

makefileTests.write(str)
