# klogger/syslog.hh syslog.cc	\	# syslogger
# klogger/filelog.hh filelog.cc		# logging to a file
AM_CPPFLAGS  =	-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align
AM_CPPFLAGS +=	-Wwrite-strings -Wmissing-declarations -Wno-long-long -Werror
AM_CPPFLAGS +=	-Wunused-variable -std=c++11 -D_XOPEN_SOURCE -Os -I.
AM_CPPFLAGS +=	-fno-elide-constructors	 -Weffc++

## Source file sets.
# Common logging interface and internal utility functions.
LOGGER_CORE =	klogger/logger.hh  logger.cc

# ConsoleLogger implementation.
CONSOLE_CC =	klogger/console.hh console.cc

# Syslogger implementation.
SYSLOG_CC =	klogger/syslog.hh  syslog.cc

# Superset of the source file sets.
LOGGER_CC =	$(LOGGER_CORE) $(CONSOLE_CC) $(SYSLOG_CC)
		

lib_LIBRARIES =			libklogger.a
nobase_include_HEADERS =	klogger/logger.hh klogger/console.hh
noinst_HEADERS =		internal.hh

libklogger_a_SOURCES =		$(LOGGER_CC)

noinst_PROGRAMS =		console_test syslog_test
console_test_SOURCES =		$(LOGGER_CC) console_test.cc
syslog_test_SOURCES =		$(LOGGER_CC) syslog_test.cc


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

