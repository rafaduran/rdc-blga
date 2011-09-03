#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:class:`ResultAPI` --- Very short description
=======================================
:py:class:`ResultAPI` long description

.. moduleauthor::  "Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import data
import data.base_models as models

class ResultsAPI(object):
    """
    Initialization description
    
    Args.
        arg...
    """

    
    def get(self, result_id, session=None):
        if session is None:
            session = data.get_session()
        result = session.query(models.Results).filter_by(result_id=result_id).first()
        return result
    
    
    def get_all(self, session=None):
        if session is None:
            session = data.get_session() 
        result = session.query(models.Results)
        return result
    
    
    def create(self, values):
        result_ref = models.Results()
        result_ref.update(values)
        result_ref.save()
        return result_ref
    
    
    def update(self, values, session=None):
        if session is None:
            session = data.get_session()
        with session.begin():
            result_ref = self.get(id, session)
            result_ref.update(values)
            result_ref.save(session=session)
            
    
    def delete(self, result_id, session=None):
        if not session:
            session = data.get_session()
        with session.begin():
            result_ref = self.get(result_id, session)
            session.delete(result_ref)
