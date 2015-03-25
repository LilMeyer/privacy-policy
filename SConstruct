import os
import sys
import glob

taget_dir = "build"

# build mode (debug/release; default to release)
build_mode = ARGUMENTS.get('mode', 'release')

if build_mode not in ['debug', 'release']:
    sys.stderr.write('Oh noes: only "debug" and "release" build modes are supported\n')
    Exit(1)

# C++ flags and defines
ccflags = [
    '-std=c++11',
    '-Wall',
    '-pedantic-errors'
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

Export('env', 'build_mode')

# library and tests
lib = SConscript(os.path.join('src', 'SConscript'))
# tests = SConscript(os.path.join('tests', 'SConscript'))
# Depends(tests, lib)

libs = [
    'cppunit',
    'boost_system'
]

files = glob.glob('src/core/*.cpp')


for file in files:
    print file
    print file[9:-4]
    t = env.Program(target=file[9:-4], source=[file], LIBS=libs)

# env.Program(target=os.path.join(taget_dir, 'main'), source=['src/core/main.cpp'], LIBS=libs)
# env.Program(target=os.path.join(taget_dir, 'bimap_test'), source=['src/core/bimap_test.cpp'], LIBS=libs)
# env.Program(target=os.path.join(taget_dir, 'graph1_test'), source=['src/core/graph1_test.cpp'], LIBS=libs)
# env.Program(target=os.path.join(taget_dir, 'graph2_test'), source=['src/core/graph2_test.cpp'], LIBS=libs)
# env.Program(target=os.path.join(taget_dir, 'graph3_test'), source=['src/core/graph3_test.cpp'], LIBS=libs)
# env.Program(target=os.path.join(taget_dir, 'graph4_test'), source=['src/core/graph4_test.cpp'], LIBS=libs)
# env.Program(target=os.path.join(taget_dir, 'graph4_test'), source=['src/core/policy_test.cpp'], LIBS=libs)
