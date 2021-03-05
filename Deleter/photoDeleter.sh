#!/bin/bash

# Input controll

# Final path for move files
finPath=$1
shift

if [ $finPath -eq "--help" ]; then
        echo "Sorry, unsupported operation."
        exit 0
fi

if ! [ -d $finPath ]; then
        echo "$finPath is not directory"
        exit 1
fi

# Path for files movement
while [ "$#" -ne 0 ]
do
        case $1 in
        	--help)
        		echo "Sorry, unsupported operation."
                        exit 0;;
        	*)
        		echo "unknown parapeter"
                        exit 1;;
        esac
        shift
done

while [ 1 ]
do
        read promenna
        case $promenna in
                END|end)
                        echo "ended"
                        exit 0;;
                *)
                        #mv
                        echo "$promenna";;
        esac
done
