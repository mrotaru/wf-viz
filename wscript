# !/bin/env/python
import os
import sys
from subprocess import *
from shutil import copy

top = '.'
if sys.platform == 'linux2':
    out                = 'waf-build-linux'
elif sys.platform == 'win32':
    out                = 'waf-build-win32'

def options( opt ):
    pass

def configure( cnf ):
    cnf.load( 'g++' )
    cnf.env.EXE_NAME = Popen( "python scripts/exe_name.py" , stdout=PIPE, stderr=PIPE, shell=True ).stdout.read().strip()

    #-------------------------------------------------------------------- LINUX
    if sys.platform.startswith( 'linux' ):
        print 'os: ' + sys.platform 
        cnf.env.INCLUDES   = [ './include' ]
        cnf.env.LINKFLAGS  = [ '-static-libgcc' ]
        cnf.env.LIB        = [ 'glut', 'GLU' ]
    
    #------------------------------------------------------------------ WINDOWS
    elif sys.platform == 'win32' or sys.platform == 'cygwin':
        print 'os: ' + sys.platform 
        cnf.env.INCLUDES   = [ './include',
                               './freeglut/include' ]
        cnf.env.DEFINES    = [ 'FREEGLUT_STATIC' ]
        cnf.env.LINKFLAGS  = [ '-static-libgcc', '-static-libstdc++', '-W1,subsystem,windows' ]
        cnf.env.LIB        = [ 'freeglut_static', 'opengl32', 'gdi32', 'glu32', 'winmm' ]
        cnf.env.LIBPATH    = [ cnf.path.abspath() + '/freeglut/lib' ]

def build( bld ):
    bld.program(
            target      = bld.env.EXE_NAME,
            features    = [ 'cxxprogram' ],
            source      = [ 'src/main.cpp',
                            'src/Point.cpp',
                            'src/Line.cpp' ],
            includes    = bld.env.INCLUDES,
            defines     = bld.env.DEFINES,
            lib         = bld.env.LIB,
            libpath     = bld.env.LIBPATH,
            linkflags   = bld.env.LINKFLAGS,
            cxxflags    = [ '-c', '-g', '-O2', '-Wall' ]
            )
    copy( 'VERSION', out )        
    call( 'python scripts/append_date.py ' + out + '/VERSION', shell=True )

# vim:filetype=python
