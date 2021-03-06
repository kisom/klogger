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


#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <unistd.h>

#include <klogger/logger.hh>
#include <klogger/binlog.hh>
#include <klogger/tlv.hh>
#include <internal.hh>


namespace klog {


constexpr auto	BIN_DEFAULT_FLAGS = std::ofstream::out | std::ofstream::app;
constexpr auto	BIN_TRUNC_FLAGS = std::ofstream::out | std::ofstream::trunc;


static void
write_tlv_log(std::ostream& outs,
	      Level level,
	      const std::string& actor,
	      const std::string& event,
	      std::map<std::string, std::string> attrs)
{
	auto		lvl = static_cast<std::underlying_type<Level>::type>(level);

	if (tlv::write_tlv_log(outs, lvl, actor, event, attrs)) {
		outs.flush();
	}
}

BinLogger::BinLogger(std::string logfile, bool truncate)
    : outs(nullptr), errs(nullptr), ilevel(DEFAULT_LEVEL),
      err(LogError::HEALTHY)
{
	auto	flags = BIN_DEFAULT_FLAGS;

	if (truncate) {
		flags = BIN_TRUNC_FLAGS;
	}

	try {
		this->outs.open(logfile, flags);
		this->errs.open(logfile, BIN_DEFAULT_FLAGS);
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


BinLogger::BinLogger(std::string logfile, std::string errfile,
		       bool truncate)
    : outs(nullptr), errs(nullptr), ilevel(DEFAULT_LEVEL),
      err(LogError::HEALTHY)
{
	auto	flags = BIN_DEFAULT_FLAGS;

	if (truncate) {
		flags = BIN_TRUNC_FLAGS;
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
BinLogger::debug(const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::DEBUG);
	write_tlv_log(this->outs, Level::DEBUG, actor, event, attrs);
	this->set_err();
}


void
BinLogger::debug(const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::DEBUG);
	write_tlv_log(this->outs, Level::DEBUG, actor, event, {});
	this->set_err();
}


void
BinLogger::info(const std::string& actor,
		    const std::string& event,
		    std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::INFO);
	write_tlv_log(this->outs, Level::INFO, actor, event, attrs);
	this->set_err();
}


void
BinLogger::info(const std::string& actor,
		    const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::INFO);
	write_tlv_log(this->outs, Level::INFO, actor, event, {});
	this->set_err();
}


void
BinLogger::warn(const std::string& actor,
		    const std::string& event,
		    std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::WARN);
	write_tlv_log(this->errs, Level::WARN, actor, event, attrs);
	this->set_err();
}


void
BinLogger::warn(const std::string& actor,
		    const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::WARN);
	write_tlv_log(this->errs, Level::WARN, actor, event, {});
	this->set_err();
}


void
BinLogger::error(const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::ERROR);
	write_tlv_log(this->errs, Level::ERROR, actor, event, attrs);
	this->set_err();
}


void
BinLogger::error(const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::ERROR);
	write_tlv_log(this->errs, Level::ERROR, actor, event, {});
	this->set_err();
}


void
BinLogger::critical(const std::string& actor,
			const std::string& event,
			std::map<std::string, std::string> attrs) 
{
	LEVEL_CHECK(this->ilevel, Level::CRITICAL);
	write_tlv_log(this->errs, Level::CRITICAL, actor, event, attrs);
	this->set_err();
}


void
BinLogger::critical(const std::string& actor,
			const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::CRITICAL);
	write_tlv_log(this->errs, Level::CRITICAL, actor, event, {});
	this->set_err();
}


void
BinLogger::fatal(const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_tlv_log(this->errs, Level::FATAL, actor, event, attrs);
	this->set_err();
	exit(EXIT_FAILURE);
}


void
BinLogger::fatal(const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_tlv_log(this->errs, Level::FATAL, actor, event, {});
	this->set_err();
	exit(EXIT_FAILURE);
}


void
BinLogger::fatal(int exitcode,
		     const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_tlv_log(this->errs, Level::FATAL, actor, event, attrs);
	this->set_err();
	exit(exitcode);
}


void
BinLogger::fatal(int exitcode,
		     const std::string& actor,
		     const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_tlv_log(this->errs, Level::FATAL, actor, event, {});
	this->set_err();
	exit(exitcode);
}


void
BinLogger::fatal_noexit(const std::string& actor,
			    const std::string& event,
			    std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_tlv_log(this->errs, Level::FATAL, actor, event, attrs);
	this->set_err();
}


void
BinLogger::fatal_noexit(const std::string& actor,
			    const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);
	write_tlv_log(this->errs, Level::FATAL, actor, event, {});
	this->set_err();
}


void
BinLogger::level(Level l)
{
	this->ilevel = l;
}


bool
BinLogger::good()
{
	return this->err == LogError::HEALTHY;
}


LogError
BinLogger::error()
{
	return this->err;
}


int
BinLogger::close()
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


BinLogger *
open_binlogfile(std::string logfile, bool truncate)
{
	return new BinLogger(logfile, truncate);
}


BinLogger *
open_binlogfile(std::string logfile, std::string errfile,
	     bool truncate)
{
	return new BinLogger(logfile, errfile, truncate);
}


} // namespace klog
