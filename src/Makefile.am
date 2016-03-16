# klogger/syslog.hh syslog.cc	\	# syslogger
# klogger/filelog.hh filelog.cc		# logging to a file
AM_CPPFLAGS  =	-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align
AM_CPPFLAGS +=	-Wwrite-strings -Wmissing-declarations -Wno-long-long -Werror
AM_CPPFLAGS +=	-Wunused-variable -std=c++11 -D_XOPEN_SOURCE -Os -I.
AM_CPPFLAGS +=	-fno-elide-constructors	 -Weffc++


LOGGER_CC =	klogger/logger.hh logger.cc	\
		klogger/console.hh console.cc

lib_LIBRARIES =			libklogger.a
nobase_include_HEADERS =	klogger/logger.hh klogger/console.hh
noinst_HEADERS =		internal.hh

libklogger_a_SOURCES =		$(LOGGER_CC)

noinst_PROGRAMS =		console_test
console_test_SOURCES =		$(LOGGER_CC) console_test.cc

.PHONY: scanners clang-scanner cppcheck-scanner
scanners: clang-scanner cppcheck-scanner

clang-scanner:
	clang++ $(AM_CPPFLAGS) --analyze $(LOGGER_CC)

cppcheck-scanner:
	cppcheck --quiet --enable=all -I ./ $(LOGGER_CC)

.PHONY: coverity-scan
	coverity-scan: clean
	cov-build --dir cov-int make all check
	tar czf $(PACKAGE_NAME)-$(PACKAGE_VERSION)_coverity.tar.gz cov-int
	rm -rf cov-int
