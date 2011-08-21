.. _blgajsonrw:
    
:cpp:class:`BlgaJsonRW`
=======================
:cpp:class:`BlgaJsonRW` implements :ref:`ResultWriter`, giving JSON outputs to 
:ref:`Blga` class and their subclasses. The outputting is implemented using
an :cpp:class:`ofstream` object.

Public methods
--------------

.. cpp:function:: cppBlgaJsonRW(const char* filename, int nVariables)

.. cpp:function:: void write(double* variables, double fitness, bool is_last = false)

.. cpp:function:: void startIteration(int iteration)

.. cpp:function:: void endIteration(bool is_last)

.. cpp:function:: void startRun(int run)

.. cpp:function:: void endRun(bool is_last)

All this methods implements parent virtual methods.

