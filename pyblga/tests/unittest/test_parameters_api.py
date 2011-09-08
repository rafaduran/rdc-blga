#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`~pyblga.tests.unittest.test_parameters_api` --- Very short description
======================================
Long description

.. module:: pyblga.tests.unittest.test_parameters_api
    :synopsis: Short description

.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
""" 
import unittest2 as unittest
import pyblga.data.apis.parameters as parameters
import pyblga.common.exception as error

class TestParametersAPI(unittest.TestCase):
    """
    Tests for :py:class:`pyblga.data.models.parameters.ParametersAPI()`
    """


    def setUp(self):
        self.api = parameters.ParamsAPI() 


    def tearDown(self):
        del self.api

    
    def test_get(self):
        parameter = self.api.get(1)
        self.assertEqual(parameter.param_id, 1)
        self.assertEqual(parameter.name, 'popSize')
        
    
    def test_get_fail(self):
        parameter = self.api.get(9999)
        self.assertEqual(parameter, None)

        
    def test_get_all(self):
        parameters = self.api.get_all()
        self.assertEqual(len(parameters), 3)
        
        for index, parameter in enumerate(parameters):
            self.assertEqual(index+1, parameter.param_id)
        
        
    def test_create_delete(self):
        parameter = self.api.create({'name': 'test_name', 'value': 1.0})
        self.assertEqual(parameter.name, 'test_name')
        self.assertEqual(parameter.value, 1)
        parameter_again = self.api.get(4)
        self.assertEqual(parameter.name, parameter_again.name)
        self.assertEqual(parameter.value, parameter_again.value)
        self.api.delete(4)
        
        
    def test_create_fail(self):
        try:
            _ = self.api.create({'name':'popSize', 'value': 500})
        except error.ModelsAPIError:
            pass
        else:
            self.fail("No exception launched after trying insert")
            
    def test_delete_fail(self):
        try:
            self.api.delete(9999)
        except error.ModelsAPIError:
            pass
        else:
            self.fail("No exception launched after trying deletion")
        
        
    def test_update(self):
        parameter = self.api.create({'name': 'test_name', 'value': 0})
        parameter.value = 1.0
        self.api.update(parameter)
        updated = self.api.get(4)
        self.assertEqual(updated.value, 1)
        self.api.delete(4)
        
        
    def test_update_fail(self):
        parameter = self.api.get(2)
        parameter.value = 500
        try:
            self.api.update(parameter)
        except error.ModelsAPIError:
            pass
        else:
            self.fail("No exception launched after trying update")
        
        
if __name__ == "__main__":
    unittest.main()
    