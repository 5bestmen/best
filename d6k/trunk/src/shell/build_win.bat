@echo off

echo **************************************************
echo pro to vcxproj begin...
echo **************************************************

rem echo generate vcproj.....
call protovcxproj.bat

echo **************************************************
echo pro to vcxproj finished!
echo **************************************************


echo **************************************************
echo building project...
echo **************************************************

if "%1" == "debug" goto DEBUG
if "%1" == "" goto RELEASE 
if "%1" == "release" goto RELEASE
if "%1" == "all" goto RELEASE


echo Usage: build_win.bat release/debug/all/clean/""
echo "      release|"" : Make version release          "
echo "      debug      : Make version debug            "
echo "      all        : Make version debug and release"
goto END


:RELEASE
MSbuild /t:rebuild /p:Configuration=Release shell_c.vcxproj
if "%1" == "all" goto DEBUG
goto END


:DEBUG
echo  --nmake projects
MSbuild /t:rebuild /p:Configuration=Debug shell_c.vcxproj
goto END


:END
del *.pdb
del *_c.vcxproj
del *_c.vcxproj.filters


echo **************************************************
echo  build finished!
echo **************************************************

rem pause


