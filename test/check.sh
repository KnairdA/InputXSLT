#!/bin/sh

for testcase in *;
do
	if [ -d $testcase ] &&  [ "$testcase" != "common" ]
	then
		echo $testcase

		cd $testcase
		rm -f actual.xml 

		./../../build/ixslt --transformation transformation.xsl --target actual.xml --include ../common/
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
