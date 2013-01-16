# !/usr/bin/python
import os
import sys
from subprocess import *
from shutil import copy

#------------------------------------------------------------------------------
top = '.'
if sys.platform == 'linux2':
    out                = 'build/linux'
elif sys.platform == 'win32':
    out                = 'build/win32'

#------------------------------------------------------------------------------
sources_geometry =  [ 
                    'src/geometry/point.cc',
                    'src/geometry/line.cc',
                    'src/geometry/layer.cc',
                    'src/geometry/shape.cc',
                    'src/geometry/group.cc',
                    'src/geometry/bezier_curve.cc'
                    ]

sources_gui      =  [   
                    'src/GUI/control.cc',
                    'src/GUI/gui_utils.cc',
                    'src/GUI/window.cc',
                    'src/GUI/button.cc',
                    'src/GUI/label.cc',
                    'src/GUI/map_display.cc',
                    'src/GUI/checkbox.cc'
                    ]

sources_platform=   [ '' ] # will be filled when the project is configured

sources         =   [                    
                    'src/data_utils.cc',
                    'src/globals.cc',
                    'src/xml_data.cc',
                    ]

utils_cc        =   'src/utils.cc' # requires boost regex

main_cc         =   'src/main.cc'

test_runner_cc  =   'tests/runner.cc' # requires boost test

test_sources    =   [ 
                    'tests/testPoint.cc',
                    'tests/testLine.cc',
                    'tests/testUtils.cc'
                    ]

#------------------------------------------------------------------------------
def options( opt ):
    opt.add_option('--check', action='store', default=False, help='compile test runners')
    opt.add_option('--static_freeglut', action='store', default=False, help='link freeglut as a static lib')

#------------------------------------------------------------------------------
def configure( cnf ):
    cnf.load( 'g++' )

    abspath = cnf.path.abspath()

    cnf.env.BOOST_VER  = '1_52_0'
    cnf.env.GCC_VER    = '4.7.2'

    cnf.env.INCLUDES_MAIN           = [ './include' ]
    cnf.env.INCLUDES_SHAPELIB       = [ './deps/shapelib/include' ]
    cnf.env.INCLUDES_FREEGLUT       = [ './deps/freeglut/2.8/include' ]
    

    cnf.env.CXXFLAGS = [ '-c', '-g', '-O2', '-Wall', '-fpermissive', '-std=c++0x' ]
    
    # LINUX
    #--------------------------------------------------------------------------
    if sys.platform.startswith( 'linux' ):
        cnf.env.BOOST_PATH = '/media/Fatty/code/boost_1_52_0'

        cnf.check_cfg(package='gtkmm-3.0',
            args=['--cflags', '--libs'],
            msg="Checking for gtkmm-3.0")

        cnf.env.INCLUDES            = [ cnf.env.BOOST_PATH ]
        cnf.env.LINKFLAGS_MAIN      = [ '-static-libgcc' ]
        cnf.env.LIB_MAIN            = [ 'GLU' ]
        cnf.env.STLIB_MAIN          = [ 'boost_regex', 'glut', 'shp' ]
        cnf.env.STLIBPATH_MAIN      = [ abspath + '/lib/gcc-ubuntu',
                                        abspath + '/deps/shapelib/lib/gcc-ubuntu' ]

        # for building the test runners
        cnf.env.INCLUDES_TEST       = cnf.env.INCLUDES_MAIN
        cnf.env.LINKFLAGS_TEST      = cnf.env.LINKFLAGS_MAIN
        cnf.env.LIB_TEST            = cnf.env.LIB_MAIN + [ 'boost_unit_test_framework' ]
        cnf.env.DEFINES_TEST        = [ 'BOOST_TEST_DYN_LINK' ]
        #cnf.env.LIBPATH_TEST        = cnf.env.LIBPATH_MAIN + [ abspath + '/lib/gcc-ubuntu' ]
        cnf.env.STLIB_TEST          = cnf.env.STLIB_MAIN
        cnf.env.STLIBPATH_TEST      = cnf.env.STLIBPATH_MAIN

    # WINDOWS
    #--------------------------------------------------------------------------
    elif sys.platform == 'win32' or sys.platform == 'cygwin':
        cnf.env.BOOST_PATH = 'e:/code/boost_' + cnf.env.BOOST_VER
        suffix = '-mgw47-mt-1_52'

        # main program
        cnf.env.INCLUDES            = [ './include',
                                        './deps/freeglut/2.8/include',
                                        './deps/shapelib/include',
                                        cnf.env.BOOST_PATH ]

        # dynamic linking freeglut
        cnf.env.LIBPATH_FREEGLUT    = [ abspath + '/deps/freeglut/2.8/lib/gcc-mingw/bin' ]
        cnf.env.LIB_FREEGLUT        = [ 'freeglut', 'opengl32', 'glu32' ]

        # static linking freeglut
        if cnf.options.static_freeglut:
            cnf.env.STLIBPATH_FREEGLUT= [ abspath + '/deps/freeglut/2.8/lib/gcc-mingw/lib' ]
            cnf.env.STLIB_FREEGLUT  = [ 'freeglut_static' ]
            cnf.env.DEFINES_FREEGLUT= [ 'FREEGLUT_STATIC' ]

        cnf.env.STLIB_BOOST_REGEX   = [ 'boost_regex' + suffix ]

        # platform-specific stuff
        global sources_platform;
        sources_platform            = [ 'src/platform/win32.cc' ]

        cnf.env.LINKFLAGS_MAIN      = [ '-static-libgcc', '-static-libstdc++', '-Wl,--subsystem,windows' ]
        cnf.env.LIB_MAIN            = [ 'gdi32', 'winmm', 'comdlg32' ]
        cnf.env.STLIBPATH           = [ abspath + '/lib/gcc-mingw',
                                        abspath + '/deps/shapelib/lib/gcc-mingw',
                                        abspath + '/deps/freeglut/2.8/lib/gcc-mingw/lib' ]
        cnf.env.LIBPATH             = [ abspath + '/lib/gcc-mingw' ]

        # for building the test runners
        cnf.env.DEFINES_TEST        = [ 'BOOST_TEST_DYN_LINK' ]
        cnf.env.LIB_TEST            = [ 'boost_unit_test_framework' + suffix ]
        cnf.env.STLIB_TEST          = [ 'freeglut_static', 'boost_regex' + suffix, 'shp' ]
        cnf.env.STLIBPATH_TEST      = cnf.env.STLIBPATH_MAIN

