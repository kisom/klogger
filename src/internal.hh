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


namespace klog {

constexpr auto	date_format = "%FT%R+z";

static std::map<Level, std::string> level_strings = {
	{Level::DEBUG,    "DEBUG"},
	{Level::INFO,     "INFO"},
	{Level::WARN,     "WARNING"},
	{Level::ERROR,    "ERROR"},
	{Level::CRITICAL, "CRITICAL"},
	{Level::FATAL,    "FATAL"}
};

std::string	timestamp(void);
std::ostream&	write_log(std::ostream& outs, 
			  Level level,
			  std::string actor,
			  std::string event,
			  std::map<std::string, std::string> attrs);
}


#endif // #ifndef __KLOGGER_INTERNAL_HH__
