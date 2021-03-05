#!/bin/bash

if [ "$#" -ne 2 ]
then
        echo "Zadejte zdrojovou a cílovou složku."
        exit 1
fi

if ! [ \( -d $1 \) -a \( -d $2 \) ]
then
        echo "$1"
        echo "$2"
        echo "Zdroj i cíl musí být složky."
        exit 1
fi

for i in "`readlink -f "$1/*.CR2"`"
do
        OUT=${i::-4}
        echo $i
	#ufraw-batch $i --out-type=jpeg --output "$OUT.jpg"
done

mv "`readlink -f "$1/*.jpg"`" "`readlink -f "$2/"`"

#for i in *.CR2; do ufraw-batch $i --out-type=jpeg --output $i.jpg; done;