#------------------------------------------------------------------------------
def build( bld ):

    # determine the name of the final executable
    #-------------------------------------------
    version_file_created = call( 'python tools/describe.py ', shell=True )
    if version_file_created == 0:
        bld.env.EXE_NAME = Popen( 'python tools/exe_name.py' , stdout=PIPE, stderr=PIPE, shell=True ).stdout.read().strip()
    else:
        bld.env.EXE_NAME = "wf-viz"

    # build platform-independent objects
    #-----------------------------------
    bld.objects(
            source      = sources,
            target      = 'objects',
            includes    = bld.env.INCLUDES_MAIN,
            defines     = bld.env.DEFINES_MAIN
            )
    bld.objects(
            source      = utils_cc,
            target      = 'utils',
            use         = 'BOOST_REGEX'
            )

    # declare shp as a static library
    #--------------------------------
    bld.read_stlib( 'shp', paths = bld.env.STLIBPATH )

    # build geometry static lib
    #-----------------------------------
    bld.shlib(
            source      = sources_geometry,
            target      = 'geometry',
            use         =  [ 'FREEGLUT', 'objects', 'utils' ]
            )

    # build GUI static lib
    #-----------------------------------
    bld.shlib(
            source      = sources_gui,
            target      = 'GUI',
            use         = [ 'FREEGLUT', 'objects', 'utils', 'shp' ]
            )


    # build platform-specific object, to be linked into the main program
    #-------------------------------------------------------------------
    bld.objects(
            source      = sources_platform,
            target      = 'platform'
            )

#    # build objects which require platform-dependant flags, and the main program
#    #---------------------------------------------------------------------------
#    if sys.platform.startswith( 'linux' ):

#        bld.objects(
#                source  = 'src/platform/linux-gtk.cpp',
#                target  = 'linux-gtk',
#                use     =  [ 'MAIN', 'GTKMM-3.0' ]
#                )

#        bld.program(
#                target      = bld.env.EXE_NAME,
#                features    = [ 'cxxprogram' ],
#                source      = main_cpp,
#                cxxflags    = gcc_flags,
#                use         = [ 'objects', 'linux-gtk', 'MAIN', 'GTKMM-3.0' ]
#                )

#    elif sys.platform == 'win32' or sys.platform == 'cygwin':

    bld.program(
            target      = bld.env.EXE_NAME,
            features    = [ 'cxxprogram' ],
            source      = main_cc,
            use         = [ 'objects', 'MAIN', 'geometry', 'GUI', 'platform' ]
            )

    if sys.platform == 'win32' or sys.platform == 'cygwin' and not bld.options.static_freeglut:
        copy( bld.env.LIBPATH_FREEGLUT[0] + '/freeglut.dll', out )

    # build test runner
    #---------------------------------------------------
    if( bld.options.check ):
        print 'building tests...'
        bld.program(
                target      = 'runner',
                features    = [ 'cxxprogram' ],
                source      = [ test_runner_cc ] + test_sources,
                use         = [ 'objects','linux-gtkmm', 'MAIN', 'TEST', 'GTKMM-3.0' ]
                )

        if sys.platform == 'win32' or sys.platform == 'cygwin':
            copy( bld.env.LIBPATH_TEST[0] + '/libboost_unit_test_framework-mgw47-mt-1_52.dll', out )

    if version_file_created == 0:
        copy( 'VERSION', out )
        call( 'python tools/append_date.py ' + out + '/VERSION', shell=True )

# vim:filetype=python
