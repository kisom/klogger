Source Files
============

Logger interface
----------------

These files contain the public interface for the Logger type.

+ ``src/klogger/logger.hh`` contains definitions for
  + the ``Level`` enum, which describes log levels,
  + the ``DEFAULT_LEVEL`` constant,
  + the ``LogError`` enum, which describes error conditions for a ``Logger``, and
  + the ``Logger`` abstract base class.
+ ``src/logger.cc`` contains common logging utility functions.
+ ``src/internal.hh`` contains a header file for internal functions.

ConsoleLogger
-------------

+ ``src/klogger/console.hh`` contains the definition for
  ``ConsoleLogger``.
+ ``src/console.cc`` contains the implementation for
  ``ConsoleLogger``.
+ ``src/console_test.cc`` contains a short test program.
