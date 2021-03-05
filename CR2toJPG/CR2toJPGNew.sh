#!/bin/bash

SOURCE=" "
DESTINATION=" "

# Check of Input
# input must be on or two paths to files
# if is set only one file, will be created file jpg in source file
if [ "$#" -ne 1 ]
then
	if [ "$#" -ne 2 ]
	then
		echo "Invalid count of arguments."
		exit 1
	else
		SOURCE=`readlink -f "$1"`
		DESTINATION=`readlink -f "$2"`

		if ! [ -d "$SOURCE" ]
        	then
                	echo "Source must be directory."
                	exit 1
        	fi

		if ! [ -d "$DESTINATION" ]
        	then
                	echo "Destination must be directory."
                	exit 1
        	fi
	fi
else
	SOURCE=`readlink -f "$1"`
	if ! [ -d "$SOURCE" ]
	then
		echo "Source must be directory."
		exit 1
	fi
	DESTINATION="${SOURCE}/jpg"
	mkdir "${DESTINATION}" 2> /dev/null
fi

echo "Source folder:      ${SOURCE}"
echo "Destination folder: ${DESTINATION}"

cd "$SOURCE"

COUNT=`ls *.CR2 | wc -l`
ACT=0
BAR_LENGTH=50

echo ""
echo "Number of files: ${COUNT}"
echo ""
echo "Progress:"

echo -ne "|"
printf ' %.0s' `seq ${BAR_LENGTH}`
echo -ne "| (0%)\r"

for i in *.CR2
do

	# convert photo and move to destination folder
	OUT="${i::-4}.jpg"
	rm -f ${OUT}
	ufraw-batch $i --out-type=jpeg --output "${OUT}" 2> /dev/null
	mv "${SOURCE}/${OUT}" "${DESTINATION}"

	ACT=`expr $ACT + 100`
	PERC=`expr $ACT / $COUNT`

	FILLED=`expr ${BAR_LENGTH} \* ${PERC}`
	FILLED=`expr ${FILLED} \/ 100`
	SPACE=`expr ${BAR_LENGTH} \- ${FILLED}`

	echo -ne "|"
	printf '#%.0s' `seq ${FILLED}`
	printf ' %.0s' `seq ${SPACE}`
	echo -ne "| (${PERC}%)               \r"
done

echo -ne "\n"
