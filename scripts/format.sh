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

files=`find . -regex "./\(src\|test\).*\.\(cpp\|h\|c\|cc\|hpp\)"`

clang-format-6.0 -output-replacements-xml $files | grep -c "<replacement " >/dev/null
if [ $? -ne 1 ]; then 
    git clang-format-6.0 --diff HEAD^ $files
	if [ $apply == 1 ]
	then
		clang-format-6.0 -i $files
		echo "formatting complete"
		exit 0
	else
		echo "formatting required"
		exit 1
	fi
else
	echo "no files need to be formatted"
fi
exit 0
