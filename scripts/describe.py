# !/bin/env/python

# looks at the source code ( using git ) and creates the VERSION file;
# the VERSION file has two lines:
#   - first line: a short description ( e.g: V0.1 )
#   - second line: a more complete description ( might be the same as the
#     first line, but might also be something like V0.1-1-dirty )

import os
from subprocess import *

# look for executable, and return full path
# from: [ http://stackoverflow.com/questions/377017/test-if-executable-exists-in-python ]
def which(program):
    import os
    def is_exe(fpath):
        return os.path.exists(fpath) and os.access(fpath, os.X_OK)

    fpath, fname = os.path.split(program)
    if fpath:
        if is_exe(program):
            return program
    else:
        for path in os.environ["PATH"].split(os.pathsep):
            exe_file = os.path.join(path, program)
            if is_exe(exe_file):
                return exe_file

    return None

if not which( 'git' ):
    pass
else:
    ver_short = Popen( "git describe --tags --abbrev=0" , stdout=PIPE, stderr=PIPE ).stdout.read().strip()
    ver_full = Popen( "git describe --tags --abbrev=7 --dirty" , stdout=PIPE, stderr=PIPE ).stdout.read().strip()

    # create VERSION
    ver_file = open( "VERSION", 'w' )
    ver_file.write( ver_short + '\n' )
    ver_file.write( ver_full + '\n')
