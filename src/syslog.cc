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
#include <syslog.h>
#include <unistd.h>

#include <klogger/logger.hh>
#include <klogger/syslog.hh>
#include <internal.hh>


namespace klog {


Syslogger::Syslogger(std::string name, syslog::Facility f,
		     std::initializer_list<syslog::Option> opts)
    : ident(name), err(LogError::HEALTHY), ilevel(DEFAULT_LEVEL)
{
	int	options = 0;

	for (auto opt : opts) {
		options |= static_cast<int>(opt);
	}

	::openlog(name.c_str(), options, static_cast<int>(f));
}


void
Syslogger::debug(const std::string& actor,
		     const std::string& event,
		     std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::DEBUG);

	std::string	out = log_to_string_nt(Level::DEBUG, actor, event, attrs);
	::syslog(LOG_DEBUG, "%s", out.c_str());
}


void
Syslogger::debug(const std::string& actor,
		 const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::DEBUG);

	std::string	out = log_to_string_nt(Level::DEBUG, actor, event, {});
	::syslog(LOG_DEBUG, "%s", out.c_str());
}


void
Syslogger::info(const std::string& actor,
		const std::string& event,
		std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::INFO);

	std::string	out = log_to_string_nt(Level::INFO, actor, event, attrs);
	::syslog(LOG_INFO, "%s", out.c_str());
}


void
Syslogger::info(const std::string& actor,
		const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::INFO);

	std::string	out = log_to_string_nt(Level::INFO, actor, event, {});
	::syslog(LOG_INFO, "%s", out.c_str());
}


void
Syslogger::warn(const std::string& actor,
		const std::string& event,
		std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::WARN);

	std::string	out = log_to_string_nt(Level::WARN, actor, event, attrs);
	::syslog(LOG_WARNING, "%s", out.c_str());
}


void
Syslogger::warn(const std::string& actor,
		const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::WARN);

	std::string	out = log_to_string_nt(Level::WARN, actor, event, {});
	::syslog(LOG_WARNING, "%s", out.c_str());
}


void
Syslogger::error(const std::string& actor,
		 const std::string& event,
		 std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::ERROR);

	std::string	out = log_to_string_nt(Level::ERROR, actor, event, attrs);
	::syslog(LOG_ERR, "%s", out.c_str());
}


void
Syslogger::error(const std::string& actor,
		 const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::ERROR);

	std::string	out = log_to_string_nt(Level::ERROR, actor, event, {});
	::syslog(LOG_ERR, "%s", out.c_str());
}


void
Syslogger::critical(const std::string& actor,
		    const std::string& event,
		    std::map<std::string, std::string> attrs) 
{
	LEVEL_CHECK(this->ilevel, Level::CRITICAL);

	std::string	out = log_to_string_nt(Level::CRITICAL, actor, event, attrs);
	::syslog(LOG_CRIT, "%s", out.c_str());
}


void
Syslogger::critical(const std::string& actor,
		    const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::CRITICAL);

	std::string	out = log_to_string_nt(Level::CRITICAL, actor, event, {});
	::syslog(LOG_CRIT, "%s", out.c_str());
}


void
Syslogger::fatal(const std::string& actor,
		 const std::string& event,
		 std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);

	std::string	out = log_to_string_nt(Level::FATAL, actor, event, attrs);
	::syslog(LOG_EMERG, "%s", out.c_str());
	exit(EXIT_FAILURE);
}


void
Syslogger::fatal(const std::string& actor,
		 const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);

	std::string	out = log_to_string_nt(Level::FATAL, actor, event, {});
	::syslog(LOG_EMERG, "%s", out.c_str());
	exit(EXIT_FAILURE);
}


void
Syslogger::fatal(int exitcode,
		 const std::string& actor,
		 const std::string& event,
		 std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);

	std::string	out = log_to_string_nt(Level::FATAL, actor, event, attrs);
	::syslog(LOG_EMERG, "%s", out.c_str());
	exit(exitcode);
}


void
Syslogger::fatal(int exitcode,
		 const std::string& actor,
		 const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);

	std::string	out = log_to_string_nt(Level::FATAL, actor, event, {});
	::syslog(LOG_EMERG, "%s", out.c_str());
	exit(exitcode);
}


void
Syslogger::fatal_noexit(const std::string& actor,
			const std::string& event,
			std::map<std::string, std::string> attrs)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);

	std::string	out = log_to_string_nt(Level::FATAL, actor, event, attrs);
	::syslog(LOG_EMERG, "%s", out.c_str());
}


void
Syslogger::fatal_noexit(const std::string& actor,
			const std::string& event)
{
	LEVEL_CHECK(this->ilevel, Level::FATAL);

	std::string	out = log_to_string_nt(Level::FATAL, actor, event, {});
	::syslog(LOG_EMERG, "%s", out.c_str());
}


void
Syslogger::level(Level l)
{
	this->ilevel = l;
}


bool
Syslogger::good()
{
	return this->err == LogError::HEALTHY;
}


LogError
Syslogger::error()
{
	return this->err;
}


int
Syslogger::close()
{
	::closelog();
	this->err = LogError::ERR_CLOSED;
	return 0;
}


} // namespace klog
