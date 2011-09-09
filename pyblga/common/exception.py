#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`pyblga.common.exception` --- Very short description
=====================================
Long description

.. module:: pyblga.common.exception 
    :synopsis: Short description
    
.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
import functools
import sqlalchemy.exc as exc
import pyblga.common.rdc_utils as utils


def raiser(cls, error, msg=None):
    if msg:
        raise cls(msg)
    raise cls(error.message)


class ModelsError(Exception):
    def __init__(self, msg=None):
        msg="Model: Error on models: {0}".format(msg)
        super(ModelsError, self).__init__(msg)


class ModelsAPIError(exc.SQLAlchemyError):
    def __init__(self, msg=None):
        msg="Data: Error on data models API: {0}".format(msg)
        super(ModelsAPIError, self).__init__(msg)

models_raiser = functools.partial(raiser, ModelsError)
api_raiser = functools.partial(raiser, ModelsAPIError)

# Error parser decorators
parsing_error_wrapper = utils.error_wrapper(
    errors=(ValueError, TypeError, KeyError),
    msg="Blga:Error while parsing Blga args",
    error_func=lambda x,y: (False, ':'.join((y, x.message)))
    )

model_error_wrapper = utils.error_wrapper(errors=ModelsAPIError,
        msg=None, error_func=models_raiser)

data_error_wrapper = utils.error_wrapper(errors=exc.SQLAlchemyError,
        msg=None, error_func=api_raiser)
