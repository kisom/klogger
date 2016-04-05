klogger
=======

.. image:: https://scan.coverity.com/projects/8282/badge.svg
   :target: https://scan.coverity.com/projects/kisom-klogger

This is a lightweight MIT-licensed attribute-based logging static library
written in C++11.

The motivations and basic design are covered in the functional spec
(``doc/funspec.rst``). Using the library is covered in the user manual
(``doc/manual.rst``).


Getting the source, building, and installation
----------------------------------------------

The source may be obtained via Git::

  git clone https://git.kyleisom.net/lib/klogger
  git clone https://github.com/kisom/klogger

If cloned from Git, the package may be built using the ``autobuild.sh``
script supplied in the root directory. The standard ``./configure &&
make && sudo make install`` dance can be used as well, once the ``configure``
script has been generated with ``autoreconf -i``.


Overview
--------

Log messages consist of four components:

1. The **level** attaches a notion of priority to the log message.

2. The **actor** is used to specify which component is generating
   the log message. This could be the program name, or it could be
   a specific component inside the system.

3. The **event** is a short message indicating what happened. This is
   most like the traditional log message.

4. The **attributes** are an optional set of key-value string pairs that
   provide additional information.

Additionally, each log message has an associated timestamp. For the
text-based logs, this is "%FT%T%z"; for the binary logs, this is a
64-bit Unix timestamp. An example text-based timestamp might look like ::

  [2016-03-27T20:59:27-0700] [INFO] [actor:server event:request received] client=192.168.2.5 request-size=839

Note that this is organised in a manner that facilitates parsing::

  /\[(\d{4}-\d{3}-\d{2}T\d{2}:\d{2}:\d{2}[+-]\d{4})\] \[(\w+\)]\) \[actor:(.+?) event:(.+?)\]/

will cover the header:

+ ``$1`` contains the timestamp
+ ``$2`` contains the level
+ ``$3`` contains the actor
+ ``$4`` contains the event

The library revolves around the ``Logger`` virtual class. There are
several implementations:

* ``ConsoleLogger``: writes DEBUG and INFO messages to standard output,
   and other messages to standard error. The constructor takes no
   arguments. See ``src/console_test.cc``
* ``FileLogger``: writes DEBUG and INFO messages to a log file, and
  other messages to an error log file. There are two constructors:
  the first takes a single filename as an argument; the error log and
  the log file are the same file. Alternatively, separate log files
  can be specified. See ``src/file_test.cc``.
* ``Syslogger``: writes logs to syslog. See ``src/syslog_test.cc``.
* ``BinLogger``: operates similarly to ``FileLogger`` except that
   logs are written in a binary format.

The user's manual is in ``doc/manual.rst``.


Example
-------

This is the ``src/console_test.cc`` program::

  #include <cstdlib>
  #include <map>
  #include <string>
   
  #include <klogger/console.hh>
   
   
  int
  main(int argc, char *argv[])
  {
          std::string                             name = std::string(argv[0]);
          klog::ConsoleLogger                     console(name);
          std::map<std::string, std::string>      args;
   
          if (!console.good()) {
                  ::abort();
          }
   
          console.debug("main", "starts");
   
          for (int i = 1; i < argc; i++) {
                  auto        k = "argv[" + std::to_string(i) + "]";
                  args[k] = std::string(argv[i]);
          }
   
          console.info("main", "starts", args);
          console.warn("main", "depleted", {{"args", std::to_string(argc)}});
          console.fatal("main", "ends");
  }


License
-------

Copyright (c) 2016 Kyle Isom <coder@kyleisom.net>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

