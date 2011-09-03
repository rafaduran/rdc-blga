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
from __future__ import absolute_import

from sqlalchemy import Column, String, Integer, Float, ForeignKey, \
    UniqueConstraint
from sqlalchemy.exc import IntegrityError
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy.orm import relationship, object_mapper

import data.jsoncol as jsoncol
import data

Base = declarative_base()


class BlgaBase(object):
    """Base class for Keystone Models."""
    __api__ = None
    _i = None

    def save(self, session=None):
        """Save this object."""

        if not session:
            session = data.get_session()
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
        Includes attributes from joins."""
        local = dict(self)
        joined = dict([(k, v) for k, v in self.__dict__.iteritems()
                      if not k[0] == '_'])
        local.update(joined)
        return local.iteritems()


def with_orm_session(func):
    def inner(*args, **kwargs):
        if 'session' not in kwargs or \
            kwargs['session'] is None:
            kwargs['session'] = data.get_session()
        with kwargs['sesion'].begin():
            return func(*args, **kwargs)
    return inner


# Define associations first
class RunsParamsAssoc(BlgaBase, Base):
    __tablename__ = 'runs_params'
    run_id = Column(Integer, ForeignKey('runs.run_id'), 
                    primary_key=True)
    param_id = Column(Integer, ForeignKey('params.param_id'), 
                    primary_key=True)
    run = relationship('Runs')
    param = relationship('Parameters')
    
    
class Runs(BlgaBase, Base):
    __tablename__ = 'runs'
    run_id = Column(Integer, primary_key=True, autoincrement=True)
    searcher_id = Column(Integer, ForeignKey('searchers.searcher_id'))
    result_id = Column(Integer, ForeignKey('results.result_id'))
    
    searcher = relationship('Searchers')
    result = relationship('Results')
    rpa = relationship(RunsParamsAssoc, cascade='all')
    
class Searchers(BlgaBase, Base):
    __tablename__ = 'searchers'
    searcher_id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(25), nullable=False)
    tag = Column(String(50))
    __table_args__ = (UniqueConstraint("name", "tag"), {})
    runs = relationship(Runs, backref='searchers', cascade="all")
    

class Parameters(BlgaBase, Base):
    __tablename__ = 'params'
    param_id = Column(Integer, primary_key=True, autoincrement=True)
    name = Column(String(25), nullable=False)
    value = Column(Float, nullable=False)
    __table_args__ = (UniqueConstraint("name", "value"), {})
    rpa = relationship(RunsParamsAssoc, cascade='all')


class Results(BlgaBase, Base):
    __tablename__ = 'results'
    result_id = Column(Integer, primary_key=True, autoincrement=True)
    result = Column(jsoncol.JSONCol, nullable=False)
    run = relationship(Runs, backref='results', cascade="all")
