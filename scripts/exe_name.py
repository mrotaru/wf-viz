# !/bin/env/python

# This script should only be run by the build tools, after creating the
# VERSION file.

from subprocess import *
from datetime import date
import os

exe_name_base = 'wf-viz_' + Popen( 'head -n 1 VERSION' , stdout=PIPE, stderr=PIPE, shell=True ).stdout.read().strip()
print exe_name_base
