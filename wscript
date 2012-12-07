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
                    'src/data_utils.cpp',
                    'src/xml_data.cpp'
                    ]
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
    abspath = cnf.path.abspath()

    # LINUX
    #--------------------------------------------------------------------------
    if sys.platform.startswith( 'linux' ):
        cnf.env.BOOST_PATH = '/media/Fatty/code/boost_1_52_0'

        cnf.check_cfg(package='gtkmm-3.0',
            args=['--cflags', '--libs'],
            msg="Checking for gtkmm-3.0")

        # main program
        cnf.env.INCLUDES   = [ './include',
                               './deps/freeglut/2.8/include',
                               './deps/shapelib/include',
                               cnf.env.BOOST_PATH ]
        cnf.env.LINKFLAGS  = [ '-static-libgcc' ]
        cnf.env.LIB        = [ 'GLU' ]
        cnf.env.STLIB      = [ 'boost_regex', 'glut', 'shp' ]
        cnf.env.STLIBPATH  = [ abspath + '/lib/gcc-ubuntu',
                               abspath + '/deps/shapelib/lib/gcc-ubuntu' ]

        # for building the test runners
        cnf.env.TEST_STLIBPATH = abspath + '/lib/gcc-ubuntu'
        cnf.env.TEST_STLIB = 'boost_unit_test_framework'
        cnf.env.TEST_INCLUDES = cnf.env.BOOST_PATH
    
    # WINDOWS
    #--------------------------------------------------------------------------
    elif sys.platform == 'win32' or sys.platform == 'cygwin':
        cnf.env.BOOST_VER  = '1_52_0'
        cnf.env.GCC_VER    = '4.7.2'
        cnf.env.BOOST_PATH = 'e:/code/boost_' + cnf.env.BOOST_VER
        suffix = '-mgw47-mt-1_52'

        sources.append( 'src/platform/win32.cpp')

        # main program
        cnf.env.INCLUDES   = [ './include',
                               './deps/freeglut/2.8/include',
                               './deps/shapelib/include',
                               cnf.env.BOOST_PATH ]
        cnf.env.DEFINES    = [ 'FREEGLUT_STATIC' ]
        cnf.env.LINKFLAGS  = [ '-static-libgcc', '-static-libstdc++', '-Wl,--subsystem,windows' ]
        cnf.env.LIB        = [ 'freeglut_static', 'opengl32', 'gdi32', 'glu32', 'winmm', 'comdlg32' ]
        cnf.env.LIBPATH    = [ abspath + '/deps/freeglut/2.8/lib/gcc-mingw' ]
        cnf.env.STLIBPATH  = [ abspath + '/lib/gcc-mingw',
                               abspath + '/deps/shapelib/lib/gcc-mingw',
                               abspath + '/deps/freeglut/2.8/lib/gcc-mingw/lib' ]
        cnf.env.STLIB      = [ 'boost_regex' + suffix, 'shp' ]

        # for building the test runners
        cnf.env.TEST_LIBPATH = abspath + '/lib/gcc-mingw'
        cnf.env.TEST_LIB = 'boost_unit_test_framework' + suffix
        cnf.env.TEST_INCLUDES = cnf.env.BOOST_PATH

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
            includes    = bld.env.INCLUDES,
            defines     = bld.env.DEFINES,
            cxxflags    = gcc_flags
            )

    # build objects which require platform-dependant flags, and the main program
    #---------------------------------------------------------------------------
    if sys.platform.startswith( 'linux' ):

        bld.objects(
                source  = 'src/platform/linux-gtk.cpp',
                target  = 'linux-gtk',
                includes = bld.env.INCLUDES + bld.env[ 'INCLUDES_GTKMM-3.0' ],
                cxxflags = gcc_flags + bld.env[ 'CXXFLAGS_GTKMM-3.0' ]
                )

        bld.program(
                target      = bld.env.EXE_NAME,
                features    = [ 'cxxprogram' ],
                includes    = bld.env.INCLUDES + bld.env[ 'INCLUDES_GTKMM-3.0' ],
                source      = main_cpp,
                defines     = bld.env.DEFINES,
                lib         = bld.env.LIB + bld.env[ 'LIB_GTKMM-3.0' ],
                libpath     = bld.env.LIBPATH,
                linkflags   = bld.env.LINKFLAGS + bld.env[ 'LINKFLAGS_GTKMM-3.0' ],
                cxxflags    = gcc_flags,
                use         = [ 'objects', 'linux-gtk' ]
                )

    elif sys.platform == 'win32' or sys.platform == 'cygwin':

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
                use         = [ 'objects' ]
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
                lib         = bld.env.LIB,
                libpath     = bld.env.LIBPATH,
                linkflags   = bld.env.LINKFLAGS,
                cxxflags    = gcc_flags,
                use         = 'objects'
                )
        #copy( bld.env.TEST_LIBPATH + '/lib' + bld.env.TEST_LIB + '.dll', out )

    if version_file_created == 0:
        copy( 'VERSION', out )        
        call( 'python tools/append_date.py ' + out + '/VERSION', shell=True )

# vim:filetype=python
