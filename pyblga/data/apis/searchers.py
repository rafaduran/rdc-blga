#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:class:`SearcherAPI` --- Very short description
=======================================
:py:class:`SearcherAPI` long description

.. moduleauthor::  "Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import pyblga.data.base_models as bases
import pyblga.common.exception as error

class SearchersAPI(object):
    """
    API used to manage Searchers objects
    """

    @bases.with_orm_session
    def get(self, searcher_id, session=None):
        result = session.query(bases.Searchers).filter_by(\
                searcher_id=searcher_id).first()
        return result
    
    @bases.with_orm_session
    def get_all(self, session=None):
        result = session.query(bases.Searchers).all()
        return result
    
    
    @bases.with_orm_session
    def get_by_name(self, name, session=None):
        result = session.query(bases.Searchers).filter_by(name=name).all()
        return result
    

    @bases.with_transaction
    def create(self, values, session=None):
        searcher_ref = bases.Searchers()
        searcher_ref.update(values)
        searcher_ref.save(session=session)
        return searcher_ref
    
    
    @bases.with_transaction
    def update(self, values, session=None):
        searcher_ref = self.get(values.searcher_id, session=session)
        searcher_ref.update(values)
        searcher_ref.save(session=session)

            
    @bases.with_transaction
    def delete(self, searcher_id, session=None):
        searcher_ref = self.get(searcher_id, session=session)
        session.delete(searcher_ref)