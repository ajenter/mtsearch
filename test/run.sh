#!/bin/sh

for case in `find . -maxdepth 1 -mindepth 1 -type d -name "case*"`
do
    mask=`cat $case/mask`
    mask="${mask%
}"                # Remove a trailing newline.
    `$1 $case/in "$mask" > $case.out`

    passed=0
    for possible_out in `find $case -maxdepth 1 -mindepth 1 -type f -name "*.out"`
    do
        if cmp -s "$possible_out" "$case.out" ; then
            passed=1
            break
        fi
    done

    if [ $passed -eq 1 ] ; then
        echo "$case [OK]"
    else
        echo "$case [FAILED]"
    fi

done
