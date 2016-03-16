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


#ifndef __KLOGGER_CONSOLE_HH__
#define __KLOGGER_CONSOLE_HH__


#include <map>

#include <klogger/logger.hh>


namespace klog {


class ConsoleLogger : Logger {
public:
	ConsoleLogger(std::string name) :
		iname(name), err(LogError::HEALTHY), ilevel(DEFAULT_LEVEL) {};
	~ConsoleLogger(void) {};

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
private:
	std::string	iname;
	LogError	err;
	Level		ilevel;
};


} // namespace klog


#endif // #ifndef __KLOGGER_CONSOLE_HH__
