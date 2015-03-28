import glob

makefileTests = file('Makefile_tests', 'w')
files = glob.glob('src/core/*_test.cpp')
str = ''
all = ''

for file in files:
  f = file[9:-4]
  all = all + ' ' + f
  str = str + f + ':\n\tscons ' + f + '\n\t./' + f + '\n'
str = str + '\nall_tests:' + all + '\n\n'
str = str + '.PHONY:' + all + '\n'

makefileTests.write(str)
