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
import sqlalchemy.exc as exc
import pyblga.common.rdc_utils as utils


def raiser(error, msg=None):
    if msg:
        raise ModelsAPIError(msg)
    raise ModelsAPIError(error.message)


class ModelsAPIError(exc.SQLAlchemyError):
    def __init__(self, msg=None):
        msg="Data: Error on data models API: {0}".format(msg)
        super(ModelsAPIError, self).__init__(msg)


# Error parser decorators
parsing_error_wrapper = utils.error_wrapper(
    errors=(ValueError, TypeError, KeyError),
    msg="Blga:Error while parsing Blga args",
    error_func=lambda x,y: (False, ':'.join((y, x.message)))
    )

data_error_wrapper = utils.error_wrapper(errors=exc.SQLAlchemyError,
        msg=None, error_func=raiser)