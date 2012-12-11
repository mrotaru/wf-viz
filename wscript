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
                    'src/gui_utils.cpp',
                    'src/control.cpp',
                    'src/window.cpp',
                    'src/button.cpp',
                    'src/label.cpp',
                    'src/mapdisplay.cpp',
                    'src/checkbox.cpp',
                    'src/data_utils.cpp',
                    'src/xml_data.cpp'
                    ]
main_cpp        =   'src/main.cpp'
test_runner_cpp =   'tests/runner.cpp'
test_sources    = [ 'tests/testPoint.cpp',
                    'tests/testLine.cpp',
                    'tests/testUtils.cpp'
                    ]

#------------------------------------------------------------------------------
def options( opt ):
    opt.add_option('--check', action='store', default=False, help='compile test runners')

#------------------------------------------------------------------------------
def configure( cnf ):
    cnf.load( 'g++' )

    print 'os: ' + sys.platform
    abspath = cnf.path.abspath()

    # LINUX
    #--------------------------------------------------------------------------
    if sys.platform.startswith( 'linux' ):
        cnf.env.BOOST_PATH = '/media/Fatty/code/boost_1_52_0'

        cnf.check_cfg(package='gtkmm-3.0',
            args=['--cflags', '--libs'],
            msg="Checking for gtkmm-3.0")

        # main program
        cnf.env.INCLUDES_MAIN   = [ './include',
                                    './deps/freeglut/2.8/include',
                                    './deps/shapelib/include',
                                    cnf.env.BOOST_PATH ]
        cnf.env.LINKFLAGS_MAIN  = [ '-static-libgcc' ]
        cnf.env.LIB_MAIN        = [ 'GLU' ]
        cnf.env.STLIB_MAIN      = [ 'boost_regex', 'glut', 'shp' ]
        cnf.env.STLIBPATH_MAIN  = [ abspath + '/lib/gcc-ubuntu',
                                    abspath + '/deps/shapelib/lib/gcc-ubuntu' ]
        cnf.env.CXXFLAGS_MAIN   = [ '-c', '-g', '-O2', '-Wall', '-fpermissive', '-std=c++11' ]

        # for building the test runners
        cnf.env.INCLUDES_TEST   = cnf.env.INCLUDES_MAIN
        cnf.env.LINKFLAGS_TEST  = cnf.env.LINKFLAGS_MAIN
        cnf.env.LIB_TEST        = cnf.env.LIB_MAIN + [ 'boost_unit_test_framework' ]
        cnf.env.DEFINES_TEST    = [ 'BOOST_TEST_DYN_LINK' ]
        #cnf.env.LIBPATH_TEST    = cnf.env.LIBPATH_MAIN + [ abspath + '/lib/gcc-ubuntu' ]
        cnf.env.STLIB_TEST      = cnf.env.STLIB_MAIN
        cnf.env.STLIBPATH_TEST  = cnf.env.STLIBPATH_MAIN

    # WINDOWS
    #--------------------------------------------------------------------------
    elif sys.platform == 'win32' or sys.platform == 'cygwin':
        cnf.env.BOOST_VER  = '1_52_0'
        cnf.env.GCC_VER    = '4.7.2'
        cnf.env.BOOST_PATH = 'e:/code/boost_' + cnf.env.BOOST_VER
        suffix = '-mgw47-mt-1_52'

        sources.append( 'src/platform/win32.cpp')

        # main program
        cnf.env.INCLUDES_MAIN   = [ './include',
                                    './deps/freeglut/2.8/include',
                                    './deps/shapelib/include',
                                    cnf.env.BOOST_PATH ]
        cnf.env.DEFINES_MAIN    = [ 'FREEGLUT_STATIC' ]
        cnf.env.LINKFLAGS_MAIN  = [ '-static-libgcc', '-static-libstdc++', '-Wl,--subsystem,windows' ]
        cnf.env.LIB_MAIN        = [ 'opengl32', 'gdi32', 'glu32', 'winmm', 'comdlg32' ]
        cnf.env.LIBPATH_MAIN    = [ abspath + '/deps/freeglut/2.8/lib/gcc-mingw' ]
        cnf.env.STLIB_MAIN      = [ 'freeglut_static', 'boost_regex' + suffix, 'shp' ]
        cnf.env.STLIBPATH_MAIN  = [ abspath + '/lib/gcc-mingw',
                                    abspath + '/deps/shapelib/lib/gcc-mingw',
                                    abspath + '/deps/freeglut/2.8/lib/gcc-mingw/lib' ]

        # for building the test runners
        cnf.env.DEFINES_TEST    = [ 'BOOST_TEST_DYN_LINK' ]
        cnf.env.LIBPATH_TEST    = [ abspath + '/lib/gcc-mingw' ]
        cnf.env.LIB_TEST        = cnf.env.LIB_MAIN + [ 'boost_unit_test_framework' + suffix ]
        cnf.env.STLIB_TEST      = [ 'freeglut_static', 'boost_regex' + suffix, 'shp' ]
        cnf.env.STLIBPATH_TEST  = cnf.env.STLIBPATH_MAIN

#------------------------------------------------------------------------------
def build( bld ):
    version_file_created = call( 'python tools/describe.py ', shell=True )
    if version_file_created == 0:
        bld.env.EXE_NAME = Popen( 'python tools/exe_name.py' , stdout=PIPE, stderr=PIPE, shell=True ).stdout.read().strip()
    else:
        bld.env.EXE_NAME = "wf-viz"

    gcc_flags = [ '-c', '-g', '-O2', '-Wall', '-fpermissive', '-std=c++0x' ]

    # build platform-independent objects
    #---------------------------------------------------
    bld.objects(
            source      = sources,
            target      = 'objects',
            includes    = bld.env.INCLUDES_MAIN,
            defines     = bld.env.DEFINES_MAIN,
            cxxflags    = gcc_flags
            )

    # build objects which require platform-dependant flags, and the main program
    #---------------------------------------------------------------------------
    if sys.platform.startswith( 'linux' ):

        bld.objects(
                source  = 'src/platform/linux-gtk.cpp',
                target  = 'linux-gtk',
                use     =  [ 'MAIN', 'GTKMM-3.0' ]
                )

        bld.program(
                target      = bld.env.EXE_NAME,
                features    = [ 'cxxprogram' ],
                source      = main_cpp,
                cxxflags    = gcc_flags,
                use         = [ 'objects', 'linux-gtk', 'MAIN', 'GTKMM-3.0' ]
                )

    elif sys.platform == 'win32' or sys.platform == 'cygwin':

        bld.program(
                target      = bld.env.EXE_NAME,
                features    = [ 'cxxprogram' ],
                source      = main_cpp,
                cxxflags    = gcc_flags,
                use         = [ 'objects', 'MAIN' ]
                )

    # build test runner
    #---------------------------------------------------
    if( bld.options.check ):
        print 'building tests...'
        bld.program(
                target      = 'runner',
                features    = [ 'cxxprogram' ],
                source      = [ test_runner_cpp ] + test_sources,
                use         = [ 'objects','linux-gtkmm', 'MAIN', 'TEST', 'GTKMM-3.0' ]
                )

    if sys.platform == 'win32' or sys.platform == 'cygwin':
        copy( bld.env.LIBPATH_TEST[0] + '/libboost_unit_test_framework-mgw47-mt-1_52.dll', out )

    if version_file_created == 0:
        copy( 'VERSION', out )
        call( 'python tools/append_date.py ' + out + '/VERSION', shell=True )

# vim:filetype=python
