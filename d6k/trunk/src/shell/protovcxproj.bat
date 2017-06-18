@echo off

echo Setting up project...

echo -- set QMAKESPEC=win32-msvc2010
set QMAKESPEC=win32-msvc2010

echo qmake vcxproj......
qmake -tp vc -o shell_c.vcxproj shell.pro

echo qmake for vcxproj finished!





