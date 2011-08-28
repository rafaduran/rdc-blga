#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`pyblga.data.models` --- Very short description
=====================================
Long description

.. module:: pyblga.data.models 
    :synopsis: Short description
    
.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""

from sqlalchemy import Column, String, Integer, Float, ForeignKey, \
    UniqueConstraint
from sqlalchemy.exc import IntegrityError
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship, object_mapper

Base = declarative_base()


class BlgaBase(object):
    """Base class for Keystone Models."""
    __api__ = None
    _i = None

    def save(self, session=None):
        """Save this object."""

        if not session:
            from pyblga.data import get_session
            session = get_session()
        session.add(self)
        try:
            session.flush()
        except IntegrityError:
            raise

    def delete(self, session=None):
        """Delete this object."""
        self.save(session=session)

    def __setitem__(self, key, value):
        setattr(self, key, value)

    def __getitem__(self, key):
        return getattr(self, key)

    def get(self, key, default=None):
        return getattr(self, key, default)

    def __iter__(self):
        self._i = iter(object_mapper(self).columns)
        return self

    def next(self):
        n = self._i.next().name
        return n, getattr(self, n)

    def update(self, values):
        """Make the model object behave like a dict"""
        for k, v in values.iteritems():
            setattr(self, k, v)

    def iteritems(self):
        """
        Make the model object behave like a dict.
        Includes attributes from joins.
        """
        local = dict(self)
        joined = dict([(k, v) for k, v in self.__dict__.iteritems()
                      if not k[0] == '_'])
        local.update(joined)
        return local.iteritems()


# Define associations first
class RPReAssoc(BlgaBase, Base):
    __tablename__ = 'rpra'
    run_id = Column(Integer, ForeignKey('runs.run_id'), primary_key=True)
    param_id = Column(Integer, ForeignKey('params.param_id'), primary_key=True)
    result_id = Column(Integer, ForeignKey('results.result_id'), 
                       primary_key=True)
    
    run = relationship('Runs')
    param = relationship('Parameters')
    result = relationship('Results')
    
class Runs(BlgaBase, Base):
    """
    :py:class:`Run` description
    """
    __tablename__ = 'runs'
    run_id = Column(Integer, primary_key=True, autoincrement=True)
    iterations = Column(Integer)
    feval = Column(Integer)
    name = Column(String(255))
    rprR = relationship(RPReAssoc, cascade="all")
    

class Parameters(BlgaBase, Base):
    __tablename__ = 'params'
    param_id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(255))
    value = Column(Float)
    __table_args__ = (UniqueConstraint("name", "value"), {})
    rprP = relationship(RPReAssoc, cascade="all")


class Results(BlgaBase, Base):
    __tablename__ = 'results'
    result_id = Column(Integer, primary_key=True, autoincrement=True)
    result = Column(Integer)
    rprRe = relationship(RPReAssoc, cascade="all")