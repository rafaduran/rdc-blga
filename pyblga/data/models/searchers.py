#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:class:`SearcherAPI` --- Very short description
=======================================
:py:class:`SearcherAPI` long description

.. moduleauthor::  "Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import data
import data.base_models as models

class SearcherAPI(object):
    """
    Initialization description
    
    Args.
        arg...
    """

    @models.with_orm_session
    def get(self, searcher_id, session=None):
#        if session is None:
#            session = data.get_session()
        result = session.query(models.Runs).filter_by(\
                searcher_id=searcher_id).first()
        return result
    
    
    def get_all(self, session=None):
        if session is None:
            session = data.get_session() 
        result = session.query(models.Searchers)
        return result
    
    
    def create(self, values):
        searcher_ref = models.Searchers()
        searcher_ref.update(values)
        searcher_ref.save()
        return searcher_ref
    
    
    def update(self, values, session=None):
        if session is None:
            session = data.get_session()
        with session.begin():
            searcher_ref = self.get(id, session)
            searcher_ref.update(values)
            searcher_ref.save(session=session)
            
    
    def delete(self, searcher_id, session=None):
        if not session:
            session = data.get_session()
        with session.begin():
            searcher_ref = self.get(searcher_id, session)
            session.delete(searcher_ref)