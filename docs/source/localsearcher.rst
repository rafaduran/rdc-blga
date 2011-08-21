.. _localsearcher:

:cpp:class:`LocalSearcher`
==========================
:cpp:class:`LocalSearcher` is the base class interface for all local search 
algorithms. 

Macros
------

.. c:macro:: ITERATE_CALLS
  
  This constant defines :cpp:class:`ClusteredClearing` subclass needed attribute 

Public members
--------------

.. cpp:function:: LocalSearcher()
  
  Is the class constructor, it does nothing.

.. cpp:function:: void setFF(FitnessFunction *ff)

  This **virtual** method is used for setting the FitnessFunction object needed 
  by LocalSearcher objects fitness calculations.

.. cpp:function:: int improve(char *s, double& fitness, int size)

  This **virtual** method inits the optimization process. It receives current
  solution and their fitness and population size and tries to improve it until
  convergence is reach, returning number of fitness evaluations.

.. cpp:function:: int improve(char *s, double& fitness, int size,int maxEvaluations, int iRuns, int current_nFEs, int fNumber, const char *name)

  **Overloaded** improve method, in this case it also receives maximum number of
  evaluations and tries improving solution until this number is reached,
  reseting algorithm if necessary.
 
.. cpp:function:: static LocalSearcher* getLS(int nArgs, char **args, FitnessFunction *ff, Random *random, ostringstream &algName, int maxEvaluations)

  This **static** method creates a new LocalSearcher subclass object and returns
  a pointer to it. This methods receives all needed attributes and calls the
  subclass constructor.

.. cpp:function:: static void showLSs()

  This **static** methods returns information about run arguments of each
  LocalSearcher subclasses. Any new subclass information should be added here. 

.. cpp:function:: void reset(char *lsName)

  Resets LocalSearcher so multiples run can be achieved using same object 
  instead of creating/destructing objects each run.

.. cpp:function:: void writeResults(int iRuns, int nFEs, int functionNumber, bool is_last_run, bool is_last_iteration)

  This **virtual** method writes results using an :ref:`ResultWriter` class.

