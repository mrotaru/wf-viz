# !/usr/bin/python
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
                'src/utils.cpp',
                'src/globals.cpp',
                'src/BezierCurve.cpp',
                'src/Shape.cpp',
                'src/Group.cpp'
                ]

#----------------------------------------------------------------
main_cpp        = "src/main.cpp"    # used to run the application
test_runner_cpp = "tests/runner.cpp"  # used to run the tests

def options( opt ):
    opt.add_option('--check', action='store', default=False, help='compile test runners')
    
def configure( cnf ):
    cnf.load( 'g++' )
    print 'os: ' + sys.platform 

    # LINUX
    #--------------------------------------------------------------------------
    if sys.platform.startswith( 'linux' ):
        cnf.env.BOOST_PATH = "/usr/local/boost_1_49_0"

        # main program
        #----------------------------------------
        cnf.env.INCLUDES   = [ './include',
                               cnf.env.BOOST_PATH ]
        cnf.env.LINKFLAGS  = [ '-static-libgcc' ]
        cnf.env.LIB        = [ 'glut', 'GLU' ]
        cnf.env.STLIBPATH  = [ '/usr/local/boost_1_49_0/stage/lib' ]
        cnf.env.STLIB      = [ 'boost_regex' ]

        # for building the test runners
        #----------------------------------------
        cnf.env.TEST_STLIBPATH = "/usr/local/boost_1_49_0/stage/lib"
        cnf.env.TEST_STLIB = "boost_test_exec_monitor"
        cnf.env.TEST_INCLUDES = cnf.env.BOOST_PATH
    
    # WINDOWS
    #--------------------------------------------------------------------------
    elif sys.platform == 'win32' or sys.platform == 'cygwin':
        cnf.env.BOOST_PATH = "c:/pdev/boost_1_48_0"

        # main program
        #----------------------------------------
        cnf.env.INCLUDES   = [ './include',
                               './external/freeglut/2.6/mingw/include',
                               cnf.env.BOOST_PATH ]
        cnf.env.DEFINES    = [ 'FREEGLUT_STATIC' ]
        cnf.env.LINKFLAGS  = [ '-static-libgcc', '-static-libstdc++', '-W1,subsystem,windows' ]
        cnf.env.LIB        = [ 'freeglut_static', 'opengl32', 'gdi32', 'glu32', 'winmm' ]
        cnf.env.LIBPATH    = [ cnf.path.abspath() + '/external/freeglut/2.6/mingw/lib' ]
        cnf.env.STLIBPATH  = [ cnf.path.abspath() + '/libs/regex/gcc-mingw-4.6.1' ]
        cnf.env.STLIB      = [ 'boost_regex-gcc-1_48' ]

        # for building the test runners
        #----------------------------------------
        cnf.env.TEST_STLIBPATH = cnf.path.abspath() + "/libs/test/gcc-mingw-4.6.1/debug/link-static"
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
