#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`~pyblga.tests.unittest.long_blga_runs` --- Very short description
======================================
Long description

.. module:: pyblga.tests.unittest.long_blga_runs
    :synopsis: Short description

.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import unittest2 as unittest

import pyblga.tests.unittest.test_blga_runner as tbr
import pyblga.blga.runner as blga_runner

class LongBlgaRuns(tbr.TestBlgaRunner):
    """
    Tests Long Blga runs
    """
    
    def test_long_run_ok(self):
        """
        Runs a long time execution
        """
        self.kwargs['popSize'] = '500'
        self.kwargs['n'] = '10000'
        self.kwargs['functionNumber'] = '2'
        self.kwargs['endRun'] = '20'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(0, result, out)
        self.assertEqual('Ok!\n', out)
        
    def test_long_run_clustered_clearing_ok(self):
        """
        Runs a long time execution
        """
        self.kwargs['name'] = 'ClusteredClearing'
        self.kwargs['popSize'] = '500'
        self.kwargs['n'] = '10000'
        self.kwargs['functionNumber'] = '2'
        self.kwargs['clRadius'] =  '0.1'
        self.kwargs['endRun'] = '20'
        result, out = blga_runner.run(**self.kwargs)
        self.assertEqual(0, result, out)
        self.assertEqual('Ok!\n', out)


if __name__ == "__main__":
    unittest.main()
    
