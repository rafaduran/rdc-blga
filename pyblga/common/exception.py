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
from __future__ import unicode_literals
from __future__ import absolute_import
from __future__ import print_function
from future_builtins import *

import pyblga.common.rdc_utils as utils

parsing_error_wrapper = utils.error_wrapper(
    errors=(ValueError, TypeError, KeyError),
    msg="Blga:Error while parsing Blga args",
    error_func=lambda x,y: (False, ':'.join((y, x.message)))
    )
    