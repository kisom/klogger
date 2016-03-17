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


#include <cstdio>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include <tlv.hh>
#include <klogger/console.hh>

using namespace std;


auto console	= klog::ConsoleLogger("tlv_test");

struct he_test {
	bool		 s;	// If true, use the string field.
	string		 e;	// Expected.
	string		 v;	// String value.
	const char	*vs;	// C string value.
	size_t		 vsl;	// C string length.
};

static struct he_test
build_he_test(string e, string v)
{
	struct he_test	t;

	t.s = true;
	t.e = e;
	t.v = v;
	return t;
}

static struct he_test
build_he_test_chars(string e, const char *s, size_t length)
{
	struct he_test	t;

	if (0 == length) {
		length = strnlen(s, 64);
	}

	t.s = false;
	t.e = e;
	t.vs = s;
	t.vsl = length;
	return t;
}


static bool
run_he_test(struct he_test& t)
{
	console.debug("test_hex_encode", "test case",
	    {{"expected", t.e}, {"string?", to_string(t.s)}});

	string	s;

	if (t.s) {
		s = klog::tlv::hex_encode(t.v);
	}
	else {
		s = klog::tlv::hex_encode(t.vs, t.vsl);
	}

	if (s != t.e) {
		console.error("test_hex_encode", "test failed",
		    {{"encoded", s}, {"expected", t.e}});
		return false;
	}

	return true;
}

vector<struct he_test> he_tests = {
	build_he_test("010203", "\x01\x02\x03"),
	build_he_test("68656C6C6F", "hello"),

	build_he_test_chars("000102", u8"\x00\x01\x02", 3),
	build_he_test_chars("68656C6C6F", u8"hello", 5),
};


int
test_hex_encode(void)
{
	int	fails = 0;

	for (auto t : he_tests) {
		if (!run_he_test(t)) {
			fails++;
		}
	}
	return fails == 0;
}


struct length_test {
	size_t		 length;
	const char	*expect;
};

vector<length_test> length_tests = {
	{32,			"\x20"},
	{172,   		"\x81\xac"},
	{1025,			"\x82\x04\x01"},
	{66051,			"\x83\x01\x02\x03"},
	{16909060,		"\x84\x01\x02\x03\x04"},
	{4328719365L,		"\x85\x01\x02\x03\x04\x05"},
	{1108152157446L,	"\x86\x01\x02\x03\x04\x05\x06"},
	{283686952306183L,	"\x87\x01\x02\x03\x04\x05\x06\x07"},
	{72623859790382856L,	"\x88\x01\x02\x03\x04\x05\x06\x07\x08"},
};


static bool
compare_strings(const char *expect, string value)
{
	string	expect_string(expect);

	return value == expect_string;
}


int
test_write_length(void)
{
	int	fails = 0;

	for (auto t : length_tests) {
		console.debug("test_write_length", "test case",
		    {{"length", to_string(t.length)}});

		stringstream	ss;

		if (!klog::tlv::write_length(ss, t.length)) {
			console.error("test_write_length",
			    "write_length failed",
			    {{"length", to_string(t.length)}});
			fails++;
			continue;
		}
		else if (!compare_strings(t.expect, ss.str())) {
			console.error("test_write_length",
			    "output failure",
			    {{"length", to_string(t.length)},
			     {"expected", klog::tlv::hex_encode(t.expect)},
			     {"actual", klog::tlv::hex_encode(ss.str())}});
			fails++;
			continue;
		}
	}
	return fails == 0;
}


int
test_write_timestamp(void)
{
	uint64_t	t = 1458304571;
	string		e = "02080000000056EBF63B";
	stringstream	ss;
	int		fails = 0;

	if (!klog::tlv::write_timestamp(ss, t)) {
		console.error("test_write_timestamp", "test fail",
		    {{"timestamp", to_string(t)}});
		fails++;
	}

	string	hv = klog::tlv::hex_encode(ss.str());
	if (hv != e) {
		console.error("test_write_timestamp", "test fail",
		    {{"timestamp", to_string(t)},
		     {"expected",  e},
		     {"actual",    hv}});
		fails++;
	}

	return fails == 0;
}


int
test_write_loglevel(void)
{
	uint8_t		lvl = 0x1;	// INFO
	string		e = "040101";
	stringstream	ss;
	int		fails = 0;

	if (!klog::tlv::write_loglevel(ss, lvl)) {
		console.error("test_write_loglevel", "test fail",
		    {{"level", to_string(lvl)}});
		fails++;
	}

	string	hv = klog::tlv::hex_encode(ss.str());
	if (hv != e) {
		console.error("test_write_loglevel", "test fail",
		    {{"level", to_string(lvl)},
		     {"expected", e},
		     {"actual", hv}});
		fails++;
	}

	return fails == 0;
}


struct string_test {
	string	s;
	string	e;
};


vector<string_test> string_tests = {
	{"hello world",		"080B68656C6C6F20776F726C64"},
	{"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
	 "0881806161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161616161"},
};


static bool
run_string_test(const struct string_test& t)
{
	stringstream	ss;

	if (!klog::tlv::write_string(ss, t.s)) {
		console.error("run_string_test", "test fail",
		    {{"string", t.s}});
		return false;
	}

	string	hv = klog::tlv::hex_encode(ss.str());
	if (hv != t.e) {
		console.error("run_string_test", "test fail",
		    {{"string", t.s},
		     {"expected", t.e},
		     {"actual", hv}});
		return false;
	}

	return true;
}


int
test_write_string(void)
{
	int	fails = 0;

	for (auto t : string_tests) {
		if (!run_string_test(t)) {
			fails++;
		}
	}
	return fails == 0;
}

static map<string, function<int(void)>> tests = {
	{"hex_encode", test_hex_encode},
	{"write_length", test_write_length},
	{"write_timestamp", test_write_timestamp},
	{"write_loglevel", test_write_loglevel},
	{"write_string", test_write_string},
};


int
main(void)
{
	// console.level(klog::Level::DEBUG);
	for (auto it = tests.begin(); it != tests.end(); it++) {
		console.info("tlv_test", "test run", {{"test", 	it->first}});
		if (-1 == tests[it->first]()) {
			console.fatal("tlv_test", "test fail",
			    {{"test", it->first}});
		}
	}
}
