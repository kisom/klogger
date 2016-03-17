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


#include <iostream>
#include <map>
#include <unistd.h>

#include <klogger/logger.hh>
#include <klogger/console.hh>
#include <internal.hh>


namespace klog {


void
ConsoleLogger::debug(const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::DEBUG);
	write_log(std::cout, Level::DEBUG, actor, event, attrs);
}


void
ConsoleLogger::debug(const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::DEBUG);
	write_log(std::cout, Level::DEBUG, actor, event, {});
}


void
ConsoleLogger::info(const std::string& actor,
		    const std::string& event,
		    std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::INFO);
	write_log(std::cout, Level::INFO, actor, event, attrs);
}


void
ConsoleLogger::info(const std::string& actor,
		    const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::INFO);
	write_log(std::cout, Level::INFO, actor, event, {});
}


void
ConsoleLogger::warn(const std::string& actor,
		    const std::string& event,
		    std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::WARN);
	write_log(std::cerr, Level::WARN, actor, event, attrs);
}


void
ConsoleLogger::warn(const std::string& actor,
		    const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::WARN);
	write_log(std::cerr, Level::WARN, actor, event, {});
}


void
ConsoleLogger::error(const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::ERROR);
	write_log(std::cerr, Level::ERROR, actor, event, attrs);
}


void
ConsoleLogger::error(const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::ERROR);
	write_log(std::cerr, Level::ERROR, actor, event, {});
}


void
ConsoleLogger::critical(const std::string& actor,
			const std::string& event,
			std::map<std::string, std::string> attrs) 
{
	LEVEL_CHECK(this->ilevel, Level::CRITICAL);
	write_log(std::cerr, Level::CRITICAL, actor, event, attrs);
}


void
ConsoleLogger::critical(const std::string& actor,
			const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::CRITICAL);
	write_log(std::cerr, Level::CRITICAL, actor, event, {});
}


void
ConsoleLogger::fatal(const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(std::cerr, Level::FATAL, actor, event, attrs);
	exit(EXIT_FAILURE);
}


void
ConsoleLogger::fatal(const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(std::cerr, Level::FATAL, actor, event, {});
	exit(EXIT_FAILURE);
}


void
ConsoleLogger::fatal(int exitcode,
		     const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(std::cerr, Level::FATAL, actor, event, attrs);
	exit(exitcode);
}


void
ConsoleLogger::fatal(int exitcode,
		     const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(std::cerr, Level::FATAL, actor, event, {});
	exit(exitcode);
}


void
ConsoleLogger::fatal_noexit(const std::string& actor,
			    const std::string& event,
			    std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(std::cerr, Level::FATAL, actor, event, attrs);
}


void
ConsoleLogger::fatal_noexit(const std::string& actor,
			    const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(std::cerr, Level::FATAL, actor, event, {});
}


void
ConsoleLogger::level(Level l)
{
	this->ilevel = l;
}


bool
ConsoleLogger::good()
{
	return this->err == LogError::HEALTHY;
}


LogError
ConsoleLogger::error()
{
	return this->err;
}


int
ConsoleLogger::close()
{
	if (-1 == ::close(STDOUT_FILENO)) {
		this->err = LogError::ERR_CLOSEFAIL;
		return -1;
	}

	if (-1 == ::close(STDERR_FILENO)) {
		this->err = LogError::ERR_CLOSEFAIL;
		return -1;
	}

	this->err = LogError::ERR_CLOSED;
	return 0;
}


} // namespace klog
