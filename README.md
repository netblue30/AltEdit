# AltEdit

AltEdit is a Qt5-based tiling programming text editor, featuring four editing windows, C/C++ syntax highlighting,
auto indentation, bracket matching, and spelling. The editor is integrated with ctags, grep, make and git diff utilities.

Download: http://github.com/netblue30/AltEdit

Dependencies: Qt5 library, qmake

Build and install: ./configure && make && sudo make install

Documentation and support: http://github.com/netblue30/AltEdit

License: GPL v3

## Setting up a compilation environment on Debian/Ubuntu
`````
$ sudo apt-get install build-essential qt5-default qt5-qmake qtbase5-dev-tools libqt5svg5 git
`````

## Compile and install
`````
$ git clone http://github.com/netblue30/AltEdit
$ cd alt
$ ./configure --prefix=/usr && make && sudo make install strip
`````

## Running the program:
`````
$ alt
`````
