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

try:
    import simplejson as json
except ImportError:
    import json

import pyblga.data.apis.results as results
import pyblga.data.apis.searchers as searchers
import pyblga.data.apis.parameters as params
import pyblga.data.apis.runs as runs
import pyblga.common.exception as error
#import pyblga.data

    
def import_results(filename):
    with open(filename,'rt') as json_file:
        reAPI = results.ResultsAPI()
        result = reAPI.create({'data': json.loads(json_file.read())})
        ls_name, args = parse_args(parse_dir(filename))
        param_list = create_parameters(args)
        seAPI = searchers.SearchersAPI()
        searcher = seAPI.create({'name': ls_name})
        ruAPI = runs.RunsAPI()
        run = ruAPI.create({'searcher_id': searcher.searcher_id, 
                      'result_id': result.result_id})
        for param in param_list:
            ruAPI.add_param({'run_id': run.run_id,
                             'param_id': param.param_id})
        return run
    return None


def create_parameters(args):
    paAPI = params.ParamsAPI()
    param_list = []
    for name, value in args.items():
        try:
            para = paAPI.create({'name': name, 'value': value})
        except error.ModelsAPIError:
            para = paAPI.get_id(name, value)
        finally:
            param_list.append(para)
    return param_list


def parse_dir(filename):
    return os.path.abspath(filename).split(os.sep)[-2]
    

def parse_args(string):
    tokens = string.split('_')[1:] # Removing 'msls'
    ls_name = tokens[0]
    pattern = re.compile(r"\d+\.?\d*")
    args = {}
    for arg in tokens[1:]:
        index = pattern.search(arg).start()
        args[arg[:index]] = arg[index:]
    return ls_name, args


if __name__ == '__main__':
#    print parse_args('msls_blga_itCalls10_N100_A7_m5_pamn5_rtsn14')
#    print parse_args('msls_blga_itCalls10_N500_A7_m5_pamn5_rtsn14')
#    print parse_args(
#        'msls_ClusteredClearing_nOff10_N500_A7_m5_pamn5_rtsn14_clRarius0.1')
    print import_results('/home/rdc/projects/rdc-blga/msls_ClusteredClearing_nOff10_N500_A7_m5_pamn5_rtsn14_clRarius0.1/Hump function')