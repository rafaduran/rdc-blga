#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`~pyblga.tests.unittest.sampledata` --- Very short description
======================================
Long description

.. module:: pyblga.tests.unittest.sampledata
    :synopsis: Short description

.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.abspath(__file__), '..',
                                             '..', '..')))
import pyblga.data.apis as apis

def load_data():
    result_model = apis.results.ResultsAPI()
    for data in ({"0":{"1":[{"fitness":1.0,"variable":{"0": 0.9}},{"fitness":0.5,"variable":{"0": 0.7}}]}},
                {"0":{"1":[{"fitness":0.10,"variable":{"0": 0.6}},{"fitness":0.3,"variable":{"0": 0.07}}]}},
                {"0":{"1":[{"fitness":0.99999999,"variable":{"0": 0.5}},{"fitness":1,"variable":{"0": 0.33}}]}},
                 ):
        result_model.create({'data': data})
    
    searcher_model = apis.searchers.SearchersAPI()
    for name in ('Blga', 'Blga', 'ClusteredClearing'):
        if name == 'ClusteredClearing':
            searcher_model.create({'name': name, 'tag': 'tag'})
        else:
            searcher_model.create({'name': name})
        
    param_model = apis.parameters.ParamsAPI()      
    for name, value in (('popSize', 500), ('pamnass', 5), ('popSize', 300)):
        param_model.create({'name': name, 'value': value})
    
    run_model = apis.runs.RunsAPI()
    for index in range(1,4):
        run_model.create({'searcher_id': index ,'result_id': index})
        if index == 3:
            start, end = 2, 4
        else:
            start, end = 1, 3
        for param in range(start,end):
            run_model.add_param({'run_id': index, 'param_id': param})
            
    
        
if __name__ == '__main__':
    load_data()