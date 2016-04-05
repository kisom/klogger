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


#include <fstream>
#include <map>
#include <unistd.h>

#include <klogger/logger.hh>
#include <klogger/filelog.hh>
#include <internal.hh>


namespace klog {


constexpr auto	DEFAULT_FLAGS = std::ofstream::out | std::ofstream::app;
constexpr auto	TRUNC_FLAGS = std::ofstream::out | std::ofstream::trunc;


FileLogger::FileLogger(std::string logfile, bool truncate)
    : outs(nullptr), errs(nullptr), ilevel(DEFAULT_LEVEL),
      err(LogError::HEALTHY)
{
	auto	flags = DEFAULT_FLAGS;

	if (truncate) {
		flags = TRUNC_FLAGS;
	}

	try {
		this->outs.open(logfile, flags);
		this->errs.open(logfile, DEFAULT_FLAGS);
	}
	catch (std::ifstream::failure &e) {
		this->err = LogError::ERR_OPEN;
		return;
	}

	if (!this->outs.good()) {
		this->err = LogError::ERR_OPEN;
		return;
	}
	else if (!this->errs.good()) {
		this->err = LogError::ERR_OPEN;
		return;
	}
}


FileLogger::FileLogger(std::string logfile, std::string errfile,
		       bool truncate)
    : outs(nullptr), errs(nullptr), ilevel(DEFAULT_LEVEL),
      err(LogError::HEALTHY)
{
	auto	flags = DEFAULT_FLAGS;

	if (truncate) {
		flags = TRUNC_FLAGS;
	}

	try {
		this->outs.open(logfile, flags);
		this->errs.open(errfile, flags);
	}
	catch (std::ifstream::failure &e) {
		this->err = LogError::ERR_OPEN;
		return;
	}


	if (!this->outs.good()) {
		this->err = LogError::ERR_OPEN;
		return;
	}
	else if (!this->errs.good()) {
		this->err = LogError::ERR_OPEN;
		return;
	}
}

void
FileLogger::debug(const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::DEBUG);
	write_log(this->outs, Level::DEBUG, actor, event, attrs);
}


void
FileLogger::debug(const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::DEBUG);
	write_log(this->outs, Level::DEBUG, actor, event, {});
}


void
FileLogger::info(const std::string& actor,
		    const std::string& event,
		    std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::INFO);
	write_log(this->outs, Level::INFO, actor, event, attrs);
}


void
FileLogger::info(const std::string& actor,
		    const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::INFO);
	write_log(this->outs, Level::INFO, actor, event, {});
}


void
FileLogger::warn(const std::string& actor,
		    const std::string& event,
		    std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::WARN);
	write_log(this->errs, Level::WARN, actor, event, attrs);
}


void
FileLogger::warn(const std::string& actor,
		    const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::WARN);
	write_log(this->errs, Level::WARN, actor, event, {});
}


void
FileLogger::error(const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::ERROR);
	write_log(this->errs, Level::ERROR, actor, event, attrs);
}


void
FileLogger::error(const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::ERROR);
	write_log(this->errs, Level::ERROR, actor, event, {});
}


void
FileLogger::critical(const std::string& actor,
			const std::string& event,
			std::map<std::string, std::string> attrs) 
{
	LEVEL_CHECK(this->ilevel, Level::CRITICAL);
	write_log(this->errs, Level::CRITICAL, actor, event, attrs);
}


void
FileLogger::critical(const std::string& actor,
			const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::CRITICAL);
	write_log(this->errs, Level::CRITICAL, actor, event, {});
}


void
FileLogger::fatal(const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(this->errs, Level::FATAL, actor, event, attrs);
	exit(EXIT_FAILURE);
}


void
FileLogger::fatal(const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(this->errs, Level::FATAL, actor, event, {});
	exit(EXIT_FAILURE);
}


void
FileLogger::fatal(int exitcode,
		     const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(this->errs, Level::FATAL, actor, event, attrs);
	exit(exitcode);
}


void
FileLogger::fatal(int exitcode,
		     const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(this->errs, Level::FATAL, actor, event, {});
	exit(exitcode);
}


void
FileLogger::fatal_noexit(const std::string& actor,
			    const std::string& event,
			    std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(this->errs, Level::FATAL, actor, event, attrs);
}


void
FileLogger::fatal_noexit(const std::string& actor,
			    const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_log(this->errs, Level::FATAL, actor, event, {});
}


void
FileLogger::level(Level l)
{
	this->ilevel = l;
}


bool
FileLogger::good()
{
	return this->err == LogError::HEALTHY;
}


LogError
FileLogger::error()
{
	return this->err;
}


int
FileLogger::close()
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


FileLogger *
open_logfile(std::string logfile, bool truncate)
{
	return new FileLogger(logfile, truncate);
}


FileLogger *
open_logfile(std::string logfile, std::string errfile,
	     bool truncate)
{
	return new FileLogger(logfile, errfile, truncate);
}


} // namespace klog
