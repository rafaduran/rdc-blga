#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:class:`SearcherAPI` --- Very short description
=======================================
:py:class:`SearcherAPI` long description

.. moduleauthor::  "Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import pyblga.data as data
import pyblga.data.base_models as bases

class SearchersAPI(object):
    """
    Initialization description
    
    Args.
        arg...
    """

    @bases.with_orm_session
    def get(self, searcher_id, session=None):
#        if session is None:
#            session = data.get_session()
        result = session.query(bases.Searchers).filter_by(\
                searcher_id=searcher_id).first()
        return result
    
    
    def get_all(self, session=None):
        if session is None:
            session = data.get_session() 
        result = session.query(bases.Searchers).all()
        return result
    
    @bases.with_orm_session
    def get_by_name(self, name, session=None):
        result = session.query(bases.Searchers).filter_by(name=name).all()
        return result
    
    def create(self, values):
        searcher_ref = bases.Searchers()
        searcher_ref.update(values)
        searcher_ref.save()
        return searcher_ref
    
    
    def update(self, values, session=None):
        if session is None:
            session = data.get_session()
        with session.begin():
            searcher_ref = self.get(values.searcher_id, session=session)
            searcher_ref.update(values)
            searcher_ref.save(session=session)
            
    
    def delete(self, searcher_id, session=None):
        if not session:
            session = data.get_session()
        with session.begin():
            searcher_ref = self.get(searcher_id, session=session)
            session.delete(searcher_ref)