
testsFile = file('tests.txt', 'r')
makefileTests = file('Makefile_tests', 'w')

str = ''
all = ''
for line in testsFile:
  s = line.rstrip()
  s = s.replace('\n', '')
  s = s + '_test'
  all += ' ' + s
  str = str + s + ':\n\tscons build/' + s + '\n\t./build/' + s + '\n'

str += 'all_tests:' + all + '\n'

makefileTests.write(str)

print 'Finished'
