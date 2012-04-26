# !/usr/bin/python
import os
import sys
from subprocess import *
from shutil import copy

#------------------------------------------------------------------------------
top = '.'
if sys.platform == 'linux2':
    out                = 'build/waf/linux'
elif sys.platform == 'win32':
    out                = 'build/waf/win32'

#------------------------------------------------------------------------------
sources         = [ 'src/Point.cpp',
                    'src/Line.cpp',
                    'src/utils.cpp',
                    'src/globals.cpp',
#                    'src/BezierCurve.cpp',
#                    'src/Shape.cpp',
#                    'src/Group.cpp',
                    'src/gui_utils.cpp',
                    'src/control.cpp',
                    'src/window.cpp',
                    'src/button.cpp',
                    'src/label.cpp',
                    'src/mapdisplay.cpp',
                    'src/checkbox.cpp',
                    'src/platform/win32.cpp' ]
main_cpp        =   'src/main.cpp'
test_runner_cpp =   'tests/runner.cpp'
test_sources    = [ 'tests/testPoint.cpp',
                    'tests/testLine.cpp',
                    'tests/testUtils.cpp' ]

#------------------------------------------------------------------------------
def options( opt ):
    opt.add_option('--check', action='store', default=False, help='compile test runners')
    
#------------------------------------------------------------------------------
def configure( cnf ):
    cnf.load( 'g++' )
    
    print 'os: ' + sys.platform 

    # LINUX
    #--------------------------------------------------------------------------
    if sys.platform.startswith( 'linux' ):
        cnf.env.BOOST_PATH = '/usr/local/boost_1_49_0'

        # main program
        cnf.env.INCLUDES   = [ './include',
                               cnf.env.BOOST_PATH ]
        cnf.env.LINKFLAGS  = [ '-static-libgcc' ]
        cnf.env.LIB        = [ 'glut', 'GLU' ]
        cnf.env.STLIBPATH  = [ '/usr/local/boost_1_49_0/stage/lib' ]
        cnf.env.STLIB      = [ 'boost_regex' ]

        # for building the test runners
        cnf.env.TEST_LIBPATH = '/usr/local/boost_1_49_0/stage/lib'
        cnf.env.TEST_LIB = 'boost_unit_test_framework'
        cnf.env.TEST_INCLUDES = cnf.env.BOOST_PATH
    
    # WINDOWS
    #--------------------------------------------------------------------------
    elif sys.platform == 'win32' or sys.platform == 'cygwin':
        cnf.env.BOOST_PATH = 'c:/pdev/boost_1_49_0'

        # main program
        cnf.env.INCLUDES   = [ './include',
                               './external/freeglut/2.8/mingw/include',
                               './external/shapelib/mingw/include',
                               cnf.env.BOOST_PATH ]
        cnf.env.DEFINES    = [ 'FREEGLUT_STATIC' ]
        cnf.env.LINKFLAGS  = [ '-static-libgcc', '-static-libstdc++', '-W1,subsystem,windows' ]
        cnf.env.LIB        = [ 'freeglut_static', 'opengl32', 'gdi32', 'glu32', 'winmm', 'comdlg32' ]
        cnf.env.LIBPATH    = [ cnf.path.abspath() + '/external/freeglut/2.8/mingw/lib' ]
        cnf.env.STLIBPATH  = [ cnf.path.abspath() + '/libs/win32/gcc-mingw-4.6.2',
                               cnf.path.abspath() + '/external/shapelib/mingw/lib' ]
        cnf.env.STLIB      = [ 'boost_regex-mgw46-1_49', 'shp' ]

        # for building the test runners
        cnf.env.TEST_LIBPATH = cnf.path.abspath() + '/libs/win32/gcc-mingw-4.6.2'
        cnf.env.TEST_LIB = 'boost_unit_test_framework-mgw46-1_49'
        cnf.env.TEST_INCLUDES = cnf.env.BOOST_PATH

#------------------------------------------------------------------------------
def build( bld ):
    version_file_created = call( 'python scripts/describe.py ', shell=True )
    if version_file_created == 0: 
        bld.env.EXE_NAME = Popen( 'python scripts/exe_name.py' , stdout=PIPE, stderr=PIPE, shell=True ).stdout.read().strip()
    else:
        bld.env.EXE_NAME = "wf-viz"
    
    gcc_flags = [ '-c', '-g', '-O2', '-Wall', '-fpermissive', '-std=c++0x' ]

    # build objects
    #---------------------------------------------------
    bld.objects(
            source      = sources,
            target      = 'objects',
            includes    = bld.env.INCLUDES,
            defines     = bld.env.DEFINES,
            cxxflags    = gcc_flags
            )

    # build main program
    #---------------------------------------------------
    bld.program(
            target      = bld.env.EXE_NAME,
            features    = [ 'cxxprogram' ],
            includes    = bld.env.INCLUDES,
            source      = main_cpp,
            defines     = bld.env.DEFINES,
            lib         = bld.env.LIB,
            libpath     = bld.env.LIBPATH,
            linkflags   = bld.env.LINKFLAGS,
            cxxflags    = gcc_flags,
            use         = 'objects'
            )

    # build test runner
    #---------------------------------------------------
    if( bld.options.check ):
        print 'building tests...'
        bld.program(
                target      = 'runner',
                features    = [ 'cxxprogram' ],
                includes    = bld.env.TEST_INCLUDES,
                source      = [ test_runner_cpp ] + test_sources,
                libpath     = bld.env.TEST_LIBPATH,
                lib         = bld.env.TEST_LIB,
                linkflags   = bld.env.LINKFLAGS,
                cxxflags    = gcc_flags,
                use         = 'objects'
                )
        copy( bld.env.TEST_LIBPATH + '/lib' + bld.env.TEST_LIB + '.dll', out )

    if version_file_created == 0:
        copy( 'VERSION', out )        
        call( 'python scripts/append_date.py ' + out + '/VERSION', shell=True )

# vim:filetype=python
