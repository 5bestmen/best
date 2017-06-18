#!/bin/bash

function DEBUG
{
	echo  --make debug  projects
	qmake   *.pro -r -spec linux-g++-64 CONFIG+=debug 
	make
	rm -f  debug/*.cpp debug/*.o
}

function RELEASE
{
	echo  --make release projects
	rm -f  release/*.cpp release/*.o
	./qmake.sh   *.pro -r -spec linux-g++-64 CONFIG+=release 
	make	
}

function ALL
{
	DEBUG
	RELEASE
}

function CLEAN
{
	make clean
	rm -f debug/*.cpp debug/*.o
	rm -f release/*.cpp release/*.o
}



	echo Usage: build_comlile.sh release/debug/all/clean/""
	echo "      release|"" : Make version release          "
	echo "      debug      : Make version debug            "
	echo "      all        : Make version debug and release"
	echo "      clean      : Make clean                    "

	echo building project...

	if [ "$1" == "debug" ] 
		then   DEBUG
	fi

	if [ "$1" == ""  ]
		then  RELEASE
	fi

	if [ "$1" == "release" ]
		then  RELEASE
	fi

	if [ "$1" ==  "all" ]
		then   ALL
	fi

	if [ "$1" == "clean" ]
		then  CLEAN
	fi

	echo  --build finished!!!





