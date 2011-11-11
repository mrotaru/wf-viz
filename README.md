CIA World Factbook visualizer
============================
This project aims to make it easy to browse and understand the wealth of
statistics provided by the CIA World Factbook.

To build
--------
There are three supported ways of building from the source code: from the
Visual Studio project, by using waf, or make; all of which are explained
in detail below. But bear in mind that all the build methods are supported
only on the `master` branch; on the `develop` or topic branches building
is not guaranteed to work with all methods.

To build by using the waf script just enter inside the folder continaing
the wscript file ( the root of the project ), and type `python waf configure`,
and then `python waf build`; this should work on both Windows and Linux.
Built files ( including the executable ) will be placed inside a folder
named waf-build-[win32|linux], depending on the platform.

The makefile story is very similar; change into the root directory, and
simply type `make`; like the waf method, should work on both Windows and
Linux. The build files will be placed in a folder named make-build-[win32|linux].

Not much to say about the third option; just open the VS solution and select
Build -> Build Solution - or simply press F7, like a boss.
