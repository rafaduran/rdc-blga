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
                      #'clRadius': 0.1,
                      'blgaPopSize' : '100',
                      'alfa': '7',
                      #'r': 0.1,
                      'numMates': '5',
                      'pamnass': '5',
                      'rtsnass': '14',
                      'functionNumber': '0',
                      #'f' : 'f0.9',
                      'n': '1000',
                      'initRun': '0',
                      'endRun' : '1',
        }
    
    def tearDown(self):
        del self.kwargs
        
        
    def test_run_ok(self):
        """
        Checks ...
        """
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(0, result)
        self.assertEqual('Todo bien!\n', out)
   
    
    def test_wrong_localsearcher(self):
        """
        Checks ...
        """
        self.kwargs['name'] = 'non_exists'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(1, result)
        self.assert_(out.startswith('Blga:Error'), out)
   
        
    def test_error_parsing_localsearcher(self):
        """
        Checks ...
        """
        self.kwargs.pop('name')
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(1,result, out)
        self.assert_(out.startswith('Blga:Error'), out)
   
        
if __name__ == "__main__":
    unittest.main()
    