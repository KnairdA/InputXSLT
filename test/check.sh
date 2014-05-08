#!/bin/sh

for testcase in *;
do
	if [ -d $testcase ] &&  [ "$testcase" != "common" ]
	then
		echo $testcase

		cd $testcase
		rm -f actual.xml 

		./../../build/test --transformation transformation.xsl --target actual.xml
		diff -u reference.xml actual.xml

		if [ $? = 0 ]
		then
			echo "passed."
		else
			echo "failed."
		fi

		cd ..
	fi
done