Logger for C++11 Projects
==========================

Summary
-------

klog is a C++11 logging library that provides multiple logging back
ends.

After looking through a number of libraries, none of them are really
palatable. In general, they fall into one of more of the following
categories:

+ Ugly interfaces. Things like LOG_INIT(), or very C-style
  interfaces. I'd prefer a Go-style logging package that works the
  same way (in a sane way) whether logging to console, file, or
  syslog.
+ Bad license. The library needs to be MIT, BSD, or similar.
+ External dependencies. I'd rather avoid second- and third- order
  dependencies.
+ Not packaged. Ideally, it's apt-get installable even if via my own
  apt server. I don't mind doing the packaging, but I'd rather it be
  for a library I wanted to use.
+ Build issues. The projects that require a logging package use a
  rather restrictive set of build flags. Many candidate libraries
  would not compile under these flags, or would compile only with a
  specific compiler on a certain OS (ex., clang++ on Debian Linux).
+ No use of attributes. I've come to like loggers that use key-value
  pairs for log data. These generally produce messages that are easier
  to process with standard text tools (like ``awk(1)``).
  
The libraries I've explored include `spdlog
<https://github.com/gabime/spdlog>`_, glog (the Google logging
library), and log4cpp. Many candidate libraries were excluded due to
licensing or build issues. ``spdlog`` was the most promising (and had
the most palatable interface) but was plagued with build problems.

Usage sketches
--------------

Setting up a logger::

  // Open a console logger.
  auto log = logger::open_console(argv[0]);

  // Set a syslog handle.
  auto log = logger::open_syslog(argv[0], logger::syslog::LOG_CONS,
                                 logger::syslog::LOG_DAEMON);

  // Open a file for logging, appending new log messages. All messages
  // go to the same file. Candidate signature:
  // 
  //   logger::Logger& logger::open_logfile(std::string path, std::bool create);
  auto log = logger::open_logfile("myserviced.log", false);

  // Log to a file, creating a new log file. Debug and info messages
  // go to the service.log file; warning, error, and fatal go to
  // service.err. The ``create`` argument applies to both log files;
  // while providing separate arguments might be more flexible, in practice,
  // this isn't a useful feature. Candidate signature:
  //
  //   logger::Logger& logger::open_logfile(std::string outpath,
  //                                        std::string errpath,
  //                                        std::bool create);
  auto log = logger::open_logfile("service.log", "service.err", true);

  // Log to an ostream; all log messages go to the same stream. Candidate
  // signature:
  //
  //   logger::Logger& logger::to_ostream(std::ostream& outs);
  auto log = logger::open_logfile(cout);

  // Log to an ostream; Debug and info messages go to cout; warning,
  // error, and fatal message go to cerr. Candidate function
  //
  //   logger::Logger& logger::to_ostream(std::ostream& outs, std::ostream& errs);
  auto log = logger::open_logfile(cout);
  
Writing log messages::

  // Write an informational message. Candidate signature:
  //
  //   void info(const std::string actor, const std::string event,
  //             std::map<std::string, void *> attrs);
  log->info("servicemgr", "job start", {{"process_name", "myserviced"}, {"pid", 12345}});

  // outputs: [2016-03-16T04:45:32+0000] [INFO] [actor:servicemgr event:job start] process_name=myserviced pid=12345

  // Write a warning message.
  log->warn("servicemgr", "job exit", {{"process_name", "myserviced"}, {"pid", 12345}, {"exit_code", 127}});

  // outputs: [2016-03-16T05:17:26+0000] [WARNING] [actor:servicemgr event:job start] process_name=myserviced pid=12345 exit_code=127

The supported log levels mostly mirror those for syslog; the names
below are the names for the methods on a ``Logger``. I've found in
practice that differentiating between ``LOG_ALERT`` and ``LOG_CRIT``
isn't useful, and similarly that there isn't much difference between
``LOG_NOTICE`` and ``LOG_INFO``.

+ fatal: system is unusable, the equivalent of syslog's ``emerg``
  level ([EMERG]); this has the additional effect of exiting. An
  example is a self-check assertion failure, indicating that the
  system is in an inconsistent state. The ops team would be paged for
  this condition. This is the only function that has three different
  signatures::

    // Exits with exitcode.
    void Logger::fatal(int exitcode,
                       const std::string actor,
                       const std::string event,
                       std::map<std::string, void *> attrs);

    // Exits with EXIT_FAILURE.
    void Logger::fatal(const std::string actor,
                       const std::string event,
                       std::map<std::string, void *> attrs);

    // Doesn't exit; the caller is expected to exit the process after
    // any cleanup (or perhaps the caller wishes to dump core).
    void Logger::fatal(const std::string actor,
                       const std::string event,
                       std::map<std::string, void *> attrs);
			 
+ critical: critical conditions ([CRITICAL]). The error, if
  uncorrected, is likely to cause a fatal condition shortly.  An
  example is running out of disk space. This is something that the ops
  team should get paged for.
+ error: error conditions ([ERROR]). A single error doesn't require an
  ops team to be paged, but repeated errors should often trigger a
  page based on threshold triggers. An example is a network failure:
  it might be a transient failure (these do happen), but most of the
  time it's self-correcting.
+ warn: warning conditions ([WARNING]). An example of this is a bad
  request sent to a server. This isn't an error on the part of the
  program, but it may be indicative of other things. Like errors, the
  ops team shouldn't be paged for errors, but a page might be
  triggered if a certain threshold of warnings is reached (which is
  typically much higher than errors). For example, repeated warnings
  might be a sign that the system is under attack.
