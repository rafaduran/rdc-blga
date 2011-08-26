#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`blga.runner` --- Runs Blga
===================================
This module is doing dummy python integration, since right now it just runs Blga
using a `subprocess` moudule

.. module:: blga.runner 
    :synopsis: Short description
    
.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
from __future__ import unicode_literals
from __future__ import absolute_import
from __future__ import print_function
from future_builtins import *

import subprocess
import os
import logging

import pyblga.common.exception as error

blga_path = os.path.join(os.path.dirname(__file__),'../../executables/blga.out')

@error.parsing_error_wrapper
def get_args(kwargs):
    args = []
    name = kwargs['name']
    args.append(name)
    if name == 'blga':
        args.append(kwargs['itC'])
        
    elif name == 'ClusteredClearing':
        args.append(kwargs['nOff'])
    else:
        raise ValueError('Unknown localSearch')

    args.append(kwargs['blgaPopSize'])
    if 'alfa' in kwargs:
        args.append(kwargs['alfa'])
    elif 'r' in kwargs:
        args.append(kwargs['r'])
    else:
        raise ValueError('Missing arguments')
    
    args.append(kwargs['numMates'])
    args.append(kwargs['pamnass'])
    args.append(kwargs['rtsnass'])
    
    if name == 'ClusteredClearing':
        args.append(kwargs['clRadius'])
        
    if kwargs['functionNumber'] not in ('0', '1' ,'2'):
        raise ValueError("Unknown fitness function")
    else:
        args.append(kwargs['functionNumber'])
    
    args.append(kwargs['path'])
        
    if 'f' in kwargs and not 'n' in kwargs:
        args.append('f' + kwargs['f'])
    elif 'n' in kwargs and not 'f' in kwargs:
        args.append('n' + kwargs['n'])
    else:
        raise ValueError('Unknown stop criteria')
    
    if kwargs['initRun'] >= kwargs['endRun']:
        raise ValueError('initRun must be lower than endRun')
    
    args.append(kwargs['initRun'])
    args.append(kwargs['endRun'])
    
    return True, args


def run(*args, **kwargs):
    """
    Runs blga with given args
    Args:
        <localSearch> <functionNumber> <path> <stopc> <initRun> <numRuns>
        <localSearch>:  
            Blga <itC> <blgaPopSize> (<alfa>|r<pf>) <numMates> <pamnass> <rtsnass>
            ClusterdClearing <nOff> <blgaPopSize> (<alfa>|r<pf>) <numMates> <pamnass> <rtsnass> <clRadius>
        <stopC>: (f<fitness>|n<numEvaluations>)
    Returns
        return code from blga running
    """
    if not args:
        ok, args = get_args(kwargs)
        if not ok:
            return 1, args
    call_args = [blga_path,]
    [call_args.append(arg) for arg in args]
    logging.debug(call_args)
    blga = subprocess.Popen(call_args, bufsize=-1, stderr=subprocess.STDOUT,
                            stdout=subprocess.PIPE)
    out, err = blga.communicate()
    logging.debug(out)
    logging.debug(err)
    return blga.returncode, out


if __name__ == '__main__':
    run()
