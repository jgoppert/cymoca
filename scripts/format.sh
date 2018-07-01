#!/bin/bash
if [ $# != 1 ]
then
	echo """
	usage: $0 apply

		apply, if 1 then applies formatting, otherwise shows diff
	"""
	exit 1
fi
apply=$1

files=`find . -regex "./\(src\|test\).*\.\(cpp\|h\|c\|hpp\)"`

if [ $apply == 1 ]
then
	clang-format-6.0 -i $files
else
	clang-format-6.0 -output-replacements-xml $files | grep -c "<replacement " >/dev/null
	if [ $? -ne 1 ]; then 
    	echo "Commit did not match clang-format"
		git-clang-format-6.0 --diff
    	exit 1
	fi
fi
