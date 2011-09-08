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
    for result in ({'0': '0.99'}, {'1': '0.5'}, {'2': '0.33'}):
        result_model = apis.results.ResultsAPI()
        result_model.create({'result': result})
        
    for name in ('Blga', 'Blga', 'ClusteredClearing'):
        searcher_model = apis.searchers.SearchersAPI()
        searcher_model.create({'name': name})
        
    for name, value in (('popSize', 500), ('popSize', 300), ('pamnass', 5)):
        param_model = apis.parameters.ParamsAPI()  
        param_model.create({'name': name, 'value': value})
        
    for index in range(1,4):
        run_model = apis.runs.RunsAPI()
        run_model.create({'searcher_id': index ,'result_id': index})
        for param in range(1,4):
            run_model.add_param({'run_id': index, 'param_id': param})
        
if __name__ == '__main__':
    load_data()