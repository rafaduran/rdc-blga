#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`~pyblga.data.fileinout` --- Very short description
======================================
Long description

.. module:: pyblga.data.fileinout
    :synopsis: Short description

.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import sys
import os
import re
sys.path.append(os.path.abspath(os.path.join(os.path.abspath(__file__), '..', 
                                             '..','..')))
import pyblga.data.apis.results as results
try:
    import simplejson as json
except ImportError:
    import json
    
def import_results(filename):
    with open(filename,'rt') as json_file:
        reAPI = results.ResultsAPI()
        return reAPI.create({'data': json.loads(json_file.read())})
    return None


def parse_args(filename):
    tokens = filename.split('_')[1:] # Removing 'msls'
    ls_name = tokens[0]
    pattern = re.compile(r"\d+\.?\d*")
    args = {}
    for arg in tokens[1:]:
        index = pattern.search(arg).start()
        args[arg[:index]] = arg[index:]
    return ls_name, args


if __name__ == '__main__':
    print parse_args('msls_blga_itCalls10_N100_A7_m5_pamn5_rtsn14')
    print parse_args('msls_blga_itCalls10_N500_A7_m5_pamn5_rtsn14')
    print parse_args(
        'msls_ClusteredClearing_nOff10_N500_A7_m5_pamn5_rtsn14_clRarius0.1')