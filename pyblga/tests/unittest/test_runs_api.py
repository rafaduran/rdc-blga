#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`~pyblga.tests.unittest.test_runs_api` --- Tests for runs API
=====================================================================
Long description

.. module:: pyblga.tests.unittest.test_runs_api
    :synopsis: Tests for runs API

.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.abspath(__file__), '..',
                                             '..', '..','..')))
import unittest2 as unittest
import pyblga.data.apis.runs as runs
import pyblga.data.apis.results as results
import pyblga.data.apis.searchers as searchers
import pyblga.data.apis.parameters as params
import pyblga.common.exception as error

class TestRunsAPI(unittest.TestCase):
    """
    Tests for :py:class:`pyblga.data.models.runs.RunsAPI()`
    """


    def setUp(self):
        self.api = runs.RunsAPI() 


    def tearDown(self):
        del self.api

    
    def test_get(self):
        run = self.api.get(1)
        self.assertEqual(run.searcher_id, 1)
        self.assertEqual(run.result_id, 1)
        
    
    def test_get_fail(self):
        run = self.api.get(9999)
        self.assertEqual(run, None)
        
    
    def test_get_data(self):
        run = self.api.get_runs_data(3)
        self.assertEqual(run.searcher_id, 3)
        self.assertEqual(run.searcher_id, 3)
        self.assertEqual(run.searcher.name, 'ClusteredClearing')
        self.assertEqual(len(run.params), 2)
        for index, assoc in enumerate(run.params, 2):
            self.assertEqual(index, assoc.param_id)
            
            
    def test_get_result(self):
        result = self.api.get_result(1)
        self.assertEqual(result.result_id, 1)

        
    def test_get_all(self):
        runs = self.api.get_all()
        self.assertEqual(len(runs), 3)
        
        for index, run in enumerate(runs):
            self.assertEqual(index+1, run.run_id)
            
            
    def test_get_param_by_name(self):
        assoc = self.api.get_param_by_name(1, 'popSize')
        self.assertEqual(assoc.param.name, 'popSize')
        self.assertEqual(assoc.param.value, 500)
        
        
    def test_get_param_by_name_non_exists(self):
        param = self.api.get_param_by_name(1, 'non_exists')
        self.assertEqual(param, None)
        
        
    def test_create_delete(self):
        searcher = searchers.SearchersAPI()
        nse = searcher.create({'name': 'test'})
        result = results.ResultsAPI()
        nre = result.create({'data': {'2': '0.33'}})
        run = self.api.create({'searcher_id': nse.searcher_id ,
                               'result_id': nre.result_id})
        self.assertEqual(run.searcher_id, nse.searcher_id)
        self.assertEqual(run.result_id, nre.result_id)
        _ = self.api.get(run.run_id)
        self.api.delete(run.run_id)
        
        
    def test_create_fail_wrong_searcher_result(self):
        searcher_api = searchers.SearchersAPI()
        nse = searcher_api.create({'name': 'test'})
        result_api = results.ResultsAPI()
        nre = result_api.create({'data': {'2': '0.33'}})
        
        for searcher, result in \
            ((9999, nre.result_id), (nse.searcher_id, 9999), (1, nre.result_id),
             (nse.searcher_id, 1)):
            try:
                self.api.create({'result_id': result, 'searcher_id': searcher})
            except error.ModelsAPIError:
                pass
            else:
                self.fail("No exception launched after inserting")
            
        result_api.delete(nre.result_id)
        searcher_api.delete(nse.searcher_id)
    
    
    def test_add_param(self):
        param_api = params.ParamsAPI()
        param = param_api.create({'name': 'test', 'value': 0.33})
        searcher = searchers.SearchersAPI()
        nse = searcher.create({'name': 'test'})
        result = results.ResultsAPI()
        nre = result.create({'data': {'2': '0.33'}})
        run = self.api.create({'searcher_id': nse.searcher_id ,
                               'result_id': nre.result_id})
        self.api.add_param({'run_id': run.run_id, 'param_id': param.param_id})
        data = self.api.get_runs_data(run.run_id)
        found = False
        for par in data.params:
            if par.param.param_id == param.param_id:
                found = True
        self.assert_(found, "Not found added param")
        self.api.delete(run.run_id)
        param_api.delete(param.param_id)           
    
    
    def test_add_param_fail(self):
        searcher = searchers.SearchersAPI()
        nse = searcher.create({'name': 'test'})
        result = results.ResultsAPI()
        nre = result.create({'data': {'2': '0.33'}})
        run = self.api.create({'searcher_id': nse.searcher_id ,
                               'result_id': nre.result_id})
        try:
            self.api.add_param({'run_id': run.run_id, 'param_id': 9999})
        except error.ModelsAPIError:
            pass
        else:
            self.fail("No exception after adding parameter")
        self.api.delete(run.run_id)
        
    
    def test_add_param_fail_already_exists(self):
        run = self.api.get(1)
        try:
            self.api.add_param({'run_id': run.run_id, 'param_id': 3})
        except error.ModelsAPIError:
            pass
        else:
            self.fail("No exception after adding parameter")
    
                
    def test_delete_fail(self):
        try:
            self.api.delete(9999)
        except error.ModelsAPIError:
            pass
        else:
            self.fail("No exception launched after trying deletion")
        
        
    def test_update(self):
        searcher = searchers.SearchersAPI()
        nse = searcher.create({'name': 'test'})
        nse2 = searcher.create({'name': 'test2'})
        result = results.ResultsAPI()
        nre = result.create({'data': {'2': '0.33'}})
        run = self.api.create({'searcher_id': nse.searcher_id ,
                               'result_id': nre.result_id})
        run.searcher_id = nse2.searcher_id
        self.api.update(run)
        run_again = self.api.get(run.run_id)
        self.assertEqual(run_again.searcher_id, nse2.searcher_id)
        self.api.delete(run.run_id)
        searcher.delete(nse.searcher_id)
        
    
    def test_update_fail_missing(self):
        searcher = searchers.SearchersAPI()
        nse = searcher.create({'name': 'test'})
        result = results.ResultsAPI()
        nre = result.create({'data': {'2': '0.33'}})
        run = self.api.create({'searcher_id': nse.searcher_id ,
                               'result_id': nre.result_id})
        run.searcher_id = 9999
        try:
            self.api.update(run)
        except error.ModelsAPIError:
            pass
        else:
            self.fail("No exception after trying update")
        self.api.delete(run.run_id)
        
        
    def test_update_fail_exists(self):
        searcher = searchers.SearchersAPI()
        nse = searcher.create({'name': 'test'})
        result = results.ResultsAPI()
        nre = result.create({'data': {'2': '0.33'}})
        run = self.api.create({'searcher_id': nse.searcher_id ,
                               'result_id': nre.result_id})
        run.searcher_id = 1
        try:
            self.api.update(run)
        except error.ModelsAPIError:
            pass
        else:
            self.fail("No exception after trying update")
        self.api.delete(run.run_id)
        
        
if __name__ == "__main__":
    unittest.main()