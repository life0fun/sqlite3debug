# How to step through sqlite3 in a debugger

we try to create a local dev environment on my OSX Sierra so I can debug sqlite3.
Tried with Eclipse Neo on my OSX Sierra with the following setup, no luck.

https://www.ics.uci.edu/~pattis/common/handouts/macmingweclipse/allexperimental/mac-gdb-install.html

Finally, made it work with xcode.
folder sqlitetest containers the xcode project for this. download latest xcode and import the project, you can setup breakpoints and step through.

## database
we use chinook.db from sqlite tutorial.

the current working dir in mac is somewhere under $HOME/Library/Developer/Xcode/DerivedData/sqlitetest-fuhccdahfchkkphkkellpeacfgxk/Build/Products/Debug.
It is not at the project home. wierd.
