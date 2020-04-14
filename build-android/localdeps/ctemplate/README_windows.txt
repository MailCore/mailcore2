This project has been ported to Windows.  A working solution file
exists in this directory:
    ctemplate.sln

You can load this solution file into either VC++ 7.1 (Visual Studio
2003) or VC++ 8.0 (Visual Studio 2005) -- in the latter case, it will
automatically convert the files to the latest format for you.

When you build the solution, it will create libctemplate.dll, the main
library for this project, plus a number of unittests, which you can
run by hand (or, more easily, under the Visual Studio debugger) to
make sure everything is working properly on your system.  The binaries
will end up in a directory called "debug" or "release" in the
top-level directory (next to the .sln file).

I don't know very much about how to install DLLs on Windows, so you'll
have to figure out that part for yourself.  If you choose to just
re-use the existing .sln, make sure you set the IncludeDir's
appropriately!  Look at the properties for libctemplate.dll.

If you wish to link to ctemplate statically instead of using the .dll,
you can; see the example project template_unittest_static.  For this
to work, you'll need to add "/D CTEMPLATE_DLL_DECL=" to the compile
line of every ctemplate .cc file.

Note that these systems are set to build in Debug mode by default.
You may want to change them to Release mode.

Currently, Template::StringToTemplate returns a Template object that
you, the caller, must free.  We've heard reports that Windows can have
trouble allocating memory in a .dll that is meant to be freed in the
application.  Thus, we suggest you not use StringToTemplate from
Windows.  Instead, you can use Template::StringToTemplateCache()
followed by Template::GetTemplate().

I have little experience with Windows programming, so there may be
better ways to set this up than I've done!  If you run across any
problems, please post to the google-ctemplate Google Group, or report
them on the ctemplate Google Code site:
   http://groups.google.com/group/google-ctemplate
   http://code.google.com/p/ctemplate/issues/list

-- craig
