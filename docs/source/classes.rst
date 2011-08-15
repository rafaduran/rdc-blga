Classes
=======
Main avaliable classes are listed here.

Local searcher classes:
-----------------------

.. toctree::
  :maxdepth: 2

  localsearcher
  blga
  clusteredclearing

Other local searcher classes:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Above can be found docs for main LocalSeearcher classes, however there are more
classes out of these docs:

* :cpp:class:`BlgaClearing`
* :cpp:class:`BlgaClustering`
* :cpp:class:`BlgaSharing`

These classes didn't suppose an improvement over :ref:`Blga` so I didn't work on
them anymore.

Fitness function classes
------------------------

.. toctree::
  :maxdepth: 2
  
  fitnessfunction
  periodicequal
  periodicunequal
  humpfunction

Support classes
---------------
There are also some support classes, doing some job needed by main classes, so
I'm going to do a brief introduction to them

.. _random:

:cpp:class:`Random`
^^^^^^^^^^^^^^^^^^^
This class generates random numbers and permutations. 

:cpp:class:`StopCondition`
^^^^^^^^^^^^^^^^^^^^^^^^^^
It's the base class for all classes implementing a stop criteria. These classes
are:

* :cpp:class:`FitnessStopC`: iterates until found a solution matching desired 
  fitness

* :cpp:class:`NRestartsSC`: iterates until a certain number of restarts occur

* :cpp:class:`NFEsStopC`: iterates until given number of evaluations is reached.
  This is the criteria used by :ref:`Blga` and their subclasses in this project.
   