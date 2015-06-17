OpenIndy-Core
=============

OpenIndy-Core is the kernel of [OpenIndy](https://github.com/OpenIndy/OpenIndy). It contains all classes that represent the structure behind OpenIndy.
OpenIndy-Core is included as a shared library into [OpenIndy](https://github.com/OpenIndy/OpenIndy) as well as all plugins developed for [OpenIndy](https://github.com/OpenIndy/OpenIndy).

IDE
----

OpenIndy-Core is developed with the Qt framework (Qt libs + Qt Creator IDE). You can download the framework [here](http://qt-project.org/downloads).

Dependencies
------------

- [OpenIndy-Math](https://github.com/OpenIndy/OpenIndy-Math)
- [Qt](http://qt-project.org)

Build
-----

The easiest way to build OpenIndy-Core is to use the Qt Creator. You can build OpenIndy-Core in `debug` or `release` mode.
First you need to compile the [OpenIndy-Math](https://github.com/OpenIndy/OpenIndy-Math) library.
Afterwards you can compile OpenIndy-Core using the same compiler as for the [OpenIndy-Math](https://github.com/OpenIndy/OpenIndy-Math) project.