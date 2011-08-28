#!/usr/bin/env python
# -*- Encoding: utf-8 -*-
# vim: tabstop=4 shiftwidth=4 softtabstop=4
"""
:py:mod:`~pyblga.blgagui` --- Very short description
======================================
Long description

.. module:: pyblga.blgagui
    :synopsis: Short description

.. moduleauthor::"Rafael Durán Castañeda <rafadurancastaneda@gmail.com>"
"""
from __future__ import unicode_literals
from __future__ import absolute_import
from __future__ import print_function
from future_builtins import * #@UnusedWildImport pylint:disable=W0614

import sys
import os
import functools
import platform
import webbrowser

import sip
sip.setapi('QString', 2) # utf-8

from PyQt4 import QtCore
from PyQt4 import QtGui

import  pyblga.static.qrc_resources #@UnusedImport

# TODO: add log widget
# TODO: interface to ask which draws are available

__version__ = '0.1'
__blga_version__ = '1.1.0'

class BlgaGUI(QtGui.QMainWindow):
    """
    Main window for Blga GUI
    """
    
    def __init__(self, parent=None):
        super(BlgaGUI, self).__init__(parent)

        self.makeLayout()
        self.createMenu()
        self.status_bar = self.statusBar()
        self.status_bar.setSizeGripEnabled(False)
        self.loadSettings()
        self.status_bar.showMessage("Ready", 5000)        
        self.setWindowTitle("Blga GUI")
        self.updateFileMenu()
        
        self.show()
        
        
    def updateUI(self):
        """
        Shows menu
        """
        pass
    
    @QtCore.pyqtSlot()
    def updateFileMenu(self):
        """
        Updates menu
        """
        pass
    
    
    @QtCore.pyqtSlot(int)
    def draw(self, index):
        pass
    
    
    @QtCore.pyqtSlot(int)
    def showHelp(self):
        help_file = "static/html/help.html"
        helppath = os.path.join(os.path.dirname(__file__), help_file)
        webbrowser.open_new("file://{0}".format(helppath))
    
    
    @QtCore.pyqtSlot(int)
    def about(self):
        QtGui.QMessageBox.about(self, "About BlgaGUI",
                """<b>Blga Grpaphic User Interface</b> v {0}
                <p>Copyright &copy; 2011 RDC 
                <p>This application can be used to manage Blga 
                V {1} runs and results.
                <p>Python {2} - Qt {3} - PyQt {4} on {5}""".format(
                __version__, __blga_version__, platform.python_version(),
                QtCore.QT_VERSION_STR, QtCore.PYQT_VERSION_STR, 
                platform.system()))

    
   
    @QtCore.pyqtSlot()            
    def run(self):
        pass
    
    
    @QtCore.pyqtSlot()
    def import_from_file(self):
        pass
    
    
    @QtCore.pyqtSlot()
    def export_to_file(self):
        pass
    
    
    def makeLayout(self):
        centralwidget = QtGui.QWidget()
        grid_layout = QtGui.QGridLayout(centralwidget)
        self.splitter = QtGui.QSplitter(centralwidget)
        self.splitter.setOrientation(QtCore.Qt.Horizontal)
        self.runs_table = QtGui.QTableWidget(self.splitter)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, 
                                       QtGui.QSizePolicy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.runs_table.sizePolicy().\
                                     hasHeightForWidth())
        #self.runs_table.setSizePolicy(sizePolicy)
        #self.runs_table.setLocale(QtCore.QLocale(QtCore.QLocale.English, 
        #                                          QtCore.QLocale.UnitedStates))
        layout_widget = QtGui.QWidget(self.splitter)
        vertical_layout = QtGui.QVBoxLayout(layout_widget)
        vertical_layout.setMargin(0)
        self.label = QtGui.QLabel(layout_widget)
        self.label.setText("No data to show")
        vertical_layout.addWidget(self.label)
        self.results_table = QtGui.QTableWidget(layout_widget)
        self.results_table.setColumnCount(0)
        self.results_table.setRowCount(0)
        vertical_layout.addWidget(self.results_table)
        grid_layout.addWidget(self.splitter, 0, 0, 1, 1)
        self.setCentralWidget(centralwidget)
    
    def createMenu(self):
        """
        Creates Menu on startup
        """
        self.menu = self.menuBar()
        run_action = self.createAction("&New Blga run", self.run,
                QtGui.QKeySequence.New, "run", "New Blga run")
        import_action = self.createAction("&Import", self.import_from_file,
                None, "import", "Import data from an existing file")
        export_action = self.createAction("&Export", self.export_to_file,
                None, "export", "Export to file")
        close_action = self.createAction("&Quit", self.close,
                "Ctrl+Q", "filequit", "Close the application")

        
        file_toolbar = self.addToolBar("File")
        file_toolbar.setObjectName('file_toolbar')
        blga_menu = self.menu.addMenu("&Blga")
        self.addActions(blga_menu,(run_action, None, import_action, 
                                  export_action, None, close_action))
        self.addActions(file_toolbar,(run_action, None, import_action, 
                                  export_action,None, close_action))
        
        chart_menu = self.menu.addMenu("&Charts menu")
        chart_toolbar = self.addToolBar("Chart")
        chart_toolbar.setObjectName('chart_toolbar')
        for index, draw_type in enumerate((("&Line chart", "line"), 
                                           ("&Pie chart", "pie"),)):
            draw = functools.partial(self.draw, index)
            draw_action = self.createAction(draw_type[0], draw, None, 
                                        draw_type[1], draw_type[0][1:])
            self.addActions(chart_menu, (draw_action,))
            self.addActions(chart_toolbar, (draw_action,))
            
        help_action = self.createAction("&Help", self.showHelp,
                QtGui.QKeySequence.HelpContents, "helpcontents", "Help")
        about_action = self.createAction("&About", self.about, None, 
                                         "about", "About BlgaGUI")
        
        help_menu = self.menu.addMenu("H&elp")
        self.addActions(help_menu, (help_action, about_action,))    

    
    def createAction(self, text, slot=None, shortcut=None, icon=None,
                     tip=None, checkable=False, signal="triggered()"):
        action = QtGui.QAction(text, self)
        if icon is not None:
            action.setIcon(QtGui.QIcon(":/%s.png" % icon))
        if shortcut is not None:
            action.setShortcut(shortcut)
        if tip is not None:
            action.setToolTip(tip)
            action.setStatusTip(tip)
        if slot is not None:
            self.connect(action, QtCore.SIGNAL(signal), slot)
        if checkable:
            action.setCheckable(True)
        return action


    def addActions(self, target, actions):
        for action in actions:
            if action is None:
                target.addSeparator()
            else:
                target.addAction(action)
    
    
    def loadSettings(self):
        settings = QtCore.QSettings()
        size = settings.value("MainWindow/Size",
                              QtCore.QVariant(QtCore.QSize(1020, 610))).toSize()
        self.resize(size)
        position = settings.value("MainWindow/Position",
                            QtCore.QVariant(QtCore.QPoint(0, 0))).toPoint()
        self.move(position)
        self.restoreState(
                settings.value("MainWindow/State").toByteArray())
        sizes = [211, 411]
        for index, _ in enumerate(sizes):
            sizes[index] = settings.value("MainWindow/Splitter/{0}".\
                        format(index), sizes[index]).toInt()[0]
        
        self.splitter.setSizes(sizes)
    
    
    def closeEvent(self, event):
        if self.okToContinue():

            settings = QtCore.QSettings()
            settings.setValue("MainWindow/Size", QtCore.QVariant(self.size()))
            settings.setValue("MainWindow/Position", QtCore.QVariant(self.pos()))
            settings.setValue("MainWindow/State", QtCore.QVariant(self.saveState()))
            sizes = self.splitter.sizes()
            for index, size in enumerate(sizes):
                settings.setValue("MainWindow/Splitter/{0}".format(index), 
                              QtCore.QVariant(size))
            event.accept()
        else:
            event.ignore()
            
            
    def okToContinue(self):
        if  QtGui.QMessageBox.Yes == \
            QtGui.QMessageBox.question(self, 'Exit', 'Are you sure?', 
                QtGui.QMessageBox.Yes | QtGui.QMessageBox.No):
            return True
        return False


if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    app.setOrganizationName("RDC")
    app.setOrganizationDomain("rdc.dynds-at-home.com")
    app.setApplicationName("BlgaGUI")
    app.setWindowIcon(QtGui.QIcon(":/dna.png"))
    form = BlgaGUI()
    form.show()
    app.exec_()    
    