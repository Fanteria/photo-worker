#!/bin/bash

# Set default options
name="photo"
sep="_"
start=1
end=1000000
startingFile=1
suf="CR2"
path="./"
zeros=4

# Input controll
while [ "$#" -ne 0 ]
do
        case $1 in
        	--help)
        		echo "Sorry, unsupported operation."
                        exit 0;;
                -n|-N)
                        shift; name=$1;;
                -f|-F)
                        shift; start=$1;;
                -l|-L)
                        shift; end=$1;;
                -p|-P)
                        shift; path=$1;;
                -s|-S)
                        shift; startingFile=$1;;
                -z|-Z)
                        shift; zeros=$1;;
                --separator)
                        shift; sep=$1;;
                --suffix)
                        shift; suf=$1;;
        	*)
        		echo "unknown parapeter"
                        exit 1;;
        esac
        shift
done

# Files rename
act=$startingFile

for i in `ls ${path}*.${suf}`
do
        string=$i
        string=${string%$suf}
        num=$(echo $string | egrep -o [0-9]+)
        num=$(echo $num | tr "\n" ".")
        num="${num##* }"
        num=${num%"."}
        [ $num ]\
        && [ $start -le $num ]\
        && [ $end -ge $num ]\
        && var=$(printf "%0${zeros}d\n" $act)\
        && act=`expr $act + 1`\
        && mv "$i" "${path}${name}${sep}${var}.${suf}"
done
