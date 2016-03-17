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


#ifndef __KLOGGER_INTERNAL_HH__
#define __KLOGGER_INTERNAL_HH__


#include <map>
#include <ostream>

#include <klogger/logger.hh>


// Determine whether to write the log message based on the current
// level. l is the logger's current minimum logging level, and c is the
// log level for the current method. For example,
//   void
//   Logger::info(std::string actor, std::string event)
//   {
//           LEVEL_CHECK(this->ilevel, Level::DEBUG);
//           // do the actual logging
//   }
#define LEVEL_CHECK(l, c) if ((l) > (c)) { return; }

namespace klog {

constexpr auto	date_format = "%FT%T%z";

static std::map<Level, std::string> level_strings = {
	{Level::DEBUG,    "DEBUG"},
	{Level::INFO,     "INFO"},
	{Level::WARN,     "WARNING"},
	{Level::ERROR,    "ERROR"},
	{Level::CRITICAL, "CRITICAL"},
	{Level::FATAL,    "FATAL"}
};

std::string	timestamp(void);

// Standard functions for writing out log messages and building
// strings. The _nt variants do not log timestamps, expecting that
// the logging backend is also adding timestamps.
std::ostream&	write_log(std::ostream& outs, 
			  Level level,
			  std::string actor,
			  std::string event,
			  std::map<std::string, std::string> attrs);
std::string	log_to_string(Level level,
			      std::string actor,
			      std::string event,
			      std::map<std::string, std::string> attrs);
std::string	log_to_string(std::string actor,
			      std::string event,
			      std::map<std::string, std::string> attrs);
std::ostream&	write_log_nt(std::ostream& outs, 
			     Level level,
			     std::string actor,
			     std::string event,
			     std::map<std::string, std::string> attrs);
std::string	log_to_string_nt(Level level,
			         std::string actor,
				 std::string event,
				 std::map<std::string, std::string> attrs);

} // namespace klog


#endif // #ifndef __KLOGGER_INTERNAL_HH__
