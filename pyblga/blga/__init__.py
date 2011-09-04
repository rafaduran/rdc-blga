from __future__ import unicode_literals
from __future__ import print_function
from future_builtins import * #@UnusedWildImport pylint:disable=W0614

import os
import subprocess

top_dir = os.path.join(os.path.dirname(__file__),'../../')

if not os.path.exists("".join((top_dir,'executables/blga.out'))):
    if subprocess.call(''.join(['cd ',top_dir, ' && make install']), 
                       shell=True):
        raise IOError('Blga neither found, nor build from Makefile')