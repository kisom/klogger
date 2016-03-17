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


#ifndef __KLOGGER_SYSLOG_HH__
#define __KLOGGER_SYSLOG_HH__


#include <map>
#include <syslog.h>

#include <klogger/logger.hh>


namespace klog {
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
		Console = LOG_CONS,
		NoDelay = LOG_NDELAY,
		NoWait = LOG_NOWAIT,
		PError = LOG_PERROR,
		PID = LOG_PID,
	};
} // namespace syslog


// Syslogger writes its logs to syslogd. Note that the nature of the
// syslog(3) facilities means that any other use of those functions
// outside of this logger in the program will be affected by this
// program (e.g. if a call is made to openlog("some other name")).
// The use of Syslogger with calls to syslog(3) is not recommended.
class Syslogger : Logger {
public:
	Syslogger(std::string, syslog::Facility,
		  std::initializer_list<syslog::Option>);
	~Syslogger() {};

	// debug writes a log message with the DEBUG level.
	void debug(const std::string& actor,
		   const std::string& event,
		   std::map<std::string, std::string> attrs);
	void debug(const std::string& actor,
		   const std::string& event);

	// info writes a log message with the INFO level.
	void info(const std::string& actor,
		  const std::string& event,
		  std::map<std::string, std::string> attrs);
	void info(const std::string& actor,
		  const std::string& event);

	// warn writes a log message with the WARN level.
	void warn(const std::string& actor,
		  const std::string& event,
		  std::map<std::string, std::string> attrs);
	void warn(const std::string& actor,
		  const std::string& event);

	// error writes a log message with the ERROR level.
	void error(const std::string& actor,
		   const std::string& event,
		   std::map<std::string, std::string> attrs);
	void error(const std::string& actor,
		   const std::string& event);

	// critical writes a log message with the CRITICAL level.
	void critical(const std::string& actor,
		      const std::string& event,
		      std::map<std::string, std::string> attrs);
	void critical(const std::string& actor,
		      const std::string& event);

	// fatal writes a log message with the FATAL level. The process
	// will exit with exit code EXIT_FAILURE.
	void fatal(const std::string& actor,
		   const std::string& event,
		   std::map<std::string, std::string> attrs);
	void fatal(const std::string& actor,
		   const std::string& event);

	// fatal writes a log message with the FATAL level. The process
	// will exit with exitcode.
	void fatal(int exitcode,
		   const std::string& actor,
		   const std::string& event,
		   std::map<std::string, std::string> attrs);
	void fatal(int exitcode,
		   const std::string& actor,
		   const std::string& event);
   
	// fatal_noexit writes a log message with the FATAL level but
	// does not exit; the caller is expected to handle exiting the
	// process after any cleanup.
	void fatal_noexit(const std::string& actor,
			  const std::string& event,
			  std::map<std::string, std::string> attrs);
	void fatal_noexit(const std::string& actor,
			  const std::string& event);
   
	// level sets the minimum logging level.
	void            level(Level);

	// good returns true if the logger is healthy.
	bool            good(void);

	// error returns the current error condition for a logger.
	LogError        error(void);

	// close provides a mechanism for shutting down a logger.
	int		close(void);

private:
	std::string	ident;
	LogError	err;
	Level		ilevel;
};

} // namespace klog


#endif // #ifndef __KLOGGER_SYSLOG_HH__
