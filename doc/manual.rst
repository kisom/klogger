libklogger user's manual
========================

This is a lightweight MIT-licensed attribute-based logging static library
written in C++11.

See also the functional spec (``doc/funspec.rst``) and the README
(``README.rst``).


Overview
--------

All of the components in libklogger are in the ``klog`` namespace.

Log messages consist of four components:

1. The **level** attaches a notion of priority to the log message.
   Several log levels are available:

   + FATAL (32): the system is in an unsuable state, and cannot
     continue to run. Most of the logging for this 
   + CRITICAL (16): critical conditions. The error, if uncorrected, is
     likely to cause a fatal condition shortly.  An example is running
     out of disk space. This is something that the ops team should get
     paged for.
   + ERROR (8): error conditions. A single error doesn't require an
     ops team to be paged, but repeated errors should often trigger a
     page based on threshold triggers. An example is a network
     failure: it might be a transient failure (these do happen), but
     most of the time it's self-correcting.
   + WARNING (4): warning conditions. An example of this is a bad
     request sent to a server. This isn't an error on the part of the
     program, but it may be indicative of other things. Like errors,
     the ops team shouldn't be paged for errors, but a page might be
     triggered if a certain threshold of warnings is reached (which is
     typically much higher than errors). For example, repeated
     warnings might be a sign that the system is under attack.
   + INFO (2): informational message. This is a normal log message
     that is used to deliver information, such as recording
     requests. Ops teams are never paged for informational
     messages. This is the default log level.
   + DEBUG (1): debug-level message. These are only used during
     development or if a deployed system repeatedly sees abnormal
     errors.

   The numeric values indicate the priority of a given level.

2. The **actor** is used to specify which component is generating
   the log message. This could be the program name, or it could be
   a specific component inside the system.

3. The **event** is a short message indicating what happened. This is
   most like the traditional log message.

4. The **attributes** are an optional set of key-value string pairs that
   provide additional information.

Additionally, each log message has an associated timestamp. For the
text-based logs, this is "%FT%T%z"; for the binary logs, this is a
64-bit Unix timestamp. An example text-based timestamp might look like ::

  [2016-03-27T20:59:27-0700] [INFO] [actor:server event:request received] client=192.168.2.5 request-size=839

Note that this is organised in a manner that facilitates parsing::

  /\[(\d{4}-\d{3}-\d{2}T\d{2}:\d{2}:\d{2}[+-]\d{4})\] \[(\w+\)]\) \[actor:(.+?) event:(.+?)\]/

will cover the header:

+ ``$1`` contains the timestamp
+ ``$2`` contains the level
+ ``$3`` contains the actor
+ ``$4`` contains the event

The library revolves around the ``Logger`` virtual class. There are
several implementations:

+ ``ConsoleLogger``: writes DEBUG and INFO messages to standard output,
  and other messages to standard error. The constructor takes no
  arguments. See ``src/console_test.cc``
+ ``FileLogger``: writes DEBUG and INFO messages to a log file, and
  other messages to an error log file. There are two constructors:
  the first takes a single filename as an argument; the error log and
  the log file are the same file. Alternatively, separate log files
  can be specified. See ``src/file_test.cc``.
+ ``Syslogger``: writes logs to syslog. See ``src/syslog_test.cc``.
+ ``BinLogger``: operates similarly to ``FileLogger`` except that
  logs are written in a binary format.


Example
-------

This is the ``src/console_test.cc`` program::

  #include <cstdlib>
  #include <map>
  #include <string>
   
  #include <klogger/console.hh>
   
   
  int
  main(int argc, char *argv[])
  {
          std::string                             name = std::string(argv[0]);
          klog::ConsoleLogger                     console(name);
          std::map<std::string, std::string>      args;
   
          if (!console.good()) {
                  ::abort();
          }
   
          console.debug("main", "starts");
   
          for (int i = 1; i < argc; i++) {
                  auto        k = "argv[" + std::to_string(i) + "]";
                  args[k] = std::string(argv[i]);
          }
   
          console.info("main", "starts", args);
          console.warn("main", "depleted", {{"args", std::to_string(argc)}});
          console.fatal("main", "ends");
  }


Levels
------

Levels are implemented as a class enumeration, and are in their own namespace
(``klog::Level``). The available levels are defined as::

  enum class Level : int {
	// DEBUG is a debug-level log message. These are only used during
	// development or if a deployed system repeatedly sees abnormal
	// errors.
	DEBUG = 1<<0,

	// INFO is an informational log message. This is a normal
	// log message that is used to deliver information, such as
	// recording requests. Ops teams are never paged for informational
	// messages. This is the default log level.
	INFO = 1<<1,

	// WARN is a warning message. An example of this is a bad request
	// sent to a server. This isn't an error on the part of the program,
	// but it may be indicative of other things. Like errors, the ops
	// team shouldn't be paged for errors, but a page might be triggered
	// if a certain threshold of warnings is reached (which is typically
	// much higher than errors). For example, repeated warnings might
	// be a sign that the system is under attack.
	WARN = 1<<2,

	// ERROR is an error message.  A single error doesn't require an
	// ops team to be paged, but repeated errors should often trigger
	// a page based on threshold triggers. An example is a network
	// failure: it might be a transient failure (these do happen),
	// but most of the time it's self-correcting.
	ERROR = 1<<3,

	// CRITICAL is a message for critical conditions. The error,
	// if uncorrected, is likely to cause a fatal condition shortly.
	// An example is running out of disk space. This is something that
	// the ops team should get paged for.
	CRITICAL = 1<<4,

	// FATAL indicates the system is unusable, the equivalent of
	// syslog's ``emerg`` level ([EMERG]); this has the additional
	// effect of exiting. An example is a self-check assertion failure,
	// indicating that the system is in an inconsistent state. The ops
	// team would be paged for this condition.
	FATAL = 1<<5,
  }

