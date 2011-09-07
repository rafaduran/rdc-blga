#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`~pyblga.tests.unittest.test_searchers` --- Very short description
======================================
Long description

.. module:: pyblga.tests.unittest.test_searchers
    :synopsis: Short description

.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import unittest2 as unittest
import pyblga.data.models.searchers as searchers
import pyblga.common.exception as error

class TestSearchers(unittest.TestCase):
    """
    Tests for :py:class:`pyblga.data.models.runs.RunsAPI()`
    """


    def setUp(self):
        self.api = searchers.SearchersAPI() 


    def tearDown(self):
        del self.api

    
    def test_get(self):
        searcher = self.api.get(1)
        self.assertEqual(searcher.searcher_id, 1)
        self.assertEqual(searcher.name, 'Blga')
        
    
    def test_get_fail(self):
        searcher = self.api.get(9999)
        self.assertEqual(searcher, None)

        
    def test_get_all(self):
        searchers = self.api.get_all()
        self.assertEqual(len(searchers), 3)
        
        for index, searcher in enumerate(searchers):
            self.assertEqual(index+1, searcher.searcher_id)
            
    
    def test_get_by_name(self):
        searchers = self.api.get_by_name('Blga')
        for searcher in searchers:
            self.assertEqual(searcher.name, 'Blga')
        self.assertEqual(len(searchers), 2)
        
        
    def test_get_by_name_fail(self):
        searcher = self.api.get_by_name('NonExists')
        self.assertEqual(searcher, [])
        
        
    def test_create_delete(self):
        searcher = self.api.create({'name': 'test_name', 'tag':'test_tag'})
        self.assertEqual(searcher.name, 'test_name')
        self.assertEqual(searcher.tag, 'test_tag')
        searcher_again = self.api.get(4)
        self.assertEqual(searcher.name, searcher_again.name)
        self.assertEqual(searcher.tag, searcher_again.tag)
        self.api.delete(4)
        
        
    def test_create_fail(self):
        searcher = self.api.get(1)
        searcher.tag = 'fail'
        self.api.update(searcher)
        try:
            _ = self.api.create({'name':'Blga', 'tag': 'fail'})
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
        searcher = self.api.create({'name': 'test_name'})
        searcher.tag = 'test_tag'
        self.api.update(searcher)
        updated = self.api.get(4)
        self.assertEqual(updated.tag, 'test_tag')
        self.api.delete(4)
        
        
    def test_update_fail(self):
        searcher = self.api.get(2)
        searcher.tag = 'fail'
        try:
            self.api.update(searcher)
        except error.ModelsAPIError:
            pass
        else:
            self.fail("No exception launched after trying update")
        
        
if __name__ == "__main__":
    unittest.main()
    