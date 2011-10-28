#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`~pyblga.gui.dialogs.run_dialog` --- Run dialog
=======================================================
This dialog gets all needed information in order to run Blga

.. module:: pyblga.gui.dialogs.run_dialog
    :synopsis: Run dialog

.. moduleauthor::Rafael Durán Castañeda <rafadurancastaneda@gmail.com>
"""
from __future__ import unicode_literals
from __future__ import absolute_import
from __future__ import print_function
from future_builtins import * #@UnusedWildImport
import sys
import os
sys.path.insert(0,os.path.abspath(os.path.join(os.path.abspath(__file__), '..',
                                             '..', '..', '..')))

from PyQt4 import QtCore
from PyQt4 import QtGui

import pyblga.gui.dialogs.ui_run_dialog as template

# TODO: unicode dict conversion

class RunDialog(QtGui.QDialog, template.Ui_RunDialog):
    def __init__(self, parent=None):
        super(RunDialog, self).__init__(parent)
        self.setupUi(self)
        self.connect(self.fitnessclass_combobox, QtCore.SIGNAL(\
            "currentIndexChanged(int)"), lambda index: \
            self.enable_file_button(index))
        self.connect(self.path_button, QtCore.SIGNAL("clicked()"),
                     self.select_path)
        self.options = {}
    
    
    @QtCore.pyqtSlot(int)
    def enable_file_button(self, index):
        self.path_button.setEnabled(index==2)
        
    
    @QtCore.pyqtSlot()
    def select_path(self):
        self.options['path'] = unicode(QtGui.QFileDialog.getExistingDirectory( 
                caption="Select Hump Function files directory",
                directory= os.path.abspath(os.path.join(
                    os.path.abspath(__file__), '..','..', '..', '..'))))
    
    
    @QtCore.pyqtSlot()   
    def accept(self):
        if not 'path' in self.options or not self.options['path']:
            if self.fitnessclass_combobox.currentIndex() == 2:
                QtGui.QMessageBox.critical(self, "Wrong parameters", 
                "If you choose Hump Function, you must select path")
                return
            else:
                # Setting a dummy value, just for compatibility
                self.options['path'] = os.path.dirname(\
                    os.path.abspath(__file__))
        
        self.options['name'] = unicode(self.ls_combobox.currentText())
        
        if self.options['name'] == 'Blga':
            self.options['name'] = self.options['name'].lower()
            self.options['itC'] = unicode(self.itcalls_spinbox.value())
        else:
            self.options['clRadius'] = unicode(self.clearing_spinbox.value())
            self.options['nOff'] = unicode(self.offsprings_spinbox.value())
        
        self.options['popSize'] = unicode(self.popsize_spinbox.value())
        
        if self.alfa_r_combo_box.currentIndex() == 0:
            self.options['alfa'] = unicode(self.alfa_spinbox.value())
        else:
            self.options['r'] = unicode(self.r_dspinbox.value())
            
        self.options['functionNumber'] = \
            unicode(self.fitnessclass_combobox.currentIndex())
        self.options['n'] = unicode(self.evalnum_spinbox.value())
        self.options['initRun'] = unicode(self.startrun_spinbox.value())
        self.options['endRun'] = unicode(self.endrun_spinbox.value())
        self.options['pamnass'] = unicode(self.pamnass_spinbox.value())
        self.options['rtsnass'] = unicode(self.rtsnass_spinbox.value())
        self.options['numMates'] = unicode(self.mates_spinbox.value())
        super(RunDialog, self).accept()           
            

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    form = RunDialog()
    form.show()
    app.exec_()
    print(form.options)
    
    
