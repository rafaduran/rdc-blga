#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:class:`ResultAPI` --- Very short description
=======================================
:py:class:`ResultAPI` long description

.. moduleauthor::  "Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import pyblga.data.base_models as bases

class ResultsAPI(object):
    """
    Initialization description
    
    Args.
        arg...
    """

    
    @bases.with_orm_session    
    def get(self, result_id, session=None):
        result = session.query(bases.Results).filter_by(result_id=result_id).\
            first()
        return result
    
    
    @bases.with_orm_session
    def get_all(self, session=None): 
        result = session.query(bases.Results).all()
        return result
    
    
    @bases.with_orm_session
    def get_data(self, result_id, session=None):
        result = session.query(bases.Results.data).filter_by(\
                result_id=result_id).first()
        return result
    
    
    @bases.with_transaction
    def create(self, values, session=None):
        result_ref = bases.Results()
        result_ref.update(values)
        result_ref.save(session=session)
        return result_ref
    
    
    @bases.with_transaction
    def update(self, values, session=None):
        result_ref = self.get(values.result_id, session=session)
        result_ref.update(values)
        result_ref.save(session=session)
            
    
    @bases.with_transaction    
    def delete(self, result_id, session=None):
        result_ref = self.get(result_id, session=session)
        session.delete(result_ref)
