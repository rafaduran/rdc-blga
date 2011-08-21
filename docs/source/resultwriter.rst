.. _resultwriter:

:cpp:class:`ResultWriter`
=========================
:cpp:class:`ResultWriter` classes give flexible outputting for :ref:`LocalSearcher`
classes. This virtual class is implemented using three design patterns:

* Singleton [#]_
* Abstract factory [#]_
* Protected variations (from GRASP patterns [#]_)

Public members
--------------

.. cpp:function:: void write(double* variables,double fitness, bool is_last=false)

    Writes a single chromosoma, **virtual** method

.. cpp:function:: void startIteration(int iteration)

    Start writing a new iteration, **virtual** method

.. cpp:function:: void endIteration(bool is_last = false)

    Ends current iteration, **virtual** method

.. cpp:function:: void startRun(int run)

    Starts a new run, **virtual** method

.. cpp:function:: void endRun(bool is_last = false)

    Ends current run, **virtual** method

.. cpp:function:: void setNVariables(int nVar)

    Sets :cpp:member:`nVariables_`, not usually needed, since usually it is 
    initialized on class constructor

.. cpp:function:: int getNVariables()

    Getter for :cpp:member:`nVariables_`

.. cpp:function:: void setFilename(const char *filename)

    Setter for :cpp:member:`filename_`, not usually needed (same case as above)
    
.. cpp:function:: const char* getFilename()

    The getter

.. cpp:function:: static ResultWriter<T>* getResultWriter(int rwNumber, const char* filename, int nVariables)

    This method implements the singleton and abstract factory patterns

Protected members
-----------------

.. cpp:member:: const char *filename_

    Keeps filename where outputs are done

.. cpp:member:: int nVariables_

    Needed for :cpp:func:`write`, since variable number can be different from
    run to run

.. cpp:member:: T out_

    Template object, used for outputting

Private members
_______________

.. cpp:member:: static ResultWriter<T>* _rw

    Needed object for :cpp:func:`getResultWriter`

.. [#] http://en.wikipedia.org/wiki/Singleton_pattern
.. [#] http://en.wikipedia.org/wiki/Abstract_factory_pattern
.. [#] `http://en.wikipedia.org/wiki/GRASP_(object-oriented_design) <http://en.wikipedia.org/wiki/GRASP_%28object-oriented_design%29>`_