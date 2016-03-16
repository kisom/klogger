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


#include <iomanip>
#include <map>
#include <iostream>
#include <sstream>
#include <string>

#include <klogger/logger.hh>
#include <internal.hh>


namespace klog {


constexpr size_t	TIMESTAMP_BUF_SIZE = 32;


std::string
timestamp()
{
	std::time_t		t = std::time(nullptr);
        std::tm			tm = *std::localtime(&t);
	std::stringstream	ss;
	char			buf[TIMESTAMP_BUF_SIZE];

	if (0 != std::strftime(buf, TIMESTAMP_BUF_SIZE, date_format, (const std::tm *)&tm)) {
		return std::to_string(t);
	}
	else {
		ss << buf;
		return ss.str();
	}
}


/*
 * N.B.: put_time doesn't seem to be widely available.
std::ostream&
timestamp(std::ostream& outs)
{
	std::time_t		t = std::time(nullptr);
        std::tm			tm = *std::localtime(&t);

	outs << std::put_time(&tm, date_format);
	return outs;
}
 */


std::ostream&
write_log(std::ostream& outs, 
	  Level level,
	  std::string actor,
	  std::string event,
	  std::map<std::string, std::string> attrs)
{
	auto	t = timestamp();
	outs << "[" << t << "] ";
	outs << "[" << level_strings[level] << "] ";
	outs << "[" << "actor:" << actor << " event:" << event << "]";

	for (auto it = attrs.begin(); it != attrs.end(); it++) {
		outs << " " << it->first << "=" << it->second;
	}

	outs << std::endl;
	return outs;
}


} // namespace klog
