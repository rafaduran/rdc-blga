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


# TODO: all this tasks should be done into a single transaction    
def import_results(filename):
    with open(filename,'rt') as json_file:
        reAPI = results.ResultsAPI()
        job = json.loads(json_file.read())
        args = job['params']
        del job['params']
        result = reAPI.create({'data': job})
        ls_name = args['lsname']
        del args['lsname']
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


def export_results(filename, run_id):
    ruAPI = runs.RunsAPI()
    data = ruAPI.get_runs_data(run_id)
    params = {}
    for assoc in data.params:
        params[assoc.param.name] = assoc.param.value
    params['lsname'] = data.searcher.name
    with open(filename,'wt') as json_file:
        data.results.data['params'] = params
        json_file.write(json.dumps(data.result.data))
        return 1    # Success
    return 0        # Fail



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


if __name__ == '__main__':
#    print parse_args('msls_blga_itCalls10_N100_A7_m5_pamn5_rtsn14')
#    print parse_args('msls_blga_itCalls10_N500_A7_m5_pamn5_rtsn14')
#    print parse_args(
#        'msls_ClusteredClearing_nOff10_N500_A7_m5_pamn5_rtsn14_clRarius0.1')
    print import_results('/home/rdc/projects/rdc-blga/msls_ClusteredClearing_nOff10_N500_A7_m5_pamn5_rtsn14_clRarius0.1/Hump function')