For the most part, these values are only used to set the logger's
level.

  
The Logger virtual class
------------------------

The ``Logger`` class is the base for all loggers. There are several
methods that don't generate log messages:

+ ``void level(klog::Level)`` sets the minimum priority for the
  logger. The default is ``klog::Level::INFO``, which means that DEBUG
  messages will be ignored.

+ ``bool good(void)`` returns ``true`` if the logger is healthy.

+ ``klog::LogError error(void)`` returns the current health state of
  the logger. The ``LogError`` type is discussed below.

+ ``int close(void)`` closes the logger. This may be a no-op on some
  loggers, but in others it may perform useful shutdown logic.


A ``Logger`` implements a number of logging methods. These methods generally come in
two forms; the ``debug`` method is an example.

The first form takes only an actor and an event.::

        void debug(const std::string& actor,
                   const std::string& event);

The second form takes an actor, event, and a set of attributes.::

        void debug(const std::string& actor,
                   const std::string& event,
                   std::map<std::string, std::string> attrs);

There are similar methods for the other log levels:

+ ``debug``
+ ``info``
+ ``warn``
+ ``error``
+ ``critical``
+ ``fatal``

This version of ``fatal`` will exit with status code 1. There are
two other variants: the first specifies the exit code::

        void fatal(int exitcode,
                   const std::string& actor,
                   const std::string& event);

        void fatal(int exitcode,
                   const std::string& actor,
                   const std::string& event,
                   std::map<std::string, std::string> attrs);

The second form doesn't exit; it's provided for the case where additional
cleanup needs to be done::

        void fatal_noexit(const std::string& actor,
                          const std::string& event);

        void fatal_noexit(const std::string& actor,
                          const std::string& event,
                          std::map<std::string, std::string> attrs);


Console Logger
--------------

The ``ConsoleLogger`` class writes logs to the console. DEBUG and INFO messages
go to standard output; the other levels go to standard error. The constructor
takes no arguments. ::

  #include <cstdlib>
  #include <map>
  #include <string>
  
  #include <klogger/console.hh>
  
  
  int
  main(int argc, char *argv[])
  {
          std::string             name = std::string(argv[0]);
          klog::ConsoleLogger     console(name);
          std::map<std::string, std::string>      args;
  
          if (!console.good()) {
                  ::abort();
          }
  
          console.debug("main", "starts");
  
          for (int i = 1; i < argc; i++) {
                  auto    k = "argv[" + std::to_string(i) + "]";
                  args[k] = std::string(argv[i]);
          }
  
          console.info("main", "starts", args);
          console.warn("main", "depleted");
          console.fatal("main", "ends");
  }

The ``close`` method closes standard input and standard error.


FileLogger
----------

The ``FileLogger`` class writes logs to files on disk in text form. There are
two constructors for this class.

The first creates a new file logger where all log messages are
written to ``logfile``. If ``truncate`` is true, the logfile will
be truncated at initialisation. If the log file does not exist, it
will be created.::

        FileLogger(std::string logfile, bool truncate);

The second creates a new file logger where debug and info messages
are written to ``logfile``, and all other messages are written to
``errfile``. If ``truncate`` is true, the logfiles will be truncated
at initialisation. If the files do not exist, they will be created.::

        FileLogger(std::string logfile, std::string errfile, bool truncate);

Each of these forms has a function that provides the same functionality
for where a constructor may not be appropriate::

  FileLogger      *open_logfile(std::string logfile, bool truncate);
  FileLogger      *open_logfile(std::string logfile, std::string errfile,
                                bool truncate);

The ``close`` function closes the file handles.


Syslogger
---------

The ``Syslogger`` class writes logs to the syslog(1) facility.

Under the ``klog::syslog`` namespace, there are two enumerations
defined that are important: the ``Facility`` and ``Option``::

  namespace syslog {
          enum class Facility : int {
                  Auth = LOG_AUTH,
                  AuthPriv = LOG_AUTHPRIV,
                  Cron = LOG_CRON,
                  Daemon = LOG_DAEMON,
                  FTP = LOG_FTP,
                  Local0 = LOG_LOCAL0,
                  Local1 = LOG_LOCAL1,
                  Local2 = LOG_LOCAL2,
                  Local3 = LOG_LOCAL3,
                  Local4 = LOG_LOCAL4,
                  Local5 = LOG_LOCAL5,
                  Local6 = LOG_LOCAL6,
                  Local7 = LOG_LOCAL7,
                  LPR = LOG_LPR,
                  Mail = LOG_MAIL,
                  News = LOG_NEWS,
                  User = LOG_USER,
          };
  
          enum class Option : int {
                  // Log to the console if syslog can't be opened.
                  Console = LOG_CONS,
  
                  // Open the connection to syslog immediately; the
                  // default is to open the log on the first log
                  // message.
                  NoDelay = LOG_NDELAY,
  
                  // Don't wait for child processes that may be
                  // created while logging; on most platforms,
                  // this has no effect.
                  NoWait = LOG_NOWAIT,
  
                  // Also write log messages to standard error.
                  PError = LOG_PERROR,
  
                  // Log the process ID with each message.
                  PID = LOG_PID,
          };
  } // namespace syslog

The ``Facility`` enumeration corresponds to the facility from the
``syslog(3)`` manpage. The constructor takes an initialiser list
of ``klog::syslog::Options``.

For example, from ``src/syslog_test.cc``::

        klog::Syslogger         logger(name, klog::syslog::Facility::Daemon,
                                       {klog::syslog::Option::Console,
                                        klog::syslog::Option::PID,
                                        klog::syslog::Option::PError});

The ``close`` method calls the ``closelog(3)`` function.


