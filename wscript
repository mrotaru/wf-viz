# !/bin/env/python
import os

top = '.'
out = 'build'

def options( opt ):
#    opt.load( 'compiler_cxx')
    pass

def configure( cnf ):
    cnf.load( 'g++')

def build( bld ):
    a_path = bld.path.abspath()
    bld.program(
            target      = 'wfb',
            features    = [ 'cxxprogram' ],
            source      = [ 'src/main.cpp',
                            'src/Point.cpp',
                            'src/Line.cpp' ],
            includes    = [ './include',
                            './freeglut/include' ],
            defines     = [ 'FREEGLUT_STATIC' ],
            lib         = [ 'freeglut_static', 'opengl32', 'gdi32', 'glu32', 'winmm' ],
            libpath     = [ a_path + '/freeglut/lib' ],
            linkflags   = [ '-static-libgcc', '-static-libstdc++', '-W1,subsystem,windows' ],
            cxxflags    = [ '-c', '-g', '-O2', '-Wall' ]
            )

# vim:filetype=python
