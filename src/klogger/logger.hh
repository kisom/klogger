/*
 * Copyright (c) 2016 K. Isom <coder@kyleisom.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * copy of this  software and associated documentation  files (the "Software"),
 * to deal  in the Software  without restriction, including  without limitation
 * the rights  to use,  copy, modify,  merge, publish,  distribute, sublicense,
 * and/or  sell copies  of the  Software,  and to  permit persons  to whom  the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS  PROVIDED "AS IS", WITHOUT WARRANTY OF  ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING  BUT NOT  LIMITED TO  THE WARRANTIES  OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS  OR COPYRIGHT  HOLDERS BE  LIABLE FOR  ANY CLAIM,  DAMAGES OR  OTHER
 * LIABILITY,  WHETHER IN  AN ACTION  OF CONTRACT,  TORT OR  OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */


#ifndef __KLOGGER_LOGGER_HH__
#define __KLOGGER_LOGGER_HH__


#include <map>


namespace klog {


// Level denotes the lowest level of log messages that will be output. The
// list is presented in order of least level to highest; that is, DEBUG
// < INFO.
enum class Level : int {
	// DEBUG is a debug-level log message. These are only used during
	// development or if a deployed system repeatedly sees abnormal
	// errors.
	DEBUG,

	// INFO is an informational log message. This is a normal
	// log message that is used to deliver information, such as
	// recording requests. Ops teams are never paged for informational
	// messages. This is the default log level.
	INFO,

	// WARN is a warning message. An example of this is a bad request
	// sent to a server. This isn't an error on the part of the program,
	// but it may be indicative of other things. Like errors, the ops
	// team shouldn't be paged for errors, but a page might be triggered
	// if a certain threshold of warnings is reached (which is typically
	// much higher than errors). For example, repeated warnings might
	// be a sign that the system is under attack.
	WARN,

	// ERROR is an error message.  A single error doesn't require an
	// ops team to be paged, but repeated errors should often trigger
	// a page based on threshold triggers. An example is a network
	// failure: it might be a transient failure (these do happen),
	// but most of the time it's self-correcting.
	ERROR,

	// CRITICAL is a message for critical conditions. The error,
	// if uncorrected, is likely to cause a fatal condition shortly.
	// An example is running out of disk space. This is something that
	// the ops team should get paged for.
	CRITICAL,

	// FATAL indicates the system is unusable, the equivalent of
	// syslog's ``emerg`` level ([EMERG]); this has the additional
	// effect of exiting. An example is a self-check assertion failure,
	// indicating that the system is in an inconsistent state. The ops
	// team would be paged for this condition.
	FATAL,
};

constexpr Level	DEFAULT_LEVEL = Level::INFO;

   
// A LogError describes an error condition for a logger. This error
// indicates the reason that the logger cannot write log messages.
enum class LogError {
	// The logger is currently healthy.
	HEALTHY,

	// The logger has been closed.
	ERR_CLOSED,

	// The logger couldn't be opened: maybe the TTY is unavailable for
	// writing, or a log file couldn't be opened.
	ERR_OPEN,
   
	// The logger failed due to insufficient system permissions.
	ERR_NOPERM,
   
	// The logger couldn't write to disk due to running out of disk space.
	ERR_DISK,
   
	// The logger couldn't write a message because the resource is
	// unavailable. There might be a network failure, or a file stream
	// might have been closed.
	ERR_UNAVAILABLE,

	// The logger couldn't close itself.
	ERR_CLOSEFAIL,
   
	// If the error doesn't meet one of the above categories, let the user
	// know that we didn't think about this condition.  Normally, I'd
	// ``abort()`` here, but that should left to the user's discretion.
	ERR_UNKNOWN,
};

// A Logger is a handle to some logging backend that log messages should
// be written to.
class Logger {
public:
	virtual
	~Logger() {};

	// debug writes a log message with the DEBUG level.
	virtual
	void debug(const std::string& actor,
		   const std::string& event,
		   std::map<std::string, std::string> attrs) = 0;
	virtual
	void debug(const std::string& actor,
		   const std::string& event) = 0;

	// info writes a log message with the INFO level.
	virtual
	void info(const std::string& actor,
		  const std::string& event,
		  std::map<std::string, std::string> attrs) = 0;
	virtual
	void info(const std::string& actor,
		  const std::string& event) = 0;

	// warn writes a log message with the WARN level.
	virtual
	void warn(const std::string& actor,
		  const std::string& event,
		  std::map<std::string, std::string> attrs) = 0;
	virtual
	void warn(const std::string& actor,
		  const std::string& event) = 0;

	// error writes a log message with the ERROR level.
	virtual
	void error(const std::string& actor,
		   const std::string& event,
		   std::map<std::string, std::string> attrs) = 0;
	virtual
	void error(const std::string& actor,
		   const std::string& event) = 0;

	// critical writes a log message with the CRITICAL level.
	virtual
	void critical(const std::string& actor,
		      const std::string& event,
		      std::map<std::string, std::string> attrs) = 0;
	virtual
	void critical(const std::string& actor,
		      const std::string& event) = 0;

	// fatal writes a log message with the FATAL level. The process
	// will exit with exit code EXIT_FAILURE.
	virtual
	void fatal(const std::string& actor,
		   const std::string& event,
		   std::map<std::string, std::string> attrs) = 0;
	virtual
	void fatal(const std::string& actor,
		   const std::string& event) = 0;

	// fatal writes a log message with the FATAL level. The process
	// will exit with exitcode.
	virtual
	void fatal(int exitcode,
		   const std::string& actor,
		   const std::string& event,
		   std::map<std::string, std::string> attrs) = 0;
	virtual
	void fatal(int exitcode,
		   const std::string& actor,
		   const std::string& event) = 0;
   
	// fatal_noexit writes a log message with the FATAL level but
	// does not exit; the caller is expected to handle exiting the
	// process after any cleanup.
	virtual
	void fatal_noexit(const std::string& actor,
			  const std::string& event,
			  std::map<std::string, std::string> attrs) = 0;
	virtual
	void fatal_noexit(const std::string& actor,
			  const std::string& event) = 0;
   
	// level sets the minimum logging level.
	virtual
	void            level(Level) = 0;

	// good returns true if the logger is healthy.
	virtual
	bool            good(void) = 0;

	// error returns the current error condition for a logger.
	virtual
	LogError        error(void) = 0;
	  
	// close provides a mechanism for shutting down a logger. Generally,
	// a closed logger should return ERR_CLOSED once it is closed.
	virtual
	int		close(void) = 0;
};


} // namespace klog


#endif // #ifndef __KLOGGER_LOGGER_HH__
