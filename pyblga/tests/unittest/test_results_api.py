#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`~pyblga.tests.unittest.test_results_api` --- Very short description
======================================
Long description

.. module:: pyblga.tests.unittest.test_results_api
    :synopsis: Short description

.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.abspath(__file__), '..',
                                             '..', '..','..')))
import unittest2 as unittest
import pyblga.data.apis.results as results
import pyblga.common.exception as error

class TestResultsAPI(unittest.TestCase):
    """
    Tests for :py:class:`pyblga.data.models.results.ResultsAPI()`
    """


    def setUp(self):
        self.api = results.ResultsAPI() 


    def tearDown(self):
        del self.api

    
    def test_get(self):
        result = self.api.get(1)
        self.assertEqual(result.result_id, 1)
        self.assertDictEqual(result.data, {'0': '0.99'})
        
    
    def test_get_fail(self):
        result = self.api.get(9999)
        self.assertEqual(result, None)
        
    
    def test_get_data(self):
        result = self.api.get_data(3)
        self.assertDictEqual(result.data, {'2': '0.33'})

        
    def test_get_all(self):
        results = self.api.get_all()
        self.assertEqual(len(results), 3)
        
        for index, result in enumerate(results):
            self.assertEqual(index+1, result.result_id)
        
        
    def test_create_delete(self):
        result = self.api.create({'data':{'0': '1.0'}})
        self.assertDictEqual(result.data, {'0': '1.0'})
        result_again = self.api.get(4)
        self.assertDictEqual(result.data, result_again.data)
        self.api.delete(4)
        
            
    def test_delete_fail(self):
        try:
            self.api.delete(9999)
        except error.ModelsAPIError:
            pass
        else:
            self.fail("No exception launched after trying deletion")
        
        
    def test_update(self):
        result = self.api.create({'data': {'1': '0'}})
        result.data = {'1': '99'}
        self.api.update(result)
        updated = self.api.get(4)
        self.assertDictEqual(updated.data, {'1': '99'})
        self.api.delete(4)
        
        
if __name__ == "__main__":
    unittest.main()
    