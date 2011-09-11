#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:class:`RunsAPI` --- Very short description
=======================================
:py:class:`RunsAPI` long description

.. moduleauthor::  "Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import pyblga.common.exception as error
import pyblga.data.base_models as bases
import pyblga.data as data

class RunsAPI(object):
    """
    Initialization description
    
    Args.
        arg...
    """
    
    @bases.with_orm_session
    def get(self, run_id, session=None):
        result = session.query(bases.Runs).filter_by(run_id=run_id).first()
        return result
    
    
    @bases.with_orm_session
    def get_all(self, session=None):
        result = session.query(bases.Runs).all()
        return result
    
    
    @bases.with_orm_session
    def get_runs_data(self, run_id, session=None):
        result = session.query(bases.Runs).filter_by(\
            run_id=run_id).options(data.joinedload(bases.Runs.params),
                                   data.joinedload(bases.Runs.searcher)).first()
        return result
    
    
    @bases.with_orm_session
    def get_result(self, run_id, session=None):
        result = session.query(bases.Runs).filter_by(run_id=run_id).options(\
                data.joinedload(bases.Runs.result)).first()
        return result
    
    
    @bases.with_orm_session
    def get_param_by_name(self, run_id, param_name, session=None):
        result = session.query(bases.RunsParamsAssoc).filter_by(\
                    run_id=run_id).join(bases.Parameters).filter(\
                    bases.Parameters.name==param_name).options(\
                    data.joinedload(bases.RunsParamsAssoc.param)).first()
        return result
    
    
    @bases.with_transaction
    def create(self, values, session=None):
        run_ref = bases.Runs()
        run_ref.update(values)
        run_ref.save(session=session)
        return run_ref
    
    
    @bases.with_transaction
    def update(self, values, session=None):
        run_ref = self.get(values.run_id, session=session)
        run_ref.update(values)
        run_ref.save(session=session)
            
    
    @bases.with_transaction
    def delete(self, run_id, session=None):
        run_ref = self.get(run_id, session=session)
        session.delete(run_ref)
            
    
    @bases.with_transaction        
    def add_param(self, values, session=None):
        if self.__can_add_param(values['run_id'], values['param_id'], 
                                session=session):
            run_param_ref = bases.RunsParamsAssoc()
            run_param_ref.update(values)
            run_param_ref.save(session=session)
            return run_param_ref
        raise error.ModelsAPIError(\
                "Runs can't have several values for same parameter")

    
    @bases.with_transaction
    def __can_add_param(self, run_id, param_id, session=None):
        param = session.query(bases.Parameters.name).filter_by(\
                    param_id=param_id).first()
        if param:
            return not self.get_param_by_name(run_id, param.name, 
                                              session=session)
        raise error.ModelsAPIError("Parameter doesn't exist")
            