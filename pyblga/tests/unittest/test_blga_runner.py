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
from future_builtins import *

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
        
        
    def test_run_p1_ok(self):
        """
        Tinny run
        """
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(0, result, out)
        self.assertEqual('Ok!\n', out)
        
    def test_run_p2_ok(self):
        """
        Tinny P2 run
        """
        self.kwargs['functionNumber'] = '1'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(0, result, out)
        self.assertEqual('Ok!\n', out)
        
        
    def test_run_list_args(self):
        """
        Test running from args instead o kwargs 
        """
        args = []
        args.append('blga')
        args.append('10')
        args.append('100')
        args.append('7')
        args.append('5')
        args.append('5')
        args.append('14')
        args.append('0')
        args.append('/home/rdc/projects/rdc-blga/executables/')
        args.append('n1000')
        args.append('0')
        args.append('1')
        result, out = blga_runner.run(*args)
        self.assertEqual(0, result, out)
        self.assertEqual('Ok!\n', out)
        
    def test_run_r(self):
        """
        Tinny run
        """
        self.kwargs['r'] = '0.1'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(0, result, out)
        self.assertEqual('Ok!\n', out)
        
        
    def test_error_wrong_localsearcher(self):
        """
        Checks running with wrong localsearcher name
        """
        self.kwargs['name'] = 'non_exists'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(1, result, out)
        self.assert_(out.startswith('Blga:Error'), out)
   
        
    def test_error_no_name(self):
        """
        Checks parsing args with no name attribute
        """
        self.kwargs.pop('name')
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(1, result, out)
        self.assert_(out.startswith('Blga:Error'), out)
        
        
    def test_error_no_stop_criteria(self):
        """
        Checks parsing arsg with no stop criteria
        """
        self.kwargs.pop('n')
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(1, result, out)
        self.assert_(out.startswith('Blga:Error'), out)

        
    def test_error_two_stop_criteria(self):
        """
        Checks parsing args with both stop criteria
        """
        self.kwargs['f'] = '0.5'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(1, result, out)
        self.assert_(out.startswith('Blga:Error'), out)
        
    
    def test_missing_arg(self):
        """
        Checks parsing args with no name attribute
        """
        self.kwargs.pop('pamnass')
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(1, result, out)
        self.assert_(out.startswith('Blga:Error'), out)
        
    
    def test_wrong_run_limits(self):
        """
        Checks wrong initRun/endRun
        """
        self.kwargs['initRun'] = '3'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(1, result, out)
        self.assert_(out.startswith('Blga:Error'), out)
        
        
    def test_missing_clearingRadius(self):
        """
        Checks running ClusteredClearing with no clearingRadius
        """
        self.kwargs['name'] = 'ClusteredClearing'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(1, result, out)
        self.assert_(out.startswith('Blga:Error'), out)

        
if __name__ == "__main__":
    unittest.main()
    