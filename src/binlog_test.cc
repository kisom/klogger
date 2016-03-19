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


#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <getopt.h>

#include <klogger/binlog.hh>


int
main(int argc, char *argv[])
{
	char			**pargv;
	int			  pargc;
	klog::BinLogger		 *flog = nullptr;
	std::string		  name = std::string(argv[0]);
	int			  nargs;
	int			  opt;

	while (-1 != (opt = ::getopt(argc, argv, "r"))) {
		switch (opt) {
		case 'r':
			std::cerr << "Not implemented.\n";
			exit(1);
		default:
			::abort();
		}
	}

	pargc = argc - optind;
	pargv = argv + optind;

	if (pargc > 1) {
		flog = klog::open_binlogfile(std::string(pargv[0]),
		    std::string(pargv[1]), true);
		nargs = 3;
	}
	else if (pargc > 0) {
		flog = klog::open_binlogfile(std::string(pargv[0]), true);
		nargs = 2;
	}
	else {
		std::cerr << "Usage: " << argv[0] << " logfile [errfile]\n";
		exit(EXIT_FAILURE);
	}

	std::map<std::string, std::string>	args;

	if (!flog->good()) {
		::abort();
	}

	flog->debug("main", "starts");

	for (int i = nargs; i < argc; i++) {
		auto	k = "argv[" + std::to_string(i) + "]";
		args[k] = std::string(argv[i]);
	}

	flog->info("main", "starts", args);
	flog->warn("main", "depleted");
	flog->fatal("main", "ends");
}
