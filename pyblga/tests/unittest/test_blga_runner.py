#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`~tests.test_blga_runner` --- Very short description
======================================
Long description

.. module:: tests.test_blga_runner
    :synopsis: Short description

.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
from __future__ import unicode_literals
from __future__ import absolute_import
from __future__ import print_function

import unittest2 as unittest
import pyblga.blga.runner as blga_runner

class TestBlgaRunner(unittest.TestCase):
    """
    Tests something ...
    """


    def setUp(self):
        self.kwargs = {'name': 'blga',
                      'itC': '10',
                      'nOff': '10',
                      'blgaPopSize' : '100',
                      'alfa': '7',
                      'numMates': '5',
                      'pamnass': '5',
                      'rtsnass': '14',
                      'functionNumber': '0',
                      'n': '1000',
                      'initRun': '0',
                      'endRun' : '1',
                      'path': '/home/rdc/projects/rdc-blga/executables/'
        }
    
    def tearDown(self):
        del self.kwargs
        
        
    def test_run_ok(self):
        """
        Checks ...
        """
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(0, result, out)
        self.assertEqual('Todo bien!\n', out)
   
   
    def test_long_run_ok(self):
        """
        Runs a long time execution
        """
        self.kwargs['blgaPopSize'] = '500'
        self.kwargs['n'] = '10000'
        self.kwargs['functionNumber'] = '2'
        self.kwargs['endRun'] = '20'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(0, result, out)
        self.assertEqual('Todo bien!\n', out)
        
    def test_long_run_clustered_clearing_ok(self):
        """
        Runs a long time execution
        """
        self.kwargs['name'] = 'ClusteredClearing'
        self.kwargs['blgaPopSize'] = '500'
        self.kwargs['n'] = '10000'
        self.kwargs['functionNumber'] = '2'
        self.kwargs['clRadius'] =  '0.1'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(0, result, out)
        self.assertEqual('Todo bien!\n', out)
        
        
    def test_wrong_localsearcher(self):
        """
        Checks running with wrong localsearcher name
        """
        self.kwargs['name'] = 'non_exists'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(1, result, out)
        self.assert_(out.startswith('Blga:Error'), out)
   
        
    def test_error_parsing_localsearcher(self):
        """
        Checks parsing args with no name attribute
        """
        self.kwargs.pop('name')
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(1, result, out)
        self.assert_(out.startswith('Blga:Error'), out)
        
        
if __name__ == "__main__":
    unittest.main()
    