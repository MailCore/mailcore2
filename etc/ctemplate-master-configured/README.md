Welcome to the C++ CTemplate system!
====================================

This library provides an easy to use and lightning fast text templating system
to use with C++ programs.

It was originally called Google Templates, due to its origin as the template
system used for Google search result pages. Now it has a more general name
matching its community-owned nature.

Documentation
-------------

Refer to the [Project
Documentation](<https://htmlpreview.github.io/?https://github.com/OlafvdSpek/ctemplate/blob/master/doc/index.html>)
to learn how to use the CTemplate system.

There is also a
[HOWTO](<https://htmlpreview.github.io/?https://raw.githubusercontent.com/OlafvdSpek/ctemplate/master/doc/howto.html>)
and
[Examples](<https://htmlpreview.github.io/?https://raw.githubusercontent.com/OlafvdSpek/ctemplate/master/doc/example.html>).

Compiling
---------

To compile test applications with these classes, run `./autogen.sh && ./configure` followed by
`make` on unixoid platforms like Linux or MacOSX.

To install these header files on your system, run `make install`.

See INSTALL for more details.

This code should work on any modern C++ system. It has been tested on Linux
(Ubuntu, Fedora, RedHat), Solaris 10 x86, FreeBSD 6.0, OS X 10.3 and 10.4, and
Windows under both VC++7 and VC++8.

There are a few Windows-specific details; see README.windows for more
information.

CTemplate and Threads
---------------------

The ctemplate library has thread support, so it works properly in a threaded
environment.

For this to work, if you link libraries with `-lctemplate` you may also need to
add `-pthread` (or, on some systems, `-pthreads`, and on others, `-lpthread`) to
get the library to compile. If you leave out the `-pthread`, you'll see errors
like this:

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
symbol lookup error: /usr/local/lib/libctemplate.so.0: undefined symbol: pthread_rwlock_init
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If your code isn't multi-threaded, you can instead use the ctemplate\_nothread
library: `-lctemplate_nothreads`

To summarize, there are two ways to link in ctemlpate in non-threaded
applications. For instance:

1.  Thread safe build:`gcc -o my_app my_app.o -lctemplate -pthread`

2.  Unthreaded code:`gcc -o my_app my_app.o -lctemplate_nothreads`

If your application uses threads, you should use form (1).


