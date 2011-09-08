#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:class:`ParamsAPI` --- Very short description
=======================================
:py:class:`ParamsAPI` long description

.. moduleauthor::  "Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import pyblga.data as data
import pyblga.data.base_models as bases


class ParamsAPI(object):
    """
    Initialization description
    
    Args.
        arg...
    """


    def get(self, param_id, session=None):
        if session is None:
            session = data.get_session()
        result = session.query(bases.Parameters).filter_by(param_id=param_id).first()
        return result
    
    
    def get_all(self, session=None):
        if session is None:
            session = data.get_session() 
        result = session.query(bases.Parameters)
        return result
    
    
    def create(self, values):
        param_ref = bases.Parameters()
        param_ref.update(values)
        param_ref.save()
        return param_ref
    
    
    def update(self, values, session=None):
        if session is None:
            session = data.get_session()
        with session.begin():
            param_ref = self.get(id, session)
            param_ref.update(values)
            param_ref.save(session=session)
            
    
    def delete(self, param_id, session=None):
        if not session:
            session = data.get_session()
        with session.begin():
            param_ref = self.get(param_id, session)
            session.delete(param_ref)