# !/bin/env/python

# Will append the current date and time to a file passed as the first ( and
# only ) parameter.  Intended to be called by build systems ( make, waf or VS )
# to append the date to the VERSION file in the build directory.

import sys
from datetime import datetime

with open( sys.argv[1], "a" ) as version_file:
    version_file.write( datetime.today().strftime("%a, %d %b %Y, %H:%M %p\r\n") ) 
