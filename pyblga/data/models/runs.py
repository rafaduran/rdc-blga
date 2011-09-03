#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:class:`RunsAPI` --- Very short description
=======================================
:py:class:`RunsAPI` long description

.. moduleauthor::  "Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import pyblga.data.base_models as models
import data 

class RunsAPI(object):
    """
    Initialization description
    
    Args.
        arg...
    """
    
    
    def get(self, run_id, session=None):
        if session is None:
            session = data.get_session()
        result = session.query(models.Runs).filter_by(run_id=run_id).first()
        return result
    
    
    def get_all(self, session=None):
        if session is None:
            session = data.get_session() 
        result = session.query(models.Runs)
        return result
    
    
    def get_runs_data(self, run_id, session=None):
        if session is None:
            session = data.get_session()
        result = session.query(models.RunsParamsAssoc).filter_by(\
            run_id=run_id).options(data.joinedload(models.Parameters), 
            data.joinedload(models.Searcher))
        return result
    
    
    def get_result(self, run_id, session=None):
        if session is None:
            session = data.get_session()
        result = session.query(models.RunsParamsAssoc).filter_by(\
            run_id=run_id).options(data.joinedload(models.Results))
        return result
    
    
    def create(self, values):
        run_ref = models.Runs()
        run_ref.update(values)
        run_ref.save()
        return run_ref
    
    
    def update(self, values, session=None):
        if session is None:
            session = data.get_session()
        with session.begin():
            run_ref = self.get(id, session)
            run_ref.update(values)
            run_ref.save(session=session)
            
    
    def delete(self, run_id, session=None):
        if not session:
            session = data.get_session()
        with session.begin():
            run_ref = self.get(run_id, session)
            session.delete(run_ref)