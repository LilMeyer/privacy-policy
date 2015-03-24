
testFile = file('tests.txt', 'r')
makefileTests = file('Makefile_tests', 'w')

s = 'init'
while(s != EOF):
  s = testFile.readline()
  print s
  s = s.replace('\n', '')
  s = s + '_test'
  str = s + ':\n\tscons build/' + s + '\n\t./build/' + s
  # makefileTests.write(str)

print str
