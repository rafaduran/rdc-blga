Periodic equal
==============
:cpp:class:`PeriodicEqual` inherits from :ref:`FitnessFunction` implementing
next function:

.. math:: 

  P_1(x) = sin^6(5\pi x)

This function has ﬁve peaks in the interval 0 ≤ x ≤ 1, as evident from the 
argument of the sine function and they are located at x = 0.1, 0.3, 0.5, 0.7 and
0.9. In all these locations, the objective function values are same and equal
to one.

Macros
------

.. c:macro:: PI

  Constant value of :math:`\pi`, in this case I'm using 3.14159265358979323846

Public members
--------------

.. cpp:function:: PeriodicEqual(int dimension)

  Class constructor, it just receives :cpp:member:`_dimension` value to be
  assigned to.

.. cpp:function:: double fitness(char* genes)

  Computes fitness for a given chromosome

.. cpp:function:: double compare(double f1, double f2)
 
  Compares two fitness values, returning greater, equal
  or less than zero depending if f1 fitness is better, equal or worse than f2's.

.. cpp:function:: stringstream& getName()

  Returns :cpp:member:`_ss` object containing fitness function name

.. cpp:function:: int getFunctionNumber()

  Returns fitness function number

.. cpp:function:: int getDim()

  Returns :cpp:member:`_dimension` value

.. cpp:function:: double distance(char *first, char* second)

  Computes real distance between two individuals

.. cpp:function:: double binaryToDouble(char *genes)

  This method computes real value for a given chromosome 
  (only one variable)
