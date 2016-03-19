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


#ifndef __KLOGGER_BINLOG_HH__
#define __KLOGGER_BINLOG_HH__


#include <fstream>
#include <map>

#include <klogger/logger.hh>


namespace klog {

// BinLogger writes logs to disk in a binary format.
class BinLogger : Logger {
public:
	// Create a new file logger where all messages are written
	// to logfile. If truncate is true, the logfile will be
	// truncated at initialisation. If the log file does not
	// exist, it will be created.
	BinLogger(std::string logfile, bool truncate);

	// Create a new file logger where debug and info messages
	// are written to logfile, and all other messages are written
	// to errfile. If truncate is true, the logfiles will be
	// truncated at initialisation. If the files do not exist,
	// they will be created.
	BinLogger(std::string logfile, std::string errfile, bool truncate);

	~BinLogger() {};

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
	std::ofstream	outs;
	std::ofstream	errs;
	Level		ilevel;
	LogError	err;

	inline void	set_err(void) {
		if (this->outs.good() && this->errs.good()) {
			this->err = LogError::HEALTHY;
		}
		else if (this->outs.bad() || this->errs.bad()) {
			this->err = LogError::ERR_DISK;
			return;
		}
		else if (this->outs.fail() || this->errs.fail()) {
			this->err = LogError::ERR_DISK;
		}
	}
};


BinLogger	*open_binlogfile(std::string logfile, bool truncate);
BinLogger	*open_binlogfile(std::string logfile, std::string errfile,
				 bool truncate);


} // namespace klog


#endif // #ifndef __KLOGGER_BINLOG_HH__
