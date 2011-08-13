Fitness function
================
:cpp:class:`FitnessFunction` is the base class for all fitness functions.

Macros
------

.. c:macro:: P1_P2_dimension

  Represents the number of bits used by each one of the population chromosomes.
  Its value limits precision to 1/(2\ :sup:`dim`\-1), where *dim* is
  :c:macro:`P1_P2_dimension`. This value is used only by 
  :cpp:class:`PeriodicEqual` and :cpp:class:`PeriodicUnequal`

Public members
--------------

.. cpp:function:: double fitness(char* genes)

  **Virtual** method, computes fitness for a given chromosome

.. cpp:function:: double compare(double f1, double f2)
 
  This **virtual** methods compares two fitness values, returning greater, equal
  or less than zero depending if f1 fitness is better, equal or worse than f2's.

.. cpp:function:: stringstream& getName()

  Returns :cpp:member:`_ss` object containing fitness function name
  (**virtual**).

.. cpp:function:: int getFunctionNumber()

  **Virtual** method, returns fitness function number

.. cpp:function:: int getDim()

  **Virtual** method, returns :cpp:member:`_dimension` value

.. cpp:function:: double distance(char *first, char* second)

  Computes real distance between two individuals

.. cpp:function:: void binaryToDoubleVector(char* genes, double* vector)

  It returns a vector containing real value for each variable represented by the
  given chromosome (**virtual**)

.. cpp:function:: double binaryToDouble(char *genes)

  This **virtual** method computes real value for a given chromosome 
  (only one variable)

.. cpp:function:: static FitnesFunction* getFitnessFunction(int i, int numRun = 0)

  Creates a new :cpp:class:`FitnessFuncion` object and returns a pointer reference
  to it.

.. cpp:function:: int getNvariables()

  **Virtual** method, returns number of variables

.. cpp:function:: void inverseGrayVector(char* gray, char* binary)

  Performs gray decoding, returning binary equivalents, in this case multiple
  variables are expected.

.. cpp:function:: void inverseGray(char* gray, char* binary)

  As above, but for just one variable

.. cpp:function:: getHumpFunction(int numRum)

  **Static** auxiliary function used for :cpp:func:`getFitnessFunction`
  when :cpp:class:`FitnessFunction` object being created is a
  :cpp:class:`HumpFunction`.


Protected members
-----------------

.. cpp:member:: char* _binary

  Auxiliary vector used to perform some operations

.. cpp:member:: int _dimension

  Holds dimension information

.. cpp:member:: stringstream _ss

  Contains fitness function name

Private members
---------------

.. cpp:member:: static FitnessFunction *ff

  **static** variable containing reference to current 
  :cpp:class:`FitnessFunction` object. Singleton pattern is applied here, so
  when a new object is requested if :cpp:member:`ff` is not NULL it will be
  destroyed before creating a new object.
