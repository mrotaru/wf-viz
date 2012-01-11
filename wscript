# !/bin/env/python
import os
import sys
from subprocess import *
from shutil import copy

# select where to build
#---------------------------------------------
top = '.'
if sys.platform == 'linux2':
    out                = 'build/waf/linux'
elif sys.platform == 'win32':
    out                = 'build/waf/win32'

# source files, without the main.cpp or test runners
#---------------------------------------------------
sources     = [ 'src/Point.cpp',
                'src/Line.cpp',
                'src/utils.cpp' ]

#----------------------------------------------------------------
main_cpp        = "src/main.cpp"    # used to run the application
test_runner_cpp = "tests/runner.cpp"  # used to run the tests

def options( opt ):
    opt.add_option('--check', action='store', default=False, help='compile test runners')
    
def configure( cnf ):
    cnf.load( 'g++' )

    # LINUX
    #--------------------------------------------------------------------------
    if sys.platform.startswith( 'linux' ):
        print 'os: ' + sys.platform 
        cnf.env.INCLUDES   = [ './include' ]
        cnf.env.LINKFLAGS  = [ '-static-libgcc' ]
        cnf.env.LIB        = [ 'glut', 'GLU' ]
    
    # WINDOWS
    #--------------------------------------------------------------------------
    elif sys.platform == 'win32' or sys.platform == 'cygwin':
        print 'os: ' + sys.platform 

        # main program
        #----------------------------------------
        cnf.env.INCLUDES   = [ './include',
                               './external/freeglut/2.6/mingw/include' ]
        cnf.env.DEFINES    = [ 'FREEGLUT_STATIC' ]
        cnf.env.LINKFLAGS  = [ '-static-libgcc', '-static-libstdc++', '-W1,subsystem,windows' ]
        cnf.env.LIB        = [ 'freeglut_static', 'opengl32', 'gdi32', 'glu32', 'winmm' ]
        cnf.env.LIBPATH    = [ cnf.path.abspath() + '/external/freeglut/2.6/mingw/lib' ]

        # for building the test runners
        #----------------------------------------
        cnf.env.BOOST_PATH ="c:/pdev/boost_1_48_0"
        cnf.env.TEST_STLIBPATH = cnf.env.BOOST_PATH + "/bin.v2/libs/test/build/gcc-mingw-4.6.1/debug/link-static"
        cnf.env.TEST_STLIB = "boost_test_exec_monitor-mgw46-d-1_48"
        cnf.env.TEST_INCLUDES = cnf.env.BOOST_PATH

def build( bld ):
    call( 'python scripts/describe.py ', shell=True )
    bld.env.EXE_NAME = Popen( "python scripts/exe_name.py" , stdout=PIPE, stderr=PIPE, shell=True ).stdout.read().strip()
    
    # build main program
    #---------------------------------------------------
    bld.program(
            target      = bld.env.EXE_NAME,
            features    = [ 'cxxprogram' ],
            includes    = bld.env.INCLUDES,
            source      = sources + [ main_cpp ],
            defines     = bld.env.DEFINES,
            lib         = bld.env.LIB,
            libpath     = bld.env.LIBPATH,
            linkflags   = bld.env.LINKFLAGS,
            cxxflags    = [ '-c', '-g', '-O2', '-Wall' ]
            )

    # build test runners
    #---------------------------------------------------
    if( bld.options.check ):
        print 'building tests...'
        bld.program(
                target      = 'runner',
                features    = [ 'cxxprogram' ],
                includes    = bld.env.TEST_INCLUDES,
                source      = sources + [ test_runner_cpp ],
                stlibpath   = bld.env.TEST_STLIBPATH,
                stlib       = bld.env.TEST_STLIB,
                linkflags   = bld.env.LINKFLAGS,
                cxxflags    = [ '-c', '-g', '-O2', '-Wall' ]
                )

    copy( 'VERSION', out )        
    call( 'python scripts/append_date.py ' + out + '/VERSION', shell=True )

# vim:filetype=python
