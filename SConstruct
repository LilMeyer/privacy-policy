import glob
import os
import sys

import buildscripts.lint

class colors:
  BOLD      = '\033[1m'
  ENDC      = '\033[0m'
  FAIL      = '\033[91m'
  HEADER    = '\033[95m'
  OKBLUE    = '\033[94m'
  OKGREEN   = '\033[92m'
  UNDERLINE = '\033[4m'
  WARNING   = '\033[93m'

def print_line(title, content):
  print colors.BOLD + title + ': ' + colors.ENDC + content

def print_info():
  color = '\033[95m'
  print colors.BOLD + '---Info---' + colors.ENDC
  print_line('Build mode'  , build_mode)
  print_line('CXX compiler', env['CXX'])
  print_line('CC compiler' , env['CC'])
  print_line('CCFLAGS'     , ', '.join(env['CCFLAGS']))
  print_line('Libs'        , ', '.join(env['libs']))
  print_line('CPPDEFINES'  , ', '.join(env['CPPDEFINES']))
  print_line('CPPPATH'     , ', '.join(env['CPPPATH']))

# --- lint ----
def doLint(env, target, source ):
  if not buildscripts.lint.run_lint( [ 'src/core/' ] ):
    raise Exception( 'lint errors' )

taget_dir = 'build'

# build mode (debug/release; default to release)
build_mode = ARGUMENTS.get('mode', 'release')

if build_mode not in ['debug', 'release']:
    sys.stderr.write('Oh noes: only "debug" and "release" build modes are supported\n')
    Exit(1)


# C++ flags and defines
ccflags = [
    '-std=c++11',
    '-Wall',
    '-pedantic-errors',
    '-Wfatal-errors' # stop at the first error
]

cppdefines = []

if build_mode == 'debug':
    ccflags += ['-g', '-O0']
    cppdefines += ['_DEBUG']
elif build_mode == 'release':
    ccflags += ['-O2']
    cppdefines += ['NDEBUG']

# this is to allow colorgcc
custom_env = {
    'PATH': os.environ['PATH'],
    'TERM': os.environ['TERM'],
    'HOME': os.environ['HOME'],
}

# create root environment
env = Environment(CCFLAGS=ccflags,
                       CPPDEFINES=cppdefines,
                       ENV=custom_env,
                       CPPPATH=['#/include'])

# custom C++ compiler
if 'CXX' in os.environ:
    env['CXX'] = os.environ['CXX']

# Why do I have to do that ?
env['CC'] = 'g++'


env['libs'] = [
  'cppunit',
  'boost_system'
]

Export('env', 'build_mode')


env.VariantDir('build/release/src', 'src', duplicate=0)

# library and tests
lib = SConscript('build/release/src/SConscript', { 'env': env })

print_info()

env.Alias('lint', [], [ doLint ])
env.AlwaysBuild('lint')
