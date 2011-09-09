#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`pyblga.gui.models` --- Models for Blga GUI
===================================================
Models for Blga GUI, using Model/View/Delegate (M/V/D) pattern. Check 
`Qt Model/View <http://doc.qt.nokia.com/latest/model-view-programming.html>`_
web page for further information about M/V/D.

.. module:: pyblga.gui.models 
    :synopsis: Models for Blga GUI
    
.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
from __future__ import unicode_literals
#import collections

import PyQt4.QtCore as core

import pyblga.data.apis.runs as runs
import pyblga.data.apis.searchers as searchers
import pyblga.data.apis.parameters as parameters
import pyblga.data.apis.results as results
import pyblga.common.exception as error
import pyblga.common.rdc_utils as utils

#TODO: order models methods
  
class RunsTableModel(core.QAbstractTableModel):
    SEARCHER_NAME = 0
    SEARCHER_TAG = 1
    @utils.property_from_class 
    class columns(object):
        '''Values must be 2-tuples'''
        def fget(self):
            try:
                return self.__values.values()
            except AttributeError:
                self.__values = {}
                return self.__values.values()

        def fset(self, value):
            try:
                self.__values[value[0]] = value[1]
            except AttributeError:
                self.__values = {}
                self.__values[value[0]] = value[1]
            except (TypeError, IndexError) as exc:
                raise error.ModelsAPIError(exc.message)

    
    @error.model_error_wrapper
    def __init__(self):
        super(RunsTableModel, self).__init__()
        self.ruAPI = runs.RunsAPI()
        self.runs = self.ruAPI.get_all()
        self.pAPI = parameters.ParamsAPI()
        self.sAPI = searchers.SearchersAPI()
        self.__init_columns()
    
        
    @error.model_error_wrapper    
    def __init_columns(self):
        for index, param in enumerate(self.pAPI.get_names(), 2):
            self.columns = (index, param.name)
    
    
    def __del_column(self, index):
        del self._columns__values[index]
        
        
    def rowCount(self, index=core.QModelIndex()):
        return len(self.runs)
    
    
    def columnCount(self, index=core.QModelIndex()):
        return len(self.columns)+2
    
        
    def flags(self, index):
        return super(RunsTableModel,self).flags(index)
    
    @error.model_error_wrapper
    def data(self, index, role=core.Qt.DisplayRole):
        if(not index.isValid()) or not (0 <= index.row() < len(self.runs)):
            return core.QVariant()
        column = index.column()
        row = index.row()
        if role == core.Qt.DisplayRole:
            if column == RunsTableModel.SEARCHER_NAME:
                return core.QVariant(self.sAPI.get_name(self.runs[row].\
                                                        searcher_id).name)
            elif column == RunsTableModel.SEARCHER_TAG:
                return core.QVariant(self.sAPI.get_tag(self.runs[row].\
                                                        searcher_id).tag)
            elif 2 <= column < len(self.columns)+2:
                assoc = self.ruAPI.get_param_by_name(self.runs[row].run_id, 
                                            self.columns[column-2])
                if assoc:
                    return core.QVariant(assoc.param.value)
                return core.QVariant()
            else:
                return core.QVariant()
        elif role == core.Qt.TextAlignmentRole:
            return core.QVariant(int(core.Qt.AlignCenter))
        elif role == core.Qt.ToolTipRole:
            if column == RunsTableModel.SEARCHER_NAME:
                return core.QVariant("Local searcher name")
            elif column == RunsTableModel.SEARCHER_TAG:
                return core.QVariant("Repository tag, used to distinguish "
                                     "different version of same local searcher")
            elif 2 <= column < len(self.columns)+2:
                return core.QVariant("Parameter: {0}".format(\
                                    self.columns[column-2]))
            return core.QVariant()
        return core.QVariant()
        
    
    def headerData(self, section, orientation, role=core.Qt.DisplayRole):
        if role == core.Qt.TextAlignmentRole:
            if orientation == core.Qt.Horizontal:
                return core.QVariant(int(core.Qt.AlignLeft|core.Qt.AlignCenter))
            return core.QVariant(int(core.Qt.AlignRight|core.Qt.AlignCenter))
        elif role == core.Qt.DisplayRole and orientation == core.Qt.Horizontal:
            if section ==  RunsTableModel.SEARCHER_NAME:
                return core.QVariant("LS Name")
            elif section == RunsTableModel.SEARCHER_TAG:
                return core.QVariant("LS tag")
            elif 2 <= section < len(self.columns)+2:
                return core.QVariant(self.columns[section-2])
            return core.QVariant()
        return core.QVariant()
        