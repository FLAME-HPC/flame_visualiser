FLAME Visualiser
================
Please note that FLAME Visualiser has been migrated from an svn repository. It is possible, therefore, that some of the branches might not have all the expected shared history with their parent branch etc.

LINUX
-----
To compile you will need Qt4.
The package for Ubuntu is called: libqt4-dev
You will also need a C++ compiler.
The package for a build environment is: build-essential
You will also need OpenGL dev packages:
libgl1-mesa-dev
libglu1-mesa-dev

To compile type:

	qmake flame_visualiser.pro
	make

To run (requires sudo or options will be greyed out):

	sudo ./FLAME\ Visualiser

MAC OSX
-------
To compile you will need Qt4.
Download from the Qt web site.
Normally qmake creates xcode projects on mac.
To stop this use the -spec flag below.
You will also need a C++ compiler. Install Xcode either from
provided disc or from the Mac App Store.

To compile type:

	qmake -spec macx-g++ flame_visualiser.pro
	make

To run:

	open FLAME\ Visualiser.app

If you have any problems try using Qt Creator.

WINDOWS
-------
To compile you will need Qt4.
Download from the Qt web site.
If you don't have MinGW already installed select the version of Qt Creator
that includes MinGW (a C++ compiler for Windows).
Use Qt Creator to open the project flame_visualiser.pro and compile.
