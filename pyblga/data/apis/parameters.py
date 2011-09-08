#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:class:`ParamsAPI` --- Very short description
=======================================
:py:class:`ParamsAPI` long description

.. moduleauthor::  "Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import pyblga.data.base_models as bases
import pyblga.common.exception as error


class ParamsAPI(object):
    """
    Initialization description
    
    Args.
        arg...
    """

    @bases.with_orm_session
    def get(self, param_id, session=None):
        result = session.query(bases.Parameters).filter_by(param_id=param_id).\
            first()
        return result

    
    @bases.with_orm_session
    def get_all(self, session=None):
        result = session.query(bases.Parameters).all()
        return result
    
    
    @bases.with_transaction
    def create(self, values, session=None):
        param_ref = bases.Parameters()
        param_ref.update(values)
        param_ref.save(session=session)
        return param_ref
    
    
    @bases.with_transaction
    def update(self, values, session=None):
        param_ref = self.get(values.param_id, session=session)
        param_ref.update(values)
        param_ref.save(session=session)
            
    
    @bases.with_transaction
    def delete(self, param_id, session=None):
        param_ref = self.get(param_id, session=session)
        session.delete(param_ref)
        return 'algo'