+ info: informational message ([INFO]). This is a normal log message
  that is used to deliver information, such as recording requests. Ops
  teams are never paged for informational messages. This is the
  default log level.
+ debug: debug-level message ([DEBUG]). These are only used during
  development or if a deployed system repeatedly sees abnormal errors.

In other log packages I've written or worked on, I've used other
levels; in practice, these six are the most useful.

Changing the level of a logger is mostly useful at start up::

  log->level(logger::Level::WARN);

It's generally preferred to avoid exceptions in libraries; as a consequence,
the following methods should be provided to ensure the logger is valid and to
return error information::

  // Returns true if the logger is in a healthy state.
  std::bool good(void);

  // The following can be used to provide additional error condition
  // checking:
  enum class LogError {
  
          // The logger couldn't be opened: maybe the TTY is
          // unavailable for writing, or a log file couldn't
	  // be opened.
          ERR_OPEN,

	  // The logger failed due to insufficient system permissions.
	  ERR_NOPERM,

	  // The logger couldn't write to disk due to running out of disk space.
	  ERR_DISK,

	  // The logger couldn't write a message because the resource
	  // is unavailable.
	  ERR_UNAVAILABLE,

	  // If the error doesn't meet one of the above categories,
	  // let the user know that we didn't think about this condition.
	  // Normally, I'd ``abort()`` here, but that should left to the
	  // user's discretion.
	  ERR_UNKNOWN,
  };
  logger::LogError error(void);


Requirements & Assumptions
--------------------------

There are three logging backends that I generally want to use:

+ the console
+ syslog
+ a log file

For the other cases, logging to an ``ostream`` should be supported.

Furthermore, there can be single-threaded and multi-threaded
loggers. Single-threaded loggers are useful where a logger isn't
shared across threads, and the performance gained by not locking is
desired.

A particular feature of this library is that it shouldn't depend on
anything but the C++11 standard library and a POSIX header set.

There's always going to be edge cases in text-based loggers; some
semantic data will be lost.

System Design
-------------

The library will centre around the concept of the ``Logger`` virtual
class. It might be useful to return a ``unique_ptr<Logger>``.

An example interface header, sans comments::

  enum class Level : int {
	  DEBUG,
	  INFO,
	  WARN,
	  ERROR,
	  CRITICAL,
	  FATAL,
  };
   
  enum class LogError {
    
	  // The logger couldn't be opened: maybe the TTY is unavailable for
	  // writing, or a log file couldn't be opened.
	  ERR_OPEN,
   
	  // The logger failed due to insufficient system permissions.
	  ERR_NOPERM,
   
	  // The logger couldn't write to disk due to running out of disk space.
	  ERR_DISK,
   
	  // The logger couldn't write a message because the resource is
	  // unavailable.
	  ERR_UNAVAILABLE,
   
	  // If the error doesn't meet one of the above categories, let the user
	  // know that we didn't think about this condition.  Normally, I'd
	  // ``abort()`` here, but that should left to the user's discretion.
	  ERR_UNKNOWN,
  };
   
  class Logger {
  public:
	  void debug(const std::string actor,
		     const std::string event,
		     std::map<std::string, void *> attrs);
	  void info(const std::string actor,
		    const std::string event,
		    std::map<std::string, void *> attrs);
	  void warn(const std::string actor,
		    const std::string event,
		    std::map<std::string, void *> attrs);
	  void error(const std::string actor,
		     const std::string event,
		     std::map<std::string, void *> attrs);
	  void critical(const std::string actor,
			const std::string event,
			std::map<std::string, void *> attrs);
	  void fatal(const std::string actor,
		     const std::string event,
		     std::map<std::string, void *> attrs);
	  void fatal(int exitcode,
		     const std::string actor,
		     const std::string event,
		     std::map<std::string, void *> attrs);
	  void fatal_noexit(const std::string actor,
		     const std::string event,
		     std::map<std::string, void *> attrs);		     
   
	  void            level(Level);
	  bool            good(void);
	  LogError        error(void);
	  
  };

Supportability
--------------

1. Exploration of failure modes:

   + Console logger: failure to open or write to a console.
     + Console might be unavailable.
     + Standard output or error might have been closed.
   + File logger: failure to open or write to a file.
     + File might be unavailable (general disk error).
     + The user might not have permissions to access the log file.
     + The disk might have run out of space.
   + Log corruption: I'm not sure how to detect this.
   + General principle: no exceptions, and no calls to abort(3).
2. The package will be designed for Unix-based systems. Particularly,
   support for Linux, BSD, and OS X is expected; OS X is not a hard
   requirement.
3. Packages should be provided for Debian-based systems. Packages for
   FreeBSD and OpenBSD should be explored.

Security
--------

+ Possibility of race conditions with files, for example using the
  logger to write to ``/etc/passwd``.
+ Generally, relies on OS protections.
+ If user-supplied data is sent as an attribute, it could potentially
  cause a buffer overflow.

Project Dependencies
--------------------

+ The project should not depend on any external libraries.
+ The project depends on a C++11 standard library. This guarantees a
  set of useful, modern constructs.

Open Issues
-----------

+ The attribute type remains an open issue. It should be a map of
  strings to ostream-able objects.

Milestones
----------

+ Version 1.0.0 (MVP): single-threaded console, syslog, and file
  logger.
+ Version 1.1.0: Add an ostream logger.
+ Version 1.2.0: Add multi-threaded loggers.

Review History
--------------

