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


#ifndef __KLOGGER_TLV_HH
#define __KLOGGER_TLV_HH


#include <cstdint>
#include <map>
#include <ostream>
#include <string>


namespace klog {
namespace tlv {


// The following constants denote tags.
constexpr std::uint8_t	TLogEntry =	0x01;
constexpr std::uint8_t	TTimestamp =	0x02;
constexpr std::uint8_t	TLevel =	0x04;
constexpr std::uint8_t	TString =	0x08;

// Utility functions.
std::string	hex_encode(const std::string&);
std::string	hex_encode(const char *, size_t);

// TLV serialisation support.
bool	write_length(std::ostream &outs, size_t length);
bool	write_timestamp(std::ostream &outs, uint64_t t);
bool	write_loglevel(std::ostream &outs, uint8_t lvl);
bool	write_string(std::ostream &outs, std::string s);
bool	write_header(std::ostream& outs, std::uint8_t tag, std::uint64_t length);
bool	write_tlv_log(std::ostream& outs, std::uint8_t lvl,
		      std::string actor, std::string event,
		      std::map<std::string, std::string> attrs);


} // namespace tlv
} // namespace klog


#endif // #ifndef __KLOGGER_TLV_HH
