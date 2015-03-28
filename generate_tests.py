import glob

makefileTests = file('Makefile_tests', 'w')
files = glob.glob('src/core/*_test.cpp')
str = ''
all = ''
run = ''

for file in files:
  f = file[9:-4]
  all = all + ' ' + f
  run = run + '\n\t' + './' + f
  str = str + f + ':\n\tscons ' + f + '\n\t./' + f + '\n'
str = str + '\nall_tests:' + all + '\n\n'
str = str + 'run_tests:' + run + '\n\n'
str = str + '.PHONY:' + all + '\n'

makefileTests.write(str)
