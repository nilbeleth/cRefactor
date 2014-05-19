#! /bin/bash


declare sources
declare -i diffs=0




function gather
{
    for file in *.orig
    do
        # extract correct names
        filename="${file##*/}"
        path="${file:0:${#file} - ${#filename}}"
        cpp="${filename%.[^.]*}"

        # copy new file
        cp -f $file $cpp

	sources="$sources $cpp"

    
    done
}

function testDiffs
{
    for file in $sources
    do
        diff -q $file $file.ref > /dev/null 2>&1
        if [ $? -eq 0 ]
	then
	    echo "Same"
	else
	    echo "Different"
	fi
    done
}

function cleanup
{
    rm -rf $sources
    exit $diffs
}

# copy all files to temp
gather

# run tester
#../tester -R -o calls -n called $sources
testDiffs
# test result
cleanup
