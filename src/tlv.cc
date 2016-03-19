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
#include <cstring>
#include <iostream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <time.h>

#include <klogger/tlv.hh>


namespace klog {
namespace tlv {


static const char	hexlut[] = "0123456789ABCDEF";


std::string
hex_encode(const std::string& s)
{
	size_t		length = s.size();
	std::string	out;

	out.reserve(2 * length);
	for (size_t i = 0; i < length; i++) {
		unsigned char	c = s[i];
		out.push_back(hexlut[c >>  4]);
		out.push_back(hexlut[c & 0xF]);
	}

	return out;
}


std::string
hex_encode(const char *s, size_t length)
{
	std::string	out;

	out.reserve(2 * length);
	for (size_t i = 0; i < length; i++) {
		unsigned char	c = s[i];
		out.push_back(hexlut[c >>  4]);
		out.push_back(hexlut[c & 0xF]);
	}

	return out;
}


static inline void
write_tag(std::ostream& outs, std::uint8_t t)
{
	outs.write(reinterpret_cast<const char *>(&t), 1);
}


static inline size_t
length_octets(size_t length)
{
	std::uint8_t	loct = 1;

	if (length <= 0x7F) {
		return loct;
	}

	for (size_t i = (sizeof(length) * 8) - 1; i >= 1; i--) {
		size_t	bits = 1;
		bits <<= i;
		if (0 == (length & bits)) {
			continue;
		}
		loct = (i + 8) >> 3;
		break;
	}

	return loct;
}


bool
write_length(std::ostream &outs, size_t length)
{
	std::uint8_t	loct = 1;

	if (length <= 0x7F) {
		outs.write(reinterpret_cast<const char *>(&length), 1);
	}
	else {
		for (size_t i = (sizeof(length) * 8) - 1; i >= 1; i--) {
			size_t	bits = 1;
			bits <<= i;
			if (0 == (length & bits)) {
				continue;
			}
			loct = (i + 8) >> 3;
			break;
		}

		loct = 0x80 + loct;
		outs.write(reinterpret_cast<const char *>(&loct), 1);
		if (!outs.good()) {
			return false;
		}

		char	buf[8];

		::memcpy(buf, &length, 8);
		size_t	sz = sizeof(length);

		for (size_t i = 0; i < sz / 2; i++) {
			char	tmp = buf[i];
			buf[i] = buf[sz-i-1];
			buf[sz-i-1] = tmp;
		}

		size_t	off	= 0;
		for (size_t i = 0; i < sz; i++) {
			if (0 != (buf[i] & 0xFF)) {
				off = i;
				break;
			}
		}

		outs.write(reinterpret_cast<const char *>(buf+off), sz-off);
		if (!outs.good()) {
			return false;
		}
	}
	return outs.good();
}


bool
write_timestamp(std::ostream &outs, uint64_t t)
{
	write_tag(outs, TTimestamp);
	if (!outs.good()) {
		return false;
	}

	if (!write_length(outs, sizeof(t))) {
		return false;
	}

	char	buf[8];

	::memcpy(buf, &t, sizeof(t));
	size_t	sz = sizeof(t);

	for (size_t i = 0; i < sz / 2; i++) {
		char	tmp = buf[i];
		buf[i] = buf[sz-i-1];
		buf[sz-i-1] = tmp;
	}

	outs.write(reinterpret_cast<const char *>(buf), sz);
	return outs.good();
}


bool
write_loglevel(std::ostream &outs, std::uint8_t lvl)
{
	write_tag(outs, TLevel);
	if (!outs.good()) {
		return false;
	}

	if (!write_length(outs, sizeof(lvl))) {
		return false;
	}
	
	outs.write(reinterpret_cast<const char *>(&lvl), sizeof(lvl));
	return outs.good();
}


bool
write_string(std::ostream &outs, std::string s)
{
	size_t	length = s.size();

	write_tag(outs, TString);
	if (!outs.good()) {
		return false;
	}

	if (!write_length(outs, length)) {
		return false;
	}

	outs.write(reinterpret_cast<const char *>(s.c_str()), length);
	return outs.good();
}


bool
write_header(std::ostream& outs, std::uint8_t tag, std::uint64_t length)
{
	write_tag(outs, tag);
	if (!outs.good()) {
		return false;
	}

	return write_length(outs, length);
}


static inline size_t
string_record_length(std::string s)
{
	size_t	slen = s.size();
	size_t	length;

	length = sizeof(TString);
	length += length_octets(slen);
	length += slen;
	return length;
}


static inline size_t
log_length(std::string actor, std::string event,
	   std::map<std::string, std::string> attrs)
{
	// Timestamp record is 1 byte tag + 1 byte length + 8 byte
	// value (10 bytes total); level record is 1 byte tag +
	// 1 byte length + 1 byte value (3 bytes).
	size_t	length = 13;

	length += string_record_length(actor);
	length += string_record_length(event);

	for (auto it = attrs.begin(); it != attrs.end(); it++) {
		length += string_record_length(it->first);
		length += string_record_length(it->second);
	}

	return length;
}


bool
write_tlv_log(std::ostream& outs, std::uint8_t lvl,
	      std::string actor, std::string event,
	      std::map<std::string, std::string> attrs)
{
	std::time_t		t = std::time(nullptr);
	size_t			l = log_length(actor, event, attrs);

	std::cerr << "record length: " << l << std::endl;
	std::cerr << "writing header\n";
	if (!write_header(outs, TLogEntry, static_cast<std::uint64_t>(l))) {
		return false;
	}

	std::cerr << "writing timestamp\n";
	if (!write_timestamp(outs, std::uint64_t(t))) {
		return false;
	}
	else if (!write_loglevel(outs, lvl)) {
		return false;
	}
	else if (!write_string(outs, actor)) {
		return false;
	}
	else if (!write_string(outs, event)) {
		return false;
	}

	for (auto it = attrs.begin(); it != attrs.end(); it++) {
		if (!write_string(outs, it->first)) {
			return false;
		}
		else if (!write_string(outs, it->second)) {
			return false;
		}
	}

	return outs.good();
}


} // namespace tlv
} // namespace klog
